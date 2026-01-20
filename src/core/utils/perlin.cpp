#include "perlin.hpp"

#include "core/utils/utils.hpp"
#include "core/vec3.hpp"
#include <cmath>

// Public
perlin::perlin() {
    for (int i = 0; i < pointCount; i++) {
        randVec[i] = unitVector(vec3::random(-1, 1));
    }

    perlinGeneratePerm(perm_x);
    perlinGeneratePerm(perm_y);
    perlinGeneratePerm(perm_z);
}

double perlin::noise(const point3 &p) const {
    auto u = p.x() - std::floor(p.x());
    auto v = p.y() - std::floor(p.y());
    auto w = p.z() - std::floor(p.z());

    auto i = int(std::floor(p.x()));
    auto j = int(std::floor(p.y()));
    auto k = int(std::floor(p.z()));
    vec3 c[2][2][2];

    for (int di = 0; di < 2; di++)
        for (int dj = 0; dj < 2; dj++)
            for (int dk = 0; dk < 2; dk++)
                c[di][dj][dk] =
                    randVec[perm_x[(i + di) & 255] ^ perm_y[(j + dj) & 255] ^
                            perm_z[(k + dk) & 255]];

    return perlinInterpolate(c, u, v, w);
}

double perlin::turbulence(const point3 &p, int depth) const {
    auto accum = 0.0f;
    auto tempPoint = p;
    auto weight = 1.0f;

    for (int i = 0; i < depth; i++) {
        accum += weight * noise(tempPoint);
        weight *= 0.5f;
        tempPoint *= 2;
    }

    return std::fabs(accum);
}

// Private
void perlin::perlinGeneratePerm(int *p) {
    for (int i = 0; i < pointCount; i++)
        p[i] = i;

    permute(p, pointCount);
}

void perlin::permute(int *p, int n) {
    for (int i = n - 1; i > 0; i--) {
        int target = randomInt(0, i);
        int tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
    }
}

double perlin::perlinInterpolate(const vec3 c[2][2][2], double u, double v,
                                 double w) {
    auto uu = u * u * (3 - 2 * u);
    auto vv = v * v * (3 - 2 * v);
    auto ww = w * w * (3 - 2 * w);
    auto accum = 0.0;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                vec3 weightV(u - i, v - j, w - k);
                accum += (i * uu + (1 - i) * (1 - uu)) *
                         (j * vv + (1 - j) * (1 - vv)) *
                         (k * ww + (1 - k) * (1 - ww)) *
                         dot(c[i][j][k], weightV);
            }
        }
    }
    return accum;
}
