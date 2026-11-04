import '../contentblock/diagram.css'

// Sources under projects/ai/NovelAnalyzer: prompts/{orchestrator,shard_filler}.md,
// templates/*.j2, scripts/gemma4_adapter.py and configs/*_qlora_*.yaml.
export default function NovelBudsAgentDiagram() {
  return (
    <figure className="cb__dg">
      <figcaption className="cb__dg-cap">
        A multi-agent data factory turns public-domain novels into agent conversations, a
        from-scratch renderer casts them into Gemma 4&apos;s training format, and a single-epoch
        QLoRA pass fine-tunes the 31B model — then ships it in three forms.
      </figcaption>

      <div className="cb__dg-node">
        <div className="cb__dg-node-title">Project Gutenberg corpus</div>
        <div className="cb__dg-node-sub">Despina collection · public-domain fiction</div>
      </div>
      <span className="cb__dg-link" />

      <div className="cb__dg-group">
        <span className="cb__dg-badge">orchestrator + parallel sub-agents</span>
        <div className="cb__dg-row">
          <div className="cb__dg-node">
            <div className="cb__dg-node-title">Novel sub-agent ×N</div>
            <div className="cb__dg-node-sub">one novel each · [ ] / [-] / [x] checklist</div>
          </div>
          <div className="cb__dg-node cb__dg-node--alt">
            <div className="cb__dg-node-title">Typed extraction</div>
            <div className="cb__dg-node-sub">OpenAI + Gemini · Pydantic schemas</div>
          </div>
        </div>
        <div className="cb__dg-tag">→ structured project · characters · locations · lore · outline</div>
      </div>

      <span className="cb__dg-link" />
      <div className="cb__dg-node">
        <div className="cb__dg-node-title">40+ Jinja2 scenario templates</div>
        <div className="cb__dg-node-sub">render → ~500 skeletal shards · placeholders</div>
      </div>

      <span className="cb__dg-link" />

      <div className="cb__dg-group">
        <span className="cb__dg-badge">orchestrator + parallel sub-agents</span>
        <div className="cb__dg-node">
          <div className="cb__dg-node-title">Shard-filler sub-agents</div>
          <div className="cb__dg-node-sub">thinking blocks · in-voice prose · realistic tool results</div>
        </div>
        <div className="cb__dg-tag">→ complete agent conversations · train / validation / test</div>
      </div>

      <span className="cb__dg-link" />
      <div className="cb__dg-node cb__dg-node--alt">
        <div className="cb__dg-node-title">Custom Gemma 4 renderer</div>
        <div className="cb__dg-node-sub">gemma4_adapter.py · turn · channel · tool stream · thinking kept</div>
      </div>

      <span className="cb__dg-link" />
      <div className="cb__dg-node">
        <div className="cb__dg-node-title">QLoRA fine-tune · Gemma 4 31B</div>
        <div className="cb__dg-node-sub">Unsloth 4-bit · LoRA r16 / α16 · 1 epoch · RTX PRO 6000</div>
      </div>

      <span className="cb__dg-link" />
      <div className="cb__dg-out">
        <div className="cb__dg-node-title">Push to HuggingFace Hub</div>
        <div className="cb__dg-node-sub">merged · qlora · gguf</div>
      </div>
    </figure>
  )
}
