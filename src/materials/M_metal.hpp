#ifndef M_METAL_HPP
#define M_METAL_HPP

#include "core/vec3.hpp"
#include "hittable/hittable.hpp"
#include "materials/material.hpp"
#include "textures/T_solidColor.hpp"
#include "textures/texture.hpp"

#include <memory>

using std::make_shared;
using std::shared_ptr;

class M_metal : public material {
public:
  M_metal(const color &albedo, double fuzz)
      : tex(make_shared<T_solidColor>(albedo)), fuzz(fuzz < 1 ? fuzz : 1) {}

  M_metal(shared_ptr<texture> tex, double fuzz)
      : tex(tex), fuzz(fuzz < 1 ? fuzz : 1) {}

  bool scatter(const ray &r_in, const hitRecord &rec, color &attenuation,
               ray &scattered) const override {
    vec3 reflected = reflect(r_in.direction(), rec.normal);
    reflected = unitVector(reflected) + (fuzz * randomUnitVector());
    scattered = ray(rec.p, reflected, r_in.time());
    attenuation = tex->value(rec.u, rec.v, rec.p);
    return (dot(scattered.direction(), rec.normal) > 0);
  }

private:
  shared_ptr<texture> tex;
  double fuzz;
};

#endif // M_METAL_HPP
