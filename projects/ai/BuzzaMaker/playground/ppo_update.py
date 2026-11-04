import torch
import torch.nn as nn

# --- PPO 하이퍼파라미터 ---
GAMMA = 0.99            # 미래 보상 할인율 (미래의 100원보다 현재의 100원을 더 쳐줌)
GAE_LAMBDA = 0.95       # GAE 파라미터 (Advantage 계산 시 편향/분산 조절)
CLIP_EPSILON = 0.2      # PPO 클리핑 범위 (갑격스러운 변화 방지, 보통 0.1 ~ 0.2)
K_EPOCHS = 4            # 수집한 데이터로 몇 번 반복 학습할지
MINI_BATCH_SIZE = 64    # 한 번 업데이트할 때 사용할 데이터 묶음 크기
VALUE_COEF = 0.5        # 가치 손실(Critic Loss) 반영 비율
ENTROPY_COEF = 0.01     # 엔트로피 보너스 (탐험 유도)

def compute_gae(rewards, values, dones, next_value):
    """
    GAE (Generalized Advantage Estimation) 계산
    : 보상이 얼마나 좋았는지(Advantage)와 가치 목표(Returns)를 계산함
    """
    advantages = []
    returns = []
    gae = 0
    
    # 뒤에서부터 계산 (Backtracking)
    # values: [v0, v1, ..., vn]
    # next_value: vn+1 (끝난 시점의 가치)
    values = values + [next_value] 
    
    for t in reversed(range(len(rewards))):
        # delta = 실제보상 + (할인된 미래가치) - 현재가치
        delta = rewards[t] + GAMMA * values[t + 1] * (1 - dones[t]) - values[t]
        
        # gae = delta + (할인율 * 람다 * 미래의 gae)
        gae = delta + GAMMA * GAE_LAMBDA * (1 - dones[t]) * gae
        
        advantages.insert(0, gae)
        
        # Return = Advantage + Value (또는 보상의 누적 합)
        returns.insert(0, gae + values[t])
        
    return advantages, returns

def ppo_update(model, optimizer, memory):
    """
    PPO 업데이트 로직
    memory: [obs_stock, obs_news, actions, log_probs, rewards, dones, values] 리스트들
    """
    
    # 1. 리스트를 텐서로 변환 (데이터 정리)
    # stock_tensor shape: (Total_Step, N, T, F) -> 이미 수집 때 처리됨
    stock_batch = torch.stack(memory['stock'])
    stock_news_batch = torch.stack(memory['stock_news'])
    global_news_batch = torch.stack(memory['global_news'])
    actions_batch = torch.stack(memory['actions'])
    old_log_probs_batch = torch.stack(memory['log_probs']).detach() # 구버전 정책의 확률
    rewards_batch = memory['rewards']
    dones_batch = memory['dones']
    values_batch = memory['values'] # (List of floats or tensors)
    
    # 마지막 상태의 가치(Next Value) 구하기 (GAE용)
    # (여기서는 편의상 마지막이 종료(Done)되었다고 가정하고 0으로 둠, 
    #  실제로는 episode 끝나기 전 update면 model.get_value(last_obs) 해야 함)
    next_value = 0 
    
    # 2. GAE (Advantage) 계산
    # advantages: "이 행동이 평균보다 얼마나 더 좋았나?"
    # returns: "실제로 얻은 총 보상은 얼마인가?" (Critic의 정답지)
    advantages, returns = compute_gae(rewards_batch, values_batch, dones_batch, next_value)
    
    advantages = torch.tensor(advantages, dtype=torch.float32)
    returns = torch.tensor(returns, dtype=torch.float32)
    
    # Advantage 정규화 (학습 안정성을 위해 필수!)
    advantages = (advantages - advantages.mean()) / (advantages.std() + 1e-8)
    
    # 3. K_EPOCHS 만큼 반복 학습
    dataset_size = len(rewards_batch)
    
    for _ in range(K_EPOCHS):
        # 랜덤 셔플 (Batch 단위로 쪼개서 학습)
        indices = torch.randperm(dataset_size)
        
        for start in range(0, dataset_size, MINI_BATCH_SIZE):
            end = start + MINI_BATCH_SIZE
            idx = indices[start:end]
            
            # 미니배치 데이터 준비
            mb_stock = stock_batch[idx]
            mb_stock_news = stock_news_batch[idx]
            mb_global_news = global_news_batch[idx]
            mb_actions = actions_batch[idx]
            mb_old_log_probs = old_log_probs_batch[idx]
            mb_advantages = advantages[idx]
            mb_returns = returns[idx]
            
            # --- 현재 모델로 다시 평가 (Re-evaluation) ---
            # 여기서 model을 다시 돌려서 "지금 모델은 어떻게 생각하는지" 값을 얻음
            # new_log_probs: 업데이트 중인 모델의 확률
            # new_values: 업데이트 중인 모델의 가치 평가
            # entropy: 탐험을 위한 엔트로피

            # get_action_and_value 함수에서 action을 넣으면, 그 action에 대한 log_prob를 구해줌
            _, new_log_probs, entropy, new_values = model.get_action_and_value(
                mb_stock, mb_stock_news, mb_global_news, action=mb_actions
            )
            
            # 가치 함수 차원 맞추기 (Batch, 1) -> (Batch)
            new_values = new_values.view(-1)
            
            # --- Loss 1: Actor Loss (Policy Gradient) ---
            # Ratio = (새로운 확률 / 옛날 확률)
            # exp(log_new - log_old) = new / old
            ratio = (new_log_probs - mb_old_log_probs).exp()
            
            # Surrogate Objective (클리핑)
            surr1 = ratio * mb_advantages
            surr2 = torch.clamp(ratio, 1.0 - CLIP_EPSILON, 1.0 + CLIP_EPSILON) * mb_advantages
            actor_loss = -torch.min(surr1, surr2).mean() # Maximize 해야 하므로 - 붙임
            
            # --- Loss 2: Critic Loss (Value Function) ---
            # 모델이 예측한 가치(new_values)와 실제 얻은 보상합(mb_returns)의 차이 제곱
            critic_loss = 0.5 * ((new_values - mb_returns) ** 2).mean()
            
            # --- Loss 3: Entropy Bonus ---
            # 분포가 너무 한쪽으로 쏠리지 않게(탐험 유지) 보너스를 줌
            entropy_loss = -ENTROPY_COEF * entropy.mean()

            # --- 최종 Loss 및 업데이트 ---
            total_loss = actor_loss + VALUE_COEF * critic_loss + entropy_loss
            
            optimizer.zero_grad()
            total_loss.backward()
            
            # Gradient Clipping (기울기 폭주 방지)
            nn.utils.clip_grad_norm_(model.parameters(), max_norm=0.5)
            
            optimizer.step()
            
    # 메모리 비우기
    # 리스트 내용물 삭제
    memory['stock'].clear()
    memory['stock_news'].clear()
    memory['global_news'].clear()
    memory['actions'].clear()
    memory['log_probs'].clear()
    memory['rewards'].clear()
    memory['dones'].clear()
    memory['values'].clear()