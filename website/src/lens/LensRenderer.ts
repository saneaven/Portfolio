import vertSource from './glsl/lens.vert.glsl?raw'
import fragSource from './glsl/lens.frag.glsl?raw'
import type { LensOptions } from './types'

/** Draws on demand; callers own the animation loop and must dispose GPU resources. */
export class LensRenderer {
  private readonly gl: WebGL2RenderingContext
  private readonly program: WebGLProgram
  private readonly vao: WebGLVertexArrayObject
  private readonly buffer: WebGLBuffer
  private readonly texture: WebGLTexture
  private readonly uniforms: Record<string, WebGLUniformLocation | null>

  private dpr = 1
  private hasBackground = false
  private options: LensOptions | null = null
  // Reuse texture storage until the source dimensions change.
  private texW = 0
  private texH = 0

  constructor(canvas: HTMLCanvasElement) {
    const gl = canvas.getContext('webgl2', {
      antialias: false,
      premultipliedAlpha: false,
    })
    if (!gl) throw new Error('WebGL2 is not supported in this browser')
    this.gl = gl

    this.program = this.createProgram(vertSource, fragSource)
    this.vao = gl.createVertexArray()
    this.buffer = gl.createBuffer()
    this.texture = gl.createTexture()
    // One oversized triangle covers the viewport without an internal seam.
    const loc = gl.getAttribLocation(this.program, 'aPos')
    gl.bindVertexArray(this.vao)
    gl.bindBuffer(gl.ARRAY_BUFFER, this.buffer)
    gl.bufferData(gl.ARRAY_BUFFER, new Float32Array([-1, -1, 3, -1, -1, 3]), gl.STATIC_DRAW)
    gl.enableVertexAttribArray(loc)
    gl.vertexAttribPointer(loc, 2, gl.FLOAT, false, 0, 0)
    gl.bindVertexArray(null)

    gl.bindTexture(gl.TEXTURE_2D, this.texture)
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE)
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE)
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR_MIPMAP_LINEAR)
    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR)
    gl.enable(gl.BLEND)
    gl.blendFunc(gl.SRC_ALPHA, gl.ONE_MINUS_SRC_ALPHA)
    gl.clearColor(0, 0, 0, 0)

    const names = [
      'uTex', 'uResolution', 'uCenter', 'uSize', 'uCornerRadius',
      'uRefraction', 'uDispersion', 'uBevelWidth', 'uFrostRadius',
      'uTint', 'uTintOpacity', 'uEdgeHighlight', 'uLightAngle',
    ]
    this.uniforms = {}
    for (const n of names) this.uniforms[n] = gl.getUniformLocation(this.program, n)
  }

  // Keep canvas rows unflipped; the shader uses top-left coordinates.
  setBackground(source: HTMLCanvasElement): void {
    const gl = this.gl
    const w = source.width
    const h = source.height
    gl.bindTexture(gl.TEXTURE_2D, this.texture)
    if (w !== this.texW || h !== this.texH || !this.hasBackground) {
      gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, source)
      this.texW = w
      this.texH = h
    } else {
      gl.texSubImage2D(gl.TEXTURE_2D, 0, 0, 0, gl.RGBA, gl.UNSIGNED_BYTE, source)
    }
    // Frost samples prefiltered mip levels instead of blurring on the CPU.
    gl.generateMipmap(gl.TEXTURE_2D)
    this.hasBackground = true
  }

  /** CSS dimensions are converted to device pixels here. */
  resize(cssWidth: number, cssHeight: number, dpr: number): void {
    this.dpr = dpr
    const canvas = this.gl.canvas as HTMLCanvasElement
    canvas.width = Math.max(1, Math.round(cssWidth * dpr))
    canvas.height = Math.max(1, Math.round(cssHeight * dpr))
    this.gl.viewport(0, 0, canvas.width, canvas.height)
  }

  setOptions(options: LensOptions): void {
    this.options = options
  }

  render(): void {
    const gl = this.gl
    if (!this.hasBackground || !this.options) return

    const o = this.options
    const dpr = this.dpr
    const canvas = gl.canvas as HTMLCanvasElement

    gl.clear(gl.COLOR_BUFFER_BIT)
    gl.useProgram(this.program)
    gl.bindVertexArray(this.vao)
    gl.activeTexture(gl.TEXTURE0)
    gl.bindTexture(gl.TEXTURE_2D, this.texture)

    const u = this.uniforms
    gl.uniform1i(u.uTex, 0)
    gl.uniform2f(u.uResolution, canvas.width, canvas.height)
    gl.uniform2f(u.uCenter, o.center.x * dpr, o.center.y * dpr)
    gl.uniform2f(u.uSize, o.size.x * dpr, o.size.y * dpr)
    gl.uniform1f(u.uCornerRadius, o.cornerRadius * dpr)
    gl.uniform1f(u.uRefraction, o.refraction)
    gl.uniform1f(u.uDispersion, o.dispersion)
    gl.uniform1f(u.uBevelWidth, o.bevelWidth * dpr)
    gl.uniform1f(u.uFrostRadius, o.frost > 0 ? o.frost * dpr : 0)
    gl.uniform3f(u.uTint, o.tint.r, o.tint.g, o.tint.b)
    gl.uniform1f(u.uTintOpacity, o.tintOpacity)
    gl.uniform1f(u.uEdgeHighlight, o.edgeHighlight)
    gl.uniform1f(u.uLightAngle, (o.lightAngle * Math.PI) / 180)

    gl.drawArrays(gl.TRIANGLES, 0, 3)
  }

  dispose(): void {
    const gl = this.gl
    gl.deleteProgram(this.program)
    gl.deleteVertexArray(this.vao)
    gl.deleteBuffer(this.buffer)
    gl.deleteTexture(this.texture)
  }

  private createProgram(vert: string, frag: string): WebGLProgram {
    const gl = this.gl
    const program = gl.createProgram()
    gl.attachShader(program, this.compileShader(gl.VERTEX_SHADER, vert))
    gl.attachShader(program, this.compileShader(gl.FRAGMENT_SHADER, frag))
    gl.linkProgram(program)
    if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
      const log = gl.getProgramInfoLog(program)
      gl.deleteProgram(program)
      throw new Error(`Lens shader link failed: ${log}`)
    }
    return program
  }

  private compileShader(type: number, source: string): WebGLShader {
    const gl = this.gl
    const shader = gl.createShader(type)
    if (!shader) throw new Error('Failed to create shader')
    gl.shaderSource(shader, source)
    gl.compileShader(shader)
    if (!gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
      const log = gl.getShaderInfoLog(shader)
      gl.deleteShader(shader)
      throw new Error(`Lens shader compile failed: ${log}`)
    }
    return shader
  }
}
