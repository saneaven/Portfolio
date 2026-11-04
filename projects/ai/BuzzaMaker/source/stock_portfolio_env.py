import gymnasium as gym
from gymnasium import spaces
import numpy as np

class PortfolioEnv(gym.Env):
    metadata = {"render_modes": ["human"]}

    def __init__(
        self,
        stock_feat: np.ndarray,    # (T, N, F_stock)
        stock_news: np.ndarray,    # (T, N, F_news)
        global_news: np.ndarray,   # (T, F_macro)
        close_prices: np.ndarray,  # (T, N)
        window_size: int = 60,
        trading_cost: float = 0.001,  # 0.1% commission
    ):
        super().__init__()

        assert stock_feat.shape[0] == stock_news.shape[0] == global_news.shape[0] == close_prices.shape[0]
        assert stock_feat.shape[1] == stock_news.shape[1] == (close_prices.shape[1] - 1) # Close prices include cash asset.

        self.stock_feat = stock_feat.astype(np.float32)
        self.stock_news = stock_news.astype(np.float32)
        self.global_news = global_news.astype(np.float32)
        self.close_prices = close_prices.astype(np.float32)

        self.T_total, self.num_stocks, self.stock_feat_dim = stock_feat.shape
        _, _, self.news_feat_dim = stock_news.shape
        _, self.macro_dim = global_news.shape

        self.window_size = window_size
        self.trading_cost = trading_cost

        # --- Observation Space (Dict) ---
        self.observation_space = spaces.Dict({
            "stock": spaces.Box(
                low=-np.inf, high=np.inf,
                shape=(self.num_stocks, self.window_size, self.stock_feat_dim),
                dtype=np.float32
            ),
            "stock_news": spaces.Box(
                low=-np.inf, high=np.inf,
                shape=(self.num_stocks, self.window_size, self.news_feat_dim),
                dtype=np.float32
            ),
            "global_news": spaces.Box(
                low=-np.inf, high=np.inf,
                shape=(self.window_size, self.macro_dim),
                dtype=np.float32
            ),
        })

        # --- Action Space: raw logits for portfolio weights ---
        self.action_space = spaces.Box(
            low=-1.0, high=1.0,
            shape=(self.num_stocks + 1,), # Includes cash asset
            dtype=np.float32
        )

        # Internal state
        self.current_step = None
        self.wealth = None
        self.prev_weights = None

    # -------------------------------------------------
    # Helper: Create observation at current step (Window sliding)
    # -------------------------------------------------
    def _get_obs(self):
        start = self.current_step - self.window_size # type: ignore
        end = self.current_step

        # (window_size, N, F) -> (N, window_size, F)
        stock_win = self.stock_feat[start:end].transpose(1, 0, 2)
        news_win = self.stock_news[start:end].transpose(1, 0, 2)
        global_win = self.global_news[start:end]

        obs = {
            "stock": stock_win,
            "stock_news": news_win,
            "global_news": global_win,
        }
        return obs

    # -------------------------------------------------
    # reset: Start episode
    # -------------------------------------------------
    def reset(self, *, seed=None, options=None):
        super().reset(seed=seed)

        self.current_step = self.window_size  # First few steps are for building history
        self.wealth = 1.0

        # Initial portfolio is equally distributed
        self.prev_weights = np.ones(self.num_stocks + 1, dtype=np.float32) / (self.num_stocks + 1)

        obs = self._get_obs()
        info = {"wealth": self.wealth}
        return obs, info

    # -------------------------------------------------
    # step: Action -> Next state, Reward
    # -------------------------------------------------
    def step(self, action):
        # 1. Convert action to portfolio weights (softmax)
        action = np.asarray(action, dtype=np.float32)

        # Stable softmax
        logits = action - np.max(action)
        exp_logits = np.exp(logits)
        weights = exp_logits / (np.sum(exp_logits) + 1e-8)

        # 2. Calculate asset returns from current step to next step
        t = self.current_step
        # Valid only until just before the end
        done = (t >= self.T_total - 2)

        # Asset returns r_t = P_{t+1} / P_t - 1
        price_rel = self.close_prices[t + 1] / self.close_prices[t]  # shape: (N,) # type: ignore
        asset_ret = price_rel - 1.0

        # 3. Transaction cost (based on portfolio turnover)
        turnover = np.sum(np.abs(weights - self.prev_weights))
        cost = self.trading_cost * turnover

        # 4. Portfolio return & Reward (Log return)
        gross_ret = float(np.dot(weights, asset_ret))
        net_ret = gross_ret - cost
        reward = np.log(1.0 + net_ret)

        # 5. Update internal state
        self.wealth *= (1.0 + net_ret)
        self.prev_weights = weights
        self.current_step += 1  # type: ignore

        obs = self._get_obs()
        terminated = done
        truncated = False

        info = {
            "wealth": self.wealth,
            "portfolio_return": net_ret,
            "turnover": turnover,
        }
        return obs, reward, terminated, truncated, info

    def render(self):
        # Simply print current wealth trend
        print(f"Step: {self.current_step}, Wealth: {self.wealth:.4f}")
