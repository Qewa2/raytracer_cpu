#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "core/interval.hpp"
#include "core/ray.hpp"
#include "hittable/hittable.hpp"
#include "materials/material.hpp"

#include <memory>

using std::shared_ptr;

class sphere : public hittable {
  public:
    // Stationary Sphere
    sphere(const point3 &staticCenter, double radius, shared_ptr<material> mat);

    // Moving Sphere
    sphere(const point3 &center1, const point3 &center2, double radius,
           shared_ptr<material> mat);

    virtual bool hit(const ray &r, interval ray_t,
                     hitRecord &rec) const override;

    aabb boundingBox() const override;

  private:
    ray center;
    double radius;
    std::shared_ptr<material> mat;
    aabb bbox;

    static void getSphereUV(const point3 &p, double &u, double &v);
};

#endif // SPHERE_HPP
