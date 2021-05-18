///////////////////////////////////////////////////////////////////////////////
// 文件名：CommonSkill002.h
// 功能说明：召唤宠物
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef COMMONSKILL002_H
#define COMMONSKILL002_H

#include "Type.h"
#include "SkillLogic.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;
namespace Combat_Module
{
	namespace Skill_Module
	{
		class CommonSkill002_T : public SkillLogic_T
		{
			public:
				enum
				{
					ID = COMMON_SKILL_002,
				};
				// public interface
				CommonSkill002_T(){};
				~CommonSkill002_T(){};
				BOOL IsPassive(VOID) const {return FALSE;};
			protected: //interface for interval using 
				BOOL EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag=FALSE) const;
			private:
		};
	};
};
#endif //COMMONSKILL002_H
