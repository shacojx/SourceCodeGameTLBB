///////////////////////////////////////////////////////////////////////////////
// 文件名：XiaoYaoSkill001.h
// 功能说明：
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef XiaoYaoSKILL001_H
#define XiaoYaoSKILL001_H

#include "SkillLogic.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;
namespace Combat_Module
{
	namespace Skill_Module
	{
		class XiaoYaoSkill001_T : public SkillLogic_T
		{
			public:
				enum
				{
					ID = XIAO_YAO_SKILL_001,
				};
				// public interface
				XiaoYaoSkill001_T(){};
				~XiaoYaoSkill001_T(){};
				BOOL IsPassive(VOID) const {return FALSE;};
			protected: //interface for interval using 
				BOOL SpecificConditionCheck(Obj_Character& rMe) const;
				BOOL EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag=FALSE) const;
			private:
		};
	};
};
#endif //XiaoYaoSKILL001_H

