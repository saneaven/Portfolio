// Render static HTML per route for no-JS readers.
import { existsSync } from 'node:fs'
import { mkdir, readFile, writeFile } from 'node:fs/promises'
import { dirname, join } from 'node:path'
import { fileURLToPath, pathToFileURL } from 'node:url'

const root = join(dirname(fileURLToPath(import.meta.url)), '..')
const dist = join(root, 'dist')
const { NOT_FOUND_TITLE, ROUTES, SHELL_ROUTES, render, head, sitemapXml, llmsTxt } = await import(
  pathToFileURL(join(root, 'dist-ssr', 'entry-server.js')).href
)

const TITLE = '<title>Saneaven</title>'
const ROOT = '<div id="root"></div>'
const template = await readFile(join(dist, 'index.html'), 'utf8')
for (const marker of [TITLE, ROOT]) {
  if (!template.includes(marker)) throw new Error(`dist/index.html no longer contains ${marker}`)
}

for (const { path } of ROUTES) {
  const html = render(path)
  if (!html) throw new Error(`${path} rendered nothing`)
  // Both builds hash assets from content, so URLs match; fail loudly if they ever drift.
  for (const [asset] of html.matchAll(/\/assets\/[^"'\s)&]+/g)) {
    if (!existsSync(join(dist, asset))) throw new Error(`${path} references missing ${asset}`)
  }
  // /home → home.html: Cloudflare serves it at /home with no trailing-slash redirect.
  const file = join(dist, `${path}.html`)
  await mkdir(dirname(file), { recursive: true })
  // Replacer functions, so a `$` in rendered content isn't read as a replace pattern.
  const page = template.replace(TITLE, () => head(path)).replace(ROOT, () => `<div id="root">${html}</div>`)
  await writeFile(file, page)
}

for (const path of SHELL_ROUTES) await writeFile(join(dist, `${path}.html`), template)

// Served with a 404 status for any path without a file (see wrangler.jsonc).
const notFound = template
  .replace(TITLE, () => `<title>${NOT_FOUND_TITLE}</title>`)
  .replace(ROOT, () => `<div id="root">${render('/404')}</div>`)
await writeFile(join(dist, '404.html'), notFound)

await writeFile(join(dist, 'sitemap.xml'), sitemapXml())
await writeFile(join(dist, 'llms.txt'), llmsTxt())
console.log(`prerendered ${ROUTES.length} routes, sitemap.xml, llms.txt`)
