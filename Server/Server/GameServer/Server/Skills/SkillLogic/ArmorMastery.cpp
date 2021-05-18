#include "stdafx.h"
///////////////////////////////////////////////////////////////////////////////
// 文件名：ArmorMastery.cpp
// 功能说明：
//
// 修改记录：
//
//
//
///////////////////////////////////////////////////////////////////////////////

#include "Type.h"
#include "Obj_Character.h"
#include "ArmorMastery.h"
#include "Scene.h"
#include "Obj.h"
#include "Obj_Human.h"
#include "LogicManager.h"


namespace Combat_Module
{
	namespace Skill_Module
	{
		using namespace Combat_Module;
		using namespace Impact_Module;
		//class ArmorMastery_T : public SkillLogic_T
		BOOL ArmorMastery_T::Refix_ItemEffect(SkillInfo_T & rThisPassiveSkill, INT nSlotID, INT nItemType, INT nAttrID, SHORT & nAttrValue) const
		{
			__ENTER_FUNCTION
			INT nRefixDefencePhysics = rThisPassiveSkill.GetDescriptorByIndex(BASE_DEFENCE_PHYSICS)->GetValue();
			INT nRefixDefenceMagic = rThisPassiveSkill.GetDescriptorByIndex(BASE_DEFENCE_MAGIC)->GetValue();
			switch (nSlotID)
			{
				case	HEQUIP_WEAPON: 	//武器	WEAPON
					break;
				case	HEQUIP_CAP:		//帽子	DEFENCE
				case	HEQUIP_ARMOR:	//盔甲	DEFENCE
				case	HEQUIP_CUFF:	//护腕	DEFENCE
				case	HEQUIP_BOOT:	//鞋	DEFENCE
				case	HEQUIP_SASH:	//腰带	ADORN
					{
						switch (nAttrID)
						{
							case IATTRIBUTE_BASE_DEFENCE_P:			//基础物理防御
								{
									nAttrValue = Float2Int((nAttrValue * nRefixDefencePhysics)/100.0f);
								}
								break;
							case IATTRIBUTE_BASE_DEFENCE_M:			//基础魔法防御
								{
									nAttrValue = Float2Int((nAttrValue * nRefixDefenceMagic)/100.0f);
								}
								break;
							default:
								break;
						}
					}
					break;
				case	HEQUIP_RING:	//戒子	ADORN
				case	HEQUIP_NECKLACE://项链	ADORN
				case	HEQUIP_RIDER:   //骑乘  ADORN
					break;
				default:
					break;
			}
			return TRUE;
			__LEAVE_FUNCTION
			return FALSE;
		}
	};
};
