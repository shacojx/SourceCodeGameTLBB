///////////////////////////////////////////////////////////////////////////////
// 文件名：CommonSkill003.h
// 功能说明：
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef COMMONSKILL003_H
#define COMMONSKILL003_H

#include "SkillLogic.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;
namespace Combat_Module
{
	namespace Skill_Module
	{
		class CommonSkill003_T : public SkillLogic_T
		{
			public:
				enum
				{
					ID = COMMON_SKILL_003,
				};
				// public interface
				CommonSkill003_T(){};
				~CommonSkill003_T(){};
				BOOL IsPassive(VOID) const {return FALSE;};
			protected: //interface for interval using 
				BOOL EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag=FALSE) const;
			private:
		};
	};
};
#endif //COMMONSKILL003_H

