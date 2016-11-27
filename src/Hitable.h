/*
 * Hitable.h
 *
 *  Created on: Nov 23, 2016
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_HITABLE_H_
#define TRACERAPTOR_HITABLE_H_

namespace traceraptor {

#include "Ray.h"
#include "Vec3.h"

class Material;

struct hit_record{
    float t;
    Vec3 p;
    Vec3 normal;
    std::shared_ptr<Material> material;
};

class Hitable{
public:
	Hitable() {}
    virtual ~Hitable() {}

    virtual bool hit(const Ray &r, float t_min, float t_max, hit_record &rec) const = 0;
};

} /* namespace traceraptor */

#endif /* TRACERAPTOR_HITABLE_H_ */
