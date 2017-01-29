/*
 * Camera.h
 *
 *  Created on: Nov 23, 2016
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_CAMERA_H_
#define TRACERAPTOR_CAMERA_H_

#include <MathUtil.h>
#include "Vec3.h"
#include "Ray.h"

namespace traceraptor {

class Camera {
public:
	Camera(Vec3 lookfrom, Vec3 lookat, Vec3 vup, float vfov, float aspect, float aperture, float focus_dist) { // vfov is top to bottom in degrees
		lens_radius = aperture / 2;
		float theta = vfov*M_PI/180;
		float half_height = tan(theta/2);
		float half_width = aspect * half_height;
		origin = lookfrom;
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);
		lower_left_corner = origin  - half_width*focus_dist*u -half_height*focus_dist*v - focus_dist*w;
		horizontal = 2*half_width*focus_dist*u;
		vertical = 2*half_height*focus_dist*v;
	}

	Ray get_ray(float s, float t) const {
		Vec3 rd = lens_radius*MathUtil::random_in_unit_disk();
		Vec3 offset = u * rd.x() + v * rd.y();
		return Ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset);
	}

	Vec3 origin;
	Vec3 lower_left_corner;
	Vec3 horizontal;
	Vec3 vertical;
	Vec3 u, v, w;
	float lens_radius;
};

} /* namespace traceraptor */

#endif /* TRACERAPTOR_CAMERA_H_ */
