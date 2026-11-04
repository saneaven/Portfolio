import PartLanding, { type SectionTitle } from '../PartLanding/PartLanding'
import { CATEGORIES, toSectionPath } from '../../data/sections'

const SECTIONS: SectionTitle[] = CATEGORIES.game.projects.map((p) => ({
  title: p.title,
  subtitle: p.subtitle,
  image: p.image,
  to: toSectionPath(CATEGORIES.game.key, p.slug),
}))

export default function Game() {
  return <PartLanding sections={SECTIONS} />
}
