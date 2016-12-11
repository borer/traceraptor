/*
 * Sphere.h
 *
 *  Created on: Nov 23, 2016
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_SPHERE_H_
#define TRACERAPTOR_SPHERE_H_

#include "Hitable.h"
#include "Material.h"
#include "BBox.h"

namespace traceraptor {

class Sphere: public Hitable {
public:
	Sphere(Vec3 center, float radius, std::shared_ptr<Material> obj_material) : center(center), radius(radius), material(obj_material) {};
	virtual bool hit(const Ray &r, float tmin, float tmax, IntersectionInfo &rec) const;
	virtual BBox get_bbox() const;
	virtual Vec3 get_centroid() const;
	virtual UV get_uv(const Vec3& point) const;

	Vec3 center;
	float radius;
	std::shared_ptr<Material> material;
};

bool Sphere::hit(const Ray &r, float tmin, float tmax, IntersectionInfo &rec) const {
	Vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius*radius;
	float discriminant = b*b - a*c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant))/ a;
		if (temp < tmax && temp > tmin){
			rec.t = temp;
			rec.hit_point = r.point_at_parameter(rec.t);
			rec.normal = (rec.hit_point - center) / radius;
			rec.material = material;
			rec.uv = get_uv(rec.normal);
			rec.hit_something = true;
			return true;
		}

		temp = (-b + sqrt(discriminant))/ a;
		if (temp < tmax && temp > tmin){
			rec.t = temp;
			rec.hit_point = r.point_at_parameter(rec.t);
			rec.normal = (rec.hit_point - center) / radius;
			rec.material = material;
			rec.uv = get_uv(rec.normal);
			rec.hit_something = true;
			return true;
		}
	}

	return false;
}

BBox Sphere::get_bbox() const {
	return BBox(center-Vec3(radius,radius,radius), center+Vec3(radius,radius,radius));
}

Vec3 Sphere::get_centroid() const {
    return center;
}

UV Sphere::get_uv(const Vec3& point) const {
	 float phi = atan2(point.z(), point.x());
	 float theta = asin(point.y());
	 float u = 1-(phi + M_PI) / (2*M_PI);
	 float v = (theta + M_PI/2) / M_PI;

	 return UV(u,v);
}

} /* namespace traceraptor */

#endif /* TRACERAPTOR_SPHERE_H_ */
