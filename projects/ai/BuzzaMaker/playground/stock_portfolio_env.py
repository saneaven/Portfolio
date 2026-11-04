import gymnasium as gym
from gymnasium import spaces
import numpy as np


class StockPortfolioEnv(gym.Env):
    """
    stock_data:            (T, N, F_stock)          - 피처 (예: 우리가 만든 8개 피처)
    stock_news_embeddings: (T, N, F_news)           - 종목별 뉴스 임베딩
    global_news_embeddings:(T, F_global)            - 글로벌 뉴스 임베딩
    """
    metadata = {"render_modes": []}

    def __init__(
        self,
        stock_data,
        stock_news_embeddings,
        global_news_embeddings,
        window_size: int = 60,
        initial_balance: float = 1_0000.0,
    ):
        super().__init__()

        # ---- 데이터 저장 & 형식 통일 ----
        self.stock_data = np.asarray(stock_data, dtype=np.float32)          # (T, N, F_stock)
        self.stock_news_data = np.asarray(stock_news_embeddings, dtype=np.float32)  # (T, N, F_news)
        self.global_news_data = np.asarray(global_news_embeddings, dtype=np.float32)  # (T, F_global)

        self.window_size = int(window_size)
        self.initial_balance = float(initial_balance)

        # 기본 shape 체크
        self.T, self.N, self.F_stock = self.stock_data.shape
        T2, N2, self.F_news = self.stock_news_data.shape
        T3, self.F_global = self.global_news_data.shape

        assert T2 == self.T and N2 == self.N, "stock_news_embeddings shape must match stock_data (T, N, ·)"
        assert T3 == self.T, "global_news_embeddings first dim (T) must match stock_data"

        # ---- Action space: 연속 포트폴리오 비중 벡터 ----
        #  raw 비중이지만 step에서 softmax/정규화할 거라 [-1, 1]로 두는 것도 가능
        self.action_space = spaces.Box(
            low=0.0,
            high=1.0,
            shape=(self.N,),
            dtype=np.float32,
        )

        # ---- Observation space: Dict ----
        #  stock:       (N, T_window, F_stock)
        #  stock_news:  (N, T_window, F_news)
        #  global_news: (T_window, F_global)
        self.observation_space = spaces.Dict(
            {
                "stock": spaces.Box(
                    low=-np.inf,
                    high=np.inf,
                    shape=(self.N, self.window_size, self.F_stock),
                    dtype=np.float32,
                ),
                "stock_news": spaces.Box(
                    low=-np.inf,
                    high=np.inf,
                    shape=(self.N, self.window_size, self.F_news),
                    dtype=np.float32,
                ),
                "global_news": spaces.Box(
                    low=-np.inf,
                    high=np.inf,
                    shape=(self.window_size, self.F_global),
                    dtype=np.float32,
                ),
            }
        )

        # 내부 상태
        self.current_step: int | None = None
        self.portfolio_value: float | None = None

    # ------------------------------------
    # reset
    # ------------------------------------
    def reset(self, seed=None, options=None):
        super().reset(seed=seed)

        self.current_step = self.window_size
        self.portfolio_value = float(self.initial_balance)

        obs = self._get_obs()
        info = {"value": self.portfolio_value}
        return obs, info

    # ------------------------------------
    # 내부: obs 만들기
    # ------------------------------------
    def _get_obs(self):
        if self.current_step is None:
            raise RuntimeError("Env must be reset before calling _get_obs.")

        start = self.current_step - self.window_size
        end = self.current_step  # [start, end)

        # stock_data: (T, N, F) -> window: (W, N, F) -> transpose: (N, W, F)
        stock_win = self.stock_data[start:end]           # (W, N, F_stock)
        stock_win = np.transpose(stock_win, (1, 0, 2))   # (N, W, F_stock)

        stock_news_win = self.stock_news_data[start:end]         # (W, N, F_news)
        stock_news_win = np.transpose(stock_news_win, (1, 0, 2)) # (N, W, F_news)

        global_news_win = self.global_news_data[start:end]       # (W, F_global)

        # dtype 강제
        return {
            "stock": stock_win.astype(np.float32),
            "stock_news": stock_news_win.astype(np.float32),
            "global_news": global_news_win.astype(np.float32),
        }

    # ------------------------------------
    # step
    # ------------------------------------
    def step(self, action):
        assert self.current_step is not None, "Env must be reset before stepping."

        # 1. 액션 정규화 (합 = 1)
        weights = np.asarray(action, dtype=np.float32)
        weights = np.clip(weights, 0.0, 1.0)
        s = weights.sum()
        if s <= 0.0:
            weights = np.ones_like(weights, dtype=np.float32) / self.N
        else:
            weights /= s

        # 2. 개별 종목 수익률 가져오기
        #   여기서는 stock_data[..., 0]이 단순 수익률 r_t (예: 0.01 = 1%)라고 가정
        asset_returns = self.stock_data[self.current_step, :, 0].astype(np.float32)  # (N,)

        # 3. 포트폴리오 수익률 r_p = Σ w_i * r_i
        portfolio_return = np.dot(weights, asset_returns)  # python float

        # 4. 자산 업데이트: V_{t+1} = V_t * (1 + r_p)
        old_value = self.portfolio_value
        self.portfolio_value = old_value * (1.0 + portfolio_return)

        # 5. 보상: log( V_{t+1} / V_t )
        eps = 1e-8
        reward = np.log((self.portfolio_value + eps) / (old_value + eps))

        # 6. 타임스텝 이동
        self.current_step += 1
        terminated = bool(self.current_step >= self.T - 1)
        truncated = False  # 필요하면 max_steps 같은 걸로 True 줄 수 있음

        obs = self._get_obs()
        info = {
            "value": self.portfolio_value,
            "portfolio_return": portfolio_return,
        }
        return obs, reward, terminated, truncated, info
