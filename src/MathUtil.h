/*
 * Math.h
 *
 *  Created on: Nov 26, 2016
 *      Author: bogdan
 */

//#define TRACERAPTOR_FLOAT_AS_DOUBLE
#ifdef TRACERAPTOR_FLOAT_AS_DOUBLE
typedef double Float;
#else
typedef float Float;
#endif  // TRACERAPTOR_FLOAT_AS_DOUBLE

#ifndef TRACERAPTOR_UTIL_MATH_H_
#define TRACERAPTOR_UTIL_MATH_H_

#include <Vec.h>
#include <random>
#include <cmath>

namespace traceraptor {

class Sampler {
public:
	Sampler(Float seed = 1.234f) {
		prng = std::mt19937(seed);
		distribution = std::uniform_real_distribution<Float>(0.0, 1.0);
	}

    // generates random number [0,1)
	Float random01f() {
    	return distribution(prng);
    }

    Vec3f random_in_unit_disk() {
    	Vec3f p;
    	do {
    		p = 2.0f * Vec3f{random01f(), random01f(), 0} - Vec3f{1,1,0};
    	} while (dot(p,p) >= 1.0);
    	return p;
    }

    Vec3f random_in_unit_sphere() {
    	Vec3f p;
    	do {
    		p = 2.0f * Vec3f{random01f(), random01f(), random01f()} - Vec3f{1,1,1};
    	} while(length(p) >= 1.0);

    	return p;
    }

private:
    std::mt19937 prng;
    std::uniform_real_distribution<Float> distribution;
};

constexpr inline int pow2(int x) { return 1 << x; }

static constexpr Float Pi = 3.14159265358979323846;

constexpr inline Float Radians(Float deg) { return (Pi / 180) * deg; }

constexpr inline Float Degrees(Float rad) { return (180 / Pi) * rad; }

} /* namespace traceraptor */

#endif /* TRACERAPTOR_UTIL_MATH_H_ */
