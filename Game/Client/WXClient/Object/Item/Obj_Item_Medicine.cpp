#include "StdAfx.h"
#include "Obj_Item_Medicine.h"
#include "TDException.h"
#include "..\character\obj_playermyself.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\ObjectManager.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Global.h"
#include "..\..\World\WorldManager.h"
#include "TDGameInterface.h"

CObject_Item_Medicine::CObject_Item_Medicine(INT id)
		: CObject_Item(id)
{
	m_theBaseDef = NULL;
}

CObject_Item_Medicine::~CObject_Item_Medicine()
{
}

VOID CObject_Item_Medicine::AsMedicine(const _DBC_ITEM_MEDIC* pMedicDefine)
{
	TDAssert(pMedicDefine);

	m_theBaseDef = pMedicDefine;
	m_nParticularID = ( ( ( ( ( m_theBaseDef->nClass * 100 ) + m_theBaseDef->nQuality ) * 100 ) + m_theBaseDef->nType ) * 1000 ) + m_theBaseDef->nIndex;

}

LPCTSTR  CObject_Item_Medicine::GetIconName(VOID) const
{
	return m_theBaseDef->szIcon;
}

//设置详细解释
VOID CObject_Item_Medicine::SetExtraInfo(const _ITEM * pItemInfo)
{
	TDAssert(pItemInfo);

	//调用基类函数，保存_ITEM结构
	CObject_Item::SetExtraInfo(pItemInfo);

	const MEDIC_INFO& infoMedic = pItemInfo->m_Medic;

	SetNumber(pItemInfo->GetItemCount());
	SetManufacturer(pItemInfo);
}

LPCTSTR  CObject_Item_Medicine::GetExtraDesc(VOID)
{
	m_strExtraDesc = m_theBaseDef->szDesc;
	return m_strExtraDesc.c_str();
}

//克隆详细信息
VOID	CObject_Item_Medicine::Clone(const CObject_Item * pItemSource)
{
	SetNumber(((CObject_Item_Medicine*)pItemSource)->GetNumber());
	CObject_Item::Clone(pItemSource);
}

//得到物品的类别
ITEM_CLASS	CObject_Item_Medicine::GetItemClass(VOID) const 
{ 
	return (ITEM_CLASS)m_theBaseDef->nClass; 
}

// 得到物品的品质
INT		CObject_Item_Medicine::GetQuantity()
{
	if(m_theBaseDef)
	{
		return m_theBaseDef->nQuality;
	}

	return -1;
}

	
	// 得到物品卖给npc的价格
INT		CObject_Item_Medicine::GetItemPrice()
{
	if(m_theBaseDef)
	{
		return (m_theBaseDef->nSalePrice*this->GetNumber());
	}
	return 0;
}

// 得到物品会引起的技能ID
INT		CObject_Item_Medicine::GetItemSkillID()
{
	if(m_theBaseDef)
	{
		return m_theBaseDef->nSkillID;
	}
	return 0;
}

INT		CObject_Item_Medicine::GetItemTargetType()
{
	if(m_theBaseDef)
	{
		return m_theBaseDef->nTargetType;
	}
	return ITEM_TARGET_TYPE_INVALID;
}

INT		CObject_Item_Medicine::GetNeedLevel()
{
	if(m_theBaseDef)
	{
		return m_theBaseDef->nLevelRequire;
	}
	return 0;
}

	// 得到消耗品在表中的类型
INT CObject_Item_Medicine::GetMedicineItemTableType()
{
	if(m_theBaseDef)
	{
		return m_theBaseDef->nType;
	}

	return -1;
}

// 得到quality信息
INT	CObject_Item_Medicine::GetItemTableQuality(VOID)
{
	return m_theBaseDef->nQuality;
}

// 得到type信息
INT	CObject_Item_Medicine::GetItemTableType(VOID)
{
	return m_theBaseDef->nType;
}

bool CObject_Item_Medicine::IsValidTarget(CObject* pSelectObj, fVector2& fvPos, int& objID, PET_GUID_t& petID)
{
	if(!m_theBaseDef) return false;
	//玩家自己
	CObject_PlayerMySelf* pMySlef = CObjectManager::GetMe()->GetMySelf();

	//无需目标
	if(ITEM_TARGET_TYPE_NONE==m_theBaseDef->nTargetType) 
		return true;

	//需要选择一个场景位置
	if(ITEM_TARGET_TYPE_POS==m_theBaseDef->nTargetType) 
	{
		return CWorldManager::GetMe()->GetActiveScene()->IsValidPosition(fvPos)==TRUE;
	}

	//目前不支持的方式
	if(	ITEM_TARGET_TYPE_DIR==m_theBaseDef->nTargetType ||	//方向
		ITEM_TARGET_TYPE_ITEM==m_theBaseDef->nTargetType)	//物品->物品
		return false;

	switch(m_theBaseDef->nTargetType)
	{
	case ITEM_TARGET_TYPE_SELF:	//自已
		{
			objID = pMySlef->GetServerID();
		}
		return true;

	case ITEM_TARGET_TYPE_SELF_PET:	//自己的宠物
		{
			petID = pMySlef->GetCharacterData()->Get_CurrentPetGUID();
			//宠物尚未释放
			if(petID.IsNull())
				return false;

			objID = pMySlef->GetServerID();
			return true;
		}
		break;

	case ITEM_TARGET_TYPE_FRIEND:	//友好的目标
	case ITEM_TARGET_TYPE_ENEMY:	//敌对目标
		{
			//必须需要一个目标
			if(!pSelectObj) return false;
			//必须是角色
			if(!g_theKernel.IsKindOf(pSelectObj->GetClass(), GETCLASS(CObject_Character)))
				return false;

			//检查阵营
			ENUM_RELATION eCampType = 
				CGameProcedure::s_pGameInterface->GetCampType(pMySlef, pSelectObj);

			//必须是友好目标/敌对目标
			if( (m_theBaseDef->nTargetType==ITEM_TARGET_TYPE_FRIEND && 
					(pSelectObj==pMySlef || eCampType==RELATION_FRIEND)) ||
				(m_theBaseDef->nTargetType==ITEM_TARGET_TYPE_ENEMY && 
					(eCampType==RELATION_ENEMY)))
			{
				objID = pSelectObj->GetServerID();
				return true;
			}
		}
		return false;

	case ITEM_TARGET_TYPE_FRIEND_PLAYER: // 友好玩家	
	case ITEM_TARGET_TYPE_ENEMY_PLAYER:	//	敌对玩家
		{
			//必须需要一个目标
			if(!pSelectObj) return false;
			//必须是玩家
			if(!g_theKernel.IsKindOf(pSelectObj->GetClass(), GETCLASS(CObject_PlayerOther)))
				return false;

			//检查阵营
			ENUM_RELATION eCampType = 
				CGameProcedure::s_pGameInterface->GetCampType(pMySlef, pSelectObj);

			//必须是友好玩家/敌对玩家
			if(	(m_theBaseDef->nTargetType==ITEM_TARGET_TYPE_FRIEND_PLAYER &&
					(pSelectObj==pMySlef || eCampType==RELATION_FRIEND)) ||
				(m_theBaseDef->nTargetType==ITEM_TARGET_TYPE_ENEMY_PLAYER &&
					(eCampType==RELATION_ENEMY)))
			{
				objID = pSelectObj->GetServerID();
				return true;
			}
		}
		return false;

	case ITEM_TARGET_TYPE_FRIEND_MONSTER:	//友好怪物
	case ITEM_TARGET_TYPE_ENEMY_MONSTER:	//敌对怪物
		{
			//必须需要一个目标
			if(!pSelectObj) return false;
			//必须是角色
			if(!g_theKernel.IsKindOf(pSelectObj->GetClass(), GETCLASS(CObject_Character)))
				return false;
			//必须是NPC
			if(((CObject_Character*)pSelectObj)->GetCharacterType() != 
					CObject_Character::CT_MONSTER) return false;

			//检查阵营
			ENUM_RELATION eCampType = 
				CGameProcedure::s_pGameInterface->GetCampType(pMySlef, pSelectObj);

			//必须是友好NPC
			if(	(m_theBaseDef->nTargetType==ITEM_TARGET_TYPE_FRIEND_MONSTER && 
					eCampType==RELATION_FRIEND) ||
				(m_theBaseDef->nTargetType==ITEM_TARGET_TYPE_ENEMY_MONSTER && 
					eCampType==RELATION_ENEMY))
			{
				objID = pSelectObj->GetServerID();
				return true;
			}
		}
		return false;

	case ITEM_TARGET_TYPE_FRIEND_PET:	//友好宠物
	case ITEM_TARGET_TYPE_ENEMY_PET:	//敌对宠物	
		{
			//必须需要一个目标
			if(!pSelectObj) return false;
			//必须是角色
			if(!g_theKernel.IsKindOf(pSelectObj->GetClass(), GETCLASS(CObject_Character)))
				return false;
			//必须是NPC
			if(((CObject_Character*)pSelectObj)->GetCharacterType() != 
					CObject_Character::CT_MONSTER) return false;
			//必须是宠物
			if(((CObject_PlayerNPC*)pSelectObj)->GetNpcType() !=
					NPC_TYPE_PET) return false;

			//检查阵营
			ENUM_RELATION eCampType = 
				CGameProcedure::s_pGameInterface->GetCampType(pMySlef, pSelectObj);

			//必须是友好宠物
			if(	(m_theBaseDef->nTargetType==ITEM_TARGET_TYPE_FRIEND_MONSTER && 
					eCampType==RELATION_FRIEND) ||
				(m_theBaseDef->nTargetType==ITEM_TARGET_TYPE_ENEMY_PET && 
					eCampType==RELATION_ENEMY))
			{
				objID = pSelectObj->GetServerID();
				return true;
			}
		}
		return false;

	case ITEM_TARGET_TYPE_ALL_CHARACTER:	//所有角色
		{
			//必须需要一个目标
			if(!pSelectObj) return false;
			//必须是角色
			if(!g_theKernel.IsKindOf(pSelectObj->GetClass(), GETCLASS(CObject_Character)))
				return false;

			return true;
		}
		return false;

	default:
		break;
	}

	return false;
}

bool CObject_Item_Medicine::IsAreaTargetType(void) const
{
	if(!m_theBaseDef) return false;
	//玩家自己
	CObject_PlayerMySelf* pMySlef = CObjectManager::GetMe()->GetMySelf();

	//无需目标
	return (ITEM_TARGET_TYPE_POS==m_theBaseDef->nTargetType);
}

bool CObject_Item_Medicine::IsTargetOne(void) const
{
	if(!m_theBaseDef) return false;

	return (ITEM_TARGET_TYPE_SELF==m_theBaseDef->nTargetType || 
			ITEM_TARGET_TYPE_SELF_PET==m_theBaseDef->nTargetType);
}


	// 得到物品的类型描述2006-4-28
LPCTSTR	CObject_Item_Medicine::GetItemTableTypeDesc()
{
	return m_theBaseDef->szTypeDesc;
}

// 得到物品档次描述信息
INT	CObject_Item_Medicine::GetItemLevelDesc()
{

	return 1;//m_theBaseDef->nLevelDesc;
}

