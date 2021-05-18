// AbilityMachining.h

#ifndef __ABILITYMACHINING_H__
#define __ABILITYMACHINING_H__

#include "Ability.h"
#include "AbilityCompound.h"

class AbilityMachining : public AbilityCompound
{
	
public:
	enum MachiningType
	{
		NORMAL_TYPE = 0,		// 普通加工
		GEM_COMPOUND,			// 宝石合成
		GEM_EMBED,				// 宝石镶嵌
		TYPE_SIZE,				// 加工类型数量
	};

public:
	AbilityMachining();
	AbilityMachining(const Ability& a);
	~AbilityMachining();

	virtual
		ORESULT					CanUseAbility(Obj_Human* pHuman);

	virtual
		ORESULT					OnProcOver(Obj_Human* pHuman);

	BOOL						IsGemConflict(Obj_Human* pHuman, BYTE GemIndex, BYTE EquipIndex);

protected:
	virtual
		ORESULT					OnProcSuccess(Obj_Human* pHuman, const _PRESCRIPTION_DATA* pPrescrData);

private:
	ORESULT						OnProcSuccess(Obj_Human* pHuman);
};

#endif // __ABILITYMACHINING_H__