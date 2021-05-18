///////////////////////////////////////////////////////////////////////////////
// 文件名：PetSkill001.h
// 功能说明：
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef PETSKILL001_H
#define PETSKILL001_H

#include "SkillLogic.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;
namespace Combat_Module
{
	namespace Skill_Module
	{
		typedef class PetSkill001_T : public SkillLogic_T
		{
			public:
				enum
				{
					ID = PET_SKILL_001,
				};
				class SkillInfoDescriptorIndex_T
				{
					public:
						enum
						{
							IDX_IMPACT_DATA_INDEX =0,
						};
				};

				// public interface
				PetSkill001_T(){};
				~PetSkill001_T(){};
				BOOL IsPassive(VOID) const {return FALSE;};
			protected: //interface for interval using 
				BOOL EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag=FALSE) const;
			private:
				
		} PetSkill_AIRefixImpacts_T;
	};
};
#endif //PETSKILL001_H

