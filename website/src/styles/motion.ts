import { useSyncExternalStore } from 'react'
import { type Easing } from 'motion/react'

export const EASE_OUT: Easing = [0.16, 1, 0.3, 1]

export const EASE_IN: Easing = [0.42, 0, 1, 1]

export const EASE_IN_OUT: Easing = [0.65, 0, 0.35, 1]

const REDUCED_MOTION = '(prefers-reduced-motion: reduce)'
const subscribeReducedMotion = (onChange: () => void) => {
  const query = window.matchMedia(REDUCED_MOTION)
  query.addEventListener('change', onChange)
  return () => query.removeEventListener('change', onChange)
}

// Match the prerender during hydration, then read the user's preference.
export function useReducedMotion() {
  return useSyncExternalStore(
    subscribeReducedMotion,
    () => window.matchMedia(REDUCED_MOTION).matches,
    () => false,
  )
}
