#include "StdAfx.h"
#include "Obj_PlayerOther.h"
#include "..\..\Global.h"
#include "..\..\Network\NetManager.h"
#include "CGAttack.h"
#include "CGCharAskEquipment.h"
#include "..\ObjectManager.h"
#include "..\Character\Obj_PlayerMySelf.h"
#include "CGOtherEquip.h"
#include "..\..\NetWork\NetManager.h"
#include "..\..\Procedure\GameProcedure.h"
#include "TDTimeSystem.h"
#include "TDSound.h"
#include "..\ObjectDef.h"
#include "..\ActionSetMgr.h"
#include "..\..\DBC\GMDataBase.h"
#include "TDDBC_Struct.h"
#include "TDException.h"
#include "..\ObjectCommandDef.h"
#include "..\..\GameCommand.h"
#include "..\Item\Obj_Item_Equip.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Event\GMEventSystem.h"
#include "TDProfile.h"
#include "AI\GMAI_Base.h"

#include "TDActionSystem.h"
#include "TDGameInterface.h"
#include "..\..\DataPool\GMDP_Struct_Skill.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\DataPool\GMDataPool.h"
#include "GameStruct_Skill.h"
#include "Obj_PlayerMySelf.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\WxRender\RenderSystem.h"

WX_IMPLEMENT_DYNAMIC(CObject_PlayerOther, GETCLASS(CObject_PlayerNPC));
CObject_PlayerOther::CObject_PlayerOther()
{
	m_fDebugRate	= 1.4f;
	m_pCharRace		= NULL;
}

CObject_PlayerOther::~CObject_PlayerOther()
{
}

VOID CObject_PlayerOther::Initial( VOID *pParam )
{
	CObject_PlayerNPC::Initial( pParam );

	//空手
	SetWeaponType(WEAPON_TYPE_NONE);
}

RC_RESULT CObject_PlayerOther::OnCommand(const SCommand_Object *pCmd )
{
	RC_RESULT rcResult = RC_SKIP;

	switch ( pCmd->m_wID )
	{
	case OC_UPDATE_EQUIPMENT:
		{
			UINT	dwModifyFlags	= pCmd->m_adwParam[0];
			UINT	*pData			= (UINT*)(pCmd->m_apParam[1]);
			UINT	i, dwIndex;
			dwIndex = 0;
			for ( i = 0; i < HEQUIP_NUMBER; i++ )
			{
				if ( dwModifyFlags & (1<<i) )
				{
					GetCharacterData()->Set_Equip((HUMAN_EQUIP)i, pData[dwIndex]);
					dwIndex++;
				}
			}
			rcResult = RC_OK;
		}
		break;
	case OC_UPDATE_TEAM_FLAG:
		{
			if(this != CObjectManager::GetMe()->GetMySelf())
			{
				CCharacterData* pData = GetCharacterData();
				pData->Set_HaveTeamFlag( pCmd->m_abParam[0] );
				pData->Set_TeamLeaderFlag( pCmd->m_abParam[1] );
				pData->Set_TeamFullFlag( pCmd->m_abParam[2] );
			}
			rcResult = RC_OK;
		}
		break;
	case OC_TEAM_FOLLOW_MEMBER:
		{
			CCharacterData* pData = GetCharacterData();

			pData->Set_TeamFollowListCount( pCmd->m_adwParam[0] );

			for( INT i=0; i<(INT)(pCmd->m_adwParam[0]); ++i )
			{
				pData->Set_TeamFollowedMember(i, pCmd->m_adwParam[i+1]);
			}

			rcResult = RC_OK;
		}
		break;
	case OC_UPDATE_TEAM_FOLLOW_FLAG:
		{
			CCharacterData* pData = GetCharacterData();

			pData->Set_TeamFollowFlag( pCmd->m_abParam[0] );
			CharacterLogic_GetAI()->OnAIStopped();

			rcResult = RC_OK;
		}
		break;
	default:
		return CObject_PlayerNPC::OnCommand(pCmd);
	}

	return rcResult;
}

VOID CObject_PlayerOther::OnHideWeapon(INT nAppointedWeaponID)
{
	if(nAppointedWeaponID != INVALID_ID)
	{
		EquipItem_BodyLocator( HEQUIP_WEAPON, nAppointedWeaponID);
	}
	else
	{
		//卸载装备
		UnEquipItem(HEQUIP_WEAPON);
	}
}

VOID CObject_PlayerOther::OnShowWeapon(VOID)
{
	UpdateEquip(HEQUIP_WEAPON);
}

VOID CObject_PlayerOther::CreateRenderInterface(VOID)
{
	if(m_pCharRace != NULL)
	{
		//模型定义
		DBC_DEFINEHANDLE(s_pCharModelDBC, DBC_CHARACTER_MODEL)

		LPCSTR lpszModelFileName = NULL;
		INT nModelID = GetCharModelID();
		const _DBC_CHARACTER_MODEL *pCharModel = (const _DBC_CHARACTER_MODEL*)(s_pCharModelDBC->Search_Index_EQU(nModelID));
		if(pCharModel != NULL)
		{
			lpszModelFileName = pCharModel->m_pszModelName;
		}

		if(lpszModelFileName != NULL)
		{
			m_pRenderInterface = CGameProcedure::s_pGfxSystem->New_EntityObject(tEntityNode::ETYPE_ACTOR);
			m_pRenderInterface->SetData(GetID());

			//设置ActorFile
			m_pRenderInterface->Actor_SetFile( lpszModelFileName );

			if (GetConstCharacterData()->Get_ModelID() == -1)
			{
				UpdateFaceMesh();
				UpdateHairMesh();
				UpdateHairColor();

				//重新刷新装备
				UpdateEquip( HEQUIP_WEAPON );
				UpdateEquip( HEQUIP_CAP );
				UpdateEquip( HEQUIP_ARMOR );
				UpdateEquip( HEQUIP_CUFF );
				UpdateEquip( HEQUIP_BOOT );
				UpdateEquip( HEQUIP_SASH );
			}

			SendEuipRequest();
		}
	}
	CObject_Character::CreateRenderInterface();
}

//发出请求更详细信息的请求
VOID CObject_PlayerOther::SendEuipRequest(VOID)
{
	if(GetServerID() == -1) return;

	CGCharAskEquipment msg;
	msg.setObjID((ObjID_t)GetServerID());

	CNetManager::GetMe()->SendPacket(&msg);
}

// 左键指令的分析
BOOL CObject_PlayerOther::FillMouseCommand_Left( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill )
{
	pOutCmd->m_typeMouse = SCommand_Mouse::MCT_NULL;

	//是物品
	if(pActiveSkill && pActiveSkill->GetType() == AOT_ITEM)
	{
		//鼠标上挂的是一个物品
		return CObject_PlayerNPC::FillMouseCommand_Left(pOutCmd, pActiveSkill);
	}
	else
	{
		ENUM_RELATION eCampType = CGameProcedure::s_pGameInterface->GetCampType( CObjectManager::GetMe()->GetMySelf(), this );
		switch ( eCampType )
		{
		case RELATION_FRIEND:
			{
				if ( pActiveSkill != NULL
					&& pActiveSkill->GetType() == AOT_MOUSECMD_ADDFRIEND )
				{
					pOutCmd->m_typeMouse  = SCommand_Mouse::MCT_UI_USE_ADDFRIEND;
					pOutCmd->m_adwParam[0] = GetServerID();
				}
				else 
				if ( pActiveSkill != NULL
					&& pActiveSkill->GetType() == AOT_MOUSECMD_EXCHANGE )
				{
					pOutCmd->m_typeMouse  = SCommand_Mouse::MCT_UI_USE_EXCHANGE;
					pOutCmd->m_adwParam[0] = GetServerID();
				}
				break;
			}
		case RELATION_ENEMY:
		default:
			{
				if ( pActiveSkill != NULL
					&& pActiveSkill->GetType() == AOT_SKILL )
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
	return TRUE;
}

// 右键指令的分析
BOOL CObject_PlayerOther::FillMouseCommand_Right( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill )
{
	pOutCmd->m_typeMouse = SCommand_Mouse::MCT_NULL;
	ENUM_RELATION eCampType = CGameProcedure::s_pGameInterface->GetCampType( CObjectManager::GetMe()->GetMySelf(), this );
	switch ( eCampType )
	{
	case RELATION_FRIEND:
		{
			pOutCmd->m_typeMouse	= SCommand_Mouse::MCT_CONTEXMENU;
			pOutCmd->m_adwParam[0]	= GetServerID();
		}
		break;
	case RELATION_ENEMY:
	default:
		{
			pOutCmd->m_typeMouse	= SCommand_Mouse::MCT_PLAYER_SELECT;
			pOutCmd->m_adwParam[0]	= GetServerID();
		}
		break;
	}
	return TRUE;
}

VOID CObject_PlayerOther::Debug_ChangeSpeed(FLOAT fStep)
{
	FLOAT fSpeed = GetCharacterData()->Get_MoveSpeed() + fStep;

	if(fSpeed < 0.0f) fSpeed = 0.0f;
	if(fSpeed > 10.0f) fSpeed = 10.0f;

	GetCharacterData()->Set_MoveSpeed(fSpeed);
}

VOID CObject_PlayerOther::Debug_ChangeRate(FLOAT fStep)
{
	//FLOAT fRate = m_fDebugRate + fStep;

	//if(fRate < 0.0f) fRate = 0.0f;
	//if(fRate > 3.0f) fRate = 3.0f;

	//m_fDebugRate = fRate;

	//CHAR szTemp[8];
	//_snprintf(szTemp, 8, "%.2f", m_fDebugRate);
	//if(m_pRenderInterface)
	//	m_pRenderInterface->Actor_SetObjectProperty("RunSpeed", szTemp);
}

VOID CObject_PlayerOther::OnDataChanged_Equip(HUMAN_EQUIP point)
{
	UpdateEquip(point);
}

VOID CObject_PlayerOther::UpdateFaceMesh(VOID)
{
	if(m_pRenderInterface != NULL)
	{
		INT nFaceMeshID = GetCharacterData()->Get_FaceMesh();
		if(nFaceMeshID == INVALID_ID)
		{
			nFaceMeshID = m_pCharRace->nDefHeadGeo;
		}
		if(nFaceMeshID != INVALID_ID)
		{
			//-----------------------------------------------------------
			//角色头部模型数据库
			DBC_DEFINEHANDLE(s_pCharHeadGeo, DBC_CHARACTER_HEAD_GEO);
			//查找相应记录
			const _DBC_CHAR_HEAD_GEO* pFaceGeo = (const _DBC_CHAR_HEAD_GEO* )s_pCharHeadGeo->Search_Index_EQU(nFaceMeshID);
			if(pFaceGeo != NULL)
			{
				m_pRenderInterface->Actor_SetObjectProperty(szIDSTRING_FACE_MESH, pFaceGeo->pMeshFile);
			}
			else
			{
				m_pRenderInterface->Actor_SetObjectProperty(szIDSTRING_FACE_MESH, "");
			}

			//如果是UI物体，重新设置VisibleFlag
			if(GetFakeObjectFlag())
			{
				m_pRenderInterface->Actor_SetUIVisibleFlag();
			}
		}
	}
}

VOID CObject_PlayerOther::UpdateHairMesh(VOID)
{
	if(m_pRenderInterface != NULL)
	{
		INT nHairMesh = GetCharacterData()->Get_HairMesh();
		if(nHairMesh == INVALID_ID)
		{
			nHairMesh = m_pCharRace->nDefHairGeo;
		}
		if(nHairMesh != INVALID_ID)
		{
			//-----------------------------------------------------------
			//角色头发模型数据库
			DBC_DEFINEHANDLE(s_pCharHairMesh, DBC_CHARACTER_HAIR_GEO);
			//查找相应记录
			const _DBC_CHAR_HAIR_GEO* pHairMesh = (const _DBC_CHAR_HAIR_GEO*)s_pCharHairMesh->Search_Index_EQU(nHairMesh);
			if(pHairMesh != NULL)
			{
				m_pRenderInterface->Actor_SetObjectProperty(szIDSTRING_HAIR_MESH, pHairMesh->pMeshFile);
			}
			else
			{
				m_pRenderInterface->Actor_SetObjectProperty(szIDSTRING_HAIR_MESH, "");
			}
			//如果是UI物体，重新设置VisibleFlag
			if(GetFakeObjectFlag())
			{
				m_pRenderInterface->Actor_SetUIVisibleFlag();
			}
		}
	}
}

VOID CObject_PlayerOther::UpdateHairColor(VOID)
{
	//-----------------------------------------------------------
	//渲染层设置
	if(m_pRenderInterface)
	{
		UINT uHairColor = GetCharacterData()->Get_HairColor();
		m_pRenderInterface->Actor_SetHairColor(uHairColor);
	}
}

VOID CObject_PlayerOther::UpdateEquip(HUMAN_EQUIP point)
{
	CCharacterData *pCharacterData = GetCharacterData();
	if(pCharacterData == NULL)
	{
		return ;
	}

	if(point == HEQUIP_WEAPON && IsHideWeapon())
	{
		return ;
	}

	if(!GetFakeObjectFlag())
	{
		if(pCharacterData->Get_ModelID() != INVALID_ID)
		{
			return ;
		}
	}

	//生成一个临时的Item对象
	CObject_Item_Equip *pTempEquip = NULL;
	INT nID = pCharacterData->Get_Equip(point);
	if(nID != INVALID_ID)
	{
		pTempEquip = (CObject_Item_Equip *)CObject_Item::NewItem(nID);
	}

	//如果是装备根据装配点装备物品
	if(pTempEquip != NULL && ICLASS_EQUIP == pTempEquip->GetItemClass())
	{
		switch(pTempEquip->GetItemType())
		{
		case HEQUIP_WEAPON:				//武器，挂接类装备
			{
				EquipItem_BodyLocator(pTempEquip->GetItemType(), pTempEquip->GetVisualID());
				SetWeaponType( pTempEquip->GetWeaponType() );
			}
			break;
		case HEQUIP_CAP:				//帽子, 身体, 手臂, 鞋, 装备类物品
		case HEQUIP_ARMOR:				
		case HEQUIP_CUFF:				
		case HEQUIP_BOOT:	
			{
				EquipItem_BodyPart(pTempEquip->GetItemType(), pTempEquip->GetVisualID());
			}
			break;
		case HEQUIP_SASH:				//腰带, 戒子, 项链
		case HEQUIP_RING:				
		case HEQUIP_NECKLACE:		
		default:
			break;
		}
	}
	else
	{
		//卸载装备
		UnEquipItem(point);
	}

	//如果是UI物体，重新设置VisibleFlag
	if(GetFakeObjectFlag())
	{
		m_pRenderInterface->Actor_SetUIVisibleFlag();
	}

	CObject_Item::DestroyItem(pTempEquip);
}

VOID CObject_PlayerOther::EquipItem_BodyPart(HUMAN_EQUIP nPart, INT nID)
{
	//-----------------------------------------------------------
	//渲染层设置
	if(!m_pRenderInterface)
	{
		return;
	}

	//-----------------------------------------------------------
	//角色装备类数据库
	DBC_DEFINEHANDLE(s_pEquipItem, DBC_ITEM_VISUAL_CHAR);
	const _DBC_ITEM_VISUAL_CHAR* pEquipVisual = (const _DBC_ITEM_VISUAL_CHAR*)s_pEquipItem->Search_Index_EQU(nID);
	if(!pEquipVisual)
	{
		return;
	}

	//种族(男，女)
	INT nRaceID = GetCharacterData()->Get_RaceID();
	TDAssert(nRaceID >= 0 && nRaceID<CHAR_RACE_NUM);
	if(nRaceID < 0 || nRaceID >= CHAR_RACE_NUM)
	{
		return ;
	}

	switch(nPart)
	{
	case HEQUIP_CAP:				//帽子
		{
			m_pRenderInterface->Actor_SetObjectProperty(
				szIDSTRING_CAP_MESH, pEquipVisual->pVisualEntity[nRaceID*2+0]);

			m_pRenderInterface->Actor_SetObjectProperty(
				szIDSTRING_CAP_MAT, pEquipVisual->pVisualEntity[nRaceID*2+1]);
		}
		break;
	case HEQUIP_ARMOR:				//身体
		{
			m_pRenderInterface->Actor_SetObjectProperty(
				szIDSTRING_MAINBODY_MESH, pEquipVisual->pVisualEntity[nRaceID*2+0]);

			m_pRenderInterface->Actor_SetObjectProperty(
				szIDSTRING_MAINBODY_MAT, pEquipVisual->pVisualEntity[nRaceID*2+1]);
		}
		break;
	case HEQUIP_CUFF:				//手臂
		{
			m_pRenderInterface->Actor_SetObjectProperty(
				szIDSTRING_ARM_MESH, pEquipVisual->pVisualEntity[nRaceID*2+0]);

			m_pRenderInterface->Actor_SetObjectProperty(
				szIDSTRING_ARM_MAT, pEquipVisual->pVisualEntity[nRaceID*2+1]);
		}
		break;
	case HEQUIP_BOOT:				//鞋
		{
			m_pRenderInterface->Actor_SetObjectProperty(
				szIDSTRING_FOOT_MESH, pEquipVisual->pVisualEntity[nRaceID*2+0]);

			m_pRenderInterface->Actor_SetObjectProperty(
				szIDSTRING_FOOT_MAT, pEquipVisual->pVisualEntity[nRaceID*2+1]);
		}
		break;
	}
}

VOID CObject_PlayerOther::EquipItem_BodyLocator(HUMAN_EQUIP nPart, INT nID)
{
	//-----------------------------------------------------------
	//渲染层设置
	if(!m_pRenderInterface)
	{
		return;
	}

	//-----------------------------------------------------------
	//角色挂接类数据库
	DBC_DEFINEHANDLE(s_pWeaponItem, DBC_ITEM_VISUAL_LOCATOR);
	const _DBC_ITEM_VISUAL_LOCATOR* pEquipVisual = (const _DBC_ITEM_VISUAL_LOCATOR*)s_pWeaponItem->Search_Index_EQU(nID);
	if(!pEquipVisual)
	{
		return;
	}

	//右手
	if(pEquipVisual->pObjFile_Right && pEquipVisual->pObjFile_Right[0] != '\0')
	{
		m_pRenderInterface->Actor_SetObjectProperty(szIDSTRING_CURRENT_RIGHTWEAPON, pEquipVisual->pObjFile_Right);
		m_pRenderInterface->SetRightWeaponMat(szIDSTRING_WEAPON_MAT, pEquipVisual->pMatName_Right);
		/*		
		tObject_Item* MyWeapon = CDataPool::GetMe()->UserEquip_GetItem(HEQUIP_WEAPON);

		CObject_Item_Equip::EXTRA_DEFINE* Extra = (CObject_Item_Equip::EXTRA_DEFINE*)((CObject_Item_Equip*)(CObject_Item*)MyWeapon)->GetExtraInfo();

		CObject_Item_Equip::EQUIP_ATTACH_GEM Gems = Extra->m_vEquipAttachGem;

		INT Gem_MaxLevel = 0;
		LPCTSTR szColor;

		for(INT GemIndex = 0; GemIndex < Gems.size(); GemIndex++)
		{
		UINT uGemType = Gems[GemIndex].m_GemType;

		// 从数据库中查找到这个Gem的信息
		DBC_DEFINEHANDLE(s_pItem_Gem, DBC_ITEM_GEM);
		//搜索纪录
		const _DBC_ITEM_GEM* pGem = (const _DBC_ITEM_GEM*)s_pItem_Gem->Search_Index_EQU(uGemType);
		if(!pGem) continue;

		if(pGem->nQuality > Gem_MaxLevel)
		{
		Gem_MaxLevel = pGem->nQuality;
		szColor = pGem->szColor;
		}
		}
		*/
		/////////////// code//////////////////
		UINT uGemType = GetCharacterData()->Get_EquipGem(HEQUIP_WEAPON);
		if(INVALID_ID != uGemType)
		{
			DBC_DEFINEHANDLE(s_pItem_Gem, DBC_ITEM_GEM);
			//搜索纪录
			const _DBC_ITEM_GEM* pGem = (const _DBC_ITEM_GEM*)s_pItem_Gem->Search_Index_EQU(uGemType);
			UINT nColor = 0;
			LPCTSTR szColor;
			szColor = pGem->szColor;

			INT eff_index = pGem->nEffectIndex;
			eff_index--;//表编号从1开始，对应列从0开始

			//如果策划在这列填了-1，表示不显示颜色。
			if(szColor[0] != '-' && szColor[1] != '1' && (eff_index >= 0) ) 
			{
				sscanf(szColor,"%8X",&nColor);

				if( pEquipVisual->pEffectName[eff_index] )//&& pEquipVisual->pEffectName[0] != '\0')
				{
					m_pRenderInterface->SetRightWeaponEffect(pEquipVisual->pEffectName[eff_index],nColor);
				}
			}
			else
			{
				m_pRenderInterface->SetRightWeaponEffect('\0',0);
			}
		}
		else
		{
//			if(pEquipVisual->pEffectName[eff_index] && pEquipVisual->pEffectName[0] != '\0')
//			{
				m_pRenderInterface->SetRightWeaponEffect('\0',0);
//			}
		}
	}
	else
		m_pRenderInterface->Actor_SetObjectProperty(szIDSTRING_CURRENT_RIGHTWEAPON, "");

	//左手
	if(pEquipVisual->pObjFile_Left && pEquipVisual->pObjFile_Left[0] != '\0' )
	{
		m_pRenderInterface->Actor_SetObjectProperty(szIDSTRING_CURRENT_LEFTWEAPON, pEquipVisual->pObjFile_Left);
		m_pRenderInterface->SetLeftWeaponMat(szIDSTRING_WEAPON_MAT, pEquipVisual->pMatName_Left);


		UINT uGemType = GetCharacterData()->Get_EquipGem(HEQUIP_WEAPON);
		if(INVALID_ID != uGemType)
		{
			DBC_DEFINEHANDLE(s_pItem_Gem, DBC_ITEM_GEM);
			//搜索纪录
			const _DBC_ITEM_GEM* pGem = (const _DBC_ITEM_GEM*)s_pItem_Gem->Search_Index_EQU(uGemType);
			UINT nColor = 0;
			LPCTSTR szColor;
			szColor = pGem->szColor;
			INT eff_index = pGem->nEffectIndex;
			eff_index--;//表编号从1开始，对应列从0开始

			//如果策划在这列填了-1，表示不显示颜色。
			if(szColor[0] != '-' && szColor[1] != '1' && (eff_index >= 0)) 
			{
				sscanf(szColor,"%8X",&nColor);


				if( pEquipVisual->pEffectName[eff_index] )//&& pEquipVisual->pEffectName[0] != '\0')
				{
					m_pRenderInterface->SetLeftWeaponEffect(pEquipVisual->pEffectName[eff_index],nColor);
				}
			}
			else
			{
				m_pRenderInterface->SetLeftWeaponEffect('\0',0);
			}
		}
		else
		{
//			if(pEquipVisual->pEffectName && pEquipVisual->pEffectName[0] != '\0')
//			{
				m_pRenderInterface->SetLeftWeaponEffect('\0',0);
//			}
		}
	}
	else
		m_pRenderInterface->Actor_SetObjectProperty(szIDSTRING_CURRENT_LEFTWEAPON, "");
}

VOID CObject_PlayerOther::UnEquipItem(HUMAN_EQUIP nPart)
{
	if(nPart == HEQUIP_WEAPON)
	{
		SetWeaponType(WEAPON_TYPE_NONE);
	}

	if(!m_pRenderInterface)
	{
		return;
	}

	switch(nPart)
	{
	case HEQUIP_WEAPON:
		m_pRenderInterface->Actor_SetObjectProperty(szIDSTRING_CURRENT_LEFTWEAPON, "");
		m_pRenderInterface->Actor_SetObjectProperty(szIDSTRING_CURRENT_RIGHTWEAPON, "");
		break;

	case HEQUIP_CAP:
		m_pRenderInterface->Actor_SetObjectProperty(szIDSTRING_CAP_MESH, "");
		m_pRenderInterface->Actor_SetObjectProperty(szIDSTRING_CAP_MAT, "");
		break;

	case HEQUIP_ARMOR:
		//设置缺省装备
		EquipItem_BodyPart(HEQUIP_ARMOR, m_pCharRace->nDefBody);		//衣服
		break;

	case HEQUIP_CUFF:
		//设置缺省装备
		EquipItem_BodyPart(HEQUIP_CUFF, m_pCharRace->nDefArm);		//手
		break;

	case HEQUIP_BOOT:
		//设置缺省装备
		EquipItem_BodyPart(HEQUIP_BOOT, m_pCharRace->nDefFoot);		//脚
		break;

	case HEQUIP_SASH:
	case HEQUIP_RING:
	case HEQUIP_NECKLACE:
		//Not Care...
		break;

	default:
		break;
	}
}

VOID CObject_PlayerOther::OnDataChanged_FaceMesh(VOID)
{
	UpdateFaceMesh();
}

VOID CObject_PlayerOther::OnDataChanged_HairMesh(VOID)
{
	UpdateHairMesh();
}

VOID CObject_PlayerOther::OnDataChanged_HairColor(VOID)
{
	UpdateHairColor();
}

VOID CObject_PlayerOther::OnDataChanged_EquipVer(VOID)
{
	//发送装备请求
	SendEuipRequest();
}

INT CObject_PlayerOther::CalcCharModelID( VOID )const
{
	//经过变身
	if (GetConstCharacterData()->Get_ModelID() != -1)
	{
		//直接从模型定义表中读取模型名
		return GetConstCharacterData()->Get_ModelID();
	}
	else
	{
		if ( m_pCharRace != NULL )
			return m_pCharRace->nModelID;
		else
			return INVALID_ID;
	}
}

UINT CObject_PlayerOther::GetIdleInterval(VOID)const
{
	if(m_pCharRace != NULL && m_pCharRace->nIdleInterval >= 0)
	{
		return m_pCharRace->nIdleInterval * 1000;
	}
	else
	{
		return CObject_Character::GetIdleInterval();
	}
}

VOID CObject_PlayerOther::UpdateCharRace(VOID)
{
	m_pCharRace = NULL;

	CCharacterData *pCharacterData = GetCharacterData();
	if(pCharacterData != NULL && pCharacterData->Get_RaceID() != INVALID_ID)
	{
		DBC_DEFINEHANDLE(s_pCharacterRace, DBC_CHARACTER_RACE);
		m_pCharRace = (const _DBC_CHAR_RACE*)(s_pCharacterRace->Search_Index_EQU(pCharacterData->Get_RaceID()));
	}
}

VOID CObject_PlayerOther::OnDataChanged_RaceID( VOID )
{
	CObject_Character::OnDataChanged_RaceID();
	Enable(OSF_RAY_QUERY);
}

VOID CObject_PlayerOther::OnDataChanged_FaceImage(VOID)			// 头像改变事件函数, 2006-3-9
{

}
