/*
 * Integrator.h
 *
 *  Created on: May 07, 2017
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_INTEGRATOR_H_
#define TRACERAPTOR_INTEGRATOR_H_

#include <string>
#include <thread>

#include <Logger.h>
#include <MathUtil.h>
#include <Primitive.h>
#include <Vec.h>
#include <Ray.h>
#include <Camera.h>
#include <Material.h>
#include <BVH.h>
#include <RayTracingStatistics.h>

namespace traceraptor {

class Integrator {
public:
	virtual Vec3f Li(const Ray& ray, const BVH &world, Sampler& sampler) const = 0;
	virtual ~Integrator();
};

class SimpleIntegrator : public Integrator {
public:
	SimpleIntegrator(bool render_skybox,
                     int max_ray_bounces) : max_ray_bounce(max_ray_bounces), render_skybox(render_skybox) { }
	Vec3f Li(const Ray& ray, const BVH& world, Sampler& sampler) const;

private:
	Vec3f skybox_shade(const Ray& ray, Vec3f& top_color, Vec3f& bottom_color) const;
	Vec3f shadeNoIntersection(const Ray& ray) const;

	int max_ray_bounce;
	bool render_skybox;
};

} /* namespace traceraptor */
#endif /* TRACERAPTOR_INTEGRATOR_H_ */

