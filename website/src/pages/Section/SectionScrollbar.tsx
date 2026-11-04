import { useCallback, useEffect, useLayoutEffect, useRef, useState } from 'react'

// Replaces the native scrollbar, so the page keeps its full width while it scrolls.
export default function SectionScrollbar({ visible }: { visible: boolean }) {
  const trackRef = useRef<HTMLDivElement>(null)
  const [thumb, setThumb] = useState({ height: 0, top: 0 })
  const [dragging, setDragging] = useState(false)

  useLayoutEffect(() => {
    const el = document.documentElement
    el.toggleAttribute('data-section-scrollbar-hidden', true)
    return () => el.removeAttribute('data-section-scrollbar-hidden')
  }, [])

  const measure = useCallback(() => {
    const track = trackRef.current
    if (!track) return
    const viewport = window.innerHeight
    const docHeight = document.documentElement.scrollHeight
    const maxScroll = docHeight - viewport
    const trackHeight = track.clientHeight
    if (maxScroll <= 0) {
      setThumb({ height: 0, top: 0 })
      return
    }
    const height = Math.max((viewport / docHeight) * trackHeight, 40)
    const top = (window.scrollY / maxScroll) * (trackHeight - height)
    setThumb({ height, top })
  }, [])

  useEffect(() => {
    measure()
    window.addEventListener('scroll', measure, { passive: true })
    window.addEventListener('resize', measure)
    // Docking and media loads can change the document height without scrolling.
    const ro = new ResizeObserver(measure)
    ro.observe(document.documentElement)
    return () => {
      window.removeEventListener('scroll', measure)
      window.removeEventListener('resize', measure)
      ro.disconnect()
    }
  }, [measure])

  const onPointerDown = (e: React.PointerEvent) => {
    e.preventDefault()
    const track = trackRef.current
    if (!track) return
    setDragging(true)
    const startY = e.clientY
    const startScroll = window.scrollY
    const trackHeight = track.clientHeight
    const maxScroll = document.documentElement.scrollHeight - window.innerHeight
    const scrollPerPx = maxScroll / (trackHeight - thumb.height)

    const onMove = (ev: PointerEvent) => {
      window.scrollTo({ top: startScroll + (ev.clientY - startY) * scrollPerPx })
    }
    const onUp = () => {
      setDragging(false)
      window.removeEventListener('pointermove', onMove)
      window.removeEventListener('pointerup', onUp)
    }
    window.addEventListener('pointermove', onMove)
    window.addEventListener('pointerup', onUp)
  }

  return (
    <div className="section__scrollbar" data-visible={visible} aria-hidden ref={trackRef}>
      <div
        className="section__scrollbar-thumb"
        data-dragging={dragging}
        style={{ height: thumb.height, transform: `translateY(${thumb.top}px)` }}
        onPointerDown={onPointerDown}
      />
    </div>
  )
}
