#ifndef _ABILITY_GATHER_FISH_H_
#define _ABILITY_GATHER_FISH_H_

#include "AbilityGather.h"

class AbilityGatherFish:public AbilityGather
{

public:
	AbilityGatherFish(Ability& a):AbilityGather(a)
	{
	};
	~AbilityGatherFish();

	//virtual	VOID				OnProcInterrupt(Obj_Human* pHuman){};

	virtual	ORESULT				CanUseAbility(Obj_Human* pHuman);

	virtual	ORESULT				OnProcOver(Obj_Human* pHuman);

protected:
	virtual	ORESULT					OnProcSuccess(Obj_Human* pHuman){return OR_OK;	}

	virtual	ORESULT					OnProcFailure(Obj_Human* pHuman){ return OR_OK; };

	
};




#endif