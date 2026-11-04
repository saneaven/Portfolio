import FinanceDataReader as fdr
import yfinance as yf
import pandas as pd
from datetime import datetime
import time

def find_all_long_lived_stocks(years=30):
    # 1. 나스닥 전체 종목 리스트 가져오기 (약 3,400개)
    print("📥 나스닥 전체 리스트 다운로드 중...")
    df_nasdaq = fdr.StockListing('NASDAQ')
    
    # 종목 코드(Symbol)만 리스트로 추출
    all_tickers = df_nasdaq['Symbol'].tolist()
    
    print(f"총 {len(all_tickers)}개의 종목을 검사합니다.")
    print(f"기준: {datetime.now().year - years}년 이전 상장 기업\n")
    
    cutoff_year = datetime.now().year - years
    long_lived_stocks = []
    
    # 2. 반복문(Loop) 시작: 하나씩 꺼내서 검사
    # (테스트용으로 50개만 하려면: for ticker in all_tickers[:50]:)
    for i, ticker in enumerate(all_tickers):
        try:
            # 진행 상황 출력 (100개마다)
            if i % 100 == 0:
                print(f"[{i}/{len(all_tickers)}] 진행 중... (발견된 30년+ 기업: {len(long_lived_stocks)}개)")

            stock = yf.Ticker(ticker)
            
            # 메타데이터에서 상장일(firstTradeDate) 확인
            # (네트워크 요청이 일어나는 부분)
            hist_metadata = stock.history_metadata
            first_trade_ts = hist_metadata.get('firstTradeDate')

            if first_trade_ts:
                first_year = datetime.fromtimestamp(first_trade_ts).year
                
                # 30년 이상 되었는지 판별
                if first_year <= cutoff_year:
                    # 회사 이름 가져오기 (없으면 티커로 대체)
                    name = stock.info.get('shortName', ticker)
                    print(df_nasdaq[df_nasdaq['Symbol'] == ticker]['Industry'])
                    
                    long_lived_stocks.append({
                        "Ticker": ticker,
                        "Name": name,
                        "Listed_Year": first_year,
                        "Sector": df_nasdaq[df_nasdaq['Symbol'] == ticker]['Industry'].values[0] # 산업군 추가
                    })

        except Exception:
            # 상장폐지되거나 정보가 없는 경우 무시하고 계속 진행
            continue
            
        # ⚠️ 너무 빠르게 요청하면 차단당할 수 있어 0.1초 쉬기
        time.sleep(0.1)

    # 결과 정리
    return pd.DataFrame(long_lived_stocks)

# --- 실행 ---
# 시간이 오래 걸리므로 마음의 준비를 하고 실행하세요!
df_result = find_all_long_lived_stocks(years=30)

# 결과 저장
if not df_result.empty:
    df_result = df_result.sort_values(by='Listed_Year') # 오래된 순 정렬
    print(f"\n✅ 검사 완료! 총 {len(df_result)}개의 장수 기업 발견")
    print(df_result.head(10)) # 가장 오래된 10개 출력
    
    # CSV 파일로 저장
    df_result.to_csv("nasdaq_old_stocks.csv", index=False)
    print("파일로 저장되었습니다: nasdaq_old_stocks.csv")
else:
    print("검색된 종목이 없습니다.")