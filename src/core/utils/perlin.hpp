#ifndef PERLIN_HPP
#define PERLIN_HPP

#include "core/vec3.hpp"

class perlin {
  public:
    perlin();

    double noise(const point3 &p) const;

    double turbulence(const point3 &p, int depth) const;

  private:
    static const int pointCount = 256;
    vec3 randVec[pointCount];
    int perm_x[pointCount];
    int perm_y[pointCount];
    int perm_z[pointCount];

    static void perlinGeneratePerm(int *p);

    static void permute(int *p, int n);

    static double perlinInterpolate(const vec3 c[2][2][2], double u, double v,
                                    double w);
};

#endif // PERLIN_HPP
