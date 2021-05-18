#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact028.cpp
// 功能说明：
//
// 效果描述部分的格式：|效果ID|持续时间|冰属性修正|火属性修正|电属性修正|毒属性修正
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact028.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact028_T::OnDamages(OWN_IMPACT& rImp, Obj_Character& rMe, Obj_Character* const pAttacker, INT* const pDamageList, SkillID_t nSkillID) const
		{
			__ENTER_FUNCTION
			if(TRUE==rImp.IsFadeOut())
			{
				return;
			}
			INT nIncreaceHP = 0;
			INT nDamage = 0;
			INT nValue = 0;
			// 读出的比率<0则表示将该类型对应伤害的一定比率的数值转换成被击者的HP
			// 读出的比率>0则表示将该类型对应伤害的一定比率的数值叠加到施法者上
			
			if(0!=GetColdRefix(rImp))
			{
				nValue = Float2Int((pDamageList[DAMAGE_COLD] * GetColdRefix(rImp))/100.0f);
				if(0<=nValue)
				{
					pDamageList[DAMAGE_COLD] += nValue;
				}
				else
				{
					pDamageList[DAMAGE_COLD]=0;
					nIncreaceHP += -nValue;
				}
			}
			if(0!=GetFireRefix(rImp))
			{
				nValue = Float2Int((pDamageList[DAMAGE_FIRE] * GetFireRefix(rImp))/100.0f);
				if(0<=nValue)
				{
					pDamageList[DAMAGE_FIRE] += nValue;
				}
				else
				{
					pDamageList[DAMAGE_FIRE]=0;
					nIncreaceHP += -nValue;
				}
			}
			if(0!=GetLightRefix(rImp))
			{
				nValue = Float2Int((pDamageList[DAMAGE_LIGHT] * GetLightRefix(rImp))/100.0f);
				if(0<=nValue)
				{
					pDamageList[DAMAGE_LIGHT] += nValue;
				}
				else
				{
					pDamageList[DAMAGE_LIGHT]=0;
					nIncreaceHP += -nValue;
				}
			}
			if(0!=GetPoisonRefix(rImp))
			{
				nValue = Float2Int((pDamageList[DAMAGE_POISON] * GetPoisonRefix(rImp))/100.0f);
				if(0<=nValue)
				{
					pDamageList[DAMAGE_POISON] += nValue;
				}
				else
				{
					pDamageList[DAMAGE_POISON]=0;
					nIncreaceHP += -nValue;
				}
			}
			// 将转换来的HP加到自己身上
			if(0<nIncreaceHP)
			{
				rMe.HealthIncrement(nIncreaceHP, &rMe);
			}
			__LEAVE_FUNCTION
		}
	};
};

