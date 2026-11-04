import type { ContentBlocks } from '../contentblock/types'
import githubIcon from '../../../../assets/github-white-icon.svg'
import JamoArchDiagram from './jamo-tokenization-translator.diagram'

const jamoTokenizationTranslator: ContentBlocks = [
  {
    type: 'lead',
    eyebrow: 'AI · NLP',
    title: 'Jamo Tokenization Translator',
    subtitle:
      'Korean morpheme boundaries often fall inside a syllable, where character-level BPE can\'t cut. This project decomposes each syllable into its jamo (initial, medial, final) before applying BPE — yielding fewer, more morphologically aligned tokens — and measures the effect by training an identical Korean→English Seq2Seq Transformer on each tokenizer.',
    meta: [
      { label: 'Role', value: 'Main researcher & implementer (co-author: Ju-ve Chankasemporn)' },
      { label: 'Focus', value: 'Korean tokenization · Seq2Seq translation' },
      { label: 'Stack', value: 'PyTorch · HuggingFace tokenizers · kss/jamo · Streamlit' },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Motivation',
    title: 'The morpheme boundary lives inside the syllable',
    blocks: [
      {
        type: 'text',
        paragraphs: [
          'Standard tokenizers — BPE, WordPiece, SentencePiece — treat the character or byte as the smallest unit. Korean breaks that assumption: in 노랫말 (lyrics) the interfix ㅅ sits inside the syllable 랫, and in 먹었어요 the past-tense ㅆ is fused into 었. A character tokenizer can never split those apart.',
          'The hypothesis: decompose each Hangul syllable into its jamo first, then let BPE merge. Sub-syllable structure becomes visible, so the tokenizer can land on morpheme-aligned pieces and represent the language with fewer tokens — especially at small vocabularies and on out-of-domain text.',
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Core — Tokenization',
    title: 'Two tokenizers, one idea: decompose, then merge',
    blocks: [
      {
        type: 'cardGrid',
        cards: [
          {
            title: 'Reversible jamo decomposition',
            body: 'Hangul syllables (U+AC00–U+D7A3) decompose by S = ((init × 21) + med) × 28 + fin + 0xAC00. Using kss\'s h2hcj / hcj2h, text round-trips losslessly between syllables and compatibility jamo, so any tokenization is fully reversible.',
          },
          {
            title: 'JamoBPE — from scratch',
            body: 'A pure-Python BPE: build word-frequency tuples, count adjacent pair statistics, merge the most frequent pair, repeat until the vocab target or a min-frequency floor. Written for conceptual clarity over the whole train→merge loop.',
          },
          {
            title: 'HFJamoBPE — production',
            body: 'A HuggingFace tokenizers build on the Rust backend with end_of_word_suffix "</w>", trained on millions of CC-100 Korean lines at 32k and 16k vocabularies — the version used for the real experiments.',
          },
          {
            title: 'Lossless decoding',
            body: 'Decoding rejoins tokens, turns </w> back into spaces, and reconstructs Hangul with hcj2h() — so the jamo round-trip is invisible to anything downstream.',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Translation Model',
    title: 'An identical Transformer behind each tokenizer',
    blocks: [
      {
        type: 'custom',
        render: () => <JamoArchDiagram />,
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Findings',
    title: 'Clear token savings, an honest downstream caveat',
    blocks: [
      {
        type: 'minis',
        items: [
          {
            title: 'Fewer tokens off-domain',
            body: 'On small or domain-mismatched text, jamo uses 30–50% fewer tokens than character BPE — BTS\'s 봄날 came to 530 tokens vs 953 — with the gap converging to ~1.6% on large general CC-100 text.',
          },
          {
            title: 'Tighter at small vocab',
            body: 'At a 16k vocabulary, jamo averaged ~5.5% fewer tokens and showed lower variance, so the representation is both shorter and more consistent.',
          },
          {
            title: 'Morpheme-aligned splits',
            body: 'Qualitatively, jamo segmentation exposes morpheme boundaries inside syllables (interfix ㅅ, past-tense ㅆ) that character BPE structurally cannot reach.',
          },
          {
            title: 'No free lunch downstream',
            body: 'At this model scale the jamo and character training-loss curves were nearly identical (<1%) — the efficiency gain did not translate into measurably better translation, stated plainly as a limitation.',
          },
        ],
      },
    ],
  },
  {
    type: 'section',
    eyebrow: 'Demo',
    title: 'Tokenize and translate, live',
    blocks: [
      {
        type: 'text',
        paragraphs: [
          'A Streamlit app lets you pick a dataset (small / large / large-16k) and a mode (jamo / full character), type Korean, and see the tokenization and token count side by side — with live Korean→English translation on the large models. It made the comparison between approaches something you can feel rather than just read off a table.',
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
          { layer: 'Tokenization', items: ['jamo', 'kss', 'HuggingFace tokenizers', 'tiktoken (baseline)'] },
          { layer: 'Modeling', items: ['PyTorch', 'Seq2Seq Transformer', 'Sinusoidal PE', 'Greedy decode'] },
          { layer: 'Data', items: ['CC-100 (ko)', 'AI Hub ko–en', 'datasets'] },
          { layer: 'Demo & infra', items: ['Streamlit', 'Python 3.12', 'bfloat16'] },
        ],
      },
    ],
  },
  {
    type: 'links',
    links: [
      {
        label: 'Source on GitHub',
        href: 'https://github.com/saneaven/Portfolio/tree/main/projects/ai/JamoBased%20Tokenization%20Translator',
        icon: githubIcon,
        hint: 'Code & technical docs',
      },
    ],
  },
  {
    type: 'closing',
    text: 'Carried end to end — the tokenizer design, the translation model and training, the quantitative and qualitative evaluation, and the live demo — to probe an underexplored direction in Korean tokenization.',
  },
]

export default jamoTokenizationTranslator
