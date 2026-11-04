// Colors shared with CSS must match their values in src/style.css.
export const PALETTE = {
  blue: 'oklch(62.99% 0.1722 248.65)',
  teal: 'oklch(43.06% 0.0850 229.18)',
  green: 'oklch(63.43% 0.1378 158.93)',
  greenPale: 'oklch(95.03% 0.1160 130.59)',
  leaf: 'oklch(68.00% 0.1600 145.00)',
  gold: 'oklch(82.17% 0.1682 92.47)',
  honey: 'oklch(80.00% 0.1650 82.00)',
  crimson: 'oklch(59.42% 0.2149 18.92)',
  red: 'oklch(61.59% 0.2262 28.66)',
  pine: 'oklch(26.78% 0.0321 168.52)',
  burgundy: 'oklch(36.85% 0.0899 356.93)',
} as const

export const MAIN_DARK = 'oklch(14.96% 0.0000 89.88)'
export const MAIN_LIGHT = 'oklch(93.47% 0.0046 78.30)'

/** Accepts opaque oklch(L% C H) colors authored in this palette. */
export function alpha(c: string, a: number): string {
  return c.replace(/\)\s*$/, ` / ${a})`)
}

// L stays on the authored 0..100 percentage scale.
function parseOklch(s: string): [number, number, number] {
  const m = s.match(/oklch\(\s*([\d.]+)%?\s+([\d.]+)\s+([\d.]+)/)
  return m ? [+m[1], +m[2], +m[3]] : [0, 0, 0]
}

// Canvas interpolates in sRGB; sample the shorter OKLCH hue arc to match the CSS gradient.
export function oklchGradientStops(grad: CanvasGradient, from: string, to: string, steps = 8): void {
  const [l0, c0, h0] = parseOklch(from)
  const [l1, c1, h1] = parseOklch(to)
  let dh = h1 - h0
  if (dh > 180) dh -= 360
  else if (dh < -180) dh += 360
  for (let i = 0; i <= steps; i++) {
    const t = i / steps
    grad.addColorStop(t, `oklch(${l0 + (l1 - l0) * t}% ${c0 + (c1 - c0) * t} ${h0 + dh * t})`)
  }
}
