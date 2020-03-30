/*
    MathTypes.h

    Copyright © 2019 Occipital, Inc. All rights reserved.
    This file is part of the Structure SDK.
    Unauthorized copying of this file, via any medium is strictly prohibited.
    Proprietary and confidential.

    http://structure.io
*/

#pragma once

#include <ST/Macros.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <ostream>
#include <limits>

#if defined(_MSC_VER) && _MSC_VER <= 1800
#define constexpr
#endif

namespace ST
{

//------------------------------------------------------------------------------

static inline bool epsilonEquals          (float  first,  float second,  float epsilon = std::numeric_limits<float> ::epsilon());
static inline bool epsilonEqualsOrBothNan (float  first,  float second,  float epsilon = std::numeric_limits<float> ::epsilon());
static inline bool epsilonEquals          (double first, double second, double epsilon = std::numeric_limits<double>::epsilon());
static inline bool epsilonEqualsOrBothNan (double first, double second, double epsilon = std::numeric_limits<double>::epsilon());

//------------------------------------------------------------------------------

template <class Scalar>
struct Vector2
{
    union
    {
        struct { Scalar x, y; };
        struct { Scalar s, t; };

        Scalar v[2];
    };

    static inline constexpr Vector2 zero();

    static Vector2 lerp(const Vector2& from, const Vector2& to, Scalar t);

    Vector2() = default;
    Vector2(const Vector2&) = default;

    constexpr Vector2(Scalar _x, Scalar _y);

    Vector2& operator=(const Vector2&) = default;

    bool epsilonEquals(const Vector2& other, Scalar epsilon = std::numeric_limits<Scalar>::epsilon());
    bool epsilonEqualsOrBothNan(const Vector2& other, Scalar epsilon = std::numeric_limits<Scalar>::epsilon());
};

using Vector2u = Vector2<unsigned>;
using Vector2f = Vector2<float>;
using Vector2d = Vector2<double>;

//------------------------------------------------------------------------------

template <class Scalar>
struct Vector3
{
    union
    {
        struct { Scalar x, y, z; };
        struct { Scalar r, g, b; };
        struct { Scalar s, t, p; };

        float v[3];
    };

    static constexpr Vector3 zero();

    Vector3() = default;
    Vector3(const Vector3&) = default;

    constexpr Vector3(Scalar _x, Scalar _y, Scalar _z);

    Vector3& operator=(const Vector3&) = default;

    Vector3 operator-() const;

    Vector3 operator-(const Vector3& rv) const;
    Vector3 operator+(const Vector3& rv) const;

    Vector3& operator+=(const Vector3& rv);

    Vector3 operator*(Scalar s) const;

    float length() const;
    Vector3 normalized() const;
    Scalar dot(const Vector3& v) const;
    Vector3 cross(const Vector3& v) const;

    bool epsilonEquals(const Vector3& other, Scalar epsilon = std::numeric_limits<Scalar>::epsilon());
    bool epsilonEqualsOrBothNan(const Vector3& other, Scalar epsilon = std::numeric_limits<Scalar>::epsilon());
};

using Vector3f = Vector3<float>;
using Vector3d = Vector3<double>;

//------------------------------------------------------------------------------

struct ST_ALIGNED(16) Vector4
{
    union
    {
        struct { float x, y, z, w; };
        struct { float r, g, b, a; };
        struct { float s, t, p, q; };

        float v[4];
    };

    Vector4() = default;
    Vector4(const Vector4&) = default;

    Vector4& operator=(const Vector4&) = default;

    inline constexpr Vector4(float _x, float _y, float _z, float _w);

    bool epsilonEquals(const Vector4& other, float epsilon = std::numeric_limits<float>::epsilon());
    bool epsilonEqualsOrBothNan(const Vector4& other, float epsilon = std::numeric_limits<float>::epsilon());
};

//------------------------------------------------------------------------------

enum class Handedness
{
    Left,
    Right
};

//------------------------------------------------------------------------------

// Column major.
// Indices are named: mColumnRow.
// Translation component is thus m30, m31, m32.
struct ST_API ST_ALIGNED(16) Matrix4
{
    union
    {
        struct
        {
            float m00, m01, m02, m03;
            float m10, m11, m12, m13;
            float m20, m21, m22, m23;
            float m30, m31, m32, m33;
        };

        float m[16];
    };

    inline static Matrix4 identity();
    inline static Matrix4 nan();

    inline static Matrix4 lookAt(const Vector3f& eyePosition, const Vector3f& lookAt, const Vector3f& up);
    inline static Matrix4 projection(float fovY, float aspectRatio, float minZ, float maxZ, Handedness handedness = Handedness::Left);

    Matrix4() = default;
    Matrix4(const Matrix4& copy) = default;

    inline Matrix4(const std::array<float, 16>& data);

    Matrix4& operator=(const Matrix4& rhs) = default;

    // Note: not exposing operator() for a while to catch any misuse of the col, row order.
    inline float& atRowCol(int row, int col);
    inline const float& atRowCol(int row, int col) const;

    /*inline float& operator()(int col, int row);
    inline const float& operator()(int col, int row) const;*/

    inline Matrix4  operator*(const Matrix4& rv) const;
    inline Vector4  operator*(const Vector4& v) const;
    inline Vector3f operator*(const Vector3f& v) const;

    inline Matrix4 transposed() const;

    inline Matrix4 inversed() const;

    bool isZero() const;
    bool hasNan() const;
    bool isApprox(const Matrix4& rhs) const;

    Matrix4 scaled(float scaleX, float scaleY, float scaleZ) const;

    // Applies the translation on the right side: output = input * translation
    Matrix4 translated(float tX, float tY, float tZ) const;

    // Applies the rotation on the right side: output = input * rotation
    Matrix4 rotated(const Vector4& quaternion) const;

    // Applies the rotation on the right side: output = input * rotation
    // The eulerAngles will be applied using the Z * Y * X order, assuming radians.
    Matrix4 rotated(const Vector3f& eulerAnglesRad) const;

    Vector3f translation() const;

    Vector4 rotationAsQuaternion() const;

    float angleBetweenPoses(const Matrix4& worldFromFrame) const;

    Vector3f rvecBetweenPoses(const ST::Matrix4& worldFromFrame) const;

    Matrix4 extrapolatePose(ST::Vector3f& linearVelocityInWorld,
                            ST::Vector3f& linearAccelerationInWorld,
                            ST::Vector3f& angularVelocityInFrame,
                            ST::Vector3f& angularAccelerationInFrame,
                            const double dt) const;

    void fromExtrinsics(float tx, float ty, float tz, float qx, float qy, float qz, float qw);

    inline Vector3f rotationAsZYXEuler() const;

    // Returns a transform that has the same rotation but a zero translation.
    inline Matrix4 rotationOnly() const;

    std::string toString() const;

    Matrix4 interpolateBetweenPoses(const double alpha, const Matrix4& other) const;
};

inline std::ostream&
operator<< (std::ostream& o, const Matrix4& m);

//------------------------------------------------------------------------------

static inline bool epsilonEquals(float lhs, float rhs, float epsilon)
{
    return std::abs(lhs - rhs) <= epsilon;
}

static inline bool epsilonEqualsOrBothNan(float lhs, float rhs, float epsilon)
{
    return (std::abs(lhs - rhs) <= epsilon) || (std::isnan(lhs) && std::isnan(rhs));
}

static inline bool epsilonEquals(double lhs, double rhs, double epsilon)
{
    return std::abs(lhs - rhs) <= epsilon;
}

static inline bool epsilonEqualsOrBothNan(double lhs, double rhs, double epsilon)
{
    return (std::abs(lhs - rhs) <= epsilon) || (std::isnan(lhs) && std::isnan(rhs));
}

//------------------------------------------------------------------------------

template <class Scalar>
inline constexpr Vector2<Scalar>
Vector2<Scalar>::zero()
{
    return Vector2(Scalar(0), Scalar(0));
}

template <class Scalar>
inline constexpr Vector2<Scalar>::Vector2(Scalar _x, Scalar _y) : x(_x) , y(_y) { /* do nothing */ }

template <class Scalar>
inline Vector2<Scalar> Vector2<Scalar>::lerp(const Vector2<Scalar>& from, const Vector2<Scalar>& to, Scalar t)
{
    t = std::max(Scalar(0), std::min(Scalar(1), t));
    const Scalar oneMinusT = Scalar(1) - t;
    return Vector2<Scalar>(from.x * oneMinusT + to.x * t, from.y * oneMinusT + to.y * t);
}

template <class Scalar>
inline bool Vector2<Scalar>::epsilonEquals(const Vector2& other, Scalar epsilon)
{
    return ::ST::epsilonEquals(v[0], other.v[0], epsilon)
        && ::ST::epsilonEquals(v[1], other.v[1], epsilon);
}

template <class Scalar>
inline bool Vector2<Scalar>::epsilonEqualsOrBothNan(const Vector2& other, Scalar epsilon)
{
    return ::ST::epsilonEqualsOrBothNan(v[0], other.v[0], epsilon)
        && ::ST::epsilonEqualsOrBothNan(v[1], other.v[1], epsilon);
}

//------------------------------------------------------------------------------

template <class Scalar>
inline constexpr Vector3<Scalar> Vector3<Scalar>::zero()
{
    return Vector3(Scalar(0), Scalar(0), Scalar(0));
}

template <class Scalar>
inline constexpr Vector3<Scalar>::Vector3(Scalar _x, Scalar _y, Scalar _z) : x(_x) , y(_y) , z(_z) { /* do nothing */ }

template <class Scalar>
inline Vector3<Scalar> Vector3<Scalar>::operator-() const
{
    return Vector3(-x, -y, -z);
}

template <class Scalar>
inline Vector3<Scalar> Vector3<Scalar>::operator-(const Vector3& rv) const
{
    return Vector3(x - rv.x, y - rv.y, z - rv.z);
}

template <class Scalar>
inline Vector3<Scalar> Vector3<Scalar>::operator+(const Vector3& rv) const
{
    return Vector3(x + rv.x, y + rv.y, z + rv.z);
}

template <class Scalar>
inline Vector3<Scalar> Vector3<Scalar>::operator*(Scalar s) const
{
    return Vector3(x * s, y * s, z * s);
}

template <class Scalar>
inline Vector3<Scalar>& Vector3<Scalar>::operator+=(const Vector3& rv)
{
    x += rv.x;
    y += rv.y;
    z += rv.z;

    return *this;
}

template <class Scalar>
inline float Vector3<Scalar>::length() const
{
    return std::sqrt(x * x + y * y + z * z);
}

template <class Scalar>
inline Vector3<Scalar> Vector3<Scalar>::normalized() const
{
    const Scalar invLen = Scalar(1) / std::sqrt(x * x + y * y + z * z);
    return Vector3(x * invLen, y * invLen, z * invLen);
}

template <class Scalar>
inline Scalar Vector3<Scalar>::dot(const Vector3& v_) const
{
    return x * v_.x + y * v_.y + z * v_.z;
}

template <class Scalar>
inline Vector3<Scalar> Vector3<Scalar>::cross(const Vector3& v_) const
{
    return Vector3(y * v_.z - z * v_.y, z * v_.x - x * v_.z, x * v_.y - y * v_.x);
}

template <class Scalar>
inline bool Vector3<Scalar>::epsilonEquals(const Vector3& other, Scalar epsilon)
{
    return ::ST::epsilonEquals(v[0], other.v[0], epsilon)
        && ::ST::epsilonEquals(v[1], other.v[1], epsilon)
        && ::ST::epsilonEquals(v[2], other.v[2], epsilon);
}

template <class Scalar>
inline bool Vector3<Scalar>::epsilonEqualsOrBothNan(const Vector3& other, Scalar epsilon)
{
    return ::ST::epsilonEqualsOrBothNan(v[0], other.v[0], epsilon)
        && ::ST::epsilonEqualsOrBothNan(v[1], other.v[1], epsilon)
        && ::ST::epsilonEqualsOrBothNan(v[2], other.v[2], epsilon);
}

//------------------------------------------------------------------------------

/// @cond
// Suppress out-of-line definitions that cause Doxygen warnings

inline constexpr Vector4::Vector4 (float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

inline bool Vector4::epsilonEquals(const Vector4& other, float epsilon)
{
    return ::ST::epsilonEquals(v[0], other.v[0], epsilon)
        && ::ST::epsilonEquals(v[1], other.v[1], epsilon)
        && ::ST::epsilonEquals(v[2], other.v[2], epsilon)
        && ::ST::epsilonEquals(v[3], other.v[3], epsilon);
}

inline bool Vector4::epsilonEqualsOrBothNan(const Vector4& other, float epsilon)
{
    return ::ST::epsilonEqualsOrBothNan(v[0], other.v[0], epsilon)
        && ::ST::epsilonEqualsOrBothNan(v[1], other.v[1], epsilon)
        && ::ST::epsilonEqualsOrBothNan(v[2], other.v[2], epsilon)
        && ::ST::epsilonEqualsOrBothNan(v[3], other.v[3], epsilon);
}

//------------------------------------------------------------------------------

inline Matrix4 Matrix4::identity()
{
    Matrix4 m;

    m.m00 = 1.0f;    m.m01 = 0.0f;    m.m02 = 0.0f;    m.m03 = 0.0f;
    m.m10 = 0.0f;    m.m11 = 1.0f;    m.m12 = 0.0f;    m.m13 = 0.0f;
    m.m20 = 0.0f;    m.m21 = 0.0f;    m.m22 = 1.0f;    m.m23 = 0.0f;
    m.m30 = 0.0f;    m.m31 = 0.0f;    m.m32 = 0.0f;    m.m33 = 1.0f;

    return m;
}

inline Matrix4 Matrix4::nan()
{
    Matrix4 m;

    m.m00 = NAN;    m.m01 = NAN;    m.m02 = NAN;    m.m03 = NAN;
    m.m10 = NAN;    m.m11 = NAN;    m.m12 = NAN;    m.m13 = NAN;
    m.m20 = NAN;    m.m21 = NAN;    m.m22 = NAN;    m.m23 = NAN;
    m.m30 = NAN;    m.m31 = NAN;    m.m32 = NAN;    m.m33 = NAN;

    return m;
}

inline Matrix4 Matrix4::lookAt(const Vector3f& eyePosition, const Vector3f& lookAt, const Vector3f& up)
{
    Matrix4 m;

    Vector3f fwd = (lookAt - eyePosition).normalized();
    Vector3f nUp = up.normalized();
    Vector3f right = fwd.cross(nUp).normalized();

    nUp = right.cross(fwd);

    m.m[0] = right.x;
    m.m[1] = nUp.x;
    m.m[2] = -fwd.x;
    m.m[3] = 0.0f;

    m.m[4] = right.y;
    m.m[5] = nUp.y;
    m.m[6] = -fwd.y;
    m.m[7] = 0.0f;

    m.m[8] = right.z;
    m.m[9] = nUp.z;
    m.m[10] = -fwd.z;
    m.m[11] = 0.0f;

    m.m[12] = -right.dot(eyePosition);
    m.m[13] = -nUp.dot(eyePosition);
    m.m[14] = fwd.dot(eyePosition);
    m.m[15] = 1.0f;

    return m;
}

inline Matrix4 Matrix4::projection(float fovY, float aspectRatio, float minZ, float maxZ, Handedness handedness)
{
    Matrix4 m;

    const float t = minZ * std::tan(fovY * 0.5f);
    const float r = t * aspectRatio;

    // for right handed coordinate system
    m.m[0] = minZ / r;
    m.m[1] = 0.0f;
    m.m[2] = 0.0f;
    m.m[3] = 0.0f;

    m.m[4] = 0.0f;
    m.m[5] = minZ / t;
    m.m[6] = 0.0f;
    m.m[7] = 0.0f;

    m.m[8] = 0.0f;
    m.m[9] = 0.0f;
    m.m[10] = -(maxZ + minZ) / (maxZ - minZ);
    m.m[11] = -1.0f;

    m.m[12] = 0.0f;
    m.m[13] = 0.0f;
    m.m[14] = -2.0f * maxZ * minZ / (maxZ - minZ);
    m.m[15] = 0.0f;

    if (handedness == Handedness::Left)
    {
        m.m[11] = 1.0f;
        m.m[14] *= -1.0f;
    }

    return m;
}

inline float& Matrix4::atRowCol (int row, int col)
{
    return m[col * 4 + row];
}

inline const float& Matrix4::atRowCol (int row, int col) const
{
    return m[col * 4 + row];
}

inline Matrix4::Matrix4(const std::array<float, 16>& data)
{
    for(int i = 0; i < 16; ++i)
        m[i] = data[i];
}

inline Matrix4 Matrix4::operator*(const Matrix4& rv) const
{
    Matrix4 m_;

    m_.m00 = m00 * rv.m00 + m10 * rv.m01 + m20 * rv.m02 + m30 * rv.m03;
    m_.m10 = m00 * rv.m10 + m10 * rv.m11 + m20 * rv.m12 + m30 * rv.m13;
    m_.m20 = m00 * rv.m20 + m10 * rv.m21 + m20 * rv.m22 + m30 * rv.m23;
    m_.m30 = m00 * rv.m30 + m10 * rv.m31 + m20 * rv.m32 + m30 * rv.m33;

    m_.m01 = m01 * rv.m00 + m11 * rv.m01 + m21 * rv.m02 + m31 * rv.m03;
    m_.m11 = m01 * rv.m10 + m11 * rv.m11 + m21 * rv.m12 + m31 * rv.m13;
    m_.m21 = m01 * rv.m20 + m11 * rv.m21 + m21 * rv.m22 + m31 * rv.m23;
    m_.m31 = m01 * rv.m30 + m11 * rv.m31 + m21 * rv.m32 + m31 * rv.m33;

    m_.m02 = m02 * rv.m00 + m12 * rv.m01 + m22 * rv.m02 + m32 * rv.m03;
    m_.m12 = m02 * rv.m10 + m12 * rv.m11 + m22 * rv.m12 + m32 * rv.m13;
    m_.m22 = m02 * rv.m20 + m12 * rv.m21 + m22 * rv.m22 + m32 * rv.m23;
    m_.m32 = m02 * rv.m30 + m12 * rv.m31 + m22 * rv.m32 + m32 * rv.m33;

    m_.m03 = m03 * rv.m00 + m13 * rv.m01 + m23 * rv.m02 + m33 * rv.m03;
    m_.m13 = m03 * rv.m10 + m13 * rv.m11 + m23 * rv.m12 + m33 * rv.m13;
    m_.m23 = m03 * rv.m20 + m13 * rv.m21 + m23 * rv.m22 + m33 * rv.m23;
    m_.m33 = m03 * rv.m30 + m13 * rv.m31 + m23 * rv.m32 + m33 * rv.m33;

    return m_;
}

inline Vector4 Matrix4::operator*(const Vector4& v) const
{
    return Vector4(
        m00 * v.x + m10 * v.y + m20 * v.z + m30 * v.w,
        m01 * v.x + m11 * v.y + m21 * v.z + m31 * v.w,
        m02 * v.x + m12 * v.y + m22 * v.z + m32 * v.w,
        m03 * v.x + m13 * v.y + m23 * v.z + m33 * v.w
    );
}

inline Vector3f Matrix4::operator*(const Vector3f& v) const
{
    // This ignores w.
    return Vector3f(
        m00 * v.x + m10 * v.y + m20 * v.z + m30,
        m01 * v.x + m11 * v.y + m21 * v.z + m31,
        m02 * v.x + m12 * v.y + m22 * v.z + m32
    );
}

inline Matrix4 Matrix4::transposed() const
{
    Matrix4 r;

    r.m00 = m00; r.m01 = m10; r.m02 = m20; r.m03 = m30;
    r.m10 = m01; r.m11 = m11; r.m12 = m21; r.m13 = m31;
    r.m20 = m02; r.m21 = m12; r.m22 = m22; r.m23 = m32;
    r.m30 = m03; r.m31 = m13; r.m32 = m23; r.m33 = m33;

    return r;
}

inline Matrix4 Matrix4::scaled(float x, float y, float z) const
{
    Matrix4 r = *this;

    r.m[0] *= x;    r.m[4] *= x;     r.m[8 ] *= x;    r.m[12] *= x;
    r.m[1] *= y;    r.m[5] *= y;     r.m[9 ] *= y;    r.m[13] *= y;
    r.m[2] *= z;    r.m[6] *= z;     r.m[10] *= z;    r.m[14] *= z;

    return r;
}

inline Matrix4 Matrix4::translated(float x, float y, float z) const
{
    Matrix4 r = *this;

    r.m[0] += r.m[3] * x;    r.m[4] += r.m[7] * x;    r.m[8 ] += r.m[11] * x;    r.m[12] += r.m[15] * x;
    r.m[1] += r.m[3] * y;    r.m[5] += r.m[7] * y;    r.m[9 ] += r.m[11] * y;    r.m[13] += r.m[15] * y;
    r.m[2] += r.m[3] * z;    r.m[6] += r.m[7] * z;    r.m[10] += r.m[11] * z;    r.m[14] += r.m[15] * z;

    return r;
}

inline Matrix4 Matrix4::inversed() const
{
    // Based on Sony Computer Entertainment's implementation used by Bullet.
    // https://github.com/erwincoumans/sce_vectormath/blob/master/include/vectormath/scalar/cpp/mat_aos.h

    /*

   Vector Math library for 3-D linear algebra (vector, matrix, quaternion)
   SIMD support for x86 SSE, PowerPC (PPU) and the SPU.
   Also includes generic multi-platform scalar version.

   Copyright (C) 2006-2011 Sony Computer Entertainment Inc.
   All rights reserved.

   Redistribution and use in source and binary forms,
   with or without modification, are permitted provided that the
   following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Sony Computer Entertainment Inc nor the names
      of its contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.

    */

    Vector4 res0, res1, res2, res3;
    float mA, mB, mC, mD, mE, mF, mG, mH, mI, mJ, mK, mL, mM, mN, mO, mP;
    float tmp0, tmp1, tmp2, tmp3, tmp4, tmp5;
    float detInv;

    mA = m00;
    mB = m01;
    mC = m02;
    mD = m03;
    mE = m10;
    mF = m11;
    mG = m12;
    mH = m13;
    mI = m20;
    mJ = m21;
    mK = m22;
    mL = m23;
    mM = m30;
    mN = m31;
    mO = m32;
    mP = m33;

    tmp0 = ( ( mK * mD ) - ( mC * mL ) );
    tmp1 = ( ( mO * mH ) - ( mG * mP ) );
    tmp2 = ( ( mB * mK ) - ( mJ * mC ) );
    tmp3 = ( ( mF * mO ) - ( mN * mG ) );
    tmp4 = ( ( mJ * mD ) - ( mB * mL ) );
    tmp5 = ( ( mN * mH ) - ( mF * mP ) );

    res0.x = ( ( ( mJ * tmp1 ) - ( mL * tmp3 ) ) - ( mK * tmp5 ) );
    res0.y = ( ( ( mN * tmp0 ) - ( mP * tmp2 ) ) - ( mO * tmp4 ) );
    res0.z = ( ( ( mD * tmp3 ) + ( mC * tmp5 ) ) - ( mB * tmp1 ) );
    res0.w = ( ( ( mH * tmp2 ) + ( mG * tmp4 ) ) - ( mF * tmp0 ) );

    detInv = ( 1.0f / ( ( ( ( mA * res0.x ) + ( mE * res0.y ) ) + ( mI * res0.z ) ) + ( mM * res0.w ) ) );

    res1.x = ( mI * tmp1 );
    res1.y = ( mM * tmp0 );
    res1.z = ( mA * tmp1 );
    res1.w = ( mE * tmp0 );

    res3.x = ( mI * tmp3 );
    res3.y = ( mM * tmp2 );
    res3.z = ( mA * tmp3 );
    res3.w = ( mE * tmp2 );

    res2.x = ( mI * tmp5 );
    res2.y = ( mM * tmp4 );
    res2.z = ( mA * tmp5 );
    res2.w = ( mE * tmp4 );

    tmp0 = ( ( mI * mB ) - ( mA * mJ ) );
    tmp1 = ( ( mM * mF ) - ( mE * mN ) );
    tmp2 = ( ( mI * mD ) - ( mA * mL ) );
    tmp3 = ( ( mM * mH ) - ( mE * mP ) );
    tmp4 = ( ( mI * mC ) - ( mA * mK ) );
    tmp5 = ( ( mM * mG ) - ( mE * mO ) );

    res2.x = ( ( ( mL * tmp1 ) - ( mJ * tmp3 ) ) + res2.x );
    res2.y = ( ( ( mP * tmp0 ) - ( mN * tmp2 ) ) + res2.y );
    res2.z = ( ( ( mB * tmp3 ) - ( mD * tmp1 ) ) - res2.z );
    res2.w = ( ( ( mF * tmp2 ) - ( mH * tmp0 ) ) - res2.w );
    res3.x = ( ( ( mJ * tmp5 ) - ( mK * tmp1 ) ) + res3.x );
    res3.y = ( ( ( mN * tmp4 ) - ( mO * tmp0 ) ) + res3.y );
    res3.z = ( ( ( mC * tmp1 ) - ( mB * tmp5 ) ) - res3.z );
    res3.w = ( ( ( mG * tmp0 ) - ( mF * tmp4 ) ) - res3.w );
    res1.x = ( ( ( mK * tmp3 ) - ( mL * tmp5 ) ) - res1.x);
    res1.y = ( ( ( mO * tmp2 ) - ( mP * tmp4 ) ) - res1.y );
    res1.z = ( ( ( mD * tmp5 ) - ( mC * tmp3 ) ) + res1.z );
    res1.w = ( ( ( mH * tmp4 ) - ( mG * tmp2 ) ) + res1.w );

    Matrix4 res;

    res.m00 = res0.x * detInv;
    res.m01 = res0.y * detInv;
    res.m02 = res0.z * detInv;
    res.m03 = res0.w * detInv;

    res.m10 = res1.x * detInv;
    res.m11 = res1.y * detInv;
    res.m12 = res1.z * detInv;
    res.m13 = res1.w * detInv;

    res.m20 = res2.x * detInv;
    res.m21 = res2.y * detInv;
    res.m22 = res2.z * detInv;
    res.m23 = res2.w * detInv;

    res.m30 = res3.x * detInv;
    res.m31 = res3.y * detInv;
    res.m32 = res3.z * detInv;
    res.m33 = res3.w * detInv;

    return res;
}

inline Vector3f Matrix4::translation() const
{
    return Vector3f(m30, m31, m32);
}

inline Vector3f Matrix4::rotationAsZYXEuler() const
{
    const auto& R = *this;

    // Heading
    double h = atan2(R.atRowCol(1, 0), R.atRowCol(0, 0));

    // Compute cos & sin of heading
    double ch = cos(h);
    double sh = sin(h);

    //Pitch
    double p = atan2(-R.atRowCol(2, 0), R.atRowCol(0, 0)*ch + R.atRowCol(1, 0)*sh);

    // Roll
    double r = atan2(R.atRowCol(0, 2)*sh - R.atRowCol(1, 2)*ch, -R.atRowCol(0, 1)*sh + R.atRowCol(1, 1)*ch);

    return Vector3f(float(r), float(p), float(h));
}

inline Matrix4 Matrix4::rotationOnly() const
{
    Matrix4 output = *this;

    output.m30 = output.m31 = output.m32 = 0;

    return output;
}

/// @endcond

//------------------------------------------------------------------------------

inline std::ostream& operator<< (std::ostream& o, const Matrix4& m)
{
    return o
        << m.m00 << " " << m.m10 << " " << m.m20 << " " << m.m30 << std::endl
        << m.m01 << " " << m.m11 << " " << m.m21 << " " << m.m31 << std::endl
        << m.m02 << " " << m.m12 << " " << m.m22 << " " << m.m32 << std::endl
        << m.m03 << " " << m.m13 << " " << m.m23 << " " << m.m33 << std::endl;
}

} // ST namespace
