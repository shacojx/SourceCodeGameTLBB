// Obj_Effect.cpp
// 
/////////////////////////////////////////////////////

#include "StdAfx.h"
#include "TDTimeSystem.h"
#include "TDDBC_Struct.h"
#include "..\..\DBC\GMDataBase.h"
#include "..\..\Procedure\GameProcedure.h"
#include "TDException.h"
#include "..\ObjectManager.h"
#include "Obj_Effect.h"
#include "..\WxRender\RenderSystem.h"

WX_IMPLEMENT_DYNAMIC(CObject_Effect, GETCLASS(CObject_Surface));
CObject_Effect::CObject_Effect()
{
	m_bLoopEffect		= FALSE;
	m_fEffectTime		= 0.f;
}

CObject_Effect::~CObject_Effect()
{
	Assert( m_pRenderInterface == NULL && "CObject_Effect::~CObject_Effect" );
}

VOID CObject_Effect::Initial( VOID *pInit )
{
	CObject_Surface::Initial(pInit);

	SObject_EffectInit *pEffectInit = (SObject_EffectInit*)(pInit);
	if ( pEffectInit != NULL )
	{
		if ( pEffectInit->m_pszEffectName != NULL )
		{
			ChangEffect( pEffectInit->m_pszEffectName, pEffectInit->m_bLoopEffect );
		}
	}
	m_fEffectTime = 0;
}

VOID CObject_Effect::Release( VOID )
{
	if(m_pRenderInterface)
	{
		m_pRenderInterface->Destroy();
		m_pRenderInterface = NULL;
	}
	CObject_Surface::Release();
}

VOID CObject_Effect::Tick( VOID )
{
	m_fEffectTime += CGameProcedure::s_pTimeSystem->GetDeltaTime()/1000.0f;
	if ( m_pRenderInterface == NULL )
	{
		Destroy();
		return;
	}
	else if ( !m_bLoopEffect )
	{
		BOOL bEffectDie = IsEffectStopped();
		if ( bEffectDie )
		{
			Destroy();
			return;
		}
	}
}

BOOL CObject_Effect::IsEffectStopped( VOID )const
{
	if ( !m_bLoopEffect && m_fEffectTime > 5.f )
	{
		return TRUE;
	}
	return FALSE;
}

VOID CObject_Effect::ChangEffect( const CHAR *pszEffectName, BOOL bLoop )
{
	if ( pszEffectName != NULL )
	{
		if(!m_pRenderInterface)
			m_pRenderInterface = CGameProcedure::s_pGfxSystem->New_EntityObject(tEntityNode::ETYPE_DUMMY);

		m_pRenderInterface->Detach_Effect();
		m_pRenderInterface->Attach_Effect( pszEffectName );
	}
	else
	{
		if(m_pRenderInterface)
		{
			m_pRenderInterface->Destroy();
			m_pRenderInterface = NULL;
		}
	}
	m_bLoopEffect	= bLoop;
	m_fEffectTime	= 0;
	SetPosition( GetPosition() );
}
