import requests
import pandas as pd
from datetime import datetime, timedelta
import time
import os
from requests.adapters import HTTPAdapter
from urllib3.util.retry import Retry
import json
from concurrent.futures import ThreadPoolExecutor, as_completed

# --- 1. Settings and Keywords ---
global_keywords = {
    "Politics_and_Diplomacy": [
        "Multipolarity", "Global South", "Hegemony", "Decoupling", "De-risking",
        "Protectionism", "Nationalism", "Trade War", "Sanctions", "Soft Power",
        "NATO Expansion", "Indo-Pacific Strategy", "Belt and Road Initiative",
        "Strategic Ambiguity", "Sovereignty", "Unilateralism", "Multilateralism",
        "Geopolitical Risk", "Proxy War", "Diplomatic Ties",
        "Autocracy", "Populism", "Kleptocracy", "Technocracy", "Coalition",
        "Partisanship", "Bipartisan", "Gridlock", "Gerrymandering", "Filibuster",
        "Impeachment", "Referendum", "Veto", "Lobbying", "Bureaucracy",
        "Regime Change", "Coup d etat", "Dissident", "Swing State", "Grassroots",
        "Sovereign Debt", "Austerity", "Tax Haven", "Money Laundering", "Corruption",
        "Cartel", "Insurgency", "Martial Law", "Curfew", "Extradition",
        "Asylum", "Amnesty", "Genocide", "Ethnic Cleansing", "Human Trafficking",
        "Humanitarian Aid", "NGO", "Geneva Convention", "Collateral Damage", "Status Quo"
    ],
    "Economy_and_Market": [
        "Inflation", "Stagflation", "Interest Rate Hike", "Interest Rate Cut", "Recession",
        "Supply Chain Resilience", "Global Value Chain", "Fiscal Policy",
        "Monetary Policy", "Quantitative Easing", "Tightening",
        "Emerging Markets", "Trade Deficit", "Trade Surplus", "Cryptocurrency",
        "CBDC", "Income Inequality", "Gig Economy", "Reshoring",
        "Friend-shoring", "Soft Landing", "Default",
        "Mergers and Acquisitions", "IPO", "Volatility", "Liquidity", "Solvency",
        "Private Equity", "Hedge Fund", "Venture Capital", "Angel Investor",
        "Unicorn", "Monopoly", "Oligopoly", "Antitrust", "Short Selling",
        "Arbitrage", "Bull Market", "Bear Market", "Correction", "Bubble", "Bailout"
    ],
    "Technology_and_Science": [
        "Generative AI", "LLM", "AGI", "Semiconductor", "Chip War",
        "Quantum Computing", "Cybersecurity", "Big Data", "Cloud Computing",
        "IoT", "Autonomous Vehicles", "Biotechnology", "Gene Editing",
        "Metaverse", "Digital Transformation", "Fintech", "Algorithm Bias",
        "Deepfake", "Tech Regulation", "Space Race",
        "Decentralization", "Blockchain", "Augmented Reality", "Virtual Reality",
        "Machine Learning", "Neural Network", "Open Source", "Proprietary", "SaaS",
        "Disruptive Innovation", "Scalability", "Encryption", "Surveillance",
        "Biometrics", "Drone Swarm", "Space Debris", "Fusion Energy",
        "Telemedicine", "Bioethics", "Singularity"
    ],
    "Society_and_Culture": [
        "Pandemic", "Endemic", "Aging Population", "Demographic Crisis",
        "Migration", "Refugees", "Urbanization", "Polarization",
        "Disinformation", "Misinformation", "Censorship", "Human Rights",
        "Hybrid Warfare", "Asymmetric Warfare", "Nuclear Proliferation",
        "Terrorism", "Food Security", "DEI", "Remote Work", "Mental Health",
        "Social Safety Net", "Civil Unrest",
        "Gentrification", "Cancel Culture", "Woke", "Echo Chamber", "Filter Bubble",
        "Identity Politics", "Glass Ceiling", "Meritocracy", "Nepotism",
        "Brain Drain", "Great Resignation", "Quiet Quitting", "Work-Life Balance",
        "Digital Nomad", "Whistleblower", "Xenophobia", "Alt-Right",
        "Political Correctness", "Conspiracy Theory", "Post-Truth"
    ],
    "Environment_and_Energy": [
        "Climate Change", "Global Warming", "Carbon Neutrality", "Net Zero", "ESG",
        "Renewable Energy", "Fossil Fuels", "Greenwashing", "Decarbonization",
        "Energy Security", "Critical Minerals", "Biodiversity", "Sustainability",
        "Paris Agreement", "Extreme Weather", "Circular Economy", "Carbon Footprint",
        "Green Hydrogen", "Electric Vehicle", "Battery Tech",
        "Small Modular Reactor", "Carbon Capture",
        "Direct Air Capture", "Energy Transition", "Grid Parity", "Smart Grid",
        "Liquefied Natural Gas", "Fracking", "OPEC Plus", "Peak Oil",
        "Anthropocene", "Tipping Point", "Permafrost", "Ocean Acidification",
        "Microplastics", "Rewilding", "Deforestation", "Desertification",
        "El Niño", "La Niña", "Carbon Tax", "Cap and Trade", "Climate Justice",
        "Urban Heat Island", "Ecocide", "Single-use Plastics"
    ]
}

GDELT_API_URL = "https://api.gdeltproject.org/api/v2/doc/doc"
OUTPUT_FILE = "gdelt_news_final_relevance.csv"
LOG_FILE = "gdelt_processed_days_rel.txt"


HEADERS = {
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36'
} # Headers to mimic a real browser request to avoid basic blocking

# --- 2. Helper Functions ---
def create_retry_session(retries=3, backoff_factor=0.5):
    session = requests.Session()
    retry = Retry(
        total=retries,
        read=retries,
        connect=retries,
        backoff_factor=backoff_factor,
        status_forcelist=[500, 502, 503, 504, 429],
    )
    adapter = HTTPAdapter(max_retries=retry)
    session.mount('http://', adapter)
    session.mount('https://', adapter)
    return session

def format_keyword_for_query(k):
    if " " in k or "-" in k:
        return f'"{k}"'
    return k

def fetch_chunk(session, start_dt, end_dt, chunk_keywords, theme):
    formatted_keywords = [format_keyword_for_query(k) for k in chunk_keywords]
    query_keywords = " OR ".join(formatted_keywords)
    
    if len(formatted_keywords) > 1:
        full_query = f'({query_keywords}) sourcelang:eng'
    else:
        full_query = f'{query_keywords} sourcelang:eng'
    
    params = {
        'query': full_query,
        'mode': 'artlist',
        'maxrecords': 10,
        'format': 'json',
        'startdatetime': start_dt,
        'enddatetime': end_dt
    }
    
    max_retries = 3
    for attempt in range(max_retries):
        try:
            response = session.get(GDELT_API_URL, params=params, headers=HEADERS, timeout=15)
            if response.status_code != 200:
                time.sleep(0.5)
                continue
            
            data = response.json(strict=False)
            return data.get('articles', [])

        except json.JSONDecodeError:
            if attempt < max_retries - 1:
                time.sleep(1)
                continue
            return []
        except Exception:
            if attempt < max_retries - 1:
                time.sleep(1)
                continue
            return []
    return []

def match_keyword_in_title(title, keywords):
    title_lower = title.lower()
    for k in keywords:
        if k.lower() in title_lower:
            return k
    return "Body Match/Other"

# --- 3. Daily Processing Function ---
def process_single_day(target_date):
    session = create_retry_session()
    
    date_str = target_date.strftime("%Y-%m-%d")
    start_dt = target_date.strftime("%Y%m%d000000")
    end_dt = target_date.strftime("%Y%m%d235959")
    
    daily_results = []
    
    for theme, keywords in global_keywords.items():
        chunk_size = 10 
        for i in range(0, len(keywords), chunk_size):
            chunk = keywords[i:i + chunk_size]
            articles = fetch_chunk(session, start_dt, end_dt, chunk, theme)
            
            for art in articles:
                daily_results.append({
                    'article': art,
                    'theme': theme,
                    'keywords': keywords
                })
            time.sleep(0.01)
            
    final_records = []
    seen_urls = set()
    
    for item in daily_results:
        art = item['article']
        url = art.get('url')
        
        if url not in seen_urls:
            seen_urls.add(url)
            title = art.get('title', '')
            media = art.get('domain', '')
            pub_date_raw = art.get('seendate', '') 
            pub_date = f"{pub_date_raw[:4]}-{pub_date_raw[4:6]}-{pub_date_raw[6:8]}" if pub_date_raw else date_str
            
            matched_keyword = match_keyword_in_title(title, item['keywords'])
            
            final_records.append({
                'Date': pub_date,
                'Theme': item['theme'],
                'Keyword': matched_keyword,
                'Title': title,
                'Media': media,
                'Link': url
            })
            
    return date_str, final_records

# --- 4. Recovery Related ---
def load_processed_dates():
    if not os.path.exists(LOG_FILE):
        return set()
    with open(LOG_FILE, 'r', encoding='utf-8') as f:
        return set(line.strip() for line in f if line.strip())

def mark_date_as_processed(date_str):
    with open(LOG_FILE, 'a', encoding='utf-8') as f:
        f.write(f"{date_str}\n")

# --- 5. Main Execution ---
def main():
    if not os.path.exists(OUTPUT_FILE):
        pd.DataFrame(columns=['Date', 'Theme', 'Keyword', 'Title', 'Media', 'Link']).to_csv(OUTPUT_FILE, index=False, encoding='utf-8-sig')

    start_date = datetime(2017, 1, 1)
    end_date = datetime.now() - timedelta(days=1)
    
    processed_dates = load_processed_dates()
    print(f"Loaded {len(processed_dates)} processed days.")

    all_dates = []
    curr = start_date
    while curr <= end_date:
        all_dates.append(curr)
        curr += timedelta(days=1)
    
    dates_to_process = [d for d in all_dates if d.strftime("%Y-%m-%d") not in processed_dates]
    
    print(f"Remaining to process: {len(dates_to_process)} days.")
    
    if not dates_to_process:
        print("All dates are already processed!")
        return

    with ThreadPoolExecutor(max_workers=15) as executor:
        future_to_date = {executor.submit(process_single_day, d): d for d in dates_to_process}
        
        for future in as_completed(future_to_date):
            d = future_to_date[future]
            try:
                date_str, records = future.result()
                
                if records:
                    df = pd.DataFrame(records)
                    df.to_csv(OUTPUT_FILE, mode='a', header=False, index=False, encoding='utf-8-sig')
                    print(f"[{date_str}] Saved {len(records)}.")
                else:
                    print(f"[{date_str}] No articles found.")
                
                mark_date_as_processed(date_str)
                    
            except Exception as e:
                print(f"Error processing {d.date()}: {e}")

if __name__ == "__main__":
    main()