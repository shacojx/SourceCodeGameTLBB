///////////////////////////////////////////////////////////////////////////////
// 文件名：EMeiSkill001.h
// 功能说明：峨嵋-太阿倒持
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef EMEISKILL001_H
#define EMEISKILL001_H

#include "SkillLogic.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;
namespace Combat_Module
{
	namespace Skill_Module
	{
		class EMeiSkill001_T : public SkillLogic_T
		{
			public:
				enum
				{
					ID = E_MEI_SKILL_001,
				};
				// public interface
				EMeiSkill001_T(){};
				~EMeiSkill001_T(){};
				BOOL IsPassive(VOID) const {return FALSE;};
			protected: //interface for interval using 
				BOOL EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag=FALSE) const;
			private:
		};
	};
};
#endif //EMEISKILL001_H
