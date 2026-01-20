#ifndef AABB_HPP
#define AABB_HPP

#include "core/interval.hpp"
#include "core/ray.hpp"
#include "core/vec3.hpp"

class aabb {
  public:
    interval x, y, z;

    aabb(); // default AABB is empty since intervals are empty

    aabb(const interval &x, const interval &y, const interval &z);

    aabb(const point3 &a, const point3 &b);

    aabb(const aabb &bbox0, const aabb &bbox1);

    const interval &axisInterval(int n) const;

    bool hit(const ray &r, interval ray_t) const;

    int longestAxis() const;

    static const aabb empty;
    static const aabb universe;

  private:
    void padToMinimum();
};

#endif
