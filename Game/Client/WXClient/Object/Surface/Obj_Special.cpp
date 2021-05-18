// Obj_Special.cpp

#include "StdAfx.h"

#include "TDException.h"
#include "TDDBC_Struct.h"

#include "TDActionSystem.h"
#include "TDGameInterface.h"
#include "..\..\DataPool\GMDP_Struct_Skill.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "GameStruct_Skill.h"
#include "..\Character\Obj_PlayerMySelf.h"
#include "..\..\Procedure\GameProcedure.h"

#include "..\ObjectCommandDef.h"
#include "..\ObjectManager.h"
#include "..\Character\Obj_Character.h"
#include "..\Surface\Obj_Bullet.h"
#include "..\..\Global.h"
#include "..\..\Dbc\GMDataBase.h"

#include "Obj_Special.h"

WX_IMPLEMENT_DYNAMIC(CObject_Special, GETCLASS(CObject_Effect));

CObject_Special::CObject_Special( VOID )
{
	m_eSpecialState		= SPECIAL_STATE_INVALID;
	m_idOwner			= INVALID_ID;
	m_pSpecialObjData	= NULL;
}

CObject_Special::~CObject_Special( VOID )
{
}

VOID CObject_Special::Initial( VOID* pInit )
{
	CObject_Effect::Initial( pInit );

	m_eSpecialState		= SPECIAL_STATE_NORMAL;

	SObject_SpecialInit *pSpecialInit = (SObject_SpecialInit*)(pInit);
	if ( pSpecialInit != NULL )
	{
		m_idOwner		= pSpecialInit->m_idOwner;
		if ( pSpecialInit->m_nDataID != INVALID_ID )
		{
			DBC_DEFINEHANDLE(s_pSpecialDataDBC, DBC_SPECIAL_OBJ_DATA);
			m_pSpecialObjData = (const _DBC_SPECIAL_OBJ_DATA*)s_pSpecialDataDBC->Search_Index_EQU(pSpecialInit->m_nDataID);
		}
		else
		{
			m_pSpecialObjData = NULL;
		}
	}
	else
	{
		m_idOwner			= INVALID_ID;
		m_pSpecialObjData	= NULL;
	}

	if ( GetSpecialObjData() != NULL && GetSpecialObjData()->m_lpszEffect_Normal != NULL && strlen(GetSpecialObjData()->m_lpszEffect_Normal) > 0 )
	{
		ChangEffect( GetSpecialObjData()->m_lpszEffect_Normal, TRUE );
	}
}

VOID CObject_Special::Release( VOID )
{
	ReleaseCommandList();
	m_eSpecialState		= SPECIAL_STATE_INVALID;
	m_idOwner			= INVALID_ID;
	m_pSpecialObjData	= NULL;

	CObject_Effect::Release();
}

VOID CObject_Special::Tick( VOID )
{
	switch ( GetSpecialState() )
	{
	case SPECIAL_STATE_NORMAL:
		{
			// 如果有消息就执行消息， 否则就是等待状态
			SCommand_Object *pCommand = PopCommand( );
			if ( pCommand != NULL )
			{
				OnCommand( pCommand );

				// 因消息不支持存储数组，所以只能特殊处理一下
				if ( pCommand->m_wID == OC_SPECIAL_OBJ_TRIGGER )
				{
					ObjID_t *paTargetIDs = (ObjID_t*)(pCommand->m_apParam[2]);
					if ( paTargetIDs != NULL )
					{
						delete [] paTargetIDs;
						paTargetIDs = NULL;
					}
				}

				delete pCommand;
				pCommand = NULL;
			}
		}
		break;
	case SPECIAL_STATE_DIE:
		break;
	default:
		break;
	}
	CObject_Effect::Tick();
}

// 左键指令的分析
BOOL CObject_Special::FillMouseCommand_Left( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill )
{
	pOutCmd->m_typeMouse = SCommand_Mouse::MCT_NULL;
	if ( pActiveSkill != NULL
		&& pActiveSkill->GetType() == AOT_SKILL )
	{
		const SCLIENT_SKILL* pSkillImpl = (const SCLIENT_SKILL*)pActiveSkill->GetImpl();
		if(pSkillImpl != NULL
			&& pSkillImpl->m_pDefine->m_nSelectType == SELECT_TYPE_CHARACTER)
		{
			ENUM_RELATION eCampType = CGameProcedure::s_pGameInterface->GetCampType( CObjectManager::GetMe()->GetMySelf(), this );
			switch ( eCampType )
			{
			case RELATION_FRIEND:
				break;
			case RELATION_ENEMY:
			default:
				{
					pOutCmd->m_typeMouse = SCommand_Mouse::MCT_SKILL_OBJ;
					pOutCmd->m_apParam[0] = (VOID*)pActiveSkill;
					pOutCmd->m_adwParam[1] = GetServerID();
				}
				break;
			}
		}
	}
	return TRUE;
}

// 右键指令的分析
BOOL CObject_Special::FillMouseCommand_Right( SCommand_Mouse *pOutCmd, tActionItem* pActiveSkill )
{
	pOutCmd->m_typeMouse	= SCommand_Mouse::MCT_PLAYER_SELECT;
	pOutCmd->m_adwParam[0]	= GetServerID();
	return TRUE;
}

// 压入一条指令
BOOL CObject_Special::PushCommand(const SCommand_Object *pCmd )
{
	SCommand_Object *pNewCommand = new SCommand_Object;
	*pNewCommand = *pCmd;

	// 因消息不支持存储数组，所以只能特殊处理一下
	if ( pCmd->m_wID == OC_SPECIAL_OBJ_TRIGGER )
	{
		INT nTargetCount;
		ObjID_t *paTargetIDs, *paNewTargetIDs;
		nTargetCount	= pCmd->m_anParam[1];
		paTargetIDs		= (ObjID_t*)(pCmd->m_apParam[2]);

		if ( nTargetCount > 0 )
		{
			paNewTargetIDs = new ObjID_t [nTargetCount];
			memcpy( paNewTargetIDs, paTargetIDs, sizeof( ObjID_t ) * nTargetCount );
			pNewCommand->m_apParam[2] = paNewTargetIDs;
		}
	}

	m_listCommand.insert( m_listCommand.end(), CCommandList::value_type( pNewCommand )  );

	return TRUE;
}

RC_RESULT CObject_Special::OnCommand(const SCommand_Object *pCmd )
{
	switch( pCmd->m_wID )
	{
	case OC_SPECIAL_OBJ_TRIGGER:
		{
			INT nLogicCount, nTargetCount;
			ObjID_t *paTargetIDs;
			nLogicCount		= pCmd->m_anParam[0];
			nTargetCount	= pCmd->m_anParam[1];
			paTargetIDs		= (ObjID_t*)(pCmd->m_apParam[2]);

			DoTrigger( nLogicCount, nTargetCount, paTargetIDs );
		}
		break;
	case OC_SPECIAL_OBJ_DIE:
		{
			DoDie();
		}
		break;
	default:
		break;
	}
	return RC_SKIP;
}

VOID CObject_Special::DoTrigger( INT nLogicCount, INT nTargetCount, ObjID_t *paTargetIDs )
{
	SetLogicCount( nLogicCount );

	if ( GetSpecialObjData() != NULL && GetSpecialObjData()->m_lpszEffect_Active != NULL && strlen(GetSpecialObjData()->m_lpszEffect_Active) > 0 )
	{
		CObject_Effect *pEffectObj = (CObject_Effect*)(CObjectManager::GetMe()->NewObject( "CObject_Effect" ));
		if ( pEffectObj != NULL )
		{
			SObject_EffectInit initEffect;
			initEffect.m_fvPos 			= GetPosition();
			initEffect.m_fvRot 			= fVector3( 0.f, 0.f, 0.f );
			initEffect.m_pszEffectName	= GetSpecialObjData()->m_lpszEffect_Active;
			initEffect.m_bLoopEffect	= FALSE;
			pEffectObj->Initial( &initEffect );
		}
	}


	// 有子弹
	if ( GetSpecialObjData() != NULL && GetSpecialObjData()->m_nBulletID != INVALID_ID )
	{
		SObject_BulletInit initBullet;
		initBullet.m_fvPos				= GetPosition();
		initBullet.m_fvRot				= GetRotation();
		initBullet.m_idSend				= GetServerID();
		initBullet.m_nSendLogicCount	= GetLogicCount();
		initBullet.m_nBulletID			= m_pSpecialObjData->m_nBulletID;
		initBullet.m_bSingleTarget		= TRUE;
		initBullet.m_fvTargetPos		= fVector3( -1.f, -1.f, -1.f );
		INT i;
		for ( i = 0; i < nTargetCount; i++ )
		{
			initBullet.m_idTarget	= paTargetIDs[i];

			CObject_Bullet *pBullet = (CObject_Bullet*)CObjectManager::GetMe()->NewObject( "CObject_Bullet");
			pBullet->Initial( &initBullet );
		}
	}
	else
	{
		CObject_Character *pCharacter;
		INT i;
		for ( i = 0; i < nTargetCount; i++ )
		{
			ObjID_t idCharacter = paTargetIDs[i];
			pCharacter = (CObject_Character*)(CObjectManager::GetMe()->FindServerObject( idCharacter ));
			if ( pCharacter != NULL && g_theKernel.IsKindOf( pCharacter->GetClass(), GETCLASS(CObject_Character) ) )
			{
				pCharacter->ShowLogicEvent( GetServerID(), nLogicCount, TRUE );
			}
		}
	}
}

VOID CObject_Special::DoDie( VOID )
{
	if ( GetSpecialObjData() != NULL && GetSpecialObjData()->m_lpszEffect_Die != NULL && strlen(GetSpecialObjData()->m_lpszEffect_Die) > 0 )
	{
		ChangEffect( GetSpecialObjData()->m_lpszEffect_Die, FALSE );
	}

	SetSpecialState( SPECIAL_STATE_DIE );
}

BOOL CObject_Special::IsEffectStopped( VOID )const
{
	if ( SPECIAL_STATE_DIE == GetSpecialState() )
	{
		return CObject_Effect::IsEffectStopped();
	}
	return FALSE;
}

VOID CObject_Special::ReleaseCommandList( VOID )
{
	if ( !m_listCommand.empty() )
	{
		CCommandList::iterator itCur, itEnd;
		SCommand_Object *pCommand;
		itEnd = m_listCommand.end();
		for ( itCur = m_listCommand.begin(); itCur != itEnd; itCur++ )
		{
			pCommand = *itCur;
			if ( pCommand != NULL )
			{
				if ( pCommand->m_wID == OC_SPECIAL_OBJ_TRIGGER )
				{
					ObjID_t *paTargetIDs = (ObjID_t*)(pCommand->m_apParam[2]);
					if ( paTargetIDs != NULL )
					{
						delete [] paTargetIDs;
						paTargetIDs = NULL;
					}
				}
				delete pCommand;
				pCommand = NULL;
			}
		}
		m_listCommand.erase( m_listCommand.begin(), m_listCommand.end() );
	}
}

SCommand_Object *CObject_Special::PopCommand( VOID )
{
	if ( m_listCommand.empty() )
	{
		return NULL;
	}

	CCommandList::iterator itCur;
	itCur = m_listCommand.begin();
	SCommand_Object *pResult = *itCur;
	m_listCommand.erase( itCur );
	return pResult;
}

const SCommand_Object *CObject_Special::GetNextCommand( VOID )const
{
	if ( m_listCommand.empty() )
	{
		return NULL;
	}

	CCommandList::const_iterator itCur;
	itCur = m_listCommand.begin();
	const SCommand_Object *pResult = *itCur;
	return pResult;
}
