#include "stdafx.h"
#include "CGUnEquip.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"

#include "GCUnEquipResult.h"
#include "GCCharEquipment.h"
#include "HumanItemLogic.h"
#include "ItemOperator.h"

extern UINT GetEquipmentMaxLevelGemID(Item *pEquipment);

uint CGUnEquipHandler::Execute( CGUnEquip* pPacket, Player* pPlayer )
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

	HUMAN_EQUIP	EquipPoint = (HUMAN_EQUIP)pPacket->getEquipPoint();

	//Assert( pHuman->GetDB()->GetEquipDB()->IsSet(EquipPoint) ) ;



	Item*	pEquipItem = HumanItemLogic::GetEquip(pHuman,EquipPoint);
	
	if(!pEquipItem)
	{
		Assert(pEquipItem);
		return	PACKET_EXE_CONTINUE;
	}
	if(pEquipItem->IsEmpty())
	{
		return	PACKET_EXE_CONTINUE;
	}

	UCHAR	itemClass = pEquipItem->GetItemClass();
	if(itemClass ==	ICLASS_EQUIP)
	{
		INT		BagIndex;//	=	pHuman->GetFirstEmptyBagIndex();

		ItemContainer*	pEquipContainer = pHuman->GetEquipContain();
		ItemContainer*	pBaseContainer	= pHuman->GetBaseContain();

		if(!pEquipContainer)
		{
			Assert(pEquipItem);
			return	PACKET_EXE_CONTINUE;
		}
		
		BYTE	DeliverBagIndex = pPacket->getBagIndex();

		if(DeliverBagIndex<pBaseContainer->GetContainerSize())
		{
			BagIndex	=	g_ItemOperator.MoveItem(pEquipContainer,
				EquipPoint,
				pBaseContainer,DeliverBagIndex);
		}
		else
		{
			BagIndex	=	g_ItemOperator.MoveItem(pEquipContainer,
							EquipPoint,
							pBaseContainer);
		}
		

		GCUnEquipResult	 Msg;
		if(BagIndex>=0
			&& BagIndex<pBaseContainer->GetContainerSize())
		{
			Msg.setEquipPoint(EquipPoint);
			Msg.setBagIndex(BagIndex);
			Msg.setItemID(pEquipItem->GetGUID());
			Msg.setItemTableIndex(pEquipItem->GetItemTableIndex());
			Msg.setResult(UNEQUIP_SUCCESS);
			pHuman->SetEquipVer(pHuman->GetEquipVer()+1);
			pHuman->ItemEffectFlush();

			pGamePlayer->SendPacket( &Msg ) ;

			//如果可见
			if(pHuman->IsVisualPart(EquipPoint))
			{	
				GCCharEquipment OtherMsg;
				OtherMsg.setObjID(pHuman->GetID());
				//if(EquipPoint == HEQUIP_WEAPON)
				//{
				//	UINT uGemID = GetEquipmentMaxLevelGemID(pEquipItem);
				//	OtherMsg.setID(EquipPoint,pEquipItem->GetItemTableIndex(), uGemID);
				//}
				//else
				//{
					OtherMsg.setID(EquipPoint,pEquipItem->GetItemTableIndex(), -1);
				//}
				pScene->BroadCast(&OtherMsg,pHuman,TRUE);
			}
		}
		else
		{
			if(ITEMOE_DESTOPERATOR_HASITEM == BagIndex)
			{
				Msg.setResult(UNEQUIP_HAS_ITEM);
				pGamePlayer->SendPacket( &Msg ); 
			}
			else if(ITEMOE_DESTOPERATOR_FULL == BagIndex)
			{
				Msg.setResult(UNEQUIP_BAG_FULL);
				pGamePlayer->SendPacket( &Msg ); 
			}
		}
		
	}
	else
	{
		Assert(FALSE);
		//装备为什么不是 ICLASS_EQUIP
	}
	
	g_pLog->FastSaveLog( LOG_FILE_1, "CGUnEquipHandler EquipPoint=%d itemClass=%d",
		EquipPoint, itemClass  ) ;

	
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}