#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "core/color.hpp"
#include "core/ray.hpp"
#include "hittable/hitRecord.hpp"
#include "hittable/hittable.hpp"

class material {
  public:
    virtual ~material() = default;

    virtual bool scatter(const ray &r_in, const hitRecord &rec,
                         color &attenuation, ray &scattered) const {
        return false;
    }
};

#endif // MATERIAL_HPP
