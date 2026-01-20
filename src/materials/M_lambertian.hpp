#ifndef M_LAMBERTIAN_HPP
#define M_LAMBERTIAN_HPP

#include "core/vec3.hpp"
#include "hittable/hittable.hpp"
#include "materials/material.hpp"
#include "textures/T_solidColor.hpp"
#include "textures/texture.hpp"

#include <memory>

using std::make_shared;
using std::shared_ptr;

class M_lambertian : public material {
public:
  M_lambertian(const color &albedo) : tex(make_shared<T_solidColor>(albedo)) {}

  M_lambertian(shared_ptr<texture> tex) : tex(tex) {}

  bool scatter(const ray &r_in, const hitRecord &rec, color &attenuation,
               ray &scattered) const override {
    auto scatterDirection = rec.normal + randomUnitVector();

    // Catch degenerate scatter scatterDirection
    if (scatterDirection.nearZero()) {
      scatterDirection = rec.normal;
    }

    scattered = ray(rec.p, scatterDirection, r_in.time());
    attenuation = tex->value(rec.u, rec.v, rec.p);
    return true;
  }

private:
  shared_ptr<texture> tex;
};

#endif // M_LAMBERTIAN_HPP
