/*
 * Integrator.cpp
 *
 *  Created on: May 7, 2017
 *      Author: bogdan
 */

#include <integrators/Integrator.h>

namespace traceraptor {

Integrator::~Integrator() {}

Vec3f SimpleIntegrator::Li(const Ray& ray, const BVH& world, Sampler& sampler) const {
	IntersectionInfo rec;
	Ray rayIn = ray;
	Vec3f shadeColor { 1.0f, 1.0f, 1.0f };
	for (int current_ray_bounce = 0;; current_ray_bounce++) {
		if (world.getIntersection(rayIn, rec, false)) {
			Vec3f attenuation;
			Vec3f emitted = rec.material->emitted(rayIn, rec);
			Ray scattered;

			if (current_ray_bounce < max_ray_bounce
					&& rec.material->scatter(rayIn, rec, sampler,
							attenuation, scattered)) {
				shadeColor *= emitted + attenuation;
				rayIn = scattered;
			} else {
				shadeColor *= emitted;
				break;
			}
		} else {
			shadeColor *= shadeNoIntersection(rayIn);
			break;
		}
	}

	return shadeColor;
}

Vec3f SimpleIntegrator::skybox_shade(const Ray& ray, Vec3f& top_color, Vec3f& bottom_color) const {
	Vec3f unit_direction = normalize(ray.direction());
	float t = 0.5f * (unit_direction[1] + 1.0f);
	return lerp(top_color, bottom_color, t);
}

Vec3f SimpleIntegrator::shadeNoIntersection(const Ray& ray) const {
	if (render_skybox) {
		Vec3f top_color { 1.0f, 1.0f, 1.0f };
		Vec3f bottom_color { 0.5f, 0.7f, 1.0f };
		return skybox_shade(ray, top_color, bottom_color);
	} else {
		Vec3f black_color { 0, 0, 0 };
		return black_color;
	}
}

} /* namespace traceraptor */
