#ifndef T_CHECKERED_HPP
#define T_CHECKERED_HPP

#include "core/color.hpp"
#include "core/vec3.hpp"
#include "textures/T_solidColor.hpp"
#include "textures/texture.hpp"

#include <memory>

using std::make_shared;
using std::shared_ptr;

class T_checkered : public texture {
public:
  T_checkered(double scale, shared_ptr<texture> even, shared_ptr<texture> odd)
      : scale(scale), even(even), odd(odd) {}

  T_checkered(double scale, const color &color1, const color &color2)
      : T_checkered(scale, make_shared<T_solidColor>(color1),
                    make_shared<T_solidColor>(color2)) {}

  color value(double u, double v, const point3 &p) const override {
    /*
    auto xInt = int(std::floor(invScale * p.x()));
    auto yInt = int(std::floor(invScale * p.y()));
    auto zInt = int(std::floor(invScale * p.z()));

    bool isEven = (xInt + yInt + zInt) % 2 == 0;

    return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    */
    int uCheck = (int)(std::floor(u * scale));
    int vCheck = (int)(std::floor(v * scale));

    bool isEven = (uCheck + vCheck) % 2 == 0;
    return isEven ? even->value(u, v, p) : odd->value(u, v, p);
  }

private:
  double scale;
  shared_ptr<texture> even;
  shared_ptr<texture> odd;
};

#endif // T_CHECKERED_HPP
