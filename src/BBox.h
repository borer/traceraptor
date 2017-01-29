/*
 * BBox.h
 *
 *  Created on: Nov 30, 2016
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_BBOX_H_
#define TRACERAPTOR_BBOX_H_

#include "Ray.h"
#include "Vec3.h"
#include "UtilMath.h"

namespace traceraptor {

class BBox {
public:
	BBox() { }
	BBox(const Vec3& min, const Vec3& max);
	BBox(const Vec3& p);

	bool intersect(const Ray& ray, float &tmin, float &tmax) const;
	void expandToInclude(const Vec3& p);
	void expandToInclude(const BBox& b);
	unsigned int maxDimension() const;
	float surfaceArea() const;

	Vec3 min, max, extent;
};

} /* namespace traceraptor */

#endif /* TRACERAPTOR_BBOX_H_ */
