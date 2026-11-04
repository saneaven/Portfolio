import type { ReactNode } from 'react'
import { motion } from 'motion/react'
import { EASE_OUT, useReducedMotion } from '../../../../styles/motion'
import type { Media as MediaData } from './types'

/** Reveal once on entry; reduced motion keeps content static and visible. */
export function Reveal({
  children,
  className,
  delay = 0,
}: {
  children: ReactNode
  className?: string
  delay?: number
}) {
  const reduce = useReducedMotion()
  if (reduce) return <div className={className}>{children}</div>
  return (
    <motion.div
      className={className}
      initial={{ opacity: 0, y: 24 }}
      whileInView={{ opacity: 1, y: 0 }}
      viewport={{ once: true, margin: '0px 0px -10% 0px' }}
      transition={{ duration: 0.6, ease: EASE_OUT, delay }}
    >
      {children}
    </motion.div>
  )
}

export function SectionHead({ eyebrow, title }: { eyebrow: string; title: string }) {
  return (
    <div className="cb__head">
      <span className="cb__eyebrow">{eyebrow}</span>
      <h2 className="cb__heading">{title}</h2>
    </div>
  )
}

export function Media({ caption, image, video }: MediaData) {
  return (
    <figure className="cb__media">
      <div className="cb__media-box">
        {video ? (
          <video src={video} autoPlay loop muted playsInline aria-hidden="true" />
        ) : (
          image && <img src={image} alt="" />
        )}
      </div>
      <figcaption className="cb__media-cap">{caption}</figcaption>
    </figure>
  )
}
