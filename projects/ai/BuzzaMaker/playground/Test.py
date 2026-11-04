import numpy as np
import torch

from ppo_update import ppo_update
from stock_portfolio_env import StockPortfolioEnv
from stock_news_agent import PortfolioMaker, PortfolioPPOLearner

# 1. 데이터 준비 (더미 데이터 예시)
# 실제로는 CSV 파일 등을 로드해서 전처리 후 넣어야 함
dummy_stock_data = np.random.randn(1000, 10, 10).astype(np.float32) # 1000일, 10종목, 10지표
# 첫 번째 feature(수익률)를 현실적인 가격 비율로 설정 (평균 1.0, 표준편차 2%)
dummy_stock_data[:, :, 0] = 1.0 + np.random.randn(1000, 10).astype(np.float32) * 0.02
dummy_stock_news_data = np.random.randn(1000, 10, 768).astype(np.float32) # 1000일, 10종목, 768 + 10지표
dummy_global_news_data = np.random.randn(1000, 768).astype(np.float32)    # 1000일, 768차원 뉴스

# 2. 환경 및 모델 생성
env = StockPortfolioEnv(dummy_stock_data, dummy_stock_news_data, dummy_global_news_data, window_size=60)
model = PortfolioPPOLearner(
    portfolio_transformer=PortfolioMaker(d_model=128, stock_dim=10, news_dim=768, n_head=8, macro_dim=768, agent_layers=6), # 아까 만든 백본
    num_stocks=10, 
    d_model=128
)

optimizer = torch.optim.Adam(model.parameters(), lr=3e-4)

# --- 1. 메모리 초기화 ---
# PPO 학습을 위해 데이터를 모아둘 리스트들입니다.
memory = {
    'stock': [],
    'stock_news': [],
    'global_news': [],
    'actions': [],
    'log_probs': [],
    'rewards': [],
    'dones': [],
    'values': []
}

# --- 2. 하이퍼파라미터 설정 ---
UPDATE_TIMESTEP = 2048  # 2048 스텝마다 학습 (데이터가 좀 모여야 학습이 잘 됨)
global_step = 0         # 전체 스텝 수 카운트

for episode in range(10):
    obs, _ = env.reset()
    done = False
    
    while not done:
        global_step += 1
        
        # --- A. 데이터 변환 (Numpy -> Tensor) ---
        stock_tensor = torch.from_numpy(obs['stock']).unsqueeze(0)
        # (Batch, Window, Stock, Feat) -> (Batch, Stock, Window, Feat)
        stock_tensor = stock_tensor.permute(0, 2, 1, 3) 
        
        stock_news_tensor = torch.from_numpy(obs['stock_news']).unsqueeze(0)
        # (Batch, Window, Stock, Feat) -> (Batch, Stock, Window, Feat)
        stock_news_tensor = stock_news_tensor.permute(0, 2, 1, 3)
        global_news_tensor = torch.from_numpy(obs['global_news']).unsqueeze(0)

        # --- B. 모델 행동 결정 및 값 추출 ---
        with torch.no_grad():
            # PPO 업데이트에 필요한 3가지(action, log_prob, value)를 모두 받아옵니다.
            raw_action, log_prob, value = model.get_action_and_value(stock_tensor, stock_news_tensor, global_news_tensor)

        # --- C. 환경에 행동 입력 ---
        weights = torch.softmax(raw_action, dim=1).squeeze(0).numpy()
        next_obs, reward, terminated, truncated, info = env.step(weights)
        done = terminated or truncated

        # --- D. 데이터 저장 (Memory) ---
        # 텐서는 CPU로 옮기고, 차원을 줄여서(squeeze) 저장해야 나중에 stack하기 편합니다.
        memory['stock'].append(stock_tensor.squeeze(0).cpu())     # (N, T, F)
        memory['stock_news'].append(stock_news_tensor.squeeze(0).cpu())       # (T, F)
        memory['global_news'].append(global_news_tensor.squeeze(0).cpu())     # (T, F)
        memory['actions'].append(raw_action.squeeze(0).cpu())     # (N,)
        memory['log_probs'].append(log_prob.squeeze(0).cpu())     # (N,) or Scalar
        memory['rewards'].append(torch.tensor(reward, dtype=torch.float32))
        memory['dones'].append(torch.tensor(done, dtype=torch.float32))

        memory['values'].append(value.squeeze(0).item())          # float 값으로 저장

        obs = next_obs
        
        # --- E. PPO 업데이트 실행 (핵심!) ---
        # 데이터가 일정량(UPDATE_TIMESTEP) 모이면 학습을 시작합니다.
        if global_step % UPDATE_TIMESTEP == 0:
            print(f"Updating PPO Agent at step {global_step}...")
            
            # 여기서 아까 만든 ppo_update 함수를 호출합니다!
            ppo_update(model, optimizer, memory)
            
            # ppo_update 함수 안에서 memory.clear()를 하므로, 여기선 따로 안 해도 됩니다.
            # (만약 ppo_update 함수 마지막에 clear가 없다면 여기서 memory = { ... } 로 초기화해야 함)

    print(f"Episode {episode} Finished. Final Value: {info['value']:.2f}")
    