#include "perlin2D.hpp"

Perlin2D::Perlin2D() {}

float Perlin2D::interpolate(float a0, float a1, float t) {
  return (a1 - a0) * t + a0;
}

glm::vec2 Perlin2D::randGrad(int xi, int yi) {
  const unsigned w = 8 * sizeof(unsigned);
  const unsigned s = w / 2; // rotation width
  unsigned a = xi, b = yi;
  a *= 3284157443;
  b ^= a << s | a >> w - s;
  b *= 1911520717;
  a ^= b << s | b >> w - s;
  a *= 2048419325;
  float random = a * (3.14159265 / ~(~0u >> 1)); // in [0, 2*Pi]
  glm::vec2 v;
  v.x = cos(random);
  v.y = sin(random);
  return v;
}

float Perlin2D::dotGrad(int xi, int yi, float x, float y)
{
    glm::vec2 grad = randGrad(xi, yi);
    float dx = x - (float)xi;
    float dy = y - (float)yi;

    return (dx*grad.x + dy*grad.y);
}

float Perlin2D::perlin(float x, float y) {
    // Determine grid cell coordinates
    int x0 = (int)floor(x);
    int x1 = x0 + 1;
    int y0 = (int)floor(y);
    int y1 = y0 + 1;

    // Determine interpolation weights
    // Could also use higher order polynomial/s-curve here
    float sx = x - (float)x0;
    float sy = y - (float)y0;

    // Interpolate between grid point gradients
    float n0, n1, ix0, ix1, value;

    n0 = dotGrad(x0, y0, x, y);
    n1 = dotGrad(x1, y0, x, y);
    ix0 = interpolate(n0, n1, sx);

    n0 = dotGrad(x0, y1, x, y);
    n1 = dotGrad(x1, y1, x, y);
    ix1 = interpolate(n0, n1, sx);

    value = interpolate(ix0, ix1, sy);
    return (value * 0.5) + 0.5; // Will return in range -1 to 1. To make it in range 0 to 1, multiply by 0.5 and add 0.5
}
