#ifndef M_DIALECTRIC_HPP
#define M_DIALECTRIC_HPP

#include "core/color.hpp"
#include "core/ray.hpp"
#include "core/utils/utils.hpp"
#include "core/vec3.hpp"
#include "materials/material.hpp"

class M_dialectric : public material {
public:
  M_dialectric(double refractionIndex) : refractionIndex(refractionIndex) {}

  bool scatter(const ray &r_in, const hitRecord &rec, color &attenuation,
               ray &scattered) const override {
    attenuation = color(1.0, 1.0, 1.0);
    double ri = rec.front_face ? (1.0 / refractionIndex) : refractionIndex;

    vec3 unitDirection = unitVector(r_in.direction());
    double cosTheta = std::fmin(dot(-unitDirection, rec.normal), 1.0);
    double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

    bool cannotRefract = ri * sinTheta > 1.0;
    vec3 direction;

    if (cannotRefract || reflectance(cosTheta, ri) > randomDouble()) {
      direction = reflect(unitDirection, rec.normal);
    } else {
      direction = refract(unitDirection, rec.normal, ri);
    }

    scattered = ray(rec.p, direction, r_in.time());
    return true;
  }

private:
  double refractionIndex;

  static double reflectance(double cosine, double refractionIndex) {
    // Use Schlick's approximation for reflectance
    auto r0 = (1 - refractionIndex) / (1 + refractionIndex);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow((1 - cosine), 5);
  }
};

#endif // M_DIALECTRIC_HPP
