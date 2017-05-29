/*
 * Shape.h
 *
 *  Created on: Nov 23, 2016
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_SHAPE_H_
#define TRACERAPTOR_SHAPE_H_

#include <Vec.h>
#include <Ray.h>
#include <BBox.h>
#include <Intersection.h>

namespace traceraptor {

class Shape {
public:
	Shape() {}
    virtual ~Shape() {}

    virtual bool Intersect(const Ray &r, float tmin, float tmax, IntersectionInfo &rec) const = 0;
    virtual BBox get_bbox() const = 0;
    virtual Vec3f get_centroid() const = 0;
    virtual UV get_uv(const Vec3f& point) const = 0;
};

} /* namespace traceraptor */

#endif /* TRACERAPTOR_SHAPE_H_ */
