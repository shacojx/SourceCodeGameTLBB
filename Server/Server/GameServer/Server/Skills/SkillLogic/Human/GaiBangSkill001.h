///////////////////////////////////////////////////////////////////////////////
// 文件名：GaiBangSkill001.h
// 功能说明：丐帮技能见龙在田
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef GAIBANGSKILL001_H
#define GAIBANGSKILL001_H

#include "SkillLogic.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;
namespace Combat_Module
{
	namespace Skill_Module
	{
		class GaiBangSkill001_T : public SkillLogic_T
		{
			public:
				enum
				{
					ID = GAI_BANG_SKILL_001,
				};
				// public interface
				GaiBangSkill001_T(){};
				~GaiBangSkill001_T(){};
				BOOL IsPassive(VOID) const {return FALSE;};
			protected: //interface for interval using 
				BOOL EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag=FALSE) const;
			private:
		};
	};
};
#endif //GAIBANGSKILL001_H
