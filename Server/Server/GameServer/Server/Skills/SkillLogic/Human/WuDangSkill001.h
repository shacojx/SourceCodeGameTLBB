///////////////////////////////////////////////////////////////////////////////
// 文件名：WuDangSkill001.h
// 功能说明：
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WuDangSKILL001_H
#define WuDangSKILL001_H

#include "SkillLogic.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;
namespace Combat_Module
{
	namespace Skill_Module
	{
		class WuDangSkill001_T : public SkillLogic_T
		{
			public:
				enum
				{
					ID = WU_DANG_SKILL_001,
				};
				// public interface
				WuDangSkill001_T(){};
				~WuDangSkill001_T(){};
				BOOL IsPassive(VOID) const {return FALSE;};
			protected: //interface for interval using 
				BOOL EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag=FALSE) const;
			private:
		};
	};
};
#endif //WuDangSKILL001_H

