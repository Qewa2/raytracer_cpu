#include "ray.hpp"
#include "core/vec3.hpp"

ray::ray() {}
ray::ray(const point3 &origin, const vec3 &direction, double time)
    : orig(origin), dir(direction), tm(time) {}

ray::ray(const point3 &origin, const vec3 &direction)
    : ray(origin, direction, 0) {}

const point3 &ray::origin() const { return orig; }
const vec3 &ray::direction() const { return dir; }

double ray::time() const { return tm; }

point3 ray::at(double t) const { return orig + t * dir; }
