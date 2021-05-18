///////////////////////////////////////////////////////////////////////////////
// 文件名：ArmorMastery.h
// 功能说明：
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef ARMORMASTERY_H
#define ARMORMASTERY_H

#include "Type.h"
#include "SkillLogic.h"

using namespace Combat_Module;
using namespace Combat_Module::Skill_Module;
namespace Combat_Module
{
	namespace Skill_Module
	{
		class ArmorMastery_T : public SkillLogic_T
		{
			public:
				enum
				{
					ID = ARMOR_MASTERY,
				};
				enum
				{
					BASE_DEFENCE_PHYSICS = 0,
					BASE_DEFENCE_MAGIC = 1,
				};
				// public interface
				ArmorMastery_T(){};
				~ArmorMastery_T(){};
				BOOL IsPassive(VOID) const {return TRUE;};
			protected: //interface for interval using 
				BOOL Refix_ItemEffect(SkillInfo_T & rThisPassiveSkill, INT nSlotID, INT nItemType, INT nATTRID, SHORT & nAttrValue) const;
			private:
		};
	};
};
#endif //ARMORMASTERY_H

