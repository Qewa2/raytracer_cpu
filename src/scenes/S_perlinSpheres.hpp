#include "Application/gui.hpp"
#include "core/color.hpp"
#include "core/vec3.hpp"
#include "hittable/bvh.hpp"
#include "hittable/sphere.hpp"
#include "materials/M_lambertian.hpp"
#include "materials/M_metal.hpp"
#include "scenes/scene.hpp"
#include "textures/T_checkered.hpp"
#include "textures/T_noise.hpp"

#include <memory>

using std::make_shared;
using std::shared_ptr;

struct S_perlinSpheres : public scene {
    S_perlinSpheres() {
        auto pertext = make_shared<T_noise>(4);
        world.add(make_shared<sphere>(point3(0, -1000, 0), 1000,
                                      make_shared<M_lambertian>(pertext)));
        world.add(make_shared<sphere>(point3(0, 2, 0), 2,
                                      make_shared<M_lambertian>(pertext)));

        cam.AspectRatio = 16.0 / 9.0;
        cam.ImageWidth = Gui::glfw::IMAGE_WIDTH;
        cam.pixels = &Gui::glfw::pixels;
        cam.SamplesPerPixel = 100;
        cam.maxDepth = 50;

        cam.vfov = 20;
        cam.lookFrom = point3(13, 2, 3);
        cam.lookAt = point3(0, 0, 0);
        cam.vup = vec3(0, 1, 0);

        cam.defocusAngle = 0;
    }
};
