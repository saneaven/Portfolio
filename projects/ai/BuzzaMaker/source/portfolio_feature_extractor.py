import gymnasium as gym
import torch
import torch.nn as nn
from stable_baselines3.common.torch_layers import BaseFeaturesExtractor

from .stock_news_backbone import PortfolioMaker

class PortfolioFeatureExtractor(BaseFeaturesExtractor):
    """
    obs(dict) -> (Batch, features_dim) tensor conversion
    Uses PortfolioMaker (Transformer) as the backbone here
    """
    def __init__(
        self,
        observation_space: gym.spaces.Dict,
        d_model: int,
        n_head: int,
        agent_layers: int,
    ):
        # Extract stock/dimension info from Dict Space
        stock_space = observation_space["stock"]
        stock_news_space = observation_space["stock_news"]
        global_news_space = observation_space["global_news"]

        if (stock_space.shape is None or
            stock_news_space.shape is None or
            global_news_space.shape is None):
            raise ValueError("Observation space shapes must be defined.") # Just to delete warning

        num_stocks = stock_space.shape[0]
        T = stock_space.shape[1]
        stock_dim = stock_space.shape[2]
        embedded_dim = stock_news_space.shape[2]
        assert global_news_space.shape[0] == T

        self.num_stocks = num_stocks
        self.d_model = d_model

        # Final feature_dim = N * d_model
        features_dim = num_stocks * d_model

        super().__init__(observation_space, features_dim)

        self.backbone = PortfolioMaker(
            d_model=d_model,
            stock_dim=stock_dim,
            embedded_dim=embedded_dim,
            n_head=n_head,
            agent_layers=agent_layers,
        )

    def forward(self, observations: dict) -> torch.Tensor:
        """
        observations:
          - "stock":       (B, N, T, F)
          - "stock_news":  (B, N, T, G)
          - "global_news": (B, T, macro_dim)
        """
        stock_x = observations["stock"].float()
        news_x = observations["stock_news"].float()
        global_news_x = observations["global_news"].float()

        # Backbone: (B, N, d_model)
        features = self.backbone(stock_x, news_x, global_news_x)

        # SB3 expects (B, features_dim) vector
        B = features.size(0)
        return features.view(B, -1)
