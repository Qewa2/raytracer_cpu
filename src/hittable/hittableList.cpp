#include "hittableList.hpp"
#include "core/interval.hpp"
#include "hittable/hittable.hpp"

#include <memory>
#include <vector>

hittableList::hittableList() {}
hittableList::hittableList(std::shared_ptr<hittable> ptr_object) {
    add(ptr_object);
}

void hittableList::clear() { objects.clear(); }
void hittableList::add(std::shared_ptr<hittable> ptr_object) {
    objects.push_back(ptr_object);
    bbox = aabb(bbox, ptr_object->boundingBox());
}

bool hittableList::hit(const ray &r, interval ray_t, hitRecord &rec) const {
    hitRecord tempRec;
    bool hitAnything = false;
    auto closest_so_far = ray_t.max;

    for (const auto &ptr_object : objects) {
        if (ptr_object->hit(r, interval(ray_t.min, closest_so_far), tempRec)) {
            hitAnything = true;
            closest_so_far = tempRec.t;
            rec = tempRec;
        }
    }

    return hitAnything;
};

aabb hittableList::boundingBox() const { return bbox; }
