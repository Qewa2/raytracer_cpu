#include "Application/gui.hpp"
#include "core/vec3.hpp"
#include "hittable/bvh.hpp"
#include "hittable/sphere.hpp"
#include "materials/M_dialectric.hpp"
#include "materials/M_lambertian.hpp"
#include "materials/M_metal.hpp"
#include "scenes/scene.hpp"
#include "textures/T_checkered.hpp"

#include <memory>

using std::make_shared;
using std::shared_ptr;

struct S_checkeredSpheres : public scene {
    S_checkeredSpheres() {
        auto checker = make_shared<T_checkered>(32, color(0.2, 0.3, 0.1),
                                                color(0.9, 0.9, 0.9));

        world.add(make_shared<sphere>(point3(0, -10, 0), 10,
                                      make_shared<M_lambertian>(checker)));
        world.add(make_shared<sphere>(point3(0, 10, 0), 10,
                                      make_shared<M_lambertian>(checker)));

        cam.AspectRatio = 16.0 / 9.0;
        cam.ImageWidth = Gui::glfw::IMAGE_WIDTH;
        cam.SamplesPerPixel = 100;
        cam.maxDepth = 50;

        cam.vfov = 20;
        cam.lookFrom = point3(13, 2, 3);
        cam.lookAt = point3(0, 0, 0);
        cam.vup = vec3(0, 1, 0);

        cam.defocusAngle = 0;
    }
};
