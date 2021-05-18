#include "stdafx.h"

///////////////////////////////////////////////////////////////////////////////
// 文件名：RootTrap01.cpp
// 程序员：高骐
// 功能说明：
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "RootTrap01.h"
#include "Obj_Special.h"
#include "ScanOpt_SObj_ScanChar.h"
#include "ScanOpt_SObj_ScanTrap.h"
#include "Logic_Util.h"
#include "SOT_Root.h"

using namespace Combat_Module::Impact_Module;

namespace Combat_Module
{
	namespace Special_Obj_Module
	{
		BOOL RootTrap01_T::EffectOnChar(Obj_Special& rThis, Obj_Character& rTar) const
		{
			SpecialObjData_T const* pData = rThis.GetDataRecord();
			if(NULL==pData)
			{
				AssertEx(FALSE,"[RootTrap01_T::EffectOnChar]: Can't find Data record.");				
				return FALSE;
			}
			INT nImpactID = pData->GetDescriptorByIndex(0)->GetValue();
			Time_t nContinuance = pData->GetDescriptorByIndex(1)->GetValue();
			OWN_IMPACT impact;
			SOT_Root_T ImpactLogic;
			ImpactLogic.InitStruct(impact);
			impact.m_nCasterID = rThis.GetOwnerID();
			impact.m_nSkillID = INVALID_ID;
			impact.m_nImpactID = nImpactID;
			ImpactLogic.SetContinuance(impact, nContinuance);
			impact.m_nPRI = rThis.GetXinFaLevelForBuffMutex();
			
			Scene* pScene = rThis.getScene();
			if(NULL!=pScene)
			{
				pScene->GetEventCore().RegisterBeSkillEvent(rTar.GetID(), rThis.GetOwnerID(), BEHAVIOR_TYPE_HOSTILITY, 500);
				pScene->GetEventCore().RegisterImpactEvent(rTar.GetID(), rThis.GetOwnerID(), impact, 500);
			}
			return FALSE;
		}
	}
}
