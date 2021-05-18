#include "stdafx.h"
#include "CGDiscardEquip.h"



#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCDiscardEquipResult.h"
#include "GCCharEquipment.h"
#include "ItemRuler.h"
#include "ItemOperator.h"
#include "HumanItemLogic.h"



uint CGDiscardEquipHandler::Execute(CGDiscardEquip* pPacket, Player* pPlayer )
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


	HUMAN_EQUIP		EquipPoint = (HUMAN_EQUIP)pPacket->GetEquipPoint();

	//Assert( pHuman->GetDB()->GetEquipDB()->IsSet(EquipPoint)) ;

	Item*		pEquip	=	HumanItemLogic::GetEquip(pHuman,EquipPoint);
	
	if(!pEquip)
	{
		Assert(pEquip);
		return PACKET_EXE_CONTINUE;
	}
	
	GCDiscardEquipResult	Msg;

	if(pEquip->IsEmpty())
	{
		Msg.SetResult(DISCARDEQUIP_NO_SUCH_EQUIP);

	}
	
	if(pEquip->IsRuler(IRL_DISCARD))
	{
		Msg.SetItemTableIndex(pEquip->GetItemTableIndex());
		ITEM_LOG_PARAM	ItemLogParam;
		ItemLogParam.OpType		=	ITEM_DISCARD_EQUIP;
		ItemLogParam.CharGUID	=	pHuman->GetGUID();
		ItemLogParam.BagPos		=	EquipPoint;
		ItemLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
		ItemLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
		ItemLogParam.SceneID	=	pHuman->getScene()->SceneID();	

		BOOL bRet =	g_ItemOperator.EraseItem(pHuman->GetEquipContain(),EquipPoint);

		if(bRet) SaveItemLog(&ItemLogParam);

		pHuman->SetEquipVer(pHuman->GetEquipVer()+1);
		pHuman->ItemEffectFlush();
		Msg.SetResult(DISCARDEQUIP_SUCCESS);
		Msg.SetEquipPoint(EquipPoint);
		//如果可见
		if(pHuman->IsVisualPart(EquipPoint))
		{	
			GCCharEquipment OtherMsg;
			OtherMsg.setObjID(pHuman->GetID());
			OtherMsg.setID(EquipPoint,-1, -1);
			pScene->BroadCast(&OtherMsg,pHuman,TRUE);
		}
		
	}
	else
	{
		Msg.SetResult(DISCARDEQUIP_CANNT_DISCARD);

	}
	

	pGamePlayer->SendPacket(&Msg);	

	g_pLog->FastSaveLog( LOG_FILE_1, "CGDiscardEquipHandler: ok EquipPoint=%d", 
		EquipPoint ) ;
	
	
	return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}