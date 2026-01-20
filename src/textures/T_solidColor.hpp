#ifndef T_SOLIDCOLOR_HPP
#define T_SOLIDCOLOR_HPP

#include "core/color.hpp"
#include "core/vec3.hpp"
#include "textures/texture.hpp"

class T_solidColor : public texture {
public:
  T_solidColor(const color &albedo) : albedo(albedo) {}

  T_solidColor(double red, double green, double blue)
      : T_solidColor(color(red, green, blue)) {}

  color value(double u, double v, const point3 &p) const override {
    return albedo;
  }

private:
  color albedo;
};

#endif // T_SOLIDCOLOR_HPP
