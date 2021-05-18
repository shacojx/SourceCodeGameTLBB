/********************************************************************************
 *	文件名：	AbilityInlay.h
 *	全路径：	d:\Prj\Server\Server\Ability\AbilityInlay.h
 *	创建时间：	2006 年 3 月 23 日	15:27
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#ifndef __ABILITYINLAY_H__
#define __ABILITYINLAY_H__

#include "AbilityCompound.h"

class AbilityInlay : public AbilityCompound
{
	enum EmbedResult
	{
		EMBED_SUCCESS = 0,
		GEM_GONE,
		EQUIP_GONE,
		BOTH_GONE,
		ENERGY_NOT_ENOUGH,
	};

public:
	AbilityInlay(const Ability& a);

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

#endif // __ABILITYINLAY_H__
