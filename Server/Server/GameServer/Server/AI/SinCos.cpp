#include "stdafx.h"
#include "SinCos.h"

int SinCos::mTrigTableSize;
float  SinCos::mTrigTableFactor;
float *SinCos::mSinTable = 0;

SinCos g_SinCos;

SinCos::SinCos( unsigned int trigTableSize )
{
	mTrigTableSize = trigTableSize;
	mTrigTableFactor = mTrigTableSize / (2*__PI);

	mSinTable = new float[mTrigTableSize];

	buildTrigTables();
}

//-----------------------------------------------------------------------
SinCos::~SinCos()
{
	delete [] mSinTable;
}

//-----------------------------------------------------------------------
void SinCos::buildTrigTables(void)
{
	// Build trig lookup tables
	// Could get away with building only PI sized Sin table but simpler this 
	// way. Who cares, it'll ony use an extra 8k of memory anyway and I like 
	// simplicity.
	float angle;
	for (int i = 0; i < mTrigTableSize; ++i)
	{
		angle = 2*__PI * i / mTrigTableSize;
		mSinTable[i] = sin(angle);
	}
}
//-----------------------------------------------------------------------	
float SinCos::SinTable (float fValue)
{
	// Convert range to index values, wrap if required
	int idx;
	if (fValue >= 0)
	{
		idx = int(fValue * mTrigTableFactor) % mTrigTableSize;
	}
	else
	{
		idx = mTrigTableSize - (int(-fValue * mTrigTableFactor) % mTrigTableSize) - 1;
	}

	return mSinTable[idx];
}