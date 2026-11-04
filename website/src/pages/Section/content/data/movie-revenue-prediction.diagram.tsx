import '../contentblock/diagram.css'

// Source: projects/ai/Movie Revenue Prediction/project_moviemovie_minjae.ipynb.
const FEATURES = [
  { title: 'Categoricals', dim: 'multi-hot', enc: 'genres · languages → [-1, 1]' },
  { title: 'Release date', dim: 'cyclic', enc: 'month/day sin · cos' },
  { title: 'Title + overview', dim: 'word emb', enc: 'pre-trained embeddings' },
  { title: 'Numerics', dim: 'scaled', enc: 'budget · votes · popularity' },
]

export default function MovieRevenueArchDiagram() {
  return (
    <figure className="cb__dg">
      <figcaption className="cb__dg-cap">
        Movie metadata is engineered into one feature vector, then a three-layer MLP and a linear
        baseline predict revenue from the same inputs — the comparison is the point.
      </figcaption>

      <div className="cb__dg-inputs">
        {FEATURES.map((f) => (
          <div className="cb__dg-input" key={f.title}>
            <span className="cb__dg-input-title">{f.title}</span>
            <span className="cb__dg-input-dim">{f.dim}</span>
            <span className="cb__dg-input-enc">{f.enc}</span>
          </div>
        ))}
      </div>

      <span className="cb__dg-link" />
      <div className="cb__dg-node">
        <div className="cb__dg-node-title">Engineered feature vector</div>
        <div className="cb__dg-node-sub">concatenated · standardised</div>
      </div>
      <span className="cb__dg-link" />

      <div className="cb__dg-cols">
        <div className="cb__dg-col">
          <div className="cb__dg-col-head">MLP regressor</div>
          <div className="cb__dg-node">
            <div className="cb__dg-node-title">Dense 100</div>
            <div className="cb__dg-node-sub">ReLU</div>
          </div>
          <span className="cb__dg-link cb__dg-link--sm" />
          <div className="cb__dg-node">
            <div className="cb__dg-node-title">Dense 100</div>
            <div className="cb__dg-node-sub">ReLU</div>
          </div>
          <span className="cb__dg-link cb__dg-link--sm" />
          <div className="cb__dg-node">
            <div className="cb__dg-node-title">Dense 100</div>
            <div className="cb__dg-node-sub">ReLU</div>
          </div>
          <div className="cb__dg-col-note">Adam · early stopping · R² ≈ 0.84 (+ overview emb)</div>
        </div>

        <div className="cb__dg-col">
          <div className="cb__dg-col-head">Linear baseline</div>
          <div className="cb__dg-node cb__dg-node--alt">
            <div className="cb__dg-node-title">LinearRegression</div>
            <div className="cb__dg-node-sub">one weight per feature</div>
          </div>
          <div className="cb__dg-col-note">R² ≈ 0.66 · most robust on outlier-heavy data</div>
        </div>
      </div>

      <span className="cb__dg-link" />
      <div className="cb__dg-out">
        <div className="cb__dg-node-title">Predicted revenue</div>
        <div className="cb__dg-node-sub">80 / 20 split + outlier benchmark set</div>
      </div>
    </figure>
  )
}
