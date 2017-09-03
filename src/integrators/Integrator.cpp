/*
 * Integrator.cpp
 *
 *  Created on: May 7, 2017
 *      Author: bogdan
 */

#include <integrators/Integrator.h>
#include <Material.h>

namespace traceraptor {

Integrator::~Integrator() {}

RGBColor SimpleIntegrator::Li(const Ray& ray, const Aggregate& world, Sampler& sampler) const {
	IntersectionInfo rec;
	Ray rayIn = ray;
	RGBColor shadeColor { 1.0f, 1.0f, 1.0f };
	for (int current_ray_bounce = 0;; current_ray_bounce++) {
		if (world.Intersect(rayIn, rec) ) {
			const std::shared_ptr<Material> intersectedMaterial = rec.material;
			Vec3f attenuation;
			Vec3f emitted = intersectedMaterial->emitted(rayIn, rec);
			Ray scattered;

			if (current_ray_bounce < max_ray_bounce &&
                intersectedMaterial->scatter(rayIn, rec, sampler, attenuation, scattered))
            {
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

RGBColor SimpleIntegrator::skybox_shade(const Ray& ray, RGBColor& top_color, RGBColor& bottom_color) const {
	Vec3f unit_direction = normalize(ray.direction());
	float t = 0.5f * (unit_direction[1] + 1.0f);
	return lerp(top_color, bottom_color, t);
}

RGBColor SimpleIntegrator::shadeNoIntersection(const Ray& ray) const {
	if (render_skybox) {
		RGBColor top_color { 1.0f, 1.0f, 1.0f };
		RGBColor bottom_color { 0.5f, 0.7f, 1.0f };
		return skybox_shade(ray, top_color, bottom_color);
	} else {
		RGBColor black_color { 0, 0, 0 };
		return black_color;
	}
}

} /* namespace traceraptor */
