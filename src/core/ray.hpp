#ifndef RAY_HPP
#define RAY_HPP

#include "core/vec3.hpp"

class ray {
  public:
    ray();
    ray(const point3 &origin, const vec3 &direction, double time);

    ray(const point3 &origin, const vec3 &direction);

    const point3 &origin() const;
    const vec3 &direction() const;

    double time() const;

    point3 at(double t) const;

  private:
    point3 orig;
    vec3 dir;
    double tm;
};

#endif // RAY_HPP
