#include "StdAfx.h"
#include "TDTimeSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerOther.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "..\..\NetWork\NetManager.h"
#include "CGCharAskBaseAttrib.h"
#include "GCNewPlayer.h"
#include "..\..\World\WorldManager.h"
#include "TDException.h"
#include "..\..\Global.h"
#include "TDDEbuger.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Object\GMObjectLoadQueue.h"
#include "..\WxRender\RenderSystem.h"

uint GCNewPlayerHandler :: Execute( GCNewPlayer* pPacket, Player* pPlayer ) 
{
//__ENTER_FUNCTION

	//当前流程是主流程
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();
		//检查位置是否合法
		if(!CWorldManager::GetMe()->GetActiveScene()->IsValidPosition(fVector2(pPacket->getWorldPos().m_fX, pPacket->getWorldPos().m_fZ)))
		{
			TDThrow("ERROR POSITION @ GCNewPlayerHandler");
		}

		//创建玩家
		CObject_PlayerOther* pOther = (CObject_PlayerOther*)(pObjectManager->FindServerObject( (INT)pPacket->getObjID()));
		fVector3 fvGame;
		CGameProcedure::s_pGfxSystem->Axis_Trans(
			CRenderSystem::AX_PLAN, fVector3(pPacket->getWorldPos().m_fX, 0.0f, pPacket->getWorldPos().m_fZ),
			CRenderSystem::AX_GAME, fvGame);

		FLOAT fFaceDir = pPacket->getDir();

		if ( pOther == NULL )
		{
			pOther = pObjectManager->NewPlayerOther( (INT)pPacket->getObjID() );

			SObjectInit initPlayerOther;
			initPlayerOther.m_fvPos	= fvGame;
			initPlayerOther.m_fvRot = fVector3( 0.f, fFaceDir, 0.f );
			pOther->Initial( &initPlayerOther );

			fVector2 mapPos(fvGame.x, fvGame.z);
			pOther->SetMapPosition( mapPos );
		}
		else
		{
			fVector2 mapPos(fvGame.x, fvGame.z);
			pOther->SetMapPosition( mapPos );
			pOther->SetFaceDir( fFaceDir );
			pOther->Enable( OSF_VISIABLE );
			pOther->Disalbe( OSF_OUT_VISUAL_FIELD );
		}

		//更新装备信息
		pOther->GetCharacterData()->Set_EquipVer(pPacket->getEquipVer());

		pOther->GetCharacterData()->Set_MoveSpeed(pPacket->getMoveSpeed());

		////激活鼠标查询
		//if(pOther->GetRenderInterface())
		//{
		//	pOther->Enable(OSF_RAY_QUERY);
		//}

		SCommand_Object cmdTemp;
		cmdTemp.m_wID			= OC_IDLE;
		cmdTemp.m_afParam[0]	= fvGame.x;
		cmdTemp.m_afParam[1]	= fvGame.z;
		cmdTemp.m_abParam[2]	= FALSE;
		pOther->PushCommand(&cmdTemp );

		//放入Ask队列
		CObjectManager::GetMe()->GetLoadQueue()->TryAddLoadTask(pOther->GetID(), CObject_Character::CT_PLAYEROTHER);

		// tempcode{
		// 此版不做服务器繁忙客户端延后发消息的处理
//		CGCharAskBaseAttrib msgAskBaseAttrib;
//		msgAskBaseAttrib.setTargetID( (INT)pPacket->getObjID() );
//		CNetManager::GetMe()->SendPacket( &msgAskBaseAttrib );

		//AxTrace(0, 0, "[%d]Ask %d", m_hmObject.size(), pPacket->getObjID());
		// }

		//如果是自己，退出死亡效果
		if(pOther == (CObject_PlayerOther*)pObjectManager->GetMySelf())
		{
			CGameProcedure::s_pGfxSystem->Scene_SetPostFilter_Death(FALSE);
		}

		pOther->PushDebugString("GCNewPlayer");
		pOther->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
	}

	return PACKET_EXE_CONTINUE ;

//__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

