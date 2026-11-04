import { useEffect, useRef } from 'react'

// Keep the board and its surrounding space intact; fit only the sentence.
export function useFitHomeIntro() {
  const introRef = useRef<HTMLDivElement>(null)
  const sayRef = useRef<HTMLParagraphElement>(null)

  useEffect(() => {
    const intro = introRef.current
    const say = sayRef.current
    if (!intro || !say) return

    const mobile = window.matchMedia('(max-width: 768px)')
    let disposed = false

    const fit = () => {
      if (disposed) return
      if (!mobile.matches) {
        say.style.removeProperty('--home-say-size')
        return
      }

      const style = getComputedStyle(intro)
      const available = intro.getBoundingClientRect().height
        - parseFloat(style.paddingTop) - parseFloat(style.paddingBottom)
      let low = 14
      let high = Math.max(low, Math.min(40, window.innerWidth * 0.076))

      // Measure actual wrapping so a three-line sentence can use more space
      // than a conservative viewport-unit estimate would allow.
      for (let i = 0; i < 10; i++) {
        const size = (low + high) / 2
        say.style.setProperty('--home-say-size', `${size}px`)
        if (say.getBoundingClientRect().height <= available) low = size
        else high = size
      }
      say.style.setProperty('--home-say-size', `${low}px`)
    }

    const observer = new ResizeObserver(fit)
    observer.observe(intro)
    mobile.addEventListener('change', fit)
    void document.fonts.ready.then(fit)
    fit()

    return () => {
      disposed = true
      observer.disconnect()
      mobile.removeEventListener('change', fit)
    }
  }, [])

  return { introRef, sayRef }
}
