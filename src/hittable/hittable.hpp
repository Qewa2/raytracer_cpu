#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "core/interval.hpp"
#include "core/ray.hpp"
#include "hittable/aabb.hpp"
#include "hittable/hitRecord.hpp"

class material;

class hittable {
  public:
    virtual ~hittable() = default;

    virtual bool hit(const ray &r, interval ray_t, hitRecord &rec) const = 0;

    virtual aabb boundingBox() const = 0;
};

#endif // HITTABLE_HPP
