#include "stdafx.h"
#include "CGOpenItemBox.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "Obj_ItemBox.h"
#include "GCBoxItemList.h"
#include "GCPickResult.h"

using namespace Packets;


uint CGOpenItemBoxHandler::Execute(CGOpenItemBox* pPacket, Player* pPlayer )
{

	__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;


	ObjID_t	 ObjID	=	pPacket->getObjID();

	if( ObjID == INVALID_ID )
	{
			Assert(FALSE) ;
			return PACKET_EXE_ERROR ;
	}

	Obj* pObj = pScene->GetObjManager()->GetObj( ObjID ) ;

	if( pObj==NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGOpenItemBoxHandler: not find obj=%d", ObjID ) ;
		return PACKET_EXE_CONTINUE ;
	}

	Obj_ItemBox*	pItemBox = (Obj_ItemBox*)pObj;

	if( pItemBox->GetObjType()!= Obj::OBJ_TYPE_ITEM_BOX)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGOpenItemBoxHandler: not Obj_ItemBox obj=%d", ObjID ) ;
		return PACKET_EXE_CONTINUE ;
	}

	if(pHuman->IsInValidRadius(pObj->getWorldPos(),MAX_BOX_OP_DISTANCE)==FALSE)
	{
		GCPickResult	Msg;
		Msg.setResult(PICK_TOO_FAR);
		pGamePlayer->SendPacket( &Msg );
		return PACKET_EXE_CONTINUE ;
	}

	if(!pHuman->IsAlive())
	{
		GCPickResult	Msg;
		Msg.setResult(PICK_DEAD);
		pGamePlayer->SendPacket( &Msg );
		return PACKET_EXE_CONTINUE ;
	}
	
	if(pItemBox->CanPickBox(pHuman->GetGUID(),pHuman->GetID()))
	{
		
		GCBoxItemList	Msg;
		
		int j = 0;
		for(int i =0;i<MAX_BOXITEM_NUMBER;i++)
		{
			_ITEM* pItem = pItemBox->GetItem(i);
			Assert(pItem);

			if(pItem->IsNullType())
				continue;
			
			Msg.setItemData(pItem,j);
			j++;
		}


		Msg.setItemNum(pItemBox->GetItemCount());
		Msg.setItemBoxId(pItemBox->GetID());
		Msg.setItemBoxType(pItemBox->GetType());
		pGamePlayer->SendPacket( &Msg ) ;
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGOpenItemBoxHandler: ok ObjId = %d", ObjID ) ;

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;

}