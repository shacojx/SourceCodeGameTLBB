///////////////////////////////////////////////////////////////////////////////
// 文件名：WeaponMastery.h
// 功能说明：
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef WEAPONMASTERY_H
#define WEAPONMASTERY_H

#include "Type.h"
#include "SkillLogic.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;
namespace Combat_Module
{
	namespace Skill_Module
	{
		class WeaponMastery_T : public SkillLogic_T
		{
			public:
				enum
				{
					ID = WEAPON_MASTERY,
				};
				enum
				{
					BASE_ATTACK_PHYSICS_REFIX = 0, 
					BASE_ATTACK_MAGIC_REFIX,
					MASTERY_OF_WEAPON_TYPE1,
					MASTERY_OF_WEAPON_TYPE2,
				};
				// public interface
				WeaponMastery_T(){};
				~WeaponMastery_T(){};
				BOOL IsPassive(VOID) const {return TRUE;};
			protected: //interface for interval using 
				BOOL Refix_ItemEffect(SkillInfo_T & rThisPassiveSkill, INT nSlotID, INT nItemType, INT nATTRID, SHORT & nAttrValue) const;
			private:
		};
	};
};
#endif //WEAPONMASTERY_H

