#include "stdafx.h"


#include "CGBankSwapItem.h"
#include "GCBankSwapItem.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "Log.h"

#include "ItemOperator.h"
#include "HumanItemLogic.h"
#include "GCItemInfo.h"
#include "GCBankItemInfo.h"


uint CGBankSwapItemHandler::Execute( CGBankSwapItem* pPacket, Player* pPlayer )
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

	UINT indexFrom = pPacket->GetIndexFrom();
	UINT indexTo   = pPacket->GetIndexTo();
	GCBankSwapItem Msg;

	switch(pPacket->GetFromType())
	{
	case CGBankSwapItem::EQUIP_POS:
		{
			switch(pPacket->GetToType())
			{
				case CGBankSwapItem::BAG_POS:
					{
					}
					break;
				case CGBankSwapItem::EQUIP_POS:
					{
					}
					break;
				case CGBankSwapItem::BANK_POS:
					{
					}
					break;
				default:
					break;
			}
		}
		break;
	case CGBankSwapItem::BAG_POS:
		{
			switch(pPacket->GetToType())
			{
			case CGBankSwapItem::BAG_POS:
				{
				}
				break;
			case CGBankSwapItem::EQUIP_POS:
				{
				}
				break;
			case CGBankSwapItem::BANK_POS:			// Bag->Bank
				{
					//验证位置的合法性
					if( indexFrom>=0 
						&& indexFrom<MAX_BAG_SIZE 
						&& indexTo>=0 
						&& indexTo<(UINT)(pHuman->__GetBankEndIndex()) 
						)
					{
						//验证位置里是否有东西
						if( ITEMREFPTRISVALID(HumanItemLogic::GetItem(pHuman, indexFrom))
							&&ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indexTo))
							)
						{
							ItemContainer*	pBagContainer =
								HumanItemLogic::GetContainer(pHuman,indexFrom);

							ItemContainer* pBankContainer = pHuman->GetBankContain();

							Item *pItem1 = pBagContainer->GetItem(pBagContainer->BagIndex2ConIndex(indexFrom));
							Item *pItem2 = pBankContainer->GetItem(indexTo);
							//检测是否能够合并，如果可以，就执行合并而不是交换
							if(pItem1->GetItemTableIndex() == pItem2->GetItemTableIndex() && pItem1->IsCanLay())
							{
								if(g_ItemOperator.MoveSpliceItem(  pBagContainer,
																(UCHAR)indexFrom,
																pBankContainer,
																indexTo)
									== ITEMOE_SUCCESS)
								{
									GCItemInfo msg1;
									msg1.setID(indexFrom);
									msg1.setIsNull(pItem1->IsEmpty());
									pItem1->SaveValueTo(msg1.getItem());
									pGamePlayer->SendPacket(&msg1);

									GCBankItemInfo msg2;
									msg2.setBankIndex(indexTo);
									msg2.setIsNull(pItem2->IsEmpty());
									pItem2->SaveValueTo(msg2.getItem());
									pGamePlayer->SendPacket(&msg2);
								}
								g_pLog->FastSaveLog( LOG_FILE_1, "CGBankSwapItemHandler: MoveSpliceItem" ) ;
								return PACKET_EXE_CONTINUE ;
							}
							else
							{
								//判断银行内的这个物品是不是能够放入背包中
								ItemContainer* pBankContainer = pHuman->GetBankContain();
								Item *pItem = pBankContainer->GetItem(indexTo); 

								ItemContainer *pHumanCon = HumanItemLogic::GetItemContain(pHuman, pItem->GetItemTableIndex());
								if( !pHumanCon )
								{
									//返回错误信息
									Msg.SetToType(GCBankSwapItem::ERROR_POS);
									break;
								}

								if( !pHumanCon->IsInContainer(indexFrom) )
								{
									//返回错误信息
									Msg.SetToType(GCBankSwapItem::ERROR_POS);
									break;
								}

								g_ItemOperator.ExchangeItem(pBagContainer,
									pBagContainer->BagIndex2ConIndex(indexFrom),
									pBankContainer,
									(UINT)indexTo);

								Msg.SetFromType(GCBankSwapItem::BAG_POS);
								Msg.SetToType(GCBankSwapItem::BANK_POS);
								Msg.SetIndexFrom(indexFrom);
								Msg.SetIndexTo(indexTo);
							}
						}
						else
						{
							Msg.SetFromType(GCBankSwapItem::ERROR_POS);
							Msg.SetToType(GCBankSwapItem::ERROR_POS);
						}
					}
					else
					{
						Msg.SetFromType(GCBankSwapItem::ERROR_POS);
						Msg.SetToType(GCBankSwapItem::ERROR_POS);
					}
				}
				break;
			default:
				break;
			}
		}
		break;
	case CGBankSwapItem::BANK_POS:
		{
			switch(pPacket->GetToType())
			{
			case CGBankSwapItem::BAG_POS:			// Bank->Bag
				{
					//验证位置的合法性
					if( indexTo>=0 
						&& indexTo<MAX_BAG_SIZE 
						&& indexFrom>=0 
						&& indexFrom<(UINT)(pHuman->__GetBankEndIndex()) 
						)
					{
						if(ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indexFrom)) 
							&& ITEMREFPTRISVALID(HumanItemLogic::GetItem(pHuman, indexTo))
							)
						{
							ItemContainer*	pBagContainer =
								HumanItemLogic::GetContainer(pHuman,indexFrom);

							ItemContainer* pBankContainer = pHuman->GetBankContain();

							//判断银行内的这个物品是不是能够放入背包中
							Item *pItem = pBankContainer->GetItem(indexFrom); 

							ItemContainer *pHumanCon = HumanItemLogic::GetItemContain(pHuman, pItem->GetItemTableIndex());
							if( !pHumanCon )
							{
								//返回错误信息
								Msg.SetToType(GCBankSwapItem::ERROR_POS);
								break;
							}

							if( !pHumanCon->IsInContainer(indexTo) )
							{
								//返回错误信息
								Msg.SetToType(GCBankSwapItem::ERROR_POS);
								break;
							}

							g_ItemOperator.ExchangeItem(pBagContainer,
								pBagContainer->BagIndex2ConIndex(indexTo),
								pBankContainer,
								(UINT)indexFrom);

							//pHuman->GetDB()->SwapBankWithBag(indexTo, indexFrom);
							Msg.SetFromType(GCBankSwapItem::BANK_POS);
							Msg.SetToType(GCBankSwapItem::BAG_POS);
							Msg.SetIndexFrom(indexFrom);
							Msg.SetIndexTo(indexTo);
						}
						else
						{
							Msg.SetFromType(GCBankSwapItem::ERROR_POS);
							Msg.SetToType(GCBankSwapItem::ERROR_POS);
						}
					}
					else
					{
						Msg.SetFromType(GCBankSwapItem::ERROR_POS);
						Msg.SetToType(GCBankSwapItem::ERROR_POS);
					}

				}
				break;
			case CGBankSwapItem::EQUIP_POS:
				{
				}
				break;
			case CGBankSwapItem::BANK_POS:			//Bank->Bank
				{
					ItemContainer *pBankContainer  = pHuman->GetBankContain();

					Item *pItem1  = pBankContainer->GetItem( indexFrom );

					INT nSpliceType	= 0;
					INT nSpliceTo	= -1;
					//银行容器
					ItemContainer* pBankContain = pHuman->GetBankContain();
					Item *pItem = pBankContain->GetItem(indexFrom);
					UINT TempIndexTo =  indexTo;

					if(pItem->IsEmpty()||pItem1->IsEmpty())
					{
						g_pLog->FastSaveLog( LOG_FILE_1, "CGBankSwapItemHandler: Too much click" ) ;
						return PACKET_EXE_CONTINUE;
					}
					
					switch(TempIndexTo)
					{
					case CGBankSwapItem::AUTO_POS_BOX1:
						{
							if(pItem1->IsCanLay())
							{
								for(INT i=RENTBOX1_START_INDEX; i<RENTBOX2_START_INDEX; i++)
								{
									Item *pItem2 = pBankContain->GetItem(i);
									if( pItem1->GetItemTableIndex() == pItem2->GetItemTableIndex() && 
										pItem1->GetLayedNum() + pItem2->GetLayedNum() <= pItem2->GetMaxLayedNum() )
									{
										nSpliceType = 1;
										nSpliceTo = i;
										indexTo = i;
										break;
									}
								}
							}

							BYTE	indextemp = RENTBOX1_START_INDEX;
							if(0 == nSpliceType)
							{
								while( ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indextemp)) 
									&&  indextemp < RENTBOX2_START_INDEX
									)
								{
									indextemp++;
								}

								if(indextemp == RENTBOX2_START_INDEX)
								{
									indexTo = indextemp-1;
								}
								else
								{
									indexTo = indextemp;
								}
							}
						}
						break;
					case CGBankSwapItem::AUTO_POS_BOX2:
						{
							if(pItem1->IsCanLay())
							{
								for(INT i=RENTBOX2_START_INDEX; i<RENTBOX3_START_INDEX; i++)
								{
									Item *pItem2 = pBankContain->GetItem(i);
									if( pItem1->GetItemTableIndex() == pItem2->GetItemTableIndex() && 
										pItem1->GetLayedNum() + pItem2->GetLayedNum() <= pItem2->GetMaxLayedNum() )
									{
										nSpliceType = 1;
										nSpliceTo = i;
										indexTo = i;
										break;
									}
								}
							}
							BYTE	indextemp = RENTBOX2_START_INDEX;
							if(0 == nSpliceType)
							{
								while( ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indextemp)) 
									&&  indextemp < RENTBOX3_START_INDEX
									)
								{
									indextemp++;
								}

								if(indextemp == RENTBOX3_START_INDEX)
								{
									indexTo = indextemp-1;
								}
								else
								{
									indexTo = indextemp;
								}
							}
						}
						break;
					case CGBankSwapItem::AUTO_POS_BOX3:
						{
							if(pItem1->IsCanLay())
							{
								for(INT i=RENTBOX3_START_INDEX; i<RENTBOX4_START_INDEX; i++)
								{
									Item *pItem2 = pBankContain->GetItem(i);
									if( pItem1->GetItemTableIndex() == pItem2->GetItemTableIndex() && 
										pItem1->GetLayedNum() + pItem2->GetLayedNum() <= pItem2->GetMaxLayedNum() )
									{
										nSpliceType = 1;
										nSpliceTo = i;
										indexTo = i;
										break;
									}
								}
							}
							BYTE	indextemp = RENTBOX3_START_INDEX;
							if(0 == nSpliceType)
							{
								while( ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indextemp)) 
									&&  indextemp < RENTBOX4_START_INDEX
									)
								{
									indextemp++;
								}

								if(indextemp == RENTBOX4_START_INDEX)
								{
									indexTo = indextemp-1;
								}
								else
								{
									indexTo = indextemp;
								}
							}
						}
						break;
					case CGBankSwapItem::AUTO_POS_BOX4:
						{
							if(pItem1->IsCanLay())
							{
								for(INT i=RENTBOX4_START_INDEX; i<RENTBOX5_START_INDEX; i++)
								{
									Item *pItem2 = pBankContain->GetItem(i);
									if( pItem1->GetItemTableIndex() == pItem2->GetItemTableIndex() && 
										pItem1->GetLayedNum() + pItem2->GetLayedNum() <= pItem2->GetMaxLayedNum() )
									{
										nSpliceType = 1;
										nSpliceTo = i;
										indexTo = i;
										break;
									}
								}
							}
							BYTE	indextemp = RENTBOX4_START_INDEX;
							if(0 == nSpliceType)
							{
								while( ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indextemp)) 
									&&  indextemp < RENTBOX5_START_INDEX
									)
								{
									indextemp++;
								}

								if(indextemp == RENTBOX5_START_INDEX)
								{
									indexTo = indextemp-1;
								}
								else
								{
									indexTo = indextemp;
								}
							}
						}
						break;
					case CGBankSwapItem::AUTO_POS_BOX5:
						{
							if(pItem1->IsCanLay())
							{
								for(INT i=RENTBOX5_START_INDEX; i<MAX_BANK_SIZE; i++)
								{
									Item *pItem2 = pBankContain->GetItem(i);
									if( pItem1->GetItemTableIndex() == pItem2->GetItemTableIndex() && 
										pItem1->GetLayedNum() + pItem2->GetLayedNum() <= pItem2->GetMaxLayedNum() )
									{
										nSpliceType = 1;
										nSpliceTo = i;
										indexTo = i;
										break;
									}
								}
							}
							BYTE	indextemp = RENTBOX5_START_INDEX;
							if(0 == nSpliceType)
							{
								while( ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indextemp)) 
									&&  indextemp < MAX_BANK_SIZE
									)
								{
									indextemp++;
								}

								if(indextemp == MAX_BANK_SIZE)
								{
									indexTo = indextemp-1;
								}
								else
								{
									indexTo = indextemp;
								}
							}
						}
						break;
					default:
						{
							if(pItem->IsCanLay())
							{
								Item *pItem2   = pBankContainer->GetItem(indexTo);
								if( pItem1->GetItemTableIndex() == pItem2->GetItemTableIndex() )
								{
									nSpliceType = 2;
									nSpliceTo = indexTo;
								}
							}
						}
						break;
					}

					//验证位置的合法性
					if( indexTo>=0 
						&& indexTo<(UINT)(pHuman->__GetBankEndIndex()) 
						&& indexFrom>=0 
						&& indexFrom<(UINT)(pHuman->__GetBankEndIndex()) 
						)
					{
						if(1 == nSpliceType)	//执行合并
						{
							//验证位置里是否有东西
							if(ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indexFrom)))
							{
								if(ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, nSpliceTo)))
								{
									g_ItemOperator.SpliceItem(pBankContainer,
										(UINT)indexFrom,
										pBankContainer,
										(UINT)nSpliceTo);

									Item *pItem1 = pBankContainer->GetItem(indexFrom);
									Item *pItem2 = pBankContainer->GetItem(nSpliceTo);

									//发一个通知更新银行的消息
									GCBankItemInfo MsgBankItemInfo1;
									MsgBankItemInfo1.setBankIndex((WORD)indexFrom);
									MsgBankItemInfo1.setIsNull(pItem1->IsEmpty());
									pItem1->SaveValueTo(MsgBankItemInfo1.getItem());
									pGamePlayer->SendPacket( &MsgBankItemInfo1 );

									//发送一个通知背包更新的消息
									GCBankItemInfo MsgBankItemInfo2;
									MsgBankItemInfo2.setBankIndex((WORD)nSpliceTo);
									MsgBankItemInfo2.setIsNull(pItem2->IsEmpty());
									pItem2->SaveValueTo(MsgBankItemInfo2.getItem());
									pGamePlayer->SendPacket( &MsgBankItemInfo2 );
								}
							}
							g_pLog->FastSaveLog( LOG_FILE_1, "CGBankSwapItemHandler: empty item" ) ;
							return PACKET_EXE_CONTINUE;
						}
						if(2 == nSpliceType)	//执行合并
						{
							//验证位置里是否有东西
							if(ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indexFrom)))
							{
								if(ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, nSpliceTo)))
								{
									g_ItemOperator.MoveSpliceItem(pBankContainer,
										(UINT)indexFrom,
										pBankContainer,
										(UINT)nSpliceTo);

									Item *pItem1 = pBankContainer->GetItem(indexFrom);
									Item *pItem2 = pBankContainer->GetItem(nSpliceTo);

									//发一个通知更新银行的消息
									GCBankItemInfo MsgBankItemInfo1;
									MsgBankItemInfo1.setBankIndex((WORD)indexFrom);
									MsgBankItemInfo1.setIsNull(pItem1->IsEmpty());
									pItem1->SaveValueTo(MsgBankItemInfo1.getItem());
									pGamePlayer->SendPacket( &MsgBankItemInfo1 );

									//发送一个通知背包更新的消息
									GCBankItemInfo MsgBankItemInfo2;
									MsgBankItemInfo2.setBankIndex((WORD)nSpliceTo);
									MsgBankItemInfo2.setIsNull(pItem2->IsEmpty());
									pItem2->SaveValueTo(MsgBankItemInfo2.getItem());
									pGamePlayer->SendPacket( &MsgBankItemInfo2 );
								}
							}

							g_pLog->FastSaveLog( LOG_FILE_1, "CGBankSwapItemHandler: empty item indexFrom = %d", indexFrom ) ;
							return PACKET_EXE_CONTINUE;
						}

						if(indexTo == indexFrom)
						{
							/*Msg.SetFromType(GCBankSwapItem::ERROR_POS);
							Msg.SetToType(GCBankSwapItem::ERROR_POS);*/
						}
                        else if(ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indexFrom)))
						{
							if(ITEMREFPTRISVALID(HumanItemLogic::GetBankItem(pHuman, indexTo)))
							{
								ItemContainer* pBankContainer = pHuman->GetBankContain();

								g_ItemOperator.ExchangeItem(pBankContainer,
									(UINT)indexFrom,
									pBankContainer,
									(UINT)indexTo);
							}
							else
							{
								ItemContainer* pBankContainer = pHuman->GetBankContain();

								g_ItemOperator.MoveItem(pBankContainer,
									(UINT)indexFrom,
									pBankContainer,
									(UINT)indexTo);
							}

							//pHuman->GetDB()->SwapBankWithBank(indexTo, indexFrom);
							Msg.SetFromType(GCBankSwapItem::BANK_POS);
							Msg.SetToType(GCBankSwapItem::BANK_POS);
							Msg.SetIndexFrom(indexFrom);
							Msg.SetIndexTo(indexTo);
						}
						else
						{
							Msg.SetFromType(GCBankSwapItem::ERROR_POS);
							Msg.SetToType(GCBankSwapItem::ERROR_POS);
						}
					}
					else
					{
						Msg.SetFromType(GCBankSwapItem::ERROR_POS);
						Msg.SetToType(GCBankSwapItem::ERROR_POS);
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

	pGamePlayer->SendPacket(&Msg);
	g_pLog->FastSaveLog( LOG_FILE_1, "CGBankSwapItemHandler: indexFrom=%d, indexTo=%d", indexFrom, indexTo ) ;

		return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
