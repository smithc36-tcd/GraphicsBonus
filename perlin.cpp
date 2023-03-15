#include "perlin.hpp"

Perlin::Perlin() {}

std::vector<float> Perlin::generate_noise_map() {
    
  std::vector<float> NormalisedNoiseValues((chunkHeight * chunkWidth), 1.0);

  float amp = 2.0, freq = 0.05, maxHeight = 0.0;

  for (int i = 0; i < octaves; i++) {
    maxHeight += amp;
    amp *= persistance;
  }

  for (int y = 0; y < chunkHeight; y++) {
    for (int x = 0; x < chunkWidth; x++) {
        amp = 1;
        freq = 1;
      for (int i = 0; i < octaves; i++) {
        float xSample =
            (float)(x * (chunkWidth - 1)) / noiseScale * freq;
        float ySample =
            (float)(y * (chunkHeight - 1)) / noiseScale * freq;

        float perlinValue = Perlin_Noise(xSample, ySample);
        NormalisedNoiseValues[x + (y * chunkWidth)] += perlinValue * amp;

        amp *= persistance;
        freq *= lacunarity;
      }
      NormalisedNoiseValues[x + y*chunkWidth] /= maxHeight;
    }
  }

  return NormalisedNoiseValues;
}

double Perlin::Perlin_Noise(float x, float y) {

  const std::vector<int> p = get_permuation_vector();
  float z = 0.5;
  // Create x, y, z coordinates to represent the unit cube that the
  // coordinate is in
  int xi = (int)floor(x) & 255;
  int yi = (int)floor(y) & 255;
  int zi = (int)floor(z) & 255;

  // Relative position in cube
  double xf = x - floor(x);
  double yf = y - floor(y);
  double zf = z - floor(z);

  double u = _fade(xf);
  double v = _fade(yf);
  double w = _fade(zf);

  int A, AA, AB, B, BA, BB;
  A = p[xi] + yi;
  AA = p[A] + zi;
  AB = p[A + 1] + zi;
  B = p[xi + 1] + yi;
  BA = p[B] + zi;
  BB = p[B + 1] + zi;

  // Tri-linear interpolation
  double x1, x2, y1, y2;
  x1 = _lerp(u, _grad(p[AA], xf, yf, zf), _grad(p[BA], xf - 1, yf, zf));
  x2 = _lerp(u, _grad(p[AB], xf, yf - 1, zf), _grad(p[BB], xf - 1, yf - 1, zf));
  y1 = _lerp(x1, x2, v);
  x1 = _lerp(u, _grad(p[AA + 1], xf, yf, zf - 1),
             _grad(p[BA + 1], xf - 1, yf, zf - 1));
  x2 = _lerp(u, _grad(p[AB + 1], xf, yf - 1, zf - 1),
             _grad(p[BB + 1], xf - 1, yf - 1, zf - 1));
  y2 = _lerp(x1, x2, v);

  double noise = _lerp(y1, y2, w);

  return noise;
}

// fade function as defined by Ken Perlin
// returns 6t^5 - 15t^4 + 10^t
// used to smooth the final output
double Perlin::_fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }

// Linear interpolation
double Perlin::_lerp(double t, double a, double b) { return a + t * (a - b); }

// calculate the dot product of a randomly selected gradient vector and the 8
// location vectors
double Perlin::_grad(int hash, double x, double y, double z) {
  int h = hash & 15;        // Convert to BYTE
  double u = h < 8 ? x : y, // Into 12 gradient directions
      v = h < 4                ? y
          : h == 12 || h == 14 ? x
                               : z;

  return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

std::vector<int> Perlin::get_permuation_vector() {
  std::vector<int> permutations;
  permutations.reserve(2 * 256); // Double to avoid overflow

  for (int i = 0; i < 256 * 2; i++) {
    permutations.push_back(permutation[i % 256]);
  }
  return permutations;
}
