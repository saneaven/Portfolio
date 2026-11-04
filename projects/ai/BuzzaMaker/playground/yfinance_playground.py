import yfinance as yf

ticker = yf.Ticker("005930.KS") # 삼성전자

# 최근 1개월 동안의 데이터를 1시간(1h) 간격으로 가져오기
df = ticker.history(period="1mo", interval="1h")

print(df.head())

# 뉴스 데이터 가져오기 (딕셔너리 리스트 형태)
news_list = ticker.news

for news in news_list:
    print(news)
    print("-" * 30)