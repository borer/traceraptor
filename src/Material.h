/*
 * Material.h
 *
 *  Created on: Nov 26, 2016
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_MATERIAL_H_
#define TRACERAPTOR_MATERIAL_H_

#include "Vec3.h"
#include "Ray.h"
#include "UtilMath.h"

namespace traceraptor {

class Material {
public :
	virtual bool scatter(const Ray &r_in, const hit_record &rec, Vec3 &attenuation, Ray& scattered) const = 0;
	virtual ~Material() {}

	Vec3 reflect(const Vec3 &v, const Vec3 &n) const{
		return v - n * (dot(v,n) * 2);
	}
};

class Lambertian: public Material{
public:
    Lambertian(const Vec3& e): albedo(e){}

    virtual bool scatter(const Ray &r_in, const hit_record &rec, Vec3 &attenuation, Ray& scattered) const{
        Vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = Ray(rec.p, target - rec.p);
        attenuation = albedo;
        return true;
    }

    Vec3 albedo;
};

class Metal: public Material{
public:

    Metal(const Vec3& e) : albedo(e) {}

    virtual bool scatter(const Ray &r_in, const hit_record &rec, Vec3 &attenuation, Ray& scattered) const{
        Vec3 v = unit_vector(r_in.direction());
        Vec3 reflected = reflect(v, rec.normal);

        scattered = Ray(rec.p, reflected);
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

    Vec3 albedo;
};

} /* namespace traceraptor */

#endif /* TRACERAPTOR_MATERIAL_H_ */
