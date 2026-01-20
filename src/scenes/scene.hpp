#ifndef SCENE_HPP
#define SCENE_HPP

#include "core/camera.hpp"
#include "hittable/hittableList.hpp"

struct scene {
    hittableList world;
    camera cam;

    virtual ~scene() = default;

    void render() { cam.render(world); }
};

#endif // SCENE_HPP
