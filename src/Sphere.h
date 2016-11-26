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

namespace traceraptor {

class Sphere: public Hitable {
public:
	Sphere(Vec3 center, float radius, Material *obj_material) : center(center), radius(radius), material(obj_material) {};
	virtual bool hit(const Ray &r, float tmin, float tmax, hit_record &rec) const;

	Vec3 center;
	float radius;
	Material *material;
};

bool Sphere::hit(const Ray &r, float tmin, float tmax, hit_record &rec) const {
	Vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius*radius;
	float discriminant = b*b - a*c;
	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant))/ a;
		if (temp < tmax && temp > tmin){
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.material_ptr = material;
			return true;
		}

		temp = (-b + sqrt(discriminant))/ a;
		if (temp < tmax && temp > tmin){
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			rec.material_ptr = material;
			return true;
		}
	}

	return false;
}

} /* namespace traceraptor */

#endif /* TRACERAPTOR_SPHERE_H_ */
