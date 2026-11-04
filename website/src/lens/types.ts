/** Spatial values use CSS pixels relative to the viewport; the renderer scales them by DPR. */
export interface LensOptions {
  center: { x: number; y: number }
  /** Half-width and half-height, not the full box dimensions. */
  size: { x: number; y: number }
  /** Clamped to min(size.x, size.y); zero gives sharp corners. */
  cornerRadius: number
  refraction: number
  /** RGB separation at the rim. */
  dispersion: number
  /** Blur radius in CSS pixels. */
  frost: number
  /** Inward depth of the curved rim in CSS pixels. */
  bevelWidth: number
  /** Each channel is in 0..1. */
  tint: { r: number; g: number; b: number }
  /** 0..1. */
  tintOpacity: number
  /** Rim-light strength, 0..1. */
  edgeHighlight: number
  /** Degrees: 0 = right, 90 = up. */
  lightAngle: number
}

export const DEFAULT_LENS_OPTIONS: LensOptions = {
  center: { x: 0, y: 0 },
  size: { x: 160, y: 160 },
  cornerRadius: 160,
  refraction: 0.75,
  dispersion: 0.25,
  frost: 8,
  bevelWidth: 24,
  tint: { r: 1, g: 1, b: 1 },
  tintOpacity: 0.01,
  edgeHighlight: 0.25,
  lightAngle: 135,
}
