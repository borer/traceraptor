/*
 * Hitable.h
 *
 *  Created on: Nov 23, 2016
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_HITABLE_H_
#define TRACERAPTOR_HITABLE_H_

#include "Ray.h"
#include "Vec3.h"
#include "Hitable.h"

namespace traceraptor {

class Material;
class BBox;
class Hitable;

class IntersectionInfo {
public:
	IntersectionInfo() : t(0), hit_point(), normal(), material(NULL), hit_something(false) {}

    float t;
    Vec3 hit_point;
    Vec3 normal;
    std::shared_ptr<Material> material;
    bool hit_something;
};

class Hitable{
public:
	Hitable() {}
    virtual ~Hitable() {}

    virtual bool hit(const Ray &r, float t_min, float t_max, IntersectionInfo &rec) const = 0;
    virtual BBox get_bbox() const = 0;
    virtual Vec3 get_centroid() const = 0;
};

} /* namespace traceraptor */

#endif /* TRACERAPTOR_HITABLE_H_ */
