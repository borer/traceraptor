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
#include <Vec.h>
#include <Color.h>
#include <Ray.h>
#include <Camera.h>
#include <accelerators/BVH.h>
#include <RayTracingStatistics.h>
#include <Shape.h>

namespace traceraptor {

class Integrator {
public:
	virtual RGBColor Li(const Ray& ray, const Aggregate &world, Sampler& sampler) const = 0;
	virtual ~Integrator();
};

class SimpleIntegrator : public Integrator {
public:
	SimpleIntegrator(bool render_skybox,
                     int max_ray_bounces) : max_ray_bounce(max_ray_bounces), render_skybox(render_skybox) { }
	RGBColor Li(const Ray& ray, const Aggregate& world, Sampler& sampler) const;

private:
	RGBColor skybox_shade(const Ray& ray, RGBColor& top_color, RGBColor& bottom_color) const;
	RGBColor shadeNoIntersection(const Ray& ray) const;

	int max_ray_bounce;
	bool render_skybox;
};

} /* namespace traceraptor */
#endif /* TRACERAPTOR_INTEGRATOR_H_ */

