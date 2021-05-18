// Obj_Bullet.cpp
// 
/////////////////////////////////////////////////////

#include "StdAfx.h"

#include "GameStruct_Skill.h"
#include "TDException.h"
#include "TDUtil.h"
#include "TDTimeSystem.h"
#include "TDDBC_Struct.h"
#include "..\ObjectManager.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Dbc\GMDataBase.h"
#include "..\Character\Obj_Character.h"

#include "Obj_Bullet.h"

WX_IMPLEMENT_DYNAMIC(CObject_Bullet, GETCLASS(CObject_Effect));
CObject_Bullet::CObject_Bullet()
{
	m_idSend			= INVALID_ID;
	m_pBulletData		= NULL;
	m_nSendLogicCount	= -1;

	m_bSingleTarget		= FALSE;
	m_idTarget			= INVALID_ID;
	m_fvTargetPos		= fVector3( -1.f, -1.f, -1.f );
	m_fvStartPos		= fVector3( -1.f, -1.f, -1.f );
	m_fStartToEndDist	= -1.f;

	m_bAlreadyHit		= FALSE;
}

CObject_Bullet::~CObject_Bullet()
{
}

VOID CObject_Bullet::Initial( VOID *pInit )
{
	CObject_Effect::Initial(pInit);

	SObject_BulletInit *pBulletInit= (SObject_BulletInit*)(pInit);
	if ( pBulletInit != NULL )
	{
		m_idSend			= pBulletInit->m_idSend;
		if ( pBulletInit->m_nBulletID != -1 )
		{
			DBC_DEFINEHANDLE(s_pBulletDataDBC, DBC_BULLET_DATA);
			m_pBulletData	= (const _DBC_BULLET_DATA*)(s_pBulletDataDBC->Search_Index_EQU( pBulletInit->m_nBulletID ));
		}
		else
		{
			m_pBulletData	= NULL;
		}
		m_nSendLogicCount	= pBulletInit->m_nSendLogicCount;

		m_bSingleTarget		= pBulletInit->m_bSingleTarget;
		m_idTarget			= pBulletInit->m_idTarget;
		m_fvTargetPos		= pBulletInit->m_fvTargetPos;
	}
	else
	{
		m_idSend			= INVALID_ID;
		m_pBulletData		= NULL;
		m_nSendLogicCount	= -1;

		m_bSingleTarget		= FALSE;
		m_idTarget			= INVALID_ID;
		m_fvTargetPos		= fVector3( -1.f, -1.f, -1.f );
	}

	if(m_pBulletData != NULL)
	{
		switch(m_pBulletData->m_nContrailType)
		{
		case BULLET_CONTRAIL_TYPE_BEELINE:
		case BULLET_CONTRAIL_TYPE_PARABOLA:
			m_fvStartPos	= GetPosition();
			if ( strlen( m_pBulletData->m_szFlyEffect ) > 0 )
			{
				ChangEffect( m_pBulletData->m_szFlyEffect, FALSE );
			}
			break;
		case BULLET_CONTRAIL_TYPE_NONE:
		default:
			m_fvStartPos	= m_fvTargetPos;
			SetPosition(m_fvStartPos);
			break;
		}

	}
	else
	{
		m_fvStartPos	= GetPosition();
	}

	m_fStartToEndDist = TDU_GetDist(m_fvTargetPos, m_fvStartPos);
	m_bAlreadyHit	= FALSE;
}

VOID CObject_Bullet::Tick( VOID )
{
	if ( m_bAlreadyHit )
	{
		CObject_Effect::Tick();
	}
	else
	{
		if ( m_pBulletData == NULL )
		{
			m_bAlreadyHit = TRUE;
			return ;
		}

		switch(m_pBulletData->m_nContrailType)
		{
		case BULLET_CONTRAIL_TYPE_BEELINE:
		case BULLET_CONTRAIL_TYPE_PARABOLA:
			{
				fVector3 fvTargetPos;
				if ( m_bSingleTarget )
				{
					CObject *pObj = (CObject*)(CObjectManager::GetMe()->FindServerObject( m_idTarget ));
					if ( pObj != NULL )
					{
						fvTargetPos = pObj->GetPosition();
						if ( pObj->GetRenderInterface() != NULL && strlen( m_pBulletData->m_szHitEffectLocator ) > 0 )
						{
							pObj->GetRenderInterface()->Actor_GetLocator( m_pBulletData->m_szHitEffectLocator, fvTargetPos );
						}
						else
						{
							fvTargetPos = pObj->GetPosition();
						}
					}
					else
					{
						fvTargetPos = fVector3( -1.f, -1.f, -1.f );
					}
				}
				else
				{
					fvTargetPos = m_fvTargetPos;
				}

				FLOAT fCurTickFlyDist = m_pBulletData->m_fSpeed * CGameProcedure::s_pTimeSystem->GetDeltaTime()/1000.0f;
				FLOAT fCurTickFlyDistSq = fCurTickFlyDist * fCurTickFlyDist;
				FLOAT fDistSq = TDU_GetDistSq( fvTargetPos, GetPosition() );
				if ( fDistSq <= fCurTickFlyDistSq )
				{
					if(m_pBulletData->m_nContrailType == BULLET_CONTRAIL_TYPE_PARABOLA)
					{
						FLOAT fStartToEndDist = m_fStartToEndDist;
						FLOAT fTargetToStartDist = TDU_GetDist(fvTargetPos, m_fvStartPos);
						FLOAT fCenterToTargetDist = fabsf(fTargetToStartDist - fStartToEndDist/2.f);
						FLOAT fTemp = (fCenterToTargetDist * 2.f)/fStartToEndDist;
						FLOAT fAddY = (1.f - (fTemp * fTemp)) * m_pBulletData->m_fContrailParam;
						fvTargetPos.y += fAddY;
					}
					FLOAT fDir = TDU_GetYAngle( fVector2( GetPosition().x, GetPosition().z ), fVector2( fvTargetPos.x, fvTargetPos.z ) );
					SetPosition( fvTargetPos );
					SetFaceDir( fDir );

					AlreadyHit();
				}
				else
				{
					fVector3 vDir = fvTargetPos - GetPosition();
					vDir.normalise();
					fVector3 vFlyLength;
					vFlyLength.x = vDir.x * fCurTickFlyDist;
					vFlyLength.y = vDir.y * fCurTickFlyDist;
					vFlyLength.z = vDir.z * fCurTickFlyDist;
					fvTargetPos = vFlyLength + GetPosition();
					if(m_pBulletData->m_nContrailType == BULLET_CONTRAIL_TYPE_PARABOLA)
					{
						FLOAT fStartToEndDist = m_fStartToEndDist;
						FLOAT fTargetToStartDist = TDU_GetDist(fvTargetPos, m_fvStartPos);
						FLOAT fCenterToTargetDist = fabsf(fTargetToStartDist - fStartToEndDist/2.f);
						FLOAT fTemp = (fCenterToTargetDist * 2.f)/fStartToEndDist;
						FLOAT fAddY = (1.f - (fTemp * fTemp)) * m_pBulletData->m_fContrailParam;
						fvTargetPos.y += fAddY;
					}
					FLOAT fDir = TDU_GetYAngle( fVector2( GetPosition().x, GetPosition().z ), fVector2( fvTargetPos.x, fvTargetPos.z ) );
					SetPosition( fvTargetPos );
					SetFaceDir( fDir );
				}
			}
			break;
		case BULLET_CONTRAIL_TYPE_NONE:
		default:
			{
				AlreadyHit();
			}
			break;
		}
	}
}

VOID CObject_Bullet::AlreadyHit( VOID )
{
	if(m_idTarget != INVALID_ID)
	{
		CObject_Character *pChar = (CObject_Character*)(CObjectManager::GetMe()->FindServerObject(m_idTarget));
		if(pChar != NULL)
		{
			pChar->ShowLogicEvent(m_idSend, m_nSendLogicCount, TRUE);
		}
	}

	m_bAlreadyHit = TRUE;
	if ( m_pBulletData != NULL && strlen( m_pBulletData->m_szHitEffect ) > 0 )
	{
		ChangEffect( m_pBulletData->m_szHitEffect, FALSE );
	}
	else
	{
		if(m_pRenderInterface != NULL)
			m_pRenderInterface->Detach_Effect();
	}
}
