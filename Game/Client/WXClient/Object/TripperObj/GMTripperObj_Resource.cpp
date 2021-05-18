#include "StdAfx.h"
#include "GMTripperObj_Resource.h"
#include "../../Global.h"
#include "../ObjectManager.h"
#include "../Character/Obj_PlayerMySelf.h"
#include "../../Procedure/GameProcedure.h"
#include "TDDBC_Struct.h"
#include "../../DBC/GMDataBase.h"
#include "../../NetWork/NetManager.h"
#include "CGUseAbility.h"
#include "../../DataPool/GMDataPool.h"
#include "../../DataPool/DetailAttrib_Player.h"
#include "../../Event\GMEventSystem.h"
#include "TDGameInterface.h"
#include "../../DataPool/GMDP_CharacterData.h"
#include "..\WxRender\RenderSystem.h"

WX_IMPLEMENT_DYNAMIC(CTripperObject_Resource, GETCLASS(CTripperObject));
CTripperObject_Resource::CTripperObject_Resource()
{

}


CTripperObject_Resource::~CTripperObject_Resource()
{
}

BOOL CTripperObject_Resource::SetResourceID(INT nID)
{
	DBC_DEFINEHANDLE(pDBC, DBC_LIFEABILITY_GROWPOINT);
	m_pResourceDef = (const _DBC_LIFEABILITY_GROWPOINT*)pDBC->Search_Index_EQU(nID);

	if(!m_pResourceDef) return FALSE;

	return true;
}
INT CTripperObject_Resource::GetResourceID(VOID)
{
	if(!m_pResourceDef) return FALSE;

	return m_pResourceDef->nLifeAbility;
}

VOID CTripperObject_Resource::Initial(VOID*)
{
	if ( m_pRenderInterface == NULL && m_pResourceDef->szMeshFile)
	{
		//根据后缀决定类型
		char* pExt = PathFindExtension(m_pResourceDef->szMeshFile);
		if(pExt && _stricmp(pExt, ".model")==0)
		{
			//创建渲染层实体
			m_pRenderInterface = CGameProcedure::s_pGfxSystem->New_EntityObject(tEntityNode::ETYPE_MODEL);
			m_pRenderInterface->StaObj_SetMeshFile(m_pResourceDef->szMeshFile);
		}
		else if(pExt && _stricmp(pExt, ".obj")==0)
		{
			//创建渲染层实体
			m_pRenderInterface = CGameProcedure::s_pGfxSystem->New_EntityObject(tEntityNode::ETYPE_ACTOR);
			m_pRenderInterface->Actor_SetFile(m_pResourceDef->szMeshFile);
		}
		else //mesh
		{
			//创建渲染层实体
			m_pRenderInterface = CGameProcedure::s_pGfxSystem->New_EntityObject(tEntityNode::ETYPE_MESH);
			m_pRenderInterface->StaObj_SetMeshFile(m_pResourceDef->szMeshFile);
		}

		//设置选择优先级
		m_pRenderInterface->RayQuery_SetLevel(tEntityNode::RL_TRIPPEROBJ);

		m_pRenderInterface->SetData(GetID());
	}
}

VOID CTripperObject_Resource::Release( VOID )
{
	if ( m_pRenderInterface != NULL )
	{
		m_pRenderInterface->Destroy();
		m_pRenderInterface = NULL;
	}
	CObject_Static::Release();
}

ENUM_CURSOR_TYPE CTripperObject_Resource::Tripper_GetCursor(VOID) const
{
	if(!m_pResourceDef) return CURSOR_NORMAL;

	INT idAbility = m_pResourceDef->nLifeAbility;
	//如果没有该生活技能，返回普通光标
/*	if(!(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(idAbility)))
	{
		return CURSOR_NORMAL;
	}
*/
	//如果此生长点对应的生活技能是-1时，对鼠标不响应。
	if(idAbility == INVALID_ID)
		return CURSOR_NORMAL;

	switch(idAbility)
	{
	// 采矿技能
	case LIFE_ABILITY_MINE:		return CURSOR_MINE;
	// 采药
	case LIFE_ABILITY_HERBS: 	return CURSOR_HERBS;
	// 钓鱼
	case LIFE_ABILITY_FISH: 	return CURSOR_INTERACT;
	// 操作
	case LIFE_ABILITY_INTERACT: return CURSOR_INTERACT;
	// 驱鬼
	case LIFE_ABILITY_SUBDUE:	return CURSOR_INTERACT;

	default:					return CURSOR_INTERACT;
	}
}

VOID CTripperObject_Resource::Tripper_Active(VOID)
{
	if(!m_pResourceDef) return;

	CGUseAbility msg;
	INT idAbility;

	idAbility = m_pResourceDef->nLifeAbility;

	if( CObjectManager::GetMe()->GetMySelf()->CharacterLogic_Get() == CObject_Character::CHARACTER_LOGIC_ABILITY_ACTION )
		return;

	//如果没有该生活技能，返回
	if(!(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(idAbility)))
	{
	
		DBC_DEFINEHANDLE(pDBC, DBC_LIFEABILITY_DEFINE);
		const _DBC_LIFEABILITY_DEFINE* pAbilityDef = (const _DBC_LIFEABILITY_DEFINE*)pDBC->Search_Index_EQU(idAbility);

		if(!pAbilityDef)
			return;

		CHAR szMsg[256];
		_snprintf(szMsg, 256, "需要%s生活技能",	pAbilityDef->szName);
		ADDNEWDEBUGMSG(STRING(szMsg));

		return;
	}

	msg.SetAbilityID(idAbility);
	msg.SetPlatformGUID(this->GetServerID());
	msg.SetPrescriptionID(INVALID_ID);
	msg.SetPrescriptionID(-1);
	CNetManager::GetMe()->SendPacket(&msg);
}

BOOL CTripperObject_Resource::Resource_IsFish(VOID)
{
	if( m_pResourceDef->nLifeAbility == LIFE_ABILITY_FISH ) return true;
	return FALSE;
}

BOOL CTripperObject_Resource::Resource_CanOperation(VOID)
{
	if(!m_pResourceDef) return FALSE;
	
	const SCLIENT_LIFEABILITY* pAbilityDef = 
		(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(m_pResourceDef->nLifeAbility));

	if(!pAbilityDef )
	{
	
		DBC_DEFINEHANDLE(pDBC, DBC_LIFEABILITY_DEFINE);
		const _DBC_LIFEABILITY_DEFINE* pAbilityDef = (const _DBC_LIFEABILITY_DEFINE*)pDBC->Search_Index_EQU(m_pResourceDef->nLifeAbility);

		if(!pAbilityDef)
			return FALSE;

		CHAR szMsg[256];
			
		STRING strTemp = "";
		strTemp = NOPARAMMSGFUNC("Ability_Requiement");
		_snprintf(szMsg, 256, strTemp.c_str(),	pAbilityDef->szName);

		ADDNEWDEBUGMSG(STRING(szMsg));
		//ADDTALKMSG(szMsg);

		return FALSE;
	}	

	INT my_level = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Level();
	if( my_level < pAbilityDef->m_pDefine->nLevelNeed) 
	{
		ADDTALKMSG("等级不够");
		return FALSE;
	}
	INT idNeedFacility = pAbilityDef->m_pDefine->nToolNeed;

	if( idNeedFacility >= 0 )
	{
		BOOL bHaveTool = FALSE;

		tObject_Item* Facility;
		Facility = CDataPool::GetMe()->UserEquip_GetItem(HEQUIP_WEAPON);
		if(Facility == NULL || idNeedFacility != Facility->GetParticularID())
		{
			for( INT i = 0; i < MAX_BAG_SIZE; i++ )
			{
				if( Facility = CDataPool::GetMe()->UserBag_GetItem(i) )
				{
					if( idNeedFacility == Facility->GetParticularID() )
					{
						bHaveTool = TRUE;
						break;
					}
				}
			}

			CHAR szTemp[256];
			//从资源表中去查找工具名称
			DBC_DEFINEHANDLE(s_pItem_WhiteEquip, DBC_ITEM_EQUIP_WHITE);
			//搜索纪录
			const _DBC_ITEM_EQUIP_WHITE* pWhiteEquip = (const _DBC_ITEM_EQUIP_WHITE*)s_pItem_WhiteEquip->Search_Index_EQU((UINT)idNeedFacility);
			if(pWhiteEquip)
			{
				if(bHaveTool)
				{
					sprintf(szTemp,"你需要先将%s装备上",pWhiteEquip->szName);
				}
				else
				{
					sprintf(szTemp,"你还没有%s",pWhiteEquip->szName);
				}
			}
			else
			{
				sprintf(szTemp,"你需要装备工具。");
			}
			CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,szTemp);

			//CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"You have not the facility.");	
			//SCRIPT_SANDBOX::Talk::s_Talk.AddSelfMessage(szTemp);
			ADDTALKMSG(szTemp);
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CTripperObject_Resource::Resource_CanFish(FLOAT distance)
{
	if(!m_pResourceDef) return FALSE;
	
	const SCLIENT_LIFEABILITY* pAbilityDef = 
		CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(m_pResourceDef->nLifeAbility);
	if(!pAbilityDef) return FALSE;

	if( pAbilityDef->m_pDefine->nID == LIFE_ABILITY_FISH )
	{
		FLOAT fishdistance = pAbilityDef->m_pDefine->fPlatformDist;
		//需要距离合适，如果距离不够，也不能开始钓。
		if ( distance > fishdistance ) 
		{
		//	CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"The distance is too far.");
			//SCRIPT_SANDBOX::Talk::s_Talk.AddSelfMessage("距离太远！");
			ADDTALKMSG("距离太远！");
			return FALSE; 
		}
	}

	return TRUE;
}

FLOAT CTripperObject_Resource::Resource_GetOperationDistance(VOID)
{
	// old code {
	//if(!m_pResourceDef) return FALSE;
	//
	//const SCLIENT_LIFEABILITY* pAbilityDef = 
	//	CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(m_pResourceDef->nLifeAbility);
	//if(!pAbilityDef) return FALSE;

	//return pAbilityDef->m_pDefine->fPlatformDist;
	// }
	if(m_pResourceDef != NULL)
	{
		const SCLIENT_LIFEABILITY* pAbilityDef = 
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(m_pResourceDef->nLifeAbility);
		if(pAbilityDef != NULL)
		{
			return pAbilityDef->m_pDefine->fPlatformDist;
		}
	}
	return 0.f;
}
