#ifndef HITTABLELIST_HPP
#define HITTABLELIST_HPP

#include "core/interval.hpp"
#include "hittable/hittable.hpp"

#include <memory>
#include <vector>

class hittableList : public hittable {
  public:
    hittableList();
    hittableList(std::shared_ptr<hittable> ptr_object);

    std::vector<std::shared_ptr<hittable>> objects;

    void clear();

    void add(std::shared_ptr<hittable> ptr_object);

    virtual bool hit(const ray &r, interval ray_t,
                     hitRecord &rec) const override;

    aabb boundingBox() const override;

  private:
    aabb bbox;
};

#endif // HITTABLELIST_HPP
