/*
 * BBox.cpp
 *
 *  Created on: Nov 30, 2016
 *      Author: bogdan
 */

#include "BBox.h"
#include <algorithm>

namespace traceraptor {

BBox::BBox(const Vec3& min, const Vec3& max)
  : min(min), max(max) { extent = max - min; centroid = extent/2.f; }

BBox::BBox(const Vec3& p)
  : min(p), max(p) { extent = max - min; centroid = extent/2.f; }

void BBox::expandToInclude(const Vec3& p) {
	min = component_min(min, p);
	max = component_max(max, p);
	extent = max - min;
	centroid = extent/2.f;
}

void BBox::expandToInclude(const BBox& b) {
	min = component_min(min, b.min);
	max = component_max(max, b.max);
	extent = max - min;
	centroid = extent/2.f;
}

unsigned int BBox::maxDimension() const {
	unsigned int result = 0;
	if(extent.y() > extent.x()) result = 1;
	if(extent.z() > extent.y()) result = 2;
	return result;
}

float BBox::surfaceArea() const {
	return 2.f*( extent.x()*extent.z() + extent.x()*extent.y() + extent.y()*extent.z() );
}

bool BBox::intersect(const Ray& ray, float &tmin, float &tmax) const {
	float t0_x = (min[0]-ray.o[0])*ray.inv_d[0];
	float t1_x = (max[0]-ray.o[0])*ray.inv_d[0];
	tmin = min_comparator(t0_x, t1_x);
	tmax = max_comparator(t0_x, t1_x);
	if (tmax <= tmin)
		return false;

	float t0_y = (min[1]-ray.o[1])*ray.inv_d[1];
	float t1_y = (max[1]-ray.o[1])*ray.inv_d[1];
	tmin = max_comparator(tmin, min_comparator(t0_y, t1_y));
	tmax = min_comparator(tmax, max_comparator(t0_y, t1_y));
	if (tmax <= tmin)
		return false;

	float t0_z = (min[2]-ray.o[2])*ray.inv_d[2];
	float t1_z = (max[2]-ray.o[2])*ray.inv_d[2];
	tmin = max_comparator(tmin, min_comparator(t0_z, t1_z));
	tmax = min_comparator(tmax, max_comparator(t0_z, t1_z));
	if (tmax <= tmin)
		return false;

	return tmax >= max_comparator(0.0f, tmin);
}

} /* namespace traceraptor */
