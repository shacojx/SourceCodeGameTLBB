#ifndef __PS_Matrix3x3_H__
#define __PS_Matrix3x3_H__

#include "PS_Vector3.h"

class PS_Matrix3x3
{
public:

    PS_Scalar m[3][3];

    ////////////////////////////////////////////////////////
    // Special constants

    static const PS_Matrix3x3 ZERO;
    static const PS_Matrix3x3 IDENTITY;

    ////////////////////////////////////////////////////////
    // Constructors

    PS_Matrix3x3(void)
    {
    }

    PS_Matrix3x3(PS_Scalar m00, PS_Scalar m01, PS_Scalar m02,
                 PS_Scalar m10, PS_Scalar m11, PS_Scalar m12,
                 PS_Scalar m20, PS_Scalar m21, PS_Scalar m22)
    {
        m[0][0] = m00; m[0][1] = m01; m[0][2] = m02;
        m[1][0] = m10; m[1][1] = m11; m[1][2] = m12;
        m[2][0] = m20; m[2][1] = m21; m[2][2] = m22;
    }

    PS_Matrix3x3(const PS_Vector3& xAxis, const PS_Vector3& yAxis, const PS_Vector3& zAxis)
    {
        m[0][0] = xAxis.x; m[0][1] = yAxis.x; m[0][2] = zAxis.x;
        m[1][0] = xAxis.y; m[1][1] = yAxis.y; m[1][2] = zAxis.y;
        m[2][0] = xAxis.z; m[2][1] = yAxis.z; m[2][2] = zAxis.z;
    }

    PS_Vector3& operator[](size_t r)
    {
        assert(r < 3);
        return *reinterpret_cast<PS_Vector3*>(m[r]);
    }

    const PS_Vector3& operator[](size_t r) const
    {
        assert(r < 3);
        return *reinterpret_cast<const PS_Vector3*>(m[r]);
    }

    PS_Vector3 getColumn(size_t c) const
    {
        assert(c < 3);
        return PS_Vector3(m[0][c], m[1][c], m[2][c]);
    }

    void setColumn(size_t c, const PS_Vector3& v)
    {
        assert(c < 3);
        m[0][c] = v.x;
        m[1][c] = v.y;
        m[2][c] = v.z;
    }

    // matrix * vector [3x3 * 3x1 = 3x1]
    friend PS_Vector3 operator* (const PS_Matrix3x3& m, const PS_Vector3& v)
    {
        return PS_Vector3(m.m[0][0]*v.x + m.m[0][1]*v.y + m.m[0][2]*v.z,
                          m.m[1][0]*v.x + m.m[1][1]*v.y + m.m[1][2]*v.z,
                          m.m[2][0]*v.x + m.m[2][1]*v.y + m.m[2][2]*v.z);
    }

    // vector * matrix [1x3 * 3x3 = 1x3]
    friend PS_Vector3 operator* (const PS_Vector3& v, const PS_Matrix3x3& m)
    {
        return PS_Vector3(m.m[0][0]*v.x + m.m[1][0]*v.y + m.m[2][0]*v.z,
                          m.m[0][1]*v.x + m.m[1][1]*v.y + m.m[2][1]*v.z,
                          m.m[0][2]*v.x + m.m[1][2]*v.y + m.m[2][2]*v.z);
    }
};

#endif // __PS_Matrix3x3_H__
