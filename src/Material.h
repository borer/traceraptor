/*
 * Material.h
 *
 *  Created on: Nov 26, 2016
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_MATERIAL_H_
#define TRACERAPTOR_MATERIAL_H_

#include <MathUtil.h>
#include "Vec3.h"
#include "Ray.h"
#include "Texture.h"

#define UNUSED(expr) (void)(expr)

namespace traceraptor {

class Material {
public :
	virtual bool scatter(const Ray &r_in, const IntersectionInfo &rec, Vec3 &attenuation, Ray& scattered) const = 0;

	virtual Vec3 emitted(float u, float v, const Vec3 &p) const {
		UNUSED(u);
		UNUSED(v);
		UNUSED(p);
		return Vec3(0,0,0);
	}

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
    explicit Lambertian(std::shared_ptr<Texture> texture): albedo(texture){}

    virtual bool scatter(const Ray &r_in, const IntersectionInfo &rec, Vec3 &attenuation, Ray& scattered) const {
    	UNUSED(r_in);
        Vec3 target = rec.hit_point + rec.normal + MathUtil::random_in_unit_sphere();
        scattered = Ray(rec.hit_point, target - rec.hit_point);
        attenuation = albedo->value(rec.uv.u, rec.uv.v, rec.hit_point);
        return true;
    }


    std::shared_ptr<Texture> albedo;
};

class Metal: public Material{
public:

    Metal(const Vec3& e, float f) : albedo(e)  {
    		fuzz = (f < 1) ? f : 1;
    }

    virtual bool scatter(const Ray &r_in, const IntersectionInfo &rec, Vec3 &attenuation, Ray& scattered) const{
        Vec3 ray_direction_unit = unit_vector(r_in.direction());
        Vec3 reflected = reflect(ray_direction_unit, rec.normal);

        scattered = Ray(rec.hit_point, reflected + fuzz*MathUtil::random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

    Vec3 albedo;
    float fuzz;
};

class Dielectric : public Material {
public:
	explicit Dielectric(float ri) : ref_idx(ri) {}

	float schlick(float cosine, float ref_idx) const {
	    float r0 = (1-ref_idx) / (1+ref_idx);
	    r0 = r0*r0;
	    return r0 + (1-r0)*pow((1 - cosine),5);
	}

	virtual bool scatter(const Ray& r_in, const IntersectionInfo& rec, Vec3& attenuation, Ray& scattered) const  {
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
			scattered = Ray(rec.hit_point, reflected);
		} else {
			scattered = Ray(rec.hit_point, refracted);
		}

		return true;
	}

	float ref_idx;
};

class DiffuseLight : public Material  {
public:
	DiffuseLight(std::shared_ptr<Texture> a) : emit(a) {}

	virtual bool scatter(const Ray &r_in, const IntersectionInfo &rec, Vec3 &attenuation, Ray &scattered) const {
		UNUSED(r_in);
		UNUSED(rec);
		UNUSED(attenuation);
		UNUSED(scattered);
		return false;
	}

	virtual Vec3 emitted(float u, float v, const Vec3 &p) const {
		return emit->value(u, v, p) *4 ;// + Vec3(2,2,2);
	}

	std::shared_ptr<Texture> emit;
};


} /* namespace traceraptor */

#endif /* TRACERAPTOR_MATERIAL_H_ */
