import '../contentblock/diagram.css'

// Source: projects/ai/BuzzaMaker/source/{stock_news_backbone,stock_portfolio_env}.py.
const INPUTS = [
  { title: 'Stock series', dim: 'N × 60 × F', enc: 'OHLCV window · Linear→256' },
  { title: 'Per-stock news', dim: 'N × 60 × 768', enc: 'Gemini emb · Linear·ReLU·Linear' },
  { title: 'Macro news', dim: '60 × 768', enc: 'global headlines · Linear·ReLU·Linear' },
]

export default function BuzzaMakerArchDiagram() {
  return (
    <figure className="cb__dg">
      <figcaption className="cb__dg-cap">
        A dual-stream Transformer (d_model 256 · 4 heads · 2 layers) reads each stock&apos;s price
        window beside its news, fuses cross-asset and macro context, and a PPO head turns the
        result into portfolio weights — built per stock, then aggregated.
      </figcaption>

      <div className="cb__dg-inputs cb__dg-inputs--3">
        {INPUTS.map((i) => (
          <div className="cb__dg-input" key={i.title}>
            <span className="cb__dg-input-title">{i.title}</span>
            <span className="cb__dg-input-dim">{i.dim}</span>
            <span className="cb__dg-input-enc">{i.enc}</span>
          </div>
        ))}
      </div>

      <span className="cb__dg-link" />

      <div className="cb__dg-group">
        <span className="cb__dg-badge">× 2 layers</span>
        <div className="cb__dg-row">
          <div className="cb__dg-node">
            <div className="cb__dg-node-title">Stock self-attn</div>
            <div className="cb__dg-node-sub">causal · MHA</div>
          </div>
          <div className="cb__dg-node">
            <div className="cb__dg-node-title">News self-attn</div>
            <div className="cb__dg-node-sub">causal · MHA</div>
          </div>
        </div>
        <div className="cb__dg-tag">cross-attn · stock ← news · gate → 0</div>
        <div className="cb__dg-row">
          <div className="cb__dg-node">
            <div className="cb__dg-node-title">Stock FFN</div>
            <div className="cb__dg-node-sub">256 → 1024 → 256 · GELU</div>
          </div>
          <div className="cb__dg-node">
            <div className="cb__dg-node-title">News FFN</div>
            <div className="cb__dg-node-sub">256 → 1024 → 256 · GELU</div>
          </div>
        </div>
      </div>

      <span className="cb__dg-link" />
      <div className="cb__dg-node">
        <div className="cb__dg-node-title">Last-step token</div>
        <div className="cb__dg-node-sub">per stock · 256-D</div>
      </div>

      <span className="cb__dg-link" />
      <div className="cb__dg-node">
        <div className="cb__dg-node-title">Portfolio attention</div>
        <div className="cb__dg-node-sub">concat [ macro, stock₁ … stockₙ ] · MHA → drop macro</div>
      </div>

      <span className="cb__dg-link" />
      <div className="cb__dg-out">
        <div className="cb__dg-node-title">PPO policy</div>
        <div className="cb__dg-node-sub">logits N+1 → softmax weights (+ cash) · reward log(1 + net − cost)</div>
      </div>
    </figure>
  )
}
