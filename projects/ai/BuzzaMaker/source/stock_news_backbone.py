import torch
import torch.nn as nn

class StockNewsTransformerLayer(nn.Module):
    """
    One Transformer Block:
    1. Stock Self-Attention (Analyze stock patterns)
    2. News Self-Attention (Analyze news patterns)
    3. Stock-News Cross-Attention (Reference news info)
    4. Feed Forward Network (Integrate info)
    """
    def __init__(self, d_model, n_head, dropout=0.1):
        super().__init__()

        # 1. Stock Self-Attention (Between stocks)
        self.stock_self_attn = nn.MultiheadAttention(d_model, n_head, batch_first=True)
        self.stock_norm1 = nn.LayerNorm(d_model)
        self.stock_dropout1 = nn.Dropout(dropout)

        # 2. News Self-Attention (Between news)
        self.news_self_attn = nn.MultiheadAttention(d_model, n_head, batch_first=True)
        self.news_norm1 = nn.LayerNorm(d_model)
        self.news_dropout1 = nn.Dropout(dropout)

        # 3. Cross-Attention (Stock references news)
        self.cross_attn = nn.MultiheadAttention(d_model, n_head, batch_first=True)
        self.cross_norm = nn.LayerNorm(d_model)
        self.cross_dropout = nn.Dropout(dropout)

        # 4. Gating Mechanism (Parameter learning how much to trust news)
        # Initial value 0: Look only at stock initially, then gradually reference news during training
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

    def forward(self, stock_emb, news_emb, causal_mask):
        # --- Step 1: Stock Self-Attention ---
        # Query=Stock, Key=Stock, Value=Stock
        # Set attention mask to prevent looking into the future
        src2, _ = self.stock_self_attn(stock_emb, stock_emb, stock_emb, attn_mask=causal_mask)
        stock_emb = self.stock_norm1(stock_emb + self.stock_dropout1(src2))

        # --- Step 2: News Self-Attention ---
        # Query=News, Key=News, Value=News
        # Set attention mask to prevent looking into the future
        src2, _ = self.news_self_attn(news_emb, news_emb, news_emb, attn_mask=causal_mask)
        news_emb = self.news_norm1(news_emb + self.news_dropout1(src2))

        # --- Step 3: Stock-News Cross-Attention ---
        # Query=Stock(Current), Key=News, Value=News
        src2, _ = self.cross_attn(query=stock_emb, key=news_emb, value=news_emb, attn_mask=causal_mask)
        # Gating: Add (Gate * News Info) to Stock Info
        stock_emb = self.cross_norm(stock_emb + self.gate * self.cross_dropout(src2))

        # --- Step 4: Stock FFN ---
        src2 = self.stock_ffn(stock_emb)
        stock_emb = self.stock_norm2(stock_emb + self.stock_dropout2(src2))

        # --- Step 5: News FFN ---
        src2 = self.news_ffn(news_emb)
        news_emb = self.news_norm2(news_emb + self.news_dropout2(src2))

        return stock_emb, news_emb


class StockNewsAgent(nn.Module):
    def __init__(self, stock_dim, embedded_dim, d_model, n_head, num_layers, result_dim, max_len=60, dropout=0.1):
        super().__init__()
        
        # Embedding Layer (Match dimensions)
        self.stock_proj = nn.Linear(stock_dim, d_model)
        self.news_proj = nn.Sequential(
            nn.Linear(embedded_dim, d_model),
            nn.ReLU(),
            nn.Linear(d_model, d_model)
        )
        
        # Positional Encoding
        # Use learnable parameters since relative order is more important than absolute date for stocks
        self.pos_emb = nn.Parameter(torch.zeros(1, max_len, d_model))
        
        # Stack Transformer Layers
        self.layers = nn.ModuleList([
            StockNewsTransformerLayer(d_model, n_head, dropout) for _ in range(num_layers)
        ])
        
        # Final Action Decision (Buy/Sell/Hold)
        self.head = nn.Linear(d_model, result_dim)

    def forward(self, stock_x, news_x, causal_mask):
        """
        stock_x: (Batch, Time, stock_dim)
        news_x:  (Batch, Time, news_dim)
        """
        B, T, _ = stock_x.size()
        
        # 1. Embedding & Position
        s_emb = self.stock_proj(stock_x) + self.pos_emb[:, :T, :]
        n_emb = self.news_proj(news_x) + self.pos_emb[:, :T, :] # Assign time order to news as well
        
        # 3. Pass through layers (Update both stock and news embeddings)
        for layer in self.layers:
            s_emb, n_emb = layer(s_emb, n_emb, causal_mask=causal_mask)
            
        # 4. Decide using only the information of the last time step (Today)
        # s_emb[:, -1, :] -> (Batch, d_model)
        return self.head(s_emb[:, -1, :])
    

class PortfolioMaker(nn.Module):
    def __init__(self, d_model, stock_dim, embedded_dim, n_head, agent_layers, dropout=0.1, window_size=60):
        super().__init__()
        
        # 1. For individual stock processing (Same as before)
        self.stock_encoder = StockNewsAgent(d_model=d_model, n_head=n_head, 
                                            stock_dim=stock_dim, 
                                            embedded_dim=embedded_dim, 
                                            num_layers=agent_layers, 
                                            result_dim=d_model,
                                            max_len=window_size)
        
        # 2. Encoder for global news processing (Newly added!)
        # Global news has different dimensions (e.g., 768), so match to d_model
        self.macro_proj = nn.Sequential(
            nn.Linear(embedded_dim, d_model),
            nn.ReLU(),
            nn.Linear(d_model, d_model)
        )
        
        # 3. Stock + Macro Integrated Attention
        self.stock_attention = nn.MultiheadAttention(d_model, n_head, batch_first=True)
        self.norm = nn.LayerNorm(d_model)

    def generate_causal_mask(self, sz, device):
        mask = (torch.triu(torch.ones(sz, sz, device=device)) == 1).transpose(0, 1)
        mask = mask.float().masked_fill(mask == 0, float('-inf')).masked_fill(mask == 1, float(0.0))
        return mask

    def forward(self, stock_x, news_x, global_news_x):
        # stock_x: (Batch, Num_Stocks, Time, Features)
        # global_news_x: (Batch, Time, Macro_Dim)
        
        B, N, T, F = stock_x.shape
        _, _, _, G = news_x.shape

        causal_mask = self.generate_causal_mask(T, stock_x.device) # (T, T)
        
        # --- Step 1: Individual Stock Encoding ---
        # (Batch * Num_Stocks, Time, Features) -> (Batch * Num_Stocks, d_model)
        flat_stock = stock_x.view(B * N, T, F)
        flat_news = news_x.view(B * N, T, G)
        stock_emb = self.stock_encoder(flat_stock, flat_news, causal_mask).view(B, N, -1) # (Batch, N, d_model)
        
        # --- Step 2: Global News Encoding (Create Super Token) ---
        # Global news exists only once per batch regardless of number of stocks (N), use only last time step info
        macro_token = self.macro_proj(global_news_x[:, -1, :]) # (Batch, d_model)

        
        # Match dimensions: (Batch, 1, d_model)
        macro_token = macro_token.unsqueeze(1) 
        
        # --- Step 3: Concatenation ---
        # [Global News(1), Stock1, Stock2, ..., StockN]
        # Size: (Batch, N + 1, d_model)
        combined_features = torch.cat([macro_token, stock_emb], dim=1)
        
        # --- Step 4: Self-Attention with combined tokens ---
        attn_out, _ = self.stock_attention(combined_features, combined_features, combined_features)
        
        # Residual + Norm
        combined_features = self.norm(combined_features + attn_out)
        
        # --- Step 5: Calculate Weights ---
        # Global news token is for 'reference', not a 'buy target'.
        # Therefore, discard the 0th token (news) and take only the remaining stock tokens.
        final_stock_features = combined_features[:, 1:, :] # (Batch, N, d_model)
        
        return final_stock_features