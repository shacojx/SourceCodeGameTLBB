#include "stdafx.h"

///////////////////////////////////////////////////////////////////////////////
// 文件名：ExplosionTrap03.cpp
// 程序员：高骐
// 功能说明：爆炸陷阱3
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "ExplosionTrap03.h"
#include "Obj_Special.h"
#include "ScanOpt_SObj_ScanChar.h"
#include "ScanOpt_SObj_ScanTrap.h"
#include "Logic_Util.h"
#include "DI_Damages.h"
#include "SOT_MoveSpeedRefix.h"

using namespace Combat_Module::Impact_Module;

namespace Combat_Module
{
	namespace Special_Obj_Module
	{
		BOOL ExplosionTrap03_T::EffectOnChar(Obj_Special& rThis, Obj_Character& rTar) const
		{
			SpecialObjData_T const* pData = rThis.GetDataRecord();
			if(NULL==pData)
			{
				AssertEx(FALSE,"[ExplosionTrap03_T::EffectOnChar]: Can't find Data record.");				
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
			INT nImpactID = pData->GetDescriptorByIndex(6)->GetValue();
			INT nContinuance = pData->GetDescriptorByIndex(7)->GetValue();
			INT nMoveSpeedRefixRate = pData->GetDescriptorByIndex(8)->GetValue();
			
			OWN_IMPACT impact1;
			DI_Damages_T logic1;
			logic1.InitStruct(impact1);
			impact1.m_nCasterID = rThis.GetOwnerID();
			impact1.m_nSkillID = Skill_Module::MELEE_ATTACK;
 			logic1.SetDamagePhy(impact1, nDamagePhysics);
			logic1.SetDamageMagic(impact1, nDamageMagic);
			logic1.SetDamageCold(impact1, nDamageCold);
			logic1.SetDamageFire(impact1, nDamageFire);
			logic1.SetDamageLight(impact1, nDamageLight);
			logic1.SetDamagePoison(impact1, nDamagePoison);

			OWN_IMPACT impact2;
			SOT_MoveSpeedRefix_T logic2;
			logic2.InitStruct(impact2);
			impact2.m_nCasterID = rThis.GetOwnerID();
			impact2.m_nSkillID = Skill_Module::MELEE_ATTACK;
			impact2.m_nImpactID = nImpactID;
			logic2.SetContinuance(impact2, nContinuance);
			logic2.SetMoveSpeedRefix(impact2, nMoveSpeedRefixRate);
			impact2.m_nPRI = rThis.GetXinFaLevelForBuffMutex();
			
			Scene* pScene = rThis.getScene();
			if(NULL!=pScene)
			{
				pScene->GetEventCore().RegisterBeSkillEvent(rTar.GetID(), rThis.GetOwnerID(), BEHAVIOR_TYPE_HOSTILITY, 500);
				pScene->GetEventCore().RegisterImpactEvent(rTar.GetID(), rThis.GetOwnerID(), impact1, 500);
				pScene->GetEventCore().RegisterImpactEvent(rTar.GetID(), rThis.GetOwnerID(), impact2, 500);
			}
			return FALSE;
		}
	}
}
