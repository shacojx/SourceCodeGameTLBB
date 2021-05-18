///////////////////////////////////////////////////////////////////////////////
// 文件名：MingJiaoSkill002.h
// 功能说明：明教技能厚积薄发
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef MINGJIAOSKILL002_H
#define MINGJIAOSKILL002_H

#include "Type.h"
#include "SkillLogic.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;
namespace Combat_Module
{
	namespace Skill_Module
	{
		class MingJiaoSkill002_T : public SkillLogic_T
		{
			public:
				enum
				{
					ID = MING_JIAO_SKILL_002,
				};
				// public interface
				MingJiaoSkill002_T(){};
				~MingJiaoSkill002_T(){};
				BOOL IsPassive(VOID) const {return FALSE;};
			protected: //interface for interval using 
				BOOL EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag=FALSE) const;
			private:
		};
	};
};
#endif //MINGJIAOSKILL002_H
