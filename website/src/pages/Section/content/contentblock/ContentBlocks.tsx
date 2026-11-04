import type { CSSProperties, ReactNode } from 'react'
import type { ContentBlocks as ContentBlocksData, ContentBlock, LeafBlock } from './types'
import { Reveal } from './primitives'
import { sectionAnchors } from './anchors'
import {
  CardGrid,
  Closing,
  Lead,
  Links,
  Minis,
  Pipeline,
  Rows,
  SectionGroup,
  Split,
  Stack,
  Text,
} from './blocks'
import './contentblock.css'

type Gradient = { from: string; to: string }

function renderLeaf(block: LeafBlock, key: number): ReactNode {
  switch (block.type) {
    case 'split':
      return <Split key={key} {...block} />
    case 'cardGrid':
      return <CardGrid key={key} {...block} />
    case 'pipeline':
      return <Pipeline key={key} {...block} />
    case 'rows':
      return <Rows key={key} {...block} />
    case 'minis':
      return <Minis key={key} {...block} />
    case 'stack':
      return <Stack key={key} {...block} />
    case 'text':
      return <Text key={key} {...block} />
    case 'links':
      return <Links key={key} {...block} />
    case 'custom': {
      const body = block.render()
      return block.reveal === false ? (
        <div key={key} className={block.className}>
          {body}
        </div>
      ) : (
        <Reveal key={key} className={block.className}>
          {body}
        </Reveal>
      )
    }
  }
}

function renderBlock(block: ContentBlock, key: number, anchorId?: string): ReactNode {
  switch (block.type) {
    case 'lead':
      return <Lead key={key} {...block} />
    case 'closing':
      return <Closing key={key} {...block} />
    case 'section':
      return <SectionGroup key={key} {...block} id={anchorId} renderLeaf={renderLeaf} />
    default:
      return renderLeaf(block, key)
  }
}

export default function ContentBlocks({ blocks, gradient }: { blocks: ContentBlocksData; gradient: Gradient }) {
  const style = {
    '--cb-from': gradient.from,
    '--cb-to': gradient.to,
  } as CSSProperties
  const anchorIds = new Map(sectionAnchors(blocks).map((a) => [a.index, a.id]))
  return (
    <div className="cb" style={style}>
      {blocks.map((b, i) => renderBlock(b, i, anchorIds.get(i)))}
    </div>
  )
}
