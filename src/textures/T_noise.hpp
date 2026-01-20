#ifndef T_PERLIN_HPP
#define T_PERLIN_HPP

#include "core/utils/perlin.hpp"
#include "texture.hpp"

class T_noise : public texture {
  public:
    T_noise(double scale) : scale(scale) {}

    color value(double u, double v, const point3 &p) const override {
        return color(0.5f, 0.5f, 0.5f) *
               (1 + std::sin(scale * p.z() + 10 * noise.turbulence(p, 7)));
    }

  private:
    double scale;
    perlin noise;
};

#endif
