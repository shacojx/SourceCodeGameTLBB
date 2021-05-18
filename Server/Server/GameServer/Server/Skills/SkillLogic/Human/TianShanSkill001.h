///////////////////////////////////////////////////////////////////////////////
// 文件名：TianShanSkill001.h
// 功能说明：
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef TianShanSKILL001_H
#define TianShanSKILL001_H

#include "SkillLogic.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;
namespace Combat_Module
{
	namespace Skill_Module
	{
		class TianShanSkill001_T : public SkillLogic_T
		{
			public:
				enum
				{
					ID = TIAN_SHAN_SKILL_001,
				};
				// public interface
				TianShanSkill001_T(){};
				~TianShanSkill001_T(){};
				BOOL IsPassive(VOID) const {return FALSE;};
			protected: //interface for interval using 
				BOOL EffectOnUnitOnce(Obj_Character& rMe, Obj_Character& rTar, BOOL bCriticalFlag=FALSE) const;
			private:
		};
	};
};
#endif //TianShanSKILL001_H

