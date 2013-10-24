#ifndef UTIL_H
#define UTIL_H

#include <Angel.h>

/**
 * \brief Creates a matrix to rotate around an arbitrary axis
 *
 * \param[in] axis  - Axis of rotation
 * \param[in] theta - Degrees to rotate by
 *
 * \return Rotation matrix
 */
inline mat4 Rotate(const vec3& axis, float theta)
{
    mat4 ret; // identity

    float radians    = theta * DegreesToRadians;
    float cosine     = cos(radians);
    float cosineComp = 1 - cosine;
    float sine       = sin(radians);

    ret[0][0] = axis[0] * axis[0] + (1 - axis[0] * axis[0]) * cosine;
    ret[0][1] = cosineComp * axis[0] * axis[1] - axis[2] * sine;
    ret[0][2] = cosineComp * axis[0] * axis[2] + axis[1] * sine;
    ret[1][0] = cosineComp * axis[0] * axis[1] + axis[2] * sine;
    ret[1][1] = axis[1] * axis[1] + (1 - axis[1] * axis[1]) * cosine;
    ret[1][2] = cosineComp * axis[1] * axis[2] - axis[0] * sine;
    ret[2][0] = cosineComp * axis[0] * axis[2] - axis[1] * sine;
    ret[2][1] = cosineComp * axis[1] * axis[2] + axis[0] * sine;
    ret[2][2] = axis[2] * axis[2] + (1 - axis[2] * axis[2]) * cosine;

    return ret;
}

/**
 * \brief Gets the square of the length of a vector
 *
 * \param[in] v - Vector to get length of
 *
 * \return Length of v squared
 */
inline float LengthSquared(const vec2& v)
{
    return dot(v, v);
}

/**
 * \brief Gets the square of the length of a vector
 *
 * \param[in] v - Vector to get length of
 *
 * \return Length of v squared
 */
inline float LengthSquared(const vec3& v)
{
    return dot(v, v);
}

/**
 * \brief Gets the square of the length of a vector
 *
 * \param[in] v - Vector to get length of
 *
 * \return Length of v squared
 */
inline float LengthSquared(const vec4& v)
{
    return dot(v, v);
}

/**
 * \brief Transforms a vec3 by a mat4 using a w component of 1
 *
 * \param[in] matrix - Transformation matrix
 * \param[in] v      - Vector to transform
 *
 * \return Transformed vector
 */
inline vec3 Transform(const mat4& matrix, const vec3& v)
{
    vec4 temp = matrix * vec4(v, 1.0f);
    return vec3(temp.x, temp.y, temp.z) / temp.w;
}

/**
 * \brief Transforms a vec3 by a mat4 using a w component of 0
 *
 * \param[in] matrix - Transformation matrix
 * \param[in] v      - Vector to transform
 *
 * \return Transformed vector
 */
inline vec3 TransformNormal(const mat4& matrix, const vec3& v)
{
    vec4 temp = matrix * vec4(v, 0.0f);
    return vec3(temp.x, temp.y, temp.z);
}

#endif
