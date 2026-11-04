import { useEffect, useRef, type RefObject } from 'react'
import { PALETTE, MAIN_DARK, MAIN_LIGHT, alpha, oklchGradientStops } from '../../styles/palette'

// Blob positions are vmin offsets from the viewport center.
const BLOBS = [
  { color: PALETTE.blue, x: 0, y: -13 },
  { color: PALETTE.crimson, x: 7, y: -9 },
  { color: PALETTE.red, x: 11, y: 9 },
  { color: PALETTE.gold, x: 4, y: 17 },
  { color: PALETTE.greenPale, x: -12, y: 8 },
  { color: PALETTE.teal, x: -18, y: -12 },
]

// Vmin uses the shorter viewport side; type sizes use fractions of width/height.
const CONFIG = {
  bg: { angleDeg: 120, from: PALETTE.pine, to: PALETTE.burgundy },
  blob: {
    radiusVmin: 24,
    driftAmpVmin: [3, 5],
    driftPeriodSec: [10, 18],
  },
  core: {
    color: MAIN_DARK,
    radiusVmin: 33,
    solidStop: 0.42, // fraction kept fully black before the edge fades
    pointerInfluence: 0.18,
    ease: 0.1,
  },
  type: {
    color: MAIN_LIGHT,
    sizeVw: 0.16,
    sizeVhCap: 0.32,
    sizeMaxPx: 320,
    marginVmin: 1.5,
    fontWeight: 700,
    fontFamily: 'Montserrat, sans-serif',
  },
  ring: { radiusVmin: 36, widthVmin: 0.15, color: alpha(MAIN_LIGHT, 0.35) },
  grain: { alpha: 0.1, tilePx: 160, blend: 'overlay' as GlobalCompositeOperation },
}

const TAU = Math.PI * 2
const rand = (min: number, max: number) => Math.random() * (max - min) + min

// Independent periods and phases avoid a synchronized, repeating orbit.
function makeDrift() {
  const [ampMin, ampMax] = CONFIG.blob.driftAmpVmin
  const [perMin, perMax] = CONFIG.blob.driftPeriodSec
  return BLOBS.map(() => ({
    ax: rand(ampMin, ampMax),
    ay: rand(ampMin, ampMax),
    wx: TAU / rand(perMin, perMax),
    wy: TAU / rand(perMin, perMax),
    px: rand(0, TAU),
    py: rand(0, TAU),
  }))
}

function makeNoiseTile(size: number): HTMLCanvasElement {
  const c = document.createElement('canvas')
  c.width = size
  c.height = size
  const ctx = c.getContext('2d')!
  const img = ctx.createImageData(size, size)
  for (let i = 0; i < img.data.length; i += 4) {
    const v = (Math.random() * 255) | 0
    img.data[i] = v
    img.data[i + 1] = v
    img.data[i + 2] = v
    img.data[i + 3] = 255
  }
  ctx.putImageData(img, 0, 0)
  return c
}

interface IrisCanvasProps {
  /** Shared with the lens so both layers use the same pixels. */
  canvasRef: RefObject<HTMLCanvasElement | null>
  className?: string
  /** 0..1: expand to solid black while fading the upper layers. */
  outro?: number
}

export default function IrisCanvas({ canvasRef, className, outro = 0 }: IrisCanvasProps) {
  const outroRef = useRef(0)
  useEffect(() => {
    outroRef.current = outro
  }, [outro])

  useEffect(() => {
    const canvas = canvasRef.current
    if (!canvas) return
    const ctx = canvas.getContext('2d')
    if (!ctx) return

    const drift = makeDrift()
    const noise = ctx.createPattern(makeNoiseTile(CONFIG.grain.tilePx), 'repeat')
    let coreTarget = { x: 0, y: 0 }
    const coreOffset = { x: 0, y: 0 }

    let fontReady = false
    document.fonts
      ?.load(`${CONFIG.type.fontWeight} 100px Montserrat`)
      .then(() => {
        fontReady = true
      })
      .catch(() => {})

    let raf = 0
    let w = 0
    let h = 0

    const resize = () => {
      // Bound full-screen redraw and texture-upload costs on HiDPI displays.
      const dpr = Math.min(window.devicePixelRatio || 1, 1.5)
      w = window.innerWidth
      h = window.innerHeight
      canvas.width = Math.round(w * dpr)
      canvas.height = Math.round(h * dpr)
      ctx.setTransform(dpr, 0, 0, dpr, 0, 0) // draw in CSS pixels
    }
    resize()

    const draw = (now: number) => {
      const t = now / 1000
      const vmin = Math.min(w, h) / 100
      const cx = w / 2
      const cy = h / 2
      const a = (CONFIG.bg.angleDeg * Math.PI) / 180
      const dx = Math.sin(a)
      const dy = -Math.cos(a)
      const len = Math.abs(w * dx) + Math.abs(h * dy)
      const bg = ctx.createLinearGradient(cx - (dx * len) / 2, cy - (dy * len) / 2, cx + (dx * len) / 2, cy + (dy * len) / 2)
      oklchGradientStops(bg, CONFIG.bg.from, CONFIG.bg.to)
      ctx.globalCompositeOperation = 'source-over'
      ctx.fillStyle = bg
      ctx.fillRect(0, 0, w, h)

      ctx.globalCompositeOperation = 'screen'
      const r = CONFIG.blob.radiusVmin * vmin
      BLOBS.forEach((b, i) => {
        const d = drift[i]
        const bx = cx + (b.x + d.ax * Math.sin(t * d.wx + d.px)) * vmin
        const by = cy + (b.y + d.ay * Math.sin(t * d.wy + d.py)) * vmin
        const g = ctx.createRadialGradient(bx, by, 0, bx, by, r)
        g.addColorStop(0, alpha(b.color, 1))
        g.addColorStop(1, alpha(b.color, 0))
        ctx.fillStyle = g
        ctx.beginPath()
        ctx.arc(bx, by, r, 0, TAU)
        ctx.fill()
      })
      // Center the expanding pupil during the outro so it covers every corner.
      const p = outroRef.current
      coreOffset.x += (coreTarget.x - coreOffset.x) * CONFIG.core.ease
      coreOffset.y += (coreTarget.y - coreOffset.y) * CONFIG.core.ease
      const kx = cx + coreOffset.x * (1 - p)
      const ky = cy + coreOffset.y * (1 - p)
      ctx.globalCompositeOperation = 'source-over'
      const baseCr = CONFIG.core.radiusVmin * vmin
      const halfDiag = Math.hypot(w, h) / 2
      const cr = baseCr + (halfDiag * 1.25 - baseCr) * p
      const solidStop = CONFIG.core.solidStop + (1 - CONFIG.core.solidStop) * p
      const core = ctx.createRadialGradient(kx, ky, 0, kx, ky, cr)
      core.addColorStop(0, alpha(CONFIG.core.color, 1))
      core.addColorStop(solidStop, alpha(CONFIG.core.color, 1))
      core.addColorStop(1, alpha(CONFIG.core.color, 0))
      ctx.fillStyle = core
      ctx.beginPath()
      ctx.arc(kx, ky, cr, 0, TAU)
      ctx.fill()
      // These layers sit above the pupil, so they need an explicit fade.
      const fade = 1 - p
      if (fontReady && fade > 0) {
        const size = Math.min(CONFIG.type.sizeVw * w, CONFIG.type.sizeVhCap * h, CONFIG.type.sizeMaxPx)
        const margin = CONFIG.type.marginVmin * vmin
        ctx.globalAlpha = fade
        ctx.fillStyle = CONFIG.type.color
        ctx.font = `${CONFIG.type.fontWeight} ${size}px ${CONFIG.type.fontFamily}`
        ctx.textAlign = 'left'
        ctx.textBaseline = 'top'
        ctx.fillText('WORLD', margin, -0.05 * size)
        ctx.textAlign = 'right'
        ctx.textBaseline = 'bottom'
        ctx.fillText('TECH', w - margin, h + 0.05 * size)
        ctx.globalAlpha = 1
      }

      ctx.globalCompositeOperation = 'source-over'
      ctx.globalAlpha = fade
      ctx.strokeStyle = CONFIG.ring.color
      ctx.lineWidth = CONFIG.ring.widthVmin * vmin
      ctx.beginPath()
      ctx.arc(cx, cy, CONFIG.ring.radiusVmin * vmin, 0, TAU)
      ctx.stroke()
      ctx.globalAlpha = 1

      if (noise) {
        ctx.globalCompositeOperation = CONFIG.grain.blend
        ctx.globalAlpha = CONFIG.grain.alpha * fade
        ctx.fillStyle = noise
        ctx.fillRect(0, 0, w, h)
        ctx.globalAlpha = 1
        ctx.globalCompositeOperation = 'source-over'
      }

      raf = requestAnimationFrame(draw)
    }
    raf = requestAnimationFrame(draw)

    const onPointerMove = (e: PointerEvent) => {
      coreTarget = {
        x: (e.clientX - window.innerWidth / 2) * CONFIG.core.pointerInfluence,
        y: (e.clientY - window.innerHeight / 2) * CONFIG.core.pointerInfluence,
      }
    }
    window.addEventListener('pointermove', onPointerMove)
    window.addEventListener('resize', resize)
    return () => {
      cancelAnimationFrame(raf)
      window.removeEventListener('resize', resize)
      window.removeEventListener('pointermove', onPointerMove)
    }
  }, [canvasRef])

  return <canvas ref={canvasRef} className={className} aria-hidden />
}
