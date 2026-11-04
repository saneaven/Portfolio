import { Fragment } from 'react'
import '../contentblock/diagram.css'

// Sources under projects/ai/DL-Learning: PrgHW1/nn_scratch.py,
// PrgHW2/Prg2.ipynb and prgHW3/prg3.ipynb.
type Stage = { t: string; s?: string }

const STUDIES: { head: string; note: string; stages: Stage[] }[] = [
  {
    head: 'From scratch',
    note: 'He init · hand-written backprop · learns f(x, y) = x · y',
    stages: [
      { t: 'Input', s: '2' },
      { t: 'Dense 16', s: 'ReLU' },
      { t: 'Dense 16', s: 'ReLU' },
      { t: 'Output', s: '1' },
    ],
  },
  {
    head: 'CNN · transfer',
    note: 'from-scratch CNN, then a frozen ResNet50V2 backbone → GAP → head (~95%)',
    stages: [
      { t: 'Conv 32', s: '3×3 · maxpool' },
      { t: 'Conv 64', s: '3×3 · maxpool' },
      { t: 'Conv 128', s: '3×3 · maxpool' },
      { t: 'Flatten · Dense 256', s: 'ReLU' },
      { t: 'Dense 10', s: 'softmax' },
    ],
  },
  {
    head: 'Double DQN',
    note: 'online + target nets · soft sync τ=0.005 · 500k replay · Huber loss',
    stages: [
      { t: 'Grid state', s: 'rows × cols × 2' },
      { t: 'Conv 64', s: '3×3' },
      { t: 'Conv 128', s: '3×3' },
      { t: 'Conv 256', s: '3×3' },
      { t: 'Flatten · Dense 256', s: 'ReLU' },
      { t: 'Q-values', s: 'Dense 4 · linear' },
    ],
  },
]

export default function DeepLearningArchDiagram() {
  return (
    <figure className="cb__dg">
      <figcaption className="cb__dg-cap">
        Three studies up the ladder: a feed-forward net built by hand in NumPy, convolutional
        stacks ending in transfer learning, and a Double DQN agent — each shown as its actual
        layer structure.
      </figcaption>

      <div className="cb__dg-cols">
        {STUDIES.map((study) => (
          <div className="cb__dg-col" key={study.head}>
            <div className="cb__dg-col-head">{study.head}</div>
            {study.stages.map((stage, i) => (
              <Fragment key={stage.t}>
                {i > 0 && <span className="cb__dg-link cb__dg-link--sm" />}
                <div className={i === study.stages.length - 1 ? 'cb__dg-out' : 'cb__dg-node'}>
                  <div className="cb__dg-node-title">{stage.t}</div>
                  {stage.s && <div className="cb__dg-node-sub">{stage.s}</div>}
                </div>
              </Fragment>
            ))}
            <div className="cb__dg-col-note">{study.note}</div>
          </div>
        ))}
      </div>
    </figure>
  )
}
