import type { ReactNode } from 'react'

/** Video takes precedence over image and loops muted without controls. */
export type Media = { caption: string; image?: string; video?: string }

/** Paragraphs beside captioned media in two columns. */
export type SplitBlock = {
  type: 'split'
  paragraphs: string[]
  media: Media
}

/** Cards are numbered by their position in the array. */
export type CardGridBlock = {
  type: 'cardGrid'
  cards: { title: string; body: string }[]
}

/** The final pipeline step is styled as success. */
export type PipelineBlock = {
  type: 'pipeline'
  title: string
  body: string
  steps: string[]
}

/** Alternating rows flip the media side on desktop. */
export type RowsBlock = {
  type: 'rows'
  rows: { eyebrow: string; title: string; body: string; media: Media }[]
}

/** Compact two-column title/body cards. */
export type MinisBlock = {
  type: 'minis'
  items: { title: string; body: string }[]
}

export type StackBlock = {
  type: 'stack'
  rows: { layer: string; items: string[] }[]
}

export type TextBlock = {
  type: 'text'
  paragraphs: string[]
}

/** Links open in a new tab; icon is an imported SVG/image URL. */
export type LinksBlock = {
  type: 'links'
  links: { label: string; href: string; icon: string; hint?: string }[]
}

/** Custom JSX is evaluated during render and revealed by default. */
type CustomBlock = {
  type: 'custom'
  render: () => ReactNode
  reveal?: boolean
  className?: string
}

export type LeafBlock =
  | SplitBlock
  | CardGridBlock
  | PipelineBlock
  | RowsBlock
  | MinisBlock
  | StackBlock
  | TextBlock
  | LinksBlock
  | CustomBlock

export type SectionBlock = {
  type: 'section'
  // Overrides the slugified eyebrow used in #hash links.
  id?: string
  eyebrow: string
  title: string
  blocks: LeafBlock[]
}

/** YouTube video; the iframe loads on click. Poster defaults to its thumbnail. */
export type VideoData = { id: string; poster?: string; title?: string }

export type LeadBlock = {
  type: 'lead'
  eyebrow: string
  title: string
  subtitle: string
  meta: { label: string; value: string }[]
  video?: VideoData
  image?: Media
}

export type ClosingBlock = {
  type: 'closing'
  text: string
}

export type ContentBlock = LeadBlock | SectionBlock | ClosingBlock | LeafBlock

export type ContentBlocks = ContentBlock[]
