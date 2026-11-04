import { useState, type CSSProperties } from 'react'
import { Reveal, SectionHead, Media } from './primitives'
import { siteHref } from '../../../../data/routes'
import type {
  CardGridBlock,
  ClosingBlock,
  LeadBlock,
  LeafBlock,
  LinksBlock,
  MinisBlock,
  PipelineBlock,
  RowsBlock,
  SectionBlock,
  SplitBlock,
  StackBlock,
  TextBlock,
  VideoData,
} from './types'

function VideoEmbed({ id, poster, title }: VideoData) {
  const [playing, setPlaying] = useState(false)
  const thumb = poster ?? `https://img.youtube.com/vi/${id}/maxresdefault.jpg`
  return (
    <div className="cb__video">
      {playing ? (
        <iframe
          className="cb__video-frame"
          src={`https://www.youtube-nocookie.com/embed/${id}?autoplay=1`}
          title={title ?? 'Trailer'}
          allow="autoplay; encrypted-media; fullscreen; picture-in-picture"
          allowFullScreen
        />
      ) : (
        <button
          type="button"
          className="cb__video-poster"
          style={{ backgroundImage: `url(${thumb})` }}
          onClick={() => setPlaying(true)}
          aria-label={title ? `Play ${title}` : 'Play trailer'}
        >
          <span className="cb__video-play" aria-hidden="true" />
        </button>
      )}
    </div>
  )
}

export function Lead({ eyebrow, title, subtitle, meta, video, image }: LeadBlock) {
  return (
    <Reveal className="cb__lead">
      <span className="cb__eyebrow">{eyebrow}</span>
      <p className="cb__lead-title">{title}</p>
      {video && <VideoEmbed {...video} />}
      {image && <Media {...image} />}
      <p className="cb__lead-sub">{subtitle}</p>
      <dl className="cb__meta">
        {meta.map((m) => (
          <div className="cb__meta-item" key={m.label}>
            <dt className="cb__meta-label">{m.label}</dt>
            <dd className="cb__meta-value">{m.value}</dd>
          </div>
        ))}
      </dl>
    </Reveal>
  )
}

export function SectionGroup({
  id,
  eyebrow,
  title,
  blocks,
  renderLeaf,
}: SectionBlock & { renderLeaf: (block: LeafBlock, key: number) => React.ReactNode }) {
  // Anchor jumps must measure the resting section, outside Reveal's transform.
  return (
    <section className="cb__section" id={id}>
      <Reveal>
        <SectionHead eyebrow={eyebrow} title={title} />
      </Reveal>
      {blocks.map((b, i) => renderLeaf(b, i))}
    </section>
  )
}

export function Split({ paragraphs, media }: SplitBlock) {
  return (
    <div className="cb__split">
      <Reveal className="cb__split-text">
        {paragraphs.map((p, i) => (
          <p key={i}>{p}</p>
        ))}
      </Reveal>
      <Reveal className="cb__split-media" delay={0.08}>
        <Media {...media} />
      </Reveal>
    </div>
  )
}

export function CardGrid({ cards }: CardGridBlock) {
  return (
    <div className="cb__grid">
      {cards.map((c, i) => (
        <Reveal className="cb__card" key={c.title} delay={(i % 2) * 0.08}>
          <span className="cb__card-index">{String(i + 1).padStart(2, '0')}</span>
          <h3 className="cb__card-title">{c.title}</h3>
          <p className="cb__card-body">{c.body}</p>
        </Reveal>
      ))}
    </div>
  )
}

export function Pipeline({ title, body, steps }: PipelineBlock) {
  return (
    <Reveal className="cb__hitl">
      <div className="cb__hitl-text">
        <h3 className="cb__card-title">{title}</h3>
        <p className="cb__card-body">{body}</p>
      </div>
      <ol className="cb__pipe">
        {steps.map((s) => (
          <li className="cb__pipe-step" key={s}>
            {s}
          </li>
        ))}
      </ol>
    </Reveal>
  )
}

export function Rows({ rows }: RowsBlock) {
  return (
    <div className="cb__rows">
      {rows.map((r, i) => (
        <Reveal className={i % 2 ? 'cb__row cb__row--flip' : 'cb__row'} key={r.title}>
          <div className="cb__row-text">
            <span className="cb__row-eyebrow">{r.eyebrow}</span>
            <h3 className="cb__card-title">{r.title}</h3>
            <p className="cb__card-body">{r.body}</p>
          </div>
          <Media {...r.media} />
        </Reveal>
      ))}
    </div>
  )
}

export function Minis({ items }: MinisBlock) {
  return (
    <div className="cb__minis">
      {items.map((m) => (
        <Reveal className="cb__mini" key={m.title}>
          <h3 className="cb__card-title">{m.title}</h3>
          <p className="cb__card-body">{m.body}</p>
        </Reveal>
      ))}
    </div>
  )
}

export function Stack({ rows }: StackBlock) {
  return (
    <dl className="cb__stack">
      {rows.map((row) => (
        <Reveal className="cb__stack-row" key={row.layer}>
          <dt className="cb__stack-layer">{row.layer}</dt>
          <dd className="cb__stack-items">
            {row.items.map((it) => (
              <span className="cb__chip" key={it}>
                {it}
              </span>
            ))}
          </dd>
        </Reveal>
      ))}
    </dl>
  )
}

export function Text({ paragraphs }: TextBlock) {
  return (
    <Reveal>
      {paragraphs.map((p, i) => (
        <p className="cb__text" key={i}>
          {p}
        </p>
      ))}
    </Reveal>
  )
}

export function Links({ links }: LinksBlock) {
  return (
    <Reveal className="cb__links">
      {links.map((l) => (
        <a className="cb__link" key={l.href} href={l.href.startsWith('/') ? siteHref(l.href) : l.href} target="_blank" rel="noopener noreferrer">
          {/* Quotes keep inlined SVG data URLs valid inside url(). */}
          <span className="cb__link-icon" style={{ '--cb-icon': `url("${l.icon}")` } as CSSProperties} aria-hidden="true" />
          <span className="cb__link-label">{l.label}</span>
          {l.hint && <span className="cb__link-hint">{l.hint}</span>}
          <span className="cb__link-arrow" aria-hidden="true">↗</span>
        </a>
      ))}
    </Reveal>
  )
}

export function Closing({ text }: ClosingBlock) {
  return (
    <Reveal className="cb__closing">
      <p>{text}</p>
    </Reveal>
  )
}
