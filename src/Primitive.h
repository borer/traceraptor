/*
 * Primitive.h
 *
 *  Created on: Nov 23, 2016
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_PRIMITIVE_H_
#define TRACERAPTOR_PRIMITIVE_H_

#include <Primitive.h>
#include <Vec.h>
#include "Ray.h"

namespace traceraptor {

class Material;
class BBox;
class Primitive;

class UV {
public:
	UV(float u, float v) : u(u), v(v) {}

	float u;
	float v;
};

class IntersectionInfo {
public:
	IntersectionInfo() : t(0), uv(0,0), hit_point(), normal(), material(NULL), hit_something(false) {}

    float t;
    UV uv;
    Vec3f hit_point;
    Vec3f normal;
    std::shared_ptr<Material> material;
    bool hit_something;
};

class Primitive {
public:
	Primitive() {}
    virtual ~Primitive() {}

    virtual bool Intersect(const Ray &r, float t_min, float t_max, IntersectionInfo &rec) const = 0;
    virtual BBox get_bbox() const = 0;
    virtual Vec3f get_centroid() const = 0;
    virtual UV get_uv(const Vec3f& point) const = 0;
};

} /* namespace traceraptor */

#endif /* TRACERAPTOR_HITABLE_H_ */
