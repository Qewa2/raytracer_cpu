#include "camera.hpp"
#include "Application/gui.hpp"
#include "core/DebugConsole.hpp"
#include "core/color.hpp"
#include "core/utils/utils.hpp"
#include "core/vec3.hpp"
#include "hittable/bvh.hpp"
#include "hittable/hittable.hpp"
#include "hittable/hittableList.hpp"
#include "materials/material.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using std::string;

// Public
camera::camera() : imagePath("image.ppm"), Image(imagePath) {}
camera::camera(const string &ImagePath)
    : imagePath(ImagePath), Image(imagePath) {}

void camera::render(const hittableList &world) {
    initialize();

    hittableList renderWorld;
    if (useBvh) {
        renderWorld = hittableList(std::make_shared<bvhNode>(world));
    } else {
        renderWorld = world;
    }

    Image << "P3\n" << ImageWidth << ' ' << ImageHeight << "\n255\n";

    for (int j = 0; j < ImageHeight; j++) {
        std::ostringstream oss;
        oss << "Scanlines remaining: " << (ImageHeight - j) << " ("
            << 100 - (int)(100.0 * ((double)(ImageHeight - j) / ImageHeight))
            << "% finished)\n";
        std::cout << oss.str();
        Gui::ScanlineLog.Log(oss.str());
        for (int i = 0; i < ImageWidth; i++) {
            color pixelColor(0, 0, 0);
            for (int sample = 0; sample < SamplesPerPixel; sample++) {
                ray r = getRay(i, j);
                pixelColor += rayColor(r, maxDepth, renderWorld);
            }
            color scaled = pixelColor * pixelSamplesScale;
            writeColor(Image, scaled);

            WriteColorToVector(scaled, i, ImageHeight - 1 - j, ImageWidth);
        }
    }
    Image.close();
    Gui::ScanlineLog.Log("Finished Rendering");
    std::clog << "\nDone\n";
}

// Private
void camera::initialize() {
    ImageHeight = static_cast<int>(ImageWidth / AspectRatio);
    if (ImageHeight < 1) {
        ImageHeight = 1;
    }

    pixelSamplesScale = 1.0 / SamplesPerPixel;

    center = lookFrom;

    // Viewport Dimensions
    auto theta = DegreesToRadians(vfov);
    auto h = std::tan(theta / 2);
    auto viewportHeight = 2 * h * focusDist;
    auto viewportWidth = viewportHeight * AspectRatio;

    // Calculate u, v, w vectors
    w = unitVector(lookFrom - lookAt);
    u = unitVector(cross(vup, w));
    v = cross(w, u);

    // Calculate the viewport edges as Vectors
    auto viewport_u = viewportWidth * u;
    auto viewport_v = viewportHeight * -v;

    // Calculate delta vectors  from pixel to pixel
    pixelDelta_u = viewport_u / ImageWidth;
    pixelDelta_v = viewport_v / ImageHeight;

    // Calculate the location of the upperleft Corner
    auto viewportUpperLeft =
        center - (focusDist * w) - viewport_u / 2 - viewport_v / 2;
    pixel00_loc = viewportUpperLeft + (pixelDelta_u + pixelDelta_v) / 2;

    // Calculate the camera defocus disk basis vectors
    auto defocusRadius =
        focusDist * std::tan(DegreesToRadians(defocusAngle / 2));
    defocusDisk_u = u * defocusRadius;
    defocusDisk_v = v * defocusRadius;
}

ray camera::getRay(int i, int j) const {
    // Construct a camera ray origination from the defocus disk and directed
    // at a randomly sampled point around the pixel location i, j

    auto offset = sampleSquare();
    auto pixelSample = pixel00_loc + ((i + offset.x()) * pixelDelta_u) +
                       ((j + offset.y()) * pixelDelta_v);

    auto rayOrigin = (defocusAngle <= 0) ? center : defocusDiskSample();
    auto rayDirection = pixelSample - rayOrigin;
    auto rayTime = randomDouble();

    return ray(rayOrigin, rayDirection, rayTime);
}

vec3 camera::sampleSquare() const {
    // Returns the vector to a random Point in the unit square
    return vec3(randomDouble() - 0.5, randomDouble() - 0.5, 0);
}

point3 camera::defocusDiskSample() const {
    // Returns a random point in the camera defocus disk
    auto p = randomInUnitDisk();
    return center + (p[0] * defocusDisk_u) + (p[1] * defocusDisk_v);
}

color camera::rayColor(const ray &r, int depth, const hittable &world) const {
    if (depth <= 0) {
        return color(0, 0, 0);
    }

    hitRecord rec;

    if (world.hit(r, interval(0.001, infinity), rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat->scatter(r, rec, attenuation, scattered)) {
            return attenuation * rayColor(scattered, depth - 1, world);
        }
        return color(0, 0, 0);
    }

    vec3 unitDirection = unitVector(r.direction());
    auto t = 0.5 * (unitDirection.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1);
}
