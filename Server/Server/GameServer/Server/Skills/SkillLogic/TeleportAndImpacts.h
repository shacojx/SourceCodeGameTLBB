///////////////////////////////////////////////////////////////////////////////
// 文件名：TeleportAndImpacts.h
// 功能说明：
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef TELEPORTANDIMPACTS_H
#define TELEPORTANDIMPACTS_H

#include "SkillLogic.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;
namespace Combat_Module
{
	namespace Skill_Module
	{
		class TeleportAndImpacts_T : public SkillLogic_T
		{
			public:
				enum
				{
					ID = TELEPORT_AND_IMPACTS,
				};
				class SkillInfoDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_IMPACT_1 =0,
							IDX_IMPACT_2,
							IDX_IMPACT_3,
							IDX_IMPACT_TELEPORT
						};
				};
				// public interface
				TeleportAndImpacts_T(){};
				~TeleportAndImpacts_T(){};
				BOOL IsPassive(VOID) const {return FALSE;};
				// Accessor
				INT GetImpact1(SkillInfo_T const& rSkill) const;
				INT GetImpact2(SkillInfo_T const& rSkill) const;
				INT GetImpact3(SkillInfo_T const& rSkill) const;
				INT GetImpactTeleport(SkillInfo_T const& rSkill) const;
			protected: //interface for interval using 
				BOOL EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag=FALSE) const;
			private:				// public interface
		};
	};
};
#endif //TELEPORTANDIMPACTS_H

