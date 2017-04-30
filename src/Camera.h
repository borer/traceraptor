/*
 * Camera.h
 *
 *  Created on: Nov 23, 2016
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_CAMERA_H_
#define TRACERAPTOR_CAMERA_H_

#include <MathUtil.h>
#include <Vec.h>
#include "Ray.h"

namespace traceraptor {

class Camera {
public:
	Camera(Vec3f lookfrom, Vec3f lookat, Vec3f vup, float vfov, float aspect, float aperture, float focus_dist) { // vfov is top to bottom in degrees
		lens_radius = aperture / 2;
		float theta = vfov*M_PI/180;
		float half_height = tan(theta/2);
		float half_width = aspect * half_height;
		origin = lookfrom;
		w = normalize(lookfrom - lookat);
		u = normalize(cross(vup, w));
		v = cross(w, u);
		lower_left_corner = origin  - half_width*focus_dist*u -half_height*focus_dist*v - focus_dist*w;
		horizontal = 2*half_width*focus_dist*u;
		vertical = 2*half_height*focus_dist*v;
	}

	Ray get_ray(float s, float t, Sampler sampler) const {
		Vec3f rd = lens_radius*sampler.random_in_unit_disk();
		Vec3f offset = u * rd[0] + v * rd[1];
		return Ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset);
	}

	Vec3f origin;
	Vec3f lower_left_corner;
	Vec3f horizontal;
	Vec3f vertical;
	Vec3f u, v, w;
	float lens_radius;
};

} /* namespace traceraptor */

#endif /* TRACERAPTOR_CAMERA_H_ */
