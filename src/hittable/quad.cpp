#include "quad.hpp"

#include "core/interval.hpp"
#include "hittable/aabb.hpp"
#include "hittable/hitRecord.hpp"
#include "hittable/hittable.hpp"

quad::quad(const point3 &Q, const vec3 &u, const vec3 &v,
           shared_ptr<material> mat)
    : Q(Q), u(u), v(v), mat(mat) {
    setBoundingBox();
}

void quad::setBoundingBox() {
    auto bboxDiagonal1 = aabb(Q, Q + u + v);
    auto bboxDiagonal2 = aabb(Q + u, Q + v);
    bbox = aabb(bboxDiagonal1, bboxDiagonal2);
}

aabb quad::boundingBox() const { return bbox; }

bool quad::hit(const ray &r, interval ray_t, hitRecord &rec) const {
    return false; // To be implemented
}
