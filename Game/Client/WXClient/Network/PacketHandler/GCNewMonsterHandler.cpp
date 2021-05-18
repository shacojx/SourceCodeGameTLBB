#include "StdAfx.h"
#include "TDTimeSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerNPC.h"

#include "..\..\NetWork\NetManager.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "CGCharAskBaseAttrib.h"
#include "GCNewMonster.h"
#include "..\..\World\WorldManager.h"
#include "TDException.h"
#include "AxProfile.h"
#include "TDVariable.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Object\GMObjectLoadQueue.h"
#include "..\WxRender\RenderSystem.h"

uint GCNewMonsterHandler :: Execute( GCNewMonster* pPacket, Player* pPlayer ) 
{
//__ENTER_FUNCTION
	//AxTrace(0, 2, "GCNewMonsterHandler");
	//return PACKET_EXE_CONTINUE;

//	AxTrace(0, 2, "********NewMonster Begin %d", pPacket->getObjID());
	//当前流程是主流程
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
//		AxProfile::AxProfile_PushNode("NewMonster");

		CObjectManager* pObjectManager = CObjectManager::GetMe();
		//检查位置是否合法
		if(!CWorldManager::GetMe()->GetActiveScene()->IsValidPosition(fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ)))
		{
			TDThrow("ERROR POSITION @ GCNewMonsterHandler");
		}

		//创建玩家
		CObject_PlayerNPC* pNPC = (CObject_PlayerNPC*)pObjectManager->FindServerObject( (INT)pPacket->getObjID() );
		fVector3 fvGame;
		CGameProcedure::s_pGfxSystem->Axis_Trans(
			CRenderSystem::AX_PLAN, fVector3(pPacket->getWorldPos().m_fX, 0.0f, pPacket->getWorldPos().m_fZ),
			CRenderSystem::AX_GAME, fvGame);

		FLOAT fFaceDir = pPacket->getDir();
		if(pNPC==NULL)
		{
//			AxProfile::AxProfile_PushNode("NewMonster_CreateObject");
			pNPC = pObjectManager->NewPlayerNPC( (INT)pPacket->getObjID() );
//			AxProfile::AxProfile_PopNode("NewMonster_CreateObject");

			SObjectInit initPlayerNPC;

			initPlayerNPC.m_fvPos = fvGame;
			initPlayerNPC.m_fvRot = fVector3( 0.f, fFaceDir, 0.f );

//			AxProfile::AxProfile_PushNode("NewMonster_InitObject");
			pNPC->Initial( &initPlayerNPC );
//			AxProfile::AxProfile_PopNode("NewMonster_InitObject");
		}
		else
		{
			fVector2 mapPos(fvGame.x, fvGame.z);
			pNPC->SetMapPosition(mapPos);
			pNPC->SetFaceDir( fFaceDir );
			pNPC->Enable( OSF_VISIABLE );
			pNPC->Disalbe( OSF_OUT_VISUAL_FIELD );
		}
		if( pPacket->getIsNPC() )
			pNPC->SetNpcType(NPC_TYPE_NPC);
		else
			pNPC->SetNpcType(NPC_TYPE_MONSTER);

		pNPC->GetCharacterData()->Set_MoveSpeed(pPacket->getMoveSpeed());

		SCommand_Object cmdTemp;
		cmdTemp.m_wID			= OC_IDLE;
		cmdTemp.m_afParam[0]	= fvGame.x;
		cmdTemp.m_afParam[1]	= fvGame.z;
		cmdTemp.m_abParam[2]	= FALSE;	
		pNPC->PushCommand(&cmdTemp );

		//放入Ask队列
		CObjectManager::GetMe()->GetLoadQueue()->TryAddLoadTask(pNPC->GetID(), CObject_Character::CT_MONSTER);

		// tempcode{
		// 此版不做服务器繁忙客户端延后发消息的处理
		//CGCharAskBaseAttrib msgAskBaseAttrib;
		//msgAskBaseAttrib.setTargetID( (INT)pPacket->getObjID() );
		//CNetManager::GetMe()->SendPacket( &msgAskBaseAttrib );

		// }

		//同步渲染层
		char szTemp[MAX_PATH];
		_snprintf(szTemp, MAX_PATH, "GCNewMonster(%.1f,%.1f)", 
			pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ);
		pNPC->PushDebugString(szTemp);
		pNPC->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());

//		AxProfile::AxProfile_PopNode("NewMonster");
		//AxTrace(0, 2, "********NewMonster End");
	}

	return PACKET_EXE_CONTINUE ;

//__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
//__ENTER_FUNCTION
//	AxTrace(0, 2, "GCNewMonsterHandler");
//
//	//当前流程是主流程
//	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
//	{
//		CObjectManager* pObjectManager = CObjectManager::GetMe();
//
//		//创建玩家
//		CObject_PlayerNPC* pOther = pObjectManager->CreateNPC(
//											(INT)pPacket->getObjID(),
//											(INT)pPacket->getRace(),
//											fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ));
//
//		if(!pOther) return PACKET_EXE_CONTINUE;
//
//		//设置新的位置
//		pOther->SetMapPosition(fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ));
//		pOther->RegisterToZone();
//		//同步渲染层
//		pOther->GetRenderInterface()->SetPosition( pOther->GetPosition() );
//		//IDLE状态
//		pOther->SetMotionStatus(CMS_STATUS_IDLE);
//		char szTemp[MAX_PATH];
//		_snprintf(szTemp, MAX_PATH, "GCNewMonster(%.1f,%.1f)", 
//			pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ);
//		pOther->PushDebugString(szTemp);
//		pOther->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
//	}
//
//	return PACKET_EXE_CONTINUE ;
//
//__LEAVE_FUNCTION
//
//	return PACKET_EXE_ERROR ;
}

