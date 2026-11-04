// Module state resets on reload; only a live category click should play the hero intro.
let pending = false

export const markFromPartLanding = () => {
  pending = true
}

export const takeFromPartLanding = () => {
  const v = pending
  pending = false
  return v
}
