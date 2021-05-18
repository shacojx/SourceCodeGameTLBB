#include "stdafx.h"

///////////////////////////////////////////////////////////////////////////////
// 文件名：ExplosionTrap01.cpp
// 程序员：高骐
// 功能说明：爆炸陷阱1
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "ExplosionTrap01.h"
#include "Obj_Special.h"
#include "ScanOpt_SObj_ScanChar.h"
#include "ScanOpt_SObj_ScanTrap.h"
#include "Logic_Util.h"
#include "DI_Damages.h"

using namespace Combat_Module::Impact_Module;

namespace Combat_Module
{
	namespace Special_Obj_Module
	{
		BOOL ExplosionTrap01_T::EffectOnChar(Obj_Special& rThis, Obj_Character& rTar) const
		{
			SpecialObjData_T const* pData = rThis.GetDataRecord();
			if(NULL==pData)
			{
				AssertEx(FALSE,"[ExplosionTrap01_T::EffectOnChar]: Can't find Data record.");				
				return FALSE;
			}
			SpecialObjData_T::Descriptor_T const* pDescriptor0 = pData->GetDescriptorByIndex(0);
			INT nDamagePhysics = 0;
			if(NULL != pDescriptor0)
			{
				nDamagePhysics = pDescriptor0->GetValue();
			}
			SpecialObjData_T::Descriptor_T const* pDescriptor1 = pData->GetDescriptorByIndex(1);
			INT nDamageMagic = 0;
			if(NULL != pDescriptor1)
			{
				nDamageMagic = pDescriptor1->GetValue();
			}
			SpecialObjData_T::Descriptor_T const* pDescriptor2 = pData->GetDescriptorByIndex(2);
			INT nDamageCold = 0;
			if(NULL != pDescriptor2)
			{
				nDamageCold = pDescriptor2->GetValue();
			}
			SpecialObjData_T::Descriptor_T const* pDescriptor3 = pData->GetDescriptorByIndex(3);
			INT nDamageFire = 0;
			if(NULL != pDescriptor3)
			{
				nDamageFire = pDescriptor3->GetValue();
			}
			SpecialObjData_T::Descriptor_T const* pDescriptor4 = pData->GetDescriptorByIndex(4);
			INT nDamageLight = 0;
			if(NULL != pDescriptor4)
			{
				nDamageLight = pDescriptor4->GetValue();
			}
			SpecialObjData_T::Descriptor_T const* pDescriptor5 = pData->GetDescriptorByIndex(5);
			INT nDamagePoison = 0;
			if(NULL != pDescriptor5)
			{
				nDamageLight = pDescriptor5->GetValue();
			}
			OWN_IMPACT impact;
			DI_Damages_T ImpactLogic;
			ImpactLogic.InitStruct(impact);
			impact.m_nCasterID = rThis.GetOwnerID();
			impact.m_nSkillID = Skill_Module::MELEE_ATTACK;
 			ImpactLogic.SetDamagePhy(impact, nDamagePhysics);
			ImpactLogic.SetDamageMagic(impact, nDamageMagic);
			ImpactLogic.SetDamageCold(impact, nDamageCold);
			ImpactLogic.SetDamageFire(impact, nDamageFire);
			ImpactLogic.SetDamageLight(impact, nDamageLight);
			ImpactLogic.SetDamagePoison(impact, nDamagePoison);
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
