#include "StdAfx.h"

#include "AxTrace.h"
#include <math.h>
#include "TDUtil.h"
#include "TDSound.h"
#include "TDException.h"
#include "TDTimeSystem.h"
#include "Obj_PlayerMySelf.h"
#include "Obj_PlayerOther.h"
#include "SkillDataMgr.h"
#include "..\ObjectManager.h"
#include "..\ObjectCommandDef.h"
#include "..\TripperObj\GMTripperObj_ItemBox.h"
#include "..\TripperObj\GMTripperObj_Resource.h"
#include "..\..\GameCommand.h"
#include "..\ObjectDef.h"
#include "..\ActionSetMgr.h"
#include "..\..\Common\BuffImpactMgr.h"
#include "..\..\Common\DirectlyImpactMgr.h"
#include "..\Surface\Obj_Effect.h"
#include "..\Surface\Obj_Bullet.h"
#include "..\Surface\Obj_SkillObj.h"
#include "..\..\Global.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\DBC\GMDataBase.h"
#include "..\..\Action\GMActionSystem.h"
#include "TDDBC_Struct.h"
#include "Obj_Character.h"
#include "AI\GMAI_Base.h"
#include "AI\GMAI_MySelf.h"
#include "..\..\DataPool\GMDP_Struct_Pet.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "TDUISystem.h"
#include "..\..\DataPool\GMDataPool.h"
#include "TDProfile.h"
#include "AI\GMAICommandDef.h"
#include "..\..\NetWork\NetManager.h"
#include "CGCharIdle.h"
#include "CGCharSit.h"
#include "..\..\Sound\GMSoundSystem.h"
#include "AxProfile.h"
#include "..\GMObjectLoadQueue.h"
#include "..\..\World\WorldManager.h"
#include "..\ObjectManager.h"
#include "..\..\Sound\GMSoundSource.h"
#include "..\ObjectCommand.h"
#include "TDGameInterface.h"
#include "..\..\Procedure\GamePro_Main.h"
#include "..\..\Object\ProjTex\Obj_ProjTex.h"
#include "TDDebuger.h"
#include "..\WxRender\RenderSystem.h"

WX_IMPLEMENT_DYNAMIC(CObject_Character, GETCLASS(CObject_Dynamic));
CObject_Character::CObject_Character()
{
	m_pTheAI			= NULL;

	m_pInfoBoard		= NULL;

	m_pCharacterData	= NULL;

	m_uTime_LogicEventListTick	= 0;

	m_bJumping					= FALSE;
	
	m_pCurrentLogicCommand		= NULL;
	m_paramLogic_MagicSend.m_nSaveMagicID				= -1;
	m_paramLogic_MagicSend.m_bBeAttackEffectShowed	= FALSE;
	m_paramLogic_MagicSend.m_nActionIndex			= -1;

	//m_nIdleIntervalTime = (UINT)-1;
	//m_nLastIdleMotionTime = 0;
	m_bAnimationEnd				= FALSE;
	m_uFightStateTime			= 0;
	m_fLogic_Speed				= 1.f;

	m_pMountRenderInterface		= NULL;
	m_pCharActionSetFile		= NULL;
	m_pMountActionSetFile		= NULL;
	m_nCharModelID				= INVALID_ID;
	m_nMountModelID				= INVALID_ID;
	m_fMountAddHeight			= 0.0F;
	m_bHideWeapon				= FALSE;

	m_pWlakSound = 0;
	m_pLifeAbilitySound = 0;

	m_bIsChatMoodPlaying		= FALSE;
	m_bNeedStandUp				= FALSE;
	m_bStanding					= FALSE;

	m_bDropBox_HaveData			= FALSE;
	m_nDropBox_ItemBoxID		= INVALID_ID;
	m_DropBox_OwnerGUID			= INVALID_ID;
	m_posDropBox_CreatePos		= WORLD_POS(-1.f, -1.f);
}

CObject_Character::~CObject_Character()
{
	TDAssert(m_pCharacterData == NULL);
	TDAssert(m_pRenderInterface == NULL);
	TDAssert(m_pInfoBoard == NULL);
}

VOID CObject_Character::Initial( VOID* pParam )
{
	//AxProfile::AxProfile_PopNode("ObjInit");
	CObject_Phy::Initial(pParam);

	m_bAnimationEnd				= FALSE;
	m_uFightStateTime	= 0;

	SetCurrentLogicCommand(NULL);
	m_fLogic_Speed				= 1.f;

	SetMapPosition( fVector2( GetPosition().x, GetPosition().z ) );
	
	//创建逻辑信息
	m_pCharacterData = CDataPool::GetMe()->CharacterData_Create(this);

	//创建信息板
	//AxProfile::AxProfile_PushNode("ObjInit_CreateBoard");
	if(!GetFakeObjectFlag()&& CGameProcedure::s_pUISystem)
	{
		m_pInfoBoard = CGameProcedure::s_pUISystem->CreateCreatureBoard();
		m_pInfoBoard->SetElement_ObjId(this->GetServerID());

		m_pInfoBoard->SetElement_LeaderFlag(FALSE);
		m_pInfoBoard->SetElement_Name("");
		m_pInfoBoard->SetElement_SaleSign(FALSE);
		SetElement_Title("",_TITLE::NO_TITLE);
		m_pInfoBoard->Show(FALSE);
	}
	//AxProfile::AxProfile_PopNode("ObjInit_CreateBoard");

	m_uTime_LogicEventListTick	= CGameProcedure::s_pTimeSystem->GetTimeNow();

	Enable(OSF_VISIABLE);
	Disalbe(OSF_OUT_VISUAL_FIELD);

	m_bDropBox_HaveData			= FALSE;
	m_nDropBox_ItemBoxID		= INVALID_ID;
	m_DropBox_OwnerGUID			= INVALID_ID;
	m_posDropBox_CreatePos		= WORLD_POS(-1.f, -1.f);

	m_bHideWeapon = FALSE;
	UpdateCharRace();
	UpdateCharModel();
	UpdateMountModel();

	SObjectInit *pCharacterInit = (SObjectInit*)pParam;
	if ( pCharacterInit != NULL )
	{
		// 状态信息
		RemoveAllImpact();
		Start_Idle();
	}
}

VOID CObject_Character::Release(VOID)
{
	DeleteObjectCommand(m_pCurrentLogicCommand);
	m_pCurrentLogicCommand = NULL;

	CleanupLogicCommandList();

	if ( !m_mapImpactEffect.empty() )
	{
		SImpactEffect *pImpactEffect;
		CImpactEffectMap::iterator itCur, itEnd;
		itEnd = m_mapImpactEffect.end();
		for ( itCur = m_mapImpactEffect.begin(); itCur != itEnd; itCur++ )
		{
			pImpactEffect = (itCur->second);
			if ( pImpactEffect != NULL )
			{
				delete pImpactEffect;
			}
		}
		m_mapImpactEffect.erase( m_mapImpactEffect.begin(), itEnd );
	}

	//删除逻辑信息
	CDataPool::GetMe()->CharacterData_Destroy(this);
	m_pCharacterData = NULL;

	//从加载队列中清除
	CObjectManager::GetMe()->GetLoadQueue()->ClearTask(GetID());

	ReleaseRenderInterface();
	ReleaseMountRenderInterface();

	if(m_pInfoBoard && CGameProcedure::s_pUISystem)
	{
		m_pInfoBoard->Destroy();
		m_pInfoBoard = NULL;
	}

	m_uTime_LogicEventListTick	= 0;

	//释放声音资源
	CGameProcedure::s_pSoundSystem->Source_Destroy(m_pWlakSound);
	m_pWlakSound = 0;
	CGameProcedure::s_pSoundSystem->Source_Destroy(m_pLifeAbilitySound);
	m_pLifeAbilitySound = 0;

	Disalbe(OSF_RAY_QUERY);

	m_pCharActionSetFile		= NULL;
	m_pMountActionSetFile		= NULL;
	m_nCharModelID				= INVALID_ID;
	m_nMountModelID				= INVALID_ID;
	m_fMountAddHeight			= 0.0F;
	m_uFightStateTime		= 0;

	m_bDropBox_HaveData			= FALSE;
	m_nDropBox_ItemBoxID		= INVALID_ID;
	m_DropBox_OwnerGUID			= INVALID_ID;
	m_posDropBox_CreatePos		= WORLD_POS(-1.f, -1.f);

	CObject_Dynamic::Release();
}

VOID CObject_Character::CreateRenderInterface(VOID)
{
	if(!m_pRenderInterface) return;

	//在渲染层刷新位置
	SetPosition(GetPosition());
	Enable(OSF_VISIABLE);

	//根据是否是FakeObject作一定处理
	if(GetFakeObjectFlag())
	{
		//禁止鼠标选择
		m_pRenderInterface->SetRayQuery(FALSE); 
		//设置特殊的VisibleFlag
		m_pRenderInterface->Actor_SetUIVisibleFlag();
	}
	else
	{
		m_pRenderInterface->Attach_ProjTexture(tEntityNode::SHADOW_BLOB, TRUE, GetShadowRange());

		//设置选择优先级
		if(CT_PLAYERMYSELF == GetCharacterType()) 
			m_pRenderInterface->RayQuery_SetLevel(tEntityNode::RL_PLAYERMYSLEF);
		else if(CT_PLAYEROTHER == GetCharacterType()) 
			m_pRenderInterface->RayQuery_SetLevel(tEntityNode::RL_PLAYEROTHER);
		else if(NPC_TYPE_PET == ((CObject_PlayerNPC*)this)->GetNpcType())
			m_pRenderInterface->RayQuery_SetLevel(tEntityNode::RL_PET);
		else
			m_pRenderInterface->RayQuery_SetLevel(tEntityNode::RL_CREATURE);
	}

	//设置骨骼动画回调
	m_pRenderInterface->SetAnimationEndEvent( (FUNC_ONANIMATIONEND)(CObject_Character::_OnAnimationEnd), (DWORD)GetID() );
	m_pRenderInterface->SetAnimationCanBreakEvent( (FUNC_ONANIMATIONEND)(CObject_Character::_OnAnimationCanBreak), (DWORD)GetID() );
	m_pRenderInterface->SetAnimationHitEvent( (FUNC_ONANIMATIONEND)(CObject_Character::_OnAnimationHit), (DWORD)GetID() );
	m_pRenderInterface->SetAnimationShakeEvent( (FUNC_ONANIMATIONEND)(CObject_Character::_OnAnimationShakeEvent), (DWORD)GetID() );
}

VOID CObject_Character::ReleaseRenderInterface( VOID )
{
	if ( m_pRenderInterface != NULL )
	{
		m_pRenderInterface->Destroy();
		m_pRenderInterface = NULL;
	}
}

VOID CObject_Character::CreateMountRenderInterface(VOID)
{
	if(m_pRenderInterface != NULL)
	{
		INT nMountModelID = GetMountModelID();
		if ( nMountModelID != -1 )
		{
			LPCSTR lpszModelFileName = NULL;
			DBC_DEFINEHANDLE(s_pCharModelDBC, DBC_CHARACTER_MODEL)
			const _DBC_CHARACTER_MODEL *pCharModel = (const _DBC_CHARACTER_MODEL*)(s_pCharModelDBC->Search_Index_EQU( nMountModelID ));
			if (pCharModel != NULL)
			{
				lpszModelFileName = pCharModel->m_pszModelName;
			}

			if (lpszModelFileName != NULL)
			{
				m_pMountRenderInterface = CGameProcedure::s_pGfxSystem->New_EntityObject(tEntityNode::ETYPE_ACTOR);
				m_pMountRenderInterface->SetData(GetID());

				//设置ActorFile
				m_pMountRenderInterface->Actor_SetFile( lpszModelFileName );

				m_pMountRenderInterface->RayQuery_SetLevel(tEntityNode::RL_PLAYERMYSLEF);

				m_pMountRenderInterface->Attach_Object(m_pRenderInterface, "attach1" );
			}
		}
	}
}

VOID CObject_Character::ReleaseMountRenderInterface( VOID )
{
	if ( m_pMountRenderInterface != NULL )
	{
		m_pMountRenderInterface->Detach_Object( m_pRenderInterface );
		m_pMountRenderInterface->Destroy();
		m_pMountRenderInterface = NULL;
	}
}

VOID CObject_Character::ChangeAction(INT nSetID, FLOAT fSpeed, BOOL bLoop, FLOAT fFuseParam )
{
	SetAnimationEnd(FALSE);
	BOOL bMustDoCharAction, bMustDoMountAction;
	(m_pRenderInterface != NULL)?(bMustDoCharAction = TRUE):(bMustDoCharAction = FALSE);
	(m_pMountRenderInterface != NULL)?(bMustDoMountAction = TRUE):(bMustDoMountAction = FALSE);

	BOOL bHideWeapon = FALSE;
	INT nAppointedWeaponID = INVALID_ID;
	LPCSTR lpszCharActionName = NULL;
	if(m_pRenderInterface != NULL)
	{
		lpszCharActionName = GetCharActionNameByActionSetID(nSetID, GetWeaponType(), &bHideWeapon, &nAppointedWeaponID);
		if(lpszCharActionName == NULL)
			return;
	}

	LPCSTR lpszMountActionName = NULL;
	if(m_pMountRenderInterface != NULL)
	{
		lpszMountActionName = GetMountActionNameByActionSetID(nSetID, GetWeaponType());
		if(lpszMountActionName == NULL)
			return;
	}

	if(bHideWeapon && !m_bHideWeapon)
	{
		m_bHideWeapon = bHideWeapon;
		OnHideWeapon(nAppointedWeaponID);
	}
	else if(!bHideWeapon && m_bHideWeapon)
	{
		m_bHideWeapon = bHideWeapon;
		OnShowWeapon();
	}

	if(m_pRenderInterface != NULL)
	{
		m_pRenderInterface->Actor_EnterSkill(FALSE, lpszCharActionName, bLoop, fFuseParam);
	}

	if(m_pMountRenderInterface != NULL)
	{
		m_pMountRenderInterface->Actor_EnterSkill(FALSE, lpszMountActionName, bLoop, fFuseParam);
	}
	ChangeActionSpeed( fSpeed );
}

VOID CObject_Character::ChangeActionSpeed(FLOAT fSpeed)
{
	if(m_pRenderInterface)
	{
		m_pRenderInterface->Actor_ChangeActionRate( fSpeed );
	}
	if(m_pMountRenderInterface)
	{
		m_pMountRenderInterface->Actor_ChangeActionRate( fSpeed );
	}
}

VOID CObject_Character::UpdateTransparent(VOID)
{
	if(GetCharacterData()->Get_StealthLevel() > 0)
	{
		SetTransparent(0.6f);
	}
	else
	{
		SetTransparent(0.f);
	}
}

VOID CObject_Character::SetTransparent(FLOAT fTransparency, FLOAT fTime/* = 1.f*/)
{
	if(GetFakeObjectFlag())
	{
		return;
	}
	if(m_pRenderInterface)
	{
		m_pRenderInterface->Actor_SetTransparent(fTransparency, fTime);
	}

	if(m_pMountRenderInterface)
	{
		m_pMountRenderInterface->Actor_SetTransparent(fTransparency, fTime);
	}
}

VOID CObject_Character::ChangeImpact( INT idImpact, ObjID_t nCreatorID, BOOL bEnable, BOOL bAttrRefesh, BOOL bShowEnableEffect )
{
	// Effect
	_BUFF_IMPACT *pBuffImpact = GetBuffImpactMgr()->GetBuffImpact( idImpact );
	if ( pBuffImpact == NULL )
		return ;

	if ( bEnable )
	{
		InsertImpact( idImpact, nCreatorID, bShowEnableEffect );
	}
	else
	{
		RemoveImpact( idImpact );
	}
}

VOID CObject_Character::OnDataChanged_RaceID( VOID )
{
	UpdateCharRace();
	UpdateCharModel();

	// 渐出
	if(GetCharacterType() == CT_MONSTER)
	{
		SetTransparent(1.0f, 0.f);
		SetTransparent(0.0f, 1.f);
	}
}

VOID CObject_Character::OnDataChanged_ModelID(VOID)
{
	UpdateCharModel();
}

VOID CObject_Character::OnDataChanged_MountID( VOID )
{
	UpdateMountModel();
}

VOID CObject_Character::OnDataChanged_Name(VOID)
{
	DBC_DEFINEHANDLE(s_pSystemColorDBC, DBC_SYSTEM_COLOR);

	if(m_pInfoBoard) 
	{
		//设置
		m_pInfoBoard->SetElement_Name(GetNameWithColor().c_str());
		Tick_UpdateInfoBoard();
	}
}

STRING CObject_Character::GetNameWithColor(void)
{
	if(strlen(GetCharacterData()->Get_Name()) == 0)
	{
		return "";
	}

	DBC_DEFINEHANDLE(s_pSystemColorDBC, DBC_SYSTEM_COLOR);
	const _DBC_SYSTEM_COLOR* pNameColor = 0;
	int MeCamp = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CampData()->m_nCampID;
	int ObjCamp = this->GetCampData()->m_nCampID;
	//玩家自己
	if(CObjectManager::GetMe()->GetMySelf() == this)
	{
		pNameColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(
			_DBC_SYSTEM_COLOR::NAME_PLAYER_MYSELF);
	}
	//其他玩家
	else if(GetCharacterType() == CT_PLAYEROTHER)
	{ 
		ENUM_RELATION eCampTypeMetoIt = CalcRelationType( 
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CampData()->m_nCampID, 
			this->GetCampData()->m_nCampID,
			CGameProcedure::s_pCampDataMgr );

		ENUM_RELATION eCampTypeIttoMe = CalcRelationType( 
			this->GetCampData()->m_nCampID, 
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CampData()->m_nCampID, 
			CGameProcedure::s_pCampDataMgr );

		if ( eCampTypeMetoIt == RELATION_ENEMY && eCampTypeIttoMe == RELATION_FRIEND )//可以攻击_不还击的玩家
			pNameColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(_DBC_SYSTEM_COLOR::NANE_ISACK_NOTRETORT_PLAYER);
		else if ( eCampTypeMetoIt == RELATION_ENEMY && eCampTypeIttoMe == RELATION_ENEMY )//可以攻击_会还击的玩家
			pNameColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(_DBC_SYSTEM_COLOR::NANE_ISACK_ISTRETORT_PLAYER);
		else if ( eCampTypeMetoIt == RELATION_FRIEND && eCampTypeIttoMe == RELATION_ENEMY )//不可攻击_会还击的玩家
			pNameColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(_DBC_SYSTEM_COLOR::NANE_NOTACK_ISTRETORT_PLAYER);
		else if ( eCampTypeMetoIt == RELATION_FRIEND && eCampTypeIttoMe == RELATION_FRIEND )//不可攻击_不还击的玩家
			pNameColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(_DBC_SYSTEM_COLOR::NANE_NOTACK_NOTRETORT_PLAYER);
	}
	//npc
	else if(GetCharacterType() == CT_MONSTER)
	{
		ENUM_RELATION eCampTypeMetoIt = CalcRelationType( 
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CampData()->m_nCampID, 
			this->GetCampData()->m_nCampID, 
			CGameProcedure::s_pCampDataMgr );

		ENUM_RELATION eCampTypeIttoMe = CalcRelationType( 
			this->GetCampData()->m_nCampID, 
			CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CampData()->m_nCampID, 
			CGameProcedure::s_pCampDataMgr );

		ENUM_NPC_AI_ATTR epAIType = (ENUM_NPC_AI_ATTR)GetCharacterData()->Get_AIType();
		BOOL IsTretort, IsAckIt, IsAckMe;
		switch(epAIType)
		{
		case NPC_AI_TYPE_INVALID:
			IsTretort = FALSE,IsAckMe = FALSE,IsAckIt = FALSE;
			break;
		case NPC_AI_TYPE_SCANNPC:
			IsTretort = TRUE,IsAckMe = TRUE,IsAckIt = TRUE;
			break;
		case NPC_AI_TYPE_NOTSCANNPC:
			IsTretort = TRUE,IsAckMe = FALSE,IsAckIt = TRUE;
			break;
		case NPC_AI_TYPE_CANNOTATTACK:
			IsTretort = FALSE,IsAckMe = FALSE,IsAckIt = FALSE;
			break;
		case NPC_AI_TYPE_IS_3:
			IsTretort = FALSE,IsAckMe = FALSE,IsAckIt = FALSE;
			break;
		case NPC_AI_TYPE_IS_4:
			IsTretort = TRUE,IsAckMe = TRUE,IsAckIt = TRUE;
			break;
		case NPC_AI_TYPE_IS_5:
			IsTretort = TRUE,IsAckMe = TRUE,IsAckIt = TRUE;
			break;
		case NPC_AI_TYPE_IS_6:
			IsTretort = TRUE,IsAckMe = FALSE,IsAckIt = TRUE;
			break;
		case NPC_AI_TYPE_IS_7:
			IsTretort = FALSE,IsAckMe = FALSE,IsAckIt = TRUE;
			break;
		case NPC_AI_TYPE_IS_8:
			IsTretort = FALSE,IsAckMe = FALSE,IsAckIt = TRUE;
			break;
		case NPC_AI_TYPE_IS_9:
			IsTretort = TRUE,IsAckMe = TRUE,IsAckIt = TRUE;
			break;
		default:
			IsTretort = FALSE,IsAckMe = FALSE,IsAckIt = FALSE;
			break;	
		}
		if ( eCampTypeIttoMe == RELATION_FRIEND )
		{
			IsAckMe = FALSE;
			IsTretort = FALSE;
		}
		if ( eCampTypeMetoIt == RELATION_FRIEND )
		{
			IsAckIt = FALSE;
		}

		if		( IsAckIt && !IsTretort && !IsAckMe )//可攻击、不还击、不主动
			pNameColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(
				_DBC_SYSTEM_COLOR::NANE_ISACK_NOTRETORT_MONSTER);
		else if ( IsAckIt && IsTretort && !IsAckMe )//可攻击、会还击、不主动
			pNameColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(
				_DBC_SYSTEM_COLOR::NANE_ISACK_ISRETORT_MONSTER);
		else if ( IsAckIt && IsTretort && IsAckMe )//可攻击、会还击、会主动
			pNameColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(
				_DBC_SYSTEM_COLOR::NANE_ISACK_ISACK_MONSTER);
		else if ( !IsAckIt && IsTretort && IsAckMe )//不可攻击、会还击、会主动
			pNameColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(
				_DBC_SYSTEM_COLOR::NANE_NOTACK_ISACK_MONSTER);
		else if ( !IsAckIt && !IsTretort && !IsAckMe )//不可攻击、不还击、不主动
			pNameColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(
				_DBC_SYSTEM_COLOR::NANE_NOTACK_NOTRETORT_MONSTER);

		////是否不可攻击
		//bool bCantBeAttack = ((CObject_PlayerNPC*)this)->GetCreatureAtt() && ((((CObject_PlayerNPC*)this)->GetCreatureAtt()->nCannotBeAttack) == 1);

		////阵营
		//ENUM_RELATION eCampType = CGameProcedure::s_pGameInterface->GetCampType(
		//	CObjectManager::GetMe()->GetMySelf(), this );

		////和平
		//if(bCantBeAttack || eCampType == RELATION_FRIEND)
		//{
		//	pNameColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(
		//		_DBC_SYSTEM_COLOR::NAME_NPC_PEACE);
		//}
		//else
		//{
		//	//主动攻击
		//	if(GetCharacterData()->Get_AIType() == NPC_AI_TYPE_SCANNPC)
		//	{
		//		pNameColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(
		//			_DBC_SYSTEM_COLOR::NAME_NPC_ATTACK_ACTIVE);
		//	}
		//	//非主动攻击
		//	else if(GetCharacterData()->Get_AIType() == NPC_AI_TYPE_NOTSCANNPC)
		//	{
		//		pNameColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(
		//			_DBC_SYSTEM_COLOR::NAME_NPC_ATTACK_PASS);
		//	}
		//}

	//	//查询是不是自己的宠物，自己的宠物有单独的一个颜色设置
	//	SDATA_PET* pPet = CDataPool::GetMe()->Pet_GetPet(CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID());

	//	if(pPet)
	//	{
	//		if(this->GetServerID() == pPet->m_idServer)
	//		{
	//			pNameColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(
	//				_DBC_SYSTEM_COLOR::NANE_SELF_PET);
	//		}
	//	}
	}

	STRING strColorName;
	if(pNameColor)
	{
		//字体颜色
		if(strlen(pNameColor->strFontColor) == 6)
		{
			strColorName += "#c";
			strColorName += pNameColor->strFontColor;
		}

		//扩边颜色
		if(strlen(pNameColor->strExtenColor) == 6)
		{
			strColorName += "#e";
			strColorName += pNameColor->strExtenColor;
		}
	}

	//加上名字
	strColorName += GetCharacterData()->Get_Name();

	return strColorName;
}

VOID CObject_Character::OnDataChanged_MoveSpeed( VOID )
{
	FLOAT fSpeedRate = GetCharacterData()->Get_SpeedRate();

	if ( CHARACTER_LOGIC_MOVE == CharacterLogic_Get() )
	{
		//ChangeActionSpeed( fSpeedRate );
	}
}

VOID CObject_Character::OnDataChanged_CurTitles(VOID)
{
	if(!m_pInfoBoard) return;
	//char szInfo[]
	LPCTSTR szTitle = m_pCharacterData->Get_CurTitle();
	//if(!szTitle || strlen(szTitle)==0) return;
	if(NULL == szTitle || strlen(szTitle)==0)
	{
		SetElement_Title("",_TITLE::NO_TITLE);
		return;
	}

	INT		nTitleType = m_pCharacterData->Get_CurTitleType();
	SetElement_Title(szTitle, nTitleType);

	//更新可能开着的好友界面
	CEventSystem::GetMe()->PushEvent(GE_UPDATE_FRIEND);
}

//设置称号
VOID CObject_Character::SetElement_Title(LPCTSTR szTitle, INT nType)
{
	TDAssert(szTitle);

	if(strlen(szTitle) == 0)
	{
		m_pInfoBoard->SetElement_Title("");
		return;
	}

	DBC_DEFINEHANDLE(s_pSystemColorDBC, DBC_SYSTEM_COLOR);

	const _DBC_SYSTEM_COLOR* pTitleColor = 0;

	if(nType == _TITLE::BANGPAI_TITLE)			// 帮派称号
	{
		pTitleColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(_DBC_SYSTEM_COLOR::TITLE_BANGPAI);
	}
	else if(nType == _TITLE::NICK_TITLE)		// 玩家自己定义的称号
	{
		pTitleColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(_DBC_SYSTEM_COLOR::TITLE_NORMAL);
	}
	else if(nType == _TITLE::MOOD_TITLE)		// 玩家心情
	{
		pTitleColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(_DBC_SYSTEM_COLOR::TITLE_MOOD);
	}
	else if(nType == _TITLE::SHANGDIAN_TITLE)	// 玩家商店称号
	{
		pTitleColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(_DBC_SYSTEM_COLOR::TITLE_PLAYER_SHOP);
	}
	else
	{
		pTitleColor = (const _DBC_SYSTEM_COLOR*)s_pSystemColorDBC->Search_Index_EQU(_DBC_SYSTEM_COLOR::TITLE_NORMAL);
	}

	STRING strColorTitle;

	if(pTitleColor)
	{
		//字体颜色
		if(strlen(pTitleColor->strFontColor) == 6)
		{
			strColorTitle += "#c";
			strColorTitle += pTitleColor->strFontColor;
		}

		//扩边颜色
		if(strlen(pTitleColor->strExtenColor) == 6)
		{
			strColorTitle += "#e";
			strColorTitle += pTitleColor->strExtenColor;
		}
	}
	strColorTitle += szTitle;

	m_pInfoBoard->SetElement_Title(strColorTitle.c_str(), nType);

}

//当摆摊状态发生改变
VOID CObject_Character::OnDataChanged_IsInStall(VOID)
{
	CCharacterData *pCharacterData = GetCharacterData();
	if(pCharacterData == NULL)
		return;

	BOOL bIsInStall = pCharacterData->Get_IsInStall();

	if(m_pInfoBoard) 
	{
		m_pInfoBoard->SetElement_SaleSign(bIsInStall);
	}

	if(bIsInStall)
	{
		Start_Stall(TRUE);
	}
}
//当别人的摊位名改变时
VOID CObject_Character::OnDataChanged_StallName(VOID)
{
	if(CObjectManager::GetMe()->GetMySelf() == this )
	{//自己
		CDataPool::GetMe()->MyStallBox_SetStallName(GetCharacterData()->Get_StallName());
		//CEventSystem::GetMe()->PushEvent(GE_IMPACT_SELF_UPDATE);
	}
	if(CGameProcedure::s_pUISystem && CGameProcedure::s_pUISystem->IsWindowShow("StallBuy"))
	{
		if(GetServerID() == CDataPool::GetMe()->OtStallBox_GetObjID())
		{//别人
			CDataPool::GetMe()->OtStallBox_SetStallName(GetCharacterData()->Get_StallName());
			CEventSystem::GetMe()->PushEvent(GE_UPDATE_STALL_BUY);
		}
	}
	if(m_pInfoBoard) 
	{
		m_pInfoBoard->SetElement_SaleText(GetCharacterData()->Get_StallName());
	}
}

BOOL CObject_Character::InsertImpact( INT idImpact, ObjID_t nCreatorID, BOOL bShowEnableEffect )
{
	if ( idImpact == INVALID_ID )
	{
		//Assert( idImpact != INVALID_ID && "CObject_Character::InsertImpact" );
		return FALSE;
	}

	CImpactEffectMap::iterator itFind, itEnd;
	SImpactEffect *pImpactEffect;
	itFind = m_mapImpactEffect.find( idImpact );
	itEnd = m_mapImpactEffect.end();
	if ( itFind == itEnd )
	{
		pImpactEffect = new SImpactEffect;
		pImpactEffect->SetImpactID(idImpact);
		pImpactEffect->SetCreatorID(nCreatorID);
		m_mapImpactEffect[idImpact] = pImpactEffect;

		_BUFF_IMPACT *pBuffImpact = GetBuffImpactMgr()->GetBuffImpact( idImpact );

		if ( pBuffImpact != NULL )
		{
			BOOL bLineEffect;
			LPCSTR lpszCreatorLocator;
			bLineEffect			= pBuffImpact->m_bLineEffect;
			lpszCreatorLocator	= pBuffImpact->m_pszCreatorLocator;

			pImpactEffect->SetLineEffect(bLineEffect);
			pImpactEffect->SetCreatorLocator(lpszCreatorLocator);
			if ( bShowEnableEffect )
			{
				// 效果触发的特效（RenderInterface自动会删除）
				if ( strlen( pBuffImpact->m_lpszEffect_Active ) > 0 && m_pRenderInterface)
					m_pRenderInterface->AddEffect( pBuffImpact->m_lpszEffect_Active, pBuffImpact->m_lpszBind_Active );

				if ( strlen( pBuffImpact->m_lpszSound_Active ) > 0 )
				{
					fVector3 fvPos = GetPosition();
					//sunyu此接口是提供给fairy调用的音效接口，所以传入坐标必须是gfx坐标
					fVector3 fvGame;
					if(!CGameProcedure::s_pGfxSystem->Axis_Trans(CRenderSystem::AX_GAME, fvPos, 
						CRenderSystem::AX_GFX, fvGame))
					{
						return FALSE;
					}
					CSoundSystemFMod::_PlaySoundFunc( pBuffImpact->m_lpszSound_Active, &fvGame.x, false );
				}
			}

			// 需要我们控制的特效
			if ( strlen( pBuffImpact->m_lpszEffect_Continuous ) > 0 && m_pRenderInterface)
			{
				UINT uEffect = m_pRenderInterface->AddEffect( pBuffImpact->m_lpszEffect_Continuous, pBuffImpact->m_lpszBind_Continuous );
				pImpactEffect->SetEffect( uEffect );
				UpdateEffectTargetPos(pImpactEffect);
			}
		}
	}
	else
	{
		pImpactEffect = itFind->second;
	}

	pImpactEffect->AddRefCount();

	//更新宠物Frame
	UpdatePetFrame();
	return TRUE;
}

VOID CObject_Character::RemoveImpact( INT idImpact )
{
	CImpactEffectMap::iterator itFind, itEnd;
	SImpactEffect *pImpactEffect;
	itFind = m_mapImpactEffect.find( idImpact );
	itEnd = m_mapImpactEffect.end();
	if ( itFind != itEnd )
	{
		pImpactEffect = itFind->second;
		pImpactEffect->DecRefCount();
		if ( pImpactEffect->GetRefCount() == 0 )
		{
			UINT uEffect = pImpactEffect->GetEffect();
			if ( uEffect != 0 && m_pRenderInterface)
			{
				m_pRenderInterface->DelEffect( uEffect );
			}

			m_mapImpactEffect.erase( idImpact );
			delete pImpactEffect;
			pImpactEffect = NULL;
		}
	}

	//更新宠物Frame
	UpdatePetFrame();
}

VOID CObject_Character::RemoveAllImpact( VOID )
{
	CImpactEffectMap::iterator itCur, itEnd;
	SImpactEffect *pImpactEffect;
	itCur = m_mapImpactEffect.begin();
	itEnd = m_mapImpactEffect.end();
	while ( itCur != itEnd )
	{
		pImpactEffect = itCur->second;
		itCur++;

		UINT uEffect = pImpactEffect->GetEffect();
		if ( uEffect != 0 && m_pRenderInterface)
		{
			m_pRenderInterface->DelEffect( uEffect );
		}
		delete pImpactEffect;
		pImpactEffect = NULL;
	}
	m_mapImpactEffect.erase( m_mapImpactEffect.begin(), itEnd );

	//更新宠物Frame
	UpdatePetFrame();
}

VOID CObject_Character::UpdatePetFrame()
{
	if(!CObjectManager::GetMe()) return;
	if(!CObjectManager::GetMe()->GetMySelf()) return;
	if(!CObjectManager::GetMe()->GetMySelf()->GetCharacterData()) return;

	if(!CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID().IsNull())
	{
		PET_GUID_t pg = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID();
		if(CDataPool::GetMe()->Pet_GetPet(pg))
		{
			if((CDataPool::GetMe()->Pet_GetPet(pg))->m_idServer == GetServerID())
			{
				CEventSystem::GetMe()->PushEvent(GE_UPDATE_PET_PAGE);
			}
		}
	}
}

VOID CObject_Character::Tick_UpdateEffect( VOID )
{
	if(m_mapImpactEffect.empty())
	{
		return ;
	}

	CImpactEffectMap::iterator itCur, itEnd;
	SImpactEffect *pImpactEffect;
	itCur = m_mapImpactEffect.begin();
	itEnd = m_mapImpactEffect.end();
	while ( itCur != itEnd )
	{
		pImpactEffect = itCur->second;
		itCur++;

		if(pImpactEffect->IsLineEffect())
		{
			UpdateEffectTargetPos(pImpactEffect);
		}
	}
}

VOID CObject_Character::UpdateEffectTargetPos(const SImpactEffect *pImpactEffect)
{
	if(pImpactEffect == NULL)
		return;

	if(pImpactEffect->IsLineEffect())
	{
		UINT uEffect = pImpactEffect->GetEffect();
		if ( uEffect != 0 && m_pRenderInterface != NULL && pImpactEffect->GetCreatorID() != INVALID_ID )
		{
			CObject_Character *pCreator = (CObject_Character*)(CObjectManager::GetMe()->FindServerObject(pImpactEffect->GetCreatorID()));
			if(pCreator != NULL && g_theKernel.IsKindOf(pCreator->GetClass(), GETCLASS(CObject_Character)))
			{
				fVector3 fvPos;
				if ( pCreator->GetRenderInterface() != NULL && strlen( pImpactEffect->GetCreatorLocator() ) > 0 )
				{
					pCreator->GetRenderInterface()->Actor_GetLocator( pImpactEffect->GetCreatorLocator(), fvPos );
				}
				else
				{
					fvPos = pCreator->GetPosition();
				}
				m_pRenderInterface->Actor_SetEffectExtraTransformInfos(uEffect, fvPos);
			}
		}
	}
}

VOID CObject_Character::Tick(VOID)
{
	//物理Tick
	CObject_Phy::Tick();

	ProcessLogicCommand();

	//AI逻辑Tick
//	PROFILE_PUSH(Object_Tick_AI);
	if(m_pTheAI) m_pTheAI->Tick();

//	PROFILE_POP(Object_Tick_AI);

//	PROFILE_PUSH(Object_Tick_OBJECT);
	if ( !CharacterLogic_IsStopped() )
	{
		UINT uElapseTime = (UINT)(CGameProcedure::s_pTimeSystem->GetDeltaTime());
		uElapseTime = (UINT)((FLOAT)uElapseTime * GetLogicSpeed());
		BOOL bResult = FALSE;
		switch ( CharacterLogic_Get() )
		{
		case CHARACTER_LOGIC_IDLE:
			bResult = Tick_Idle(uElapseTime);
			break;
		case CHARACTER_LOGIC_ACTION:
			bResult = Tick_Action(uElapseTime);
			break;
		case CHARACTER_LOGIC_MOVE:
			bResult = Tick_Move(uElapseTime);
			break;
		case CHARACTER_LOGIC_SKILL_GATHER:
			bResult = Tick_MagicCharge(uElapseTime);
			break;
		case CHARACTER_LOGIC_SKILL_LEAD:
			bResult = Tick_MagicChannel(uElapseTime);
			break;
		case CHARACTER_LOGIC_SKILL_SEND:
			bResult = Tick_MagicSend(uElapseTime);
			break;
		case CHARACTER_LOGIC_ABILITY_ACTION:
			bResult = Tick_Ability(uElapseTime);
			break;
		case CHARACTER_LOGIC_DEAD:
			bResult = Tick_Dead(uElapseTime);
			break;
		case CHARACTER_LOGIC_STALL:
			bResult = Tick_Stall(uElapseTime);
			break;
		default:
			bResult = FALSE;
			break;
		}
		if(!bResult)
		{
			Start_Idle();
		}
	}
//	PROFILE_POP(Object_Tick_OBJECT);

	if(this == CObjectManager::GetMe()->GetMySelf())
	{
		//根据玩家位置同步摄像机
		CGameProcedure::s_pGfxSystem->Camera_SetLookAt(GetFootPosition());
		//根据玩家位置同步环境特效
		CGameProcedure::s_pGfxSystem->Scene_SetEnviromentInfo(GetPosition());
		//同步指示光环
		if(CGameProcedure::s_pProcMain->IsAutoRun())
		{
			CGameProcedure::s_pProcMain->GetMouseTarget()->UpdateAsCursor();
		}
	}

	//头顶信息板Tick
	Tick_UpdateInfoBoard();

	//特效刷新
	Tick_UpdateEffect();

	// 更新伤害信息
	Update_LogicEvent();

	// 战斗状态
	if(m_uFightStateTime > 0)
	{
		UINT uET = (UINT)(CGameProcedure::s_pTimeSystem->GetDeltaTime());
		if(uET > m_uFightStateTime)
		{
			m_uFightStateTime = 0;
		}
		else
		{
			m_uFightStateTime -= uET;
		}
	}
}

VOID CObject_Character::Tick_UpdateInfoBoard(VOID)
{
	if(!m_pInfoBoard) return;
	//隐藏中，不再计算
	if(!IsEnable(OSF_VISIABLE) || !GetRenderInterface()) 
	{
		m_pInfoBoard->Show(FALSE);
		return;
	}

	if(strlen(GetCharacterData()->Get_Name()) == 0)
	{
		m_pInfoBoard->Show(FALSE);
		return;
	}

	if (CharacterLogic_Get() == CHARACTER_LOGIC_DEAD && GetCharacterType() == CT_MONSTER)
	{
		m_pInfoBoard->Show(FALSE);
		return ;
	}

	//计算位置
	fVector2 fvPos = m_pInfoBoard->GetPosition();
	fVector3 fvCurPos = GetPosition();
	if ( m_pMountRenderInterface != NULL )
	{
		fvCurPos.y += m_fMountAddHeight;
	}
	BOOL bVisible;

	if(	GetCharacterType() == CT_MONSTER &&
		(((CObject_PlayerNPC*)this)->GetBoardHeight() > 0 ) )
		bVisible = GetRenderInterface()->Actor_GetInfoBoardPos(fvPos,&fvCurPos,((CObject_PlayerNPC*)this)->GetBoardHeight());
	else
		bVisible = GetRenderInterface()->Actor_GetInfoBoardPos(fvPos,&fvCurPos);

	if(!bVisible)
	{
		m_pInfoBoard->Show(FALSE);
		return;
	}

	// 设置离主角的距离
	fVector3 fMyselfPos = CObjectManager::GetMe()->GetMySelf()->GetPosition();
	FLOAT fDistance = TDU_GetDistSq( CObjectManager::GetMe()->GetMySelf()->GetPosition(), GetPosition() );

	//--for debug
	{
		if(CGameProcedure::s_pDebuger && this == CObjectManager::GetMe()->GetMainTarget())
		{
			if(m_fvInfoBoardPos != fvPos)
			{
				CGameProcedure::s_pDebuger->InfoTrack_AddPoint(
					(INT)(m_fvInfoBoardPos.x+0.5f), (INT)(m_fvInfoBoardPos.y+0.5f));
			}
		}
	}
	//--for debug

	m_fvInfoBoardPos = fvPos;

	m_pInfoBoard->SetInfoDistance( fDistance );

	if(	GetCharacterType() == CT_MONSTER &&
		(((CObject_PlayerNPC*)this)->IsDisplayBoard() == FALSE) )
		m_pInfoBoard->Show(FALSE);
	else
		m_pInfoBoard->Show(TRUE);

	m_pInfoBoard->SetPosition(m_fvInfoBoardPos.x, m_fvInfoBoardPos.y);
}

VOID CObject_Character::UpdateCharModel(VOID)
{
	INT nCharModelID = CalcCharModelID();
	if(nCharModelID != m_nCharModelID)
	{
		m_nCharModelID = nCharModelID;

		UpdateCharActionSetFile();
		ReleaseRenderInterface();
		if(m_nCharModelID != INVALID_ID)
		{
			CreateRenderInterface();
		}

		SetPosition(GetPosition());
		SetFaceDir( GetFaceDir() );
		Enable( IsEnable(OSF_VISIABLE) );
		Disalbe( IsEnable(OSF_OUT_VISUAL_FIELD) );

		UpdateTransparent();
		UpdateBuffEffect();
		// 这里可能有问题
		FreshAnimation();
		if(CObjectManager::GetMe()->GetMainTarget() == this && GetRenderInterface() != NULL)
		{
			GetRenderInterface()->Attach_ProjTexture(tEntityNode::SELECT_RING, TRUE, GetProjtexRange() );
		}
	}
}

VOID CObject_Character::UpdateMountModel(VOID)
{
	INT	nMountModelID = INVALID_ID;
	INT	npasslevel = INVALID_ID;
	m_fMountAddHeight = 0.0f;
	if(GetCharacterData() != NULL)
	{
		INT nMountID = GetCharacterData()->Get_MountID();
		if(nMountID != INVALID_ID)
		{
			DBC_DEFINEHANDLE(s_pCharMountDBC, DBC_CHARACTER_MOUNT)
			const _DBC_CHARACTER_MOUNT *pCharMount = (const _DBC_CHARACTER_MOUNT*)(s_pCharMountDBC->Search_Index_EQU(nMountID));
			if(pCharMount != NULL)
			{
				nMountModelID = pCharMount->m_nModelID;
				m_fMountAddHeight = pCharMount->m_fAddHeight;
				npasslevel = pCharMount->m_nPassLevel;
			}
		}
	}

	if(m_nMountModelID != nMountModelID)
	{
		m_nMountModelID = nMountModelID;

		UpdateCharActionSetFile();
		UpdateMountActionSetFile();
		ReleaseMountRenderInterface();
		if(m_nMountModelID != INVALID_ID)
		{
			CreateMountRenderInterface();
		}

		SetPosition(GetPosition());
		SetFaceDir( GetFaceDir() );
		Enable( IsEnable(OSF_VISIABLE) );
		Disalbe( IsEnable(OSF_OUT_VISUAL_FIELD) );

		UpdateTransparent();

		// 这里可能有问题
		if ( CharacterLogic_Get() != CHARACTER_LOGIC_DEAD
			&& CharacterLogic_Get() != CHARACTER_LOGIC_MOVE )
		{
			Start_Idle();
		}
		else
		{
			FreshAnimation();
		}
	}
}

VOID CObject_Character::UpdateCharActionSetFile(VOID)
{
	m_pCharActionSetFile	= NULL;

	if(GetCharacterData() != NULL)
	{
		INT nCharModelID	= GetCharModelID();
		INT nMountID		= GetCharacterData()->Get_MountID();

		DBC_DEFINEHANDLE(s_pCharModelDBC, DBC_CHARACTER_MODEL)
		if(nCharModelID != INVALID_ID)
		{
			const _DBC_CHARACTER_MODEL *pCharModel = (const _DBC_CHARACTER_MODEL*)(s_pCharModelDBC->Search_Index_EQU(nCharModelID));
			if (pCharModel != NULL)
			{
				const CHAR *pszActionSetFileName = NULL;
				if(nMountID >= 0 && nMountID < MAX_MOUNT_NUMBER)
				{
					if(strlen(pCharModel->m_apszActionSetName_Mount[nMountID]) > 0)
					{
						pszActionSetFileName = pCharModel->m_apszActionSetName_Mount[nMountID];
					}
				}
				else
				{
					if(strlen(pCharModel->m_pszActionSetName_None) > 0)
					{
						pszActionSetFileName = pCharModel->m_pszActionSetName_None;
					}
				}

				if(pszActionSetFileName != NULL)
				{
					m_pCharActionSetFile = GetActionSetMgr()->GetActionSetFile(pszActionSetFileName);
				}
			}
		}
	}
}

VOID CObject_Character::UpdateMountActionSetFile(VOID)
{
	m_pMountActionSetFile	= NULL;

	if(GetCharacterData() != NULL)
	{
		INT nMountModelID	= GetMountModelID();

		DBC_DEFINEHANDLE(s_pCharModelDBC, DBC_CHARACTER_MODEL)
		if(nMountModelID != INVALID_ID)
		{
			const _DBC_CHARACTER_MODEL *pMountModel = (const _DBC_CHARACTER_MODEL*)(s_pCharModelDBC->Search_Index_EQU(nMountModelID));
			if (pMountModel != NULL && strlen(pMountModel->m_pszActionSetName_None) > 0)
			{
				m_pMountActionSetFile = GetActionSetMgr()->GetActionSetFile(pMountModel->m_pszActionSetName_None);
			}
		}
	}
}

LPCSTR CObject_Character::GetCharActionNameByActionSetID(INT nActionSetID, INT nWeaponType, BOOL *pbHideWeapon, INT *pnAppointedWeaponID)
{
	INT nCalcWeaponType = nWeaponType;
	switch ( GetCharacterType() )
	{
	case CT_MONSTER:
		nCalcWeaponType = CannotBeAttack() ? WEAPON_TYPE_NPC : WEAPON_TYPE_MONSTER;
		break;
	case CT_PLAYEROTHER:
	case CT_PLAYERMYSELF:
	default:
		if(GetConstCharacterData() != NULL && GetConstCharacterData()->Get_ModelID() != INVALID_ID)
		{
			nCalcWeaponType = WEAPON_TYPE_MONSTER;
		}
		break;
	}
	if(m_pCharActionSetFile != NULL && nActionSetID != -1 && nCalcWeaponType >= 0 && nCalcWeaponType < MAX_WEAPON_TYPE_NUMBER)
	{
		const _DBC_CHARACTER_ACTION_SET *pActionSet = (const _DBC_CHARACTER_ACTION_SET*)(m_pCharActionSetFile->Search_Index_EQU(nActionSetID));
		if(pActionSet != NULL)
		{
			if(pbHideWeapon != NULL)
			{
				*pbHideWeapon = pActionSet->bHideWeapon;
			}
			if(pnAppointedWeaponID != NULL)
			{
				*pnAppointedWeaponID	= pActionSet->nAppointedWeaponID;
			}
			return pActionSet->pWeapon_Set[nCalcWeaponType];
		}
	}
	return NULL;
}

LPCSTR CObject_Character::GetMountActionNameByActionSetID(INT nActionSetID, INT nWeaponType)
{
	INT nCalcWeaponType = nWeaponType;
	switch ( GetCharacterType() )
	{
	case CT_MONSTER:
		nCalcWeaponType = CannotBeAttack() ? WEAPON_TYPE_NPC : WEAPON_TYPE_MONSTER;
		break;
	case CT_PLAYEROTHER:
	case CT_PLAYERMYSELF:
	default:
		break;
	}
	if(m_pMountActionSetFile != NULL && nActionSetID != -1 && nCalcWeaponType >= 0 && nCalcWeaponType < MAX_WEAPON_TYPE_NUMBER)
	{
		const _DBC_CHARACTER_ACTION_SET *pActionSet = (const _DBC_CHARACTER_ACTION_SET*)(m_pMountActionSetFile->Search_Index_EQU(nActionSetID));
		if(pActionSet != NULL)
		{
			return pActionSet->pWeapon_Set[nCalcWeaponType];
		}
	}
	return NULL;
}

bool CObject_Character::_OnAnimationEnd(LPCTSTR szAnimationName, UINT dwParam)
{
	//取得该Object
	CObject* pObj = (CObject*)CObjectManager::GetMe()->FindObject((INT)dwParam);

	if(pObj && g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
		//调用相应的对象函数
		return (((CObject_Character*)pObj)->OnAnimationEnd(szAnimationName))?(true):(false);
	else
		//该对象已经销毁?
		return false;
}

bool CObject_Character::_OnAnimationCanBreak(LPCTSTR szAnimationName, UINT dwParam)
{
	//取得该Object
	CObject* pObj = (CObject*)CObjectManager::GetMe()->FindObject((INT)dwParam);

	if(pObj && g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
		//调用相应的对象函数
		return (((CObject_Character*)pObj)->OnAnimationCanBreak(szAnimationName))?(true):(false);
	else
		//该对象已经销毁?
		return false;
}

bool CObject_Character::_OnAnimationHit(LPCTSTR szAnimationName, UINT dwParam)
{
	//取得该Object
	CObject* pObj = (CObject*)CObjectManager::GetMe()->FindObject((INT)dwParam);

	if(pObj && g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
		//调用相应的对象函数
		return (((CObject_Character*)pObj)->OnAnimationHit(szAnimationName))?(true):(false);
	else
		//该对象已经销毁?
		return false;
}

bool CObject_Character::_OnAnimationShakeEvent(LPCTSTR szAnimationName, UINT dwParam)
{
	//取得该Object
	CObject* pObj = (CObject*)CObjectManager::GetMe()->FindObject((INT)dwParam);

	if(pObj && g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
		//调用相应的对象函数
		return (((CObject_Character*)pObj)->OnAnimationShakeEvent(szAnimationName))?(true):(false);
	else
		//该对象已经销毁?
		return false;
}
//
////检查是否不再可见
//VOID CObject_Character::Tick_CheckAlive(VOID)
//{
//	//如果已经脱离视野
//	if(IsEnable(OSF_OUT_VISUAL_FIELD) )
//	{
//		const static UINT PLAYER_AUTO_DESTROY_TIME	=	30*1000;	//玩家2分钟没有消息传来删除
//		const static UINT    NPC_AUTO_DESTROY_TIME	=	30*1000;	//NPC20秒没有消息传来删除
//		
//		UINT nAutoDestroyTime = 0;
//		if(GetClass() == GETCLASS(CObject_PlayerOther))
//		{
//			nAutoDestroyTime = PLAYER_AUTO_DESTROY_TIME;
//		}
//		else if(GetClass() == GETCLASS(CObject_PlayerNPC))
//		{
//			nAutoDestroyTime = NPC_AUTO_DESTROY_TIME;
//		}
//		
//		//超过一定时间没有消息传来
//		UINT nLostTime = CGameProcedure::s_pTimeSystem->CalSubTime( 
//							GetMsgTime(), 
//							CGameProcedure::s_pTimeSystem->GetTimeNow());
//
//		if(nLostTime >= nAutoDestroyTime)
//		{
//			//FLOAT fDistance = TDU_GetDist(GetPosition(), pMe->GetPosition());
//
//			//CGameProcedure::s_pGfxSystem->PushDebugString("Remove NPC(%.1f)", 
//			//	fDistance);
//			//自动删除自己
//			Release();
//			CGameProcedure::s_pObjectManager->DestroyObject(this);
//			//不能再调用任何代码
//			return;
//		}
//	}
//}

//通知一个物理事件发生
VOID	CObject_Character::NotifyPhyEvent(PHY_EVENT_ID eventid, VOID* pParam)
{
	switch(eventid)
	{
	case PE_COLLISION_WITH_GROUND:
		{
			if(GetbJumping())
			{//落地了
				SetbJumping(FALSE);
				FreshAnimation();
				PhyEnable(FALSE);
				UnRegisterPhyEvent(PE_COLLISION_WITH_GROUND);
			}
		}
		break;
	default:
		break;
	}
	return;
}


BOOL CObject_Character::OnAnimationShakeEvent(LPCTSTR szAnimationName)
{
	CGameProcedure::s_pGfxSystem->Camera_VibrateOneTime();
	return TRUE;
}

STRING CObject_Character::GetDebugDesc(VOID)
{
	STRING strHistory;
	for(INT i=0; i<(INT)m_qDebugString.size(); i++)
	{
		strHistory += m_qDebugString[i];
		strHistory += "\n";
	}

	CHAR* szLogic = NULL;
	{
		switch(CharacterLogic_Get())
		{
		case CHARACTER_LOGIC_IDLE:				szLogic = "IDLE";			break;
		case CHARACTER_LOGIC_ACTION:			szLogic = "ACTION";			break;
		case CHARACTER_LOGIC_MOVE:				szLogic = "MOVE";			break;
		case CHARACTER_LOGIC_SKILL_GATHER:		szLogic = "SKILL_GATHER";	break;
		case CHARACTER_LOGIC_SKILL_LEAD:		szLogic = "SKILL_LEAD";		break;
		case CHARACTER_LOGIC_SKILL_SEND:		szLogic = "SKILL_SEND";		break;
		case CHARACTER_LOGIC_DEAD:				szLogic = "DEAD";			break;
		case CHARACTER_LOGIC_ABILITY_ACTION:	szLogic = "ABILITY_ACTION";	break;
		case CHARACTER_LOGIC_STALL:				szLogic = "STALL";			break;
		default:								szLogic = "UNKNOWN";		break;
		}
	}


	TCHAR szTemp[1024];
	_sntprintf(szTemp, 1024, 
		_T("ID:(%d) Pos:(%.1f,%.1f)\n")
		_T("Logic:(%s)\n")
		_T("HP:%.1f%%  MP:%.1f%%\n")
		_T("Board(%.2f, %.2f)")
		_T("%s"), 

		GetServerID(),
		this->GetPosition().x, this->GetPosition().z,
		szLogic,
		GetCharacterData()->Get_HPPercent()*100.0f, 
		GetCharacterData()->Get_MPPercent()*100.0f, 
		m_fvInfoBoardPos.x, m_fvInfoBoardPos.y,
		strHistory.c_str());

	return STRING(szTemp);
}

const _CAMP_DATA *CObject_Character::GetCampData(VOID) const
{
	if ( GetConstCharacterData() != NULL )
		return GetConstCharacterData()->Get_CampData();
	else
		return NULL;
}

ObjID_t CObject_Character::GetOwnerID(VOID) const
{
	if ( GetConstCharacterData() != NULL )
		return GetConstCharacterData()->Get_OwnerID();
	else
		return INVALID_ID;
}

VOID CObject_Character::OnDataChanged_TeamLeaderFlag(VOID)
{
	m_pInfoBoard->SetElement_LeaderFlag(GetCharacterData()->Get_TeamLeaderFlag());
}

VOID CObject_Character::OnDataChanged_Die(VOID)
{
	if(GetCharacterData()->IsDie())
	{
		if(m_listLogicEvent.empty())
		{
			Start_Dead(TRUE);
		}
	}
}

VOID CObject_Character::OnDataChanged_StealthLevel(VOID)
{
	UpdateTransparent();
}

VOID CObject_Character::OnDataChanged_FightState(VOID)
{
}

VOID CObject_Character::OnDataChanged_Sit(VOID)
{
	if(CharacterLogic_Get() == CHARACTER_LOGIC_IDLE && !CharacterLogic_IsStopped())
	{
		if(!m_bJumping)
		{
			if(GetCharacterData()->IsSit())
			{
				ClearAllChatMoodAction();
				ChangeAction(BASE_ACTION_N_SIT_DOWN, GetLogicSpeed(), FALSE);
			}
			else
			{
				ChangeAction(BASE_ACTION_N_SIT_STAND, GetLogicSpeed(), FALSE);
			}
		}
	}
}

VOID CObject_Character::OnDataChanged_Level(VOID)
{

}

INT CObject_Character::GetCharModelID( VOID )const
{
	return m_nCharModelID;
}

INT CObject_Character::GetMountModelID( VOID )const
{
	return m_nMountModelID;
}

INT CObject_Character::CalcCharModelID( VOID )const
{
	return INVALID_ID;
}

VOID CObject_Character::SetFightState(BOOL bSet)
{
	if(bSet)
	{
		m_uFightStateTime = FIGHT_STATE_DURATION_TIME;
	}
	else
	{
		m_uFightStateTime = 0;
	}
}

VOID CObject_Character::SetPosition(const fVector3& fvPosition) 
{
	if(m_pMountRenderInterface)
	{
		m_fvPosition = fvPosition;
		m_pMountRenderInterface->SetPosition( GetPosition() );
	}
	else
	{
		Tick_UpdateInfoBoard();
		CObject_Dynamic::SetPosition( fvPosition );
	}
}

VOID CObject_Character::SetRotation(const fVector3& fvRotation) 
{ 
	if(m_pMountRenderInterface)
	{
		m_fvRotation = fvRotation;
		m_pMountRenderInterface->SetOrientation( GetRotation() );
	}
	else
	{
		CObject_Dynamic::SetRotation( fvRotation );
	}
}

VOID CObject_Character::Enable(UINT dwFlag)
{
	CObject_Dynamic::Enable( dwFlag );

	switch(dwFlag)
	{
	case OSF_VISIABLE:
		if(m_pMountRenderInterface)m_pMountRenderInterface->SetVisible(true);
		break;

	case OSF_RAY_QUERY:
		if(m_pMountRenderInterface)m_pMountRenderInterface->SetRayQuery(true);
		break;

	default:
		break;
	}
}

VOID CObject_Character::Disalbe(UINT dwFlag)
{ 
	CObject_Dynamic::Disalbe( dwFlag );

	switch(dwFlag)
	{
	case OSF_VISIABLE:
		if(m_pMountRenderInterface)m_pMountRenderInterface->SetVisible(FALSE);
		break;

	case OSF_RAY_QUERY:
		if(m_pMountRenderInterface)m_pMountRenderInterface->SetRayQuery(FALSE);
		break;
	default:
		break;
	}
}

void CObject_Character::Say( const STRING& str )
{
	m_pInfoBoard->SetElement_PaoPaoText( str.c_str() );
}

UINT CObject_Character::CalcIdleRandInterval(VOID)
{
	UINT uInterval = GetIdleInterval();
	if(uInterval == UINT_MAX)
	{
		uInterval = UINT_MAX;
	}
	else
	{
		uInterval = (UINT)((FLOAT)(GetIdleInterval()) * ((FLOAT)(64 - (rand() % 16)))/64.f);
	}
	return uInterval;
}

BOOL CObject_Character::IsDie(VOID)const
{
	if(m_pCharacterData != NULL)
	{
		return m_pCharacterData->IsDie();
	}
	else
	{
		return FALSE;
	}
}

BOOL CObject_Character::AddDropBoxEvent(ObjID_t idItemBox, GUID_t idOwner, const WORLD_POS *pCreatePos)
{
	if(pCreatePos == NULL)
	{
		return FALSE;
	}

	m_bDropBox_HaveData		= TRUE;
	m_nDropBox_ItemBoxID	= idItemBox;
	m_DropBox_OwnerGUID		= idOwner;
	m_posDropBox_CreatePos	= *pCreatePos;
	return TRUE;
}
