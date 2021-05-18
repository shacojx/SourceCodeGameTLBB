///////////////////////////////////////////////////////////////////////////////
// 文件名：ImpactsToTargetAndSelf.h
// 功能说明：基础技能-只产生(特定效果)， 1对1，不是AE
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef IMPACTSTOTARGETANDSELF_H
#define IMPACTSTOTARGETANDSELF_H

#include "Type.h"
#include "SkillLogic.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;
namespace Combat_Module
{
	namespace Skill_Module
	{
		class ImpactsToTargetAndSelf_T : public SkillLogic_T
		{
			public:
				enum
				{
					ID = IMPACTS_TO_TARGET_AND_SELF,
				};
				class SkillInfoDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_ACTIVATE_ONCE_IMPACT_FOR_SELF =0,
							IDX_ACTIVATE_ONCE_IMPACT_FOR_TARGET,
							IDX_ACTIVATE_EACH_TICK_IMPACT_FOR_SELF,
							IDX_ACTIVATE_EACH_TICK_IMPACT_FOR_TARGET,
						};
				};
				// public interface
				ImpactsToTargetAndSelf_T(){};
				~ImpactsToTargetAndSelf_T(){};
				BOOL IsPassive(VOID) const {return FALSE;};
				// Accessor
				INT GetActivateOnceImpactForSelf(SkillInfo_T const& rSkill) const;
				INT GetActivateEachTickImpactForSelf(SkillInfo_T const& rSkill) const;
				INT GetActivateOnceImpactForTarget(SkillInfo_T const& rSkill) const;
				INT GetActivateEachTickImpactForTarget(SkillInfo_T const& rSkill) const;
			protected: //interface for interval using 
				BOOL EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag=FALSE) const;
				BOOL EffectOnUnitEachTick(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag=FALSE) const;
			private:
		};
	};
};
#endif //IMPACTSTOTARGETANDSELF_H
