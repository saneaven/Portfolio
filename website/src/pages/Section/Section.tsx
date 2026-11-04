import { useEffect, useLayoutEffect, useRef, useState, type CSSProperties } from 'react'
import { useNavigate, useParams } from 'react-router-dom'
import { motion, animate, useScroll, useTransform, useMotionValue, useMotionValueEvent } from 'motion/react'
import { EASE_IN, EASE_OUT, useReducedMotion } from '../../styles/motion'
import { getProject, type Category, type CategoryKey, type Project } from '../../data/sections'
import { takeFromPartLanding } from '../PartLanding/fromLanding'
import NotFound from '../NotFound/NotFound'
import ContentBlocks from './content/contentblock/ContentBlocks'
import { sectionAnchors } from './content/contentblock/anchors'
import OtherProjectsHub, { HubNav } from './content/OtherProjectsHub'
import SectionToc from './content/SectionToc'
import SectionDrawer from './SectionDrawer'
import SectionHeader from './SectionHeader'
import SectionScrollbar from './SectionScrollbar'
import { hashTarget, landOnAnchor } from './anchorScroll'
import Hero from '../../components/Hero/Hero'
import '../../styles/brand.css'
import './Section.css'

const HOME_FADE = 0.35

// Hero dimensions and pinned scroll distance in dvh.
const PIN = 50
const BAND = 38
const TOP_GAP = 15
const MORPH_END = 0.9
const HEADER_END = 0.4

export default function Section({ category }: { category: CategoryKey }) {
  const { slug, child } = useParams()
  const found = getProject(category, slug, child)
  if (!found) return <NotFound />
  return <SectionPage cat={found.category} project={found.project} />
}

function SectionPage({ cat, project }: { cat: Category; project: Project }) {
  const navigate = useNavigate()
  const reduce = useReducedMotion()

  // A fixed track keeps hero resizing out of the scroll measurement.
  const trackRef = useRef<HTMLDivElement>(null)
  const bodyRef = useRef<HTMLElement>(null)
  const { scrollYProgress } = useScroll({ target: trackRef, offset: ['start start', 'end end'] })
  const heroHeight = useTransform(scrollYProgress, [0, MORPH_END], ['100dvh', `${BAND}dvh`])
  const heroMargin = useTransform(scrollYProgress, [0, MORPH_END], ['0dvh', `${TOP_GAP}dvh`])
  const subtitleFade = useTransform(scrollYProgress, [0, 0.5], [1, 0])
  const indicatorFade = useTransform(scrollYProgress, [0, 0.3], [1, 0])
  const scrimReveal = useMotionValue(0)
  const indicatorReveal = useMotionValue(0)
  useEffect(() => {
    const scrim = animate(scrimReveal, 1, { duration: 0.8, delay: 0.1, ease: EASE_OUT })
    const hint = animate(indicatorReveal, 1, { duration: 0.45, delay: 0.45, ease: 'easeOut' })
    return () => {
      scrim.stop()
      hint.stop()
    }
  }, [scrimReveal, indicatorReveal])
  const indicatorComposed = useTransform(() => indicatorFade.get() * indicatorReveal.get())
  const scrimComposed = useTransform(() => indicatorFade.get() * scrimReveal.get())
  const headerSlide = useTransform(scrollYProgress, [0, HEADER_END], ['-100%', '0%'])

  const [menuOpen, setMenuOpen] = useState(false)
  const [pickerOpen, setPickerOpen] = useState(false)
  const [leaving, setLeaving] = useState(false)
  const started = useRef(false)

  // Unhide prerendered markup after this layout pass restores its scroll position.
  useLayoutEffect(() => {
    const el = document.documentElement
    el.toggleAttribute('data-section-ready', true)
    return () => el.removeAttribute('data-section-ready')
  }, [])

  const [animDone, setAnimDone] = useState(false)
  useMotionValueEvent(scrollYProgress, 'change', (v) => setAnimDone(v >= 1))

  const projectKey = `${cat.key}/${project.slug}`
  const [seenKey, setSeenKey] = useState(projectKey)
  if (projectKey !== seenKey) {
    setSeenKey(projectKey)
    setMenuOpen(false)
    setPickerOpen(false)
  }

  // Only a live category click plays the full hero. Other entries open the band.
  // Keep that decision stable across StrictMode effect re-runs and project switches.
  const intro = useRef<boolean | null>(null)
  const firstKey = useRef('')
  const switched = useRef(false)
  useLayoutEffect(() => {
    if (intro.current === null) {
      intro.current = takeFromPartLanding()
      firstKey.current = projectKey
    } else if (projectKey !== firstKey.current) {
      switched.current = true
    }
    // Contents-nav hash updates must not reset the page's scroll position.
    const target = hashTarget()
    if (target && bodyRef.current) return landOnAnchor(target, bodyRef.current)
    const fullHero = intro.current && !switched.current && !reduce
    const top = fullHero || reduce ? 0 : window.innerHeight * (PIN / 100)
    window.scrollTo({ top, behavior: 'instant' })
  }, [projectKey, reduce])

  const leave = (to: string) => {
    if (started.current) return
    started.current = true
    setLeaving(true)
    animate(0, 1, { duration: HOME_FADE, ease: EASE_IN, onComplete: () => navigate(to) })
  }

  const trackHeight = reduce ? 'auto' : `calc(100dvh + ${PIN}dvh)`
  const heroH = reduce ? `${BAND}dvh` : heroHeight
  const heroM = reduce ? `${TOP_GAP}dvh` : heroMargin
  const subOpacity = reduce ? 0 : subtitleFade
  const indicatorOpacity = reduce ? 0 : indicatorComposed
  const scrimOpacity = reduce ? 0 : scrimComposed
  const headerY = reduce ? '0%' : headerSlide
  // Dock the body under the band independently of the hero's animation rate.
  const bodyDock = reduce ? 0 : `calc(${TOP_GAP}dvh + ${BAND}dvh - 100dvh)`

  const toggleMenu = () => {
    setMenuOpen((v) => !v)
    setPickerOpen(false)
  }
  const togglePicker = () => {
    setPickerOpen((v) => !v)
    setMenuOpen(false)
  }
  const closePicker = () => setPickerOpen(false)
  const isHub = !!project.children?.length
  const navLabel = isHub ? 'Projects' : 'Contents'
  const nav = (onNavigate?: () => void) =>
    isHub ? (
      <HubNav category={cat.key} project={project} onNavigate={onNavigate} />
    ) : (
      <SectionToc anchors={sectionAnchors(project.blocks)} onNavigate={onNavigate} />
    )

  return (
    <motion.main
      className="section"
      animate={{ opacity: leaving ? 0 : 1 }}
      transition={{ duration: HOME_FADE, ease: EASE_IN }}
    >
      <div className="section__hero-track" ref={trackRef} style={{ height: trackHeight }}>
        <div className="section__hero-pin">
          <motion.section className="section__hero" style={{ height: heroH, marginTop: heroM }}>
            <Hero
              image={project.image}
              title={project.title}
              subtitle={project.subtitle}
              as="h1"
              subtitleOpacity={subOpacity}
            >
              <motion.div className="section__hero-back" style={{ opacity: indicatorOpacity }}>
                <button className="section__hero-back-btn" type="button" onClick={() => leave(`/${cat.key}`)} aria-label="Back to part">
                  <svg className="section__hero-back-arrow" viewBox="0 0 24 24" aria-hidden>
                    <path d="M15 5 L8 12 L15 19" />
                  </svg>
                  Back
                </button>
              </motion.div>
              <motion.div
                className="section__hero-scrim"
                style={{ opacity: scrimOpacity, scaleY: scrimReveal }}
                aria-hidden
              />
              <motion.div className="section__hero-indicator" style={{ opacity: indicatorOpacity }}>
                <span className="section__hero-indicator-label">Scroll to explore</span>
                <motion.svg
                  className="section__hero-indicator-chevron"
                  viewBox="0 0 24 14"
                  aria-hidden
                  animate={reduce ? undefined : { y: [0, 6, 0] }}
                  transition={reduce ? undefined : { duration: 1.6, repeat: Infinity, ease: 'easeInOut' }}
                >
                  <path d="M2 2 L12 12 L22 2" />
                </motion.svg>
              </motion.div>
            </Hero>
          </motion.section>
        </div>
      </div>

      <article className="section__body" ref={bodyRef} style={{ marginTop: bodyDock }}>
        {isHub ? (
          <OtherProjectsHub category={cat.key} project={project} gradient={cat.gradient} />
        ) : (
          <ContentBlocks blocks={project.blocks} gradient={cat.gradient} />
        )}
        <nav
          className="section__rail"
          style={{ '--cb-from': cat.gradient.from, '--cb-to': cat.gradient.to } as CSSProperties}
          aria-label={navLabel}
        >
          <div className="section__rail-inner">
            <span className="section__nav-label">{navLabel}</span>
            {nav()}
          </div>
        </nav>
      </article>

      <SectionHeader
        y={headerY}
        category={cat}
        currentSlug={project.slug}
        menuOpen={menuOpen}
        onToggleMenu={toggleMenu}
        onCloseMenu={() => setMenuOpen(false)}
        onBrandClick={() => leave('/home')}
        pickerOpen={pickerOpen}
        onTogglePicker={togglePicker}
      />

      <SectionScrollbar visible={reduce || animDone} />

      <SectionDrawer
        open={pickerOpen}
        onClose={closePicker}
        label={navLabel}
        gradient={cat.gradient}
      >
        {nav(closePicker)}
      </SectionDrawer>
    </motion.main>
  )
}
