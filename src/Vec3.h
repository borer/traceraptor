/*
 * Vec3.h
 *
 *  Created on: Nov 21, 2016
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_VEC3_H_
#define TRACERAPTOR_VEC3_H_

#include <iostream>
#include <cmath>

namespace traceraptor {

class Vec3 {
public:
    Vec3(){}

    Vec3(float e0, float e1, float e2) {
    	e[0] = e0;e[1] = e1;e[2] = e2;
    }

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

    inline float length() const {return std::sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);}
    inline float squared_length() const {return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];}
    inline void make_unit_vector();

    float e[3];
};

inline void Vec3::make_unit_vector() {
	float k = 1.0/ std::sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
	e[0] = k;
	e[1] *= k;
	e[2] *= k;
}

inline std::istream& operator >> (std::istream &is, Vec3 &t) {
	is >> t.e[0] >> t.e[1] >> t.e[2];
	return is;
}

inline std::ostream& operator << (std::ostream &os, Vec3 &t) {
	os << t.e[0] << " " << t.e[1] << " " << t.e[2];
	return os;
}

inline Vec3 operator + (const Vec3 &v1, const Vec3 &v2) {
	return Vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline Vec3 operator - (const Vec3 &v1, const Vec3 &v2) {
	return Vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline Vec3 operator * (const Vec3 &v1, const Vec3 &v2) {
	return Vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline Vec3 operator * (float t, const Vec3 &v1) {
	return Vec3(v1.e[0]*t, v1.e[1]*t, v1.e[2]*t);
}

inline Vec3 operator * (const Vec3 &v1, float t) {
	return Vec3(v1.e[0]*t, v1.e[1]*t, v1.e[2]*t);
}

inline Vec3 operator / (const Vec3 &v1, const Vec3 &v2) {
	return Vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline Vec3 operator / (const Vec3 &v1, float t) {
	return Vec3(v1.e[0]/t, v1.e[1]/t, v1.e[2]/t);
}

inline Vec3 component_min(const Vec3& a, const Vec3& b) {
	return Vec3(a[0] < b[0] ? a[0] : b[0],
			a[1] < b[1] ? a[1] : b[1],
			a[2] < b[2] ? a[2] : b[2]);
}

inline Vec3 component_max(const Vec3& a, const Vec3& b) {
	return Vec3(a[0] > b[0] ? a[0] : b[0],
			  	a[1] > b[1] ? a[1] : b[1],
			  	a[2] > b[2] ? a[2] : b[2]);
}

inline Vec3& Vec3::operator +=(const Vec3& v2) {
	e[0] += v2[0];
	e[1] += v2[1];
	e[2] += v2[2];

	return *this;
}

inline Vec3& Vec3::operator -=(const Vec3& v2) {
	e[0] -= v2[0];
	e[1] -= v2[1];
	e[2] -= v2[2];

	return *this;
}

inline Vec3& Vec3::operator *=(const Vec3& v2) {
	e[0] *= v2[0];
	e[1] *= v2[1];
	e[2] *= v2[2];

	return *this;
}

inline Vec3& Vec3::operator /=(const Vec3& v2) {
	e[0] /= v2[0];
	e[1] /= v2[1];
	e[2] /= v2[2];

	return *this;
}

inline Vec3& Vec3::operator *=(float t) {
	e[0] *= t;
	e[1] *= t;
	e[2] *= t;

	return *this;
}

inline Vec3& Vec3::operator /=(float t) {
	float k = 1.0/t;

	e[0] *= k;
	e[1] *= k;
	e[2] *= k;

	return *this;
}

inline Vec3 unit_vector(Vec3 v){
    return v / v.length();
}

inline float dot(const Vec3 &v1, const Vec3 &v2) {
	return v1.e[0]*v2.e[0] + v1.e[1]*v2.e[1] + v1.e[2]*v2.e[2];
}

inline Vec3 cross(const Vec3 &v1, const Vec3 &v2){
	return Vec3(v1.e[1]*v2.e[2] - v1.e[2]*v2.e[1],
			-(v1.e[0]*v2.e[2]-v1.e[2]*v2.e[0]),
			v1.e[0]*v2.e[1]-v1.e[1]*v2.e[0]);
}

} /* namespace traceraptor */

#endif /* TRACERAPTOR_VEC3_H_ */
