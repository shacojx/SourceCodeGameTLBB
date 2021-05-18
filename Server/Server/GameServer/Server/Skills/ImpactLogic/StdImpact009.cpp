#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：StdImpact009.cpp
// 功能说明：效果--传送角色到指定位置
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////
#include "StdImpact009.h"
#include "Obj_Human.h"
#include "Scene.h"

namespace Combat_Module
{
	using namespace Combat_Module::Skill_Module;

	namespace Impact_Module
	{
		BOOL StdImpact009_T::InitFromData(OWN_IMPACT& rImp, ImpactData_T const& rData) const
		{
			__ENTER_FUNCTION
			SetSceneID(rImp, Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_SCENE_ID));
			SetPosition_X(rImp, Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_POSITION_X)+0.f);
			SetPosition_Z(rImp, Impact_GetImpactDataDescriptorValueByIndex(rImp.GetDataIndex(), ImpactDataDescriptorIndex_T::IDX_POSITION_Z)+0.f);
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
		VOID StdImpact009_T::OnActive(OWN_IMPACT& rImp, Obj_Character& rMe) const
		{
			__ENTER_FUNCTION
			Scene* pScene = rMe.getScene();
			if(NULL==pScene)
			{
				return;
			}
			//Teleport here
			WORLD_POS pos;
			SceneID_t nScene = GetSceneID(rImp);
			pos.m_fX = GetPosition_X(rImp)/100.0f;
			pos.m_fZ = GetPosition_Z(rImp)/100.0f;
			if(INVALID_ID==nScene || pScene->SceneID() == nScene)
			{
				Obj& rObj = static_cast<Obj&>(rMe);
				rObj.Teleport(&pos);
			}
			else
			{
				if(Obj::OBJ_TYPE_HUMAN==rMe.GetObjType())
				{
					Obj_Human& rHuman = static_cast<Obj_Human&>(rMe);
					rHuman.ChangeScene(pScene->SceneID(), nScene, pos, 9);
				}
			}
			__LEAVE_FUNCTION
		}
	};
};

