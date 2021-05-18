///////////////////////////////////////////////////////////////////////////////
// 文件名：CommonSkill001.h
// 功能说明：捉宠物
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef COMMONSKILL001_H
#define COMMONSKILL001_H

#include "Type.h"
#include "SkillLogic.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;
namespace Combat_Module
{
	namespace Skill_Module
	{
		class CommonSkill001_T : public SkillLogic_T
		{
			public:
				enum
				{
					ID = COMMON_SKILL_001,
				};
				// public interface
				CommonSkill001_T(){};
				~CommonSkill001_T(){};
				BOOL IsPassive(VOID) const {return FALSE;};
				BOOL SpecificOperationOnSkillStart(Obj_Character& rMe)const;
				BOOL OnInterrupt(Obj_Character& rMe) const;
				BOOL OnCancel(Obj_Character& rMe) const;

			protected: //interface for interval using 
				BOOL EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag=FALSE) const;
			private:
		};
	};
};
#endif //COMMONSKILL001_H
