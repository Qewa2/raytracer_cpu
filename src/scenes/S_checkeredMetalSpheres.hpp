#include "Application/gui.hpp"
#include "hittable/bvh.hpp"
#include "hittable/sphere.hpp"
#include "materials/M_lambertian.hpp"
#include "materials/M_metal.hpp"
#include "scenes/scene.hpp"
#include "textures/T_checkered.hpp"

#include <memory>

using std::make_shared;
using std::shared_ptr;

struct S_checkeredMetalSpheres : public scene {
    S_checkeredMetalSpheres() {
        auto checker = make_shared<T_checkered>(
            32, make_shared<T_solidColor>(color(1.0, 0, 1.0)),
            make_shared<T_solidColor>(color(1.0, 1.0, 1.0)));

        world.add(make_shared<sphere>(point3(0, -10, 0), 10,
                                      make_shared<M_metal>(checker, 1)));
        world.add(make_shared<sphere>(point3(0, 10, 0), 10,
                                      make_shared<M_metal>(checker, 20)));

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
