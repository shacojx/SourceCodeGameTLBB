#include "stdafx.h"
/*
同步客户端物品变化
*/
#include "CGItemSynch.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"

#include "ItemOperator.h"
#include "HumanItemLogic.h"

#include "GCItemSynch.h"

#include "GCPlayerShopError.h"
#include "LogDefine.h"
#include "Obj_Pet.h"
#include "RecordOpt.h"

using namespace ITEM_SYNCH_STRUCT;

uint CGItemSynchHandler::Execute( CGItemSynch* pPacket, Player* pPlayer )
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

	//操作
	BYTE		Opt				=	pPacket->GetOpt();
	//来自哪个容器
	BYTE		FromType		=	pPacket->GetFromType();
	//放到哪个容器
	BYTE		ToType			=	pPacket->GetToType();
	//放到目的容器哪个位置
	BYTE		ToIndex			=	pPacket->GetToIndex();
	//被移动物体的guid
	_ITEM_GUID	ItemGuid		=	pPacket->GetItemGUID();
	//被移动宠物的guid
	PET_GUID_t	PetGuid			=	pPacket->GetPetGUID();
	//附加信息长度
	BYTE		ExtraInfoL		=	pPacket->GetExtraInfoLength();
	//附加信息内容
	BYTE*		pExtraInfo		=	pPacket->GetExtraInfoData();
	//最终序列号
	UINT		uFinalSerial	=	0;
	//最后的目的位置
	UINT		uFinalToIndex	=	0;
	//玩家商店错误信息
	GCPlayerShopError	MsgError;
	//发送消息给客户端
	GCItemSynch			MsgToClient;

	switch(Opt)
	{
		case CGItemSynch::OPT_MOVE_ITEM_AUTO:
			{//自动找格
				switch(FromType)
				{
				case CGItemSynch::POS_BAG:
					{
						switch(ToType)
						{
							case CGItemSynch::POS_PLAYERSHOP:
								{//背包->商店
									CGAutoMoveItemFromBagToPlayerShop_t ExtraData;
                                    Assert(ExtraInfoL == ExtraData.GetSize());
									memcpy(&ExtraData, pExtraInfo, ExtraInfoL);

									_PLAYERSHOP_GUID	nShopID		= ExtraData.m_ShopGuid;
									BYTE				nStallIndex	= ExtraData.m_nStallIndex;
									BYTE				nShopSerial = ExtraData.m_nShopSerial;

									PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
									PlayerShop*		pPlayerShop		= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
									Assert(pPlayerShop);
									if(pPlayerShop->GetShopType() == PLAYER_SHOP::TYPE_PLAYER_SHOP_PET)
									{//商店类型错误
										MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL:BAG->SHOP"
											,pHuman->GetName()) ;
										return PACKET_EXE_CONTINUE ;
									}

									if(nShopSerial != pPlayerShop->GetSerial())
									{
										MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_SALE_OUT_SERIAL_IS_CHANGED);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "CGItemSynchHandler::Name=%s Serials had changed"
											,pHuman->GetName()) ;
										return PACKET_EXE_CONTINUE ;
									}

									//是不是自己的店
									BOOL bIsMine = (pHuman->GetGUID() == pPlayerShop->GetOwnerGuid())? TRUE:FALSE;

									//是不是自己可以管理的店
									BOOL bCanManager = pPlayerShop->IsPartner(pHuman->GetGUID());

									if(bIsMine == FALSE && bCanManager == FALSE)
									{
										//log...
										return PACKET_EXE_CONTINUE;
									}

									PlayerStallBox* pStallBox		= pPlayerShop->GetPlayerStallBoxByIndex(nStallIndex);
									Assert(pStallBox);
									ItemContainer*  pStallContainer = pStallBox->GetContainer();
									//ItemContainer* pMatContainer	= pHuman->GetMatContain();
									ItemContainer* pBaseContainer	= pHuman->GetBaseContain();
									//ItemContainer* pBagContainer	= NULL;

//									if(pMatContainer->GetIndexByGUID(&ItemGuid) >= 0)
//									{
//										pBagContainer = pMatContainer;
//									}
//									else if(pBaseContainer->GetIndexByGUID(&ItemGuid) >=0 )
//									{
//										pBagContainer = pBaseContainer;
//									}
//									else
//									{//guid非法
////										Assert(0);
//										MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
//										pGamePlayer->SendPacket(&MsgError);
//										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL:World = %d, Server = %d, Serial = %d "
//											,pHuman->GetName(), ItemGuid.m_World, ItemGuid.m_Server, ItemGuid.m_Serial) ;
//										return PACKET_EXE_CONTINUE ;
//									}

									INT	IndexInBag = pBaseContainer->GetIndexByGUID(&ItemGuid);

									//拷贝到摊位盒
									//自动找格
									INT result = 
										g_ItemOperator.MoveItem
										(
										pBaseContainer,
										IndexInBag,
										pStallContainer
										);

									if(result>=0)
									{//移动成功
										
										
										//发送消息给客户端
										Item* pIt = pStallContainer->GetItem(result);
										if(pIt->IsEmpty() == FALSE)
										{


											ITEM_LOG_PARAM	ItemLogParam;
											ItemLogParam.OpType			=	ITEM_PLAYERSHOP_UP;
											ItemLogParam.ItemGuid		=	ItemGuid;
											ItemLogParam.ItemType		=	pIt->GetItemTableIndex();
											ItemLogParam.BagPos			=	IndexInBag;
											ItemLogParam.CharGUID		=	pHuman->GetGUID();
											ItemLogParam.XPos			=	pHuman->getWorldPos()->m_fX;
											ItemLogParam.ZPos			=	pHuman->getWorldPos()->m_fZ;
											ItemLogParam.SceneID		=	pHuman->getScene()->SceneID();
											SaveItemLog(&ItemLogParam);
											

											//序列号递增
											uFinalSerial = pStallBox->IncSerialByIndex(result);
											
											//上货：AAA上货了BBBC件。（AAA为操作者名，BBB为商品名C为件数）
											RecordOpt::Excute(REC_ADDITEM, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (INT)pIt->GetItemTableIndex(), (INT)pIt->GetLayedNum());
											
											//发送消息给客户端
											MsgToClient.SetOpt(GCItemSynch::OPT_MOVE_ITEM);
											MsgToClient.SetFromType(GCItemSynch::POS_BAG);
											MsgToClient.SetToType(GCItemSynch::POS_PLAYERSHOP);
											MsgToClient.SetToIndex(result);
											MsgToClient.SetItemGuid(ItemGuid);

											GCMoveItemFromBagToPlayerShop_t ExtraInfoData;
											ExtraInfoData.m_ShopGuid		=	nShopID;
											ExtraInfoData.m_nStallIndex		=	nStallIndex;
											ExtraInfoData.m_uSerial			=	uFinalSerial;
											ExtraInfoData.m_nShopSerial		=	pPlayerShop->IncSerial();
											MsgToClient.SetExtraInfoLength(ExtraInfoData.GetSize());
											MsgToClient.SetExtraInfoData((BYTE*)&ExtraInfoData);
											pGamePlayer->SendPacket(&MsgToClient);
										}
									}
									else
									{//失败
										if(result == ITEMOE_DESTOPERATOR_FULL)
										{
											MsgError.SetID(PLAYERSHOP_MSG::ERR_NOT_ENOUGH_ROOM_IN_STALL);
											pGamePlayer->SendPacket(&MsgError);
											g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NOT_ENOUGH_ROOM_IN_STALL"
												,pHuman->GetName()) ;
											return PACKET_EXE_CONTINUE ;
										}
										else
										{
											Assert(0);
											MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
											pGamePlayer->SendPacket(&MsgError);
											g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL, POS_BAG->POS_PLAYERSHOP AUTO POS"
												,pHuman->GetName()) ;
											return PACKET_EXE_CONTINUE ;
										}
									}
								}
								break;
							default:
								break;
						}
					}
					break;
				case CGItemSynch::POS_PET:
					{
						switch(ToType)
						{
						case CGItemSynch::POS_PLAYERSHOPPET:
							{//宠物->商店

								CGAutoMovePetFromBagToPlayerShop_t ExtraData;
								Assert(ExtraInfoL == ExtraData.GetSize());
								memcpy(&ExtraData, pExtraInfo, ExtraInfoL);

								_PLAYERSHOP_GUID	nShopID		= ExtraData.m_ShopGuid;
								BYTE				nStallIndex	= ExtraData.m_nStallIndex;
								BYTE				nShopSerial = ExtraData.m_nShopSerial;

								PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
								PlayerShop*		pPlayerShop		= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
								Assert(pPlayerShop);
								if(pPlayerShop->GetShopType() != PLAYER_SHOP::TYPE_PLAYER_SHOP_PET)
								{//商店类型错误
									MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL:TYPE ERROR PET->SHOP"
										,pHuman->GetName()) ;
									return PACKET_EXE_CONTINUE ;
								}

								//是不是自己的店
								BOOL bIsMine = (pHuman->GetGUID() == pPlayerShop->GetOwnerGuid())? TRUE:FALSE;

								//是不是自己可以管理的店
								BOOL bCanManager = pPlayerShop->IsPartner(pHuman->GetGUID());

								if(bIsMine == FALSE && bCanManager == FALSE)
								{
									//log...
									return PACKET_EXE_CONTINUE;
								}

								if(nShopSerial != pPlayerShop->GetSerial())
								{
									MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_SALE_OUT_SERIAL_IS_CHANGED);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "CGItemSynchHandler::Name=%s Serials had changed"
										,pHuman->GetName()) ;
									return PACKET_EXE_CONTINUE ;
								}

								PlayerStallBox* pStallBox		= pPlayerShop->GetPlayerStallBoxByIndex(nStallIndex);
								Assert(pStallBox);
								ItemContainer*	pStallContainer = pStallBox->GetPetContainer();

								ItemContainer*	pPetContainer = pHuman->GetPetContain();
								INT	FromIndex = pPetContainer->GetIndexByGUID( &PetGuid );

								//召回宠物
								Item* pPetItem	= pPetContainer->GetItem(FromIndex);
								Obj_Pet* pMyPet	= pHuman->GetPet();
								PET_GUID_t	MyPetGuid;
								if(pMyPet)
								{
									MyPetGuid = pMyPet->GetPetGUID();
								}
								if(MyPetGuid == pPetItem->GetPetGUID())
									pHuman->ReCallPet();

								if(FromIndex<0)
								{
//									Assert(0);
									MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL:PetGuid "
										,pHuman->GetName()) ;
									return PACKET_EXE_CONTINUE ;
								}

								//拷贝到摊位盒
								//自动找格
								INT result = 
									g_ItemOperator.MoveItem
									(
									pPetContainer,
									FromIndex,
									pStallContainer
									);

								if(result>=0)
								{//移动成功
									//发送消息给客户端
									Item* pIt = pStallContainer->GetItem(result);
									if(pIt->IsEmpty() == FALSE)
									{
										PET_LOG_PARAM	PetLogParam;
										PetLogParam.CharGUID	=	pHuman->GetGUID();
										PetLogParam.OPType		=	PET_OP_PLAYERSHOP_UP;
										PetLogParam.PetGUID		=	pIt->GetPetGUID();
										PetLogParam.DataID		=	pIt->GetDataID();
										PetLogParam.SceneID		=	pHuman->getScene()->SceneID();
										PetLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
										PetLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
										SavePetLog(&PetLogParam);

										//序列号递增
										uFinalSerial = pStallBox->IncSerialByIndex(result);

										//上货：AAA上货了BBBC件。（AAA为操作者名，BBB为商品名C为件数）
										RecordOpt::Excute(REC_ADDPET, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (CHAR*)pIt->GetName(), (INT)1);

										//发送消息给客户端
										MsgToClient.SetOpt(GCItemSynch::OPT_MOVE_ITEM);
										MsgToClient.SetFromType(GCItemSynch::POS_PET);
										MsgToClient.SetToType(GCItemSynch::POS_PLAYERSHOPPET);
										MsgToClient.SetToIndex(result);
										MsgToClient.SetPetGUID(PetGuid);

										GCMovePetFromBagToPlayerShop_t ExtraInfoData;
										ExtraInfoData.m_ShopGuid		=	nShopID;
										ExtraInfoData.m_nStallIndex		=	nStallIndex;
										ExtraInfoData.m_uSerial			=	uFinalSerial;
										ExtraInfoData.m_nShopSerial		=	pPlayerShop->IncSerial();
										MsgToClient.SetExtraInfoLength(ExtraInfoData.GetSize());
										MsgToClient.SetExtraInfoData((BYTE*)&ExtraInfoData);
										pGamePlayer->SendPacket(&MsgToClient);
									}
								}
								else
								{//失败
									if(result == ITEMOE_DESTOPERATOR_FULL)
									{
										MsgError.SetID(PLAYERSHOP_MSG::ERR_NOT_ENOUGH_ROOM_IN_STALL);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NOT_ENOUGH_ROOM_IN_STALL"
											,pHuman->GetName()) ;
										return PACKET_EXE_CONTINUE ;
									}
									else
									{
										Assert(0);
										MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL, POS_BAG->POS_PLAYERSHOP AUTO POS"
											,pHuman->GetName()) ;
										return PACKET_EXE_CONTINUE ;
									}
								}
							}
							break;
						default:
							break;
						}
					}
					break;
				case CGItemSynch::POS_PLAYERSHOP:
					{
						switch(ToType)
						{
						case CGItemSynch::POS_BAG:
							{//商店->背包

								CGAutoMoveItemFromPlayerShopToBag_t ExtraData;
								Assert(ExtraInfoL == ExtraData.GetSize());
								memcpy(&ExtraData, pExtraInfo, ExtraInfoL);
								_PLAYERSHOP_GUID	nShopID			= ExtraData.m_ShopGuid;
								BYTE				nStallIndex		= ExtraData.m_nStallIndex;
								UINT				uSerial			= ExtraData.m_uSerial;
								UINT				nShopSerial		= ExtraData.m_nShopSerial;

								PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
								PlayerShop*		pPlayerShop		= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
								Assert(pPlayerShop);
								PlayerStallBox* pStallBox		= pPlayerShop->GetPlayerStallBoxByIndex(nStallIndex);
								Assert(pStallBox);
								ItemContainer*	pStallContainer = pStallBox->GetContainer();

								//是不是自己的店
								BOOL bIsMine = (pHuman->GetGUID() == pPlayerShop->GetOwnerGuid())? TRUE:FALSE;

								//是不是自己可以管理的店
								BOOL bCanManager = pPlayerShop->IsPartner(pHuman->GetGUID());

								if(bIsMine == FALSE && bCanManager == FALSE)
								{
									//log...
									return PACKET_EXE_CONTINUE;
								}

								if(nShopSerial != pPlayerShop->GetSerial())
								{
									MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_SALE_OUT_SERIAL_IS_CHANGED);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "CGItemSynchHandler::Name=%s Serials had changed"
										,pHuman->GetName()) ;
									return PACKET_EXE_CONTINUE ;
								}

								INT	IndexInStall = pStallContainer->GetIndexByGUID(&ItemGuid);
								if(IndexInStall<0)
								{
									MsgError.SetID(PLAYERSHOP_MSG::ERR_NEED_NEW_COPY);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NEED_NEW_COPY: IndexInStall = %d"
										,pHuman->GetName(), IndexInStall) ;
									return PACKET_EXE_CONTINUE;
								}

								if( pStallBox->GetSerialByIndex(IndexInStall) != uSerial)
								{
									MsgError.SetID(PLAYERSHOP_MSG::ERR_NEED_NEW_COPY);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NEED_NEW_COPY: Serial = %d, BoxSerial = %d"
										,pHuman->GetName(), uSerial, pStallBox->GetSerialByIndex(IndexInStall)) ;
									return PACKET_EXE_CONTINUE;
								}

								Item* pItem = pStallContainer->GetItem(IndexInStall);

								ItemContainer* pBagContainer = HumanItemLogic::GetItemContain(pHuman, pItem->GetItemTableIndex());
								
								ITEM_LOG_PARAM	ItemLogParam;
								ItemLogParam.OpType			=	ITEM_PLAYERSHOP_DOWN;
								ItemLogParam.ItemGuid		=	pItem->GetGUID();
								ItemLogParam.ItemType		=	pItem->GetItemTableIndex();
								

								//计算空间
								_EXCHANGE_ITEM_LIST			ItemListOtToMe;
								ItemListOtToMe.Init();
								ItemListOtToMe.AddItem(pItem);
								if( FALSE == HumanItemLogic::CanReceiveExchangeItemList( pHuman, ItemListOtToMe) )
								{//空间不够
									MsgError.SetID(PLAYERSHOP_MSG::ERR_NOT_ENOUGH_ROOM);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NOT_ENOUGH_ROOM"
										,pHuman->GetName()) ;
									return PACKET_EXE_CONTINUE;
								}

								//移动物品
								//先解锁
								//自动找格,支持自动叠加
								INT result = 
									g_ItemOperator.MoveItem
									(
									pStallContainer,
									IndexInStall,
									pBagContainer
									);

								if(result<0)
								{//失败
									if(result == ITEMOE_DESTOPERATOR_FULL)
									{
										MsgError.SetID(PLAYERSHOP_MSG::ERR_NOT_ENOUGH_ROOM);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NOT_ENOUGH_ROOM_IN_STALL"
											,pHuman->GetName()) ;
										return PACKET_EXE_CONTINUE ;
									}
									else
									{
										Assert(0);
										MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL, POS_PLAYERSHOP->POS_BAG AUTO POS"
											,pHuman->GetName()) ;
										return PACKET_EXE_CONTINUE ;
									}
								}
								pItem = pBagContainer->GetItem(result);
								UINT	ToBagIndex = pBagContainer->ConIndex2BagIndex(result);
								
								ItemLogParam.BagPos			=	ToBagIndex;
								ItemLogParam.CharGUID		=	pHuman->GetGUID();
								ItemLogParam.XPos			=	pHuman->getWorldPos()->m_fX;
								ItemLogParam.ZPos			=	pHuman->getWorldPos()->m_fZ;
								ItemLogParam.SceneID		=	pHuman->getScene()->SceneID();
								SaveItemLog(&ItemLogParam);
								
								//取回：AAA取回了BBBC件。（AAA为操作者名，BBB为商品名C为件数）
								RecordOpt::Excute(REC_DELITEM, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (INT)pItem->GetItemTableIndex(), (INT)pItem->GetLayedNum());

								//发消息通知客户端
								//序列号递增
								uFinalSerial = pStallBox->IncSerialByIndex(IndexInStall);
								MsgToClient.SetOpt(GCItemSynch::OPT_MOVE_ITEM);
								MsgToClient.SetFromType(GCItemSynch::POS_PLAYERSHOP);
								MsgToClient.SetToType(GCItemSynch::POS_BAG);
								MsgToClient.SetToIndex(ToBagIndex);
								MsgToClient.SetItemGuid(ItemGuid);

								GCMoveItemFromPlayerShopToBag_t ExtraInfoData;
								ExtraInfoData.m_ShopGuid		=	nShopID;
								ExtraInfoData.m_nStallIndex		=	nStallIndex;
								ExtraInfoData.m_Flag			=	1;
								ExtraInfoData.m_uSerial			=	uFinalSerial;
								ExtraInfoData.m_nShopSerial		=	pPlayerShop->IncSerial();
								MsgToClient.SetExtraInfoLength(ExtraInfoData.GetSize());
								MsgToClient.SetExtraInfoData((BYTE*)&ExtraInfoData);
								pGamePlayer->SendPacket(&MsgToClient);
							}
							break;
						default:
							break;
						}
					}
					break;
				case CGItemSynch::POS_PLAYERSHOPPET:
					{
						switch(ToType)
						{
							case CGItemSynch::POS_PET:
								{
									CGAutoMovePetFromPlayerShopToBag_t ExtraData;
									Assert(ExtraInfoL == ExtraData.GetSize());
									memcpy(&ExtraData, pExtraInfo, ExtraInfoL);
									_PLAYERSHOP_GUID	nShopID			= ExtraData.m_ShopGuid;
									BYTE				nStallIndex		= ExtraData.m_nStallIndex;
									UINT				uSerial			= ExtraData.m_uSerial;
									BYTE				nShopSerial		= ExtraData.m_nShopSerial;

									PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
									PlayerShop*		pPlayerShop		= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
									Assert(pPlayerShop);
									PlayerStallBox* pStallBox		= pPlayerShop->GetPlayerStallBoxByIndex(nStallIndex);
									Assert(pStallBox);
									ItemContainer*	pStallContainer = pStallBox->GetPetContainer();

									//是不是自己的店
									BOOL bIsMine = (pHuman->GetGUID() == pPlayerShop->GetOwnerGuid())? TRUE:FALSE;

									//是不是自己可以管理的店
									BOOL bCanManager = pPlayerShop->IsPartner(pHuman->GetGUID());

									if(bIsMine == FALSE && bCanManager == FALSE)
									{
										//log...
										return PACKET_EXE_CONTINUE;
									}

									if(nShopSerial != pPlayerShop->GetSerial())
									{
										MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_SALE_OUT_SERIAL_IS_CHANGED);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "CGItemSynchHandler::Name=%s Serials had changed"
											,pHuman->GetName()) ;
										return PACKET_EXE_CONTINUE ;
									}

									INT	IndexInStall = pStallContainer->GetIndexByGUID(&PetGuid);
									if(IndexInStall<0)
									{
										MsgError.SetID(PLAYERSHOP_MSG::ERR_NEED_NEW_COPY);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NEED_NEW_COPY: IndexInStall = %d"
											,pHuman->GetName(), IndexInStall) ;
										return PACKET_EXE_CONTINUE;
									}

									if( pStallBox->GetSerialByIndex(IndexInStall) != uSerial)
									{
										MsgError.SetID(PLAYERSHOP_MSG::ERR_NEED_NEW_COPY);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NEED_NEW_COPY: Serial = %d, BoxSerial = %d"
											,pHuman->GetName(), uSerial, pStallBox->GetSerialByIndex(IndexInStall)) ;
										return PACKET_EXE_CONTINUE;
									}

									ItemContainer*	pPetContainer = pHuman->GetPetContain();
									/*INT	IndexInBag = pPetContainer->GetIndexByGUID(&PetGuid);
									if(IndexInBag<0)
									{
										MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
										pHuman->m_StallBox.CleanUp();
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL: IndexInBag = %d"
											,pHuman->GetName(), IndexInBag) ;
										return PACKET_EXE_CONTINUE;
									}*/

									Item* pItem = pStallContainer->GetItem(IndexInStall);
									PET_LOG_PARAM	PetLogParam;
									PetLogParam.PetGUID		=	pItem->GetPetGUID();
									PetLogParam.DataID		=	pItem->GetDataID();

									//计算空间
									_EXCHANGE_ITEM_LIST			ItemListOtToMe;
									ItemListOtToMe.Init();
									ItemListOtToMe.AddItem(pItem);
									if( FALSE == HumanItemLogic::CanReceiveExchangeItemList( pHuman, ItemListOtToMe) )
									{//空间不够
										MsgError.SetID(PLAYERSHOP_MSG::ERR_NOT_ENOUGH_ROOM);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NOT_ENOUGH_ROOM"
											,pHuman->GetName()) ;
										return PACKET_EXE_CONTINUE;
									}

									//移动物品
									//先解锁
									//自动找格,支持自动叠加
									INT result = 
										g_ItemOperator.MoveItem
										(
										pStallContainer,
										IndexInStall,
										pPetContainer
										);

									if(result<0)
									{//失败
										if(result == ITEMOE_DESTOPERATOR_FULL)
										{
											MsgError.SetID(PLAYERSHOP_MSG::ERR_NOT_ENOUGH_ROOM);
											pGamePlayer->SendPacket(&MsgError);
											g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NOT_ENOUGH_ROOM_IN_STALL"
												,pHuman->GetName()) ;
											return PACKET_EXE_CONTINUE ;
										}
										else
										{
											Assert(0);
											MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
											pGamePlayer->SendPacket(&MsgError);
											g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL, POS_PLAYERSHOP->POS_BAG AUTO POS"
												,pHuman->GetName()) ;
											return PACKET_EXE_CONTINUE ;
										}
									}
									
									PetLogParam.CharGUID	=	pHuman->GetGUID();
									PetLogParam.OPType		=	PET_OP_PLAYERSHOP_DOWN;
								
									PetLogParam.SceneID		=	pHuman->getScene()->SceneID();
									PetLogParam.XPos		=	pHuman->getWorldPos()->m_fX;
									PetLogParam.ZPos		=	pHuman->getWorldPos()->m_fZ;
									SavePetLog(&PetLogParam);

									//发消息通知客户端
									//序列号递增
									uFinalSerial = pStallBox->IncSerialByIndex(IndexInStall);

									Item* pPetItem = pPetContainer->GetItem(result);

									//取回：AAA取回了BBBC件。（AAA为操作者名，BBB为商品名C为件数）
									RecordOpt::Excute(REC_DELPET, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (CHAR*)pPetItem->GetName(), (INT)1);


									MsgToClient.SetOpt(GCItemSynch::OPT_MOVE_ITEM);
									MsgToClient.SetFromType(GCItemSynch::POS_PLAYERSHOPPET);
									MsgToClient.SetToType(GCItemSynch::POS_PET);
									MsgToClient.SetToIndex(result);
									MsgToClient.SetPetGUID(PetGuid);

									GCMovePetFromPlayerShopToBag_t ExtraInfoData;
									ExtraInfoData.m_ShopGuid		=	nShopID;
									ExtraInfoData.m_nStallIndex		=	nStallIndex;
									ExtraInfoData.m_Flag			=	1;
									ExtraInfoData.m_uSerial			=	uFinalSerial;
									ExtraInfoData.m_nShopSerial		=	pPlayerShop->IncSerial();
									MsgToClient.SetExtraInfoLength(ExtraInfoData.GetSize());
									MsgToClient.SetExtraInfoData((BYTE*)&ExtraInfoData);
									pGamePlayer->SendPacket(&MsgToClient);

								}
								break;
							default:
								break;
						}
					}
					break;

				default:
					break;
				}
			}
			break;
		case CGItemSynch::OPT_MOVE_ITEM_MANU:
			{
				switch(FromType)
				{
				case CGItemSynch::POS_BAG:
					{
						switch(ToType)
						{
						case CGItemSynch::POS_PLAYERSHOP:
							{//背包->商店,指定位置

								CGManuMoveItemFromBagToPlayerShop_t ExtraData;
                                Assert(ExtraInfoL == ExtraData.GetSize());
								memcpy(&ExtraData, pExtraInfo, ExtraInfoL);
								BYTE				nToIndex	= ToIndex;
								_PLAYERSHOP_GUID	nShopID		= ExtraData.m_ShopGuid;
								BYTE				nStallIndex	= ExtraData.m_nStallIndex;
								UINT				uSerial		= ExtraData.m_uSerial;//商店里此位置的序列号(目的位置序列号)
								BYTE				nShopSerial	= ExtraData.m_nShopSerial;

                                PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
								PlayerShop*		pPlayerShop		= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
								Assert(pPlayerShop);
								PlayerStallBox* pStallBox		= pPlayerShop->GetPlayerStallBoxByIndex(nStallIndex);
								Assert(pStallBox);
								ItemContainer*	pStallContainer = pStallBox->GetContainer();

								//是不是自己的店
								BOOL bIsMine = (pHuman->GetGUID() == pPlayerShop->GetOwnerGuid())? TRUE:FALSE;

								//是不是自己可以管理的店
								BOOL bCanManager = pPlayerShop->IsPartner(pHuman->GetGUID());

								if(bIsMine == FALSE && bCanManager == FALSE)
								{
									//log...
									return PACKET_EXE_CONTINUE;
								}

								if(nShopSerial != pPlayerShop->GetSerial())
								{
									MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_SALE_OUT_SERIAL_IS_CHANGED);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "CGItemSynchHandler::Name=%s Serials had changed"
										,pHuman->GetName()) ;
									return PACKET_EXE_CONTINUE ;
								}

								Assert(pStallContainer);

								if( pStallBox->GetSerialByIndex(nToIndex) != uSerial)
								{//目的位置的物品已经改变
									MsgError.SetID(PLAYERSHOP_MSG::ERR_NEED_NEW_COPY);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NEED_NEW_COPY: Serial = %d, BoxSerial = %d"
										,pHuman->GetName(), uSerial, pStallBox->GetSerialByIndex(nToIndex)) ;
									return PACKET_EXE_CONTINUE;
								}

								//ItemContainer* pMatContainer	= pHuman->GetMatContain();
								ItemContainer* pBaseContainer	= pHuman->GetBaseContain();
								//ItemContainer* pBagContainer	= NULL;

								//if(pMatContainer->GetIndexByGUID(&ItemGuid) >= 0)
								//{
								//	pBagContainer = pMatContainer;
								//}
								//else if(pBaseContainer->GetIndexByGUID(&ItemGuid) >=0 )
								//{
								//	pBagContainer = pBaseContainer;
								//}
								//else
								//{//guid非法
								////	Assert(0);
								//	MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
								//	pGamePlayer->SendPacket(&MsgError);
								//	g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL:World = %d, Server = %d, Serial = %d "
								//		,pHuman->GetName(), ItemGuid.m_World, ItemGuid.m_Server, ItemGuid.m_Serial) ;
								//	return PACKET_EXE_CONTINUE ;
								//}

								INT	IndexInBag = pBaseContainer->GetIndexByGUID(&ItemGuid);

								Item* pDestItem = pStallContainer->GetItem(nToIndex);
								if(!pDestItem->IsEmpty())
								{//此位置已经有东西了，只能与背包中的物品交换
									//交换
									INT result = 
										g_ItemOperator.ExchangeItem
										(
										pBaseContainer,
										IndexInBag,
										pStallContainer,
										nToIndex
										);

									if(result>=0)
									{//移动成功
										//发送消息给客户端
										Item* pIt = pStallContainer->GetItem(nToIndex);
										if(!pIt->IsEmpty())
										{
											//序列号递增
											uFinalSerial = pStallBox->IncSerialByIndex(nToIndex);

											Item* pItemInBag = pBaseContainer->GetItem(IndexInBag);

											//上货：AAA取回了BBBC件。（AAA为操作者名，BBB为商品名C为件数）
											RecordOpt::Excute(REC_DELITEM, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (INT)pItemInBag->GetItemTableIndex(), (INT)pItemInBag->GetLayedNum());

											//上货：AAA上货了BBBC件。（AAA为操作者名，BBB为商品名C为件数）
											RecordOpt::Excute(REC_ADDITEM, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (INT)pIt->GetItemTableIndex(), (INT)pIt->GetLayedNum());
											
											//发送消息给客户端
											MsgToClient.SetOpt(GCItemSynch::OPT_EXCHANGE_ITEM);
											MsgToClient.SetFromType(GCItemSynch::POS_BAG);
											MsgToClient.SetToType(GCItemSynch::POS_PLAYERSHOP);
											MsgToClient.SetToIndex(nToIndex);
											MsgToClient.SetItemGuid(ItemGuid);

											GCExchangeItemFromBagToPlayerShop_t ExtraInfoData;
											ExtraInfoData.m_ShopGuid	= nShopID;
											ExtraInfoData.m_nStallIndex = nStallIndex;
											ExtraInfoData.m_uSerial		= uFinalSerial;
											ExtraInfoData.m_nShopSerial	= pPlayerShop->IncSerial();
											MsgToClient.SetExtraInfoLength(ExtraInfoData.GetSize());
											MsgToClient.SetExtraInfoData((BYTE*)&ExtraInfoData);
											pGamePlayer->SendPacket(&MsgToClient);
										}
									}
									else
									{//失败
										Assert(0);	
										MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL, POS_BAG->POS_PLAYERSHOP MANU POS ExchangeItem"
											,pHuman->GetName()) ;
										return PACKET_EXE_CONTINUE ;
									}
								}
								else
								{//此位置没东西，移动过来
										INT result = 
										g_ItemOperator.MoveItem
										(
										pBaseContainer,
										IndexInBag,
										pStallContainer,
										nToIndex
										);

									if(result>=0)
									{//移动成功
										//发送消息给客户端
										Item* pIt = pStallContainer->GetItem(nToIndex);
										if(!pIt->IsEmpty())
										{
											//序列号递增
											uFinalSerial = pStallBox->IncSerialByIndex(nToIndex);

											//上货：AAA上货了BBBC件。（AAA为操作者名，BBB为商品名C为件数）
											RecordOpt::Excute(REC_ADDITEM, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (INT)pIt->GetItemTableIndex(), (INT)pIt->GetLayedNum());

											//发送消息给客户端
											MsgToClient.SetOpt(GCItemSynch::OPT_MOVE_ITEM);
											MsgToClient.SetFromType(GCItemSynch::POS_BAG);
											MsgToClient.SetToType(GCItemSynch::POS_PLAYERSHOP);
											MsgToClient.SetToIndex(nToIndex);
											MsgToClient.SetItemGuid(ItemGuid);

											GCMoveItemFromBagToPlayerShop_t ExtraInfoData;
											ExtraInfoData.m_ShopGuid	= nShopID;
											ExtraInfoData.m_nStallIndex = nStallIndex;
											ExtraInfoData.m_uSerial		= uFinalSerial;
											ExtraInfoData.m_nShopSerial	= pPlayerShop->IncSerial();
											MsgToClient.SetExtraInfoLength(ExtraInfoData.GetSize());
											MsgToClient.SetExtraInfoData((BYTE*)&ExtraInfoData);
											pGamePlayer->SendPacket(&MsgToClient);
										}
									}
									else
									{//失败
										Assert(0);
										MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL, POS_BAG->POS_PLAYERSHOP MANU POS MoveItem"
											,pHuman->GetName()) ;
										return PACKET_EXE_CONTINUE ;
									}
								}
							}
							break;
						default:
							break;
						}
					}
					break;
				case CGItemSynch::POS_PET:
					{
						switch(ToType)
						{
						case CGItemSynch::POS_PLAYERSHOP:
							{//宠物->商店

							}
							break;
						default:
							break;
						}
					}
					break;
				case CGItemSynch::POS_PLAYERSHOP:
					{
						switch(ToType)
						{
						case CGItemSynch::POS_BAG:
							{//商店->背包
								CGManuMoveItemFromPlayerShopToBag_t ExtraData;
								Assert(ExtraInfoL == ExtraData.GetSize());
								memcpy(&ExtraData, pExtraInfo, ExtraInfoL);
								BYTE				nToIndex	= ToIndex;
								_PLAYERSHOP_GUID	nShopID		= ExtraData.m_ShopGuid;
								BYTE				nStallIndex	= ExtraData.m_nStallIndex;
								UINT				uSerial		= ExtraData.m_uSerial;//商店里此位置的序列号
								BYTE				nShopSerial	= ExtraData.m_nShopSerial;

								PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
								PlayerShop*		pPlayerShop		= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
								Assert(pPlayerShop);
								PlayerStallBox* pStallBox		= pPlayerShop->GetPlayerStallBoxByIndex(nStallIndex);
								Assert(pStallBox);
								ItemContainer*	pStallContainer = pStallBox->GetContainer();

								//是不是自己的店
								BOOL bIsMine = (pHuman->GetGUID() == pPlayerShop->GetOwnerGuid())? TRUE:FALSE;

								//是不是自己可以管理的店
								BOOL bCanManager = pPlayerShop->IsPartner(pHuman->GetGUID());

								if(bIsMine == FALSE && bCanManager == FALSE)
								{
									//log...
									return PACKET_EXE_CONTINUE;
								}

								if(nShopSerial != pPlayerShop->GetSerial())
								{
									MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_SALE_OUT_SERIAL_IS_CHANGED);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "CGItemSynchHandler::Name=%s Serials had changed"
										,pHuman->GetName()) ;
									return PACKET_EXE_CONTINUE ;
								}

								Assert(pStallContainer);

								INT	IndexInStall = pStallContainer->GetIndexByGUID(&ItemGuid);
								if(IndexInStall<0)
								{
									MsgError.SetID(PLAYERSHOP_MSG::ERR_NEED_NEW_COPY);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NEED_NEW_COPY: IndexInStall = %d"
										,pHuman->GetName(), IndexInStall) ;
									return PACKET_EXE_CONTINUE;
								}

								if( pStallBox->GetSerialByIndex(IndexInStall) > uSerial)
								{
									MsgError.SetID(PLAYERSHOP_MSG::ERR_NEED_NEW_COPY);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NEED_NEW_COPY: Serial = %d, BoxSerial = %d"
										,pHuman->GetName(), uSerial, pStallBox->GetSerialByIndex(IndexInStall)) ;
									return PACKET_EXE_CONTINUE;
								}

								Item* pItem = pStallContainer->GetItem(IndexInStall);

								ItemContainer* pBagContainer = HumanItemLogic::GetItemContain(pHuman, pItem->GetItemTableIndex());
								Item* pDestItem = pBagContainer->GetItem(nToIndex);
								
								if(!pDestItem->IsEmpty())
								{//此位置已经有东西了，只能与背包中的物品交换
									//交换
									INT result = 
										g_ItemOperator.ExchangeItem
										(
										pStallContainer,
										IndexInStall,
										pBagContainer,
										nToIndex
										);

									if(result>=0)
									{//移动成功
										//发送消息给客户端
										Item* pIt = pBagContainer->GetItem(nToIndex);
										if(!pIt->IsEmpty())
										{
											//序列号递增
											uFinalSerial = pStallBox->IncSerialByIndex(IndexInStall);
											uFinalToIndex	= pBagContainer->ConIndex2BagIndex(nToIndex);

											Item* pItemInStall = pStallContainer->GetItem(IndexInStall);

											//上货：AAA取回了BBBC件。（AAA为操作者名，BBB为商品名C为件数）
											RecordOpt::Excute(REC_DELITEM, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (INT)pIt->GetItemTableIndex(), (INT)pIt->GetLayedNum());

											//上货：AAA上货了BBBC件。（AAA为操作者名，BBB为商品名C为件数）
											RecordOpt::Excute(REC_ADDITEM, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (INT)pItemInStall->GetItemTableIndex(), (INT)pItemInStall->GetLayedNum());

											//发送消息给客户端
											MsgToClient.SetOpt(GCItemSynch::OPT_EXCHANGE_ITEM);
											MsgToClient.SetFromType(GCItemSynch::POS_PLAYERSHOP);
											MsgToClient.SetToType(GCItemSynch::POS_BAG);
											MsgToClient.SetToIndex(uFinalToIndex);
											MsgToClient.SetItemGuid(ItemGuid);

											GCExchangeItemFromPlayerShopToBag_t ExtraInfoData;
											ExtraInfoData.m_ShopGuid		= nShopID;
											ExtraInfoData.m_nStallIndex		= nStallIndex;
											ExtraInfoData.m_uSerial			= uFinalSerial;
											ExtraInfoData.m_nShopSerial		= pPlayerShop->IncSerial();
											MsgToClient.SetExtraInfoLength(ExtraInfoData.GetSize());
											MsgToClient.SetExtraInfoData((BYTE*)&ExtraInfoData);
											pGamePlayer->SendPacket(&MsgToClient);
										}
									}
									else
									{//失败
										Assert(0);
										MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL, POS_PLAYERSHOP->POS_BAG MANU POS ExchangeItem"
											,pHuman->GetName()) ;
										return PACKET_EXE_CONTINUE ;										
									}
								}
								else
								{//此位置没东西，移动过来
									//拷贝到摊位盒
									//自动找格
									INT result = 
										g_ItemOperator.MoveItem
										(
										pStallContainer,
										IndexInStall,
										pBagContainer,
										nToIndex
										);

									if(result>=0)
									{//移动成功
										//发送消息给客户端
										Item* pIt = pBagContainer->GetItem(nToIndex);
										if(!pIt->IsEmpty())
										{
											//序列号递增
											uFinalSerial = pStallBox->IncSerialByIndex(IndexInStall);
											uFinalToIndex = pBagContainer->ConIndex2BagIndex(nToIndex);

											//上货：AAA取回了BBBC件。（AAA为操作者名，BBB为商品名C为件数）
											RecordOpt::Excute(REC_DELITEM, pPlayerShop->GetManagerRecord(), (CHAR*)pHuman->GetName(), (INT)pIt->GetItemTableIndex(), (INT)pIt->GetLayedNum());

											//发送消息给客户端
											MsgToClient.SetOpt(GCItemSynch::OPT_MOVE_ITEM );
											MsgToClient.SetFromType(GCItemSynch::POS_PLAYERSHOP);
											MsgToClient.SetToType(GCItemSynch::POS_BAG);
											MsgToClient.SetToIndex(uFinalToIndex);
											MsgToClient.SetItemGuid(ItemGuid);

											GCMoveItemFromPlayerShopToBag_t ExtraInfoData;
											ExtraInfoData.m_ShopGuid	= nShopID;
											ExtraInfoData.m_nStallIndex = nStallIndex;
											ExtraInfoData.m_Flag		= 1;
											ExtraInfoData.m_uSerial		= uFinalSerial;
											ExtraInfoData.m_nShopSerial	= pPlayerShop->IncSerial();
											MsgToClient.SetExtraInfoLength(ExtraInfoData.GetSize());
											MsgToClient.SetExtraInfoData((BYTE*)&ExtraInfoData);
											pGamePlayer->SendPacket(&MsgToClient);
										}
									}
									else
									{//失败
										Assert(0);
										MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL, POS_PLAYERSHOP->POS_BAG MANU POS MoveItem"
											,pHuman->GetName()) ;
										return PACKET_EXE_CONTINUE ;
									}
								}
							}
							break;
						case CGItemSynch::POS_PLAYERSHOP:
							{//商店->商店

								CGManuMoveItemFromPlayerShopToPlayerShop_t ExtraData;
								Assert(ExtraInfoL == ExtraData.GetSize());
								memcpy(&ExtraData, pExtraInfo, ExtraInfoL);
								BYTE nToIndex				= ToIndex;
								_PLAYERSHOP_GUID	nShopID = ExtraData.m_ShopGuid;
								BYTE nStallIndex			= ExtraData.m_nStallIndex;
								UINT uSerialSource			= ExtraData.m_uSerialSource;//商店里源位置的序列号
								UINT uSerialDest			= ExtraData.m_uSerialDest;//商店里目的位置的序列号
								BYTE nShopSerial			= ExtraData.m_nShopSerial;
							
								PlayerShopManager*	pPlayerShopManager = pScene->GetPlayerShopManager();
								PlayerShop*		pPlayerShop		= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
								PlayerStallBox* pStallBox		= pPlayerShop->GetPlayerStallBoxByIndex(nStallIndex);
								ItemContainer*	pStallContainer = pStallBox->GetContainer();

								Assert(pStallContainer);

								INT	IndexInStallSource	= pStallContainer->GetIndexByGUID(&ItemGuid);
								INT	IndexInStallDest	= nToIndex;

								if(nShopSerial != pPlayerShop->GetSerial())
								{
									MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_SALE_OUT_SERIAL_IS_CHANGED);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "CGItemSynchHandler::Name=%s Serials had changed"
										,pHuman->GetName()) ;
									return PACKET_EXE_CONTINUE ;
								}

								if( IndexInStallSource<0 )
								{
									MsgError.SetID(PLAYERSHOP_MSG::ERR_NEED_NEW_COPY);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NEED_NEW_COPY: IndexInStallSource = %d, IndexInStallDest = %d"
										,pHuman->GetName(), IndexInStallSource, IndexInStallDest) ;
									return PACKET_EXE_CONTINUE;
								}

								if( pStallBox->GetSerialByIndex(IndexInStallSource) > uSerialSource || pStallBox->GetSerialByIndex(IndexInStallDest) > uSerialDest)
								{
									MsgError.SetID(PLAYERSHOP_MSG::ERR_NEED_NEW_COPY);
									pGamePlayer->SendPacket(&MsgError);
									g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_NEED_NEW_COPY: SerialSource = %d, BoxSerialDest = %d, SerialDest = %d, BoxSerialDeset = %d"
										,pHuman->GetName(), uSerialSource, pStallBox->GetSerialByIndex(IndexInStallSource), uSerialDest, pStallBox->GetSerialByIndex(IndexInStallDest)) ;
									return PACKET_EXE_CONTINUE;
								}

								Item* pItemSource	= pStallContainer->GetItem(IndexInStallSource);
								Item* pItemDest		= pStallContainer->GetItem(IndexInStallDest);

								if(!pItemDest->IsEmpty())
								{//此位置已经有东西了，只能与此位置中的物品交换
									//交换
									INT result = 
										g_ItemOperator.ExchangeItem
										(
										pStallContainer,
										IndexInStallSource,
										pStallContainer,
										IndexInStallDest
										);

									if(result>=0)
									{//移动成功
										//发送消息给客户端
										Item* pIt = pStallContainer->GetItem(IndexInStallDest);
										if(!pIt->IsEmpty())
										{
											//序列号递增
											UINT uFinalSerialSource	= pStallBox->IncSerialByIndex(IndexInStallSource);
											UINT uFinalSerialDest	= pStallBox->IncSerialByIndex(IndexInStallDest);
											
											//发送消息给客户端
											MsgToClient.SetOpt(GCItemSynch::OPT_EXCHANGE_ITEM);
											MsgToClient.SetFromType(GCItemSynch::POS_PLAYERSHOP);
											MsgToClient.SetToType(GCItemSynch::POS_PLAYERSHOP);
											MsgToClient.SetToIndex(IndexInStallDest);
											MsgToClient.SetItemGuid(ItemGuid);

											GCExchangeItemFromPlayerShopToPlayerShop_t ExtraInfoData;
											ExtraInfoData.m_ShopGuid		= nShopID;
											ExtraInfoData.m_nStallIndex		= nStallIndex;
											ExtraInfoData.m_uSerialSource	= uFinalSerialSource;
											ExtraInfoData.m_uSerialDest		= uFinalSerialDest;
											ExtraInfoData.m_nShopSerial		= pPlayerShop->IncSerial();
											MsgToClient.SetExtraInfoLength(ExtraInfoData.GetSize());
											MsgToClient.SetExtraInfoData((BYTE*)&ExtraInfoData);
											pGamePlayer->SendPacket(&MsgToClient);
										}
									}
									else
									{//失败
										Assert(0);
										MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL, POS_PLAYERSHOP->POS_PLAYERSHOP, MANU POS ExchangeItem"
											,pHuman->GetName()) ;
										return PACKET_EXE_CONTINUE ;
									}
								}
								else
								{//此位置没东西，移动过来
									//拷贝到摊位盒
									//自动找格
									INT result = 
										g_ItemOperator.MoveItem
										(
										pStallContainer,
										IndexInStallSource,
										pStallContainer,
										IndexInStallDest
										);

									if(result>=0)
									{//移动成功
										//发送消息给客户端
										Item* pIt = pStallContainer->GetItem(IndexInStallDest);
										if(!pIt->IsEmpty())
										{
											//序列号递增
											UINT uFinalSerialSource	= pStallBox->IncSerialByIndex(IndexInStallSource);
											UINT uFinalSerialDest	= pStallBox->IncSerialByIndex(IndexInStallDest);
											
											//发送消息给客户端
											MsgToClient.SetOpt(GCItemSynch::OPT_MOVE_ITEM );
											MsgToClient.SetFromType(GCItemSynch::POS_PLAYERSHOP);
											MsgToClient.SetToType(GCItemSynch::POS_PLAYERSHOP);
											MsgToClient.SetToIndex(IndexInStallDest);
											MsgToClient.SetItemGuid(ItemGuid);

											GCMoveItemFromPlayerShopToPlayerShop_t ExtraInfoData;
											ExtraInfoData.m_ShopGuid		= nShopID;
											ExtraInfoData.m_nStallIndex		= nStallIndex;
											ExtraInfoData.m_uSerialSource	= uFinalSerialSource;
											ExtraInfoData.m_uSerialDest		= uFinalSerialDest;
											ExtraInfoData.m_nShopSerial		= pPlayerShop->IncSerial();
											MsgToClient.SetExtraInfoLength(ExtraInfoData.GetSize());
											MsgToClient.SetExtraInfoData((BYTE*)&ExtraInfoData);
											pGamePlayer->SendPacket(&MsgToClient);
										}
									}
									else
									{//失败
										Assert(0);
										MsgError.SetID(PLAYERSHOP_MSG::ERR_ILLEGAL);
										pGamePlayer->SendPacket(&MsgError);
										g_pLog->FastSaveLog( LOG_FILE_1, "ERROR: CGItemSynchHandler::ObjName=%s, ERR_ILLEGAL, POS_PLAYERSHOP->POS_PLAYERSHOP MANU POS MoveItem"
											,pHuman->GetName()) ;
										return PACKET_EXE_CONTINUE ;
									}
								}
							}
							break;
						default:
							break;
						}
					}
					break;
				default:
					break;
				}
			}
			break;
		default:
			break;
	}
	g_pLog->FastSaveLog( LOG_FILE_1, "CGItemSynchHandler::ObjName=%s"
		,pHuman->GetName()) ;
	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
