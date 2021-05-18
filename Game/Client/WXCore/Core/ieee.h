#ifndef __ieee_h__
#define __ieee_h__

#include <float.h>
#include "inttypes.h"

typedef float  ieee32;
typedef double ieee64;

#ifndef __FORCEINLINE
#   ifndef _MSC_VER
#       define __FORCEINLINE   __forceinline
#   else
#       define __FORCEINLINE   inline
#   endif
#endif

// access float as int

__FORCEINLINE int32
as_int(const ieee32 &v)
{
	return *(int32*)&v;
}

__FORCEINLINE uint32
as_uint(const ieee32 &v)
{
	return *(uint32*)&v;
}

__FORCEINLINE ieee32
as_float(const uint32 &v)
{
	return *(ieee32*)&v;
}

__FORCEINLINE uint32
get_abs(const ieee32 &v)
{
	return *(uint32*)&v & 0x7FFFFFFF;
}

// type conversions complying to the ieee754 rounding rules

__FORCEINLINE static int
__internal_chop(int32 v)
{
	int32 sign = v >> 31;
	int32 bits = v & ((1u<<31)-1);
	int32 exponent = (bits >> (FLT_MANT_DIG-1)) - ((1<<(32-FLT_MANT_DIG-1))-1);
	int32 exponent_sign = ~(exponent >> 31);
	uint32 mantissa_bits = (v << (32-FLT_MANT_DIG)) | (~0u<<31);
	int32 mantissa = mantissa_bits >> (31 - exponent);
	return ((mantissa ^ sign) - sign) & exponent_sign;
}

__FORCEINLINE static int
__internal_floor(int32 v)
{
	int32 sign = v >> 31;
	int32 bits = v & ((1u<<31)-1);
	int32 exponent = (bits >> (FLT_MANT_DIG-1)) - ((1<<(32-FLT_MANT_DIG-1))-1);
	int32 exponent_sign = ~(exponent >> 31);
	uint32 mantissa_bits = (bits << (32-FLT_MANT_DIG)) | (~0u<<31);
	int32 mantissa = mantissa_bits >> (31 - exponent);
	int32 zero = ((bits - 1) >> 31) ^ exponent_sign;
	int32 carry = ((1 << (31 - exponent)) - 1) & mantissa_bits ? 0 : 1;
	return (carry & zero & sign) + ((mantissa & exponent_sign) ^ sign);
}

__FORCEINLINE int
fast_int(const ieee32 &v)
{
	return __internal_chop(*(int32*)&v);
}

__FORCEINLINE int
fast_ifloor(const ieee32 &v)
{
	return __internal_floor(*(int32*)&v);
}

__FORCEINLINE int
fast_iceil(const ieee32 &v)
{
	return - __internal_floor(*(int32*)&v ^ (1<<31));
}

#endif // __ieee_h__
