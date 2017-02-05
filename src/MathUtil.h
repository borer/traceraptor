/*
 * Math.h
 *
 *  Created on: Nov 26, 2016
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_UTIL_MATH_H_
#define TRACERAPTOR_UTIL_MATH_H_

#include <Vec.h>
#include <random>
#include <cmath>

namespace traceraptor {

static std::default_random_engine pseudo_random_generator;

inline float random01() {
	std::uniform_real_distribution<float> distribution(0.0,1.0);
	return distribution(pseudo_random_generator);
}

constexpr inline int pow2(int x) { return 1 << x; }

class MathUtil {
public:
	static Vec3f random_in_unit_disk() {
		Vec3f p;
	    do {
	        p = 2.0f * Vec3f{random01(), random01(), 0} - Vec3f{1,1,0};
	    } while (dot(p,p) >= 1.0);
	    return p;
	}

	static Vec3f random_in_unit_sphere() {
		Vec3f p;
		do {
			p = 2.0f * Vec3f{random01(), random01(), random01()} - Vec3f{1,1,1};
		} while(length(p) >= 1.0);

		return p;
	}
};

} /* namespace traceraptor */

#endif /* TRACERAPTOR_UTIL_MATH_H_ */
