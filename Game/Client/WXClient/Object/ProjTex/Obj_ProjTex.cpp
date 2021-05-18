#include "StdAfx.h"
#include "Obj_ProjTex.h"
#include "../../Global.h"
#include "../ObjectManager.h"
#include "../Character/Obj_PlayerMySelf.h"
#include "TDException.h"
#include "TDTimeSystem.h"
#include "TDUtil.h"
#include "../../Procedure/GameProcedure.h"
#include "../Character/AI/GMAI_MySelf.h"
#include "TDInputSystem.h"
#include "..\WxRender\RenderSystem.h"

WX_IMPLEMENT_DYNAMIC_VIRTUAL(CObject_ProjTex, GETCLASS(CObject_Static));
FLOAT CObject_ProjTex::PROJTEX_HEIGHT = 2.0f;
CObject_ProjTex::CObject_ProjTex()
{
}

CObject_ProjTex::~CObject_ProjTex()
{
}


VOID CObject_ProjTex::Initial(VOID*)
{
	if ( m_pRenderInterface == NULL )
	{
		//创建渲染层实体
		m_pRenderInterface = CGameProcedure::s_pGfxSystem->New_EntityObject(tEntityNode::ETYPE_DUMMY);
		m_pRenderInterface->SetData(GetID());

	}
}

VOID CObject_ProjTex::Release( VOID )
{
	if ( m_pRenderInterface != NULL )
	{
		m_pRenderInterface->Destroy();
		m_pRenderInterface = NULL;
	}
	CObject_Static::Release();
}

VOID CObject_ProjTex::SetHeight(FLOAT fHeight)
{
}

//======================================================
WX_IMPLEMENT_DYNAMIC(CObject_ProjTex_MouseTarget, GETCLASS(CObject_ProjTex));

CObject_ProjTex_MouseTarget::CObject_ProjTex_MouseTarget(void)
{
	m_uEnableTime		= 0;
}

CObject_ProjTex_MouseTarget::~CObject_ProjTex_MouseTarget(void)
{
}

VOID CObject_ProjTex_MouseTarget::Tick(VOID)
{
	FLOAT fDistSq = TDU_GetDistSq(GetPosition(), CObjectManager::GetMe()->GetMySelf()->GetPosition());
	UINT uTime = CGameProcedure::s_pTimeSystem->GetTimeNow();
	//如果主角已经达到或是Visible的标记已为FALSE，隐藏
	if(	(!(CObjectManager::GetMe()->GetMySelf()->IsMoving()) && (uTime - m_uEnableTime > 2000))
		|| (fDistSq < 1.0f) )
	{
		m_pRenderInterface->Attach_ProjTexture(tEntityNode::REACHABLE, FALSE, 100, PROJTEX_HEIGHT);
		m_pRenderInterface->Attach_ProjTexture(tEntityNode::UNREACHABLE, FALSE, 100, PROJTEX_HEIGHT);
	}
}

VOID CObject_ProjTex_MouseTarget::UpdateAsCursor(void)
{
	POINT pt = CGameProcedure::s_pInputSystem->MouseGetPos();

	fVector3 fvMousePos;
	CObjectManager::GetMe()->GetMouseOverObject(pt.x, pt.y, fvMousePos);

	SetMapPosition(fVector2(fvMousePos.x, fvMousePos.z));
}

VOID CObject_ProjTex_MouseTarget::SetReachAble(BOOL bReachAble)
{
	if(!m_pRenderInterface) return;

	m_uEnableTime = CGameProcedure::s_pTimeSystem->GetTimeNow();
	if(bReachAble)
	{
		m_pRenderInterface->Attach_ProjTexture(tEntityNode::REACHABLE, TRUE, 100, PROJTEX_HEIGHT);
		m_pRenderInterface->Attach_ProjTexture(tEntityNode::UNREACHABLE, FALSE, 100, PROJTEX_HEIGHT);
	}
	else
	{
		m_pRenderInterface->Attach_ProjTexture(tEntityNode::REACHABLE, FALSE, 100, PROJTEX_HEIGHT);
		m_pRenderInterface->Attach_ProjTexture(tEntityNode::UNREACHABLE, TRUE, 100, PROJTEX_HEIGHT);
	}
}

//======================================================
WX_IMPLEMENT_DYNAMIC(CObject_ProjTex_AuraDure, GETCLASS(CObject_ProjTex));

VOID CObject_ProjTex_AuraDure::SetShowEnable(BOOL bEnable) 
{ 
	if(m_bShowEnable == bEnable) return;
	m_bShowEnable = bEnable; 

	if(m_bShowEnable)
	{
		m_pRenderInterface->Attach_ProjTexture(tEntityNode::AURA_RUNE, TRUE, m_fRingRange);
	}
	else
	{
		m_pRenderInterface->Attach_ProjTexture(tEntityNode::AURA_RUNE, FALSE, m_fRingRange);
	}
}

VOID CObject_ProjTex_AuraDure::Tick(VOID)
{

}

VOID CObject_ProjTex_AuraDure::SetRingRange(FLOAT fRingRange) 
{ 
	if(m_fRingRange == fRingRange) return;
	m_fRingRange = fRingRange; 

	if(m_bShowEnable)
	{
		m_pRenderInterface->Attach_ProjTexture(tEntityNode::AURA_RUNE, TRUE, m_fRingRange);
	}
	else
	{
		m_pRenderInterface->Attach_ProjTexture(tEntityNode::AURA_RUNE, FALSE, m_fRingRange);
	}
}