import { useEffect, useRef, useState } from 'react'
import { useNavigate } from 'react-router-dom'
import { motion, animate, type Variants } from 'motion/react'
import SaneavenLogo from '../../components/SaneavenLogo'
import { Lens } from '../../lens'
import { EASE_IN, EASE_OUT } from '../../styles/motion'
import IrisCanvas from './IrisCanvas'
import '../../styles/brand.css'
import './Landing.css'

// Timeline values are seconds, offset by the initial pause.
const START_DELAY = 0.5
const LENS_DUR = 3
const LOGO_DELAY = START_DELAY + LENS_DUR - 1
const LOGO_STAGGER = 0.1
const LOGO_FADE_DUR = 1.0
const LOGO_ROTATE_DUR = 3.0
// The final stroke slides in after the first three finish swinging.
const LAST_DELAY = LOGO_DELAY + 2 * LOGO_STAGGER + 1
const LAST_DUR = 3
const TEXT_DELAY = START_DELAY + 4.0
const HINT_DELAY = START_DELAY + 5.0
const HINT_DUR = 0.3
const HINT_FILL_DELAY = HINT_DELAY + HINT_DUR + 0.2
const HINT_FILL_DUR = 3.0

const BASE_RADIUS = 0.6
const BASE_FROST = 8
const OUTRO_DUR = 0.5
// SVG viewBox units.
const LAST_OFFSET = { x: 16, y: -4 }

// Only propagate variant labels; each stroke has its own delay.
const logoContainer: Variants = {
  hidden: {},
  visible: {},
}

const logoPath: Variants = {
  hidden: (i: number) => (i === 3 ? { opacity: 0, ...LAST_OFFSET } : { opacity: 0, rotate: 45 }),
  visible: (i: number) =>
    i === 3
      ? {
          opacity: 1,
          x: 0,
          y: 0,
          transition: {
            opacity: { delay: LAST_DELAY, duration: LAST_DUR / 2, ease: EASE_OUT },
            x: { delay: LAST_DELAY, duration: LAST_DUR, ease: EASE_OUT },
            y: { delay: LAST_DELAY, duration: LAST_DUR, ease: EASE_OUT },
          },
        }
      : {
          opacity: 1,
          rotate: 0,
          transition: {
            opacity: { delay: LOGO_DELAY + i * LOGO_STAGGER, duration: LOGO_FADE_DUR, ease: EASE_OUT },
            rotate: { delay: LOGO_DELAY + i * LOGO_STAGGER, duration: LOGO_ROTATE_DUR, ease: EASE_OUT },
          },
        },
}

export default function Landing() {
  const irisRef = useRef<HTMLCanvasElement>(null)
  const [intro, setIntro] = useState(0)
  useEffect(() => {
    const controls = animate(-1, 1, { duration: LENS_DUR, ease: EASE_OUT, delay: START_DELAY, onUpdate: setIntro })
    return () => controls.stop()
  }, [])

  const lensRadius = BASE_RADIUS * (2 - intro)
  const lensFrost = BASE_FROST * intro

  const navigate = useNavigate()
  const [outro, setOutro] = useState(0)
  const outroStarted = useRef(false)
  const startOutro = () => {
    if (outroStarted.current) return
    outroStarted.current = true
    animate(0, 1, { duration: OUTRO_DUR, ease: EASE_IN, onUpdate: setOutro, onComplete: () => navigate('/home') })
  }

  return (
    <main className="landing" onClick={startOutro}>

      <IrisCanvas canvasRef={irisRef} className="iris-canvas" outro={outro} />

      <Lens
        sourceRef={irisRef}
        className="lens-overlay"
        radius={lensRadius}
        options={{ frost: lensFrost }}
        style={{ opacity: 1 - outro }}
      />

      <div className="brand">
        <SaneavenLogo
          className="brand__mark"
          variants={logoContainer}
          pathVariants={logoPath}
          initial="hidden"
          animate="visible"
        />
        <motion.p
          className="brand__name"
          initial={{ opacity: 0, y: -20 }}
          animate={{ opacity: 1, y: 0 }}
          transition={{ duration: 3, ease: EASE_OUT, delay: TEXT_DELAY }}
        >
          SANEAVEN
        </motion.p>
      </div>

      <div className="continue" style={{ opacity: 1 - outro }}>

        <motion.span
          className="continue__fill"
          initial={{ scaleX: 0 }}
          animate={{ scaleX: 1 }}
          transition={{ duration: HINT_FILL_DUR, ease: EASE_OUT, delay: HINT_FILL_DELAY }}
        />
        <motion.p
          className="continue__text"
          initial={{ opacity: 0 }}
          animate={{ opacity: 1 }}
          transition={{ duration: HINT_DUR, ease: EASE_OUT, delay: HINT_DELAY }}
        >
          Click any where to continue
        </motion.p>
      </div>
    </main>
  )
}
