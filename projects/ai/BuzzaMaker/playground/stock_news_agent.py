import torch
import torch.nn as nn
import torch.nn.functional as nn_F

from torch.distributions import Normal

class StockNewsTransformerLayer(nn.Module):
    """
    하나의 트랜스포머 블록:
    1. Stock Self-Attention (주가 패턴 분석)
    2. News Self-Attention (뉴스 패턴 분석)
    3. Stock-News Cross-Attention (뉴스 정보 참조)
    4. Feed Forward Network (정보 통합)
    """
    def __init__(self, d_model, n_head, dropout=0.1):
        super().__init__()

        # 1. Stock Self-Attention (주가끼리)
        self.stock_self_attn = nn.MultiheadAttention(d_model, n_head, batch_first=True)
        self.stock_norm1 = nn.LayerNorm(d_model)
        self.stock_dropout1 = nn.Dropout(dropout)

        # 2. News Self-Attention (뉴스끼리)
        self.news_self_attn = nn.MultiheadAttention(d_model, n_head, batch_first=True)
        self.news_norm1 = nn.LayerNorm(d_model)
        self.news_dropout1 = nn.Dropout(dropout)

        # 3. Cross-Attention (주가가 뉴스를 참조)
        self.cross_attn = nn.MultiheadAttention(d_model, n_head, batch_first=True)
        self.cross_norm = nn.LayerNorm(d_model)
        self.cross_dropout = nn.Dropout(dropout)

        # 4. Gating Mechanism (뉴스를 얼마나 믿을지 학습하는 파라미터)
        # 초기값 0: 처음엔 주가만 보다가 학습하면서 서서히 뉴스를 참고함
        self.gate = nn.Parameter(torch.tensor(0.0))

        # 5. Stock Feed Forward Network
        self.stock_ffn = nn.Sequential(
            nn.Linear(d_model, d_model * 4),
            nn.GELU(),
            nn.Linear(d_model * 4, d_model)
        )
        self.stock_norm2 = nn.LayerNorm(d_model)
        self.stock_dropout2 = nn.Dropout(dropout)

        # 6. News Feed Forward Network
        self.news_ffn = nn.Sequential(
            nn.Linear(d_model, d_model * 4),
            nn.GELU(),
            nn.Linear(d_model * 4, d_model)
        )
        self.news_norm2 = nn.LayerNorm(d_model)
        self.news_dropout2 = nn.Dropout(dropout)

    def forward(self, stock_emb, news_emb, causal_mask=None):
        # --- Step 1: Stock Self-Attention ---
        # Query=Stock, Key=Stock, Value=Stock
        # attn_mask=causal_mask (미래 주가 보지 마!)
        src2, _ = self.stock_self_attn(stock_emb, stock_emb, stock_emb, attn_mask=causal_mask)
        stock_emb = self.stock_norm1(stock_emb + self.stock_dropout1(src2))

        # --- Step 2: News Self-Attention ---
        # Query=News, Key=News, Value=News
        # attn_mask=causal_mask (미래 뉴스 보지 마!)
        src2, _ = self.news_self_attn(news_emb, news_emb, news_emb, attn_mask=causal_mask)
        news_emb = self.news_norm1(news_emb + self.news_dropout1(src2))

        # --- Step 3: Stock-News Cross-Attention ---
        # Query=Stock(현재상황), Key=News, Value=News
        src2, _ = self.cross_attn(query=stock_emb, key=news_emb, value=news_emb, attn_mask=causal_mask)
        # Gating: 주가 정보에 (Gate * 뉴스정보)를 더함
        stock_emb = self.cross_norm(stock_emb + self.gate * self.cross_dropout(src2))

        # --- Step 4: Stock FFN ---
        src2 = self.stock_ffn(stock_emb)
        stock_emb = self.stock_norm2(stock_emb + self.stock_dropout2(src2))

        # --- Step 5: News FFN ---
        src2 = self.news_ffn(news_emb)
        news_emb = self.news_norm2(news_emb + self.news_dropout2(src2))

        return stock_emb, news_emb


class StockNewsAgent(nn.Module):
    def __init__(self, stock_dim, news_dim, d_model, n_head, num_layers, result_dim, max_len=60):
        super().__init__()
        
        # 임베딩 레이어 (차원 맞추기)
        self.stock_proj = nn.Linear(stock_dim, d_model)
        self.news_proj = nn.Sequential(
            nn.Linear(news_dim, d_model),
            nn.ReLU(),
            nn.Linear(d_model, d_model)
        )
        
        # 위치 정보 (Positional Encoding)
        # 주식은 절대적인 날짜 위치보다 상대적 순서가 중요하므로 학습 가능한 파라미터 사용
        self.pos_emb = nn.Parameter(torch.zeros(1, max_len, d_model))
        
        # 트랜스포머 레이어 쌓기
        self.layers = nn.ModuleList([
            StockNewsTransformerLayer(d_model, n_head) for _ in range(num_layers)
        ])
        
        # 최종 행동 결정 (매수/매도/관망)
        self.head = nn.Linear(d_model, result_dim)

    def generate_causal_mask(self, sz, device):
        """미래 참조 방지 마스크 생성 (삼각형)"""
        mask = (torch.triu(torch.ones(sz, sz, device=device)) == 1).transpose(0, 1)
        mask = mask.float().masked_fill(mask == 0, float('-inf')).masked_fill(mask == 1, float(0.0))
        return mask

    def forward(self, stock_x, news_x):
        """
        stock_x: (Batch, Time, stock_dim)
        news_x:  (Batch, Time, news_dim) - 미리 뽑아둔 FinBERT 벡터
        """
        B, T, _ = stock_x.size()
        
        # 1. Embedding & Position
        s_emb = self.stock_proj(stock_x) + self.pos_emb[:, :T, :]
        n_emb = self.news_proj(news_x) + self.pos_emb[:, :T, :] # 뉴스에도 시간 순서 부여
        
        # 2. Causal Mask 생성 (미래 가리기용)
        # T x T 크기의 마스크 생성
        causal_mask = self.generate_causal_mask(T, stock_x.device)
        
        # 3. 레이어 통과 (주식과 뉴스 임베딩 모두 업데이트)
        for layer in self.layers:
            s_emb, n_emb = layer(s_emb, n_emb, causal_mask=causal_mask)
            
        # 4. 마지막 시점(Today)의 정보만 사용하여 결정
        # s_emb[:, -1, :] -> (Batch, d_model)
        return self.head(s_emb[:, -1, :])
    

class PortfolioMaker(nn.Module):
    def __init__(self, d_model, stock_dim, news_dim, n_head, macro_dim, agent_layers):
        super().__init__()
        
        # 1. 개별 주식 처리용 (기존과 동일)
        self.stock_encoder = StockNewsAgent(d_model=d_model, n_head=n_head, 
                                            stock_dim=stock_dim, 
                                            news_dim=news_dim, 
                                            num_layers=agent_layers, 
                                            result_dim=d_model)
        
        # 2. 글로벌 뉴스 처리용 인코더 (새로 추가!)
        # 글로벌 뉴스는 차원이 다르니(예: 768) d_model로 맞춰줘야 함
        self.macro_proj = nn.Sequential(
            nn.Linear(macro_dim, d_model),
            nn.ReLU(),
            nn.Linear(d_model, d_model)
        )
        
        # 3. 종목 + 매크로 통합 어텐션
        self.stock_attention = nn.MultiheadAttention(d_model, n_head, batch_first=True)
        self.norm = nn.LayerNorm(d_model)

    def forward(self, stock_x, news_x, global_news_x):
        # stock_x: (Batch, Num_Stocks, Time, Features)
        # global_news_x: (Batch, Time, Macro_Dim) -> 모든 주식에 공통 적용되는 뉴스
        
        B, N, T, F = stock_x.shape
        _, _, _, G = news_x.shape
        
        # --- Step 1: 개별 주식 인코딩 ---
        # (Batch * Num_Stocks, Time, Features) -> (Batch * Num_Stocks, d_model)
        flat_stock = stock_x.view(B * N, T, F)
        flat_news = news_x.view(B * N, T, G)
        stock_emb = self.stock_encoder(flat_stock, flat_news).view(B, N, -1) # (Batch, N, d_model)
        
        # --- Step 2: 글로벌 뉴스 인코딩 (Super Token 만들기) ---
        # 글로벌 뉴스는 주식 개수(N)와 상관없이 배치당 1개만 존재
        # 뉴스 시계열 처리 후 마지막 시점만 가져온다고 가정 (혹은 별도 인코더 사용)
        # 여기서는 간단히 마지막 시점만 Projection 한다고 가정
        macro_token = self.macro_proj(global_news_x[:, -1, :]) # (Batch, d_model)
        
        # 차원 맞추기: (Batch, 1, d_model)
        macro_token = macro_token.unsqueeze(1) 
        
        # --- Step 3: 토큰 합체 (Concatenation) ---
        # [글로벌뉴스(1개), 주식1, 주식2, ..., 주식N]
        # 크기: (Batch, N + 1, d_model)
        combined_features = torch.cat([macro_token, stock_emb], dim=1)
        
        # --- Step 4: 다같이 회의 시작 (Self-Attention) ---
        # 주식들이 서로 대화하고, 동시에 글로벌 뉴스의 눈치도 봄
        attn_out, _ = self.stock_attention(combined_features, combined_features, combined_features)
        
        # Residual + Norm
        combined_features = self.norm(combined_features + attn_out)
        
        # --- Step 5: 비중 계산 (중요!) ---
        # 글로벌 뉴스 토큰은 '참조용'이지 '매수 대상'이 아님.
        # 따라서 0번째 토큰(뉴스)은 버리고, 나머지 주식 토큰들만 가져옴.
        final_stock_features = combined_features[:, 1:, :] # (Batch, N, d_model)
        
        return final_stock_features


class PortfolioPPOLearner(nn.Module):
    def __init__(self, portfolio_transformer, num_stocks, d_model):
        super().__init__()
        
        # 1. 몸통 (Backbone): 우리가 만든 트랜스포머
        # 주의: 기존 코드에서 마지막 'portfolio_head'를 떼어내고 
        # features(벡터)만 리턴하도록 수정했다고 가정합니다.
        self.backbone = portfolio_transformer 
        
        # 2. 비평가 (Critic Head)
        # 상태 가치(V(s))는 스칼라 값 1개입니다.
        # 입력: (Batch, Num_Stocks, d_model) -> Flatten -> 1
        self.critic_head = nn.Sequential(
            nn.Linear(num_stocks * d_model, d_model),
            nn.Tanh(),
            nn.Linear(d_model, 1)
        )
        
        # 3. 배우 (Actor Head) - 연속적인 행동 (Continuous Action)
        # 포트폴리오 비중 조절을 위해 각 종목별 '매수 강도(Mean)'를 뽑습니다.
        self.actor_mean = nn.Sequential(
            nn.Linear(num_stocks * d_model, num_stocks), # 종목 수만큼 출력
            # 비중은 0~1 사이가 좋으므로 Tanh나 Softmax 등을 고려, 
            # 여기서는 PPO 기본인 Raw Logit(무한대 범위) 출력 후 나중에 처리
        )
        
        # 탐험(Exploration)을 위한 표준편차 (Log Std)
        # 학습 가능한 파라미터로 설정
        self.actor_logstd = nn.Parameter(torch.zeros(1, num_stocks))

    def forward(self):
        # PPO에서는 forward를 잘 안 쓰고 get_action, get_value를 따로 씁니다.
        raise NotImplementedError

    def get_value(self, stock_x, news_x, global_news_x):
        """
        Critic: 현재 상태의 가치 평가
        """
        # 1. 트랜스포머로 특징 추출 (features: Batch, N, d_model)
        features = self.backbone(stock_x, news_x, global_news_x)

        # 2. Flatten (Batch, N * d_model)
        features_flat = features.view(features.size(0), -1)

        # 3. 가치 출력
        return self.critic_head(features_flat)

    def get_action_and_value(self, stock_x, news_x, global_news_x, action=None):
        """
        Actor: 행동 결정 + Critic: 가치 평가 (학습용)
        """
        # 1. 특징 추출
        features = self.backbone(stock_x, news_x, global_news_x)
        features_flat = features.view(features.size(0), -1)

        # 2. Actor (행동 분포 만들기)
        action_mean = self.actor_mean(features_flat)
        action_logstd = self.actor_logstd.expand_as(action_mean)
        action_std = torch.exp(action_logstd)

        # 정규분포(Gaussian) 생성
        probs = Normal(action_mean, action_std)

        # 3. 행동 샘플링 (실전 매매 or 학습 시 탐험)
        if action is None:
            action = probs.sample()

        # 4. 포트폴리오 비중으로 변환 (합이 1이 되도록 softmax 적용)
        weights = nn_F.softmax(action, dim=-1)

        # 5. Critic (가치 평가)
        value = self.critic_head(features_flat)

        # 6. Entropy 계산 (PPO exploration 용)
        entropy = probs.entropy().sum(1)

        # PPO 학습에 필요한 4가지: 비중, 로그확률, entropy, 가치
        return weights, probs.log_prob(action).sum(1), entropy, value