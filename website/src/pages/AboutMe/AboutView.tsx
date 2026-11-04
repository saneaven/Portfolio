import { motion, type Variants } from 'motion/react'
import { EASE_OUT, EASE_IN } from '../../styles/motion'
import profileImage from '../../assets/section/profile/profile.png'
import { INTRO, NAME, ROLE } from '../../data/about'
import SectionScrollbar from '../Section/SectionScrollbar'
import './AboutMe.css'

interface TechGroup {
  label: string
  items: string[]
  wide?: boolean
}

const TECH_GROUPS: TechGroup[] = [
  { label: 'Languages', items: ['Python', 'C++', 'C', 'TypeScript / JavaScript'] },
  {
    label: 'AI / ML',
    items: ['PyTorch', 'TensorFlow / Keras', 'scikit-learn', 'HuggingFace', 'Isaac Lab', 'QLoRA / PEFT', 'RL (PPO)', 'Transformers'],
    wide: true,
  },
  { label: 'Web / Infra', items: ['React', 'FastAPI', 'Docker'] },
  { label: 'Game', items: ['C++ engines (ECS)', 'Unreal / UMG'] },
  { label: 'DB', items: ['MySQL'] },
]

// Fade this view independently while BrandShell moves the persistent logo.
const container: Variants = {
  hidden: {},
  show: { opacity: 1, transition: { delayChildren: 0.25, staggerChildren: 0.08 } },
  leaving: { opacity: 0, transition: { duration: 0.2, ease: EASE_IN } },
}

const group: Variants = {
  hidden: {},
  show: { transition: { staggerChildren: 0.05 } },
}
const rise: Variants = {
  hidden: { opacity: 0, y: 16 },
  show: { opacity: 1, y: 0, transition: { duration: 0.6, ease: EASE_OUT } },
}
const rule: Variants = {
  hidden: { opacity: 0, scaleX: 0 },
  show: { opacity: 1, scaleX: 1, transition: { duration: 0.6, ease: EASE_OUT } },
}

const progressiveBlur = (
  <div className="progressive-blur" aria-hidden>
    <div /><div /><div /><div /><div /><div />
  </div>
)

export default function AboutView() {
  return (
    <motion.main className="about" variants={container} initial="hidden" animate="show" exit="leaving">
      {/* Mobile scrolling uses the same progressive blur as the project header. */}
      <div className="about__header" aria-hidden="true">
        {progressiveBlur}
      </div>

      <div className="about__content">
        <motion.div className="about__hero" variants={group}>
          <motion.img
            className="about__portrait"
            variants={rise}
            src={profileImage}
            alt={NAME}
          />
          <div className="about__spine" aria-hidden="true" />

          <motion.div className="about__text" variants={group}>
            <motion.span className="about__eyebrow" variants={rise}>
              ABOUT
            </motion.span>
            <motion.h1 className="about__name" variants={rise}>
              {NAME}
            </motion.h1>
            <motion.div className="about__rule" variants={rule} />
            <motion.span className="about__title" variants={rise}>
              {ROLE}
            </motion.span>

            <motion.div className="about__intro" variants={group}>
              {INTRO.map((paragraph, i) => (
                <motion.p key={i} className="about__para" variants={rise}>
                  {paragraph}
                </motion.p>
              ))}
            </motion.div>
          </motion.div>
        </motion.div>

        <motion.div className="about__band" variants={group}>
          <motion.div className="about__tech" variants={group}>
            {TECH_GROUPS.map((g) => (
              <motion.div
                key={g.label}
                className={`about__tech-group${g.wide ? ' about__tech-group--wide' : ''}`}
                variants={rise}
              >
                <span className="about__tech-label">{g.label}</span>
                <ul className="about__chips">
                  {g.items.map((item) => (
                    <li key={item} className="about__chip">
                      {item}
                    </li>
                  ))}
                </ul>
              </motion.div>
            ))}
          </motion.div>
        </motion.div>
      </div>

      <div className="about__foot-band" aria-hidden="true">
        {progressiveBlur}
      </div>

      <SectionScrollbar visible />
    </motion.main>
  )
}
