import { useEffect, useState, type MouseEvent } from 'react'
import { useLocation, useNavigate } from 'react-router-dom'
import { useReducedMotion } from '../../../styles/motion'
import type { SectionAnchor } from './contentblock/anchors'
import { scrollToAnchor } from '../anchorScroll'
import { isPlainClick } from '../../../components/plainClick'
import './SectionToc.css'

const SPY_LINE = 0.3

function useActiveAnchor(anchors: SectionAnchor[]) {
  const [active, setActive] = useState<string>()
  // Callers rebuild the array; re-subscribe only when its ids change.
  const key = anchors.map((a) => a.id).join(' ')
  useEffect(() => {
    const ids = key ? key.split(' ') : []
    let frame = 0
    const update = () => {
      frame = 0
      const doc = document.documentElement
      // A short final section may never reach the spy line.
      if (window.innerHeight + window.scrollY >= doc.scrollHeight - 2) {
        setActive(ids.at(-1))
        return
      }
      const line = window.innerHeight * SPY_LINE
      let current: string | undefined
      for (const id of ids) {
        const el = document.getElementById(id)
        if (el && el.getBoundingClientRect().top <= line) current = id
      }
      setActive(current)
    }
    const schedule = () => {
      if (!frame) frame = requestAnimationFrame(update)
    }
    schedule()
    window.addEventListener('scroll', schedule, { passive: true })
    window.addEventListener('resize', schedule)
    return () => {
      cancelAnimationFrame(frame)
      window.removeEventListener('scroll', schedule)
      window.removeEventListener('resize', schedule)
    }
  }, [key])
  return active
}

export default function SectionToc({
  anchors,
  onNavigate,
}: {
  anchors: SectionAnchor[]
  onNavigate?: () => void
}) {
  const active = useActiveAnchor(anchors)
  const navigate = useNavigate()
  const { search } = useLocation()
  const reduce = useReducedMotion()

  const go = (id: string) => (e: MouseEvent) => {
    if (!isPlainClick(e)) return
    e.preventDefault()
    scrollToAnchor(id, reduce ? 'instant' : 'smooth')
    navigate({ search, hash: id }, { replace: true })
    onNavigate?.()
  }

  if (!anchors.length) return null

  return (
    <ul className="section-toc">
      {anchors.map((a) => (
        <li key={a.id}>
          <a
            className="section-toc__link"
            href={`#${a.id}`}
            aria-current={a.id === active ? 'location' : undefined}
            onClick={go(a.id)}
          >
            {a.label}
          </a>
        </li>
      ))}
    </ul>
  )
}
