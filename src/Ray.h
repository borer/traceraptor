/*
 * Ray.h
 *
 *  Created on: Nov 22, 2016
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_RAY_H_
#define TRACERAPTOR_RAY_H_

#include "Vec3.h"

namespace traceraptor {

class Ray {
public:
    Ray(){}
    Ray(const Vec3& a, const Vec3& b) : o(a), dir(b) {
    	inv_d = Vec3(1,1,1)/dir;
    }

    Vec3 origin() const {return o;}
    Vec3 direction() const {return dir;}

    Vec3 point_at_parameter(float t) const {
        return o + dir*t;
    }

    Vec3 o;
    Vec3 dir;
    Vec3 inv_d; // Inverse of each Ray Direction component
};

} /* namespace traceraptor */

#endif /* TRACERAPTOR_RAY_H_ */
