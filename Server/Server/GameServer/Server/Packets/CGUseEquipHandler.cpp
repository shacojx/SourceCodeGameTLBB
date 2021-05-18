#include "stdafx.h"
#include "CGUseEquip.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "GCCharEquipment.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCUseEquipResult.h"
#include "ItemRuler.h"
#include "HumanItemLogic.h"
#include "ItemOperator.h"
#include "GCDetailEquipList.h"


using namespace Packets;

extern UINT GetEquipmentMaxLevelGemID(Item *pEquipment);





uint CGUseEquipHandler::Execute(CGUseEquip* pPacket, Player* pPlayer )
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

	//切场景时丢弃换装消息
	if(pGamePlayer->GetPlayerStatus()!=PS_SERVER_NORMAL ||
		!pHuman->IsActiveObj() )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGUseEquipmentHandler: change scene") ;
		return PACKET_EXE_CONTINUE;
	}

	BYTE  BagIndex = pPacket->getBagIndex();

	if( BagIndex == INVALID_BAG_INDEX )
	{
		Assert(FALSE) ; //包发错了
		return PACKET_EXE_CONTINUE ;
	}
		
	Item* pUseItem = HumanItemLogic::GetItem(pHuman,BagIndex);
	
	GCUseEquipResult Msg;

	//////////////////////////////////////////////////////////////////////////
	//装配条件判断
	UCHAR	itemClass = pUseItem->GetItemClass();

	if(itemClass == ICLASS_EQUIP)
	{
		//需求等级判断
		if(pUseItem->GetRequireLevel()>pHuman->GetLevel())
		{
			Msg.setResult(USEEQUIP_IDENT_FAIL);
			pGamePlayer->SendPacket( &Msg ) ;
		}
		//鉴定判断
		else if(pUseItem->GetItemQual() == EQUALITY_BLUE && pUseItem->GetItemIdent() == FALSE)
		{
			Msg.setResult(USEEQUIP_LEVEL_FAIL);
			pGamePlayer->SendPacket( &Msg ) ;
		}
		else
		{
			
			HUMAN_EQUIP		Equip_Point =(HUMAN_EQUIP)pUseItem->GetEquipPoint();
			//const	_ITEM*			pEquip = pHuman->GetEquip(Equip_Point);
			ItemContainer*	pEquipContainer = pHuman->GetEquipContain();
			
			if(!pEquipContainer)
			{
				Assert(pEquipContainer);
				return PACKET_EXE_CONTINUE;
			}

			Item*	pEquipItem = pEquipContainer->GetItem((UINT)Equip_Point);

			if(!pEquipItem)
			{
				Assert(pEquipItem);
				return PACKET_EXE_CONTINUE;

			}
			
			BOOL			bEquipStatus = (!pEquipItem->IsEmpty());

			ItemContainer*	pBagContainer =
				HumanItemLogic::GetContainer(pHuman,BagIndex);

			if(bEquipStatus)
			{
				//装备上物品,替换下来装备点数据
				
				g_ItemOperator.ExchangeItem(pBagContainer,
											pBagContainer->BagIndex2ConIndex(BagIndex),
											pEquipContainer,
											(UINT)Equip_Point);


			}
			else
			{ 
				//pHuman->GetDB()->OverWriteBag2Equip(BagIndex,Equip_Point);
				g_ItemOperator.MoveItem(pBagContainer,
										pBagContainer->BagIndex2ConIndex(BagIndex),
										pEquipContainer,
										(UINT)Equip_Point);

							
			}
			
			Item* pEquip = pEquipContainer->GetItem((UINT)Equip_Point);
			
			if(pEquip&&pEquip->IsCanEquipBind())
			{
				g_ItemOperator.SetItemBind(pEquipContainer,(UINT)Equip_Point);	
			}

			

			Msg.setBagIndex(BagIndex);
			Msg.setEquipPoint(Equip_Point);
			Msg.setEquipID(pEquipItem->GetGUID());
			Msg.setEquipResID(pEquipItem->GetItemTableIndex());
			Msg.setItemResID(pUseItem->GetItemTableIndex());
			Msg.setBagItemId(pUseItem->GetGUID());
			Msg.setResult(USEEQUIP_SUCCESS);
			
			pGamePlayer->SendPacket( &Msg ) ;
			pHuman->SetEquipVer(pHuman->GetEquipVer()+1);
			//刷新装备对人物的属性影响

			pHuman->ItemEffectFlush();

			//如果可见
			if(pHuman->IsVisualPart(Equip_Point))
			{	
				GCCharEquipment OtherMsg;
				OtherMsg.setObjID(pHuman->GetID());
				if(Equip_Point == HEQUIP_WEAPON)
				{
					UINT uGemID = GetEquipmentMaxLevelGemID(pEquip);
					OtherMsg.setID(Equip_Point,pEquip->GetItemTableIndex(), uGemID);
				}
				else
				{
					OtherMsg.setID(Equip_Point,pEquip->GetItemTableIndex(), -1);
				}
				pScene->BroadCast(&OtherMsg,pHuman,TRUE);

			}


			WORD RetPart = 0;
			GCDetailEquipList	SelfMsg;
			SelfMsg.setObjID( pHuman->GetID() );
			pEquipItem = HumanItemLogic::GetEquip(pHuman,Equip_Point);
			if(!pEquipItem)
			{
				Assert(FALSE);
				return PACKET_EXE_CONTINUE;
			}
			if(pEquipItem->IsEmpty()) 
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGUseEquipmentHandler: Equip_Point =%d Fail", Equip_Point ) ;
				return PACKET_EXE_CONTINUE;
			}
				
			pEquipItem->SaveValueTo(SelfMsg.GetEquipData(Equip_Point));
			RetPart|=(1<<((INT)Equip_Point));
			SelfMsg.SetAskPart(RetPart);
			pGamePlayer->SendPacket( &SelfMsg ) ;
		}
		
		
		
		
		
	}
	else
	{
		Msg.setResult(USEEQUIP_TYPE_FAIL);
		Msg.setBagIndex(0);
		Msg.setEquipPoint(0);
		pGamePlayer->SendPacket( &Msg ) ;
		
	}
	
	

	
	

	g_pLog->FastSaveLog( LOG_FILE_1, "CGUseEquipmentHandler: BagIndex=%d", BagIndex ) ;

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}