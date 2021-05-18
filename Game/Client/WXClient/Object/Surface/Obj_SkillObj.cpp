// Obj_SkillObj.cpp
// 
/////////////////////////////////////////////////////

#include "StdAfx.h"
//
//#include "TDException.h"
//#include "DB_Struct.h"
//#include "TDUtil.h"
//#include "TDTimeSystem.h"
//#include "..\ObjectManager.h"
//#include "..\..\Procedure\GameProcedure.h"
//#include "..\Character\Obj_Character.h"
//#include "SkillDataMgr.h"
//#include "..\BulletDataMgr.h"

#include "Obj_SkillObj.h"
//
//WX_IMPLEMENT_DYNAMIC(CObject_SkillObj, GETCLASS(CObject_Effect));
//CObject_SkillObj::CObject_SkillObj()
//{
//	m_idSkill			= INVALID_ID;
//	m_idUser			= INVALID_ID;
//
//	m_idTarget			= INVALID_ID;
//	m_posTarget			= WORLD_POS( -1.f, -1.f );
//	m_fInitDir			= -1.f;
//}
//
//CObject_SkillObj::~CObject_SkillObj()
//{
//}
//
//VOID CObject_SkillObj::Initial( VOID *pInit )
//{
//	CObject_Effect::Initial(pInit);
//
//	SObject_SkillObjInit *pSkillObjInit= (SObject_SkillObjInit*)(pInit);
//	if ( pSkillObjInit == NULL )
//		return ;
//
//	m_idSkill			= pSkillObjInit->m_idSkill;
//	m_idUser			= pSkillObjInit->m_idUser;
//
//	m_idTarget			= pSkillObjInit->m_idTarget;
//	m_posTarget			= pSkillObjInit->m_posTarget;
//	m_fInitDir			= pSkillObjInit->m_fInitDir;
//
//	m_pSkillData			= GetSkillDataMgr()->GetSkillData( m_idSkill );
//	if ( m_pSkillData == NULL )
//		return ;
//
//	m_pSkillObjData			= GetBulletDataMgr()->GetBulletData( m_pSkillData->m_nBulletID );
//	if ( m_pSkillObjData == NULL )
//		return ;
//
//	CObject_Character *pUser = NULL;
//	if ( m_idUser != INVALID_ID )
//		pUser = (CObject_Character*)(CObjectManager::GetMe()->FindServerObject( m_idUser ));
//
//	CObject_Character* pTargetObj = NULL;
//	if ( m_idTarget != INVALID_ID )
//		pTargetObj = (CObject_Character*)(CObjectManager::GetMe()->FindServerObject( m_idTarget ));
//
//	// 决定初始位置
//	fVector3 fvPos;
//	switch ( m_pSkillData->m_nSelectType )
//	{
//	case SELECT_TYPE_CHARACTER:
//	case SELECT_TYPE_POS:
//		{
//			switch ( m_pSkillObjData->m_nInitPosType )
//			{
//			case SKILL_OBJ_INIT_POS_TYPE_USER:
//				{
//					if ( pUser != NULL )
//					{
//						fvPos = pUser->GetPosition();
//						fvPos.y += 1.f;
//					}
//					else
//					{
//						CGameProcedure::s_pGfxSystem->Axis_Trans(
//							tGfxSystem::AX_PLAN, fVector3(pSkillObjInit->m_posCreate.m_fX, 0.0f, pSkillObjInit->m_posCreate.m_fZ),
//							tGfxSystem::AX_GAME, fvPos);
//						fvPos.y += 1.f;
//					}
//				}
//				break;
//			case SKILL_OBJ_INIT_POS_TYPE_TARGET:
//				{
//					if ( pTargetObj != NULL )
//					{
//						fvPos = pTargetObj->GetPosition();
//					}
//					else
//					{
//						CGameProcedure::s_pGfxSystem->Axis_Trans(
//							tGfxSystem::AX_PLAN, fVector3(m_posTarget.m_fX, 0.0f, m_posTarget.m_fZ),
//							tGfxSystem::AX_GAME, fvPos);
//					}
//				}
//				break;
//			case SKILL_OBJ_INIT_POS_TYPE_TARGET_OVERHEAD:
//			default:
//				{
//					if ( pTargetObj != NULL )
//					{
//						fvPos = pTargetObj->GetPosition();
//						fvPos.y += 10.f;
//					}
//					else
//					{
//						CGameProcedure::s_pGfxSystem->Axis_Trans(
//							tGfxSystem::AX_PLAN, fVector3(m_posTarget.m_fX, 0.0f, m_posTarget.m_fZ),
//							tGfxSystem::AX_GAME, fvPos);
//						fvPos.y += 10.f;
//					}
//				}
//				break;
//			}
//		}
//		break;
//	case SELECT_TYPE_DIR:
//		{
//			SetFaceDir( m_fInitDir );
//			FLOAT fDist = m_pSkillData->m_fDamageRange;
//			CGameProcedure::s_pGfxSystem->Axis_Trans(
//				tGfxSystem::AX_PLAN, fVector3(pSkillObjInit->m_posCreate.m_fX, 0.0f, pSkillObjInit->m_posCreate.m_fZ),
//				tGfxSystem::AX_GAME, m_fvTargetPos);
//			m_fvTargetPos.y += 1.f;
//			fvPos = m_fvTargetPos;
//
//			m_fvTargetPos.x += sin(m_fInitDir) * fDist;
//			m_fvTargetPos.z += cos(m_fInitDir) * fDist;
//		}
//		break;
//	case SELECT_TYPE_NONE:
//		{
//			switch ( m_pSkillObjData->m_nInitPosType )
//			{
//			case SKILL_OBJ_INIT_POS_TYPE_USER:
//			case SKILL_OBJ_INIT_POS_TYPE_TARGET:
//				{
//					if ( pUser != NULL )
//					{
//						fvPos = pUser->GetPosition();
//						fvPos.y += 1.f;
//					}
//					else
//					{
//						CGameProcedure::s_pGfxSystem->Axis_Trans(
//							tGfxSystem::AX_PLAN, fVector3(pSkillObjInit->m_posCreate.m_fX, 0.0f, pSkillObjInit->m_posCreate.m_fZ),
//							tGfxSystem::AX_GAME, fvPos);
//						fvPos.y += 1.f;
//					}
//				}
//				break;
//			case SKILL_OBJ_INIT_POS_TYPE_TARGET_OVERHEAD:
//			default:
//				{
//					if ( pUser != NULL )
//					{
//						fvPos = pUser->GetPosition();
//						fvPos.y += 10.f;
//					}
//					else
//					{
//						CGameProcedure::s_pGfxSystem->Axis_Trans(
//							tGfxSystem::AX_PLAN, fVector3(pSkillObjInit->m_posCreate.m_fX, 0.0f, pSkillObjInit->m_posCreate.m_fZ),
//							tGfxSystem::AX_GAME, fvPos);
//						fvPos.y += 10.f;
//					}
//				}
//				break;
//			}
//			if ( pUser != NULL )
//				SetFaceDir( pUser->GetFaceDir() );
//		}
//		break;
//	default:
//		return ;
//	}
//
//	SetPosition( fvPos );
//
//	// 特效
//	// 下面这一句的接口已经改动
//	Assert( FALSE );
//	//ChangEffect( m_pSkillObjData->m_uEffectID, TRUE );
//
//	m_dwHitDegree			= 0;
//	m_bAlreadyHit			= FALSE;
//	m_bFlying				= TRUE;
//}
//
//VOID CObject_SkillObj::Tick( VOID )
//{
//	if ( m_pSkillObjData == NULL )
//	{
//		Destroy();
//		return ;
//	}
//
//	switch ( m_pSkillObjData->m_nType )
//	{
//	case SKILL_OBJ_TYPE_ONCE:			// 一次性杀伤的
//		if ( m_bFlying )
//		{
//			fVector3 fvTarget = *CalcTargetPos( );
//
//			FLOAT fCurTickFlyDist = m_pSkillObjData->m_fSpeed * CGameProcedure::s_pTimeSystem->GetDeltaTime()/1000.0f;
//			FLOAT fCurTickFlyDistSq = fCurTickFlyDist * fCurTickFlyDist;
//			FLOAT fDistSq = TDU_GetDistSq( fvTarget, GetPosition() );
//			FLOAT fDir = TDU_GetYAngle( fVector2( GetPosition().x, GetPosition().z ), fVector2( fvTarget.x, fvTarget.z ) );
//			if ( fDistSq <= fCurTickFlyDistSq )
//			{
//				SetPosition( fvTarget );
//				SetFaceDir( fDir );
//
//				// 下面这一句的接口已经改动
//				Assert( FALSE );
//				//ChangEffect( m_pSkillObjData->m_uHitEffectID, FALSE );
//				m_bFlying = FALSE;
//			}
//			else
//			{
//				fVector3 vDir = fvTarget - GetPosition();
//				vDir.normalise();
//				fVector3 vFlyLength;
//				vFlyLength.x = vDir.x * fCurTickFlyDist;
//				vFlyLength.y = vDir.y * fCurTickFlyDist;
//				vFlyLength.z = vDir.z * fCurTickFlyDist;
//				SetPosition( vFlyLength + GetPosition() );
//				SetFaceDir( fDir );
//			}
//		}
//		if ( !m_bFlying )
//		{
//			Tick_Once( );
//		}
//		break;
//	case SKILL_OBJ_TYPE_DURATIVE:		// 持续杀伤的
//		Tick_Durative( );
//		break;
//	case SKILL_OBJ_TYPE_TRAP:			// 陷阱
//		Tick_Trap( );
//		break;
//	default:
//		{
//			Destroy();
//			return ;
//		}
//		break;
//	}
//}
//
//BOOL CObject_SkillObj::Tick_Once( VOID )
//{
//	if ( !m_bAlreadyHit )
//	{
//		DoHit();
//		m_bAlreadyHit = TRUE;
//	}
//	else
//	{
//		CObject_Effect::Tick();
//	}
//	return TRUE;
//}
//
//BOOL CObject_SkillObj::Tick_Durative( VOID )
//{
//	CObject_Effect::Tick();
//	return TRUE;
//}
//
//BOOL CObject_SkillObj::Tick_Trap( VOID )
//{
//	CObject_Effect::Tick();
//	return TRUE;
//}
//
//const fVector3 *CObject_SkillObj::CalcTargetPos( VOID )
//{
//	if ( m_pSkillData == NULL || m_pSkillObjData == NULL )
//		return NULL;
//
//	switch ( m_pSkillData->m_nSelectType )
//	{
//	case SELECT_TYPE_CHARACTER:
//	case SELECT_TYPE_POS:
//		{
//			CObject_Character* pTargetObj = NULL;
//			if ( m_idTarget != INVALID_ID )
//				pTargetObj = (CObject_Character*)(CObjectManager::GetMe()->FindServerObject( m_idTarget ));
//
//			if ( pTargetObj != NULL )
//			{
//				m_fvTargetPos = pTargetObj->GetPosition();
//				if ( m_pSkillData->m_fDamageRange <= 0.f )
//				{
//					m_fvTargetPos.y += 1.f;
//				}
//			}
//			else
//			{
//				CGameProcedure::s_pGfxSystem->Axis_Trans(
//					tGfxSystem::AX_PLAN, fVector3(m_posTarget.m_fX, 0.0f, m_posTarget.m_fZ),
//					tGfxSystem::AX_GAME, m_fvTargetPos);
//			}
//		}
//		break;
//	case SELECT_TYPE_DIR:
//		break;
//	case SELECT_TYPE_NONE:
//		{
//			CObject_Character *pUser = NULL;
//			if ( m_idUser != INVALID_ID )
//				pUser = (CObject_Character*)(CObjectManager::GetMe()->FindServerObject( m_idUser ));
//
//			if ( pUser != NULL )
//			{
//				m_fvTargetPos = pUser->GetPosition();
//				if ( m_pSkillData->m_fDamageRange <= 0.f )
//				{
//					m_fvTargetPos.y += 1.f;
//				}
//			}
//			else
//			{
//				CGameProcedure::s_pGfxSystem->Axis_Trans(
//					tGfxSystem::AX_PLAN, fVector3(m_posTarget.m_fX, 0.0f, m_posTarget.m_fZ),
//					tGfxSystem::AX_GAME, m_fvTargetPos);
//			}
//		}
//		break;
//	default:
//		break;
//	}
//	return &m_fvTargetPos;
//}
//
//VOID CObject_SkillObj::DoHit( VOID )
//{
//	CObject_Character *pTargetObj = NULL;
//	switch ( m_pSkillData->m_nSelectType )
//	{
//	case SELECT_TYPE_CHARACTER:
//	case SELECT_TYPE_POS:
//	case SELECT_TYPE_DIR:
//		{
//			if ( m_idTarget != INVALID_ID )
//				pTargetObj = (CObject_Character*)(CObjectManager::GetMe()->FindServerObject( m_idTarget ));
//		}
//		break;
//	case SELECT_TYPE_NONE:
//		{
//			if ( m_idUser != INVALID_ID )
//				pTargetObj = (CObject_Character*)(CObjectManager::GetMe()->FindServerObject( m_idUser ));
//		}
//		break;
//	default:
//		return ;
//	}
//
//	if ( m_pSkillData->m_fDamageRange > 0.f )
//	{
//	}
//	else
//	{
//		// 单体杀伤
//		if ( pTargetObj != NULL )
//		{
//			pTargetObj->OnBeHit( m_idUser );
//		}
//	}
//}
