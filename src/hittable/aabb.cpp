#include "aabb.hpp"
#include "core/interval.hpp"
#include "core/ray.hpp"
#include "core/vec3.hpp"

aabb::aabb() {} // default AABB is empty since intervals are empty

aabb::aabb(const interval &x, const interval &y, const interval &z)
    : x(x), y(y), z(z) {
    padToMinimum();
}

aabb::aabb(const point3 &a, const point3 &b) {
    // a and b are the points for the bounding box

    x = (a[0] <= b[0]) ? interval(a[0], b[0]) : interval(b[0], a[0]);
    y = (a[1] <= b[1]) ? interval(a[1], b[1]) : interval(b[1], a[1]);
    z = (a[2] <= b[2]) ? interval(a[2], b[2]) : interval(b[2], a[2]);

    padToMinimum();
}

aabb::aabb(const aabb &bbox0, const aabb &bbox1) {
    x = interval(bbox0.x, bbox1.x);
    y = interval(bbox0.y, bbox1.y);
    z = interval(bbox0.z, bbox1.z);
}

const interval &aabb::axisInterval(int n) const {
    if (n == 1) {
        return y;
    }
    if (n == 2) {
        return z;
    }
    return x;
}

bool aabb::hit(const ray &r, interval ray_t) const {
    const point3 &rayOrig = r.origin();
    const vec3 &rayDir = r.direction();

    for (int axis = 0; axis < 3; axis++) {
        const interval &ax = axisInterval(axis);
        const double adinv = 1.0 / rayDir[axis];

        auto t0 = (ax.min - rayOrig[axis]) * adinv;
        auto t1 = (ax.max - rayOrig[axis]) * adinv;

        if (t0 < t1) {
            if (t0 > ray_t.min) {
                ray_t.min = t0;
            }
            if (t1 < ray_t.max) {
                ray_t.max = t1;
            }
        } else {
            if (t1 > ray_t.min) {
                ray_t.min = t1;
            }
            if (t0 < ray_t.max) {
                ray_t.max = t0;
            }
        }

        if (ray_t.max <= ray_t.min) {
            return false;
        }
    }
    return true;
}

int aabb::longestAxis() const {
    // Returns index of the largest axis
    if (x.size() > y.size() && x.size() > z.size()) {
        return 0;
    } else if (y.size() > x.size() && y.size() > z.size()) {
        return 1;
    } else if (z.size() > x.size() && z.size() > y.size()) {
        return 2;
    }
    return -1;
}

const aabb aabb::empty =
    aabb(interval::empty, interval::empty, interval::empty);
const aabb aabb::universe =
    aabb(interval::universe, interval::universe, interval::universe);

// Private
void aabb::padToMinimum() {
    double delta = 0.0001f;
    if (x.size() < delta) {
        x = x.expand(delta);
    }
    if (y.size() < delta) {
        y = y.expand(delta);
    }
    if (z.size() < delta) {
        z = z.expand(delta);
    }
}
