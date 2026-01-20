#ifndef T_IMAGE_HPP
#define T_IMAGE_HPP

#include "core/color.hpp"
#include "core/utils/imageLoader.hpp"
#include "core/vec3.hpp"
#include "textures/texture.hpp"

class T_image : public texture {
public:
  T_image(const char *filename) : imageFile(filename) {}

  color value(double u, double v, const point3 &p) const override {
    // no texture data -> pure magenta for debugging
    if (imageFile.height() <= 0) {
      return color(1, 0, 1);
    }

    // Clamp u and v to [0, 1] and [1, 0]
    u = interval(0, 1).clamp(u);
    v = 1.0 - interval(0, 1).clamp(v);

    auto i = int(u * imageFile.width());
    auto j = int(v * imageFile.height());
    auto pixel = imageFile.pixelData(i, j);

    auto colorScale = 1.0 / 255.0;
    return color(colorScale * pixel[0], colorScale * pixel[1],
                 colorScale * pixel[2]);
  }

private:
  imageLoader imageFile;
};

#endif // T_IMAGE_HPP
