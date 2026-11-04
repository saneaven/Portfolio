import { motion, type Variants } from 'motion/react'
import { EASE_OUT } from '../styles/motion'

interface SaneavenLogoProps {
  className?: string
  /**
   * Motion variants applied to the `<svg>` container. Use this to orchestrate
   * the four strokes (e.g. `staggerChildren`); pair with `pathVariants`.
   */
  variants?: Variants
  pathVariants?: Variants
  initial?: string | boolean
  animate?: string
  /**
   * Adds a per-stroke fill that sweeps in diagonally on `hover`. Requires a
   * motion ancestor driving `idle` / `hover` variant labels.
   */
  enableHoverFill?: boolean
}

// The four strokes of the mark, traced from the Figma source (viewBox 200×154).
const PATHS = [
  'M130.591 12.8481L7.27734 94.7271L1.30957 150.75L130.591 12.8481Z',
  'M119.735 106.317L189.356 43.6626L90.4092 82.0571L13.5498 150.406L119.735 106.317Z',
  'M141.012 140.449L181.548 105.596L27.1249 151.602L141.012 140.449Z',
  'M119.372 51.6155C140.264 46.1947 150.936 43.2572 160.623 36.9817C170.321 30.6989 179.055 21.0557 195.901 2.04712C171.488 9.7622 159.03 13.8437 149.327 20.1292C139.645 26.4014 132.686 34.8832 119.372 51.6155Z',
]

// atan(154/200) ≈ 37.6° — leading edge runs bottom-left ↔ top-right.
const SWEEP_ANGLE = -37.6
const SWEEP_DUR = 0.55
const SWEEP_STAGGER = 0.08

const sweepRectVariants: Variants = {
  idle: { x: -320, transition: { duration: SWEEP_DUR, ease: EASE_OUT } },
  hover: (i: number) => ({
    x: 320,
    transition: { duration: SWEEP_DUR, delay: i * SWEEP_STAGGER, ease: EASE_OUT },
  }),
}

export default function SaneavenLogo({
  className,
  variants,
  pathVariants,
  initial,
  animate,
  enableHoverFill = false,
}: SaneavenLogoProps) {
  return (
    <motion.svg
      className={className}
      width="100%"
      viewBox="0 0 200 154"
      fill="none"
      role="img"
      aria-label="SANEAVEN"
      xmlns="http://www.w3.org/2000/svg"
      // overflow:visible — intro strokes swing outside the viewBox.
      style={{ display: 'block', height: 'auto', overflow: 'visible' }}
      variants={variants}
      initial={initial}
      animate={animate}
    >
      {enableHoverFill && (
        <defs>
          {PATHS.map((_, i) => (
            <mask key={i} id={`sane-sweep-mask-${i}`} maskUnits="userSpaceOnUse">
              <g transform={`rotate(${SWEEP_ANGLE} 100 77)`}>
                <motion.rect
                  x={-400}
                  y={-300}
                  width={400}
                  height={760}
                  fill="white"
                  custom={i}
                  variants={sweepRectVariants}
                />
              </g>
            </mask>
          ))}
        </defs>
      )}

      {enableHoverFill &&
        PATHS.map((d, i) => (
          <path key={`fill-${d}`} d={d} fill="currentColor" mask={`url(#sane-sweep-mask-${i})`} />
        ))}

      {PATHS.map((d, i) => (
        <motion.path
          key={d}
          d={d}
          stroke="currentColor"
          strokeWidth="2"
          custom={i}
          variants={pathVariants}
          // Rotate around the mark's bottom-left, not each stroke's own center.
          // originX/originY are the only origin inputs motion respects here.
          style={{ transformBox: 'view-box', originX: 0, originY: 1 }}
        />
      ))}
    </motion.svg>
  )
}
