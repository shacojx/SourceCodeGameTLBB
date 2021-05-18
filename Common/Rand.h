

#ifndef __RAND_H__
#define __RAND_H__


#include "Type.h"

struct _RANDOM_TABLE
{
	enum	{	max_record_num = 100000	};
	UINT m_uCurPoint;
	UINT m_uRecord[max_record_num];

	_RANDOM_TABLE()
	{
		srand((unsigned)time(NULL));

		for(INT i = 0; i < max_record_num; i++)
			m_uRecord[i] = rand()*rand();

		m_uCurPoint = 0;	
	}
};

struct _100_PER_RANDOM_TABLE
{
	enum	{	max_record_num = 100	};

	UINT m_uRecord[max_record_num];
	UINT m_uCurPoint;

	_100_PER_RANDOM_TABLE()
	{
		srand((unsigned)time(NULL));

		for(INT i = 0; i < max_record_num; i++)
			m_uRecord[i] = i;

		Reset();
	}

	VOID Reset(){
		for(INT i = 0; i < 100; i++)
		{
			UINT uFrom = rand()%max_record_num;
			UINT uToIndex  = rand()%max_record_num;

			UINT uBuf = m_uRecord[uToIndex];
			m_uRecord[uToIndex] = m_uRecord[uFrom];
			m_uRecord[uFrom] =uBuf;
		}
		m_uCurPoint = 0;
	}

	UINT GetRand(){
		if(m_uCurPoint >= max_record_num)
			Reset();		

		return m_uRecord[m_uCurPoint++];
	}
	_100_PER_RANDOM_TABLE& operator=(_100_PER_RANDOM_TABLE const& rhs)
	{
		m_uCurPoint = rhs.m_uCurPoint;
		for( INT i = 0; i < max_record_num; i ++ )
		{
			m_uRecord[i] = rhs.m_uRecord[i];
		}
		return *this;
	}
};

class MyRand
{
public :
	MyRand( ) ;
	~MyRand( ) ;

	static VOID			SetRand( UINT uKey ) ;

	static UINT			Rand( ) ;

#define MAX_KEY_SIZE 100000
	static CHAR			m_pKey[MAX_KEY_SIZE] ;
	static UINT			m_uKey ;
	static MyLock		m_Lock ;
};

//随机数生成类

class RandGen
{
public:
	typedef unsigned long SeedType;
private:
	SeedType m_Seed[3];
	static const SeedType Max32BitLong = 0xFFFFFFFFLU;
public:
	static const SeedType RandomMax = Max32BitLong;

	RandGen(const SeedType p_Seed = 0)
	{
		Reset(p_Seed);
	}
	//ReSeed the random number generator
	//种子处理
	VOID Reset(const SeedType p_Seed = 0)
	{		
		m_Seed[0] = (p_Seed ^ 0xFEA09B9DLU) & 0xFFFFFFFELU;
		m_Seed[0] ^= (((m_Seed[0] << 7) & Max32BitLong) ^ m_Seed[0]) >> 31;

		m_Seed[1] = (p_Seed ^ 0x9C129511LU) & 0xFFFFFFF8LU;
		m_Seed[1] ^= (((m_Seed[1] << 2) & Max32BitLong) ^ m_Seed[1]) >> 29;

		m_Seed[2] = (p_Seed ^ 0x2512CFB8LU) & 0xFFFFFFF0LU;
		m_Seed[2] ^= (((m_Seed[2] << 9) & Max32BitLong) ^ m_Seed[2]) >> 28;

		RandUInt();
	}

	//Returns an unsigned integer from 0..RandomMax
	//0~RandMax uint 随机数
	unsigned long RandUInt(VOID)
	{
		m_Seed[0] = (((m_Seed[0] & 0xFFFFFFFELU) << 24) & Max32BitLong)
			^ ((m_Seed[0] ^ ((m_Seed[0] << 7) & Max32BitLong)) >> 7);

		m_Seed[1] = (((m_Seed[1] & 0xFFFFFFF8LU) << 7) & Max32BitLong)
			^ ((m_Seed[1] ^ ((m_Seed[1] << 2) & Max32BitLong)) >> 22);

		m_Seed[2] = (((m_Seed[2] & 0xFFFFFFF0LU) << 11) & Max32BitLong)
			^ ((m_Seed[2] ^ ((m_Seed[2] << 9) & Max32BitLong)) >> 17);

		return (m_Seed[0] ^ m_Seed[1] ^ m_Seed[2]);
	}
	//Returns a double in [0.0, 1.0]
	//返回0.0~1.0之间的双精度浮点
	double RandDouble(VOID)
	{
		return static_cast<double>(RandUInt())
			/ (static_cast<double>(RandomMax) );
	}
	
	static UINT		GetRand(INT nStart, INT nEnd);
		
};

extern	RandGen	g_RandGen;


#endif
