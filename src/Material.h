/*
 * Material.h
 *
 *  Created on: Nov 26, 2016
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_MATERIAL_H_
#define TRACERAPTOR_MATERIAL_H_

#include <MathUtil.h>
#include <Vec.h>
#include "Ray.h"
#include "Texture.h"

#define UNUSED(expr) (void)(expr)

namespace traceraptor {

class Material {
public :
	virtual bool scatter(const Ray &r_in, const IntersectionInfo &rec, Vec3f &attenuation, Ray& scattered) const = 0;

	virtual Vec3f emitted(const Ray& ray, const IntersectionInfo& info) const {
		UNUSED(ray);
		UNUSED(info);
		return Vec3f{0,0,0};
	}

	virtual ~Material() {}

	Vec3f reflect(const Vec3f &v, const Vec3f &n) const{
		return v - n * (dot(v,n) * 2);
	}

	bool refract(const Vec3f& v, const Vec3f& n, float ni_over_nt, Vec3f& refracted)  const {
		Vec3f uv = normalize(v);
	    float dt = dot(uv, n);
	    float discriminant = 1.0f - ni_over_nt*ni_over_nt*(1-dt*dt);
	    if (discriminant > 0) {
	        refracted = ni_over_nt*(uv - n*dt) - n*(float)sqrt(discriminant);
	        return true;
	    }
	    else
	        return false;
	}
};

class Lambertian: public Material{
public:
    explicit Lambertian(std::shared_ptr<Texture> texture): albedo(texture){}

    virtual bool scatter(const Ray &r_in, const IntersectionInfo &rec, Vec3f &attenuation, Ray& scattered) const {
    	UNUSED(r_in);
        Vec3f target = rec.hit_point + rec.normal + MathUtil::random_in_unit_sphere();
        scattered = Ray(rec.hit_point, target - rec.hit_point);
        attenuation = albedo->value(rec.uv.u, rec.uv.v, rec.hit_point);
        return true;
    }


    std::shared_ptr<Texture> albedo;
};

class Metal: public Material{
public:

    Metal(const Vec3f& e, float f) : albedo(e)  {
    		fuzz = (f < 1) ? f : 1;
    }

    virtual bool scatter(const Ray &r_in, const IntersectionInfo &rec, Vec3f &attenuation, Ray& scattered) const{
        Vec3f ray_direction_unit = normalize(r_in.direction());
        Vec3f reflected = reflect(ray_direction_unit, rec.normal);

        scattered = Ray(rec.hit_point, reflected + fuzz*MathUtil::random_in_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

    Vec3f albedo;
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

	virtual bool scatter(const Ray& r_in, const IntersectionInfo& rec, Vec3f& attenuation, Ray& scattered) const  {
		Vec3f outward_normal;
		Vec3f reflected = reflect(r_in.direction(), rec.normal);
		float ni_over_nt;
		attenuation = Vec3f{1.0, 1.0, 1.0};
		Vec3f refracted;
		float reflect_prob;
		float cosine;
		if (dot(r_in.direction(), rec.normal) > 0) {
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
//			cosine = ref_idx * dot(r_in.direction(), rec.normal) / length(r_in.direction());
			cosine = dot(r_in.direction(), rec.normal) / length(r_in.direction());
			cosine = sqrt(1 - ref_idx*ref_idx*(1-cosine*cosine));
		}
		else {
			outward_normal = rec.normal;
			ni_over_nt = 1.0 / ref_idx;
			cosine = -dot(r_in.direction(), rec.normal) / length(r_in.direction());
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

class Emissive : public Material  {
public:
	Emissive(std::shared_ptr<Texture> a) : emit(a) {}

	virtual bool scatter(const Ray &r_in, const IntersectionInfo &rec, Vec3f &attenuation, Ray &scattered) const {
		UNUSED(r_in);
		UNUSED(rec);
		UNUSED(attenuation);
		UNUSED(scattered);
		return false;
	}

	virtual Vec3f emitted(const Ray& ray, const IntersectionInfo& info) const {
		if (dot(info.normal, ray.direction()) < 0.0)
			return emit->value(info.uv.u, info.uv.v, info.hit_point);
		else
			return Vec3f{0,0,0};
	}

	std::shared_ptr<Texture> emit;
};


} /* namespace traceraptor */

#endif /* TRACERAPTOR_MATERIAL_H_ */
