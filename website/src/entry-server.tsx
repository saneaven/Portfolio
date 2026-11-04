import { StrictMode } from 'react'
import { renderToString } from 'react-dom/server'
import { StaticRouter } from 'react-router-dom'
import App from './App.tsx'
import { INTRO } from './data/about'
import { NOT_FOUND_TITLE, ROUTES, SHELL_ROUTES, SITE_DESCRIPTION, SITE_NAME, SITE_URL, routeMeta } from './data/routes'
import { CATEGORIES, toSectionPath } from './data/sections'

// Build-time entry for scripts/prerender.mjs; never shipped to the browser.

export { NOT_FOUND_TITLE, ROUTES, SHELL_ROUTES }

export function render(url: string) {
  return renderToString(
    <StrictMode>
      <StaticRouter location={url}>
        <App />
      </StaticRouter>
    </StrictMode>,
  )
}

const escapeHtml = (s: string) =>
  s.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;').replace(/"/g, '&quot;')

export function head(url: string) {
  const meta = routeMeta(url)
  const title = escapeHtml(meta.title)
  const description = escapeHtml(meta.description)
  const canonical = `${SITE_URL}${url}`
  return [
    `<title>${title}</title>`,
    `<meta name="description" content="${description}" />`,
    `<link rel="canonical" href="${canonical}" />`,
    `<meta property="og:type" content="website" />`,
    `<meta property="og:site_name" content="${SITE_NAME}" />`,
    `<meta property="og:title" content="${title}" />`,
    `<meta property="og:description" content="${description}" />`,
    `<meta property="og:url" content="${canonical}" />`,
    // Hero images are AVIF, which most link unfurlers can't show; the icon is a PNG.
    `<meta property="og:image" content="${SITE_URL}/icon.png" />`,
    `<meta name="twitter:card" content="summary" />`,
  ].join('\n    ')
}

export function sitemapXml() {
  const urls = ROUTES.map((r) => `  <url><loc>${SITE_URL}${r.path}</loc></url>`)
  return ['<?xml version="1.0" encoding="UTF-8"?>', '<urlset xmlns="http://www.sitemaps.org/schemas/sitemap/0.9">', ...urls, '</urlset>', ''].join('\n')
}

/** llms.txt (llmstxt.org): a markdown index of the site for agents. */
export function llmsTxt() {
  const link = (title: string, path: string, note: string) => `- [${title}](${SITE_URL}${path}): ${note}`
  const categories = Object.values(CATEGORIES).flatMap((cat) => [
    `## ${cat.name}`,
    '',
    link(cat.name, `/${cat.key}`, 'Overview of every project in this category'),
    ...cat.projects.flatMap((p) => [
      link(p.title, toSectionPath(cat.key, p.slug), p.subtitle),
      ...(p.children ?? []).map((c) => `  ${link(c.title, toSectionPath(cat.key, p.slug, c.slug), c.subtitle)}`),
    ]),
    '',
  ])
  return [
    `# ${SITE_NAME}`,
    '',
    `> ${SITE_DESCRIPTION}`,
    '',
    ...INTRO.flatMap((p) => [p.trim(), '']),
    ...categories,
    '## About',
    '',
    link('About Me', '/about-me', 'Intro, tech stack and contact links'),
    link('Home', '/home', 'Entry point to both categories'),
    '',
  ].join('\n')
}
