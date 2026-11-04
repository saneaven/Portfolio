import '../contentblock/diagram.css'

// Source: Jamo-basedKoreanTokenization/src/translation/{model,train}.py
// and src/tokenizer/hf_jamo_bpe.py in projects/ai/JamoBased Tokenization Translator.
export default function JamoArchDiagram() {
  return (
    <figure className="cb__dg">
      <figcaption className="cb__dg-cap">
        Korean is split into jamo and BPE-merged, then an identical ~89M-param encoder–decoder
        Transformer (d_model 512 · 8 heads · 4 + 4 layers · FFN 1024) translates to English — only
        the Korean tokenizer changes between runs.
      </figcaption>

      <div className="cb__dg-node">
        <div className="cb__dg-node-title">Jamo decomposition</div>
        <div className="cb__dg-node-sub">강 → ㄱ ㅏ ㅇ · reversible (kss h2hcj)</div>
      </div>
      <span className="cb__dg-link" />
      <div className="cb__dg-node">
        <div className="cb__dg-node-title">HFJamoBPE</div>
        <div className="cb__dg-node-sub">vocab 32,768 / 16,384 · end-of-word &quot;&lt;/w&gt;&quot;</div>
      </div>
      <span className="cb__dg-link" />

      <div className="cb__dg-cols">
        <div className="cb__dg-col">
          <div className="cb__dg-col-head">Encoder</div>
          <div className="cb__dg-node">
            <div className="cb__dg-node-title">src emb × √512</div>
            <div className="cb__dg-node-sub">+ sinusoidal PE</div>
          </div>
          <span className="cb__dg-link cb__dg-link--sm" />
          <div className="cb__dg-group">
            <span className="cb__dg-badge">× 4 layers</span>
            <div className="cb__dg-node">
              <div className="cb__dg-node-title">Self-attn · FFN</div>
              <div className="cb__dg-node-sub">8 heads · 512 / 1024</div>
            </div>
          </div>
          <div className="cb__dg-col-note">→ memory</div>
        </div>

        <div className="cb__dg-col">
          <div className="cb__dg-col-head">Decoder</div>
          <div className="cb__dg-node">
            <div className="cb__dg-node-title">tgt emb + PE</div>
            <div className="cb__dg-node-sub">GPT-2 tokenizer (~50k)</div>
          </div>
          <span className="cb__dg-link cb__dg-link--sm" />
          <div className="cb__dg-group">
            <span className="cb__dg-badge">× 4 layers</span>
            <div className="cb__dg-node">
              <div className="cb__dg-node-title">Masked self-attn</div>
              <div className="cb__dg-node-sub">causal</div>
            </div>
            <div className="cb__dg-tag">cross-attn ← memory</div>
            <div className="cb__dg-node">
              <div className="cb__dg-node-title">FFN</div>
              <div className="cb__dg-node-sub">512 / 1024</div>
            </div>
          </div>
          <div className="cb__dg-col-note">→ generator</div>
        </div>
      </div>

      <span className="cb__dg-link" />
      <div className="cb__dg-node">
        <div className="cb__dg-node-title">Generator</div>
        <div className="cb__dg-node-sub">Linear → EN vocab</div>
      </div>
      <span className="cb__dg-link" />
      <div className="cb__dg-out">
        <div className="cb__dg-node-title">Greedy decode → English</div>
        <div className="cb__dg-node-sub">AdamW · inv-sqrt warmup 1k · label-smooth 0.1 · bf16</div>
      </div>
    </figure>
  )
}
