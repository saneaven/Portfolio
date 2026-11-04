import { useEffect, type CSSProperties, type ReactNode } from 'react'
import { AnimatePresence, motion } from 'motion/react'
import { EASE_IN, EASE_OUT } from '../../styles/motion'
import './SectionDrawer.css'

type Gradient = { from: string; to: string }

export default function SectionDrawer({
  open,
  onClose,
  label,
  gradient,
  children,
}: {
  open: boolean
  onClose: () => void
  label: string
  gradient: Gradient
  children: ReactNode
}) {
  useEffect(() => {
    if (!open) return
    const onKey = (e: KeyboardEvent) => {
      if (e.key === 'Escape') onClose()
    }
    window.addEventListener('keydown', onKey)
    return () => window.removeEventListener('keydown', onKey)
  }, [open, onClose])

  return (
    <AnimatePresence>
      {open && (
        <div className="section-drawer" style={{ '--cb-from': gradient.from, '--cb-to': gradient.to } as CSSProperties}>
          <motion.div
            className="section-drawer__backdrop"
            onClick={onClose}
            initial={{ opacity: 0 }}
            animate={{ opacity: 1, transition: { duration: 0.25, ease: EASE_OUT } }}
            exit={{ opacity: 0, transition: { duration: 0.2, ease: EASE_IN } }}
          />
          <motion.nav
            className="section-drawer__panel"
            aria-label={label}
            initial={{ x: '100%' }}
            animate={{ x: 0, transition: { duration: 0.32, ease: EASE_OUT } }}
            exit={{ x: '100%', transition: { duration: 0.22, ease: EASE_IN } }}
          >
            <div className="section-drawer__head">
              <span className="section__nav-label">{label}</span>
              <button
                type="button"
                className="section-drawer__close"
                onClick={onClose}
                aria-label={`Close ${label.toLowerCase()}`}
              >
                <svg viewBox="0 0 24 24" aria-hidden>
                  <path d="M6 6 L18 18 M18 6 L6 18" />
                </svg>
              </button>
            </div>
            {children}
          </motion.nav>
        </div>
      )}
    </AnimatePresence>
  )
}
