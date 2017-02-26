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

class Sampler {
public:
	Sampler(float seed = 1.234f) {
		prng = std::mt19937(seed);
		distribution = std::uniform_real_distribution<float>(0.0, 1.0);
	}

    // generates random number [0,1)
    float random01f() {
    	return distribution(prng);
    }

private:
    std::mt19937 prng;
    std::uniform_real_distribution<float> distribution;
};

constexpr inline int pow2(int x) { return 1 << x; }

class MathUtil {
public:
	static Vec3f random_in_unit_disk(Sampler sampler) {
		Vec3f p;
	    do {
	        p = 2.0f * Vec3f{sampler.random01f(), sampler.random01f(), 0} - Vec3f{1,1,0};
	    } while (dot(p,p) >= 1.0);
	    return p;
	}

	static Vec3f random_in_unit_sphere(Sampler sampler) {
		Vec3f p;
		do {
			p = 2.0f * Vec3f{sampler.random01f(), sampler.random01f(), sampler.random01f()} - Vec3f{1,1,1};
		} while(length(p) >= 1.0);

		return p;
	}
};

} /* namespace traceraptor */

#endif /* TRACERAPTOR_UTIL_MATH_H_ */
