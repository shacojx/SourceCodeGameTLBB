#include "PS_Math.h"

const PS_Scalar PS_Math::POS_INFINITY = +std::numeric_limits<PS_Scalar>::infinity();
const PS_Scalar PS_Math::NEG_INFINITY = -std::numeric_limits<PS_Scalar>::infinity();
const PS_Scalar PS_Math::PI = PS_Scalar( 4.0f * atan( 1.0f ) );
const PS_Scalar PS_Math::TWO_PI = PS_Scalar( 2.0f * PI );
const PS_Scalar PS_Math::HALF_PI = PS_Scalar( 0.5f * PI );
const PS_Scalar PS_Math::SQUARE_PI = PS_Scalar(PI * PI);
const PS_Scalar PS_Math::SQUARE_ROOT_TWO = Sqrt(2.0f);
const PS_Scalar PS_Math::fDeg2Rad = PI / PS_Scalar(180.0f);
const PS_Scalar PS_Math::fRad2Deg = PS_Scalar(180.0f) / PI;
