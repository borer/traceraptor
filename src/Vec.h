/*
 * Vec.h
 *
 *  Created on: Nov 21, 2016
 *      Author: bogdan
 */

#ifndef TRACERAPTOR_VEC_H_
#define TRACERAPTOR_VEC_H_

#include <iostream>
#include <cmath>
#include <cassert>
#include <array>

namespace traceraptor {

template <typename T>
constexpr inline T min_comparator(T x, T y) {
    return (x < y) ? x : y;
}

template <typename T>
constexpr inline T max_comparator(T x, T y) {
    return (x > y) ? x : y;
}

template <typename T>
constexpr inline T clamp(T x, T min_, T max_) {
    return min_comparator(max_comparator(x, min_), max_);
}

template <typename T, size_t N>
class Vec : public std::array<T, N> {
public:
    /// default constructor
    constexpr Vec() {}

    /// list constructor
    constexpr Vec(std::initializer_list<T> vv) {
        assert(N == vv.size());
        auto i = 0;
        for (auto&& e : vv) (*this)[i++] = e;
    }

    /// copy constructor form array
    constexpr Vec(const std::array<T, N>& vv) : std::array<T, N>{vv} {}

    constexpr T x() const {
    	assert(1 <= (*this).size());
    	return (*this)[0];
    }
    constexpr T y() const {
    	assert(2 <= (*this).size());
    	return (*this)[1];
    }
    constexpr T z() const {
    	assert(3 <= (*this).size());
    	return (*this)[2];
    }
    constexpr T w() const {
    	assert(4 <= (*this).size());
    	return (*this)[3];
    }
};

using Vec1f = Vec<float, 1>;
using Vec2f = Vec<float, 2>;
using Vec3f = Vec<float, 3>;
using Vec4f = Vec<float, 4>;

using Vec1i = Vec<int, 1>;
using Vec2i = Vec<int, 2>;
using Vec3i = Vec<int, 3>;
using Vec4i = Vec<int, 4>;

template <typename T, size_t N>
constexpr inline Vec<T, N> zero_vec() {
    Vec<T, N> c;
    for (size_t i = 0; i < N; i++) c[i] = 0;
    return c;
}

const auto Zero1f = zero_vec<float, 1>();
const auto Zero2f = zero_vec<float, 2>();
const auto Zero3f = zero_vec<float, 3>();
const auto Zero4f = zero_vec<float, 4>();

const auto Zero1i = zero_vec<int, 1>();
const auto Zero2i = zero_vec<int, 2>();
const auto Zero3i = zero_vec<int, 3>();
const auto Zero4i = zero_vec<int, 4>();

template <typename T, size_t N>
constexpr inline std::istream& operator >> (std::istream &is, const Vec<T, N>& a) {
	for (auto i = 0; i < N; i++) is >> a[i];
	return is;
}

template <typename T, size_t N>
constexpr inline std::ostream& operator << (std::ostream &os, const Vec<T, N>& a) {
	for (auto i = 0; i < N; i++) os << a[i] << " ";
	return os;
}

template <typename T, size_t N>
constexpr inline Vec<T, N> operator+(const Vec<T, N>& a) {
    Vec<T, N> c;
    for (auto i = 0; i < N; i++) c[i] = +a[i];
    return c;
}

template <typename T, size_t N>
constexpr inline Vec<T, N> operator-(const Vec<T, N>& a) {
    Vec<T, N> c;
    for (size_t i = 0; i < N; i++) c[i] = -a[i];
    return c;
}

template <typename T, size_t N>
constexpr inline Vec<T, N> operator+(const Vec<T, N>& a, const Vec<T, N>& b) {
    Vec<T, N> c;
    for (size_t i = 0; i < N; i++) c[i] = a[i] + b[i];
    return c;
}

template <typename T, size_t N>
constexpr inline Vec<T, N> operator-(const Vec<T, N>& a, const Vec<T, N>& b) {
    Vec<T, N> c;
    for (size_t i = 0; i < N; i++) c[i] = a[i] - b[i];
    return c;
}

template <typename T, size_t N>
constexpr inline Vec<T, N> operator*(const Vec<T, N>& a, const Vec<T, N>& b) {
    Vec<T, N> c;
    for (size_t i = 0; i < N; i++) c[i] = a[i] * b[i];
    return c;
}

template <typename T, size_t N>
constexpr inline Vec<T, N> operator*(const Vec<T, N>& a, const T b) {
    Vec<T, N> c;
    for (size_t i = 0; i < N; i++) c[i] = a[i] * b;
    return c;
}

template <typename T, size_t N>
constexpr inline Vec<T, N> operator*(const T a, const Vec<T, N>& b) {
    Vec<T, N> c;
    for (size_t i = 0; i < N; i++) c[i] = a * b[i];
    return c;
}

template <typename T, size_t N>
constexpr inline Vec<T, N> operator/(const Vec<T, N>& a, const Vec<T, N>& b) {
    Vec<T, N> c;
    for (size_t i = 0; i < N; i++) c[i] = a[i] / b[i];
    return c;
}

template <typename T, size_t N>
constexpr inline Vec<T, N> operator/(const Vec<T, N>& a, const T b) {
    Vec<T, N> c;
    for (size_t i = 0; i < N; i++) c[i] = a[i] / b;
    return c;
}

template <typename T1, typename T, size_t N>
constexpr inline Vec<T, N> operator/(const T1& a, const Vec<T, N>& b) {
    Vec<T, N> c;
    for (size_t i = 0; i < N; i++) c[i] = a / b[i];
    return c;
}

template <typename T, size_t N>
constexpr inline Vec<T, N>& operator+=(Vec<T, N>& a, const Vec<T, N>& b) {
    return a = a + b;
}

template <typename T, size_t N>
constexpr inline Vec<T, N>& operator-=(Vec<T, N>& a, const Vec<T, N>& b) {
    return a = a - b;
}

template <typename T, size_t N>
constexpr inline Vec<T, N>& operator*=(Vec<T, N>& a, const Vec<T, N>& b) {
    return a = a * b;
}

template <typename T, size_t N>
constexpr inline Vec<T, N>& operator*=(Vec<T, N>& a, const T b) {
    return a = a * b;
}

template <typename T, size_t N>
constexpr inline Vec<T, N>& operator/=(Vec<T, N>& a, const Vec<T, N>& b) {
    return a = a / b;
}

template <typename T, size_t N>
constexpr inline Vec<T, N>& operator/=(Vec<T, N>& a, const T b) {
    return a = a / b;
}

template <typename T, size_t N>
constexpr inline T dot(const Vec<T, N>& a, const Vec<T, N>& b) {
    auto c = T(0);
    for (size_t i = 0; i < N; i++) c += a[i] * b[i];
    return c;
}

template <typename T, size_t N>
constexpr inline T length(const Vec<T, N>& a) {
    return sqrt(dot(a, a));
}

template <typename T, size_t N>
constexpr inline T lengthsqr(const Vec<T, N>& a) {
    return dot(a, a);
}

template <typename T, size_t N>
constexpr inline Vec<T, N> normalize(const Vec<T, N>& a) {
    auto l = length(a);
    if (l == 0) return a;
    return a * (1 / l);
}

template <typename T, size_t N>
constexpr inline T dist(const Vec<T, N>& a, const Vec<T, N>& b) {
    return length(a - b);
}

template <typename T, size_t N>
constexpr inline T distsqr(const Vec<T, N>& a, const Vec<T, N>& b) {
    return lengthsqr(a - b);
}

template <typename T>
constexpr inline T cross(const Vec<T, 2>& a, const Vec<T, 2>& b) {
    return a[0] * b[1] - a[1] * b[0];
}

template <typename T>
constexpr inline Vec<T, 3> cross(const Vec<T, 3>& a, const Vec<T, 3>& b) {
    return {a[1] * b[2] - a[2] * b[1], a[2] * b[0] - a[0] * b[2],
            a[0] * b[1] - a[1] * b[0]};
}

/// angle between normalized vectors
template <typename T, size_t N>
constexpr inline T uangle(const Vec<T, N>& a, const Vec<T, N>& b) {
    auto d = dot(a, b);
    return d > 1 ? 0 : std::acos(d < -1 ? -1 : d);
}

/// angle between vectors
template <typename T, size_t N>
constexpr inline T angle(const Vec<T, N>& a, const Vec<T, N>& b) {
    return uangle(normalize(a), normalize(b));
}

template <typename T, size_t N>
constexpr inline Vec<T, N> lerp(const Vec<T, N>& a, const Vec<T, N>& b, T t) {
    return a * (1 - t) + b * t;
}

template <typename T, size_t N>
constexpr inline Vec<T, N> normalized_lerp(const Vec<T, N>& a, const Vec<T, N>& b, T t) {
    return normalize(lerp(a, b, t));
}

/// vector spherical linear interpolation (vectors have to be normalized)
template <typename T, size_t N>
constexpr inline Vec<T, N> spherical_lerp(const Vec<T, N>& a, const Vec<T, N>& b, T t) {
    auto th = uangle(a, b);
    return th == 0 ? a
                   : a * (std::sin(th * (1 - t)) / std::sin(th)) +
                         b * (std::sin(th * t) / std::sin(th));
}

// http://lolengine.net/blog/2013/09/21/picking-orthogonal-vector-combing-coconuts)
template <typename T>
constexpr inline Vec<T, 3> orthogonal(const Vec<T, 3>& v) {
    return std::abs(v[0]) > std::abs(v[2]) ? Vec<T, 3>{-v[1], v[0], 0}
                                           : Vec<T, 3>{0, -v[2], v[1]};
}

template <typename T>
constexpr inline Vec<T, 3> orthonormalize(const Vec<T, 3>& a,
                                          const Vec<T, 3>& b) {
    return normalize(a - b * ym_dot(a, b));
}

template <typename T, size_t N>
constexpr inline T sum_elements(const Vec<T, N>& a) {
    auto s = T{0};
    for (auto i = 0; i < N; i++) s += a[i];
    return s;
}

template <typename T, size_t N>
constexpr inline T mean_elemens(const Vec<T, N>& a) {
    return sum(a) / N;
}

template <typename T, size_t N>
constexpr inline Vec<T, N> component_min(const Vec<T, N>& a, const Vec<T, N>& b) {
    Vec<T, N> c;
    for (size_t i = 0; i < N; i++) c[i] = min_comparator(a[i], b[i]);
    return c;
}

template <typename T, size_t N>
constexpr inline Vec<T, N> component_max(const Vec<T, N>& a, const Vec<T, N>& b) {
    Vec<T, N> c;
    for (size_t i = 0; i < N; i++) c[i] = max_comparator(a[i], b[i]);
    return c;
}

template <typename T, size_t N>
constexpr inline Vec<T, N> component_min(const Vec<T, N>& a, const T& b) {
    Vec<T, N> c;
    for (auto i = 0; i < N; i++) c[i] = min(a[i], b);
    return c;
}

template <typename T, size_t N>
constexpr inline Vec<T, N> component_max(const Vec<T, N>& a, const T& b) {
    Vec<T, N> c;
    for (auto i = 0; i < N; i++) c[i] = max(a[i], b);
    return c;
}

template <typename T, size_t N>
constexpr inline Vec<T, N> component_clamp(const Vec<T, N>& x, const T& min,
                                 const T& max) {
    Vec<T, N> c;
    for (size_t i = 0; i < N; i++) c[i] = clamp(x[i], min, max);
    return c;
}

template <typename T, size_t N>
constexpr inline Vec<T, N> component_clamp(const Vec<T, N>& x, const Vec<T, N>& min,
                                 const Vec<T, N>& max) {
    Vec<T, N> c;
    for (auto i = 0; i < N; i++) c[i] = clamp(x[i], min[i], max[i]);
    return c;
}

template <typename T, size_t N, typename T1>
constexpr inline Vec<T, N> clamp_len(const Vec<T, N> x, T1 max) {
    auto l = length(x);
    return (l > (T)max) ? x * (T)max / l : x;
}

template <typename T, size_t N>
constexpr inline int min_element_index(const Vec<T, N>& a) {
    auto v = std::numeric_limits<T>::max();
    auto pos = -1;
    for (auto i = 0; i < N; i++) {
        if (v > a[i]) {
            v = a[i];
            pos = i;
        }
    }
    return pos;
}

template <typename T, size_t N>
constexpr inline int max_element_index(const Vec<T, N>& a) {
    auto v = -std::numeric_limits<T>::max();
    auto pos = -1;
    for (auto i = 0; i < N; i++) {
        if (v < a[i]) {
            v = a[i];
            pos = i;
        }
    }
    return pos;
}

} /* namespace traceraptor */

#endif /* TRACERAPTOR_VEC_H_ */
