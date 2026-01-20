#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstdlib>
#include <limits>
#include <random>

const double pi = 3.1415926535897932385;
const double infinity = std::numeric_limits<double>::infinity();

static std::mt19937 generator(std::random_device{}());

inline double DegreesToRadians(double degrees) {
  return (degrees * pi / 180.0);
}

inline double randomDouble(double min = 0.0, double max = 1.0) {
  std::uniform_real_distribution<double> distribution(min, max);
  return distribution(generator);
}

inline int randomInt(int min, int max) {
  return (int)(randomDouble(min, max + 1));
}

#endif // UTILS_HPP
