import { useEffect, useRef, type CSSProperties, type RefObject } from 'react'
import { LensRenderer } from './LensRenderer'
import { DEFAULT_LENS_OPTIONS, type LensOptions } from './types'

// Bound full-screen texture and drawing-buffer costs on HiDPI displays.
const MAX_DPR = 1.5
const getDpr = () => Math.min(window.devicePixelRatio || 1, MAX_DPR)
const POINTER_INFLUENCE = 0.06

interface LensProps {
  /** Live canvas sampled every frame. */
  sourceRef: RefObject<HTMLCanvasElement | null>
  /** Viewport geometry supplies defaults; pointer drift always owns center. */
  options?: Partial<LensOptions>
  /** Fraction of the viewport's shorter side. */
  radius?: number
  className?: string
  style?: CSSProperties
}

export default function Lens({ sourceRef, options, radius = 0.6, className, style }: LensProps) {
  const canvasRef = useRef<HTMLCanvasElement>(null)
  // Keep committed props current without rebuilding the WebGL context.
  const propsRef = useRef({ options, radius })

  useEffect(() => {
    propsRef.current = { options, radius }
  }, [options, radius])

  useEffect(() => {
    const canvas = canvasRef.current
    if (!canvas) return
    let renderer: LensRenderer
    try {
      renderer = new LensRenderer(canvas)
    } catch {
      return // Leave the iris visible when WebGL2 is unavailable.
    }

    const screenCenter = () => ({ x: window.innerWidth / 2, y: window.innerHeight / 2 })
    let target = screenCenter()
    const current = screenCenter()

    const baseOptions = (): LensOptions => {
      const { options, radius } = propsRef.current
      const r = radius * Math.min(window.innerWidth, window.innerHeight)
      return {
        ...DEFAULT_LENS_OPTIONS,
        bevelWidth: r * 0.22,
        size: { x: r, y: r },
        cornerRadius: r,
        ...options,
        center: { ...current },
      }
    }

    renderer.resize(window.innerWidth, window.innerHeight, getDpr())

    let frame = 0
    const tick = () => {
      current.x += (target.x - current.x) * 0.1
      current.y += (target.y - current.y) * 0.1

      const source = sourceRef.current
      if (source) {
        renderer.setBackground(source)
        renderer.setOptions(baseOptions())
        renderer.render()
      }
      frame = requestAnimationFrame(tick)
    }
    frame = requestAnimationFrame(tick)

    const onPointerMove = (e: PointerEvent) => {
      const c = screenCenter()
      target = {
        x: c.x + (e.clientX - c.x) * POINTER_INFLUENCE,
        y: c.y + (e.clientY - c.y) * POINTER_INFLUENCE,
      }
    }
    const onResize = () => {
      target = screenCenter()
      current.x = target.x
      current.y = target.y
      renderer.resize(window.innerWidth, window.innerHeight, getDpr())
    }
    window.addEventListener('pointermove', onPointerMove)
    window.addEventListener('resize', onResize)

    return () => {
      cancelAnimationFrame(frame)
      window.removeEventListener('pointermove', onPointerMove)
      window.removeEventListener('resize', onResize)
      renderer.dispose()
    }
  }, [sourceRef])

  return <canvas ref={canvasRef} className={className} style={style} />
}
