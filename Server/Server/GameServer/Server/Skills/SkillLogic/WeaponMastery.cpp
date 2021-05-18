#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：WeaponMastery.cpp
// 功能说明：
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "Type.h"
#include "WeaponMastery.h"
#include "LogicManager.h"


namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Impact_Module;
		BOOL WeaponMastery_T::Refix_ItemEffect(SkillInfo_T & rThisPassiveSkill, INT nSlotID, INT nItemType, INT nAttrID, SHORT & rAttrValue) const
		{
			__ENTER_FUNCTION
			INT nRefixAttackPhysics = rThisPassiveSkill.GetDescriptorByIndex(BASE_ATTACK_PHYSICS_REFIX)->GetValue();
			INT nRefixAttackMagic = rThisPassiveSkill.GetDescriptorByIndex(BASE_ATTACK_MAGIC_REFIX)->GetValue();
			INT const nMasteryType1 = rThisPassiveSkill.GetDescriptorByIndex(MASTERY_OF_WEAPON_TYPE1)->GetValue();
			INT const nMasteryType2 = rThisPassiveSkill.GetDescriptorByIndex(MASTERY_OF_WEAPON_TYPE2)->GetValue();
			switch (nSlotID)
			{
				case	HEQUIP_WEAPON: 	//武器	WEAPON
					{
						if(nMasteryType1 == nItemType || nMasteryType2 == nItemType)
						{
							switch (nAttrID)
							{
								case IATTRIBUTE_BASE_ATTACK_P:			//基础物理防御
									{
										rAttrValue = Float2Int((rAttrValue * nRefixAttackPhysics)/100.0f);
									}
									break;
								case IATTRIBUTE_BASE_ATTACK_M:			//基础魔法防御
									{
										rAttrValue = Float2Int((rAttrValue * nRefixAttackMagic)/100.0f);
									}
									break;
								default:
									break;
							}
						}
					}
					break;
					break;
				case	HEQUIP_CAP:		//帽子	DEFENCE
				case	HEQUIP_ARMOR:	//盔甲	DEFENCE
				case	HEQUIP_CUFF:	//护腕	DEFENCE
				case	HEQUIP_BOOT:	//鞋	DEFENCE
				case	HEQUIP_SASH:	//腰带	ADORN
				case	HEQUIP_RING:	//戒子	ADORN
				case	HEQUIP_NECKLACE://项链	ADORN
				case	HEQUIP_RIDER:	//骑乘  ADORN
				default:
					break;
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};
