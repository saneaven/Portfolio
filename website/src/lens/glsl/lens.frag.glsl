#version 300 es
precision highp float;

in vec2 vUv;
out vec4 fragColor;

// Positions and lengths use device pixels; coordinates start at the top-left.
uniform sampler2D uTex;
uniform vec2 uResolution;

uniform vec2 uCenter;
uniform vec2 uSize; // half-extents
uniform float uCornerRadius;
uniform float uRefraction;
uniform float uDispersion;
uniform float uBevelWidth;
uniform float uFrostRadius;
uniform vec3 uTint;
uniform float uTintOpacity;
uniform float uEdgeHighlight;
uniform float uLightAngle; // screen-plane radians

float sdRoundedBox(vec2 p, vec2 b, float r) {
  r = min(r, min(b.x, b.y));
  vec2 q = abs(p) - b + r;
  return min(max(q.x, q.y), 0.0) + length(max(q, 0.0)) - r;
}

// Central differences approximate the outward surface normal.
vec2 sdfNormal(vec2 p, vec2 b, float r) {
  vec2 e = vec2(1.0, 0.0);
  float dx = sdRoundedBox(p + e.xy, b, r) - sdRoundedBox(p - e.xy, b, r);
  float dy = sdRoundedBox(p + e.yx, b, r) - sdRoundedBox(p - e.yx, b, r);
  vec2 g = vec2(dx, dy);
  float len = length(g);
  return len > 0.0 ? g / len : vec2(0.0);
}

// Canvas textures stay unflipped, so texture V follows the top-left pixel Y.
vec3 sampleLod(vec2 pixel, float lod) {
  vec2 uv = vec2(pixel.x / uResolution.x, pixel.y / uResolution.y);
  return textureLod(uTex, uv, lod).rgb;
}

// Golden-angle taps over prefiltered mips keep large frost radii cheap.
vec3 sampleBg(vec2 pixel) {
  if (uFrostRadius <= 0.5) return sampleLod(pixel, 0.0);

  const int TAPS = 12;
  const float GA = 2.39996323; // golden angle, radians

  float lod = max(0.0, log2(uFrostRadius) - 1.0);

  vec3 acc = vec3(0.0);
  float wsum = 0.0;
  for (int i = 0; i < TAPS; i++) {
    float t = (float(i) + 0.5) / float(TAPS);
    float r = sqrt(t) * uFrostRadius; // uniform area distribution
    float a = float(i) * GA;
    vec2 off = vec2(cos(a), sin(a)) * r;
    float w = exp(-1.8 * t);
    acc += sampleLod(pixel + off, lod) * w;
    wsum += w;
  }
  return acc / wsum;
}

void main() {
  vec2 pixel = vec2(vUv.x * uResolution.x, (1.0 - vUv.y) * uResolution.y);
  vec2 p = pixel - uCenter;

  float d = sdRoundedBox(p, uSize, uCornerRadius);
  float aa = fwidth(d) + 1e-4;

  if (d > aa) {
    discard;
  }

  vec2 normal = sdfNormal(p, uSize, uCornerRadius);

  float dist = max(-d, 0.0);
  float t = clamp(dist / max(uBevelWidth, 1.0), 0.0, 1.0);
  float bevel = 1.0 - t;
  bevel *= bevel; // concentrate refraction at the rim

  vec2 basePush = normal * bevel * uRefraction * uBevelWidth;
  float disp = uDispersion * bevel * uBevelWidth * 0.5;

  vec2 base = pixel + basePush;
  vec3 col;
  col.r = sampleBg(base + normal * disp).r;
  col.g = sampleBg(base).g;
  col.b = sampleBg(base - normal * disp).b;

  col = mix(col, uTint, uTintOpacity);

  // Fresnel lights the full rim; paired Blinn-Phong glints model opposite edges.
  const float TILT      = 1.5;
  const float ELEV      = 0.25;
  const float SHININESS = 32.0;
  const float FRESNEL_W = 0.8;

  float rimBand = 1.0 - t;
  float edge = pow(rimBand, 5.0);
  vec3 N = normalize(vec3(normal * rimBand * TILT, 1.0));
  vec3 V = vec3(0.0, 0.0, 1.0);

  float fresnel = pow(1.0 - max(N.z, 0.0), 4.0);

  vec3 L  = normalize(vec3(cos(uLightAngle), -sin(uLightAngle), ELEV));
  vec3 Lb = normalize(vec3(-L.x, -L.y, ELEV));
  float spec  = pow(max(dot(N, normalize(L  + V)), 0.0), SHININESS);
  float spec2 = pow(max(dot(N, normalize(Lb + V)), 0.0), SHININESS) * 0.4;

  col += edge * (spec + spec2 + FRESNEL_W * fresnel) * uEdgeHighlight * 2.5;

  float mask = 1.0 - smoothstep(-aa, aa, d);
  fragColor = vec4(col, mask);
}
