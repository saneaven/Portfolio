export function scrollToAnchor(id: string, behavior: ScrollBehavior) {
  const el = document.getElementById(id)
  el?.scrollIntoView({ block: 'start', behavior })
  return el
}

export function hashTarget() {
  const id = decodeURIComponent(window.location.hash.slice(1))
  return id ? document.getElementById(id) : null
}

const RELEASE_ON = ['wheel', 'touchstart', 'keydown', 'pointerdown'] as const
const HOLD_MS = 4000

// Keep the target aligned as media loads, until reader input or the timeout.
export function landOnAnchor(el: HTMLElement, root: HTMLElement) {
  const realign = () => el.scrollIntoView({ block: 'start', behavior: 'instant' })
  realign()
  const ro = new ResizeObserver(realign)
  ro.observe(root)
  const timer = window.setTimeout(release, HOLD_MS)
  RELEASE_ON.forEach((type) => window.addEventListener(type, release, { passive: true }))

  function release() {
    ro.disconnect()
    window.clearTimeout(timer)
    RELEASE_ON.forEach((type) => window.removeEventListener(type, release))
  }
  return release
}
