#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact044.cpp
// 功能说明：效果--在一定时间内，增加效果所有者的移动速度，并且下一次成功打击目标后有一定几率增加3点连击点
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact044.h"
#include "Scene.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		VOID StdImpact044_T::MarkModifiedAttrDirty(OWN_IMPACT & rImp, Obj_Character & rMe) const
		{
			__ENTER_FUNCTION
			if(0!=GetMoveSpeedRefix(rImp))
			{
				rMe.MarkMoveSpeedRefixDirtyFlag();
			}
			__LEAVE_FUNCTION
		}
		BOOL StdImpact044_T::GetIntAttrRefix(OWN_IMPACT & rImp, Obj_Character& rMe, CharIntAttrRefixs_T::Index_T nIdx, INT & rIntAttrRefix) const
		{
			__ENTER_FUNCTION
			if(CharIntAttrRefixs_T::REFIX_MOVE_SPEED==nIdx)
			{
				if(0!=GetMoveSpeedRefix(rImp))
				{
					rIntAttrRefix += Float2Int((rMe.GetBaseMoveSpeed()*GetMoveSpeedRefix(rImp))/100.0f);
					return TRUE;
				}
			}
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact044_T::OnHitTarget(OWN_IMPACT & rImp, Obj_Character & rMe, Obj_Character & rTar) const
		{
			__ENTER_FUNCTION
			if(Obj::OBJ_TYPE_HUMAN==rMe.GetObjType())
			{
				if(0>=GetActivateOdds(rImp))
				{
					return;
				}
				Scene* pScene = rMe.getScene();
				if(NULL==pScene)
				{
					return;
				}
				INT nRand = pScene->GetRand100();
				if(nRand<=GetActivateOdds(rImp))
				{
					rMe.StrikePointIncrement(GetModifyStrikePoint(rImp));
				}
			}
			__LEAVE_FUNCTION
		}
	};
};

