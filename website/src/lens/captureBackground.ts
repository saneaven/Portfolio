import { toCanvas } from 'html-to-image'

const LENS_IGNORE_ATTR = 'data-lens-ignore'

export async function captureBackground(
  node: HTMLElement = document.body,
  pixelRatio: number = window.devicePixelRatio || 1,
): Promise<HTMLCanvasElement> {
  return toCanvas(node, {
    pixelRatio,
    // Exclude the lens itself to prevent capture feedback.
    filter: (el) =>
      !(el instanceof HTMLElement && el.hasAttribute(LENS_IGNORE_ATTR)),
  })
}
