#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include "core/color.hpp"
#include "core/vec3.hpp"

class texture {
public:
  virtual ~texture() = default;

  virtual color value(double u, double v, const point3 &p) const = 0;
};

#endif // TEXTURE_HPP
