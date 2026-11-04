import { cloneElement, useRef, useState, useSyncExternalStore, type CSSProperties, type MouseEvent } from 'react'
import { useLocation, useNavigate, useOutlet } from 'react-router-dom'
import { AnimatePresence, MotionConfig, animate, motion, type Variants } from 'motion/react'
import SaneavenLogo from '../../components/SaneavenLogo'
import { isPlainClick } from '../../components/plainClick'
import { CONTACTS } from '../../data/contacts'
import { siteHref } from '../../data/routes'
import { EASE_IN, EASE_IN_OUT, EASE_OUT } from '../../styles/motion'
import '../../styles/brand.css'
import '../../styles/brandMorph.css'

// Keep the brand and contact bar mounted across Home/About so layout can morph
// one shared node while the outlet views animate independently.
const OUTRO_DUR = 0.4
const OUTRO_TRANSITION = { duration: OUTRO_DUR, ease: EASE_IN }
const MORPH = { duration: 0.55, ease: EASE_IN_OUT }

const MOBILE_QUERY = '(max-width: 768px)'
const subscribeMobile = (onChange: () => void) => {
  const query = window.matchMedia(MOBILE_QUERY)
  query.addEventListener('change', onChange)
  return () => query.removeEventListener('change', onChange)
}

// The outer layer owns category exits; the inner brand owns the Home/About morph.
const brandExit: Variants = {
  rest: { x: 0, y: 0 },
  game: { y: '110vh' },
  ai: { y: '-110vh' },
}

const controlsVariants: Variants = {
  hidden: { opacity: 0 },
  rest: { opacity: 1, x: 0, y: 0, transition: { opacity: { duration: 0.8, ease: EASE_OUT, delay: 0.5 } } },
  game: brandExit.game,
  ai: brandExit.ai,
}

export interface ShellContext {
  leaving: 'game' | 'ai' | null
  startLeave: (which: 'game' | 'ai', to?: string) => void
}

export default function BrandShell() {
  const navigate = useNavigate()
  const location = useLocation()
  const mode = location.pathname === '/about-me' ? 'about' : 'home'
  const isMobile = useSyncExternalStore(
    subscribeMobile,
    () => window.matchMedia(MOBILE_QUERY).matches,
    () => false,
  )

  const [leaving, setLeaving] = useState<'game' | 'ai' | null>(null)
  const started = useRef(false)
  // Match the home scene's exit clock before unmounting the shell.
  const startLeave = (which: 'game' | 'ai', to: string = `/${which}`) => {
    if (started.current) return
    started.current = true
    setLeaving(which)
    animate(0, 1, { duration: OUTRO_DUR, ease: EASE_IN, onComplete: () => navigate(to) })
  }

  // Layout wrappers interrupt hover variant propagation to the logo.
  const [hovering, setHovering] = useState(false)
  const filled = hovering || mode === 'about'

  const brandTo = mode === 'about' ? '/home' : '/about-me'
  const onBrandClick = (e: MouseEvent) => {
    if (!isPlainClick(e)) return
    e.preventDefault()
    navigate(brandTo)
  }

  const outlet = useOutlet({ leaving, startLeave } satisfies ShellContext)

  return (
    <MotionConfig reducedMotion="user">
      <div className="brand-shell" data-mode={mode}>
        <div className="brand-stage" data-mode={mode}>
          <motion.div className="brand-exit" variants={brandExit} initial="rest" animate={leaving ?? 'rest'} transition={OUTRO_TRANSITION}>
            <motion.a
              className="brand"
              href={siteHref(brandTo)}
              aria-label={mode === 'about' ? 'Back to home' : 'SANEAVEN, About Me'}
              draggable={false}
              onClick={onBrandClick}
              onHoverStart={() => setHovering(true)}
              onHoverEnd={() => setHovering(false)}
              layout
              layoutDependency={mode}
              transition={MORPH}
            >
              <motion.span className="brand__mark-wrap" layout layoutDependency={mode} transition={MORPH}>
                <SaneavenLogo
                  className="brand__mark"
                  enableHoverFill
                  initial={mode === 'about' ? 'hover' : 'idle'}
                  animate={filled ? 'hover' : 'idle'}
                />
              </motion.span>

              <motion.span className="brand__name" layout layoutDependency={mode} transition={MORPH}>
                SANEAVEN
              </motion.span>
            </motion.a>
          </motion.div>
        </div>

        <motion.div
          className="brand-controls"
          data-mode={mode}
          layoutRoot
          layoutScroll
          variants={controlsVariants}
          initial="hidden"
          animate={leaving ?? 'rest'}
          transition={OUTRO_TRANSITION}
        >
          <ul className="brand-controls__links">
            {CONTACTS.map((c) => (
              <li key={c.label}>
                <a className="brand-controls__link" href={c.href} aria-label={c.label} {...(c.external ? { target: '_blank', rel: 'noreferrer' } : {})}>
                  <span className="brand-controls__icon" style={{ '--brand-icon': `url("${c.icon}")` } as CSSProperties} />
                </a>
              </li>
            ))}
          </ul>
          <span className="brand-controls__sep" aria-hidden="true" />
          <motion.a
            className="brand-controls__switch"
            href={siteHref(brandTo)}
            aria-label={mode === 'about' ? 'Home' : 'About Me'}
            draggable={false}
            onClick={onBrandClick}
            // Mobile changes the label without moving the button.
            layout={isMobile ? false : 'position'}
            layoutDependency={mode}
            transition={MORPH}
          >
            <span className="brand-controls__label" data-on={mode === 'home'}>About Me</span>
            <span className="brand-controls__label" data-on={mode === 'about'}>Home</span>
          </motion.a>
        </motion.div>

        {/* A path key lets each outlet finish its exit before the next view enters. */}
        <AnimatePresence mode="wait">
          {outlet && cloneElement(outlet, { key: location.pathname })}
        </AnimatePresence>
      </div>
    </MotionConfig>
  )
}
