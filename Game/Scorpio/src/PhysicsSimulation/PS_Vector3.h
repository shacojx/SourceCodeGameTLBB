#ifndef __PS_Vector3_H__
#define __PS_Vector3_H__

#include "PS_Math.h"

class PS_Vector3
{
public:

    union {
        struct {
            PS_Scalar x, y, z;
        };
        PS_Scalar val[3];
    };

    ////////////////////////////////////////////////////////
    // Special constants

    static const PS_Vector3 ZERO;
    static const PS_Vector3 UNIT_X;
    static const PS_Vector3 UNIT_Y;
    static const PS_Vector3 UNIT_Z;
    static const PS_Vector3 UNIT_SCALE;
    static const PS_Vector3 NEGATIVE_UNIT_X;
    static const PS_Vector3 NEGATIVE_UNIT_Y;
    static const PS_Vector3 NEGATIVE_UNIT_Z;
    static const PS_Vector3 NEGATIVE_UNIT_SCALE;

    ////////////////////////////////////////////////////////
    // Constructors

    PS_Vector3(void)
    {
    }

    PS_Vector3(PS_Scalar x, PS_Scalar y, PS_Scalar z)
        : x(x), y(y), z(z)
    {
    }

    explicit PS_Vector3(PS_Scalar s)
        : x(s), y(s), z(s)
    {
    }

    PS_Vector3(const PS_Vector3& other)
        : x(other.x), y(other.y), z(other.z)
    {
    }

    PS_Vector3& operator= (const PS_Vector3& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

    ////////////////////////////////////////////////////////
    // Accessors

    PS_Scalar operator[] (size_t i) const
    {
        assert(i < 3);
        return val[i];
    }

    PS_Scalar& operator[] (size_t i)
    {
        assert(i < 3);
        return val[i];
    }

    ////////////////////////////////////////////////////////

    const PS_Vector3& operator+ (void) const
    {
        return *this;
    }

    PS_Vector3 operator- (void) const
    {
        return PS_Vector3(-x, -y, -z);
    }

    ////////////////////////////////////////////////////////
    // Operators

    PS_Vector3& operator+= (const PS_Vector3& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }

    PS_Vector3& operator-= (const PS_Vector3& rhs)
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }

    PS_Vector3& operator*= (const PS_Vector3& rhs)
    {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        return *this;
    }

    PS_Vector3& operator/= (const PS_Vector3& rhs)
    {
        x /= rhs.x;
        y /= rhs.y;
        z /= rhs.z;
        return *this;
    }

    PS_Vector3& operator*= (PS_Scalar rhs)
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }

    PS_Vector3& operator/= (PS_Scalar rhs)
    {
        PS_Scalar inv = 1 / rhs;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }

    ////////////////////////////////////////////////////////

    friend PS_Vector3 operator+ (const PS_Vector3& lhs, const PS_Vector3& rhs)
    {
        return PS_Vector3(lhs.x + rhs.x,
                          lhs.y + rhs.y,
                          lhs.z + rhs.z);
    }

    friend PS_Vector3 operator- (const PS_Vector3& lhs, const PS_Vector3& rhs)
    {
        return PS_Vector3(lhs.x - rhs.x,
                          lhs.y - rhs.y,
                          lhs.z - rhs.z);
    }

    friend PS_Vector3 operator* (const PS_Vector3& lhs, const PS_Vector3& rhs)
    {
        return PS_Vector3(lhs.x * rhs.x,
                          lhs.y * rhs.y,
                          lhs.z * rhs.z);
    }

    friend PS_Vector3 operator/ (const PS_Vector3& lhs, const PS_Vector3& rhs)
    {
        return PS_Vector3(lhs.x / rhs.x,
                          lhs.y / rhs.y,
                          lhs.z / rhs.z);
    }

    friend PS_Vector3 operator* (const PS_Vector3& lhs, PS_Scalar rhs)
    {
        return PS_Vector3(lhs.x * rhs,
                          lhs.y * rhs,
                          lhs.z * rhs);
    }

    friend PS_Vector3 operator/ (const PS_Vector3& lhs, PS_Scalar rhs)
    {
        PS_Scalar inv = 1 / rhs;
        return PS_Vector3(lhs.x * inv,
                          lhs.y * inv,
                          lhs.z * inv);
    }

    friend PS_Vector3 operator* (PS_Scalar lhs, const PS_Vector3& rhs)
    {
        return PS_Vector3(lhs * rhs.x,
                          lhs * rhs.y,
                          lhs * rhs.z);
    }

    friend PS_Vector3 operator/ (PS_Scalar lhs, const PS_Vector3& rhs)
    {
        return PS_Vector3(lhs / rhs.x,
                          lhs / rhs.y,
                          lhs / rhs.z);
    }

    ////////////////////////////////////////////////////////
    // Functions

    /** Returns the square of the length(magnitude) of the vector.
        @remarks
            This  method is for efficiency - calculating the actual
            length of a vector requires a square root, which is expensive
            in terms of the operations required. This method returns the
            square of the length of the vector, i.e. the same as the
            length but before the square root is taken. Use this if you
            want to find the longest / shortest vector without incurring
            the square root.
    */
    PS_Scalar squaredLength(void) const
    {
        return x * x + y * y + z * z;
    }

    /** Returns the length (magnitude) of the vector.
        @warning
            This operation requires a square root and is expensive in
            terms of CPU operations. If you don't need to know the exact
            length (e.g. for just comparing lengths) use squaredLength()
            instead.
    */
    PS_Scalar length(void) const
    {
        return PS_Math::Sqrt(x * x + y * y + z * z);
    }

    /** Calculates the dot (scalar) product of this vector with another.
        @remarks
            The dot product can be used to calculate the angle between 2
            vectors. If both are unit vectors, the dot product is the
            cosine of the angle; otherwise the dot product must be
            divided by the product of the lengths of both vectors to get
            the cosine of the angle. This result can further be used to
            calculate the distance of a point from a plane.
        @param
            vec Vector with which to calculate the dot product (together
            with this one).
        @returns
            A float representing the dot product value.
    */
    PS_Scalar dotProduct(const PS_Vector3& vec) const
    {
        return x * vec.x + y * vec.y + z * vec.z;
    }

    /** Calculates the cross-product of 2 vectors, i.e. the vector that
        lies perpendicular to them both.
        @remarks
            The cross-product is normally used to calculate the normal
            vector of a plane, by calculating the cross-product of 2
            non-equivalent vectors which lie on the plane (e.g. 2 edges
            of a triangle).
        @param
            vec Vector which, together with this one, will be used to
            calculate the cross-product.
        @returns
            A vector which is the result of the cross-product. This
            vector will <b>NOT</b> be normalised, to maximise efficiency
            - call PS_Vector3::normalise on the result if you wish this to
            be done. As for which side the resultant vector will be on, the
            returned vector will be on the side from which the arc from 'this'
            to rkVector is anticlockwise, e.g. UNIT_Y.crossProduct(UNIT_Z)
            = UNIT_X, whilst UNIT_Z.crossProduct(UNIT_Y) = -UNIT_X.
			This is because OGRE uses a right-handed coordinate system.
        @par
            For a clearer explanation, look a the left and the bottom edges
            of your monitor's screen. Assume that the first vector is the
            left edge and the second vector is the bottom edge, both of
            them starting from the lower-left corner of the screen. The
            resulting vector is going to be perpendicular to both of them
            and will go <i>inside</i> the screen, towards the cathode tube
            (assuming you're using a CRT monitor, of course).
    */
    PS_Vector3 crossProduct(const PS_Vector3& rkVector) const
    {
        return PS_Vector3(
            y * rkVector.z - z * rkVector.y,
            z * rkVector.x - x * rkVector.z,
            x * rkVector.y - y * rkVector.x);
    }

    /** Normalises the vector.
        @remarks
            This method normalises the vector such that it's
            length / magnitude is 1. The result is called a unit vector.
        @note
            This function will not crash for zero-sized vectors, but there
            will be no changes made to their components.
        @returns The previous length of the vector.
    */
    PS_Scalar normalise(void)
    {
        PS_Scalar fLength = PS_Math::Sqrt(x * x + y * y + z * z);

        // Will also work for zero-sized vectors, but will change nothing
        if (fLength > PS_Scalar(1e-08f))
        {
            PS_Scalar fInvLength = PS_Scalar(1.0f) / fLength;
            x *= fInvLength;
            y *= fInvLength;
            z *= fInvLength;
        }

        return fLength;
    }
};

#endif // __PS_Vector3_H__
