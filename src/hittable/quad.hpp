#ifndef QUAD_HPP
#define QUAD_HPP

#include "hittable/hitRecord.hpp"
#include "hittable/hittable.hpp"

using std::shared_ptr;

class quad : public hittable {
  public:
    quad(const point3 &Q, const vec3 &u, const vec3 &v,
         shared_ptr<material> mat);

    virtual void setBoundingBox();

    aabb boundingBox() const override;

    virtual bool hit(const ray &r, interval ray_t,
                     hitRecord &rec) const override;

  private:
    point3 Q;
    vec3 u;
    vec3 v;
    shared_ptr<material> mat;
    aabb bbox;
};

#endif
