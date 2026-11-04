import { useState, type MouseEvent } from 'react'
import { useOutletContext } from 'react-router-dom'
import { motion, type Variants } from 'motion/react'
import { EASE_IN, EASE_OUT } from '../../styles/motion'
import GradientBox, { type Blob } from './GradientBox'
import { PALETTE } from '../../styles/palette'
import type { ShellContext } from './BrandShell'
import { isPlainClick } from '../../components/plainClick'
import { NAME, ROLE } from '../../data/about'
import { siteHref } from '../../data/routes'
import { CATEGORIES, toSectionPath, type CategoryKey } from '../../data/sections'
import { useFitHomeIntro } from './useFitHomeIntro'
import './Home.css'

const BLOBS: Record<CategoryKey, Blob[]> = {
  game: [
    { color: PALETTE.gold, x: -0.38, y: -0.14 },
    { color: PALETTE.honey, x: 0.24, y: -0.44 },
    { color: PALETTE.red, x: 0.2, y: 0.1 },
    { color: PALETTE.crimson, x: -0.38, y: 0.36 },
  ],
  ai: [
    { color: PALETTE.blue, x: 0.32, y: -0.34 },
    { color: PALETTE.teal, x: -0.16, y: -0.26 },
    { color: PALETTE.green, x: 0.36, y: 0.26 },
    { color: PALETTE.leaf, x: -0.42, y: 0.34 },
  ],
}

const BOX_DUR = 1.0
const LABEL_DELAY = 0.5
const LABEL_DUR = 1.5
const FADE_IN = { opacity: 0, y: 10 }
const FADED_IN = { opacity: 1, y: 0 }
const FADE_TRANSITION = { duration: 0.8, ease: EASE_OUT, delay: LABEL_DELAY }

// Keep pulse durations fixed when the box entrance is retimed.
const BLINK_START = 0.1
const BLINK_PULSE = 0.05
const BLINK_GAPS = [0.10, 0.20]

const BLINK_TIMES = (() => {
  const ts: number[] = [0, BLINK_START]
  let t = BLINK_START
  for (let i = 0; i < 3; i++) {
    t += BLINK_PULSE / 2
    ts.push(t)
    t += BLINK_PULSE / 2
    ts.push(t)
    if (i < BLINK_GAPS.length) t += BLINK_GAPS[i]
  }
  ts.push(BOX_DUR)
  // Motion expects normalized keyframe times, while the authored timings use seconds.
  return ts.map((s) => s / BOX_DUR)
})()
const BLINK_OPACITY = [1, 1, 0, 1, 0, 1, 0, 1, 1]

const OUTRO_DUR = 0.4
const OUTRO_TRANSITION = { duration: OUTRO_DUR, ease: EASE_IN }

// Move the scene with BrandShell without overriding the boxes' entrance transforms.
const sceneLeave: Variants = {
  rest: { x: 0, y: 0 },
  game: { y: '110vh' },
  ai: { y: '-110vh' },
}

// About navigation uses AnimatePresence instead of the category exit clock.
const columnExit = { x: '-110vw', transition: OUTRO_TRANSITION }
const boardExit = { x: '110vw', transition: OUTRO_TRANSITION }

const projectLinks = (key: CategoryKey) =>
  CATEGORIES[key].projects.map((p) => ({
    slug: p.slug,
    label: p.children?.length ? `+ ${p.children.length} more` : p.title,
    more: Boolean(p.children?.length),
    path: toSectionPath(key, p.slug),
  }))

type LeaveTo = (which: CategoryKey, to?: string) => (e: MouseEvent) => void
type HoverProps = (which: CategoryKey) => { onHoverStart: () => void; onHoverEnd: () => void }

function CategorySquare({ which, active, leaveTo, hover }: { which: CategoryKey; active: boolean; leaveTo: LeaveTo; hover: HoverProps }) {
  return (
    <motion.a className="category__square" href={siteHref(`/${which}`)} aria-label={CATEGORIES[which].name} draggable={false} onClick={leaveTo(which)} {...hover(which)}>
      <motion.span
        className="category__box"
        initial={{ y: which === 'game' ? '-110vh' : '110vh' }}
        animate={{ y: 0, opacity: BLINK_OPACITY }}
        transition={{
          y: { duration: BOX_DUR, ease: EASE_OUT },
          opacity: { duration: BOX_DUR, times: BLINK_TIMES },
        }}
      >
        <GradientBox className="category__canvas" colors={BLOBS[which]} active={active} />
      </motion.span>
    </motion.a>
  )
}

// AI reverses the visual stack in CSS; keep the category label first in DOM order.
function CategoryText({ which, leaveTo, hover }: { which: CategoryKey; leaveTo: LeaveTo; hover: HoverProps }) {
  return (
    <div className={`category__cell category__cell--${which}`}>
      <motion.div
        className="category__text"
        initial={{ opacity: 0, y: which === 'game' ? '-100%' : '100%' }}
        animate={{ opacity: 1, y: 0 }}
        transition={{ duration: LABEL_DUR, ease: EASE_OUT, delay: LABEL_DELAY }}
      >
        <motion.a className="category" href={siteHref(`/${which}`)} draggable={false} onClick={leaveTo(which)} {...hover(which)}>
          <span className="category__label">{CATEGORIES[which].name}</span>
        </motion.a>
        <ul className="category__names">
          {projectLinks(which).map((p) => (
            <li key={p.slug}>
              <a className={`category__name${p.more ? ' category__name--more' : ''}`} href={siteHref(p.path)} onClick={leaveTo(which, p.path)}>
                {p.label}
              </a>
            </li>
          ))}
        </ul>
      </motion.div>
    </div>
  )
}

// The empty brand/control slots reserve room for BrandShell's persistent overlay.
export default function HomeView() {
  const { leaving, startLeave } = useOutletContext<ShellContext>()
  const { introRef, sayRef } = useFitHomeIntro()

  // Every link to a category page activates that category's square and label.
  const [hovered, setHovered] = useState<CategoryKey | null>(null)
  const hover: HoverProps = (which) => ({
    onHoverStart: () => setHovered(which),
    onHoverEnd: () => setHovered(null),
  })

  // Animate ordinary clicks while preserving native modified-link behavior.
  const leaveTo: LeaveTo = (which, to) => (e) => {
    if (!isPlainClick(e)) return
    e.preventDefault()
    startLeave(which, to)
  }

  return (
    <main className="home">
      <motion.div className="home__scene" variants={sceneLeave} initial="rest" animate={leaving ?? 'rest'} transition={OUTRO_TRANSITION}>
        <div className="home__brand-slot" aria-hidden="true" />

        <div className="home__intro" ref={introRef}>
          <motion.p className="home__say" ref={sayRef} initial={FADE_IN} animate={FADED_IN} transition={FADE_TRANSITION} exit={columnExit}>
            From{' '}
            <motion.a className="home__word home__word--game" href={siteHref('/game')} aria-label={`${CATEGORIES.game.name}: games`} onClick={leaveTo('game')} {...hover('game')}>
              games
            </motion.a>{' '}
            to{' '}
            <motion.a className="home__word home__word--ai" href={siteHref('/ai')} aria-label={`${CATEGORIES.ai.name}: AI`} onClick={leaveTo('ai')} {...hover('ai')}>
              AI
            </motion.a>
            , I’ve followed one thread: <span className="home__say-dim">finding where technology actually meets people.</span>
          </motion.p>
        </div>

        <motion.footer className="home__foot" initial={FADE_IN} animate={FADED_IN} transition={FADE_TRANSITION} exit={columnExit}>
          <div className="home__who">
            <h1 className="home__name">{NAME}</h1>
            <span className="home__role">{ROLE}</span>
          </div>
          <div className="home__controls-slot" aria-hidden="true" />
        </motion.footer>

        <motion.section className="home__board" exit={boardExit}>
          <div className="category__pair" data-active={hovered === 'game'}>
            <CategorySquare which="game" active={hovered === 'game'} leaveTo={leaveTo} hover={hover} />
            <CategoryText which="game" leaveTo={leaveTo} hover={hover} />
          </div>
          <div className="category__pair" data-active={hovered === 'ai'}>
            <CategoryText which="ai" leaveTo={leaveTo} hover={hover} />
            <CategorySquare which="ai" active={hovered === 'ai'} leaveTo={leaveTo} hover={hover} />
          </div>
        </motion.section>
      </motion.div>
    </main>
  )
}
