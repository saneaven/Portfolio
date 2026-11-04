import { useCallback, useEffect, useRef, useState } from 'react'
import { LensRenderer } from '../../lens/LensRenderer'
import { captureBackground } from '../../lens/captureBackground'
import { DEFAULT_LENS_OPTIONS, type LensOptions } from '../../lens/types'
import './LensPlayground.css'

export default function LensPlayground() {
  const canvasRef = useRef<HTMLCanvasElement>(null)
  const rendererRef = useRef<LensRenderer | null>(null)
  const frameRef = useRef(0)

  const [options, setOptions] = useState<LensOptions>(() => ({
    ...DEFAULT_LENS_OPTIONS,
    center: {
      x: typeof window !== 'undefined' ? window.innerWidth / 2 : 0,
      y: typeof window !== 'undefined' ? window.innerHeight / 2 : 0,
    },
  }))
  const optionsRef = useRef(options)

  const [follow, setFollow] = useState(true)
  const followRef = useRef(follow)
  const [live, setLive] = useState(true)
  const [captureFps, setCaptureFps] = useState(30)
  const fpsRef = useRef(captureFps)
  const captureLoopRef = useRef(0)
  const lastCaptureRef = useRef(0)
  const inFlightRef = useRef(false)
  // Fraction of DPR: trade capture detail for lower raster/upload cost.
  const [captureScale, setCaptureScale] = useState(1)
  const captureScaleRef = useRef(captureScale)

  const [error, setError] = useState<string | null>(null)

  useEffect(() => {
    optionsRef.current = options
    followRef.current = follow
    fpsRef.current = captureFps
    captureScaleRef.current = captureScale
  }, [options, follow, captureFps, captureScale])

  const scheduleRender = useCallback(() => {
    if (frameRef.current) return
    frameRef.current = requestAnimationFrame(() => {
      frameRef.current = 0
      rendererRef.current?.render()
    })
  }, [])

  const captureToTexture = useCallback(async () => {
    const renderer = rendererRef.current
    if (!renderer) return
    try {
      const px = (window.devicePixelRatio || 1) * captureScaleRef.current
      const background = await captureBackground(document.body, px)
      // A capture may finish after unmount or StrictMode recreates the renderer.
      if (rendererRef.current !== renderer) return
      renderer.setBackground(background)
      scheduleRender()
    } catch (e) {
      setError(e instanceof Error ? e.message : String(e))
    }
  }, [scheduleRender])

  useEffect(() => {
    const canvas = canvasRef.current
    if (!canvas) return
    let renderer: LensRenderer
    try {
      renderer = new LensRenderer(canvas)
    } catch (e) {
      const message = e instanceof Error ? e.message : String(e)
      const frame = requestAnimationFrame(() => setError(message))
      return () => cancelAnimationFrame(frame)
    }
    rendererRef.current = renderer
    renderer.resize(window.innerWidth, window.innerHeight, window.devicePixelRatio || 1)
    renderer.setOptions(optionsRef.current)
    // Let the page paint before rasterizing its first snapshot.
    const id = window.setTimeout(() => void captureToTexture(), 50)

    return () => {
      window.clearTimeout(id)
      if (frameRef.current) {
        cancelAnimationFrame(frameRef.current)
        frameRef.current = 0 // stale IDs would block scheduling after remount
      }
      renderer.dispose()
      rendererRef.current = null
    }
  }, [captureToTexture])
  // Await each capture to avoid overlapping DOM rasterizations.
  useEffect(() => {
    if (!live) return
    let cancelled = false
    const tick = async () => {
      if (cancelled) return
      const interval = 1000 / fpsRef.current
      const now = performance.now()
      if (!inFlightRef.current && now - lastCaptureRef.current >= interval) {
        inFlightRef.current = true
        lastCaptureRef.current = now
        await captureToTexture()
        inFlightRef.current = false
      }
      if (!cancelled) captureLoopRef.current = requestAnimationFrame(tick)
    }
    captureLoopRef.current = requestAnimationFrame(tick)
    return () => {
      cancelled = true
      if (captureLoopRef.current) cancelAnimationFrame(captureLoopRef.current)
      captureLoopRef.current = 0
    }
  }, [live, captureToTexture])

  useEffect(() => {
    rendererRef.current?.setOptions(options)
    scheduleRender()
  }, [options, scheduleRender])

  useEffect(() => {
    const onMove = (e: PointerEvent) => {
      if (!followRef.current) return
      setOptions((o) => ({ ...o, center: { x: e.clientX, y: e.clientY } }))
    }
    window.addEventListener('pointermove', onMove)
    return () => window.removeEventListener('pointermove', onMove)
  }, [])

  useEffect(() => {
    let resizeTimer = 0
    const onResize = () => {
      rendererRef.current?.resize(window.innerWidth, window.innerHeight, window.devicePixelRatio || 1)
      scheduleRender()
      window.clearTimeout(resizeTimer)
      resizeTimer = window.setTimeout(() => void captureToTexture(), 200)
    }
    window.addEventListener('resize', onResize)
    return () => {
      window.removeEventListener('resize', onResize)
      window.clearTimeout(resizeTimer)
    }
  }, [captureToTexture, scheduleRender])

  const update = useCallback(<K extends keyof LensOptions>(key: K, value: LensOptions[K]) => {
    setOptions((o) => ({ ...o, [key]: value }))
  }, [])

  const maxRadius = Math.min(options.size.x, options.size.y)

  return (
    <div className="lens-stage">
      <div className="lens-bg">
        <h1>Liquid&nbsp;Glass</h1>
        <p>WebGL · GLSL · refraction · dispersion · frost · depth</p>
        <div className="lens-swatches">
          {['#ff5f6d', '#ffc371', '#24fe41', '#22c1c3', '#7028e4', '#ff2079'].map((c) => (
            <span key={c} style={{ background: c }} />
          ))}
        </div>
        <div className="lens-grid" aria-hidden />
      </div>

      <canvas ref={canvasRef} className="lens-canvas" data-lens-ignore="" />

      {error ? (
        <div className="lens-panel" data-lens-ignore="">
          <strong>Lens unavailable</strong>
          <p className="lens-error">{error}</p>
        </div>
      ) : (
        <div className="lens-panel" data-lens-ignore="">
          <header>
            <strong>Lens</strong>
          </header>

          <label className="lens-toggle">
            <input type="checkbox" checked={follow} onChange={(e) => setFollow(e.target.checked)} />
            Follow cursor
          </label>

          <label className="lens-toggle">
            <input type="checkbox" checked={live} onChange={(e) => setLive(e.target.checked)} />
            Live capture
          </label>

          <Slider label="Capture FPS" value={captureFps} min={1} max={120} step={1}
            onChange={setCaptureFps} />
          <Slider label="Capture scale" value={captureScale} min={0.25} max={1} step={0.05}
            onChange={setCaptureScale} />

          <Slider label="Size" value={options.size.x} min={20} max={400} step={1}
            onChange={(v) => setOptions((o) => ({
              ...o, size: { x: v, y: v }, cornerRadius: Math.min(o.cornerRadius, v),
            }))} />
          <Slider label="Corner radius" value={options.cornerRadius} min={0} max={maxRadius} step={1}
            onChange={(v) => update('cornerRadius', v)} />
          <Slider label="Refraction" value={options.refraction} min={0} max={1} step={0.01}
            onChange={(v) => update('refraction', v)} />
          <Slider label="Dispersion" value={options.dispersion} min={0} max={1} step={0.01}
            onChange={(v) => update('dispersion', v)} />
          <Slider label="Frost" value={options.frost} min={0} max={30} step={0.5}
            onChange={(v) => update('frost', v)} />
          <Slider label="Depth (bevel)" value={options.bevelWidth} min={1} max={maxRadius} step={1}
            onChange={(v) => update('bevelWidth', v)} />
          <Slider label="Tint opacity" value={options.tintOpacity} min={0} max={0.5} step={0.01}
            onChange={(v) => update('tintOpacity', v)} />
          <Slider label="Edge highlight" value={options.edgeHighlight} min={0} max={1} step={0.01}
            onChange={(v) => update('edgeHighlight', v)} />
          <Slider label="Light angle" value={options.lightAngle} min={0} max={360} step={1}
            onChange={(v) => update('lightAngle', v)} />

          <p className="lens-hint">
            Corner radius at max → circle, at 0 → rectangle.
          </p>
        </div>
      )}
    </div>
  )
}

interface SliderProps {
  label: string
  value: number
  min: number
  max: number
  step: number
  onChange: (value: number) => void
}

function Slider({ label, value, min, max, step, onChange }: SliderProps) {
  return (
    <label className="lens-slider">
      <span>{label}</span>
      <input
        type="range"
        min={min}
        max={max}
        step={step}
        value={value}
        onChange={(e) => onChange(Number(e.target.value))}
      />
      <output>{Number.isInteger(step) ? value : value.toFixed(2)}</output>
    </label>
  )
}
