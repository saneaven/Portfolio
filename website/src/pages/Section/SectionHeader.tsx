import { type CSSProperties } from 'react'
import { motion, type MotionValue } from 'motion/react'
import SaneavenLogo from '../../components/SaneavenLogo'
import SectionMenu from './SectionMenu'
import { type Category } from '../../data/sections'

export default function SectionHeader({
  y,
  category,
  currentSlug,
  menuOpen,
  onToggleMenu,
  onCloseMenu,
  onBrandClick,
  pickerOpen,
  onTogglePicker,
}: {
  y: MotionValue<string> | string
  category: Category
  currentSlug: string
  menuOpen: boolean
  onToggleMenu: () => void
  onCloseMenu: () => void
  onBrandClick: () => void
  pickerOpen: boolean
  onTogglePicker: () => void
}) {
  return (
    <motion.div
      className="section__bar"
      data-menu={menuOpen ? 'open' : 'closed'}
      style={{ y }}
      onClick={() => menuOpen && onCloseMenu()}
    >

      <div className="progressive-blur" aria-hidden>
        <div /><div /><div /><div /><div /><div />
      </div>

      <header className="section__header-row" onClick={(e) => e.stopPropagation()}>
        <button type="button" className="section__brand" onClick={onBrandClick} aria-label="SANEAVEN — home">
          <span className="brand">
            <SaneavenLogo className="brand__mark" />
            <span className="brand__name">SANEAVEN</span>
          </span>
        </button>

        <button
          type="button"
          className="section__category"
          data-open={menuOpen}
          style={{ '--cat-from': category.gradient.from, '--cat-to': category.gradient.to } as CSSProperties}
          onClick={onToggleMenu}
          aria-expanded={menuOpen}
        >

          <span className="section__category-bar" aria-hidden />

          <span className="section__category-name">{category.name}</span>
          <span className="section__chevron" aria-hidden>
            <svg viewBox="0 0 24 14">
              <path d="M2 2 L12 12 L22 2" />
            </svg>
          </span>
        </button>

        <button
          type="button"
          className="section__picker"
          data-open={pickerOpen}
          onClick={onTogglePicker}
          aria-expanded={pickerOpen}
          aria-label="Open contents"
        >
          <svg viewBox="0 0 24 24" aria-hidden>
            <path d="M3 6 H21 M3 12 H21 M3 18 H21" />
          </svg>
        </button>
      </header>

      <SectionMenu open={menuOpen} category={category} currentSlug={currentSlug} onClose={onCloseMenu} />
    </motion.div>
  )
}
