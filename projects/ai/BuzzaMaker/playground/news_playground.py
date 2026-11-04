import pandas as pd
from GoogleNews import GoogleNews
from datetime import datetime
import time

# ==========================================
# 1. 분야별 핵심 키워드 딕셔너리 (전체 포함)
# ==========================================
TOPIC_KEYWORDS = {
    # --- Part 1: 큰 흐름 (The Big Picture) ---
    "Geopolitics_and_Relations": [
        "Multipolarity", "Global South", "Hegemony", "Decoupling", "De-risking",
        "Protectionism", "Nationalism", "Trade War", "Sanctions", "Soft Power",
        "NATO Expansion", "Indo-Pacific Strategy", "Belt and Road Initiative",
        "Strategic Ambiguity", "Sovereignty", "Unilateralism", "Multilateralism",
        "Geopolitical Risk", "Proxy War", "Diplomatic Ties"
    ],
    "Economy_and_Finance": [
        "Inflation", "Stagflation", "Interest Rate Hike/Cut", "Recession",
        "Supply Chain Resilience", "Global Value Chain (GVC)", "Fiscal Policy",
        "Monetary Policy", "Quantitative Easing (QE)", "Tightening",
        "Emerging Markets", "Trade Deficit/Surplus", "Cryptocurrency",
        "CBDC", "Income Inequality", "Gig Economy", "Reshoring",
        "Friend-shoring", "Soft Landing", "Default"
    ],
    "Technology_and_Innovation": [
        "Generative AI", "LLM", "AGI", "Semiconductor", "Chip War",
        "Quantum Computing", "Cybersecurity", "Big Data", "Cloud Computing",
        "IoT", "Autonomous Vehicles", "Biotechnology", "Gene Editing (CRISPR)",
        "Metaverse", "Digital Transformation (DX)", "Fintech", "Algorithm Bias",
        "Deepfake", "Tech Regulation", "Space Race", "Decentralization", "Blockchain", "Augmented Reality (AR)", "Virtual Reality (VR)",
        "Machine Learning", "Neural Network", "Open Source", "Proprietary", "SaaS",
        "Disruptive Innovation", "Scalability", "Encryption", "Surveillance",
        "Biometrics", "Drone Swarm", "Space Debris", "Fusion Energy",
        "Telemedicine", "Bioethics", "Singularity"
    ],
    "Environment_and_Energy": [
        "Climate Change", "Global Warming", "Carbon Neutrality", "Net Zero", "ESG",
        "Renewable Energy", "Fossil Fuels", "Greenwashing", "Decarbonization",
        "Energy Security", "Critical Minerals", "Biodiversity", "Sustainability",
        "Paris Agreement", "Extreme Weather", "Circular Economy", "Carbon Footprint",
        "Green Hydrogen", "EV (Electric Vehicle)", "Battery Tech"
    ],
    "Society_Health_Security": [
        "Pandemic", "Endemic", "Aging Population", "Demographic Crisis",
        "Migration / Refugees", "Urbanization", "Polarization",
        "Disinformation / Misinformation", "Censorship", "Human Rights",
        "Hybrid Warfare", "Asymmetric Warfare", "Nuclear Proliferation",
        "Terrorism", "Food Security", "DEI", "Remote Work", "Mental Health",
        "Social Safety Net", "Civil Unrest"
    ],

    # --- Part 2: 심화 분석 (Deep Dive) ---
    "Political_Systems_Dynamics": [
        "Autocracy", "Populism", "Kleptocracy", "Technocracy", "Coalition",
        "Partisanship", "Bipartisan", "Gridlock", "Gerrymandering", "Filibuster",
        "Impeachment", "Referendum", "Veto", "Lobbying", "Bureaucracy",
        "Regime Change", "Coup d'état", "Dissident", "Swing State", "Grassroots"
    ],
    "Corporate_Market_Mechanics": [
        "M&A", "IPO", "Volatility", "Liquidity", "Solvency",
        "Private Equity (PE)", "Hedge Fund", "Venture Capital (VC)", "Angel Investor",
        "Unicorn", "Monopoly", "Oligopoly", "Antitrust", "Short Selling",
        "Arbitrage", "Bull Market", "Bear Market", "Correction", "Bubble", "Bailout"
    ],
    "Social_Trends_Culture": [
        "Gentrification", "Cancel Culture", "Woke", "Echo Chamber", "Filter Bubble",
        "Identity Politics", "Glass Ceiling", "Meritocracy", "Nepotism",
        "Brain Drain", "Great Resignation", "Quiet Quitting", "Work-Life Balance",
        "Digital Nomad", "Whistleblower", "Xenophobia", "Alt-Right",
        "Political Correctness (PC)", "Conspiracy Theory", "Post-Truth"
    ],
    "Advanced_Tech_Science": [
        "Decentralization", "Blockchain", "Augmented Reality (AR)", "Virtual Reality (VR)",
        "Machine Learning", "Neural Network", "Open Source", "Proprietary", "SaaS",
        "Disruptive Innovation", "Scalability", "Encryption", "Surveillance",
        "Biometrics", "Drone Swarm", "Space Debris", "Fusion Energy",
        "Telemedicine", "Bioethics", "Singularity"
    ],
    "Law_Conflict_Humanitarian": [
        "Sovereign Debt", "Austerity", "Tax Haven", "Money Laundering", "Corruption",
        "Cartel", "Insurgency", "Martial Law", "Curfew", "Extradition",
        "Asylum", "Amnesty", "Genocide", "Ethnic Cleansing", "Human Trafficking",
        "Humanitarian Aid", "NGO", "Geneva Convention", "Collateral Damage",
        "Status Quo"
    ]
}

# ==========================================
# 2. 뉴스 수집 함수
# ==========================================
def fetch_comprehensive_news(target_date):
    # 미국(US), 영어(en) 설정
    googlenews = GoogleNews(lang='en', region='US')
    
    # 날짜 설정 (MM/DD/YYYY)
    date_str = target_date.strftime('%m/%d/%Y')
    googlenews.set_time_range(date_str, date_str)
    
    all_data = []
    
    print(f"🚀 {target_date.strftime('%Y-%m-%d')} 뉴스 수집 시작 (키워드 약 {sum(len(v) for v in TOPIC_KEYWORDS.values())}개)...")

    # 딕셔너리 순회 (Category -> Keywords)
    for category, keywords in TOPIC_KEYWORDS.items():
        print(f"\n📂 [{category}] 섹터 스캔 중...")
        
        for key in keywords:
            try:
                googlenews.search(key)
                results = googlenews.results()
                
                # 결과가 있으면 상위 1개만 저장 (속도 및 중복 방지)
                if results:
                    top_news = results[0]
                    all_data.append({
                        "Category": category,     # 분야
                        "Keyword": key,           # 검색 키워드
                        "Title": top_news['title'],
                        "Media": top_news['media'],
                        "Date": top_news['date'],
                        "Link": top_news['link']
                    })
                    print(f"  - [{key}] {top_news['title'][:30]}...")
                else:
                    print(f"  - [{key}] 뉴스 없음")
                
                # 검색어 초기화 (필수)
                googlenews.clear()
                
            except Exception as e:
                print(f"  X [{key}] 에러: {e}")
                continue

    # 데이터프레임 변환
    if not all_data:
        return pd.DataFrame()

    df = pd.DataFrame(all_data)
    
    # 중복 뉴스 제거 (여러 키워드에 같은 뉴스가 걸릴 수 있음)
    df = df.drop_duplicates(subset=['Title'])
    
    return df

# ==========================================
# 3. 실행부
# ==========================================
if __name__ == "__main__":
    # 원하는 날짜 설정 (예: 2025년 10월 13일)
    target_date = datetime(2025, 10, 13)
    
    # 수집 실행
    df_result = fetch_comprehensive_news(target_date)
    
    if not df_result.empty:
        print("\n" + "="*50)
        print(f"✅ 수집 완료! 총 {len(df_result)}개의 핵심 뉴스 발견")
        print("="*50)
        
        # 카테고리별로 정렬해서 출력
        df_sorted = df_result.sort_values(by=['Category', 'Keyword'])
        
        # 전체 출력 옵션 (잘림 방지)
        pd.set_option('display.max_rows', None)
        pd.set_option('display.max_colwidth', 50)
        
        print(df_sorted[['Category', 'Keyword', 'Title', 'Media']])
        
        # 엑셀이나 CSV로 저장하려면 아래 주석 해제
        # df_sorted.to_csv(f"news_report_{target_date.strftime('%Y%m%d')}.csv", index=False)
    else:
        print("수집된 뉴스가 없습니다.")