#include "stdafx.h"
#include "CGRemoveGem.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"

#include "GCRemoveGemResult.h"
#include "GCNotifyEquip.h"
#include "HumanItemLogic.h"


uint CGRemoveGemHandler::Execute(CGRemoveGem* pPacket,Player* pPlayer)
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

	BYTE	EquipBagIndex =	pPacket->GetEquipBagIndex();
	BYTE	GemIndex	  =	pPacket->GetGemIndex();	

	Assert(EquipBagIndex<MAX_BAG_SIZE);

	Assert(GemIndex<MAX_ITEM_GEM);

	
	REMOVEGEM_RESULT	Result = (REMOVEGEM_RESULT)pHuman->RemoveGem(EquipBagIndex,GemIndex);
	

	if(Result==REMOVEGEM_SUCCESS)
	{

		Item*	pGemItem = HumanItemLogic::GetItem(pHuman,EquipBagIndex);
		Assert(pGemItem);

		GCNotifyEquip	Msg;
		Msg.SetBagIndex(EquipBagIndex);
		pGemItem->SaveValueTo(Msg.GetItem());
		pGamePlayer->SendPacket(&Msg);
		
	}
	
	GCRemoveGemResult	Msg;
	Msg.SetResult(Result);


	pGamePlayer->SendPacket(&Msg);



	g_pLog->FastSaveLog( LOG_FILE_1, "CGRemoveGemHandler: ok EquipBagIndex=%d GemIndex=%d", 
		EquipBagIndex, GemIndex ) ;


	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}