#ifndef _ABILITY_GATHER_H_
#define _ABILITY_GATHER_H_

#include "Ability.h"

class AbilityGather : public Ability
{
public:
	
	
	AbilityGather(Ability& a);
	
	virtual
		ORESULT					CanUseAbility(Obj_Human* pHuman);

	virtual
		VOID					OnProcInterrupt(Obj_Human* pHuman) { Ability::OnProcInterrupt(pHuman); }

	virtual	ORESULT				OnProcOver(Obj_Human* pHuman);

protected:
	
	virtual
		ORESULT					OnProcSuccess(Obj_Human* pHuman);

	virtual
		ORESULT					OnProcFailure(Obj_Human* pHuman){ return OR_OK; };

	VOID						DefaultProcSuccess(Obj_Human* pHuman);
	VOID						ScriptProcSuccess(Obj_Human* pHuman);

};



#endif