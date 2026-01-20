#include "bvh.hpp"
#include "hittable/hittable.hpp"
#include "hittable/hittableList.hpp"

#include <algorithm>
#include <cstddef>
#include <memory>

using std::make_shared;
using std::shared_ptr;

bvhNode::bvhNode(hittableList list)
    : bvhNode(list.objects, 0, list.objects.size()) {
    // There's a C++ subtlety here. This constructor (without span indices)
    // creates an implicit copy of the hittable list, which we will modify. The
    // lifetime of the copied list only extends until this constructor exits.
    // That's OK, because we only need to persist the resulting bounding volume
    // hierarchy.
}

bvhNode::bvhNode(std::vector<shared_ptr<hittable>> &objects, size_t start,
                 size_t end) {
    bbox = aabb::empty;
    for (size_t objectIndex = start; objectIndex < end; objectIndex++) {
        bbox = aabb(bbox, objects[objectIndex]->boundingBox());
    }

    int axis = bbox.longestAxis();

    auto comperator =
        (axis == 0) ? boxXCompare : ((axis == 1) ? boxYCompare : boxZCompare);

    size_t objectSpan = end - start;

    if (objectSpan == 1) {
        left = right = objects[start];
    } else if (objectSpan == 2) {
        left = objects[start];
        right = objects[start + 1];
    } else {
        std::sort(std::begin(objects) + start, std::begin(objects) + end,
                  comperator);

        auto mid = start + objectSpan / 2;
        left = make_shared<bvhNode>(objects, start, mid);
        right = make_shared<bvhNode>(objects, mid, end);
    }
}

bool bvhNode::hit(const ray &r, interval ray_t, hitRecord &rec) const {
    if (!bbox.hit(r, ray_t)) {
        return false;
    }

    bool hitLeft = left->hit(r, ray_t, rec);
    bool hitRight =
        right->hit(r, interval(ray_t.min, hitLeft ? rec.t : ray_t.max), rec);

    return hitLeft || hitRight;
}

aabb bvhNode::boundingBox() const { return bbox; }

// Private
bool bvhNode::boxCompare(const shared_ptr<hittable> a,
                         const shared_ptr<hittable> b, int axisIndex) {
    auto aAxisInterval = a->boundingBox().axisInterval(axisIndex);
    auto bAxisInterval = b->boundingBox().axisInterval(axisIndex);
    return aAxisInterval.min < bAxisInterval.min;
}

bool bvhNode::boxXCompare(const shared_ptr<hittable> a,
                          const shared_ptr<hittable> b) {
    return boxCompare(a, b, 0);
}

bool bvhNode::boxYCompare(const shared_ptr<hittable> a,
                          const shared_ptr<hittable> b) {
    return boxCompare(a, b, 1);
}

bool bvhNode::boxZCompare(const shared_ptr<hittable> a,
                          const shared_ptr<hittable> b) {
    return boxCompare(a, b, 2);
}
