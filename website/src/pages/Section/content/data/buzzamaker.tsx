import type { ContentBlocks } from '../contentblock/types'
import githubIcon from '../../../../assets/github-white-icon.svg'
import BuzzaMakerArchDiagram from './buzzamaker.diagram'

const buzzamaker: ContentBlocks = [
  {
    type: 'lead',
    eyebrow: 'AI · Reinforcement Learning',
    title: 'BuzzaMaker',
    subtitle:
      'BuzzaMaker encodes a stock\'s price history, its own news and broad macro news with a dual-stream Transformer, then lets a PPO agent decide how much of each stock — and cash — to hold every step, optimising log return after trading costs. Built on a real data pipeline that scrapes prices, crawls news and embeds it end to end.',
    meta: [
      { label: 'Role', value: 'Solo — design, data pipeline, model & training' },
      { label: 'Focus', value: 'Deep RL · multi-modal Transformer' },
      { label: 'Stack', value: 'PyTorch · Stable-Baselines3 PPO · Gymnasium · Gemini embeddings' },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Overview',
    title: 'The market is unpredictable — so let a model learn to act in it',
    blocks: [
      {
        type: 'text',
        paragraphs: [
          'Markets carry too many variables and too many unforeseeable shocks to script a strategy by hand. BuzzaMaker reframes the problem as one a model can learn: understand the recent past as a time series, then take an action, observe the outcome, and improve.',
          'A Transformer reads each stock\'s price window together with the news that surrounds it — both the stock\'s own headlines and the macro backdrop — and a PPO agent turns that understanding into portfolio weights. The reward is the realised log return net of trading cost, so the policy is pushed toward risk-adjusted gains rather than raw bets.',
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Core — Model Architecture',
    title: 'A dual-stream Transformer that learns how much to trust the news',
    blocks: [
      {
        type: 'cardGrid',
        cards: [
          {
            title: 'Dual-stream Transformer',
            body: 'Each StockNewsTransformerLayer runs self-attention over the price series and self-attention over the news embeddings separately, then a stock→news cross-attention lets price queries attend to the surrounding headlines. Stock and news each get their own 4× GELU feed-forward block.',
          },
          {
            title: 'Learnable news gate',
            body: 'Cross-attention is scaled by a learnable gate initialised to zero, so the model starts ignoring news and gradually learns how much to trust it during training — a built-in curriculum that keeps early training stable.',
          },
          {
            title: 'Portfolio-level aggregation',
            body: 'PortfolioMaker runs a StockNewsAgent per stock, then builds a macro token from global news and runs self-attention over [macro, stock₁ … stockₙ] so cross-asset and macro context shape every weight before the policy head reads them.',
          },
          {
            title: 'No future leakage',
            body: 'Causal masking on the price attention and learnable positional embeddings over a 60-day window keep the encoder strictly backward-looking — the model can only ever attend to information it would actually have had.',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Reinforcement Learning',
    title: 'A PPO agent that allocates capital every step',
    blocks: [
      {
        type: 'custom',
        render: () => <BuzzaMakerArchDiagram />,
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Data Pipeline',
    title: 'Real prices and real news, scraped and embedded from scratch',
    blocks: [
      {
        type: 'minis',
        items: [
          {
            title: 'Stock prices',
            body: 'OHLCV history pulled from Yahoo Finance via yfinance — 2017–2024 for training and early 2025 held out for testing (AAPL, given the cost of broader collection).',
          },
          {
            title: 'Stock-specific news',
            body: 'GDELT DOC 2.0 queried in 7-day rolling windows, articles fetched with the newspaper library, deduplicated by URL and parsed to text.',
          },
          {
            title: 'Macro news',
            body: 'Five themed keyword sets (politics, economy, technology, society, environment) scraped concurrently through a ThreadPoolExecutor with retry/backoff and resumable date logging.',
          },
          {
            title: 'Embeddings',
            body: 'Every article title embedded with Google Gemini (gemini-embedding-001) into 768-d vectors, batched and resumable so a stopped run picks up where it left off.',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Stack',
    title: 'What it\'s built on',
    blocks: [
      {
        type: 'stack',
        rows: [
          { layer: 'Modeling', items: ['PyTorch', 'Multi-head attention', 'Gated cross-attention', 'Learnable positional embedding'] },
          { layer: 'Reinforcement learning', items: ['Stable-Baselines3 PPO', 'Gymnasium', 'MultiInputPolicy', 'Custom feature extractor'] },
          { layer: 'Data', items: ['yfinance', 'GDELT DOC 2.0', 'newspaper', 'pandas / numpy'] },
          { layer: 'Embeddings', items: ['Google Gemini (genai)', '768-d vectors', 'Batched & resumable'] },
          { layer: 'Infra', items: ['Python 3.12', 'python-dotenv', 'ThreadPoolExecutor'] },
        ],
      },
    ],
  },
  {
    type: 'links',
    links: [
      {
        label: 'Source on GitHub',
        href: 'https://github.com/saneaven/Portfolio/tree/main/projects/ai/BuzzaMaker',
        icon: githubIcon,
        hint: 'Code & technical docs',
      },
    ],
  },
  {
    type: 'closing',
    text: 'Designed and built end to end — the scraping and embedding pipeline, the multi-modal Transformer encoder, and the reinforcement-learning portfolio policy.',
  },
]

export default buzzamaker
