#include "StdAfx.h"
#include "GCDelObject.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerOther.h"
#include "..\..\GameCommand.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "TDTimeSystem.h"
#include "..\..\Global.h"
#include "..\..\World\WorldManager.h"
#include "TDDBC_Struct.h"

uint GCDelObjectHandler :: Execute( GCDelObject* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION
	//AxTrace(0, 2, "GCNewMonsterHandler");

	//当前流程是主流程
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		//判断是否是本场景
		if(CWorldManager::GetMe()->GetActiveScene()->GetSceneDefine()->nServerID != pPacket->getSceneID())
		{
			AxTrace(0, 2, "DelObject:SceneID Error!(%d != %d)", 
				pPacket->getSceneID(), 
				CWorldManager::GetMe()->GetActiveScene()->GetSceneDefine()->nServerID);
			return PACKET_EXE_CONTINUE;
		}

		//寻找obj
		CObject_Character* pObj = (CObject_Character*)(pObjectManager->FindServerObject( (INT)pPacket->getObjID() ));
		if ( pObj == NULL )
			return PACKET_EXE_CONTINUE;

		if(pObj == CObjectManager::GetMe()->GetMainTarget())
		{
			CObjectManager::GetMe()->SetMainTarget(-1);
		}

		pObj->Release();
		CObjectManager::GetMe()->DestroyObject( pObj );
		//之后不能执行任何代码
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

