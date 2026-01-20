#ifndef COLOR_HPP
#define COLOR_HPP

#include "Application/gui.hpp"
#include "core/interval.hpp"
#include "core/vec3.hpp"

#include <cmath>
#include <vector>

using color = vec3;

inline double linearToGamma(double linearComponent) {
    if (linearComponent > 0) {
        return std::sqrt(linearComponent);
    }

    return 0;
}

inline void WriteColorToVector(color &rgb, int x, int y, int width) {
    int index = (y * width + x) * 3;

    const interval intensity(0.0f, 1.0f);
    double r = intensity.clamp(rgb.x());
    double g = intensity.clamp(rgb.y());
    double b = intensity.clamp(rgb.z());

    r = linearToGamma(r);
    g = linearToGamma(g);
    b = linearToGamma(b);

    Gui::glfw::pixels[index + 0] =
        static_cast<unsigned char>(std::floor(255.999 * r));
    Gui::glfw::pixels[index + 1] =
        static_cast<unsigned char>(std::floor(255.999 * g));
    Gui::glfw::pixels[index + 2] =
        static_cast<unsigned char>(std::floor(255.999 * b));
}

inline void writeColor(std::ostream &out, const color &c_ptr_pixelColor) {
    auto r = c_ptr_pixelColor.x();
    auto g = c_ptr_pixelColor.y();
    auto b = c_ptr_pixelColor.z();

    // Apply a linear to gamma transform for gamma 2
    r = linearToGamma(r);
    g = linearToGamma(g);
    b = linearToGamma(b);

    // Translate the [0,1] component values to the byte range [0,255].
    static const interval intensity(0.0, 0.999);
    int rbyte = static_cast<int>(255 * intensity.clamp(r));
    int gbyte = static_cast<int>(255 * intensity.clamp(g));
    int bbyte = static_cast<int>(255 * intensity.clamp(b));

    // Write out the pixel color components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif // COLOR_HPP
