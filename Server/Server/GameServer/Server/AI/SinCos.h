#ifndef __Sin_Cos_H__
#define __Sin_Cos_H__

#include "Type.h"
#include "GameUtil.h"

class SinCos
{
protected:
	/// Size of the trig tables as determined by constructor.
	static int mTrigTableSize;

	/// Radian -> index factor value ( mTrigTableSize / 2 * PI )
	static float mTrigTableFactor;
	static float* mSinTable;

	/** Private function to build trig tables.
	*/
	void buildTrigTables();

	static float SinTable (float fValue);


public:	
	static inline float Cos (const float& fValue) {
		return SinTable(fValue + __HALF_PI);
	}

	static inline float Sin (const float& fValue) {
		return SinTable(fValue);
	}

	/** Default constructor.
	@param
	trigTableSize Optional parameter to set the size of the
	tables used to implement Sin, Cos, Tan
	*/
	SinCos(unsigned int trigTableSize = 4096);

	/** Default destructor.
	*/
	~SinCos();
};

extern SinCos g_SinCos;

#endif
