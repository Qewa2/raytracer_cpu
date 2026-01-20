#include "Application/gui.hpp"
#include "core/vec3.hpp"
#include "hittable/bvh.hpp"
#include "hittable/sphere.hpp"
#include "materials/M_dialectric.hpp"
#include "materials/M_lambertian.hpp"
#include "materials/M_metal.hpp"
#include "scenes/scene.hpp"
#include "textures/T_checkered.hpp"
#include "textures/T_image.hpp"

#include <memory>

using std::make_shared;
using std::shared_ptr;

struct S_earth : public scene {
    S_earth() {
        auto earthTexture = make_shared<T_image>("earthmap.jpg");
        auto earthSurface = make_shared<M_lambertian>(earthTexture);
        auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earthSurface);

        world.add(globe);

        cam.AspectRatio = 16.0 / 9.0;
        cam.ImageWidth = Gui::glfw::IMAGE_WIDTH;
        cam.SamplesPerPixel = 100;
        cam.maxDepth = 50;

        cam.vfov = 20;
        cam.lookFrom = point3(0, 0, 12);
        cam.lookAt = point3(0, 0, 0);
        cam.vup = vec3(0, 1, 0);

        cam.defocusAngle = 0;
    }
};
