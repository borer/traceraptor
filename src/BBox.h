/*
 * BBox.h
 *
 *  Created on: Nov 30, 2016
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_BBOX_H_
#define TRACERAPTOR_BBOX_H_

#include <MathUtil.h>
#include <Vec.h>
#include "Ray.h"

namespace traceraptor {

class BBox {
public:
	BBox() {}
	BBox(const Vec3f& min, const Vec3f& max);
	BBox(const Vec3f& p);

	bool intersect(const Ray& ray, float &tmin, float &tmax) const;
	void expandToInclude(const Vec3f& p);
	void expandToInclude(const BBox& b);
	unsigned int maxDimension() const;
	float surfaceArea() const;

	Vec3f min, max, extent, centroid;
};

} /* namespace traceraptor */

#endif /* TRACERAPTOR_BBOX_H_ */
