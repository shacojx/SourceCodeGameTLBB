// GCNewSpecialHandler.cpp
#include "StdAfx.h"

#include "TDTimeSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\NetWork\NetManager.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "..\..\Object\Surface\Obj_Special.h"
#include "..\..\World\WorldManager.h"
#include "TDException.h"
#include "AxProfile.h"
#include "TDVariable.h"

#include "GCNewSpecial.h"


uint GCNewSpecialHandler::Execute( GCNewSpecial* pPacket, Player* pPlayer )
{
//__ENTER_FUNCTION
	//当前流程是主流程
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();
		
		//检查位置是否合法
		if(!CWorldManager::GetMe()->GetActiveScene()->IsValidPosition(fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ)))
		{
			TDThrow("ERROR POSITION @ GCNewSpecialHandler");
		}

		CObject_Special* pSpecial = (CObject_Special*)pObjectManager->FindServerObject( (INT)pPacket->getObjID() );
		if ( pSpecial == NULL )
		{
			//创建平台生长点
			pSpecial = (CObject_Special*)CObjectManager::GetMe()->NewObject( "CObject_Special", pPacket->getObjID());
		}
		else
		{
			pSpecial->Release();
		}

		SObject_SpecialInit initSpecial;
		initSpecial.m_fvPos.x	= pPacket->getWorldPos().m_fX;
		initSpecial.m_fvPos.z	= pPacket->getWorldPos().m_fZ;
		initSpecial.m_fvPos.y	= 0.f;
		initSpecial.m_fvRot		= fVector3( 0.f, pPacket->getDir(), 0.f );
		initSpecial.m_nDataID	= pPacket->getDataID();
		pSpecial->Initial( &initSpecial );
		pSpecial->SetMapPosition( fVector2( pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ ) );

		pSpecial->PushDebugString("GCNewSpecialHandler");
		pSpecial->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
	}

	return PACKET_EXE_CONTINUE ;

//__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
