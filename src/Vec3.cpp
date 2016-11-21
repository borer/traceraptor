/*
 * Vec3.cpp
 *
 *  Created on: Nov 21, 2016
 *      Author: bogdan
 */

#include <Vec3.h>

namespace traceraptor {

Vec3::Vec3(float e0, float e1, float e2){
    e[0] = e0;e[1] = e1;e[2] = e2;
}

Vec3 Vec3::unit_vector(){
    float l = length();
    return Vec3(e[0]/l, e[1]/l, e[2]/l);
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

inline Vec3 operator / (const Vec3 &v1, const Vec3 &v2) {
	return Vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline Vec3 operator / (const Vec3 &v1, float t) {
	return Vec3(v1.e[0]/t, v1.e[1]/t, v1.e[2]/t);
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

} /* namespace traceraptor */
