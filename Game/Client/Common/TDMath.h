#pragma once

/// 向量相关
struct fVector2
{
public:
	inline fVector2& operator = ( const fVector2& rkVector )
	{
		x = rkVector.x;
		y = rkVector.y;
		return *this;
	}

	inline BOOL operator == ( const fVector2& rkVector ) const
	{
		return ( x == rkVector.x && y == rkVector.y );
	}

	inline BOOL operator != ( const fVector2& rkVector ) const
	{
		return ( x != rkVector.x || y != rkVector.y );
	}

	inline fVector2 operator + ( const fVector2& rkVector ) const
	{
		fVector2 kSum;

		kSum.x = x + rkVector.x;
		kSum.y = y + rkVector.y;

		return kSum;
	}

	inline fVector2 operator - ( const fVector2& rkVector ) const
	{
		fVector2 kDiff;

		kDiff.x = x - rkVector.x;
		kDiff.y = y - rkVector.y;

		return kDiff;
	}

	inline fVector2 operator * ( float fScalar ) const
	{
		fVector2 kProd;

		kProd.x = fScalar*x;
		kProd.y = fScalar*y;

		return kProd;
	}

	inline friend fVector2 operator * ( float fScalar, const fVector2& rkVector )
	{
		fVector2 kProd;

		kProd.x = fScalar * rkVector.x;
		kProd.y = fScalar * rkVector.y;

		return kProd;
	}


	inline FLOAT length (VOID) const;

	FLOAT normalise(FLOAT fAimLength=1.0f);
public:
	fVector2()				 : x(0.0f), y(0.0f) { }
	fVector2(FLOAT _x, FLOAT _y) : x(_x), y(_y)		{ }
public:
	FLOAT x; FLOAT y;
};

struct iVector2
{
public:
	iVector2()				 : x(0), y(0)		{ }
	iVector2(INT _x, INT _y) : x(_x), y(_y)		{ }
public:
	INT x; INT y;
};

struct  fVector3 
{
public:
	inline fVector3& operator = ( const fVector3& rkVector )
	{
		x = rkVector.x;
		y = rkVector.y;
		z = rkVector.z;            
		return *this;
	}

	inline BOOL operator == ( const fVector3& rkVector ) const
	{
		return ( x == rkVector.x && y == rkVector.y && z == rkVector.z );
	}

	inline BOOL operator != ( const fVector3& rkVector ) const
	{
		return ( x != rkVector.x || y != rkVector.y || z != rkVector.z );
	}

	inline fVector3 operator + ( const fVector3& rkVector ) const
	{
		fVector3 kSum;

		kSum.x = x + rkVector.x;
		kSum.y = y + rkVector.y;
		kSum.z = z + rkVector.z;

		return kSum;
	}

	inline fVector3 operator - ( const fVector3& rkVector ) const
	{
		fVector3 kDiff;

		kDiff.x = x - rkVector.x;
		kDiff.y = y - rkVector.y;
		kDiff.z = z - rkVector.z;

		return kDiff;
	}

	inline fVector3 operator * ( const FLOAT& rkMult ) const
	{
		fVector3 kMult;

		kMult.x = x	* rkMult;
		kMult.y = y	* rkMult;
		kMult.z = z	* rkMult;

		return kMult;
	}

	inline FLOAT length (VOID) const;

	FLOAT normalise(FLOAT fAimLength=1.0f);

public:
	fVector3()						 : x(0.0f), y(0.0f), z(0.0f) { }
	fVector3(FLOAT _x, FLOAT _y, FLOAT _z) : x(_x), y(_y), z(_z)	 	 { }
public:
	FLOAT x; FLOAT y; FLOAT z;
};

struct iVector3 
{
public:
	iVector3()						 : x(0), y(0), z(0)		{ }
	iVector3(INT _x, INT _y, INT _z) : x(_x), y(_y), z(_z)	{ }
public:
	INT x; INT y; INT z;
};


struct fRay
{
public:
	fVector3 mOrigin;
    fVector3 mDirection;
};
