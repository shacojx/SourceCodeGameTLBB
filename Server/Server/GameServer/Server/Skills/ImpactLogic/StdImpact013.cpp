#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact013.cpp
// 功能说明：效果--在一定时间内，改变效果所有者的命中闪避和会心
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact013.h"
#include "Obj_Character.h"
#include "GameStruct_Impact.h"
#include "SkillLogic.h"
#include "Scene.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		BOOL StdImpact013_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetRefixRate(rImp, 0);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact013_T::MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const
		{
			__ENTER_FUNCTION
			if(0!=GetHitRefix(rImp))
			{
				rMe.MarkHitRefixDirtyFlag();
			}
			if(0!=GetMissRefix(rImp))
			{
				rMe.MarkMissRefixDirtyFlag();
			}
			if(0!=GetCriticalRefix(rImp))
			{
				rMe.MarkCriticalRefixDirtyFlag();
			}
			__LEAVE_FUNCTION
		}
		BOOL StdImpact013_T::GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const
		{
			__ENTER_FUNCTION
			INT nRefixRate = GetRefixRate(rImp);
			nRefixRate += 100;
			INT nValue = 0;
			switch (nIdx)
			{
				case CharIntAttrRefixs_T::REFIX_HIT:
					{
						if(0!=GetHitRefix(rImp))
						{
							nValue = Float2Int((GetHitRefix(rImp)*nRefixRate)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_MISS:
					{
						if(0!=GetMissRefix(rImp))
						{
							nValue = Float2Int((GetMissRefix(rImp)*nRefixRate)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;
				case CharIntAttrRefixs_T::REFIX_CRITICAL:
					{
						if(0!=GetCriticalRefix(rImp))
						{
							nValue = Float2Int((GetCriticalRefix(rImp)*nRefixRate)/100.0f);
							rIntAttrRefix += nValue;
							return TRUE;
						}
					}
					break;				
				default:
					break;
			}
			__LEAVE_FUNCTION
			return FALSE;
		}
		BOOL StdImpact013_T::RefixPowerByRate(OWN_IMPACT & rImp, INT nRate) const
		{
			__ENTER_FUNCTION
			nRate += GetRefixRate(rImp);
			SetRefixRate(rImp, nRate);
			__LEAVE_FUNCTION
			return TRUE;
		}
	};
};

