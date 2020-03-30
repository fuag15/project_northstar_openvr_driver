/*
    C_MathTypes.h

    Copyright © 2019 Occipital, Inc. All rights reserved.
    This file is part of the Bridge Engine SDK.
    Unauthorized copying of this file, via any medium is strictly prohibited.
    Proprietary and confidential.

    http://structure.io
*/

/** @brief C_MathTypes.h contains overloaded C# bindings for the math types used in BridgeEngine.
    @see MathTypes.h
*/

#pragma once

#include <ST/Macros.h>

#include <math.h>
#include <limits.h>

ST_PLAIN_C_BEGIN

//------------------------------------------------------------------------------

ST_API bool ST_CDECL ST_epsilonEquals          (float first, float second);
ST_API bool ST_CDECL ST_epsilonEqualsOrBothNan (float first, float second);

//------------------------------------------------------------------------------

struct ST_API ST_Vector2f
{
    float v[2];
};

ST_API ST_Vector2f ST_CDECL ST_Vector2f_lerp                   (ST_Vector2f from, ST_Vector2f to, float t);
ST_API bool        ST_CDECL ST_Vector2f_epsilonEquals          (ST_Vector2f first, ST_Vector2f second);
ST_API bool        ST_CDECL ST_Vector2f_epsilonEqualsOrBothNan (ST_Vector2f first, ST_Vector2f second);

//------------------------------------------------------------------------------

struct ST_API ST_Vector3f
{
    float v[3];
};

ST_API ST_Vector3f ST_CDECL ST_Vector3f_neg                    (ST_Vector3f v);
ST_API ST_Vector3f ST_CDECL ST_Vector3f_sub                    (ST_Vector3f lhs, ST_Vector3f rhs);
ST_API ST_Vector3f ST_CDECL ST_Vector3f_add                    (ST_Vector3f lhs, ST_Vector3f rhs);
ST_API ST_Vector3f ST_CDECL ST_Vector3f_scale                  (ST_Vector3f v, float f);
ST_API float       ST_CDECL ST_Vector3f_length                 (ST_Vector3f v);
ST_API ST_Vector3f ST_CDECL ST_Vector3f_normalized             (ST_Vector3f v);
ST_API float       ST_CDECL ST_Vector3f_dot                    (ST_Vector3f lhs, ST_Vector3f rhs);
ST_API ST_Vector3f ST_CDECL ST_Vector3f_cross                  (ST_Vector3f lhs, ST_Vector3f rhs);
ST_API bool        ST_CDECL ST_Vector3f_epsilonEquals          (ST_Vector3f first, ST_Vector3f second);
ST_API bool        ST_CDECL ST_Vector3f_epsilonEqualsOrBothNan (ST_Vector3f first, ST_Vector3f second);

//------------------------------------------------------------------------------

struct ST_API ST_ALIGNED(16) ST_Vector4f
{
    float v[4];
};

ST_API bool ST_CDECL ST_Vector4f_epsilonEquals          (ST_Vector4f first, ST_Vector4f second);
ST_API bool ST_CDECL ST_Vector4f_epsilonEqualsOrBothNan (ST_Vector4f first, ST_Vector4f second);

//------------------------------------------------------------------------------

enum ST_Handedness
{
    ST_Handedness_Left,
    ST_Handedness_Right
};

//------------------------------------------------------------------------------

struct ST_API ST_ALIGNED(16) ST_Matrix4
{
    float m[16];
};

ST_API void        ST_CDECL ST_Matrix4_setNan               (ST_Matrix4* out);
ST_API void        ST_CDECL ST_Matrix4_setIdentity          (ST_Matrix4* out);
ST_API void        ST_CDECL ST_Matrix4_setLookAt            (ST_Vector3f eyePosition, ST_Vector3f lookAt, ST_Vector3f up, ST_Matrix4* out);
ST_API void        ST_CDECL ST_Matrix4_setProjection        (float fovY, float aspectRatio, float minZ, float maxZ, ST_Handedness handedness, ST_Matrix4* out);
ST_API void        ST_CDECL ST_Matrix4_setRowCol            (      ST_Matrix4* m, int row, int col, float val);
ST_API float       ST_CDECL ST_Matrix4_getRowCol            (const ST_Matrix4* m, int row, int col);
ST_API void        ST_CDECL ST_Matrix4_mat4_mul             (const ST_Matrix4* lhs, const ST_Matrix4*  rhs, ST_Matrix4* out);
ST_API ST_Vector4f ST_CDECL ST_Matrix4_vec4_mul             (const ST_Matrix4* lhs, ST_Vector4f rhs);
ST_API ST_Vector3f ST_CDECL ST_Matrix4_vec3_mul             (const ST_Matrix4* lhs, ST_Vector3f rhs);
ST_API void        ST_CDECL ST_Matrix4_transposed           (const ST_Matrix4* m, ST_Matrix4* out);
ST_API void        ST_CDECL ST_Matrix4_inversed             (const ST_Matrix4* m, ST_Matrix4* out);
ST_API bool        ST_CDECL ST_Matrix4_isZero               (const ST_Matrix4* m);
ST_API bool        ST_CDECL ST_Matrix4_hasNan               (const ST_Matrix4* m);
ST_API bool        ST_CDECL ST_Matrix4_isApprox             (const ST_Matrix4* first, const ST_Matrix4* second);
ST_API void        ST_CDECL ST_Matrix4_scaled               (const ST_Matrix4* m, float scaleX, float scaleY, float scaleZ, ST_Matrix4* out);
ST_API void        ST_CDECL ST_Matrix4_translated           (const ST_Matrix4* m, float tX, float tY, float tZ, ST_Matrix4* out);
ST_API void        ST_CDECL ST_Matrix4_rotated_quat         (const ST_Matrix4* m, ST_Vector4f quaternion, ST_Matrix4* out);
ST_API void        ST_CDECL ST_Matrix4_rotated_euler        (const ST_Matrix4* m, ST_Vector3f eulerAnglesRad, ST_Matrix4* out);
ST_API ST_Vector3f ST_CDECL ST_Matrix4_translation          (const ST_Matrix4* m);
ST_API ST_Vector4f ST_CDECL ST_Matrix4_rotationAsQuaternion (const ST_Matrix4* m);
ST_API ST_Vector3f ST_CDECL ST_Matrix4_rotationAsZYXEuler   (const ST_Matrix4* m);
ST_API void        ST_CDECL ST_Matrix4_rotationOnly         (const ST_Matrix4* m, ST_Matrix4* out);

//------------------------------------------------------------------------------


ST_PLAIN_C_END
