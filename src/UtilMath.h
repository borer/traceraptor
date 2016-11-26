/*
 * Math.h
 *
 *  Created on: Nov 26, 2016
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_UTIL_MATH_H_
#define TRACERAPTOR_UTIL_MATH_H_

#include <random>
#include <cmath>
#include "Vec3.h"

namespace traceraptor {

std::default_random_engine pseudo_random_generator;

inline float random01() {
	std::uniform_real_distribution<float> distribution(0.0,1.0);
	return distribution(pseudo_random_generator);
}

Vec3 random_in_unit_sphere() {
	Vec3 p;
	do {
		p = 2.0 * Vec3(random01(), random01(), random01()) - Vec3(1,1,1);
	} while(p.length() >= 1.0);

	return p;
}

inline Vec3 lerp(const Vec3 &v1, const Vec3 &v2, float t) {
	return ((1.0-t)*v1 + t*v2);
}

} /* namespace traceraptor */

#endif /* TRACERAPTOR_UTIL_MATH_H_ */
