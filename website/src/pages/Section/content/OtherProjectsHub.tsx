import { useEffect, useLayoutEffect, useRef, type MouseEvent } from 'react'
import { useLocation, useNavigate, useParams } from 'react-router-dom'
import { type CategoryKey, type Project, toSectionPath } from '../../../data/sections'
import { siteHref } from '../../../data/routes'
import { isPlainClick } from '../../../components/plainClick'
import ContentBlocks from './contentblock/ContentBlocks'
import { sectionAnchors } from './contentblock/anchors'
import SectionToc from './SectionToc'
import './OtherProjectsHub.css'

type Gradient = { from: string; to: string }

function useActiveChild(project: Project) {
  const { child } = useParams()
  const children = project.children ?? []
  const active = children.find((c) => c.slug === child) ?? children[0]
  return { children, active }
}

export default function OtherProjectsHub({
  category,
  project,
  gradient,
}: {
  category: CategoryKey
  project: Project
  gradient: Gradient
}) {
  const { active } = useActiveChild(project)
  const navigate = useNavigate()
  const { search, hash } = useLocation()

  // Preserve links that selected a child with ?p= before the :child route existed.
  useEffect(() => {
    const legacy = new URLSearchParams(search).get('p')
    if (legacy) navigate({ pathname: toSectionPath(category, project.slug, legacy), hash }, { replace: true })
  }, [search, hash, category, project.slug, navigate])

  if (!active) return null

  return <ContentBlocks key={active.slug} blocks={active.blocks} gradient={gradient} />
}

export function HubNav({
  category,
  project,
  onNavigate,
}: {
  category: CategoryKey
  project: Project
  onNavigate?: () => void
}) {
  const { children, active } = useActiveChild(project)
  const navigate = useNavigate()

  const listRef = useRef<HTMLUListElement>(null)
  useLayoutEffect(() => {
    const list = listRef.current
    const item = list?.querySelector('[aria-current="page"]')?.parentElement
    if (!list || !item) return
    list.scrollTop += item.getBoundingClientRect().top - list.getBoundingClientRect().top
  }, [active?.slug])

  const select = (slug: string) => (e: MouseEvent) => {
    if (!isPlainClick(e)) return
    e.preventDefault()
    navigate(toSectionPath(category, project.slug, slug))
    window.scrollTo({ top: window.innerHeight * 0.5, behavior: 'smooth' })
    onNavigate?.()
  }

  return (
    <ul className="op-hub__list" ref={listRef}>
      {children.map((c) => (
        <li key={c.slug}>
          <a
            className="op-hub__item"
            href={siteHref(toSectionPath(category, project.slug, c.slug))}
            aria-current={c === active ? 'page' : undefined}
            onClick={select(c.slug)}
          >
            <span className="op-hub__item-title">{c.title}</span>
            <span className="op-hub__item-sub">{c.subtitle}</span>
          </a>
          {c === active && <SectionToc anchors={sectionAnchors(c.blocks)} onNavigate={onNavigate} />}
        </li>
      ))}
    </ul>
  )
}
