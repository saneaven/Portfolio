import PartLanding, { type SectionTitle } from '../PartLanding/PartLanding'
import { CATEGORIES, toSectionPath } from '../../data/sections'

const SECTIONS: SectionTitle[] = CATEGORIES.ai.projects.map((p) => ({
  title: p.title,
  subtitle: p.subtitle,
  image: p.image,
  to: toSectionPath(CATEGORIES.ai.key, p.slug),
}))

export default function AI() {
  return <PartLanding sections={SECTIONS} />
}
