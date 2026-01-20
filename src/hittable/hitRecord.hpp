#ifndef HITRECORD_HPP
#define HITRECORD_HPP

#include "core/ray.hpp"
#include "core/vec3.hpp"

#include <memory>

class material;

struct hitRecord {
    point3 p;
    vec3 normal;
    std::shared_ptr<material> mat;
    double t;
    double u; // Hit Coords for texture Mapping
    double v; // Same here
    bool front_face;

    void setFaceNormal(const ray &r, const vec3 &outward_normal) {
        // 'outward_normal' is assumend to have unit lengt

        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

#endif // HITRECORD_HPP
