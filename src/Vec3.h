/*
 * Vec3.h
 *
 *  Created on: Nov 21, 2016
 *      Author: bogdan
 */

#ifndef SRC_VEC3_H_
#define SRC_VEC3_H_

#include <math.h>
#include <stdlib.h>
#include <iostream>

namespace traceraptor {

class Vec3 {
public:
    Vec3(){}

    Vec3(float e0, float e1, float e2);

    inline float x() const {return e[0];}
    inline float y() const {return e[1];}
    inline float z() const {return e[2];}

    inline float r() const {return e[0];}
    inline float g() const {return e[1];}
    inline float b() const {return e[2];}

    inline const Vec3& operator+() const {return *this;}
    inline Vec3 operator-() const {return Vec3(-e[0], -e[1], -e[2]);}
    inline float operator[](int i) const {return e[i];}
    inline float& operator[](int i) {return e[i];};

    inline Vec3& operator += (const Vec3 &v2);
    inline Vec3& operator -= (const Vec3 &v2);
    inline Vec3& operator *= (const Vec3 &v2);
    inline Vec3& operator /= (const Vec3 &v2);
    inline Vec3& operator *= (const float t);
    inline Vec3& operator /= (const float t);

    inline float length() const {return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);}
    inline float squared_length() const {return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];}
    inline void make_unit_vector();

    inline float dot(const Vec3 &v1) const{
        return v1.e[0] * e[0] + v1.e[1]*e[1] + v1.e[2]*e[2];
    }

    inline Vec3 cross(const Vec3 &v1, const Vec3 &v2){
        return Vec3(v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1],
                    -(v1.e[0]*v2.e[2]-v1.e[2]*v2.e[0]),
                    v1.e[0]*v2.e[1]-v1.e[1]*v2.e[0]);
    }

    Vec3 unit_vector();

    float e[3];
};

} /* namespace traceraptor */

#endif /* SRC_VEC3_H_ */
