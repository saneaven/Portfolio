import type { ContentBlocks } from './types'

export type SectionAnchor = { id: string; label: string; index: number }

function slugify(text: string) {
  return text
    .normalize('NFKD')
    .toLowerCase()
    .replace(/[^a-z0-9]+/g, '-')
    .replace(/^-+|-+$/g, '')
}

/** Duplicate labels receive -2/-3 suffixes so every section remains linkable. */
export function sectionAnchors(blocks: ContentBlocks): SectionAnchor[] {
  const seen = new Map<string, number>()
  const anchors: SectionAnchor[] = []
  blocks.forEach((block, index) => {
    if (block.type !== 'section') return
    const base = block.id ?? slugify(block.eyebrow)
    const count = (seen.get(base) ?? 0) + 1
    seen.set(base, count)
    anchors.push({ id: count > 1 ? `${base}-${count}` : base, label: block.eyebrow, index })
  })
  return anchors
}
