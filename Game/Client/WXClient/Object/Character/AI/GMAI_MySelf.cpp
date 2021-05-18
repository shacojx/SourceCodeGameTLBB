// GMAI_MySelf.cpp

#include "StdAfx.h"

#include "..\..\..\NetWork\NetManager.h"
#include "TDUtil.h"
#include "TDTimeSystem.h"
#include "..\..\..\Event\GMEventSystem.h"
#include "..\..\..\Action\GMActionSystem.h"
#include "..\..\..\Procedure\GameProcedure.h"
#include "..\..\..\Procedure\GamePro_Main.h"
#include "..\..\..\DataPool\DetailAttrib_Player.h"
#include "..\..\..\DataPool\GMUIDataPool.h"
#include "..\..\..\Object\Character\Obj_PlayerOther.h"
#include "..\..\..\Object\ObjectManager.h"
#include "..\..\..\World\WorldManager.h"
#include "..\..\..\Cursor\CursorMng.h"

#include "SkillDataMgr.h"
#include "CGCharJump.h"
#include "CGCharMove.h"
#include "CGCharIdle.h"
#include "CGCharUseSkill.h"
#include "CGOpenItemBox.h"
#include "CGUseAbility.h"

#include "GMAICommandDef.h"
#include "..\..\..\Object\ObjectCommandDef.h"

#include "..\..\..\Object\TripperObj\GMTripperObj.h"
#include "..\..\..\Object\TripperObj\GMTripperObj_Platform.h"
#include "..\..\..\Object\TripperObj\GMTripperObj_Resource.h"
#include "TDGameInterface.h"
#include "TDCursorSystem.h"

#include "CGCharDefaultEvent.h"

#include "..\..\..\DBC\GMDataBase.h"
#include "..\..\..\Global.h"
#include "..\Obj_PlayerMySelf.h"
#include "..\..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\..\DataPool\GMDataPool.h"


#include "GMAI_MySelf.h"
#include "..\WxRender\RenderSystem.h"

#define USE_SKILL_FORBID_TIME	(500)

CAI_MySelf::CAI_MySelf(CObject_Character* pCharObj)
	: CAI_Base(pCharObj)
{
	SetMySelfAI( MYSELF_AI_IDLE );
	m_SaveCommand.Cleanup();
	m_uForbidTime		= 0;
	m_uForbidStartTime	= 0;
	StartGuaJi = 0;
}

CAI_MySelf::~CAI_MySelf( VOID )
{
}

VOID CAI_MySelf::Reset(VOID)
{
}

VOID CAI_MySelf::SetForbidTime(UINT nForbidTime)
{
	m_uForbidTime = nForbidTime;
	m_uForbidStartTime = CGameProcedure::s_pTimeSystem->GetTimeNow();
}

BOOL CAI_MySelf::Tick( VOID )
{
	if(m_uForbidTime > 0)
	{
		if(CGameProcedure::s_pTimeSystem->GetTimeNow() - m_uForbidStartTime >= m_uForbidTime)
		{
			m_uForbidTime = 0;
		}
	}
	if ( m_SaveCommand.m_wID != AIC_NONE )
	{
		if(m_uForbidTime == 0)
		{
			if ( !IsLimitCmd( &m_SaveCommand ) )
			{
				RC_RESULT rcResult = OnCommand( &m_SaveCommand );
				if ( RC_WAIT != rcResult )
				{
					m_SaveCommand.m_wID = AIC_NONE;
				}
			}
			else
			{
				m_SaveCommand.m_wID = AIC_NONE;
			}
		}
	}

	BOOL bResult;
	switch( GetMySelfAI() )
	{
	case MYSELF_AI_IDLE:
		bResult = Tick_Idle();
		break;
	case MYSELF_AI_MOVE:
		bResult = Tick_Move();
		break;
	case MYSELF_AI_USE_SKILL:
		bResult = Tick_UseSkill();
		break;
	case MYSELF_AI_ACTIVE_TRIPPEROBJ:
		bResult = Tick_ActiveTripperObj();
		break;
	case MYSELF_AI_DEFAULT_EVENT:
		bResult = Tick_DefaultEvent();
		break;
	//case MYSELF_AI_DEAD:
	//	bResult = Tick_Dead();
	//	break;
	case MYSELF_AI_FOLLOW:
		bResult = Tick_Follow();
		break;
	case MYSELF_AI_GUAJI:
		bResult = Tick_AutoHit();
		break;
	default:
		bResult = FALSE;
		break;
	}
	return bResult;
}
BOOL CAI_MySelf::PushCommand_MoveTo(FLOAT fDestX, FLOAT fDestZ)
{
	SCommand_AI cmdTemp;
	cmdTemp.m_wID			= AIC_MOVE;
	cmdTemp.m_afParam[0]	= fDestX;
	cmdTemp.m_afParam[1]	= fDestZ;
	return PushCommand(&cmdTemp);
}

BOOL CAI_MySelf::PushCommand_AutoHit(INT isAutoHit)
{
	SCommand_AI cmdTemp;
	cmdTemp.m_wID			= AIC_GUAJI;
	cmdTemp.m_adwParam[0]	= isAutoHit;
	return PushCommand(&cmdTemp);
}

BOOL CAI_MySelf::PushCommand_Jump(VOID)
{
	if(RC_OK == Jump())
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CAI_MySelf::PushCommand_UseSkill(INT idSkill, GUID_t guidTarget, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir)
{
	SCommand_AI cmdTemp;
	cmdTemp.m_wID			= AIC_USE_SKILL;
	cmdTemp.m_adwParam[0]	= (UINT)idSkill;
	cmdTemp.m_adwParam[1]	= (UINT)idTargetObj;
	cmdTemp.m_afParam[2]	= fTargetX;
	cmdTemp.m_afParam[3]	= fTargetZ;
	cmdTemp.m_afParam[4]	= fDir;
	cmdTemp.m_auParam[5]	= guidTarget;
	return PushCommand(&cmdTemp);
}

// 压入一条指令
BOOL CAI_MySelf::PushCommand( const SCommand_AI *pCmd )
{
	// 先清空，不管此次操作是否为空
	m_SaveCommand.m_wID = AIC_NONE;
	if ( !IsLimitCmd( pCmd ) )
	{
		// 先将pCmd缓存起来
		m_SaveCommand = *pCmd;

		if(m_uForbidTime == 0)
		{
			RC_RESULT rcResult = OnCommand( pCmd );
			if (rcResult != RC_WAIT)
			{// 如果执行成功则将缓存清空
				m_SaveCommand.m_wID = AIC_NONE;
			}
		}
	}
	return TRUE;
}

RC_RESULT CAI_MySelf::OnCommand( const SCommand_AI *pCmd )
{
	RC_RESULT rcResult;

	//------------------------------------------------
	//限制摆摊状态下不能进行的活动
	if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你正在摆摊……");
		return RC_OK;
	}

	switch ( pCmd->m_wID )
	{
	case AIC_MOVE:
		{
			fVector2 fv2TargetPos;
			fv2TargetPos.x	= pCmd->m_afParam[0];
			fv2TargetPos.y	= pCmd->m_afParam[1];
			rcResult = Enter_Move( fv2TargetPos.x, fv2TargetPos.y );
		}
		break;

	case AIC_USE_SKILL:
		{
			UINT dwTargetID, dwSkillID, dwSkillLevel;
			WORLD_POS posTarget;
			FLOAT fDir;
			GUID_t guidTarget;
			dwSkillID		= pCmd->m_adwParam[0];
			dwSkillLevel	= pCmd->m_adwParam[1];
			dwTargetID		= pCmd->m_adwParam[2];
			posTarget.m_fX	= pCmd->m_afParam[3];
			posTarget.m_fZ	= pCmd->m_afParam[4];
			fDir			= pCmd->m_afParam[5];
			guidTarget		= pCmd->m_auParam[6];
			rcResult = Enter_UseSkill( dwSkillID, dwSkillLevel, guidTarget, dwTargetID, posTarget.m_fX, posTarget.m_fZ, fDir );
		}
		break;

	case AIC_TRIPPER_ACTIVE:
		{
			INT	idItemBox;
			idItemBox	= pCmd->m_adwParam[0];
			rcResult = Enter_ActiveTripperObj( idItemBox );
		}
		break;

	case AIC_COMPOSE_ITEM:
		{
			INT	idPrescription;
			idPrescription	= pCmd->m_adwParam[0];
			rcResult = ComposeItem( idPrescription );
		}
		break;

	case AIC_DEFAULT_EVENT:
		{
			UINT dwTargetID;
			dwTargetID		= pCmd->m_adwParam[0];
			rcResult = Enter_DefaultEvent( dwTargetID );
		}
		break;

	//case AIC_DIE:
	//	{
	//		rcResult = Enter_Dead( );
	//	}
	//	break;

	case AIC_JUMP:
		{
			rcResult = Jump( );
		}
		break;

	case AIC_FOLLOW:
		{
			ObjID_t idTargetObj;
			idTargetObj = pCmd->m_adwParam[0];
			BOOL bResult = Enter_Follow(idTargetObj);
			if ( bResult )
			{
				rcResult = RC_OK;
			}
			else
			{
				rcResult = RC_ERROR;
			}
		}
		break;
	case AIC_GUAJI:
		{
			if (pCmd->m_adwParam[0])
			{
				SetMySelfAI( MYSELF_AI_GUAJI );
				StartGuaJi = 1;
			}else
			{
				SetMySelfAI( MYSELF_AI_IDLE );
				StartGuaJi = 0;
				CObjectManager::GetMe()->m_pNearestTargetOLD = NULL;
			}
			rcResult = RC_OK;
		}
		break;
	default:
		rcResult = RC_SKIP;
		break;
	};
	return rcResult;
}

RC_RESULT CAI_MySelf::AI_MoveTo( FLOAT fDestX, FLOAT fDestZ )
{
	CObject_Character *pCharacter = GetCharacter();
	CCharacterData* pCharData = pCharacter->GetCharacterData();
	if (pCharData->IsLimitMove())
		return RC_ERROR;

	if ( pCharData->IsDie() )
		return RC_ERROR;

	//if ( !pCharacter->CharacterLogic_IsStopped()
	//	 && pCharacter->CharacterLogic_Get() == CObject_Character::CHARACTER_LOGIC_SKILL_SEND )
	//{
	//	//向服务器发送请求消息
	//	CGCharIdle msg;
	//	CNetManager::GetMe()->SendPacket( &msg );
	//	
	//	//pCharacter->CharacterLogic_OnStopped();
	//	
	//	return RC_WAIT;
	//}

	CPath pathFind;
	fVector2 fvCurPos, fvTarPos;
	fvCurPos.x	= pCharacter->GetPosition().x;
	fvCurPos.y	= pCharacter->GetPosition().z;
	fvTarPos.x	= fDestX;
	fvTarPos.y	= fDestZ;
	
	BOOL bResult;
	if(	CGameProcedure::s_pProcMain->IsAutoRun() )
	{
	
		if(CPath::IsPointInUnreachRegion(fvCurPos))
		{
			bResult = FALSE;
		}
		else
		{
			bResult = (BOOL)(pathFind.GetPathDirect( fvCurPos, fvTarPos ));
		}
	}
	else
	{
		bResult = (BOOL)(pathFind.CreateMovePath( fvCurPos, fvTarPos ));
		if(!bResult)
		{
			 if(CPath::IsPointInUnreachRegion(fvCurPos))
			 {
				bResult = FALSE;
			 }
			 else
			 {
				bResult = (BOOL)(pathFind.GetPathDirect( fvCurPos, fvTarPos ));//
			 }
		}
	}

	if ( !bResult )
		return RC_ERROR;

	static WORLD_POS aPathNode[1024];
	UINT dwNumPathNode = (UINT)(pathFind.GetUnitNum());
	if ( dwNumPathNode == 0 )
		return RC_ERROR;

	UINT i;
	for ( i = 0; i < dwNumPathNode; i++ )
	{
		const CPath::PathUnit &puTemp = pathFind.GetConstPathUnit( i );

		aPathNode[i].m_fX	= puTemp.fvTarget.x;
		aPathNode[i].m_fZ	= puTemp.fvTarget.y;
	}

	CObject_PlayerMySelf *pMySelf = (CObject_PlayerMySelf*)pCharacter;
	INT nSimulantLogicCount = pMySelf->GetSimulantLogicCount();
	if(pMySelf->GetLogicCount() > nSimulantLogicCount)
	{
		nSimulantLogicCount = pMySelf->GetLogicCount();
	}
	INT nLogicCount = nSimulantLogicCount + DEF_CLIENT_LOGIC_COUNT_ADD_STEP;
	pMySelf->SetSimulantLogicCount(nLogicCount);

	SCommand_Object cmd;
	cmd.m_wID			= OC_MOVE;
	cmd.m_auParam[0]	= 0;
	cmd.m_anParam[1]	= nLogicCount;
	cmd.m_anParam[2]	= 1;//(INT)(dwNumPathNode);
	cmd.m_apParam[3]	= aPathNode;
	RC_RESULT rcResult = pCharacter->DoSimulantCommand(&cmd);
	if(rcResult != RC_OK)
		return rcResult;

	SendMoveMessage( &pathFind, nLogicCount );
	return RC_OK;
}

BOOL CAI_MySelf::IsCanJump( VOID )
{
	CObject_Character *pCharacter = GetCharacter();
	CCharacterData* pCharData = pCharacter->GetCharacterData();
	if ( pCharData->IsLimitMove() )
		return FALSE;

	if(pCharData->IsDie())
		return FALSE;

	if(pCharacter->GetbJumping())
		return FALSE;

	BOOL bCharacterLogicCanJump = FALSE;
	CObject_Character::ENUM_CHARACTER_LOGIC eCharacterLogic = pCharacter->CharacterLogic_Get();
	switch(eCharacterLogic)
	{
	case CObject_Character::CHARACTER_LOGIC_IDLE:
	case CObject_Character::CHARACTER_LOGIC_MOVE:
	case CObject_Character::CHARACTER_LOGIC_SKILL_GATHER:
	case CObject_Character::CHARACTER_LOGIC_SKILL_LEAD:
	case CObject_Character::CHARACTER_LOGIC_ABILITY_ACTION:
		bCharacterLogicCanJump = TRUE;
		break;
	case CObject_Character::CHARACTER_LOGIC_ACTION:
	case CObject_Character::CHARACTER_LOGIC_SKILL_SEND:
	case CObject_Character::CHARACTER_LOGIC_DEAD:
	case CObject_Character::CHARACTER_LOGIC_STALL:
	default:
		bCharacterLogicCanJump = FALSE;
		break;
	}
	if(!bCharacterLogicCanJump)
		return FALSE;

	return TRUE;
}

RC_RESULT CAI_MySelf::Enter_Idle( VOID )
{
	CObject_Character *pCharacter = GetCharacter();

	SCommand_Object cmd;
	cmd.m_wID			= OC_IDLE;
	cmd.m_afParam[0]	= pCharacter->GetPosition().x;
	cmd.m_afParam[1]	= pCharacter->GetPosition().z;

	pCharacter->DoSimulantCommand(&cmd);
	//RC_RESULT rcResult = pCharacter->DoCommand(&cmd);
	//if ( rcResult != RC_OK )
	//	return RC_ERROR;

	if ( MYSELF_AI_IDLE != GetMySelfAI() )
	{
		SetMySelfAI( MYSELF_AI_IDLE );
	}
	return RC_OK;
}

RC_RESULT CAI_MySelf::Jump( VOID )
{
	// 监测现在是否可以JUMP
	if ( IsCanJump() )
	{
		CObject_Character *pCharacter = GetCharacter();
		BOOL bResult = pCharacter->Jump();
		if (!bResult)
			return RC_ERROR;

		// send jump msg to server.
		CGCharJump msg;
		msg.setObjID( pCharacter->GetServerID() );
		
		CNetManager::GetMe()->SendPacket( &msg );
		

		return RC_OK;
	}

	return RC_ERROR;
}

RC_RESULT CAI_MySelf::Enter_Move( FLOAT fDestX, FLOAT fDestZ )
{
	static UINT uSaveTime = 0;
	UINT uTimeNow = CGameProcedure::s_pTimeSystem->GetTimeNow();
	UINT uSleepTime = 500;
	//if(CGameProcedure::s_pProcMain->IsAutoRun())
	//{
	//	uSleepTime = 500;
	//}
	if(uSaveTime + uSleepTime > uTimeNow)
	{
		return RC_WAIT;
	}

	//if ( GetMySelfAI() == MYSELF_AI_DEAD )
	//	return RC_ERROR;

	if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"摆摊时不能移动");
		return RC_ERROR;
	}
	
	CObject_Character *pCharacter = GetCharacter();
	CCharacterData* pCharData = pCharacter->GetCharacterData();
	if ( pCharData->IsLimitMove() )
	{
		return RC_ERROR;
	}

	if ( pCharData->IsDie() )
		return RC_ERROR;

	RC_RESULT rcResult = AI_MoveTo( fDestX, fDestZ );
	if ( rcResult != RC_OK )
	{
		return rcResult;
	}

	m_paramAI_Move.m_posTarget.m_fX	= fDestX;
	m_paramAI_Move.m_posTarget.m_fZ	= fDestZ;
	if ( MYSELF_AI_MOVE != GetMySelfAI() )
	{
		SetMySelfAI( MYSELF_AI_MOVE );
	}

	uSaveTime = uTimeNow;
	return RC_OK;
}

RC_RESULT CAI_MySelf::Enter_UseSkill( INT idSkill, INT nLevel, GUID_t guidTarget, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir )
{
	static UINT uSaveTime = 0;
	UINT uTimeNow = CGameProcedure::s_pTimeSystem->GetTimeNow();
	UINT uSleepTime = 500;
	if(uSaveTime + uSleepTime > uTimeNow)
	{
		return RC_WAIT;
	}

	//限制摆摊的时候人物不能使用技能
	if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"摆摊时不能使用技能");
		return RC_ERROR;
	}

	CObject_Character *pCharacter = GetCharacter();
	CCharacterData* pCharData = pCharacter->GetCharacterData();
	//if ( pCharData->IsLimitUseSkill() )
	//	return RC_ERROR;

	BOOL bNeedMove = FALSE;
	RC_RESULT rcResult = IsCanUseSkill( idSkill, nLevel, guidTarget, idTargetObj, fTargetX, fTargetZ, fDir, bNeedMove );
	if (rcResult != RC_OK)
		return rcResult;

	if (!bNeedMove)
	{// 就在攻击范围内
		SetForbidTime(USE_SKILL_FORBID_TIME);
		SendUseSkillMessage( idSkill, nLevel, guidTarget, idTargetObj, fTargetX, fTargetZ, fDir );
	}
	else
	{
		SetMySelfAI( MYSELF_AI_USE_SKILL );
	}

	uSaveTime = uTimeNow;
	return RC_OK;
}

RC_RESULT CAI_MySelf::Enter_ActiveTripperObj( INT idTripperObj )
{
	static UINT uSaveTime = 0;
	UINT uTimeNow = CGameProcedure::s_pTimeSystem->GetTimeNow();
	UINT uSleepTime = 500;
	if(uSaveTime + uSleepTime > uTimeNow)
	{
		return RC_WAIT;
	}

	//if ( GetMySelfAI() == MYSELF_AI_DEAD )
	//	return RC_ERROR;

	CObject_Character *pCharacter = GetCharacter();
	CCharacterData* pCharData = pCharacter->GetCharacterData();
	//if ( pCharData->IsLimitHandle() )
	//	return RC_ERROR;
	if ( pCharData->IsDie() )
		return RC_ERROR;


	if ( pCharacter->CharacterLogic_Get() == CObject_Character::CHARACTER_LOGIC_DEAD )
		return RC_ERROR;

	// 判断物体是否存在
	fVector3 fvTarget;
	CObject *pTarget = (CObject*)((CObjectManager::GetMe())->FindServerObject( idTripperObj ));
	if ( pTarget == NULL )
		return RC_ERROR;

//	if(g_theKernel.IsKindOf(pTarget->GetClass(), GETCLASS(CTripperObject_Resource)))
	// 是否是tripperobj_resource
	if( tObject::TOT_RESOURCE == pTarget->Tripper_GetType() ) 
	{
		if(! ((CTripperObject_Resource*)(CTripperObject*)pTarget)->Resource_CanOperation() )
			return RC_ERROR;
	}

	fvTarget = pTarget->GetPosition();

	FLOAT fOperationDistance;
//	if ( fDistSq > MAX_OPEN_ITEMBOX_DIST_SQ )
	if(	tObject::TOT_RESOURCE == pTarget->Tripper_GetType() )
	{
		CTripperObject_Resource *pResource = (CTripperObject_Resource*)(CTripperObject*)pTarget;
		fOperationDistance = pResource->Resource_GetOperationDistance();
	}
	else
	{
		fOperationDistance = MAX_OPEN_ITEMBOX_DIST;
	}

	BOOL bCanDo = TRUE;
	if(fOperationDistance > 0.f)
	{
		FLOAT fOperationDistanceSq = fOperationDistance * fOperationDistance;
		FLOAT fDistSq = TDU_GetDistSq( fvTarget, pCharacter->GetPosition() );
		if(fOperationDistanceSq < fDistSq)
		{
			bCanDo = FALSE;
		}
	}

	if(bCanDo)
	{
		SendActiveTripperObjMessage( idTripperObj );
		//Enter_Idle();
		CGameProcedure::s_pGfxSystem->PushDebugString("CGCharTripperObj[%d]", idTripperObj);
	}
	else
	{
		//如果是钓鱼的话，就不用走到河里去了。
		//但是也需要距离合适，如果距离不够，也不能开始钓。
		if(	tObject::TOT_RESOURCE == pTarget->Tripper_GetType() && ((CTripperObject_Resource*)pTarget)->Resource_IsFish())
		{
			ADDTALKMSG("距离太远！");
			return RC_ERROR;
		}
		else
		{
			fVector3 fvTargetPos = GetTargetPos(pCharacter->GetPosition(), fvTarget, fOperationDistance);
			RC_RESULT rcResult = AI_MoveTo( fvTargetPos.x, fvTargetPos.z );
			if ( rcResult != RC_OK )
				return rcResult;

			// 参数付值
			m_paramAI_ActiveTripperObj.m_idTripperObj	= idTripperObj;
			SetMySelfAI( MYSELF_AI_ACTIVE_TRIPPEROBJ );
			CGameProcedure::s_pGfxSystem->PushDebugString("CGCharTripperObj[%d]", idTripperObj);
		}
	}

	uSaveTime = uTimeNow;
	return RC_OK;
}

RC_RESULT CAI_MySelf::ComposeItem( INT idPrescription )
{
	CObject_Character *pCharacter = GetCharacter();
	CCharacterData* pCharData = pCharacter->GetCharacterData();
	//if ( pCharData->IsLimitHandle() )
	//	return RC_ERROR;

	if ( pCharData->IsDie() )
		return RC_ERROR;

	if ( pCharacter->CharacterLogic_Get() == CObject_Character::CHARACTER_LOGIC_DEAD )
		return RC_ERROR;

	INT guidPlatform = INVALID_ID;
	INT find_platform = 0;
	//此处应检查是否满足合成之条件，如原材料，技能，配方，平台等 
///////////////////////////
	static const tDataBase* pPrescrDBC = CDataBaseSystem::GetMe()->GetDataBase(DBC_LIFEABILITY_ITEMCOMPOSE);
	const _DBC_LIFEABILITY_ITEMCOMPOSE* m_pPrescrDef = (const _DBC_LIFEABILITY_ITEMCOMPOSE*)pPrescrDBC->Search_Index_EQU(idPrescription);

	INT idAbility	=	m_pPrescrDef->nLifeAbility;

	static const tDataBase* pLifeAbilityDBC = CDataBaseSystem::GetMe()->GetDataBase(DBC_LIFEABILITY_DEFINE);
	const _DBC_LIFEABILITY_DEFINE* m_pAbilityDef = (const _DBC_LIFEABILITY_DEFINE*)pLifeAbilityDBC->Search_Index_EQU(idAbility);

	INT idPlatform	= m_pAbilityDef->nPlatformID;
	FLOAT distPlatform	= m_pAbilityDef->fPlatformDist;

	INT my_level =  CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Level();
	if( my_level < m_pAbilityDef->nLevelNeed ) 
	{
		ADDTALKMSG("人物等级不够");
		return RC_ERROR;
	}

	const SCLIENT_LIFEABILITY* player_ability = 
		CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_LifeAbility(idAbility);
	INT player_ability_level = player_ability->m_nLevel;
	if( player_ability_level < m_pPrescrDef->nLifeAbility_Level) 
	{
		ADDTALKMSG("技能等级不够");
		return RC_ERROR;
	}
	if(idPlatform>=0)
	{
		guidPlatform = Find_Platform(idAbility);
		if(guidPlatform == INVALID_ID)
			return RC_ERROR;
	}
	// 站在原地即可开始合成，无需移动 

	// 参数赋值
	SendComposeItemMessage( idAbility, idPrescription, guidPlatform);
	CGameProcedure::s_pGfxSystem->PushDebugString("CGCharComposeItem[%d],[%d],[%d]", idAbility, idPrescription, guidPlatform);
	return RC_OK;

}
INT CAI_MySelf::Find_Platform( INT idAbility )
{
	INT guidPlatform = INVALID_ID;
	INT find_platform = 0;
	CObject_Character *pCharacter = GetCharacter();

	static const tDataBase* pLifeAbilityDBC = CDataBaseSystem::GetMe()->GetDataBase(DBC_LIFEABILITY_DEFINE);
	const _DBC_LIFEABILITY_DEFINE* m_pAbilityDef = (const _DBC_LIFEABILITY_DEFINE*)pLifeAbilityDBC->Search_Index_EQU(idAbility);

	INT idPlatform	= m_pAbilityDef->nPlatformID;
	FLOAT distPlatform	= m_pAbilityDef->fPlatformDist;

	INT my_level =  CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Level();
	if( my_level < m_pAbilityDef->nLevelNeed ) 
	{
		ADDTALKMSG("人物等级不够。");
		return INVALID_ID;
	}

	if(idPlatform >=0)
	{
		CScene* pActiveScene = (CScene*)CWorldManager::GetMe()->GetActiveScene();
		if(!pActiveScene) return FALSE;

		fVector3 fvTarget,fvPlayerMySelf;

		fvPlayerMySelf	= pCharacter->GetPosition();

		INT zone_x = pActiveScene->GetZoneX(fvPlayerMySelf.x);
		INT zone_z = pActiveScene->GetZoneX(fvPlayerMySelf.z);
		CZone* pZone;

		INT final_x = zone_x + 1;
		INT final_z = zone_z + 1;
		//在当前zone和周围8个zone之中，寻找这个platform
		for( zone_x = final_x - 2; zone_x < final_x; zone_x++ )
			for( zone_z=final_z - 2; zone_z < final_z; zone_z++ )
                {
	
					pZone = pActiveScene->GetZone(zone_x,zone_z);
					if(!pZone) continue;

					FLOAT fDistSq = 0.0f;
					for (std::set<INT>::iterator it = pZone->m_setRegisterObject.begin();it!=pZone->m_setRegisterObject.end();it++)
					{		
						CObject *pTarget = (CObject*)(CObjectManager::GetMe()->FindObject((INT)*it));
						if(!pTarget) 
						{
							continue;
						}
						if(  ( tObject::TOT_PLATFORM == pTarget->Tripper_GetType() ) && ( idPlatform == ((CTripperObject_Platform*)(CTripperObject*)pTarget)->GetPlatformID() ) )
						{
							fvTarget	= pTarget->GetPosition();
							fDistSq		= TDU_GetDistSq( fvTarget, fvPlayerMySelf );
							if ( fDistSq < distPlatform ) 
							{
								guidPlatform = pTarget->GetServerID();
								find_platform = 1;				
								break;
							}
						}
					}
					//此处应检查是否满足合成之条件，如原材料，技能，配方，平台等 
					//
					//距离不够，不会走到平台前
					//if ( fDistSq > distPlatform )
					//{

					//	BOOL bResult = MoveTo( fvTarget.x, fvTarget.z );
					//	if ( !bResult )
					//		return FALSE;
					//}
				}
				if(!find_platform) 
				{
					ADDTALKMSG("附近没有操作平台。");
					return INVALID_ID;
				}
	}
	return guidPlatform;

}

RC_RESULT CAI_MySelf::Enter_DefaultEvent( INT idTargetObj )
{
	static UINT uSaveTime = 0;
	UINT uTimeNow = CGameProcedure::s_pTimeSystem->GetTimeNow();
	UINT uSleepTime = 500;
	if(uSaveTime + uSleepTime > uTimeNow)
	{
		return RC_WAIT;
	}

	CObject_Character *pCharacter = GetCharacter();
	CCharacterData* pCharData = pCharacter->GetCharacterData();
	if ( pCharData->IsDie() )
		return RC_ERROR;

	if ( pCharacter->CharacterLogic_Get() == CObject_Character::CHARACTER_LOGIC_DEAD )
		return RC_ERROR;


	fVector3 fvTarget;
	CObject *pTarget = (CObject*)((CObjectManager::GetMe())->FindServerObject( idTargetObj ));
	if ( pTarget == NULL )
		return RC_ERROR;

	fVector3 fvTargetPos = pTarget->GetPosition();

	FLOAT fDistSq = TDU_GetDistSq( fvTargetPos, GetCharacter()->GetPosition() );
	if (fDistSq <= MAX_CLIENT_DEFAULT_EVENT_DIST_SQ)
	{
		SendDefaultEventMessage(idTargetObj);

		// 如果是任务NPC，设置其自身的方向
		if(!g_theKernel.IsKindOf(pTarget->GetClass(), GETCLASS(CObject_PlayerOther)))
		{
			if ( ((CObject_PlayerNPC*)pTarget)->IsCanFaceToPlayer() )
			{// 判断该NPC是否可以转向
				((CObject_PlayerNPC*)pTarget)->SetNeedFaceToPlayer(TRUE);
			}
		}
	}
	else
	{
		// 参数付值
		m_paramAI_DefaultEvent.m_idTargetObj	= idTargetObj;
		m_paramAI_DefaultEvent.m_posPrevTarget  = fvTargetPos;
		SetMySelfAI( MYSELF_AI_DEFAULT_EVENT );
	}

	uSaveTime = uTimeNow;
	return RC_OK;
}
//
//RC_RESULT CAI_MySelf::Enter_Dead( VOID )
//{
//	SetMySelfAI( MYSELF_AI_DEAD );
//	return RC_OK;
//}

BOOL CAI_MySelf::Enter_Follow( ObjID_t idTargetObj )
{
	// 如果不符合跟随条件 return FALSE;
	if ( IsCanFollow() == FALSE )
	{
		return FALSE;
	}

	if ( GetMySelfAI() == MYSELF_AI_FOLLOW
	  && m_paramAI_Follow.m_idTargetObj == idTargetObj
	  )
	{ // 不重复对一个对象进行跟随操作
		return TRUE;
	}

	SetMySelfAI( MYSELF_AI_FOLLOW );

	CObject* pObj = (CObject*)CObjectManager::GetMe()->GetMySelf();

	m_paramAI_Follow.m_nTickCount = m_paramAI_Follow.m_nMaxTickCount;
	m_paramAI_Follow.m_idTargetObj = idTargetObj;
	m_paramAI_Follow.m_LastPos = pObj->GetPosition();

	if ( Tick_Follow( ) )
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "进入跟随状态。");
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CAI_MySelf::Tick_Idle( VOID )
{
	if (StartGuaJi)
	{
		Tick_AutoHit();
	}
	return TRUE;
}

BOOL CAI_MySelf::Tick_Move( VOID )
{
	CObject_Character *pCharacter = GetCharacter();

	if(pCharacter->CharacterLogic_Get() != CObject_Character::CHARACTER_LOGIC_MOVE || pCharacter->CharacterLogic_IsStopped())
	{
		//OnAIStopped( );
		SetMySelfAI(MYSELF_AI_IDLE);
	}

	return TRUE;
}

BOOL CAI_MySelf::Tick_UseSkill( VOID )
{
	const SSkillData *pSkillData = GetSkillDataMgr()->GetSkillData( m_paramAI_UseSkill.m_idSkill );
	if ( pSkillData == NULL )
		return FALSE;

	FLOAT fMaxSkillRange		= pSkillData->m_fMaxAttackRange;
	FLOAT fMoveRange			= fMaxSkillRange;
	FLOAT fMoveRangeForecast	= 1.f;

	fMoveRange -= fMoveRangeForecast;
	if ( fMoveRange < 0.f )
		fMoveRange = 0.f;

	CObject_Character *pCharacter = GetCharacter();

	// 对单体
	fVector3 fvTarget;
	if ( pSkillData->m_fDamageRange <= 0.f )
	{
		CObject_Character* pTargetObj = (CObject_Character*)(CObjectManager::GetMe()->FindServerObject(m_paramAI_UseSkill.m_idTarget));
		if ( pTargetObj == NULL ||
			pTargetObj->CharacterLogic_Get() == CObject_Character::CHARACTER_LOGIC_DEAD)
		{
			OnAIStopped( );
			return FALSE;
		}
		fvTarget = pTargetObj->GetPosition();
	}
	else
	{
		fvTarget = m_paramAI_UseSkill.m_posTarget;
	}

	FLOAT fDist = TDU_GetDist( fvTarget, m_paramAI_UseSkill.m_posPrevTarget );
	if ( fDist > fMoveRange+1.f )
	{
        // 设置移动到的目标位置
		fVector3 fvTargetPos = GetTargetPos(pCharacter->GetPosition(), fvTarget, fMoveRange);
		m_paramAI_UseSkill.m_posPrevTarget = fvTargetPos;

		RC_RESULT rcResult = AI_MoveTo( fvTargetPos.x, fvTargetPos.z );
		if ( rcResult == RC_ERROR )
		{
			OnAIStopped( );
			return FALSE;
		}
	}
	else if (pCharacter->CharacterLogic_Get() == CObject_Character::CHARACTER_LOGIC_IDLE)
	{
		FLOAT fMTDist = TDU_GetDist(pCharacter->GetPosition(), fvTarget);
		if (fMTDist > fMaxSkillRange)
		{
			// 设置移动到的目标位置
			fVector3 fvTargetPos = GetTargetPos(pCharacter->GetPosition(), fvTarget, fMoveRange);
			m_paramAI_UseSkill.m_posPrevTarget = fvTargetPos;

			RC_RESULT rcResult = AI_MoveTo( fvTargetPos.x, fvTargetPos.z );
			if ( rcResult == RC_ERROR )
			{
				OnAIStopped( );
				return FALSE;
			}
		}
		else
		{
			SetForbidTime(USE_SKILL_FORBID_TIME);
			SendUseSkillMessage( m_paramAI_UseSkill.m_idSkill,
				m_paramAI_UseSkill.m_SkillLevel,
				m_paramAI_UseSkill.m_guidTarget,
				m_paramAI_UseSkill.m_idTarget,
				m_paramAI_UseSkill.m_posTarget.x,
				m_paramAI_UseSkill.m_posTarget.z,
				m_paramAI_UseSkill.m_fDir );
			//只有不是连续攻击的技能才转入IDLE状态
			//if (pSkillData)
			//{
			//	if (FALSE == pSkillData->m_bAutoRedo)
			//	{
			if ( MYSELF_AI_IDLE != GetMySelfAI() )
			{
				SetMySelfAI( MYSELF_AI_IDLE );
			}
			//	}
			//}
		}
	}

	return TRUE;
}

BOOL CAI_MySelf::Tick_ActiveTripperObj( VOID )
{
	CObject_Character *pCharacter = GetCharacter();

	fVector3 fvTarget;
	CObject *pTarget = (CObject*)((CObjectManager::GetMe())->FindServerObject( m_paramAI_ActiveTripperObj.m_idTripperObj ));
	if ( pTarget == NULL )
	{
		OnAIStopped( );
		return FALSE;
	}

	if (pCharacter->CharacterLogic_Get() != CObject_Character::CHARACTER_LOGIC_MOVE)
	{
		SendActiveTripperObjMessage( m_paramAI_ActiveTripperObj.m_idTripperObj );
		if ( MYSELF_AI_IDLE != GetMySelfAI() )
		{
			SetMySelfAI( MYSELF_AI_IDLE );
		}
	}

	return TRUE;
}

BOOL CAI_MySelf::Tick_DefaultEvent( VOID )
{
	CObject_Character *pCharacter = GetCharacter();

	CObject *pTarget = (CObject*)((CObjectManager::GetMe())->FindServerObject( m_paramAI_DefaultEvent.m_idTargetObj ));
	if ( pTarget == NULL )
	{
		OnAIStopped( );
		return FALSE;
	}
	
	fVector3 fvTarget;
	fvTarget = pTarget->GetPosition();


	FLOAT fDistSq = TDU_GetDistSq( fvTarget, GetCharacter()->GetPosition() );
	if (fDistSq <= MAX_CLIENT_DEFAULT_EVENT_DIST_SQ)
	{
		SendDefaultEventMessage( m_paramAI_DefaultEvent.m_idTargetObj );

		// 如果是任务NPC，设置其自身的方向
		if(!g_theKernel.IsKindOf(pTarget->GetClass(), GETCLASS(CObject_PlayerOther)))
		{
			if ( ((CObject_PlayerNPC*)pTarget)->IsCanFaceToPlayer() )
			{// 判断该NPC是否可以转向
				((CObject_PlayerNPC*)pTarget)->SetNeedFaceToPlayer(TRUE);
			}
		}
		OnAIStopped( );
		return TRUE;
	}

	FLOAT fPrevTargetToTargetSq = TDU_GetDistSq( fvTarget, m_paramAI_DefaultEvent.m_posPrevTarget );
	if ( pCharacter->CharacterLogic_Get() == CObject_Character::CHARACTER_LOGIC_IDLE
		|| fPrevTargetToTargetSq > MAX_CLIENT_DEFAULT_EVENT_DIST_SQ + 1.f )
	{
		fVector3 fvTargetPos = GetTargetPos(pCharacter->GetPosition(), fvTarget, MAX_CLIENT_DEFAULT_EVENT_DIST);
		RC_RESULT rcResult = AI_MoveTo( fvTargetPos.x, fvTargetPos.z );
		if ( rcResult != RC_OK )
		{
			OnAIStopped( );
			return FALSE;
		}
		m_paramAI_DefaultEvent.m_posPrevTarget = fvTargetPos;
	}
	return TRUE;
}
//
//BOOL CAI_MySelf::Tick_Dead( VOID )
//{
//	return TRUE;
//}
//挂机逻辑判断  fujia 2007.12.21
BOOL CAI_MySelf::Tick_AutoHit( VOID )
{
	if (CObjectManager::GetMe()->GetMySelf()->CharacterLogic_Get() == CObject_Character::CHARACTER_LOGIC_DEAD)
	{
		StartGuaJi = 0;
	}
	CObjectManager::GetMe()->Debug_LockNearestTargetToHit();
	return TRUE;
}
BOOL CAI_MySelf::Tick_Follow( VOID )
{

	if ( m_paramAI_Follow.m_nTickCount < m_paramAI_Follow.m_nMaxTickCount )
	{
		++(m_paramAI_Follow.m_nTickCount);
		return TRUE;
	}

	m_paramAI_Follow.m_nTickCount = 0;

	CObject* pObj = (CObject*)(CObjectManager::GetMe()->FindServerObject( m_paramAI_Follow.m_idTargetObj ));
	
	if ( pObj == NULL
	 || !g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character))
	 )
	{
		; // 暂时不被动退出跟随，而是自动跑到最后一个位置
//		OnAIStopped( );
//		return FALSE;
	}
	else
	{
		const fVector3& fTargetPos = pObj->GetPosition();
		FLOAT fDistance = TDU_GetDist(m_paramAI_Follow.m_LastPos, fTargetPos);

		if ( fDistance < 2 )
		{ // 距离太短就不跑了
			return TRUE;
		}
		//else if ( fDistance > 30 )
		//{ // 距离太长就跟不上了，距离可调
		//	OnAIStopped( );
		//	return FALSE;
		//}
		else
		{
			fVector3 fPos;

			fPos.x = (m_paramAI_Follow.m_LastPos.x - fTargetPos.x) / fDistance + fTargetPos.x;
			fPos.y = fTargetPos.y; // 这个值应该会和实际情况有一定误差，但是基本上不影响实际结果
			fPos.z = (m_paramAI_Follow.m_LastPos.z - fTargetPos.z) / fDistance + fTargetPos.z;
			RC_RESULT Result = AI_MoveTo( fPos.x, fPos.z );
			if ( Result != RC_OK )
				return FALSE;

			m_paramAI_Follow.m_LastPos = fPos;
		}
	}

	return TRUE;
}

VOID CAI_MySelf::OnAIStopped( VOID )
{
	switch( GetMySelfAI() )
	{
	case MYSELF_AI_IDLE:
	case MYSELF_AI_MOVE:
	case MYSELF_AI_USE_SKILL:
	case MYSELF_AI_ACTIVE_TRIPPEROBJ:
	case MYSELF_AI_DEFAULT_EVENT:
	case MYSELF_AI_GUAJI:
		break;
	case MYSELF_AI_FOLLOW:
		{
			StopFollow();
		}
		break;
	default:
		//Assert( FALSE && "CAI_MySelf::OnAIStopped  unknow GetMySelfAI" );
		break;
	}

	Enter_Idle( );
}

VOID CAI_MySelf::StopFollow( VOID )
{
	CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "退出跟随状态");
}

fVector3 CAI_MySelf::GetTargetPos(fVector3 MePos, fVector3 TargetPos, FLOAT fDist)
{
	fVector3 fvTarget;
	FLOAT fMTDist = TDU_GetDist(MePos, TargetPos);
	FLOAT fGoDist = (fMTDist - fDist + 0.5f) > 0 ? (fMTDist - fDist + 0.5f) : 0;
	
	fvTarget.x = MePos.x + fGoDist * (TargetPos.x - MePos.x) / fMTDist;
	fvTarget.z = MePos.z + fGoDist * (TargetPos.z - MePos.z) / fMTDist;
	fvTarget.y = MePos.y + fGoDist * (TargetPos.y - MePos.y) / fMTDist;

	return fvTarget;
}

VOID CAI_MySelf::SendMoveMessage( const CPath *pPath, INT nLogicCount )
{
	CObject_Character *pCharacter = GetCharacter();

	WORLD_POS posCurrent;
	posCurrent.m_fX	= pCharacter->GetPosition().x;
	posCurrent.m_fZ	= pCharacter->GetPosition().z;

	//向服务器发送请求消息
	CGCharMove msg;
	msg.setObjID( pCharacter->GetServerID() );
	msg.setHandleID( nLogicCount );
	msg.setWorldPos( posCurrent );

	UINT dwNumPathNode = (UINT)(pPath->GetUnitNum());
	if ( dwNumPathNode == 0 )
		return ;

	UINT i;
	WORLD_POS posTarget;
	for ( i = 0; i < dwNumPathNode; i++ )
	{
		const CPath::PathUnit &puTemp = pPath->GetConstPathUnit( i );

		posTarget.m_fX	= puTemp.fvTarget.x;
		posTarget.m_fZ	= puTemp.fvTarget.y;

		//检测是否合法
		if(!CWorldManager::GetMe()->GetActiveScene()->IsValidPosition(puTemp.fvTarget))
		{
			return;
		}

		msg.addTargetPos( &posTarget );
	}

	CNetManager::GetMe()->SendPacket( &msg );
//	AxTrace(0, 3, "send move message logic to [%.1f    %.1f] path",posTarget.m_fX, posTarget.m_fZ);
}

VOID CAI_MySelf::SendUseSkillMessage( INT idSkill, INT nLevel, GUID_t guidTarget, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir )
{
    CObject_Character *pCharacter = GetCharacter();

	// 不要打开下面的代码，打开会出现连招攻击的技能第一招发不出来
	//// temp code {
	//pCharacter->m_paramLogic_SkillSend.m_fCurTime = 0;
	//pCharacter->CharacterLogic_Set(CObject_Character::CHARACTER_LOGIC_SKILL_SEND);
	//// }

	AxTrace(0, 0, "%d", pCharacter->CharacterLogic_Get());
	WORLD_POS posCurrent, posTarget;
	posCurrent.m_fX	= pCharacter->GetPosition().x;
	posCurrent.m_fZ	= pCharacter->GetPosition().z;
	posTarget.m_fX	= fTargetX;
	posTarget.m_fZ	= fTargetZ;

	// 发往服务器
	CGCharUseSkill msg;
	msg.setObjID( pCharacter->GetServerID() );
	msg.setSkillDataID( idSkill*100+nLevel );
	msg.setTargetGUID( guidTarget );
	msg.setTargetID( idTargetObj );
	msg.setTargetPos( posTarget );
	msg.setDir( fDir );

	CNetManager::GetMe()->SendPacket( &msg );
	CActionSystem::GetMe()->UpdateCommonCoolDown(idSkill);

	AxTrace(0, 0, "%s", "Send Skill...");
	CGameProcedure::s_pGfxSystem->PushDebugString("CGCharUseSkill[%d]->[%d]", 
		idSkill, idTargetObj);
}

VOID CAI_MySelf::SendActiveTripperObjMessage( INT idTripperObj )
{
	CTripperObject* pTripperObj = (CTripperObject*)CObjectManager::GetMe()->FindServerObject(idTripperObj);
	if(!pTripperObj) return;

	pTripperObj->Tripper_Active();
}

VOID CAI_MySelf::SendComposeItemMessage( INT idAbility, INT idPrescription,	ObjID_t guidPlatform )
{
	CObject_Character *pCharacter = GetCharacter();

	// 发往服务器
	CGUseAbility msg;
	msg.SetAbilityID( idAbility );
	msg.SetPrescriptionID( idPrescription );
	msg.SetPlatformGUID( guidPlatform );


	CNetManager::GetMe()->SendPacket( &msg );

	CGameProcedure::s_pGfxSystem->PushDebugString("CGCharUseAbility[%d],[%d],[%d]", 
		idAbility, idPrescription,	guidPlatform);
}

VOID CAI_MySelf::SendDefaultEventMessage( INT idTargetObj )
{
	CGCharDefaultEvent msg;
	msg.setObjID( idTargetObj );
	CNetManager::GetMe()->SendPacket( &msg );

	CGameProcedure::s_pGfxSystem->PushDebugString("CGCharDefaultEvent");
}

RC_RESULT CAI_MySelf::IsCanUseSkill( INT idSkill, INT idLevel, GUID_t guidTarget, INT idTargetObj, FLOAT fTargetX, FLOAT fTargetZ, FLOAT fDir, BOOL& bNeedMove )
{
	//if ( GetMySelfAI() == MYSELF_AI_DEAD )
	//	return RC_ERROR;

	if ( GetMySelfAI( ) ==MYSELF_AI_USE_SKILL
		&& m_paramAI_UseSkill.m_idSkill == idSkill
		&& m_paramAI_UseSkill.m_idTarget == idTargetObj
		&& fabsf( m_paramAI_UseSkill.m_posTarget.x - fTargetX ) < 0.1f
		&& fabsf( m_paramAI_UseSkill.m_posTarget.z - fTargetZ ) < 0.1f
		&& fabsf( m_paramAI_UseSkill.m_fDir - fDir ) < 0.1f )
	{
		return RC_ERROR;
	}

	CObject_Character *pCharacter = GetCharacter();
	CCharacterData* pCharData = pCharacter->GetCharacterData();
	if ( pCharData->IsDie() )
		return RC_ERROR;

	if ( pCharacter->CharacterLogic_Get() == CObject_Character::CHARACTER_LOGIC_DEAD )
		return RC_ERROR;

	const SSkillData *pSkillData = GetSkillDataMgr()->GetSkillData( idSkill );
	if ( pSkillData == NULL )
		return RC_ERROR;

	//////////////////////////////////////////////////////////////////////////////////////////
	const SCLIENT_SKILL* pSkill = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Skill( idSkill * 100 + idLevel );
	if (!pSkill)
		return RC_ERROR;

	CObject_PlayerMySelf *pMySelf = CObjectManager::GetMe()->GetMySelf( );
	INT idUser = (pMySelf)?(pMySelf->GetID()):(INVALID_ID);

	ORESULT oResult = pSkill->IsCanUse(idUser, idLevel, idTargetObj, fTargetX, fTargetZ, fDir );
    if (oResult != OR_OK)
	{
		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, GetOResultText( oResult ) );
		return RC_ERROR;
	}

	// 不需要Character目标的技能则直接返回成功
	if (pSkill->m_pDefine->m_nSelectType != SELECT_TYPE_CHARACTER)
		return RC_OK;

	fVector3 fvTarget;
	FLOAT fMaxSkillRange, fMaxSkillRangeSq, fDistToTargetSq;
	CObject *pTarget = (CObject*)((CObjectManager::GetMe())->FindServerObject( idTargetObj ));
	if ( pTarget == NULL )
		return RC_ERROR;

	fvTarget = pTarget->GetPosition();

	fMaxSkillRange = pSkillData->m_fMaxAttackRange;
	fMaxSkillRangeSq = fMaxSkillRange * fMaxSkillRange;
	fDistToTargetSq = TDU_GetDistSq( fvTarget, pCharacter->GetPosition() );
	if ( fMaxSkillRangeSq > fDistToTargetSq )
	{
		bNeedMove = FALSE;
	}
	else
	{
        // 设置移动到的目标位置
		fVector3 fvTargetPos = GetTargetPos(pCharacter->GetPosition(), fvTarget, fMaxSkillRange);

		RC_RESULT rcResult = AI_MoveTo( fvTargetPos.x, fvTargetPos.z );
		if ( rcResult != RC_OK )
		{
			return rcResult;
		}

		// 参数付值
		m_paramAI_UseSkill.m_idSkill				= idSkill;
		m_paramAI_UseSkill.m_SkillLevel				= idLevel;
		m_paramAI_UseSkill.m_guidTarget				= guidTarget;
		m_paramAI_UseSkill.m_idTarget				= idTargetObj;
		m_paramAI_UseSkill.m_posTarget				= fvTarget;
		m_paramAI_UseSkill.m_fDir					= fDir;
		m_paramAI_UseSkill.m_posPrevTarget			= fvTargetPos;

		SetMySelfAI( MYSELF_AI_USE_SKILL );

		bNeedMove = TRUE;
	}

	return RC_OK;

}

BOOL CAI_MySelf::IsCanFollow()
{	
	// 判断玩家当前的状态
	if( TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall() )
	{
		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "摆摊时不能移动。");
		return FALSE;
	}

	// 判断当前的 AI
	switch( GetMySelfAI() )
	{
	case MYSELF_AI_MOVE:
	case MYSELF_AI_IDLE:
	case MYSELF_AI_DEFAULT_EVENT:
	case MYSELF_AI_FOLLOW:
		return TRUE;
	case MYSELF_AI_ACTIVE_TRIPPEROBJ:
	case MYSELF_AI_USE_SKILL:
	case MYSELF_AI_GUAJI:
	//case MYSELF_AI_DEAD:
		{
			CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE, "无法进入跟随状态！");
		}
		return FALSE;
	default:
		Assert( FALSE && "CAI_MySelf::IsCanFollow  unknow GetMySelfAI" );
		return FALSE;
	}

	return TRUE;
}

BOOL CAI_MySelf::IsLimitCmd( const SCommand_AI *pCmd )
{
	CObject_PlayerMySelf* pMySelf = (CObject_PlayerMySelf*)GetCharacter();
	CCharacterData* pCharData = pMySelf->GetCharacterData();
	CUIDataPool* pUIDataPool = CUIDataPool::GetMe();
	// 实际上这一步不应该放以这里检测，应该放在UI里
	// 检测是否为组队跟随状态
	{
		BOOL bTeamFollowMode = pCharData->Get_TeamFollowFlag();
		BOOL bRet = FALSE;

		if ( bTeamFollowMode )
		{ // 组队跟随状态
			CTeamOrGroup* pTeam = pUIDataPool->GetTeamOrGroup();
			BOOL bIsLeader = (pTeam->GetLeaderGUID() == pMySelf->GetServerGUID());

			switch ( pCmd->m_wID )
			{
			case AIC_MOVE:
			case AIC_DEFAULT_EVENT:
			case AIC_FOLLOW:
				{
					if ( bIsLeader==FALSE )
					{
						bRet = TRUE;
					}
				}
				break;
			case AIC_USE_SKILL:
			case AIC_TRIPPER_ACTIVE:
			case AIC_COMPOSE_ITEM:
			case AIC_GUAJI:
				
				{
					bRet = TRUE;
				}
				break;
			default:
				break;
			};

			if ( bRet==TRUE )
			{
				CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"组队跟随状态,不允许进行这种操作");
			}
		}

		if ( bRet )
			return TRUE;
	}

	CDataPool* pDataPool = CDataPool::GetMe();
	switch ( pCmd->m_wID )
	{
	case AIC_MOVE:
		{
			// 是否有限制移动的状态
			const _BUFF_IMPACT_INFO* pBuffImpactInfo;
			INT nLimitMoveBuffID = INVALID_ID;
			pBuffImpactInfo = pDataPool->BuffImpact_GetByID( nLimitMoveBuffID );
			if ( pBuffImpactInfo != NULL )
			{
				return TRUE;
			}
		}
		return FALSE;
	case AIC_USE_SKILL:
		// 是否有限制施法的状态
		return FALSE;
	case AIC_TRIPPER_ACTIVE:
	case AIC_DEFAULT_EVENT:
	case AIC_COMPOSE_ITEM:
	//case AIC_DIE:
	case AIC_JUMP:
	case AIC_FOLLOW:
	case AIC_GUAJI:
		return FALSE;
	default:
		Assert( FALSE && "CAI_MySelf::IsLimitCmd  unknow pCmd->m_wID" );
		return TRUE;
	}
}
