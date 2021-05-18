///////////////////////////////////////////////////////////////////////////////
// 文件名：XiaoYaoSkill002.h
// 功能说明：
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef XiaoYaoSKILL002_H
#define XiaoYaoSKILL002_H

#include "SkillLogic.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;
namespace Combat_Module
{
	namespace Skill_Module
	{
		class XiaoYaoSkill002_T : public SkillLogic_T
		{
			public:
				enum
				{
					ID = XIAO_YAO_SKILL_002,
				};
				// public interface
				XiaoYaoSkill002_T(){};
				~XiaoYaoSkill002_T(){};
				BOOL IsPassive(VOID) const {return FALSE;};
			protected: //interface for interval using 
				BOOL EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag=FALSE) const;
			private:
		};
	};
};
#endif //XiaoYaoSKILL002_H

