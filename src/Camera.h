/*
 * Camera.h
 *
 *  Created on: Nov 23, 2016
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_CAMERA_H_
#define TRACERAPTOR_CAMERA_H_

#include "Vec3.h"
#include "Ray.h"

namespace traceraptor {

class Camera {
public:
	Camera() :
		lower_left_corner(Vec3(-2.0, -1.0, -1.0)),
		horizontal(Vec3(4.0, 0.0,0.0)),
		vertical(Vec3(0.0, 2.0, 0.0)),
		origin(Vec3(0.0, 0.0, 0.0)) {}

	Ray get_ray(float u, float v) {
		return Ray(origin, lower_left_corner + u*horizontal + v*vertical);
	}


	Vec3 origin;
	Vec3 lower_left_corner;
	Vec3 horizontal;
	Vec3 vertical;
};

} /* namespace traceraptor */

#endif /* TRACERAPTOR_CAMERA_H_ */
