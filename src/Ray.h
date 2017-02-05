/*
 * Ray.h
 *
 *  Created on: Nov 22, 2016
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_RAY_H_
#define TRACERAPTOR_RAY_H_

#include <Vec.h>

namespace traceraptor {

class Ray {
public:
    Ray(){}
    Ray(const Vec3f& a, const Vec3f& b) : o(a), dir(b) {
    	inv_d = Vec3f{1,1,1}/dir;
    }

    Vec3f origin() const {return o;}
    Vec3f direction() const {return dir;}

    Vec3f point_at_parameter(float t) const {
        return o + dir*t;
    }

    Vec3f o;
    Vec3f dir;
    Vec3f inv_d; // Inverse of each Ray Direction component
};

} /* namespace traceraptor */

#endif /* TRACERAPTOR_RAY_H_ */
