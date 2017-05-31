/*
 * Transform.h
 *
 *  Created on: May 30, 2017
 *      Author: bogdan
 */

#ifndef SRC_TRANSFORM_H_
#define SRC_TRANSFORM_H_

#include <Vec.h>
#include <MathUtil.h>
#include <Ray.h>
#include <Intersection.h>
#include <BBox.h>

namespace traceraptor {

struct Matrix4x4 {
    Matrix4x4() {
        m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.f;
        m[0][1] = m[0][2] = m[0][3] = m[1][0] = m[1][2] = m[1][3] = m[2][0] =
        m[2][1] = m[2][3] = m[3][0] = m[3][1] = m[3][2] = 0.f;
    }

    Matrix4x4(Float mat[4][4]);

    Matrix4x4(Float t00, Float t01, Float t02, Float t03, Float t10, Float t11,
    		Float t12, Float t13, Float t20, Float t21, Float t22, Float t23,
			Float t30, Float t31, Float t32, Float t33);

    bool operator==(const Matrix4x4 &m2) const {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                if (m[i][j] != m2.m[i][j]) return false;
        return true;
    }
    bool operator!=(const Matrix4x4 &m2) const {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                if (m[i][j] != m2.m[i][j]) return true;
        return false;
    }
    friend Matrix4x4 Transpose(const Matrix4x4 &);

    static Matrix4x4 Mul(const Matrix4x4 &m1, const Matrix4x4 &m2) {
        Matrix4x4 r;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                r.m[i][j] = m1.m[i][0] * m2.m[0][j] + m1.m[i][1] * m2.m[1][j] +
                            m1.m[i][2] * m2.m[2][j] + m1.m[i][3] * m2.m[3][j];
        return r;
    }
    friend Matrix4x4 Inverse(const Matrix4x4 &);

    Float m[4][4];
};

class Transform {
public:
    Transform() {}
    Transform(const Float mat[4][4]) {
        m = Matrix4x4(mat[0][0], mat[0][1], mat[0][2], mat[0][3], mat[1][0],
                      mat[1][1], mat[1][2], mat[1][3], mat[2][0], mat[2][1],
                      mat[2][2], mat[2][3], mat[3][0], mat[3][1], mat[3][2],
                      mat[3][3]);
        mInv = Inverse(m);
    }
    Transform(const Matrix4x4 &m) : m(m), mInv(Inverse(m)) {}
    Transform(const Matrix4x4 &m, const Matrix4x4 &mInv) : m(m), mInv(mInv) {}

    friend Transform Inverse(const Transform &t) {
        return Transform(t.mInv, t.m);
    }

    friend Transform Transpose(const Transform &t) {
        return Transform(Transpose(t.m), Transpose(t.mInv));
    }

    bool operator==(const Transform &t) const {
        return t.m == m && t.mInv == mInv;
    }

    bool operator!=(const Transform &t) const {
        return t.m != m || t.mInv != mInv;
    }

    bool operator<(const Transform &t2) const {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j) {
                if (m.m[i][j] < t2.m.m[i][j]) return true;
                if (m.m[i][j] > t2.m.m[i][j]) return false;
            }
        return false;
    }

    Transform operator*(const Transform &t2) const {
        return Transform(Matrix4x4::Mul(m, t2.m), Matrix4x4::Mul(t2.mInv, mInv));
    }

    template <typename T>
    inline Vec<T, 3> operator()(const Vec<T, 3> &v) const {
        T x = v.x(), y = v.y(), z = v.z();
        return Vec<T,3>({m.m[0][0] * x + m.m[0][1] * y + m.m[0][2] * z + m.m[0][3],
                         m.m[1][0] * x + m.m[1][1] * y + m.m[1][2] * z + m.m[1][3],
                         m.m[2][0] * x + m.m[2][1] * y + m.m[2][2] * z + m.m[2][3]});
    }

    inline Ray operator()(const Ray &r) const {
    	Vec3f o = (*this)(r.o);
    	Vec3f d = (*this)(r.dir);
    	return Ray(o, d);
    }

    inline void operator()(IntersectionInfo &intersectionInfo) const {
        intersectionInfo.hit_point = (*this)(intersectionInfo.hit_point);
        intersectionInfo.normal = (*this)(intersectionInfo.normal);
    }

    inline BBox operator()(const BBox &b) const {
        const Transform &M = *this;
        BBox ret(M(Vec3f({b.min.x(), b.min.y(), b.min.z()})));
        ret.expandToInclude(M(Vec3f({b.min.x(), b.min.y(), b.max.z()})));
        ret.expandToInclude(M(Vec3f({b.min.x(), b.max.y(), b.min.z()})));
        ret.expandToInclude(M(Vec3f({b.min.x(), b.max.y(), b.max.z()})));
        ret.expandToInclude(M(Vec3f({b.max.x(), b.min.y(), b.min.z()})));
        ret.expandToInclude(M(Vec3f({b.max.x(), b.min.y(), b.max.z()})));
        ret.expandToInclude(M(Vec3f({b.max.x(), b.max.y(), b.min.z()})));
        ret.expandToInclude(M(Vec3f({b.max.x(), b.max.y(), b.max.z()})));

        return ret;
    }

    bool IsIdentity() const {
        return (m.m[0][0] == 1.f && m.m[0][1] == 0.f && m.m[0][2] == 0.f &&
                m.m[0][3] == 0.f && m.m[1][0] == 0.f && m.m[1][1] == 1.f &&
                m.m[1][2] == 0.f && m.m[1][3] == 0.f && m.m[2][0] == 0.f &&
                m.m[2][1] == 0.f && m.m[2][2] == 1.f && m.m[2][3] == 0.f &&
                m.m[3][0] == 0.f && m.m[3][1] == 0.f && m.m[3][2] == 0.f &&
                m.m[3][3] == 1.f);
    }

    const Matrix4x4 &GetMatrix() const { return m; }
    const Matrix4x4 &GetInverseMatrix() const { return mInv; }

private:
    Matrix4x4 m, mInv;
};

Transform Translate(const Vec3f &delta);
Transform Scale(Float x, Float y, Float z);
Transform RotateX(Float theta);
Transform RotateY(Float theta);
Transform RotateZ(Float theta);
Transform Rotate(Float theta, const Vec3f &axis);
Transform LookAt(const Vec3f &pos, const Vec3f &look, const Vec3f &up);
Transform Orthographic(Float znear, Float zfar);
Transform Perspective(Float fov, Float znear, Float zfar);

} /* namespace traceraptor */

#endif /* SRC_TRANSFORM_H_ */
