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

#define UNUSED(expr) (void)(expr)

namespace traceraptor {

class Material {
public :
	virtual bool scatter(const Ray &r_in, const hit_record &rec, Vec3 &attenuation, Ray& scattered) const = 0;
	virtual ~Material() {}

	Vec3 reflect(const Vec3 &v, const Vec3 &n) const{
		return v - n * (dot(v,n) * 2);
	}

	bool refract(const Vec3& v, const Vec3& n, float ni_over_nt, Vec3& refracted)  const {
		Vec3 uv = unit_vector(v);
	    float dt = dot(uv, n);
	    float discriminant = 1.0 - ni_over_nt*ni_over_nt*(1-dt*dt);
	    if (discriminant > 0) {
	        refracted = ni_over_nt*(uv - n*dt) - n*sqrt(discriminant);
	        return true;
	    }
	    else
	        return false;
	}
};

class Lambertian: public Material{
public:
    Lambertian(const Vec3& e): albedo(e){}

    virtual bool scatter(const Ray &r_in, const hit_record &rec, Vec3 &attenuation, Ray& scattered) const{
    	UNUSED(r_in);
        Vec3 target = rec.p + rec.normal + random_in_unit_sphere();
        scattered = Ray(rec.p, target - rec.p);
        attenuation = albedo;
        return true;
    }

    Vec3 albedo;
};

class Metal: public Material{
public:

    Metal(const Vec3& e, float f) : albedo(e)  {
    		fuzz = (f < 1) ? f : 1;
    }

    virtual bool scatter(const Ray &r_in, const hit_record &rec, Vec3 &attenuation, Ray& scattered) const{
        Vec3 v = unit_vector(r_in.direction());
        Vec3 reflected = reflect(v, rec.normal);

        scattered = Ray(rec.p, reflected + fuzz*random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

    Vec3 albedo;
    float fuzz;
};

class Dielectric : public Material {
public:
	Dielectric(float ri) : ref_idx(ri) {}

	float schlick(float cosine, float ref_idx) const {
	    float r0 = (1-ref_idx) / (1+ref_idx);
	    r0 = r0*r0;
	    return r0 + (1-r0)*pow((1 - cosine),5);
	}

	virtual bool scatter(const Ray& r_in, const hit_record& rec, Vec3& attenuation, Ray& scattered) const  {
		Vec3 outward_normal;
		Vec3 reflected = reflect(r_in.direction(), rec.normal);
		float ni_over_nt;
		attenuation = Vec3(1.0, 1.0, 1.0);
		Vec3 refracted;
		float reflect_prob;
		float cosine;
		if (dot(r_in.direction(), rec.normal) > 0) {
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
//			cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
			cosine = dot(r_in.direction(), rec.normal) / r_in.direction().length();
			cosine = sqrt(1 - ref_idx*ref_idx*(1-cosine*cosine));
		}
		else {
			outward_normal = rec.normal;
			ni_over_nt = 1.0 / ref_idx;
			cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
		}

		if (refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
			reflect_prob = schlick(cosine, ref_idx);
		} else {
			reflect_prob = 1.0;
		}

		if (random01() < reflect_prob) {
			scattered = Ray(rec.p, reflected);
		} else {
			scattered = Ray(rec.p, refracted);
		}

		return true;
	}

	float ref_idx;
};

} /* namespace traceraptor */

#endif /* TRACERAPTOR_MATERIAL_H_ */
