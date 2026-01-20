#ifndef BVH_HPP
#define BVH_HPP

#include "hittable/aabb.hpp"
#include "hittable/hittable.hpp"
#include "hittable/hittableList.hpp"

#include <cstddef>
#include <memory>

using std::make_shared;
using std::shared_ptr;

class bvhNode : public hittable {
  public:
    bvhNode(hittableList list);

    bvhNode(std::vector<shared_ptr<hittable>> &objects, size_t start,
            size_t end);

    bool hit(const ray &r, interval ray_t, hitRecord &rec) const override;

    aabb boundingBox() const override;

  private:
    shared_ptr<hittable> left;
    shared_ptr<hittable> right;
    aabb bbox;

    static bool boxCompare(const shared_ptr<hittable> a,
                           const shared_ptr<hittable> b, int axisIndex);

    static bool boxXCompare(const shared_ptr<hittable> a,
                            const shared_ptr<hittable> b);

    static bool boxYCompare(const shared_ptr<hittable> a,
                            const shared_ptr<hittable> b);

    static bool boxZCompare(const shared_ptr<hittable> a,
                            const shared_ptr<hittable> b);
};

#endif // BVH_HPP
