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
	BBox();
	BBox(const Vec3f& min, const Vec3f& max);
	BBox(const Vec3f& p);

	bool Intersect(const Ray& ray, float &tmin, float &tmax) const;
	bool IntersectP(const Ray &ray, const int dirIsNeg[3]) const;
	bool IntersectP(const Ray &ray) const;
	void ExpandToInclude(const Vec3f& p);
	void ExpandToInclude(const BBox& b);
	Vec3f GetCentroid() const;
	unsigned int MaxDimension() const;
	float SurfaceArea() const;
	Vec3f Offset(const Vec3f &p) const;

	const Vec3f &operator[](int i) const;

	Vec3f pMin, pMax, extent, centroid;
    
    static BBox Union(const BBox &a, const BBox &b);
};

} /* namespace traceraptor */

#endif /* TRACERAPTOR_BBOX_H_ */
