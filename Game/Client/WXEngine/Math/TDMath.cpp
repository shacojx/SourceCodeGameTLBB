#include "StdAfx.h"
#include "TDBasicType.h"
#include "TDMath.h"
#include <math.h>

inline FLOAT fVector2::length (VOID) const
{
	return sqrt( x * x + y * y );
}

FLOAT fVector2::normalise(FLOAT fAimLength)
{
	FLOAT fLength = sqrt( x * x + y * y );

	// Will also work for zero-sized vectors, but will change nothing
	if ( fLength > 1e-08 )
	{
		FLOAT fInvLength = fAimLength / fLength;
		x *= fInvLength;
		y *= fInvLength;
	}

	return fLength;
}
//=============================================================
inline FLOAT fVector3::length (VOID) const
{
	return sqrt( x * x + y * y + z * z );
}

FLOAT fVector3::normalise(FLOAT fAimLength)
{
	FLOAT fLength = sqrt( x * x + y * y + z * z );

	// Will also work for zero-sized vectors, but will change nothing
	if ( fLength > 1e-08 )
	{
		FLOAT fInvLength = fAimLength / fLength;
		x *= fInvLength;
		y *= fInvLength;
		z *= fInvLength;
	}

	return fLength;
}