import { useEffect } from 'react'
import { AnimatePresence, motion } from 'motion/react'
import { useNavigate } from 'react-router-dom'
import { EASE_IN, EASE_OUT } from '../../styles/motion'
import { type Category, toSectionPath } from '../../data/sections'

export default function SectionMenu({
  open,
  category,
  currentSlug,
  onClose,
}: {
  open: boolean
  category: Category
  currentSlug: string
  onClose: () => void
}) {
  const navigate = useNavigate()

  useEffect(() => {
    if (!open) return
    const onKey = (e: KeyboardEvent) => {
      if (e.key === 'Escape') onClose()
    }
    window.addEventListener('keydown', onKey)
    return () => window.removeEventListener('keydown', onKey)
  }, [open, onClose])

  const select = (slug: string) => () => {
    navigate(toSectionPath(category.key, slug))
    // Re-selecting the current project must close the menu too.
    onClose()
  }

  return (
    <AnimatePresence>
      {open && (
        <motion.ul
          className="section-menu__list"
          onClick={(e) => e.stopPropagation()}
          initial={{ opacity: 0, y: 12 }}
          animate={{ opacity: 1, y: 0, transition: { duration: 0.3, ease: EASE_OUT, delay: 0.08 } }}
          exit={{ opacity: 0, y: 12, transition: { duration: 0.15, ease: EASE_IN } }}
        >
          {category.projects.map((p) => (
            <li key={p.slug}>
              <button
                type="button"
                className="section-menu__item"
                aria-current={p.slug === currentSlug ? 'page' : undefined}
                onClick={select(p.slug)}
              >
                {p.title}
              </button>
            </li>
          ))}
        </motion.ul>
      )}
    </AnimatePresence>
  )
}
