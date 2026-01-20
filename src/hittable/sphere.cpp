#include "sphere.hpp"
#include "core/interval.hpp"
#include "core/ray.hpp"
#include "core/utils/utils.hpp"
#include "hittable/hittable.hpp"
#include "materials/material.hpp"

#include <cmath>
#include <memory>

using std::shared_ptr;

sphere::sphere(const point3 &staticCenter, double radius,
               shared_ptr<material> mat)
    : center(staticCenter, vec3(0, 0, 0)), radius(std::fmax(0, radius)),
      mat(mat) {
    auto rvec = vec3(radius, radius, radius);
    bbox = aabb(staticCenter - rvec, staticCenter + rvec);
};

// Moving Sphere
sphere::sphere(const point3 &center1, const point3 &center2, double radius,
               shared_ptr<material> mat)
    : center(center1, center2 - center1), radius(std::fmax(0, radius)),
      mat(mat) {
    auto rvec = vec3(radius, radius, radius);
    aabb box1(center.at(0) - rvec, center.at(0) + rvec);
    aabb box2(center.at(1) - rvec, center.at(1) + rvec);
    bbox = aabb(box1, box2);
}

bool sphere::hit(const ray &r, interval ray_t, hitRecord &rec) const {
    point3 currentCenter = center.at(r.time());
    vec3 oc = currentCenter - r.origin();
    auto a = r.direction().lengthSquared();
    auto h = dot(r.direction(), oc);
    auto c = oc.lengthSquared() - radius * radius;

    auto discriminant = h * h - a * c;
    if (discriminant < 0)
        return false;

    auto sqrtd = std::sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range.
    auto root = (h - sqrtd) / a;
    if (!ray_t.surrounds(root)) {
        root = (h + sqrtd) / a;
        if (!ray_t.surrounds(root))
            return false;
    }

    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outwardNormal = (rec.p - currentCenter) / radius;
    rec.setFaceNormal(r, outwardNormal);
    getSphereUV(outwardNormal, rec.u, rec.v);
    rec.mat = mat;

    return true;
}

aabb sphere::boundingBox() const { return bbox; }

// Private
void sphere::getSphereUV(const point3 &p, double &u, double &v) {
    // p: a given point on the sphere of radius one, centered at the origin.
    // u: returned value [0,1] of angle around the Y axis from X=-1.
    // v: returned value [0,1] of angle from Y=-1 to Y=+1.
    //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
    //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
    //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

    auto theta = std::acos(-p.y());
    auto phi = std::atan2(-p.z(), p.x()) + pi;

    u = phi / (2 * pi);
    v = theta / pi;
}
