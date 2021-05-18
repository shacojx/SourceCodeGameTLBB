#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact051.cpp
// 功能说明：效果--分身护法的纪录效果
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "StdImpact051.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		BOOL StdImpact051_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetSummonerID(rImp, INVALID_ID);
			SetShadowGuardID(rImp, INVALID_ID);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact051_T::OnFadeOut(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			if(GetSummonerID(rImp)==rMe.GetID())
			{
				Obj_Character* pGuard = static_cast<Obj_Character*>(rMe.GetMyShadowGuard());
				if(NULL!=pGuard)
				{
					pGuard->GM_killObj();
				}
			}
			__LEAVE_FUNCTION
		}
		BOOL StdImpact051_T::SpecialHeartBeatCheck(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			if(GetSummonerID(rImp)==rMe.GetID())
			{
				Obj_Character* pGuard = static_cast<Obj_Character*>(rMe.GetMyShadowGuard());
				if(NULL!=pGuard)
				{
					if(TRUE==pGuard->IsAlive() && TRUE==pGuard->IsActiveObj())
					{
						return TRUE;
					}
				}
			}
			if(GetShadowGuardID(rImp)==rMe.GetID())
			{
				Obj_Character* pOwner = static_cast<Obj_Character*>(rMe.GetSpecificObjInSameSceneByID(GetSummonerID(rImp)));
				if(NULL!=pOwner)
				{
					if(pOwner->IsAlive() && TRUE==pOwner->IsActiveObj())
					{
						return TRUE;
					}
				}
			}
			return FALSE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};

