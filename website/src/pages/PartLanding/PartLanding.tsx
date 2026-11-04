import { useRef, useState, type MouseEvent } from 'react'
import { motion, animate } from 'motion/react'
import { useNavigate } from 'react-router-dom'
import { EASE_IN, EASE_IN_OUT, EASE_OUT } from '../../styles/motion'
import { markFromPartLanding } from './fromLanding'
import Hero from '../../components/Hero/Hero'
import { isPlainClick } from '../../components/plainClick'
import { siteHref } from '../../data/routes'
import './PartLanding.css'

export type SectionTitle = {
  title: string
  subtitle: string
  image: string
  to: string
}

const OUTRO_DUR = 0.5
const HOME_FADE = 0.35

// Route changes use their own clock so hover/layout animation completion cannot
// navigate early. Only ordinary link clicks are intercepted for the transition.
export default function PartLanding({ sections }: { sections: SectionTitle[] }) {
  const navigate = useNavigate()
  const [outro, setOutro] = useState<number | null>(null)
  const [leaving, setLeaving] = useState(false)
  // The ref guards both home and section exits before their state updates commit.
  const started = useRef(false)

  const goHome = (e: MouseEvent) => {
    if (!isPlainClick(e)) return
    e.preventDefault()
    if (started.current) return
    started.current = true
    setLeaving(true)
    animate(0, 1, { duration: HOME_FADE, ease: EASE_IN, onComplete: () => navigate('/home') })
  }

  const startOutro = (i: number, to: string) => (e: MouseEvent) => {
    if (!isPlainClick(e)) return
    e.preventDefault()
    if (started.current) return
    started.current = true
    setOutro(i)
    animate(0, 1, {
      duration: OUTRO_DUR,
      ease: EASE_IN_OUT,
      onComplete: () => {
        markFromPartLanding()
        navigate(to)
      },
    })
  }

  return (
    <motion.main
      className="part-landing"
      data-outro={outro !== null}
      animate={{ opacity: leaving ? 0 : 1 }}
      transition={{ duration: HOME_FADE, ease: EASE_IN }}
    >
      {outro === null && (
        <div className="part-landing__home">
          <a className="part-landing__home-btn" href={siteHref('/home')} onClick={goHome} aria-label="Back to home">
            <svg className="part-landing__home-arrow" viewBox="0 0 24 24" aria-hidden>
              <path d="M15 5 L8 12 L15 19" />
            </svg>
            Home
          </a>
        </div>
      )}
      {sections.map((s, i) => {
        const clicked = outro === i
        return (
          <motion.a
            key={s.title}
            className="part-landing__section"
            href={siteHref(s.to)}
            draggable={false}
            onClick={startOutro(i, s.to)}
            initial={{ opacity: 0, y: 20 }}
            animate={
              outro === null
                ? { opacity: 1, y: 0 }
                : clicked
                  ? { opacity: 1, y: 0, flexGrow: 50 }
                  : { opacity: 0, flexGrow: 0 }
            }
            transition={
              outro === null
                ? { delay: i * 0.12, duration: 0.6, ease: EASE_OUT }
                : { duration: OUTRO_DUR, ease: EASE_IN_OUT }
            }
          >
            <Hero image={s.image} title={s.title} subtitle={s.subtitle} as="h2" />
          </motion.a>
        )
      })}
    </motion.main>
  )
}
