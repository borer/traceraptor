/*
 * BBox.cpp
 *
 *  Created on: Nov 30, 2016
 *      Author: bogdan
 */

#include "BBox.h"
#include <algorithm>

namespace traceraptor {

BBox::BBox() {
	float minNum = std::numeric_limits<float>::lowest();
	float maxNum = std::numeric_limits<float>::max();
	pMin = Vec3f({maxNum, maxNum, maxNum});
	pMax = Vec3f({minNum, minNum, minNum});
	extent = pMax - pMin;
	centroid = extent*0.5f;
}

BBox::BBox(const Vec3f& min, const Vec3f& max)
  : pMin(min), pMax(max) { extent = max - min; centroid = extent*0.5f; }

BBox::BBox(const Vec3f& p)
  : pMin(p), pMax(p) { extent = pMax - pMin; centroid = extent*0.5f; }

void BBox::ExpandToInclude(const Vec3f& p) {
	pMin = component_min(pMin, p);
	pMax = component_max(pMax, p);
	extent = pMax - pMin;
	centroid = extent*0.5f;
}

void BBox::ExpandToInclude(const BBox& b) {
	pMin = component_min(pMin, b.pMin);
	pMax = component_max(pMax, b.pMax);
	extent = pMax - pMin;
	centroid = extent*0.5f;
}

Vec3f BBox::GetCentroid() const {
	return centroid;
}

unsigned int BBox::MaxDimension() const {
	unsigned int result = 0;
	if(extent.y() > extent.x()) result = 1;
	if(extent.z() > extent.y()) result = 2;
	return result;
}

float BBox::SurfaceArea() const {
	return 2.f*(extent.x()*extent.z() + extent.x()*extent.y() + extent.y()*extent.z());
}

Vec3f BBox::Offset(const Vec3f &p) const {
	Vec3f o = p - pMin;
	if (pMax.x() > pMin.x()) o[0] /= pMax.x() - pMin.x();
	if (pMax.y() > pMin.y()) o[1] /= pMax.y() - pMin.y();
	if (pMax.z() > pMin.z()) o[2] /= pMax.z() - pMin.z();
	return o;
}

inline const Vec3f& BBox::operator[](int i) const {
    return (i == 0) ? pMin : pMax;
}

bool BBox::Intersect(const Ray& ray, float &tmin, float &tmax) const {
	float t0_x = (pMin.x()-ray.o.x())*ray.inv_d.x();
	float t1_x = (pMax.x()-ray.o.x())*ray.inv_d.x();
	tmin = min_comparator(t0_x, t1_x);
	tmax = max_comparator(t0_x, t1_x);
	if (tmax <= tmin)
		return false;

	float t0_y = (pMin.y()-ray.o.y())*ray.inv_d.y();
	float t1_y = (pMax.y()-ray.o.y())*ray.inv_d.y();
	tmin = max_comparator(tmin, min_comparator(t0_y, t1_y));
	tmax = min_comparator(tmax, max_comparator(t0_y, t1_y));
	if (tmax <= tmin)
		return false;

	float t0_z = (pMin.z()-ray.o.z())*ray.inv_d.z();
	float t1_z = (pMax.z()-ray.o.z())*ray.inv_d.z();
	tmin = max_comparator(tmin, min_comparator(t0_z, t1_z));
	tmax = min_comparator(tmax, max_comparator(t0_z, t1_z));
	if (tmax <= tmin)
		return false;

	return tmax >= max_comparator(0.0f, tmin);
}

bool BBox::IntersectP(const Ray &ray, const int dirIsNeg[3]) const {
    const BBox &bounds = *this;
    // Check for ray intersection against $x$ and $y$ slabs
    Float tMin = (bounds[dirIsNeg[0]].x() - ray.o.x()) * ray.inv_d.x();
    Float tMax = (bounds[1 - dirIsNeg[0]].x() - ray.o.x()) * ray.inv_d.x();
    Float tyMin = (bounds[dirIsNeg[1]].y() - ray.o.y()) * ray.inv_d.y();
    Float tyMax = (bounds[1 - dirIsNeg[1]].y() - ray.o.y()) * ray.inv_d.y();

    // Update _tMax_ and _tyMax_ to ensure robust bounds intersection
    tMax *= 1 + 2 * tgamma(3);
    tyMax *= 1 + 2 * tgamma(3);
    if (tMin > tyMax || tyMin > tMax) return false;
    if (tyMin > tMin) tMin = tyMin;
    if (tyMax < tMax) tMax = tyMax;

    // Check for ray intersection against $z$ slab
    Float tzMin = (bounds[dirIsNeg[2]].z() - ray.o.z()) * ray.inv_d.z();
    Float tzMax = (bounds[1 - dirIsNeg[2]].z() - ray.o.z()) * ray.inv_d.z();

    // Update _tzMax_ to ensure robust bounds intersection
    tzMax *= 1 + 2 * tgamma(3);
    if (tMin > tzMax || tzMin > tMax) return false;
    if (tzMin > tMin) tMin = tzMin;
    if (tzMax < tMax) tMax = tzMax;
    return (tMin < ray.tMax) && (tMax > Ray::default_tmin);
}

} /* namespace traceraptor */
