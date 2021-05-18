#ifndef __PS_Math_H__
#define __PS_Math_H__

#include "PS_Prerequisites.h"

/** Class to provide access to common mathematical functions.
    @remarks
        Most of the maths functions are aliased versions of the C runtime
        library functions. They are aliased here to provide future
        optimisation opportunities, either from faster RTLs or custom
        math approximations.
    @note
        <br>This is based on MgcMath.h from
        <a href="http://www.geometrictools.com/">Wild Magic</a>.
*/
class PS_Math
{
public:
    ////////////////////////////////////////////////////////
    // Special constants

    static const PS_Scalar POS_INFINITY;
    static const PS_Scalar NEG_INFINITY;
    static const PS_Scalar PI;
    static const PS_Scalar TWO_PI;
    static const PS_Scalar HALF_PI;
    static const PS_Scalar SQUARE_PI;
    static const PS_Scalar SQUARE_ROOT_TWO;
    static const PS_Scalar fDeg2Rad;
    static const PS_Scalar fRad2Deg;

    static PS_Scalar Abs(PS_Scalar val)
    {
        return fabs(val);
    }

    static PS_Scalar Sqr(PS_Scalar val)
    {
        return val * val;
    }

    static PS_Scalar Sqrt(PS_Scalar val)
    {
        return sqrt(val);
    }

    static PS_Scalar Sin(PS_Scalar radian)
    {
        return sin(radian);
    }

    static PS_Scalar Cos(PS_Scalar radian)
    {
        return cos(radian);
    }

    static PS_Scalar ASin(PS_Scalar val)
    {
        return asin(val);
    }

    static PS_Scalar ACos(PS_Scalar val)
    {
        return acos(val);
    }

    static PS_Scalar Sinc(PS_Scalar val)
    {
        return val ? Sin(val) / val : 1;
    }

    static PS_Scalar ASinc(PS_Scalar val)
    {
        // Don't know how to implement asinc, use acos instead of, work for [0, 1] only
        return acos(val * 2 - 1);
    }
};

#endif // __PS_Math_H__
