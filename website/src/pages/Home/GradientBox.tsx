import { useEffect, useRef } from 'react'
import { alpha, MAIN_DARK } from '../../styles/palette'

// x/y are fractions of the shorter side, measured from the box center.
export interface Blob {
  color: string
  x: number
  y: number
}

interface GradientBoxProps {
  colors: Blob[]
  active: boolean
  className?: string
}

const TAU = Math.PI * 2
const rand = (min: number, max: number) => Math.random() * (max - min) + min

// While active each blob swings around its own pivot near the box center. Every
// swing rolls its own delay, end angle, start speed (rate x remaining angle per
// second) and fade, which shapes how that speed falls to zero on arrival, so a
// blob never overshoots.
const DELAY_MAX = 0.25 // seconds
const ANGLE_MIN = TAU / 6
const ANGLE_MAX = TAU / 4
const RATE_MIN = 0.5 // per second; higher snaps faster
const RATE_MAX = 4
const FADE_MIN = 6 // 2 brakes evenly; higher drops off early, then glides longer
const FADE_MAX = 12
const PIVOT_NOISE = 0.15 // shorter sides from the center, per axis

const GRAIN_SIGMA = 0.05 // standard deviation of each speck's opacity; higher is grainier
const GRAIN_TILE = 256 // pixels; large enough that the repeat doesn't show

// Independent periods and phases keep the blobs from drifting in lockstep.
function makeDrift(count: number) {
  return Array.from({ length: count }, () => ({
    ax: rand(0.04, 0.09),
    ay: rand(0.04, 0.09),
    wx: TAU / rand(10, 18), // angular frequency, radians per second
    wy: TAU / rand(10, 18),
    px: rand(0, TAU),
    py: rand(0, TAU),
  }))
}

// Black and white specks carry their own alpha, so the grain stays equally strong
// over dark and saturated colors, where an overlay blend fades out. The Gaussian
// spread adds the occasional stronger speck that uniform noise lacks.
function makeNoiseTile(size: number): HTMLCanvasElement {
  const c = document.createElement('canvas')
  c.width = size
  c.height = size
  const ctx = c.getContext('2d')!
  const img = ctx.createImageData(size, size)
  for (let i = 0; i < img.data.length; i += 4) {
    const g = Math.sqrt(-2 * Math.log(1 - Math.random())) * Math.cos(TAU * Math.random())
    const v = g > 0 ? 255 : 0
    img.data[i] = v
    img.data[i + 1] = v
    img.data[i + 2] = v
    img.data[i + 3] = Math.min(255, Math.abs(g) * GRAIN_SIGMA * 255)
  }
  ctx.putImageData(img, 0, 0)
  return c
}

export default function GradientBox({ colors, active, className }: GradientBoxProps) {
  const canvasRef = useRef<HTMLCanvasElement | null>(null)
  // The draw loop outlives renders, so it reads the latest state from a ref.
  const activeRef = useRef(active)

  useEffect(() => {
    activeRef.current = active
  }, [active])

  useEffect(() => {
    const canvas = canvasRef.current
    if (!canvas) return
    const parent = canvas.parentElement
    if (!parent) return
    const ctx = canvas.getContext('2d')
    if (!ctx) return

    const drift = makeDrift(colors.length)
    const spins = colors.map(() => ({ angle: 0, from: 0, to: 0, startedAt: 0, duration: 1, fade: 1, pivotX: 0, pivotY: 0 }))
    const noise = ctx.createPattern(makeNoiseTile(GRAIN_TILE), 'repeat')
    const reduceMotion = window.matchMedia('(prefers-reduced-motion: reduce)').matches

    let raf = 0
    let w = 0
    let h = 0
    let dpr = 1
    let wasActive = false

    const resize = () => {
      dpr = Math.min(window.devicePixelRatio || 1, 1.5)
      w = parent.clientWidth
      h = parent.clientHeight
      canvas.width = Math.max(1, Math.round(w * dpr))
      canvas.height = Math.max(1, Math.round(h * dpr))
      ctx.setTransform(dpr, 0, 0, dpr, 0, 0) // keep drawing coordinates in CSS pixels
    }

    const draw = (now: number) => {
      const t = reduceMotion ? 0 : now / 1000
      const isActive = activeRef.current
      if (isActive !== wasActive) {
        wasActive = isActive
        spins.forEach((spin) => {
          // Moving the pivot mid-swing would make the blob jump, so wait until it's home.
          if (spin.angle === 0) {
            spin.pivotX = rand(-PIVOT_NOISE, PIVOT_NOISE)
            spin.pivotY = rand(-PIVOT_NOISE, PIVOT_NOISE)
          }
          spin.from = spin.angle
          spin.to = isActive ? rand(ANGLE_MIN, ANGLE_MAX) : 0
          spin.startedAt = t + rand(0, DELAY_MAX)
          spin.fade = rand(FADE_MIN, FADE_MAX)
          // The curve's first-frame slope is fade / duration, so this keeps the start speed at the rolled rate.
          spin.duration = spin.fade / rand(RATE_MIN, RATE_MAX)
        })
      }
      const s = Math.min(w, h)
      const cx = w / 2
      const cy = h / 2

      ctx.globalCompositeOperation = 'source-over'
      ctx.fillStyle = MAIN_DARK
      ctx.fillRect(0, 0, w, h)

      ctx.globalCompositeOperation = 'screen'
      const r = 0.6 * s
      colors.forEach((b, i) => {
        const d = drift[i]
        const spin = spins[i]
        const left = Math.min(1, Math.max(0, 1 - (t - spin.startedAt) / spin.duration))
        spin.angle = spin.to + (spin.from - spin.to) * left ** spin.fade
        const cos = Math.cos(spin.angle)
        const sin = Math.sin(spin.angle)
        const x = b.x + d.ax * Math.sin(t * d.wx + d.px) - spin.pivotX
        const y = b.y + d.ay * Math.sin(t * d.wy + d.py) - spin.pivotY
        const bx = cx + (spin.pivotX + x * cos - y * sin) * s
        const by = cy + (spin.pivotY + x * sin + y * cos) * s
        const g = ctx.createRadialGradient(bx, by, 0, bx, by, r)
        g.addColorStop(0, b.color)
        g.addColorStop(1, alpha(b.color, 0))
        ctx.fillStyle = g
        ctx.beginPath()
        ctx.arc(bx, by, r, 0, TAU)
        ctx.fill()
      })

      if (noise) {
        // Draw one tile pixel per canvas pixel: the CSS-pixel scale would smooth the grain away.
        ctx.setTransform(1, 0, 0, 1, 0, 0)
        ctx.globalCompositeOperation = 'source-over'
        ctx.fillStyle = noise
        ctx.fillRect(0, 0, canvas.width, canvas.height)
        ctx.setTransform(dpr, 0, 0, dpr, 0, 0)
      }

      if (!reduceMotion) raf = requestAnimationFrame(draw)
    }

    const ro = new ResizeObserver(() => {
      resize()
      if (reduceMotion) draw(0)
    })
    ro.observe(parent)

    resize()
    raf = requestAnimationFrame(draw)

    return () => {
      cancelAnimationFrame(raf)
      ro.disconnect()
    }
  }, [colors])

  return <canvas ref={canvasRef} className={className} aria-hidden />
}
