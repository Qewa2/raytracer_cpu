#include "Application/gui.hpp"
#include "core/vec3.hpp"
#include "hittable/bvh.hpp"
#include "hittable/sphere.hpp"
#include "materials/M_dialectric.hpp"
#include "materials/M_lambertian.hpp"
#include "materials/M_metal.hpp"
#include "scenes/scene.hpp"
#include "textures/T_checkered.hpp"
#include "textures/T_noise.hpp"

#include <memory>

using std::make_shared;
using std::shared_ptr;

struct S_bouncingSpheres : public scene {
    S_bouncingSpheres() {
        // auto checker = make_shared<T_checkered>(256 * 8, color(0.2, 0.3,
        // 0.1),
        //                                        color(0.9, 0.9, 0.9));
        world.add(make_shared<sphere>(
            point3(0, -1000, 0), 1000,
            make_shared<M_lambertian>(make_shared<T_noise>(4))));

        for (int a = -11; a < 11; a++) {
            for (int b = -11; b < 11; b++) {
                auto choose_mat = randomDouble();
                point3 center(a + 0.9 * randomDouble(), 0.2,
                              b + 0.9 * randomDouble());

                if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                    shared_ptr<material> sphere_material;

                    if (choose_mat < 0.8) {
                        // diffuse
                        auto albedo = color::random() * color::random();
                        sphere_material = make_shared<M_lambertian>(albedo);
                        auto center2 =
                            center + vec3(0, randomDouble(0, 0.5), 0);
                        world.add(make_shared<sphere>(center, center2, 0.2,
                                                      sphere_material));
                    } else if (choose_mat < 0.95) {
                        // M_metal
                        auto albedo = color::random(0.5, 1);
                        auto fuzz = randomDouble(0, 0.5);
                        sphere_material = make_shared<M_metal>(albedo, fuzz);
                        world.add(
                            make_shared<sphere>(center, 0.2, sphere_material));
                    } else {
                        // glass
                        sphere_material = make_shared<M_dialectric>(1.5);
                        world.add(
                            make_shared<sphere>(center, 0.2, sphere_material));
                    }
                }
            }
        }

        auto material1 = make_shared<M_dialectric>(1.5);
        world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

        auto material2 = make_shared<M_lambertian>(color(0.4, 0.2, 0.1));
        world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

        auto material3 = make_shared<M_metal>(color(0.7, 0.6, 0.5), 0.0);
        world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

        world = hittableList(make_shared<bvhNode>(world));

        cam.AspectRatio = 16.0 / 9.0;
        cam.ImageWidth = Gui::glfw::IMAGE_WIDTH;
        cam.pixels = &Gui::glfw::pixels;
        cam.SamplesPerPixel = 100;
        cam.maxDepth = 50;

        cam.vfov = 20;
        cam.lookFrom = point3(13, 2, 3);
        cam.lookAt = point3(0, 0, 0);
        cam.vup = vec3(0, 1, 0);

        cam.defocusAngle = 0.6;
        cam.focusDist = 10.0;
    }
};
