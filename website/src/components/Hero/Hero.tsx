import { type ReactNode } from 'react'
import { motion, type MotionValue } from 'motion/react'
import './Hero.css'

// Each page supplies the animated container. Defaults match the visual handoff
// from PartLanding to Section; PartLanding.css overrides the resting appearance.
export default function Hero({
  image,
  title,
  subtitle,
  as = 'h2',
  subtitleOpacity,
  children,
}: {
  image: string
  title: string
  subtitle: string
  // h1 for Section (one per document), h2 for the landing bands.
  as?: 'h1' | 'h2'
  // Section fades the subtitle on scroll via a MotionValue; PartLanding omits it
  // so the subtitle keeps its CSS opacity (.9) and never fades.
  subtitleOpacity?: MotionValue<number> | number
  children?: ReactNode
}) {
  const Title = as
  return (
    <>
      <img className="hero__image" src={image} alt="" />
      <div className="hero__overlay" />
      {/* data-text feeds PartLanding's hover halo, a transparent ::before copy of
          the text that fades in by opacity (PartLanding.css). */}
      <div className="hero__text">
        <Title className="hero__title" data-text={title}>{title}</Title>
        <motion.p
          className="hero__subtitle"
          data-text={subtitle}
          style={subtitleOpacity !== undefined ? { opacity: subtitleOpacity } : undefined}
        >
          {subtitle}
        </motion.p>
      </div>
      {children}
    </>
  )
}
