import { CATEGORIES, toSectionPath, type Category, type Project } from './sections'
import { INTRO, NAME, ROLE } from './about'

export const SITE_URL = 'https://saneaven.com'
export const SITE_NAME = 'Saneaven'

// Absolute production links let crawlers discover routes from prerendered HTML.
export const siteHref = (path: string) => (import.meta.env.DEV ? path : `${SITE_URL}${path}`)

const categories = Object.values(CATEGORIES)
export const SITE_DESCRIPTION = `Portfolio of ${NAME}, ${ROLE}: ${categories.map((c) => c.name).join(' and ')} projects.`

type RouteMeta = { path: string; title: string; description: string }

const titled = (title: string) => `${title} | ${SITE_NAME}`
const titles = (projects: Project[]) => projects.map((p) => p.title).join(', ')

function projectRoutes(cat: Category, project: Project): RouteMeta[] {
  const children = project.children ?? []
  if (!children.length) {
    return [{ path: toSectionPath(cat.key, project.slug), title: titled(project.title), description: project.subtitle }]
  }
  // Both categories have an "Other Projects" hub, so its title carries the category.
  return [
    {
      path: toSectionPath(cat.key, project.slug),
      title: titled(`${project.title}: ${cat.name}`),
      description: `${project.subtitle}: ${titles(children)}.`,
    },
    ...children.map((c) => ({ path: toSectionPath(cat.key, project.slug, c.slug), title: titled(c.title), description: c.subtitle })),
  ]
}

// Derived from CATEGORIES so new projects are included in prerendering and the sitemap.
export const ROUTES: RouteMeta[] = [
  { path: '/home', title: SITE_NAME, description: SITE_DESCRIPTION },
  { path: '/about-me', title: titled('About Me'), description: INTRO[0].trim() },
  ...categories.flatMap((cat) => [
    { path: `/${cat.key}`, title: titled(cat.name), description: `${cat.name} projects by ${NAME}: ${titles(cat.projects)}.` },
    ...cat.projects.flatMap((p) => projectRoutes(cat, p)),
  ]),
]

// Client-only pages: no static markup, but they need a file or the server answers 404.
export const SHELL_ROUTES = ['/intro', '/lens']

export const NOT_FOUND_TITLE = titled('Page Not Found')

export function routeMeta(pathname: string): RouteMeta {
  return ROUTES.find((r) => r.path === pathname) ?? { path: pathname, title: SITE_NAME, description: SITE_DESCRIPTION }
}
