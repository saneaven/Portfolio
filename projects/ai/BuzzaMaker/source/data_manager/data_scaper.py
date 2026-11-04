import pandas as pd
import yfinance as yf
import requests
import time
import os
import csv
import threading
import queue
import warnings
from datetime import datetime, timedelta
from newspaper import Article, Config

# Ignore warnings
warnings.filterwarnings("ignore", category=SyntaxWarning)

# ==========================================
# 1. Configuration
# ==========================================
INPUT_CSV_FILE = 'company_list.csv'
BASE_OUTPUT_DIR = 'data_news_pipeline'
STOCK_OUTPUT_DIR = 'data_stocks'

# Folder paths
STEP1_DIR = os.path.join(BASE_OUTPUT_DIR, 'step1_urls')
STEP2_DIR = os.path.join(BASE_OUTPUT_DIR, 'step2_content')

GDELT_START_DATE = datetime(2017, 1, 1)
DAYS_PER_STEP = 7 

NUM_SCRAPER_THREADS = 20  # Number of scraper threads

HEADERS = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36'
}

os.makedirs(BASE_OUTPUT_DIR, exist_ok=True)
os.makedirs(STEP1_DIR, exist_ok=True)
os.makedirs(STEP2_DIR, exist_ok=True)
os.makedirs(STOCK_OUTPUT_DIR, exist_ok=True)

# Global variables
task_queue = queue.Queue()
csv_write_lock = threading.Lock()
production_done = False 
counter_found = 0   
counter_scraped = 0 

# ==========================================
# 2. GDELT API Request
# ==========================================
def fetch_gdelt_range(keyword, start_dt, end_dt):
    s_str = start_dt.strftime('%Y%m%d000000')
    e_str = end_dt.strftime('%Y%m%d235959')
    
    base_url = "https://api.gdeltproject.org/api/v2/doc/doc"
    params = {
        'query': f'"{keyword}" sourcelang:eng',
        'mode': 'artlist',
        'maxrecords': '250',
        'startdatetime': s_str,
        'enddatetime': e_str,
        'format': 'json'
    }
    
    try:
        response = requests.get(base_url, params=params, headers=HEADERS, timeout=10)
        if response.status_code != 200 or not response.text.strip(): return []
        data = response.json()
        return data.get('articles', [])
    except: return []

# ==========================================
# 3. Producer: Local Recovery + URL Collection
# ==========================================
def producer_mixed_fetcher(ticker, name, url_csv_writer, resume_date, existing_links_step2, step1_file_path):
    global production_done, counter_found
    
    # -----------------------------------------------------
    # [Phase 1] Local File Recovery (Gap Filling)
    # Queue items present in Step 1 file but missing in Step 2
    # -----------------------------------------------------
    if os.path.exists(step1_file_path):
        try:
            print(f"  [Producer] Checking local Step 1 file for unsaved articles...")
            df_step1 = pd.read_csv(step1_file_path)
            
            # Filter links not in Step 2
            # (Use existing_links_step2 set)
            df_step1_unique = df_step1[~df_step1['Link'].isin(existing_links_step2)]
            
            gap_count = len(df_step1_unique)
            
            if gap_count > 0:
                print(f"  [Producer] Found {gap_count} links in Step 1 NOT in Step 2. Queueing them first...")
                
                for _, row in df_step1_unique.iterrows():
                    item = {
                        'Date': str(row['Date']),
                        'Title': str(row['Title']),
                        'Media': str(row['Media']),
                        'Link': str(row['Link'])
                    }
                    task_queue.put(item)
                    # Add for deduplication
                    existing_links_step2.add(item['Link'])
                    
                counter_found += gap_count
            else:
                print(f"  [Producer] Local files are in sync. No gap found.")
                
        except Exception as e:
            print(f"  [Error] Failed to read local Step 1 file: {e}")

    # -----------------------------------------------------
    # [Phase 2] GDELT API Request (New Data)
    # Start search from resume_date
    # -----------------------------------------------------
    current_date = resume_date
    end_date = datetime.now()
    
    # If resume_date is too close to today, skip API request
    if current_date >= end_date - timedelta(days=1):
        print(f"  [Producer] Step 1 seems up to date. Skipping API fetch.")
        production_done = True
        return

    print(f"  [Producer] Starting GDELT API search from {current_date.date()}...")
    
    while current_date <= end_date:
        current_end = current_date + timedelta(days=DAYS_PER_STEP)
        if current_end > end_date: current_end = end_date
        
        articles = fetch_gdelt_range(name, current_date, current_end)
        
        if articles:
            count = 0
            for item in articles:
                link = item.get('url', '')
                
                # Skip if already in Step 2 (or just queued)
                if link in existing_links_step2:
                    continue

                raw_date = item.get('seendate', '')
                fmt_date = raw_date
                try:
                    fmt_date = datetime.strptime(raw_date, '%Y%m%dT%H%M%SZ').strftime('%Y-%m-%d')
                except: pass
                
                row = {
                    'Date': fmt_date,
                    'Title': item.get('title', ''),
                    'Media': item.get('domain', ''),
                    'Link': link
                }
                
                # Backup save to Step 1 file
                url_csv_writer.writerow([row['Date'], row['Title'], row['Media'], row['Link']])
                
                # Add to task queue
                task_queue.put(row)
                existing_links_step2.add(link)
                count += 1
                
            counter_found += count
                
        current_date = current_end
        time.sleep(0.1) 
        
    production_done = True 

# ==========================================
# 4. Consumer: Content Scraping
# ==========================================
def consumer_scraper(result_csv_writer):
    global counter_scraped
    config = Config()
    config.browser_user_agent = 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) ...'
    config.request_timeout = 10
    
    while True:
        try:
            row = task_queue.get(timeout=3)
        except queue.Empty:
            if production_done: break
            else: continue
        
        url = row['Link']
        content = ""
        try:
            article = Article(url, config=config)
            article.download()
            article.parse()
            content = article.text.strip()
        except: content = ""
            
        with csv_write_lock:
            result_csv_writer.writerow([
                row['Date'], row['Title'], row['Media'], row['Link'], content
            ])
        
        counter_scraped += 1
        task_queue.task_done()

# ==========================================
# 5. Load Existing Progress Function
# ==========================================
def load_existing_progress(file_step2):
    """
    Reads Step 2 file (final result) to return:
    1. Set of already completed URLs
    2. Last saved date (Resume Date)
    """
    if not os.path.exists(file_step2):
        return set(), GDELT_START_DATE, False 
    
    try:
        df = pd.read_csv(file_step2)
        if df.empty:
            return set(), GDELT_START_DATE, True 
        
        # 1. Completed links
        existing_links = set(df['Link'].dropna().unique())
        
        # 2. Last date (in case API needs to run more)
        df['Date'] = pd.to_datetime(df['Date'], errors='coerce')
        last_date = df['Date'].max()
        
        if pd.isnull(last_date):
            resume_date = GDELT_START_DATE
        else:
            resume_date = last_date - timedelta(days=7) # Safely start from 1 week ago
            if resume_date < GDELT_START_DATE:
                resume_date = GDELT_START_DATE
                
        return existing_links, resume_date, True
        
    except Exception as e:
        print(f"  [Warning] File read error: {e}. Resetting.")
        return set(), GDELT_START_DATE, False

# ==========================================
# 6. Main Execution
# ==========================================
def main():
    global production_done, counter_found, counter_scraped
    
    if not os.path.exists(INPUT_CSV_FILE):
        print(f"Error: {INPUT_CSV_FILE} not found")
        return

    company_df = pd.read_csv(INPUT_CSV_FILE)

    for idx, row in company_df.iterrows():
        ticker = row['Ticker']
        name = row['Name']
        
        print(f"\n{'='*60}")
        print(f"Processing: {name} ({ticker})")
        print(f"{'='*60}")
        
        # File paths
        file_step1 = os.path.join(STEP1_DIR, f"{ticker}_urls.csv")
        file_step2 = os.path.join(STEP2_DIR, f"{ticker}_content.csv")

        # --- [Resume Analysis] ---
        existing_links_step2, resume_date, file_exists_step2 = load_existing_progress(file_step2)
        
        file_mode = 'a' if file_exists_step2 else 'w'
        
        # Step 1 file is used only when API is called, but used as read-only here
        # Will be opened in append mode inside Producer
        
        # Open Step 2 file (for Consumer)
        f2 = open(file_step2, file_mode, newline='', encoding='utf-8-sig')
        writer2 = csv.writer(f2)
        if not file_exists_step2:
            writer2.writerow(['Date', 'Title', 'Media', 'Link', 'FullText'])
            
        # Open Step 1 file (for Producer - New URL backup)
        # Create Step 1 file if not exists
        file_mode_step1 = 'a' if os.path.exists(file_step1) else 'w'
        f1 = open(file_step1, file_mode_step1, newline='', encoding='utf-8-sig')
        writer1 = csv.writer(f1)
        if file_mode_step1 == 'w':
            writer1.writerow(['Date', 'Title', 'Media', 'Link'])
        
        # Initialization
        production_done = False
        counter_found = 0
        counter_scraped = 0
        
        # Start Consumer (Scraper) threads
        consumers = []
        for i in range(NUM_SCRAPER_THREADS):
            t = threading.Thread(target=consumer_scraper, args=(writer2,))
            t.daemon = True
            t.start()
            consumers.append(t)
        
        # Start Producer (URL Recovery + API) thread
        producer = threading.Thread(
            target=producer_mixed_fetcher, 
            args=(ticker, name, writer1, resume_date, existing_links_step2, file_step1)
        )
        producer.start()
        
        # Monitoring
        print("  [Progress] Pipeline Running...")
        try:
            while True:
                q_size = task_queue.qsize()
                # found: To process (Gap + New) / scraped: Completed count
                print(f"\r  >> Processing Queue: {q_size} | Scraped(New): {counter_scraped}    ", end="")
                
                if production_done and task_queue.empty():
                    break
                time.sleep(0.5)
        except KeyboardInterrupt:
            print("\n  [Stop] User interrupted.")
            production_done = True
            
        producer.join()
        for t in consumers: t.join()
            
        f1.close()
        f2.close()
        
        print(f"\n  [Done] {name} Finished.")
        time.sleep(2)

if __name__ == "__main__":
    main()