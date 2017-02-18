/*
 * Triangle.h
 *
 *  Created on: Feb 17, 2017
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_TRIANGLE_H_
#define TRACERAPTOR_TRIANGLE_H_

#include "Hitable.h"
#include "Material.h"
#include "BBox.h"
#include "RayTracingStatistics.h"

namespace traceraptor {

class Triangle: public Hitable {
public:
	Triangle(Vec3f v0, Vec3f v1, Vec3f v2, std::shared_ptr<Material> obj_material) :
			v0(v0), v1(v1), v2(v2), material(obj_material) {

		Vec3f min, max;
		min = component_min(v0, v1);
		min = component_min(min, v2);

		max = component_max(v0, v1);
		max = component_max(max, v2);

		bounds = std::make_shared<BBox>(min, max);
	};
	virtual bool hit(const Ray &r, float tmin, float tmax, IntersectionInfo &rec) const;
	virtual BBox get_bbox() const;
	virtual Vec3f get_centroid() const;
	virtual UV get_uv(const Vec3f& point) const;

	Vec3f v0, v1, v2;
	std::shared_ptr<BBox> bounds;
	std::shared_ptr<Material> material;
};

bool Triangle::hit(const Ray &r, float tmin, float tmax, IntersectionInfo &rec) const {
	INCREMENT_RAY_PRIMITIVES_TEST_STATISTICS;

	Vec3f edge1 = v1 - v0;
	Vec3f edge2 = v2 - v0;

	Vec3f pvec = cross(r.dir, edge2);
	float det = dot(edge1, pvec);

	// check determinant and exit if triangle and ray are parallel
	// TODO: add EPSILONS
	if (det == 0) return false;
	float inv_det = 1.0f / det;

	// compute and check first bricentric coordinated
	auto tvec = r.o - v0;
	auto u = dot(tvec, pvec) * inv_det;
	if (u < 0 || u > 1) return false;

	// compute and check second bricentric coordinated
	auto qvec = cross(tvec, edge1);
	auto v = dot(r.dir, qvec) * inv_det;
	if (v < 0 || u + v > 1) return false;

	// compute and check ray parameter
	auto t = dot(edge2, qvec) * inv_det;
	if (t < tmin || t > tmax) return false;

	rec.t = t;
	rec.hit_point = r.point_at_parameter(rec.t);
	rec.normal = cross(edge1, edge2);
	rec.material = material;
	rec.uv = get_uv(rec.normal);
	rec.hit_something = true;
	INCREMENT_RAY_PRIMITIVES_INTERSECTIONS_STATISTICS

	return true;
}

BBox Triangle::get_bbox() const {
	return BBox(bounds->min, bounds->max);
}

Vec3f Triangle::get_centroid() const {
    return (v0+v1+v2)/3.0f;
}

UV Triangle::get_uv(const Vec3f& point) const {
	return UV(0,0);
}

} /* namespace traceraptor */

#endif /* TRACERAPTOR_TRIANGLE_H_ */
