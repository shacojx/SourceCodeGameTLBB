#include "StdAfx.h"
#include "GMTripperObj_Transport.h"
#include "../../Global.h"
#include "../ObjectManager.h"
#include "../../Procedure/GameProcedure.h"
#include "..\WxRender\RenderSystem.h"

WX_IMPLEMENT_DYNAMIC(CTripperObject_Transport, GETCLASS(CTripperObject));
CTripperObject_Transport::CTripperObject_Transport()
{
	m_pRenderInterface2 = NULL;
}


CTripperObject_Transport::~CTripperObject_Transport()
{
}

VOID CTripperObject_Transport::Initial(VOID*)
{
	if ( m_pRenderInterface == NULL )
	{
		//创建渲染层实体
		m_pRenderInterface = CGameProcedure::s_pGfxSystem->New_EntityObject(tEntityNode::ETYPE_MESH);
		m_pRenderInterface->StaObj_SetMeshFile("Cast/Cast1.mesh");
		m_pRenderInterface->SetData(GetID());

		m_pRenderInterface2 = CGameProcedure::s_pGfxSystem->New_EntityObject(tEntityNode::ETYPE_MESH);
		m_pRenderInterface2->StaObj_SetMeshFile("Cast/pilon.mesh");
		m_pRenderInterface2->SetData(GetID());
	}
}


VOID CTripperObject_Transport::SetPosition(const fVector3& fvPosition) 
{ 
	CObject_Static::SetPosition(fvPosition);
	//在渲染层同步位置
	if(m_pRenderInterface2)
		m_pRenderInterface2->SetPosition( GetPosition() );
}

VOID CTripperObject_Transport::Release( VOID )
{
	if ( m_pRenderInterface != NULL )
	{
		m_pRenderInterface->Destroy();
		m_pRenderInterface = NULL;

		m_pRenderInterface2->Destroy();
		m_pRenderInterface2 = NULL;
	}
	CObject_Static::Release();
}

