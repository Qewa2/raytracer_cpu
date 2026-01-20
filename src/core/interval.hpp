#ifndef INTERVAL_HPP
#define INTERVAL_HPP

#include "core/utils/utils.hpp"

class interval {
  public:
    double min;
    double max;

    // Default initializer is empty
    interval();

    interval(double min, double max);

    interval(const interval &a, const interval &b);

    double size() const;

    bool contains(double x) const;

    bool surrounds(double x) const;

    double clamp(double x) const;

    interval expand(double delta) const;

    static const interval empty;
    static const interval universe;
};

#endif // INTERVAL_HPP
