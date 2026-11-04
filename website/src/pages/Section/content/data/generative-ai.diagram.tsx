import '../contentblock/diagram.css'

// Source: projects/ai/GenAI-Learning/Module7/Hands_on/train-cuda.ipynb.
export default function GenerativeAiArchDiagram() {
  return (
    <figure className="cb__dg">
      <figcaption className="cb__dg-cap">
        The capstone fine-tunes a <strong>frozen, 4-bit Qwen3-1.7B</strong> with low-rank LoRA
        adapters on its attention projections — only the adapters train — then merges them back
        and ships the weights to the Hub.
      </figcaption>

      <div className="cb__dg-node">
        <div className="cb__dg-node-title">Tokenized prompt</div>
        <div className="cb__dg-node-sub">Jinja2 templates · max-seq 512</div>
      </div>
      <span className="cb__dg-link" />

      <div className="cb__dg-group">
        <span className="cb__dg-badge">× N decoder blocks · Qwen3-1.7B</span>
        <div className="cb__dg-row">
          <div className="cb__dg-node">
            <div className="cb__dg-node-title">Frozen attention</div>
            <div className="cb__dg-node-sub">q · k · v · o — 4-bit nf4</div>
          </div>
          <div className="cb__dg-node cb__dg-node--alt">
            <div className="cb__dg-node-title">LoRA A · B</div>
            <div className="cb__dg-node-sub">rank 16 · α 32 · dropout 0.05 · trainable</div>
          </div>
        </div>
        <div className="cb__dg-tag">adapter ⊕ base on q / k / v / o</div>
        <div className="cb__dg-node">
          <div className="cb__dg-node-title">Frozen MLP</div>
          <div className="cb__dg-node-sub">4-bit nf4 · bf16 compute</div>
        </div>
      </div>

      <span className="cb__dg-link" />
      <div className="cb__dg-node">
        <div className="cb__dg-node-title">SFTTrainer (TRL)</div>
        <div className="cb__dg-node-sub">only the adapters update · Wandb-tracked</div>
      </div>

      <span className="cb__dg-link" />
      <div className="cb__dg-node">
        <div className="cb__dg-node-title">merge_and_unload</div>
        <div className="cb__dg-node-sub">fold adapters into base weights</div>
      </div>

      <span className="cb__dg-link" />
      <div className="cb__dg-out">
        <div className="cb__dg-node-title">Push to HuggingFace Hub</div>
        <div className="cb__dg-node-sub">merged checkpoint</div>
      </div>
    </figure>
  )
}
