import '../contentblock/diagram.css'

const INPUTS = [
  { title: 'Proprioception', dim: '~100D', enc: 'MLP' },
  { title: 'Heightmap', dim: '1×64×64', enc: 'ResBlock CNN' },
  { title: 'BEV', dim: '3×64×64', enc: 'ResBlock CNN' },
  { title: 'Nav grid', dim: '1×33×33', enc: 'ResBlock CNN' },
]

export default function SpiderBotArchDiagram() {
  return (
    <figure className="cb__dg">
      <figcaption className="cb__dg-cap">
        SharedRecurrentModel — four multi-modal encoders fuse to 320-D, run through a GRU for temporal
        context, then a shared trunk branches into policy and value heads.
      </figcaption>

      <div className="cb__dg-inputs">
        {INPUTS.map((i) => (
          <div className="cb__dg-input" key={i.title}>
            <span className="cb__dg-input-title">{i.title}</span>
            <span className="cb__dg-input-dim">{i.dim}</span>
            <span className="cb__dg-input-enc">{i.enc}</span>
          </div>
        ))}
      </div>

      <span className="cb__dg-link" />
      <div className="cb__dg-node">
        <div className="cb__dg-node-title">Concat</div>
        <div className="cb__dg-node-sub">320D fusion</div>
      </div>

      <span className="cb__dg-link" />
      <div className="cb__dg-node">
        <div className="cb__dg-node-title">GRU</div>
        <div className="cb__dg-node-sub">in 320 · hidden 512</div>
        <div className="cb__dg-recur">h(t-1) ↻ h(t) · 32-step sequences</div>
      </div>

      <span className="cb__dg-link" />
      <div className="cb__dg-node">
        <div className="cb__dg-node-title">Shared trunk</div>
        <div className="cb__dg-node-sub">512 → 256 → 128</div>
      </div>

      <span className="cb__dg-fork" />
      <div className="cb__dg-heads">
        <div className="cb__dg-head">
          <div className="cb__dg-node-title">Policy head</div>
          <div className="cb__dg-node-sub">24 joint targets · tanh</div>
        </div>
        <div className="cb__dg-head">
          <div className="cb__dg-node-title">Value head</div>
          <div className="cb__dg-node-sub">V(s)</div>
        </div>
      </div>
    </figure>
  )
}
