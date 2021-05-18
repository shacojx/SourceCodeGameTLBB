/////////////////////////////////////////////////////////////////////////////////
//文件名：Item.inl
//功能描述：物品数据读取写入的inline函数
//修改记录：2005-11-2创建
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef __ITEM_INL__
#define __ITEM_INL__
#include "DB_Struct.h"
#include "GameTable.h"

inline _ITEM_GUID Item::GetGUID( )
{
	__ENTER_FUNCTION

	Assert( m_pItem ) ;
	return m_pItem->m_ItemGUID ;

	__LEAVE_FUNCTION

	_ITEM_GUID	ig;
	return	ig;
};

inline UINT Item::GetItemTableIndex( ) const//取得物品表里的物品索引编号
{
	__ENTER_FUNCTION

	Assert( m_pItem ) ;
	return m_pItem->m_ItemIndex ;

	__LEAVE_FUNCTION

	return	-1;
}

inline UCHAR Item::GetItemClass()	const
{
	__ENTER_FUNCTION

	Assert(m_pItem);
	return	m_pItem->ItemClass();

	__LEAVE_FUNCTION

	return	0;

}

inline UCHAR	Item::GetItemQual() const
{
	__ENTER_FUNCTION

	Assert(m_pItem);
	return	m_pItem->GetQual();

	__LEAVE_FUNCTION

	return	0;
}


inline UCHAR	Item::GetItemType() const
{
	__ENTER_FUNCTION
	
	Assert(m_pItem);
	return	m_pItem->ItemType();

	__LEAVE_FUNCTION

	return	0;
}

inline UCHAR	Item::GetItemIndex() const
{
	__ENTER_FUNCTION

	Assert(m_pItem);
	return	m_pItem->GetIndex();

	__LEAVE_FUNCTION

	return	0;
}

inline INT Item::GetLayedNum( ) const//取得叠放物品的数量
{
	__ENTER_FUNCTION

	switch(m_Type)
	{
	case IDT_ITEM:
		{
			Assert( m_pItem ) ;
			return m_pItem->GetItemCount();
		}
		break;
	case IDT_PET:
		{
			Assert(m_pPet);
			return 1;
		}
		break;
	default:
		{
			Assert(FALSE);
			return 0;
		}
	}


	__LEAVE_FUNCTION

	return 0 ;
}


inline BOOL Item::IsPWLock() const
{
	__ENTER_FUNCTION
	switch(m_Type)
	{
		case IDT_ITEM:
			{
				Assert( m_pItem ) ;
				return m_pItem->GetItemPLock();
			}
			break;
		case IDT_PET:
			{
				Assert(m_pPet);
				return 1;
			}
			break;
		default:
			{
				Assert(FALSE);
				return 0;
			}
	}

	__LEAVE_FUNCTION
	return TRUE;
}

inline INT Item::GetMaxLayedNum( ) const//取得能够叠放物品的最大数量
{
	__ENTER_FUNCTION

	switch(m_Type)
	{
	case IDT_ITEM:
		{
			Assert( m_pItem ) ;
			return m_pItem->GetItemTileMax();
		}
		break;
	case IDT_PET:
		{
			Assert(m_pPet);
			return 1;
		}
		break;
	default:
		{
			Assert(FALSE);
			return 0;
		}
	}

	__LEAVE_FUNCTION

	return 0 ;
}

BOOL	Item::IsFull() const
{
	__ENTER_FUNCTION

	switch(m_Type)
	{
	case IDT_ITEM:
		{
			Assert(m_pItem);
			return m_pItem->isFullTile();
		}
		break;
	case IDT_PET:
		{
			Assert(m_pPet);
			return !m_pPet->m_GUID.IsNull();
		}
		break;
	default:
		{
			Assert(FALSE);
			return FALSE;
		}
	}

	__LEAVE_FUNCTION

	return FALSE;
}




USHORT					Item::GetSetNum()			const				//套装编号
{
	Assert(m_pItem);
	return	m_pItem->GetEquipData()->m_SetNum;
}
BYTE					Item::GetMaxSetCount()			const				//套装个数
{
	Assert(m_pItem);
	return m_pItem->GetEquipData()->m_MaxNum;
}
BYTE					Item::GetEquipPoint()		const				//物品装佩点
{
	Assert(m_pItem);
	return m_pItem->GetEquipData()->m_EquipPoint;
}
BYTE					Item::GetMaxDurPoint()		const			//最大耐久值
{
	Assert(m_pItem);
	return m_pItem->GetEquipData()->m_MaxDurPoint;
}
BYTE					Item::GetRequireLevel()		const			//需求等级
{
	Assert(m_pItem);
	return m_pItem->GetEquipData()->m_NeedLevel;
}
BYTE					Item::GetGemSlotMax()		const			//最大宝石个数
{
	Assert(m_pItem);
	return m_pItem->GetEquipData()->m_GemMax;
}
BYTE					Item::GetFailTimes()		const				//修理失败次数
{
	Assert(m_pItem);
	return m_pItem->GetEquipData()->m_FaileTimes;

}
BYTE					Item::GetDurPoints()		const 				//当前耐久值
{
	Assert(m_pItem);
	return m_pItem->GetEquipData()->m_CurDurPoint;
}
USHORT					Item::GetDamagePoint()	const
{
	Assert(m_pItem);
	return m_pItem->GetEquipData()->m_CurDamagePoint;
}

BOOL					Item::GetItemBind()	const
{
	Assert(m_pItem);
	return m_pItem->GetItemBind();
}

BOOL					Item::GetItemIdent()	const
{
	Assert(m_pItem);

	if(m_pItem->ItemClass()==ICLASS_EQUIP&&m_pItem->GetQual() == EQUALITY_BLUE)
	{
		return m_pItem->GetItemIdent();
	}
	return TRUE;
	
}

BYTE					Item::GetAttrCount()		const				//属性的数量
{
	Assert(m_pItem);
	return m_pItem->GetEquipData()->m_AttrCount;
}
BYTE					Item::GetEquipGemCount()	const				//镶嵌宝石的数量
{
	Assert(m_pItem);
	Assert(m_pItem->ItemClass()==ICLASS_EQUIP);
	return m_pItem->GetEquipData()->m_StoneCount;
}
const _ITEM_ATTR				Item::GetEquipAttr(UCHAR	nIndex)		const	//获得装备属性
{	
	Assert(m_pItem);
	Assert(nIndex<m_pItem->GetEquipData()->m_AttrCount);
	return m_pItem->GetEquipData()->m_pAttr[nIndex];
}
const _ITEM_GEMINFO			Item::GetEquipGemInfo(UCHAR nIndex)	const		//获得宝石信息
{
	Assert(m_pItem);
	Assert(nIndex<m_pItem->GetEquipData()->m_GemMax);
	return m_pItem->GetEquipData()->m_pGemInfo[nIndex];
}

BYTE						Item::GetItemLevel()		const				//物品级别,策划需求
{
	Assert(m_pItem);
	return m_pItem->GetMedicData()->m_nLevel;
}
BYTE						Item::GetReqSkillLevel()	const				//获得技能级别需求
{
	Assert(m_pItem);
	return	m_pItem->GetMedicData()->m_nReqSkillLevel;
}
INT							Item::GetReqSkill()			const				//需要拥有的技能号
{
	Assert(m_pItem);
	return m_pItem->GetMedicData()->m_nReqSkill;
}
INT							Item::GetScriptID()			const				//获得相关脚本编号
{
	Assert(m_pItem);
	return m_pItem->GetMedicData()->m_nScriptID;
}
INT							Item::GetSkillID()			const				//获得相关技能编号
{
	Assert(m_pItem);
	return m_pItem->GetMedicData()->m_nSkillID;
}
BOOL						Item::IsCosSelf()			const				//是否消耗自己
{
	Assert(m_pItem);
	return m_pItem->GetMedicData()->m_bCosSelf;
}
BYTE						Item::GetTargetType()		const			   //物品选择对象类型
{
	Assert(m_pItem);
	return	m_pItem->GetMedicData()->m_TargetType;
}


const _ITEM_ATTR				Item::GetGemAttr()		const				//宝石属性
{
	Assert(m_pItem);
	return m_pItem->GetGemData()->m_Attr;
}

INT						Item::GetMapLevel()			const				//藏宝图级别
{
	Assert(m_pItem);
	return m_pItem->GetStoreMapData()->m_nLevel;
}
FLOAT					Item::GetXPos()				const				//x坐标
{
	Assert(m_pItem);
	return m_pItem->GetStoreMapData()->m_xPos;
}
FLOAT					Item::GetZPos()				const				//z坐标
{
	Assert(m_pItem);
	return m_pItem->GetStoreMapData()->m_zPos;
}
INT						Item::GetSceneID()			const				//场景编号
{
	Assert(m_pItem);
	return m_pItem->GetStoreMapData()->m_SceneID;
}
INT						Item::GetGrowType()			const				//对应生长点类型
{
	Assert(m_pItem);
	return m_pItem->GetStoreMapData()->m_GrowType;
}

inline  PET_GUID_t		Item::GetPetGUID( )								//取得宠物GUID
{
	Assert(m_pPet);
	return m_pPet->m_GUID;
}

inline  PET_GUID_t		Item::GetSpouseGUID( )
{
	Assert(m_pPet);
	return m_pPet->m_SpouseGUID;
}


inline	INT				Item::GetDataID()					//取得宠物模型		
{
	Assert(m_pPet);
	return m_pPet->m_nDataID;
}
inline  const CHAR*		Item::GetName()	const				//取得名字
{
	Assert(m_pPet);
	return m_pPet->m_szName;	
}
inline  const CHAR*		Item::GetNick() const				//取得昵称
{
	Assert(m_pPet);
	return m_pPet->m_szNick;
}
inline  INT				Item::GetLevel() const				//取得等级
{
	Assert(m_pPet);
	return m_pPet->m_nLevel;
}
inline  INT				Item::GetTakeLevel() const			//取得携带等级
{
	Assert(m_pPet);
	return m_pPet->m_nTakeLevel;
}
inline  INT				Item::GetAttackType() const			//取得进攻类型（物/法）
{
	Assert(m_pPet);
	return m_pPet->m_nAttackType;
}
inline  INT				Item::GetAIType() const				//取得AI类型
{
	Assert(m_pPet);
	return m_pPet->m_AIType;
}
inline  const _CAMP_DATA *Item::GetCampData() const				//取得阵营
{
	Assert(m_pPet);
	return &(m_pPet->m_CampData);
}
inline  INT				Item::GetHP() const					//取得生命值
{
	Assert(m_pPet);
	return m_pPet->m_nHP;
}
inline  INT				Item::GetBaseMaxHP() const				//取得生命值上限
{
	Assert(m_pPet);

	INT nBaseParam = 0;
	INT nValue = 0;
	INT nAttribParam = 0;
	FLOAT fLevelRate = 0;
	FLOAT fAttribRate = 0.f;

	INT nLevel = GetLevel();
	FLOAT fGrowRate = GetGrowRate();

	nBaseParam = g_TableInit.m_PetConfig.m_BaseHP;
	fAttribRate = g_TableInit.m_PetConfig.m_Con_HP_Pram;
	fLevelRate = g_TableInit.m_PetConfig.m_Level_HP_Pram;
	nAttribParam = GetLvl1Attr(CATTR_LEVEL1_CON);
	nValue = (INT)(nBaseParam + nAttribParam*fAttribRate + nLevel*fLevelRate*fGrowRate);
	return nValue;
}
inline  INT				Item::GetLife() const				//取得当前寿命
{
	Assert(m_pPet);
	return m_pPet->m_nLife;
}
inline  BYTE			Item::GetPetType() const				// 宝宝，变异，野生
{
	Assert(m_pPet);
	return m_pPet->m_byPetType;
}
inline  BYTE			Item::GetGeneration() const			// 几代宠
{
	Assert(m_pPet);
	return m_pPet->m_byGeneration;
}
inline  BYTE			Item::GetHappiness() const				// 快乐度		
{
	Assert(m_pPet);
	return m_pPet->m_byHappiness;
}
inline  INT				Item::GetStrPer() const				// 力量资质	
{
	Assert(m_pPet);
	return m_pPet->m_nStrPerception;
}
inline  INT				Item::GetConPer() const				// 体力资质	
{
	Assert(m_pPet);
	return m_pPet->m_nConPerception;
}
inline  INT 			Item::GetDexPer() const				// 身法资质
{
	Assert(m_pPet);
	return m_pPet->m_nDexPerception;
}
inline  INT				Item::GetSprPer() const				// 灵气资质
{
	Assert(m_pPet);
	return m_pPet->m_nSprPerception;
}
inline  INT 			Item::GetIntPer() const				// 定力资质
{
	Assert(m_pPet);
	return m_pPet->m_nIntPerception;
}
inline	INT				Item::GetGenGu() const				// 根骨
{
	Assert(m_pPet);
	return m_pPet->m_nGenGu;
}
inline  FLOAT			Item::GetGrowRate() const			// 成长率
{
	Assert(m_pPet);
	return m_pPet->m_fGrowRate;
}
inline  INT				Item::GetRemainPoint() const			// 一级属性剩余点数
{
	Assert(m_pPet);
	return m_pPet->m_nRemainPoint;
}
inline  INT				Item::GetExp() const					// 经验值
{
	Assert(m_pPet);
	return m_pPet->m_nExp;
}
inline  INT				Item::GetLvl1Attr(CHAR_ATTR_LEVEL1 type)const// 基础一级战斗属性（不包括技能和装备增加的部分）
{
	Assert(m_pPet);
	return m_pPet->m_BaseAttrLevel1.Get(type);
}
inline  _PET_SKILL		Item::GetSkill(UINT SkillIndex)		//宠物技能
{
	Assert(m_pPet);
	Assert(SkillIndex<PET_MAX_SKILL_COUNT);
	return m_pPet->m_SkillList[SkillIndex];
}
inline  VOID			Item::SetCooldown(CooldownID_t nID, Time_t nTime)
{
	Assert(m_pPet);
	m_pPet->m_CooldownList.RegisterCooldown(nID, nTime);
}
inline	BOOL			Item::IsCooldowned(CooldownID_t nID) const
{
	Assert(m_pPet);
	return m_pPet->m_CooldownList.IsSpecificSlotCooldownedByID(nID);
}
inline	VOID			Item::HeartBeat_Cooldown(Time_t nDeltaTime)
{
	Assert(m_pPet);
	m_pPet->m_CooldownList.HeartBeat(nDeltaTime);
}
inline	Time_t			Item::GetCooldownRemained(CooldownID_t nID) const
{
	Assert(m_pPet);
	return m_pPet->m_CooldownList.GetRemainTimeByID(nID);
}

/*
inline	_IMPACT_LIST&	Item::GetImpactList()				// 得到宠物BUFF列表
{
	Assert(m_pPet);
	return m_pPet->m_ImpactList;
}
*/

#endif					
						
						
						
