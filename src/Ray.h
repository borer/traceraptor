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
    Ray(const Vec3& a, const Vec3& b){A = a; B = b;}

    Vec3 origin() const {return A;}
    Vec3 direction() const {return B;}

    Vec3 point_at_parameter(float t) const {
        return A + B*t;
    }

    Vec3 A;
    Vec3 B;
};

} /* namespace traceraptor */

#endif /* TRACERAPTOR_RAY_H_ */
