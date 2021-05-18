#include "stdafx.h"

///////////////////////////////////////////////////////////////////////////////
// 文件名：ExplosionTrap02.cpp
// 程序员：高骐
// 功能说明：爆炸陷阱2
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "ExplosionTrap02.h"
#include "Obj_Special.h"
#include "ScanOpt_SObj_ScanChar.h"
#include "ScanOpt_SObj_ScanTrap.h"
#include "Logic_Util.h"
#include "DI_Damages.h"
#include "SOT_BodyEnhance.h"

using namespace Combat_Module::Impact_Module;

namespace Combat_Module
{
	namespace Special_Obj_Module
	{
		BOOL ExplosionTrap02_T::EffectOnChar(Obj_Special& rThis, Obj_Character& rTar) const
		{
			SpecialObjData_T const* pData = rThis.GetDataRecord();
			if(NULL==pData)
			{
				AssertEx(FALSE,"[ExplosionTrap02_T::EffectOnChar]: Can't find Data record.");				
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
			INT nBodyEnchaneImpactID = pData->GetDescriptorByIndex(6)->GetValue();
			INT nBodyEnchanceContinuance = pData->GetDescriptorByIndex(7)->GetValue();
			INT nActiveOdds = pData->GetDescriptorByIndex(8)->GetValue();
			INT nAttackRefixRate = pData->GetDescriptorByIndex(9)->GetValue();
			INT nDefenceRefixRate = pData->GetDescriptorByIndex(10)->GetValue();
			
			OWN_IMPACT impact1;
			DI_Damages_T impLogic1;
			impLogic1.InitStruct(impact1);
			impact1.m_nCasterID = rThis.GetOwnerID();
			impact1.m_nSkillID = Skill_Module::MELEE_ATTACK;
 			impLogic1.SetDamagePhy(impact1, nDamagePhysics);
			impLogic1.SetDamageMagic(impact1, nDamageMagic);
			impLogic1.SetDamageCold(impact1, nDamageCold);
			impLogic1.SetDamageFire(impact1, nDamageFire);
			impLogic1.SetDamageLight(impact1, nDamageLight);
			impLogic1.SetDamagePoison(impact1, nDamagePoison);

			
			Scene* pScene = rThis.getScene();
			if(NULL!=pScene)
			{
				pScene->GetEventCore().RegisterBeSkillEvent(rTar.GetID(), rThis.GetOwnerID(), BEHAVIOR_TYPE_HOSTILITY, 500);
				pScene->GetEventCore().RegisterImpactEvent(rTar.GetID(), rThis.GetOwnerID(), impact1, 500);
				INT nRand = pScene->GetRand100();
				if(nActiveOdds>nRand)
				{
					OWN_IMPACT impact2;
					SOT_BodyEnhance_T impLogic2;
					impLogic2.InitStruct(impact2);
					impact2.m_nCasterID = rThis.GetOwnerID();
					impact2.m_nSkillID = Skill_Module::MELEE_ATTACK;
					impact2.m_nImpactID = nBodyEnchaneImpactID;
					impLogic2.SetContinuance(impact2, nBodyEnchanceContinuance);
					INT nAttackRefix=0;
					INT nDefenceRefix=0;
					//Calculate Physics attack refix
					nAttackRefix = Float2Int((rTar.GetAttackPhysics() * nAttackRefixRate + 50) / 100);
					impLogic2.SetAttackPhysicsRefix(impact2, nAttackRefix);
					//Calculate Magic attack refix
					nAttackRefix = Float2Int((rTar.GetAttackMagic() * nAttackRefixRate + 50) / 100);
					impLogic2.SetAttackMagicRefix(impact2, nAttackRefix);
					//Calculate Physics Defence refix
					nDefenceRefix = Float2Int((rTar.GetDefencePhysics() * nDefenceRefixRate + 50) / 100);
					impLogic2.SetDefencePhysicsRefix(impact2, nDefenceRefix);
					//Calculate Magic Defence refix
					nDefenceRefix = Float2Int((rTar.GetDefenceMagic() * nDefenceRefixRate + 50) / 100);
					impLogic2.SetDefencePhysicsRefix(impact2, nDefenceRefix);
					//don't refix Max_Health
					impLogic2.SetMaxHealthRefix(impact2, 0);
					
					pScene->GetEventCore().RegisterImpactEvent(rTar.GetID(), rThis.GetOwnerID(), impact2, 500);
				}
			}
			return FALSE;
		}
	}
}
