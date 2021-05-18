#include "StdAfx.h"
#include "TDTimeSystem.h"
#include "TDUtil.h"
#include "Obj_PlayerNPC.h"
#include "..\..\Global.h"
#include "..\..\World\WorldManager.h"
#include "..\..\Procedure\GameProcedure.h"
#include "Obj_PlayerMySelf.h"
#include "..\ObjectManager.h"
#include "..\ObjectDef.h"
#include "..\..\DBC\GMDataBase.h"
#include "TDDBC_Struct.h"
#include "TDException.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "TDTimeSystem.h"
#include "AxProfile.h"
#include "TDActionSystem.h"
#include "TDGameInterface.h"
#include "..\..\DataPool\GMDP_Struct_Skill.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "GameStruct_Skill.h"
#include "Obj_PlayerMySelf.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\Item\Obj_Item.h"
#include "..\Item\Obj_Item_Medicine.h"
#include "..\..\Action\GMActionSystem_Item.h"
#include "..\WxRender\RenderSystem.h"

WX_IMPLEMENT_DYNAMIC(CObject_PlayerNPC, GETCLASS(CObject_Character));
CObject_PlayerNPC::CObject_PlayerNPC()
{
	m_pCreatureInfo		= NULL;
	m_GUIDServer		= 0xFFFFFFFF;
	m_bNeedFaceToPlayer	= FALSE;
	m_eNpcType			= NPC_TYPE_INVALID;
	m_uStartFaceToPlayerTime	= 0;
	m_bFaceToPlayer				= FALSE;
	m_fvPreRotation				= fVector3(0.0f, 0.0f, 0.0f);
}

CObject_PlayerNPC::~CObject_PlayerNPC()
{
}

VOID CObject_PlayerNPC::Initial( VOID *pParam )
{
	CObject_Character::Initial( pParam );
	SetWeaponType(WEAPON_TYPE_MONSTER);
	Disalbe(OSF_RAY_QUERY);
	m_uStartFaceToPlayerTime	= 0;
	m_bFaceToPlayer				= FALSE;
}

ENUM_NPC_TYPE CObject_PlayerNPC::GetNpcType(VOID)const
{
	return m_eNpcType;
}

VOID CObject_PlayerNPC::SetNpcType(ENUM_NPC_TYPE eType)
{
	m_eNpcType = eType;
	if(m_pRenderInterface)
	{
		if(NPC_TYPE_PET == m_eNpcType)
			m_pRenderInterface->RayQuery_SetLevel(tEntityNode::RL_PET);
		else
			m_pRenderInterface->RayQuery_SetLevel(tEntityNode::RL_CREATURE);
	}
}

VOID CObject_PlayerNPC::CreateRenderInterface(VOID)
{
	if(m_pCreatureInfo != NULL)
	{
		DBC_DEFINEHANDLE(s_pCharModelDBC, DBC_CHARACTER_MODEL)

		LPCSTR lpszModelFileName = NULL;
		INT nCharModelID = GetCharModelID();
		const _DBC_CHARACTER_MODEL *pCharModel = (const _DBC_CHARACTER_MODEL*)(s_pCharModelDBC->Search_Index_EQU( nCharModelID ));
		if (pCharModel != NULL)
		{
			lpszModelFileName = pCharModel->m_pszModelName;
		}

		if (lpszModelFileName != NULL)
		{
			m_pRenderInterface = CGameProcedure::s_pGfxSystem->New_EntityObject(tEntityNode::ETYPE_ACTOR);
			m_pRenderInterface->SetData(GetID());

			//设置ActorFile
			//AxProfile::AxProfile_PushNode("LoadActorFile");
			m_pRenderInterface->Actor_SetFile( lpszModelFileName );
			//AxProfile::AxProfile_PopNode("LoadActorFile");
		}
	}
	CObject_Character::CreateRenderInterface();
}

BOOL CObject_PlayerNPC::FillMouseCommand_Left( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill )
{
	pOutCmd->m_typeMouse = SCommand_Mouse::MCT_NULL;

	if ( CanbeSelect() )
	{
		//是物品
		if(pActiveSkill && pActiveSkill->GetType() == AOT_ITEM)
		{
			//鼠标上挂的是一个物品

			//空物品
			if(!pActiveSkill || pActiveSkill->GetType() != AOT_ITEM) return TRUE;
			CObject_Item* pItem = (CObject_Item*)(((CActionItem_Item*)pActiveSkill)->GetItemImpl());
			if(!pItem) return  TRUE;
			//必须是能够使用的物品
			if(pItem->GetItemClass() != ICLASS_COMITEM) return  TRUE;
			
			//是否能够使用
			int objID;
			PET_GUID_t petID;
			bool bCanuseDir = ((CObject_Item_Medicine*)pItem)->IsValidTarget(this, 
				fVector2(-1, -1), objID, petID);

			if(bCanuseDir)
			{
				pOutCmd->m_typeMouse = SCommand_Mouse::MCT_USE_ITEM;
				pOutCmd->m_apParam[0] = (VOID*)pActiveSkill;
				pOutCmd->m_adwParam[1] = GetServerID();
				pOutCmd->m_adwParam[4] = FALSE;
			}
		}
		else
		{
			ENUM_RELATION eCampType = CGameProcedure::s_pGameInterface->GetCampType( CObjectManager::GetMe()->GetMySelf(), this );

			if(CharacterLogic_Get() == CHARACTER_LOGIC_DEAD)
			{
				pOutCmd->m_typeMouse = SCommand_Mouse::MCT_NULL;
				return TRUE;
			}

			switch ( eCampType )
			{
			case RELATION_FRIEND:
				if ( CannotBeAttack() )
				{
					pOutCmd->m_typeMouse = SCommand_Mouse::MCT_SPEAK;
					pOutCmd->m_adwParam[0] = GetServerID();
				}
				break;
			case RELATION_ENEMY:
			default:
				{
					if ( pActiveSkill != NULL
						&& pActiveSkill->GetType() == AOT_SKILL
						/*&& !CannotBeAttack()*/ )
					{
						const SCLIENT_SKILL* pSkillImpl = (const SCLIENT_SKILL*)pActiveSkill->GetImpl();
						if(pSkillImpl != NULL
							&& pSkillImpl->m_pDefine->m_nSelectType == SELECT_TYPE_CHARACTER)
						{
							pOutCmd->m_typeMouse = SCommand_Mouse::MCT_SKILL_OBJ;
							pOutCmd->m_apParam[0] = (VOID*)pActiveSkill;
							pOutCmd->m_adwParam[1] = GetServerID();
						}
					}
				}
				break;
			}
		}
	}
	return TRUE;
}

BOOL CObject_PlayerNPC::FillMouseCommand_Right( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill )
{
	pOutCmd->m_typeMouse = SCommand_Mouse::MCT_NULL;
	if ( CanbeSelect() )
	{
		ENUM_NPC_TYPE ot = GetNpcType();
		switch(ot)
		{
		case NPC_TYPE_PET:
			pOutCmd->m_typeMouse	= SCommand_Mouse::MCT_CONTEXMENU;
			pOutCmd->m_adwParam[0]	= GetServerID();
			break;
		default:
			pOutCmd->m_typeMouse	= SCommand_Mouse::MCT_PLAYER_SELECT;
			pOutCmd->m_adwParam[0]	= GetServerID();
			break;
		}
	}
	return TRUE;
}

INT CObject_PlayerNPC::CannotBeAttack( VOID )const
{
	if(m_pCreatureInfo)
		return m_pCreatureInfo->nCannotBeAttack; 
	else 
		return CObject_Character::CannotBeAttack();
}

VOID CObject_PlayerNPC::FaceToPlayer(VOID)
{
	m_bFaceToPlayer = TRUE;
	m_uStartFaceToPlayerTime = CGameProcedure::s_pTimeSystem->GetTimeNow();
	m_fvPreRotation  = GetRotation();
}

VOID CObject_PlayerNPC::StopFaceToPlayer(VOID)
{
	m_bFaceToPlayer = FALSE;
	SetFaceDir(GetPreRotation().y);
}

BOOL CObject_PlayerNPC::IsCanFaceToPlayer(VOID) 
{
	if(m_pCreatureInfo != NULL)
	{
		return (m_pCreatureInfo->nIsCanSwerve != 0)?(TRUE):(FALSE);
	}
	else
	{
		return FALSE;
	}
}

BOOL CObject_PlayerNPC::IsDisplayBoard(VOID) 
{
	if(m_pCreatureInfo != NULL)
	{
		return (m_pCreatureInfo->nIsDisplayerName != 0)?(TRUE):(FALSE);
	}
	else
	{
		return FALSE;
	}
}


FLOAT CObject_PlayerNPC::GetBoardHeight(VOID) 
{
	if(m_pCreatureInfo != NULL)
	{
		return m_pCreatureInfo->fBoardHeight;
	}
	else
	{
		return 0;
	}
}

FLOAT CObject_PlayerNPC::GetProjtexRange(VOID) 
{
	if(GetCharacterType() == CT_MONSTER)
	{
		return m_pCreatureInfo ? m_pCreatureInfo->fProjTexRange : 0.0f;
	}
	else return 100.0f;
}

FLOAT CObject_PlayerNPC::GetShadowRange(VOID) 
{
	if(GetCharacterType() == CT_MONSTER)
	{
		return m_pCreatureInfo ? m_pCreatureInfo->fShadowRange : 0.0f;
	}
	else return 100.0f;
}

LPCSTR CObject_PlayerNPC::GetPortrait(VOID) 
{
	if(m_pCreatureInfo != NULL)
	{
		return m_pCreatureInfo->szIconName;
	}
	else
	{
		return NULL;
	}
}

INT CObject_PlayerNPC::CalcCharModelID( VOID )const
{
	//经过变身
	if (GetConstCharacterData()->Get_ModelID() != -1)
	{
		//直接从模型定义表中读取模型名
		return GetConstCharacterData()->Get_ModelID();
	}
	else
	{
		if ( m_pCreatureInfo != NULL )
			return m_pCreatureInfo->nModelID;
		else
			return INVALID_ID;
	}
}

UINT CObject_PlayerNPC::GetIdleInterval(VOID)const
{
	if(m_pCreatureInfo != NULL && m_pCreatureInfo->nIdleInterval >= 0)
	{
		return m_pCreatureInfo->nIdleInterval * 1000;
	}
	else
	{
		return CObject_Character::GetIdleInterval();
	}
}

VOID CObject_PlayerNPC::UpdateCharRace(VOID)
{
	CCharacterData *pCharacterData = GetCharacterData();
	if(pCharacterData != NULL && pCharacterData->Get_RaceID() != INVALID_ID)
	{
		//打开生物定义表
		DBC_DEFINEHANDLE(s_pCreatureDBC, DBC_CREATURE_ATT)
		m_pCreatureInfo = (const _DBC_CREATURE_ATT*)(s_pCreatureDBC->Search_Index_EQU(pCharacterData->Get_RaceID()));
	}
}

VOID CObject_PlayerNPC::OnDataChanged_RaceID( VOID )
{
	CObject_Character::OnDataChanged_RaceID();

	if ( m_pCreatureInfo != NULL && m_pCreatureInfo->nCanHandle )
		Enable(OSF_RAY_QUERY);
	else
		Disalbe(OSF_RAY_QUERY);
}

VOID CObject_PlayerNPC::ProcessFaceToPlayer(VOID)
{
	// 如果是任务NPC，设置其自身的方向
	if(GetCharacterType() == CT_MONSTER)
	{// 不是自己 也不是其他玩家 只能是NPC
		BOOL bRet = GetNeedFaceToPlayer();
		if (TRUE == bRet)
		{
			FaceToPlayer();
			SetNeedFaceToPlayer(FALSE);
		}

		if(m_bFaceToPlayer)
		{
			UINT uTimeNow = CGameProcedure::s_pTimeSystem->GetTimeNow();
			if(m_uStartFaceToPlayerTime + 10000 < uTimeNow)
			{
				StopFaceToPlayer();
			}
			else
			{
				FLOAT fDir;
				fVector2 fvThis;
				fVector2 fvTarget2;
				// 玩家的位置
				CObject_PlayerMySelf* pMySelf = CObjectManager::GetMe()->GetMySelf();
				fvThis.x = pMySelf->GetPosition().x;
				fvThis.y = pMySelf->GetPosition().z;
				// fvTargent2为NPC的位置
				fvTarget2.x = GetPosition().x;
				fvTarget2.y = GetPosition().z;
				// 调用NPC对象的SetFaceDir,使其转向玩家
				fDir = TDU_GetYAngle(fvTarget2, fvThis);
				SetFaceDir( fDir );
			}
		}
	}
}

VOID CObject_PlayerNPC::CharacterLogic_OnStopped(BOOL bFinished)
{
	if(CharacterLogic_Get() == CHARACTER_LOGIC_IDLE)
	{
		if(m_bFaceToPlayer)
		{
			StopFaceToPlayer();
		}
	}

	CObject_Character::CharacterLogic_OnStopped(bFinished);
}

BOOL CObject_PlayerNPC::Tick_Idle(UINT uElapseTime)
{
	BOOL bResult = CObject_Character::Tick_Idle(uElapseTime);
	if(!bResult)
		return bResult;

	// 处理NPC转向问题
	ProcessFaceToPlayer();
	return TRUE;
}
