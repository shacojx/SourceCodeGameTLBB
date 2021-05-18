#include "stdafx.h"
/*
客户端同意交易，可以换了
*/

#include "CGExchangeOkIII.h"
#include "GCExchangeSuccessIII.h"
#include "GCExchangeError.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"

#include "ItemOperator.h"
#include "HumanItemLogic.h"
#include "Obj_Pet.h"

uint CGExchangeOkIIIHandler::Execute( CGExchangeOkIII* pPacket, Player* pPlayer )
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

	EXCHANGE_CERTIFY_EACH_OTHER(pHuman)
	ObjID_t	DestID = pHuman->m_ExchangBox.m_ObjID;
	Obj_Character *pTarget = (Obj_Character*)(pScene->GetObjManager()->GetObj( DestID ));
	if(pTarget->GetObjType() != Obj::OBJ_TYPE_HUMAN)
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeOkIIIHandler: %s ask not a human.", pHuman->GetName() ) ;
		return	PACKET_EXE_CONTINUE;
	}
	Obj_Human* pDestHuman = (Obj_Human*)pTarget;
	if( pDestHuman == NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_CONTINUE;
	}

	// 不同阵营，不让查看
	if( pHuman->IsEnemy( pDestHuman ) )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExchangeOkIIIHandler: %s cann't ask %s's detailattr ", pHuman->GetName(), pDestHuman->GetName() ) ;
		return	PACKET_EXE_CONTINUE;
	}

	if(pHuman->m_ExchangBox.m_Status == ServerExchangeBox::EXCHANGE_WAIT_FOR_CONFIRM)
	{
		//设置可以交换
		pHuman->m_ExchangBox.m_Status = ServerExchangeBox::EXCHANGE_CONFIRM_READY;
		if(pDestHuman->m_ExchangBox.m_Status ==  ServerExchangeBox::EXCHANGE_CONFIRM_READY)
		{//可以交换了

			//1.变量初始化
			ItemContainer*		pMyExchangeContainer = &(pHuman->m_ExchangBox.m_Container);
			ItemContainer*		pOtExchangeContainer = &(pDestHuman->m_ExchangBox.m_Container);
			ItemContainer*		pMyExchangePetContainer = &(pHuman->m_ExchangBox.m_PetContainer);
			ItemContainer*		pOtExchangePetContainer = &(pDestHuman->m_ExchangBox.m_PetContainer);

			_EXCHANGE_ITEM_LIST			ItemListMeToOt;
			_EXCHANGE_ITEM_LIST			ItemListOtToMe;

			ItemListMeToOt.Init();
			ItemListOtToMe.Init();

			//2.验证循环
			g_pLog->FastSaveLog( LOG_FILE_1, "<交易> [%s] 与 [%s] 开始验证循环",	pHuman->GetName(), pDestHuman->GetName()) ;
			for(INT i = 0; i<EXCHANGE_BOX_SIZE; i++)
			{
				if(pMyExchangeContainer->GetItem(i)->IsEmpty() == FALSE)
				{
					ItemListMeToOt.AddItem( pMyExchangeContainer->GetItem(i) );
				}
				if(pOtExchangeContainer->GetItem(i)->IsEmpty() == FALSE)
				{
					ItemListOtToMe.AddItem( pOtExchangeContainer->GetItem(i) );
				}
			}

			for(INT i = 0; i<EXCHANGE_PET_BOX_SIZE; i++)
			{
				if(pMyExchangePetContainer->GetItem(i)->IsEmpty() == FALSE)
				{
					ItemListMeToOt.AddItem( pMyExchangePetContainer->GetItem(i) );
				}
				if(pOtExchangePetContainer->GetItem(i)->IsEmpty() == FALSE)
				{
					ItemListOtToMe.AddItem( pOtExchangePetContainer->GetItem(i) );
				}
			}

			if( FALSE == HumanItemLogic::CanReceiveExchangeItemList( pHuman, ItemListOtToMe) )
			{//空间不够
				GCExchangeError MsgSelf;
				MsgSelf.SetID(EXCHANGE_MSG::ERR_NOT_ENOUGHT_ROOM_SELF);
				pHuman->GetPlayer()->SendPacket(&MsgSelf);

				GCExchangeError MsgOther;
				MsgOther.SetID(EXCHANGE_MSG::ERR_NOT_ENOUGHT_ROOM_OTHER);
				pDestHuman->GetPlayer()->SendPacket(&MsgOther);
				
				pHuman->m_ExchangBox.CleanUp();
				pDestHuman->m_ExchangBox.CleanUp();
				g_pLog->FastSaveLog( LOG_FILE_1, "		<交易> [%s] 空间不够", pHuman->GetName()) ;
				return PACKET_EXE_CONTINUE;
			}

			if( FALSE == HumanItemLogic::CanReceiveExchangeItemList( pDestHuman, ItemListMeToOt) )
			{//空间不够
				GCExchangeError MsgSelf;
				MsgSelf.SetID(EXCHANGE_MSG::ERR_NOT_ENOUGHT_ROOM_SELF);
				pDestHuman->GetPlayer()->SendPacket(&MsgSelf);

				GCExchangeError MsgOther;
				MsgOther.SetID(EXCHANGE_MSG::ERR_NOT_ENOUGHT_ROOM_OTHER);
				pHuman->GetPlayer()->SendPacket(&MsgOther);

				pHuman->m_ExchangBox.CleanUp();
				pDestHuman->m_ExchangBox.CleanUp();
				g_pLog->FastSaveLog( LOG_FILE_1, "		<交易> [%s] 空间不够", pDestHuman->GetName()) ;
				return PACKET_EXE_CONTINUE;
			}

			if( pHuman->m_ExchangBox.m_Money > pHuman->GetMoney() )
			{//金钱非法
				GCExchangeError MsgSelf;
				MsgSelf.SetID(EXCHANGE_MSG::ERR_NOT_ENOUGHT_MONEY_SELF);
				pHuman->GetPlayer()->SendPacket(&MsgSelf);

				GCExchangeError MsgOther;
				MsgOther.SetID(EXCHANGE_MSG::ERR_NOT_ENOUGHT_MONEY_OTHER);
				pDestHuman->GetPlayer()->SendPacket(&MsgOther);

				pHuman->m_ExchangBox.CleanUp();
				pDestHuman->m_ExchangBox.CleanUp();
				g_pLog->FastSaveLog( LOG_FILE_1, "		<交易> [%s] 金钱不足", pHuman->GetName()) ;
				return PACKET_EXE_CONTINUE;
			}

			if( pDestHuman->m_ExchangBox.m_Money > pDestHuman->GetMoney() )
			{//金钱非法
				GCExchangeError MsgSelf;
				MsgSelf.SetID(EXCHANGE_MSG::ERR_NOT_ENOUGHT_MONEY_SELF);
				pDestHuman->GetPlayer()->SendPacket(&MsgSelf);

				GCExchangeError MsgOther;
				MsgOther.SetID(EXCHANGE_MSG::ERR_NOT_ENOUGHT_MONEY_OTHER);
				pHuman->GetPlayer()->SendPacket(&MsgOther);

				pHuman->m_ExchangBox.CleanUp();
				pDestHuman->m_ExchangBox.CleanUp();
				g_pLog->FastSaveLog( LOG_FILE_1, "		<交易> [%s] 金钱不足", pDestHuman->GetName()) ;
				return PACKET_EXE_CONTINUE;
			}

			//2.1宠物验证
			Obj_Pet* pDestPet	= pDestHuman->GetPet();
			Obj_Pet* pMyPet		= pHuman->GetPet();

			PET_GUID_t	DestPetGuid;
			PET_GUID_t	MyPetGuid;
			
			if(pDestPet)
			{
				DestPetGuid = pDestPet->GetPetGUID();
			}
			if(pMyPet)
			{
				MyPetGuid = pMyPet->GetPetGUID();
			}

			//如果是当前召唤出来的宠物,先收回来再换
			for(INT i = 0; i<EXCHANGE_PET_BOX_SIZE; i++)
			{
				if(pMyExchangePetContainer->GetItem(i)->IsEmpty() == FALSE)
				{
					INT nPetLevel			= pMyExchangePetContainer->GetItem(i)->GetLevel();
					INT nHumanLevel			= pDestHuman->GetLevel();
					PET_GUID_t	ExchPetGuid	= pMyExchangePetContainer->GetItem(i)->GetPetGUID();
					if(nPetLevel>nHumanLevel)
					{
						GCExchangeError Msg;
						Msg.SetID(EXCHANGE_MSG::ERR_PET_LEVEL_TOO_HIGH);
						pHuman->GetPlayer()->SendPacket(&Msg);
						pDestHuman->GetPlayer()->SendPacket(&Msg);
						pHuman->m_ExchangBox.CleanUp();
						pDestHuman->m_ExchangBox.CleanUp();
						g_pLog->FastSaveLog( LOG_FILE_1, "		<交易> 宠物级别过高 petlevel = %d, humanlevel = %d", nPetLevel, nHumanLevel) ;
						return PACKET_EXE_CONTINUE;
					}
					if(ExchPetGuid == MyPetGuid)
					{
						pHuman->ReCallPet();
					}
				}

				if(pOtExchangePetContainer->GetItem(i)->IsEmpty() == FALSE)
				{
					INT nPetLevel = pOtExchangePetContainer->GetItem(i)->GetLevel();
					INT nHumanLevel = pHuman->GetLevel();
					PET_GUID_t	ExchPetGuid	= pOtExchangePetContainer->GetItem(i)->GetPetGUID();
					if(nPetLevel>nHumanLevel)
					{
						GCExchangeError Msg;
						Msg.SetID(EXCHANGE_MSG::ERR_PET_LEVEL_TOO_HIGH);
						pHuman->GetPlayer()->SendPacket(&Msg);
						pDestHuman->GetPlayer()->SendPacket(&Msg);
						pHuman->m_ExchangBox.CleanUp();
						pDestHuman->m_ExchangBox.CleanUp();
						g_pLog->FastSaveLog( LOG_FILE_1, "		<交易> 宠物级别过高 petlevel = %d, humanlevel = %d", nPetLevel, nHumanLevel) ;
						return PACKET_EXE_CONTINUE;
					}
					if(ExchPetGuid == DestPetGuid)
					{
						pDestHuman->ReCallPet();
					}
				}
			}

			//3.移动物品循环
			g_pLog->FastSaveLog( LOG_FILE_1, "<交易> [%s] 与 [%s] 开始移动循环",	pHuman->GetName(), pDestHuman->GetName()) ;
			GCExchangeSuccessIII MsgSuccessToMe, MsgSuccessToOt;
			GCExchangeSuccessIII::_SUCC_ITEM ItemListToMe[EXCHANGE_BOX_SIZE+EXCHANGE_PET_BOX_SIZE];
			INT	MyItemNum = 0;
			GCExchangeSuccessIII::_SUCC_ITEM ItemListToOt[EXCHANGE_BOX_SIZE+EXCHANGE_PET_BOX_SIZE];
			INT	OtItemNum = 0;
			BOOL	bFlag = FALSE;

			//用来记录可以交换的物品
			struct ExItem_t
			{
				UCHAR	uItemType;
				INT		nIndex;
				INT		nIndexInEx;
				ExItem_t()
				{
					uItemType	= 0;
					nIndex		= -1;
					nIndexInEx	= -1;
				}
			};

			INT				ExItemNumOt = 0;
			ExItem_t		ExItemIndexInOt[EXCHANGE_BOX_SIZE];
			INT				ExItemNumMy = 0;
			ExItem_t		ExItemIndexInMy[EXCHANGE_BOX_SIZE];
			INT				ExPetItemNumOt = 0;
			ExItem_t		ExPetItemIndexInOt[EXCHANGE_PET_BOX_SIZE];
			INT				ExPetItemNumMy = 0;
			ExItem_t		ExPetItemIndexInMy[EXCHANGE_PET_BOX_SIZE];

			for(INT i = 0; i<EXCHANGE_BOX_SIZE; i++)
			{
				if(pMyExchangeContainer->GetItem(i)->IsEmpty() == FALSE)
				{
					Item* pItemRef = pMyExchangeContainer->GetItem(i);
					ItemContainer*	pMyBagContainer = HumanItemLogic::GetItemContain(pHuman, pItemRef->GetItemTableIndex());
					ExItemIndexInMy[ExItemNumMy].uItemType	= pItemRef->GetItemClass();
					ExItemIndexInMy[ExItemNumMy].nIndex		= pMyBagContainer->GetIndexByGUID(&pItemRef->GetGUID());
					ExItemIndexInMy[ExItemNumMy].nIndexInEx	= i;
					ExItemNumMy++;
				}

				if(pOtExchangeContainer->GetItem(i)->IsEmpty() == FALSE)
				{
					Item* pItemRef = pOtExchangeContainer->GetItem(i);
					ItemContainer*	pOtBagContainer = HumanItemLogic::GetItemContain(pDestHuman, pItemRef->GetItemTableIndex());
					ExItemIndexInOt[ExItemNumOt].uItemType	= pItemRef->GetItemClass();
					ExItemIndexInOt[ExItemNumOt].nIndex		= pOtBagContainer->GetIndexByGUID(&pItemRef->GetGUID());
					ExItemIndexInOt[ExItemNumOt].nIndexInEx	= i;
					ExItemNumOt++;
				}
			}

			for(INT i = 0; i<EXCHANGE_PET_BOX_SIZE; i++)
			{
				if(pMyExchangePetContainer->GetItem(i)->IsEmpty() == FALSE)
				{
					Item* pItemRef = pMyExchangePetContainer->GetItem(i);
					ItemContainer*	pMyPetContainer = pHuman->GetPetContain();
					ExPetItemIndexInMy[ExPetItemNumMy].nIndex = pMyPetContainer->GetIndexByGUID(&pItemRef->GetPetGUID());
					ExPetItemIndexInMy[ExPetItemNumMy].nIndexInEx = i;
					ExPetItemNumMy++;
				}

				if(pOtExchangePetContainer->GetItem(i)->IsEmpty() == FALSE)
				{
					Item* pItemRef = pOtExchangePetContainer->GetItem(i);
					ItemContainer*	pOtPetContainer = pDestHuman->GetPetContain();
					ExPetItemIndexInOt[ExPetItemNumOt].nIndex		= pOtPetContainer->GetIndexByGUID(&pItemRef->GetPetGUID());
					ExPetItemIndexInOt[ExPetItemNumOt].nIndexInEx = i;
					ExPetItemNumOt++;
				}
			}

			//3.1先把能交换的交换掉，如果在对方的包中找到了正在跟自己交易的物品，直接交换这两个物品的位置
			for(INT i = 0; i<EXCHANGE_BOX_SIZE; i++)
			{
				if(pMyExchangeContainer->GetItem(i)->IsEmpty() == FALSE)
				{
					INT result = 0;
					Item* pItemRef = pMyExchangeContainer->GetItem(i);

					//自己的容器
					ItemContainer*	pMyBagContainer = HumanItemLogic::GetItemContain(pHuman, pItemRef->GetItemTableIndex());
					INT	BagIndex = pMyBagContainer->GetIndexByGUID(&pItemRef->GetGUID());

					//对方的容器
					ItemContainer*	pOtBagContainer = HumanItemLogic::GetItemContain(pDestHuman, pItemRef->GetItemTableIndex());

					//去换能换得
					for(INT j = 0; j<ExItemNumOt; j++)
					{

						//此位置已经无效
						if(ExItemIndexInOt[j].nIndex == -1)
							continue;

						//只有同一类型的物品才能够交换
						if(pItemRef->GetItemClass()!=ExItemIndexInOt[j].uItemType)
							continue;

						//解锁自己
						g_ItemOperator.UnlockItem( pMyBagContainer, BagIndex );

						//先解锁对方
						g_ItemOperator.UnlockItem( pOtBagContainer, ExItemIndexInOt[j].nIndex );

						//设置自己物品已经不是交易物品了
						pMyBagContainer->GetItem(BagIndex)->SetInExchange(FALSE);

						//设置对方物品已经不是交易物品了
						pOtBagContainer->GetItem(ExItemIndexInOt[j].nIndex)->SetInExchange(FALSE);

						//与一个对方欲交易的物品交换
						result = 
							g_ItemOperator.ExchangeItem
							(
							pMyBagContainer,
							BagIndex,
							pOtBagContainer,
							ExItemIndexInOt[j].nIndex
							);
						if(result<0)
						{//拷贝失败
							GCExchangeError Msg;
							Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
							pHuman->GetPlayer()->SendPacket(&Msg);
							pDestHuman->GetPlayer()->SendPacket(&Msg);
							pHuman->m_ExchangBox.CleanUp();
							pDestHuman->m_ExchangBox.CleanUp();
							g_pLog->FastSaveLog( LOG_FILE_1, "		<交易> [%s] <-> [%s] 物品 [%d]失败 result = %d", pHuman->GetName(), pDestHuman->GetName(), pItemRef->GetItemTableIndex(), result );
							return PACKET_EXE_CONTINUE;
						}
						else
						{
							ITEM_LOG_PARAM	ItemLogParam;
							ItemLogParam.OpType		=	ITEM_EXCHANGE_TOOTHER;
							ItemLogParam.CharGUID	=	pHuman->GetGUID();
							ItemLogParam.TargetGUID	=	pDestHuman->GetGUID();
							ItemLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
							ItemLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
							ItemLogParam.SceneID	=	pHuman->getScene()->SceneID();
							ItemLogParam.ItemGuid	=	pItemRef->GetGUID();
							ItemLogParam.ItemType	=	pItemRef->GetItemTableIndex();

							ItemListToOt[OtItemNum].m_FromType	= 0;
							ItemListToOt[OtItemNum].m_FromIndex = i;
							ItemListToOt[OtItemNum].m_ToType	= EXCHANGE_MSG::POS_BAG;
							ItemListToOt[OtItemNum++].m_ToIndex	= pOtBagContainer->ConIndex2BagIndex(ExItemIndexInOt[j].nIndex);

							ItemListToMe[MyItemNum].m_FromType	= 0;
							ItemListToMe[MyItemNum].m_FromIndex = ExItemIndexInOt[j].nIndexInEx;
							ItemListToMe[MyItemNum].m_ToType	= EXCHANGE_MSG::POS_BAG;
							ItemListToMe[MyItemNum++].m_ToIndex	= pMyBagContainer->ConIndex2BagIndex(BagIndex);

							//从交易盒中删掉此物品,在后面的移动中就不用再移动它了
							g_ItemOperator.EraseItem(pMyExchangeContainer, i);
							g_ItemOperator.EraseItem(pOtExchangeContainer, ExItemIndexInOt[j].nIndexInEx);
							ExItemIndexInOt[j].nIndex = -1;
						
							g_pLog->FastSaveLog( LOG_FILE_1, "		<交易> [%s] <-> [%s] 物品 [%d]成功", pHuman->GetName(), pDestHuman->GetName(), pItemRef->GetItemTableIndex() );

							break;
						}
					}
				}
			}
	
			//3.2不能交换的一个一个放进去，剩下的直接由系统找格放入
			for(INT i = 0; i<EXCHANGE_BOX_SIZE; i++)
			{
				if(pMyExchangeContainer->GetItem(i)->IsEmpty() == FALSE)
				{
					INT result = 0;
					Item* pItemRef = pMyExchangeContainer->GetItem(i);

					//自己的容器
					ItemContainer*	pMyBagContainer = HumanItemLogic::GetItemContain(pHuman, pItemRef->GetItemTableIndex());
					INT	BagIndex = pMyBagContainer->GetIndexByGUID(&pItemRef->GetGUID());

					//对方的容器
					ItemContainer*	pOtBagContainer = HumanItemLogic::GetItemContain(pDestHuman, pItemRef->GetItemTableIndex());

					//先解锁
					g_ItemOperator.UnlockItem( pMyBagContainer, BagIndex );
					pMyBagContainer->GetItem(BagIndex)->SetInExchange(FALSE);


					//自动找格，支持自动叠加
					//这里的自动找格改成可以自动叠加，这里会有一个问题，上面收取检测时是不考虑叠加状态的，所以，如果非叠加状态下的
					//可以收取一系列物品，可叠加的一定可以接受。检测的范围会大一点。
					result = 
						g_ItemOperator.MoveItem
						(
						pMyBagContainer,
						BagIndex,
						bFlag,
						pOtBagContainer
						);
					if(result<0)
					{//拷贝失败
						GCExchangeError Msg;
						Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
						pHuman->GetPlayer()->SendPacket(&Msg);
						pDestHuman->GetPlayer()->SendPacket(&Msg);
						pHuman->m_ExchangBox.CleanUp();
						pDestHuman->m_ExchangBox.CleanUp();
						g_pLog->FastSaveLog( LOG_FILE_1, "		<交易> [%s] -> [%s] 物品 [%d]失败 result = %d", pHuman->GetName(), pDestHuman->GetName(), pItemRef->GetItemTableIndex(), result );
						return PACKET_EXE_CONTINUE;
					}
					else
					{
						ITEM_LOG_PARAM	ItemLogParam;
						ItemLogParam.OpType		=	ITEM_EXCHANGE_TOOTHER;
						ItemLogParam.CharGUID	=	pHuman->GetGUID();
						ItemLogParam.TargetGUID	=	pDestHuman->GetGUID();
						ItemLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
						ItemLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
						ItemLogParam.SceneID	=	pHuman->getScene()->SceneID();
						ItemLogParam.ItemGuid	=	pItemRef->GetGUID();
						ItemLogParam.ItemType	=	pItemRef->GetItemTableIndex();

						ItemListToOt[OtItemNum].m_FromType	= 0;
						ItemListToOt[OtItemNum].m_FromIndex = i;
						ItemListToOt[OtItemNum].m_ToType	= EXCHANGE_MSG::POS_BAG;
						ItemListToOt[OtItemNum++].m_ToIndex	= pOtBagContainer->ConIndex2BagIndex(result);
						g_pLog->FastSaveLog( LOG_FILE_1, "		<交易> [%s] -> [%s] 物品 [%d]成功", pHuman->GetName(), pDestHuman->GetName(), pItemRef->GetItemTableIndex() );
					}
				}
				if(pOtExchangeContainer->GetItem(i)->IsEmpty() == FALSE)
				{
					INT result = 0;
					Item* pItemRef = pOtExchangeContainer->GetItem(i);

					//对方的容器
					ItemContainer*	pOtBagContainer = HumanItemLogic::GetItemContain(pDestHuman, pItemRef->GetItemTableIndex());
					INT	BagIndex = pOtBagContainer->GetIndexByGUID(&pItemRef->GetGUID());

					//自己的容器
					ItemContainer*	pMyBagContainer = HumanItemLogic::GetItemContain(pHuman, pItemRef->GetItemTableIndex());

					//先解锁
					g_ItemOperator.UnlockItem( pOtBagContainer, BagIndex );

					pOtBagContainer->GetItem(BagIndex)->SetInExchange(FALSE);

					
					//自动找格，支持自动叠加
					result = 
						g_ItemOperator.MoveItem
						(
						pOtBagContainer,
						BagIndex,
						bFlag,
						pMyBagContainer
						);
					if(result<0)
					{//拷贝失败
						GCExchangeError Msg;
						Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
						pHuman->GetPlayer()->SendPacket(&Msg);
						pDestHuman->GetPlayer()->SendPacket(&Msg);
						pHuman->m_ExchangBox.CleanUp();
						pDestHuman->m_ExchangBox.CleanUp();
						g_pLog->FastSaveLog( LOG_FILE_1, "		<交易> [%s] -> [%s] 物品 [%d]失败 result = %d", pDestHuman->GetName(), pHuman->GetName(), pItemRef->GetItemTableIndex(), result );
						return PACKET_EXE_CONTINUE;
					}
					else
					{
						ITEM_LOG_PARAM	ItemLogParam;
						ItemLogParam.OpType		=	ITEM_EXCHANGE_TOOTHER;
						ItemLogParam.CharGUID	=	pDestHuman->GetGUID();
						ItemLogParam.TargetGUID	=	pHuman->GetGUID();
						ItemLogParam.XPos		=	pDestHuman->getWorldPos()->m_fX;
						ItemLogParam.ZPos		=	pDestHuman->getWorldPos()->m_fZ;
						ItemLogParam.SceneID	=	pDestHuman->getScene()->SceneID();
						ItemLogParam.ItemGuid	=	pItemRef->GetGUID();
						ItemLogParam.ItemType	=	pItemRef->GetItemTableIndex();

						SaveItemLog(&ItemLogParam);
						ItemListToMe[MyItemNum].m_FromType	= 0;
						ItemListToMe[MyItemNum].m_FromIndex = i;
						ItemListToMe[MyItemNum].m_ToType	= EXCHANGE_MSG::POS_BAG;
						ItemListToMe[MyItemNum++].m_ToIndex	= pMyBagContainer->ConIndex2BagIndex(result);
						g_pLog->FastSaveLog( LOG_FILE_1, "		<交易> [%s] -> [%s] 物品 [%d]成功", pDestHuman->GetName(), pHuman->GetName(), pItemRef->GetItemTableIndex() );
					}
				}
			}

            //3.3换宠物
			g_pLog->FastSaveLog( LOG_FILE_1, "<交易> [%s] 与 [%s] 开始交换宠物",	pHuman->GetName(), pDestHuman->GetName()) ;

			for(INT i = 0; i<EXCHANGE_PET_BOX_SIZE; i++)
			{
				INT result = 0;
				Item* pItemRef = pMyExchangePetContainer->GetItem(i);

				if(pItemRef->IsEmpty())
					continue;

				PET_LOG_PARAM	PetLogParam;
				PetLogParam.PetGUID		=	pItemRef->GetPetGUID();
				PetLogParam.DataID		=	pItemRef->GetDataID();
				//自己的容器
				ItemContainer*	pMyPetContainer = pHuman->GetPetContain();
				INT	PetIndexInBag = pMyPetContainer->GetIndexByGUID(&pItemRef->GetPetGUID());

				//对方的容器
				ItemContainer*	pOtPetContainer = pDestHuman->GetPetContain();

				//去换能换得
				for(INT j = 0; j<ExPetItemNumOt; j++)
				{
					//如果无效
					if(ExPetItemIndexInOt[j].nIndex == -1)
						continue;

					//解锁自己
					g_ItemOperator.UnlockItem( pMyPetContainer, PetIndexInBag );

					//先解锁对方
					g_ItemOperator.UnlockItem( pOtPetContainer, ExPetItemIndexInOt[j].nIndex );

					//设置自己物品已经不是交易物品了
					pMyPetContainer->GetItem(PetIndexInBag)->SetInExchange(FALSE);

					//设置对方物品已经不是交易物品了
					pOtPetContainer->GetItem(ExPetItemIndexInOt[j].nIndex )->SetInExchange(FALSE);

					//与一个对方欲交易的物品交换
					result = 
						g_ItemOperator.ExchangeItem
						(
						pMyPetContainer,
						PetIndexInBag,
						pOtPetContainer,
						ExPetItemIndexInOt[j].nIndex
						);
					if(result<0)
					{//拷贝失败
						GCExchangeError Msg;
						Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
						pHuman->GetPlayer()->SendPacket(&Msg);
						pDestHuman->GetPlayer()->SendPacket(&Msg);
						pHuman->m_ExchangBox.CleanUp();
						pDestHuman->m_ExchangBox.CleanUp();
						g_pLog->FastSaveLog( LOG_FILE_1, "		<交易> [%s] <-> [%s] 物品 [%d]失败 result = %d", pHuman->GetName(), pDestHuman->GetName(), pItemRef->GetItemTableIndex(), result );
						return PACKET_EXE_CONTINUE;
					}
					else
					{
						PetLogParam.CharGUID	=	pDestHuman->GetGUID();
						PetLogParam.OPType		=	PET_OP_EXCHANGE_OTHER;
						PetLogParam.SceneID		=	pDestHuman->getScene()->SceneID();
						PetLogParam.TargetGUID	=	pHuman->GetGUID();
						PetLogParam.XPos		=	pDestHuman->getWorldPos()->m_fX;
						PetLogParam.ZPos		=	pDestHuman->getWorldPos()->m_fZ;
						SavePetLog(&PetLogParam);

						ItemListToMe[MyItemNum].m_FromType	= EXCHANGE_MSG::POS_PET;
						ItemListToMe[MyItemNum].m_FromIndex = ExPetItemIndexInOt[j].nIndexInEx;
						ItemListToMe[MyItemNum].m_ToType	= EXCHANGE_MSG::POS_PET;
						ItemListToMe[MyItemNum++].m_ToIndex	= PetIndexInBag;

						ItemListToOt[OtItemNum].m_FromType	= EXCHANGE_MSG::POS_PET;
						ItemListToOt[OtItemNum].m_FromIndex = i;
						ItemListToOt[OtItemNum].m_ToType	= EXCHANGE_MSG::POS_PET;
						ItemListToOt[OtItemNum++].m_ToIndex	= ExPetItemIndexInOt[j].nIndex;
					
						//从交易盒中删掉此物品,在后面的移动中就不用再移动它了
						g_ItemOperator.EraseItem(pMyExchangePetContainer, i);
						g_ItemOperator.EraseItem(pOtExchangePetContainer, ExPetItemIndexInOt[j].nIndexInEx);

						ExPetItemIndexInOt[j].nIndex = -1;

						g_pLog->FastSaveLog( LOG_FILE_1, "		<交易> [%s] <-> [%s] 物品 [%s]成功", pHuman->GetName(), pDestHuman->GetName(), pItemRef->GetName() );

						break;
					}
				}
			}

			//3.4移宠物
			for(INT i = 0; i<EXCHANGE_PET_BOX_SIZE; i++)
			{
				if(pMyExchangePetContainer->GetItem(i)->IsEmpty() == FALSE)
				{
					INT result = 0;
					Item* pItemRef = pMyExchangePetContainer->GetItem(i);
					
					PET_LOG_PARAM	PetLogParam;
					PetLogParam.PetGUID		=	pItemRef->GetPetGUID();
					PetLogParam.DataID		=	pItemRef->GetDataID();
					//自己的容器
					ItemContainer*	pMyPetContainer = pHuman->GetPetContain();
					INT	PetIndexInBag = pMyPetContainer->GetIndexByGUID(&pItemRef->GetPetGUID());

					//对方的容器
					ItemContainer*	pOtPetContainer = pDestHuman->GetPetContain();

					//先解锁
					g_ItemOperator.UnlockItem( pMyPetContainer, PetIndexInBag );

					pMyPetContainer->GetItem(PetIndexInBag)->SetInExchange(FALSE);

					//自动找格，支持自动叠加
					//这里的自动找格改成可以自动叠加，这里会有一个问题，上面收取检测时是不考虑叠加状态的，所以，如果非叠加状态下的
					//可以收取一系列物品，可叠加的一定可以接受。检测的范围会大一点。
					result = 
						g_ItemOperator.MoveItem
						(
						pMyPetContainer,
						PetIndexInBag,
						pOtPetContainer
						);
					if(result<0)
					{//拷贝失败
						GCExchangeError Msg;
						Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
						pHuman->GetPlayer()->SendPacket(&Msg);
						pDestHuman->GetPlayer()->SendPacket(&Msg);
						pHuman->m_ExchangBox.CleanUp();
						pDestHuman->m_ExchangBox.CleanUp();
						g_pLog->FastSaveLog( LOG_FILE_1, "		<交易> [%s] -> [%s] 宠物 [%s]失败 result = %d", pHuman->GetName(), pDestHuman->GetName(), pItemRef->GetName(), result );
						return PACKET_EXE_CONTINUE;
					}
					else
					{
						PetLogParam.CharGUID	=	pDestHuman->GetGUID();
						PetLogParam.OPType		=	PET_OP_EXCHANGE_OTHER;
						PetLogParam.SceneID		=	pDestHuman->getScene()->SceneID();
						PetLogParam.TargetGUID	=	pHuman->GetGUID();
						PetLogParam.XPos		=	pDestHuman->getWorldPos()->m_fX;
						PetLogParam.ZPos		=	pDestHuman->getWorldPos()->m_fZ;
						SavePetLog(&PetLogParam);

						ItemListToOt[OtItemNum].m_FromType	= EXCHANGE_MSG::POS_PET;
						ItemListToOt[OtItemNum].m_FromIndex = i;
						ItemListToOt[OtItemNum].m_ToType	= EXCHANGE_MSG::POS_PET;
						ItemListToOt[OtItemNum++].m_ToIndex	= result;
						g_pLog->FastSaveLog( LOG_FILE_1, "		<交易> [%s] -> [%s] 宠物 [%s]成功", pHuman->GetName(), pDestHuman->GetName(), pItemRef->GetName() );
					}
				}

				if(pOtExchangePetContainer->GetItem(i)->IsEmpty() == FALSE)
				{
					INT result = 0;
					Item* pItemRef = pOtExchangePetContainer->GetItem(i);
					PET_LOG_PARAM	PetLogParam;
					PetLogParam.PetGUID		=	pItemRef->GetPetGUID();
					PetLogParam.DataID		=	pItemRef->GetDataID();

					//对方的容器
					ItemContainer*	pOtPetContainer = pDestHuman->GetPetContain();
					INT	PetIndexInBag = pOtPetContainer->GetIndexByGUID(&pItemRef->GetPetGUID());

					//自己的容器
					ItemContainer*	pMyPetContainer = pHuman->GetPetContain();

					//先解锁
					g_ItemOperator.UnlockItem( pOtPetContainer, PetIndexInBag );
					pOtPetContainer->GetItem(PetIndexInBag)->SetInExchange(FALSE);

					//自动找格，支持自动叠加
					result = 
						g_ItemOperator.MoveItem
						(
						pOtPetContainer,
						PetIndexInBag,
						pMyPetContainer
						);
					if(result<0)
					{//拷贝失败
						GCExchangeError Msg;
						Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
						pHuman->GetPlayer()->SendPacket(&Msg);
						pDestHuman->GetPlayer()->SendPacket(&Msg);
						pHuman->m_ExchangBox.CleanUp();
						pDestHuman->m_ExchangBox.CleanUp();
						g_pLog->FastSaveLog( LOG_FILE_1, "		<交易> [%s] -> [%s] 宠物 [%s]失败 result = %d", pDestHuman->GetName(), pHuman->GetName(), pItemRef->GetName(), result );
						return PACKET_EXE_CONTINUE;
					}
					else
					{
						PetLogParam.CharGUID	=	pHuman->GetGUID();
						PetLogParam.OPType		=	PET_OP_EXCHANGE_OTHER;
						PetLogParam.SceneID		=	pHuman->getScene()->SceneID();
						PetLogParam.TargetGUID	=	pDestHuman->GetGUID();
						PetLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
						PetLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
						SavePetLog(&PetLogParam);
						ItemListToMe[MyItemNum].m_FromType	= EXCHANGE_MSG::POS_PET;
						ItemListToMe[MyItemNum].m_FromIndex = i;
						ItemListToMe[MyItemNum].m_ToType	= EXCHANGE_MSG::POS_PET;
						ItemListToMe[MyItemNum++].m_ToIndex	= result;
						g_pLog->FastSaveLog( LOG_FILE_1, "		<交易> [%s] -> [%s] 宠物 [%s]成功", pDestHuman->GetName(), pHuman->GetName(), pItemRef->GetName() );
					}
				}
			}
			

			//2.换钱
			g_pLog->FastSaveLog( LOG_FILE_1, "<交易> [%s] 与 [%s] 开始交换金钱",	pHuman->GetName(), pDestHuman->GetName()) ;
			if( pHuman->m_ExchangBox.m_Money <= pHuman->GetMoney() && pHuman->m_ExchangBox.m_Money>0 )
			{
				pDestHuman->SetMoney(pDestHuman->GetMoney()+pHuman->m_ExchangBox.m_Money);
				pHuman->SetMoney(pHuman->GetMoney()-pHuman->m_ExchangBox.m_Money);

				MONEY_LOG_PARAM	MoneyLogParam;
				MoneyLogParam.CharGUID	=	pHuman->GetGUID();
				MoneyLogParam.TargetGUID	=	pDestHuman->GetGUID();
				MoneyLogParam.OPType	=	MONEY_EXCHANGE_OUTCOME;	
				MoneyLogParam.Count		=	pHuman->m_ExchangBox.m_Money;
				MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
				MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				SaveMoneyLog(&MoneyLogParam);

				MoneyLogParam.CharGUID	=	pDestHuman->GetGUID();
				MoneyLogParam.TargetGUID	=	pHuman->GetGUID();
				MoneyLogParam.OPType	=	MONEY_EXCHANGE_INCOME;	
				MoneyLogParam.Count		=	pHuman->m_ExchangBox.m_Money;
				MoneyLogParam.SceneID	=	pDestHuman->getScene()->SceneID();
				MoneyLogParam.XPos		=	pDestHuman->getWorldPos()->m_fX;
				MoneyLogParam.ZPos		=	pDestHuman->getWorldPos()->m_fZ;
				SaveMoneyLog(&MoneyLogParam);


				g_pLog->FastSaveLog( LOG_FILE_1, "		<交易> [%s] -> [%s] 金钱 [%d]", pHuman->GetName(), pDestHuman->GetName(), pHuman->m_ExchangBox.m_Money );
			}

			if( pDestHuman->m_ExchangBox.m_Money <= pDestHuman->GetMoney() && pDestHuman->m_ExchangBox.m_Money>0 )
			{
				pHuman->SetMoney(pHuman->GetMoney()+pDestHuman->m_ExchangBox.m_Money);
				pDestHuman->SetMoney(pDestHuman->GetMoney() - pDestHuman->m_ExchangBox.m_Money);

				MONEY_LOG_PARAM	MoneyLogParam;
				MoneyLogParam.CharGUID	=	pHuman->GetGUID();
				MoneyLogParam.TargetGUID	=	pDestHuman->GetGUID();
				MoneyLogParam.OPType	=	MONEY_EXCHANGE_INCOME;	
				MoneyLogParam.Count		=	pDestHuman->m_ExchangBox.m_Money;
				MoneyLogParam.SceneID	=	pHuman->getScene()->SceneID();
				MoneyLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
				MoneyLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
				SaveMoneyLog(&MoneyLogParam);

				MoneyLogParam.CharGUID	=	pDestHuman->GetGUID();
				MoneyLogParam.TargetGUID	=	pHuman->GetGUID();
				MoneyLogParam.OPType	=	MONEY_EXCHANGE_OUTCOME;	
				MoneyLogParam.Count		=	pDestHuman->m_ExchangBox.m_Money;
				MoneyLogParam.SceneID	=	pDestHuman->getScene()->SceneID();
				MoneyLogParam.XPos		=	pDestHuman->getWorldPos()->m_fX;
				MoneyLogParam.ZPos		=	pDestHuman->getWorldPos()->m_fZ;
				SaveMoneyLog(&MoneyLogParam);


				g_pLog->FastSaveLog( LOG_FILE_1, "		<交易> [%s] -> [%s] 金钱 [%d]", pDestHuman->GetName(), pHuman->GetName(), pDestHuman->m_ExchangBox.m_Money );
			}


			//给双方发送成功消息
			MsgSuccessToMe.SetItemNum(MyItemNum);
			MsgSuccessToMe.SetItemList(ItemListToMe);
			pHuman->GetPlayer()->SendPacket(&MsgSuccessToMe);

			MsgSuccessToOt.SetItemNum(OtItemNum);
			MsgSuccessToOt.SetItemList(ItemListToOt);
			pDestHuman->GetPlayer()->SendPacket(&MsgSuccessToOt);

			g_pLog->FastSaveLog( LOG_FILE_1, "<交易> [%s] 与 [%s] 交易成功",	pHuman->GetName(), pDestHuman->GetName()) ;

			//能走到这儿证明所有操作都已完成清空各自的交易盒
			pHuman->m_ExchangBox.CleanUp();
			pDestHuman->m_ExchangBox.CleanUp();

		}
		else
		{//啥也不做,等待对方消息

		}
	}
	else
	{//一定出错了
		GCExchangeError Msg;
		Msg.SetID(EXCHANGE_MSG::ERR_ILLEGAL);
		pHuman->GetPlayer()->SendPacket(&Msg);
		pHuman->m_ExchangBox.CleanUp();
		pDestHuman->GetPlayer()->SendPacket(&Msg);
		pDestHuman->m_ExchangBox.CleanUp();
		return PACKET_EXE_CONTINUE;
	}

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
