#include "stdafx.h"
#include "HumanItemLogic.h"
#include "Player.h"
#include "Item.h"
#include "ItemOperator.h"
#include "GCItemInfo.h"
#include "ItemRuler.h"
#include "GCNotifyEquip.h"
#include "Obj_Human.h"
#include "LogDefine.h"
#include "Scene.h"




ItemContainer*	HumanItemLogic::GetItemContain(Obj_Human* pHuman,UINT uItemIndex)
{
	__ENTER_FUNCTION

	Assert(uItemIndex>0);
	Assert(uItemIndex!=INVALID_ID);
	Assert(pHuman);

	UCHAR uClass =  GetSerialClass(uItemIndex);

	switch(uClass) 
	{
	case ICLASS_TASKITEM:
		//{
		//	return pHuman->GetTaskContain();
		//}
		//break;
	case ICLASS_GEM:
	case ICLASS_MATERIAL:
		//{
		//	return pHuman->GetMatContain();
		//}
		//break;
	case ICLASS_EQUIP:
	case ICLASS_COMITEM:

	case ICLASS_STOREMAP:
		{
			return pHuman->GetBaseContain();
		}
	default:
		return NULL;
		break;
	}
	__LEAVE_FUNCTION

	return	NULL;
}

BOOL	HumanItemLogic::EraseItem(ITEM_LOG_PARAM* pLogParam,
								  Obj_Human* pHuman,UINT uItemIndex,UINT uCount)
{

	__ENTER_FUNCTION

	Assert(uItemIndex!=INVALID_ID);
	Assert(uItemIndex>0);
	Assert(uCount>0);
	Assert(pHuman);
	Assert(pLogParam);
	
	if(uCount<0)
	{
		return	FALSE;
	}

	
	Item*				pItem;
	ItemContainer*		pContainer;
	UINT	i;

	for(i=0; i<MAX_BAG_SIZE && uCount>0; ++i)
	{
		pItem		=	GetItem(pHuman,i);
		pContainer	=	GetContainer(pHuman,i);

		Assert( pItem != NULL );
		if(!pItem)
		{return FALSE;}

		if( pItem->GetItemTableIndex() == uItemIndex )
		{
			if( pItem->GetMaxLayedNum() == 1 )
			{
				BOOL bRet =	EraseItem(pLogParam,pHuman,i);
				Assert(bRet);

				SaveItemLog(pLogParam);
				--uCount;
			}
			else
			{
				do
				{					
					g_ItemOperator.DecItemLayCount(pContainer,
												   pContainer->BagIndex2ConIndex(i),
												   1);
					--uCount;
		
				} while(pItem->GetLayedNum()>0 && uCount>0);
		
		
				if( pItem->GetLayedNum() <= 0 )
				{
					BOOL bRet	= EraseItem(pLogParam,pHuman,i);
					Assert(bRet);
					SaveItemLog(pLogParam);
		
				}
				else
				{
					GCItemInfo Msg;
					pItem->SaveValueTo(Msg.getItem());
					Msg.setID(i);
					Msg.setIsNull(FALSE);
					pHuman->GetPlayer()->SendPacket(&Msg);
				}
			}
		
		
		}
	}
	if(uCount>0)
		return FALSE;
		
	return TRUE;
		
	__LEAVE_FUNCTION
			
	return FALSE ;
}

BOOL	HumanItemLogic::EraseItem(ITEM_LOG_PARAM* pLogParam,Obj_Human* pHuman,UINT uBagIndex)
{

	__ENTER_FUNCTION
	
	Assert(uBagIndex<MAX_BAG_SIZE);
	Assert(pHuman);

	ItemContainer* pContainer = GetContainer(pHuman,uBagIndex);

	if(!pContainer)
	{
		Assert(FALSE);
		return FALSE;
	}

	Item* pItem = pContainer->GetItem(pContainer->BagIndex2ConIndex(uBagIndex));
	Assert(pItem);
	pLogParam->ItemGuid		=	pItem->GetGUID();
	pLogParam->Count		=	pItem->GetLayedNum();
	
	BOOL bRet = g_ItemOperator.EraseItem(pContainer,
				   pContainer->BagIndex2ConIndex(uBagIndex));

	if(bRet)
	{
		
		pLogParam->CharGUID		=	pHuman->GetGUID();
		pLogParam->SceneID		=	pHuman->getScene()->SceneID();
		pLogParam->XPos			=	pHuman->getWorldPos()->m_fX;
		pLogParam->ZPos			=	pHuman->getWorldPos()->m_fZ;
		
	}
	Assert(bRet);
	
	GCItemInfo	 Msg;
	Msg.setIsNull(TRUE);
	Msg.setID(uBagIndex);
	pHuman->GetPlayer()->SendPacket(&Msg);

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	HumanItemLogic::DecItemLayCount(Obj_Human* pHuman,UINT uBagIndex,UINT uCount /* =1 */)
{
	__ENTER_FUNCTION

	Assert(uBagIndex<MAX_BAG_SIZE);
	Assert(pHuman);

	ItemContainer* pContainer = GetContainer(pHuman,uBagIndex);

	if(!pContainer)
	{
		Assert(FALSE);
		return FALSE;
	}

	Item* pItem = pContainer->GetItem(pContainer->BagIndex2ConIndex(uBagIndex));

	if(!pItem)
	{
		Assert(FALSE);
		return FALSE;
	}

	if(pItem->IsEmpty())
		return FALSE;

	if(pItem->IsLock())
		return FALSE;

	if(pItem->GetLayedNum()==0)
	{
		Assert(FALSE);
	}
	else if(pItem->GetLayedNum()>0)
	{
		BOOL bRet = g_ItemOperator.DecItemLayCount(pContainer,
			pContainer->BagIndex2ConIndex(uBagIndex),
			uCount);

		if(bRet)
		{
			Item* pItem = pContainer->GetItem(pContainer->BagIndex2ConIndex(uBagIndex));
			if(pItem->GetLayedNum()==0)
			{
				g_ItemOperator.EraseItem(pContainer,
					pContainer->BagIndex2ConIndex(uBagIndex));

				GCItemInfo	Msg;
				Msg.setID(uBagIndex);
				Msg.setIsNull(TRUE);
				pHuman->GetPlayer()->SendPacket(&Msg);
			}
			else
			{
				GCItemInfo	 Msg;
				Msg.setID(uBagIndex);
				Msg.setIsNull(FALSE);
				pItem->SaveValueTo(Msg.getItem());
				pHuman->GetPlayer()->SendPacket(&Msg);
			}
			
		}

		return bRet;
	}
	

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	HumanItemLogic::IncItemLayCount(Obj_Human* pHuman,UINT uBagIndex,UINT uCount )
{
	__ENTER_FUNCTION
		
		Assert(uBagIndex<MAX_BAG_SIZE);
	Assert(pHuman);

	ItemContainer* pContainer = GetContainer(pHuman,uBagIndex);

	if(!pContainer)
	{
		Assert(FALSE);
		return FALSE;
	}

	Item* pItem = pContainer->GetItem(pContainer->BagIndex2ConIndex(uBagIndex));

	if(!pItem)
	{
		Assert(FALSE);
		return FALSE;
	}

	if(pItem->IsEmpty())
		return FALSE;

	if(pItem->IsLock())
		return FALSE;

	
		BOOL bRet = g_ItemOperator.IncItemLayCount(pContainer,
			pContainer->BagIndex2ConIndex(uBagIndex),
			uCount);

		if(bRet)
		{
			Item* pItem = pContainer->GetItem(pContainer->BagIndex2ConIndex(uBagIndex));
			
			GCItemInfo	Msg;
			Msg.setID(uBagIndex);
			Msg.setIsNull(FALSE);
			pItem->SaveValueTo(Msg.getItem());
			pHuman->GetPlayer()->SendPacket(&Msg);
		}
		return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	HumanItemLogic::EraseEquip(Obj_Human* pHuman,HUMAN_EQUIP EquipPoint)
{
	__ENTER_FUNCTION
	
		Assert(pHuman);
		Assert(EquipPoint<HEQUIP_NUMBER&&EquipPoint>=0);

		ItemContainer* pContainer =  pHuman->GetEquipContain();

		Assert(pContainer);

		return g_ItemOperator.EraseItem(pContainer,(UINT)EquipPoint);

	__LEAVE_FUNCTION

		return FALSE;
}


BOOL	HumanItemLogic::CalcItemSpace(Obj_Human* pHuman,UINT uItemIndex,INT Number,INT& NewSpace)
{
	__ENTER_FUNCTION

	 Assert(Number>0);
	 Assert(uItemIndex!=INVALID_ID);
		
	 INT		Count		= 0; // 空格子能够提供的空间
	 INT		PileCount	= 0; // 已经占用了格子的同类物品可以提供的容纳空间，仅对可叠加物品有效
	 INT		Tile		= 0; // 一格能够放几个同类物品
	 
	 _ITEM_TYPE	 type = ConvertSerial2ItemType(uItemIndex);
	 Tile	=	GetItemTileMax(type);

	 //找到对应的Container 

	 ItemContainer*	pContainer = GetItemContain(pHuman,uItemIndex);

	 Assert(pContainer);
     Assert(Tile>0);
	


	 for( UINT i=0; i<(UINT)pContainer->GetContainerSize(); ++i )
	 {
		 UINT iBagIndex = pContainer->ConIndex2BagIndex(i);
		 Item* pItem = GetItem(pHuman,iBagIndex);
		 Assert(pItem);

		 if( Tile == 1 )
			{
				if( pItem->IsEmpty() == TRUE )
				{
					++Count;
				}
			}
		 else
			{
				if( pItem->IsEmpty() == TRUE )
				{
					Count += Tile;
				}
				else if( (pItem->GetItemTableIndex() == uItemIndex)
					&& (pItem->IsCanLay()))
				{
					PileCount += Tile - pItem->GetLayedNum();
				}
			}
	 }

	 if( Count + PileCount < Number )
	 {
		 return FALSE;
	 }
	 else
	 {
		 Number		-=  PileCount;
		 if(Number<=0)
			 NewSpace = 0;
		 else
			 NewSpace	=	Number / Tile + ((Number % Tile) ? 1 : 0);
		 return TRUE;
	 }
	

	__LEAVE_FUNCTION

	return FALSE;
}

INT		HumanItemLogic::CalcBagSpace(Obj_Human* pHuman,UINT uItemIndex)
{
	__ENTER_FUNCTION
		
		Assert(pHuman);
		if(!pHuman)
		{
			Assert(FALSE);
			return 0;
		}

		ItemContainer*	pContainer = GetItemContain(pHuman,uItemIndex);
		Assert(pContainer);

		if(!pContainer)
		{
			Assert(FALSE);
			return 0;
		}

		return pContainer->CalcItemSpace();
	

	__LEAVE_FUNCTION

		return	0;
}


BOOL	HumanItemLogic::CanReceiveItemList(Obj_Human* pHuman,_ITEM_LIST& List)
{
	__ENTER_FUNCTION

	ItemContainer*  pBaseContain	= pHuman->GetBaseContain();
	Assert(pBaseContain);

	INT iBaseSize = pBaseContain->CalcItemSpace();

	for( INT iListIndex = 0; iListIndex < List.m_ListCount; iListIndex++ )
	{
		UINT uItemIndex = List.m_ListType[iListIndex].ToSerial();
		UINT uCount		= List.m_TypeCount[iListIndex];
		INT iNewSpace;
		if( HumanItemLogic::CalcItemSpace( pHuman, uItemIndex, uCount, iNewSpace ) )
		{
			iBaseSize -= iNewSpace;
			if( iBaseSize < 0 )
				return FALSE;
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;

/*	__ENTER_FUNCTION
		

		ItemContainer*  pBaseContain	= pHuman->GetBaseContain();
		ItemContainer*	pTaskContain	= pHuman->GetTaskContain();
		ItemContainer*	pMatContain		= pHuman->GetMatContain();

		Assert(pBaseContain);
		Assert(pTaskContain);
		Assert(pMatContain);

		INT iBaseSize = pBaseContain->CalcItemSpace();
		INT iTaskSize = pTaskContain->CalcItemSpace();
		INT iMatSize =  pMatContain->CalcItemSpace();


		for(INT iListIndex =0;iListIndex<List.m_ListCount;iListIndex++ )
		{	
			

			UINT uItemIndex = List.m_ListType[iListIndex].ToSerial();
			UINT uCount		= List.m_TypeCount[iListIndex];
			INT iNewSpace;
			if(HumanItemLogic::CalcItemSpace(pHuman,uItemIndex,uCount,iNewSpace))
			{

					UCHAR uClass =  GetSerialClass(uItemIndex);
				
						switch(uClass) 
						{
						case ICLASS_TASKITEM:
							{
								iTaskSize-=iNewSpace;
								if(iTaskSize<0)
									return FALSE;
							}
							break;
						case ICLASS_EQUIP:
						case ICLASS_COMITEM:
						case ICLASS_STOREMAP:
							{
								iBaseSize-=iNewSpace;
								if(iBaseSize<0)
									return FALSE;
							}
							break;
						case ICLASS_GEM:
						case ICLASS_MATERIAL:
							{
								iMatSize-= iNewSpace;
								if(iMatSize<0)
									return FALSE;
							}
							break;
						default:
							Assert(FALSE);
							return FALSE;
							break;
						}
			}
			else
				return FALSE;
	
		}
	
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;*/
}

BOOL	HumanItemLogic::ReceiveItemList(ITEM_LOG_PARAM* pLogParam,Obj_Human* pHuman,_ITEM_LIST& List, BYTE nCoinType)
{
	__ENTER_FUNCTION

	Assert(List.m_ListCount>0);

	for(INT i =0;i<List.m_ListCount;i++)
	{
		_ITEM_TYPE	itemType =List.m_ListType[i];
		if(!CreateMultiItemToBag(pLogParam,pHuman,itemType.ToSerial(),List.m_TypeCount[i], nCoinType))
		{
			return FALSE;
		}

	}
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}



Item*	HumanItemLogic::GetItem(Obj_Human* pHuman,UINT uBagIndex)
{
	__ENTER_FUNCTION
	
	Assert(uBagIndex<MAX_BAG_SIZE);
	Assert(pHuman);


	ItemContainer* pContainer = GetContainer(pHuman,uBagIndex);
	if(!pContainer)
	{	
		Assert(FALSE);
		return NULL;
	}

	return pContainer->GetItem(pContainer->BagIndex2ConIndex(uBagIndex));

		
	__LEAVE_FUNCTION

	return NULL;
}

INT HumanItemLogic::GetItemPosByGUID(Obj_Human* pHuman, _ITEM_GUID guid)
{
	Assert(pHuman);
	Assert(guid.isNull()==FALSE);
	
	INT iFinalIndex = INVALID_ID;

	//iFinalIndex = pHuman->GetTaskContain()->GetIndexByGUID( &guid );
	//
	//if(iFinalIndex!=INVALID_ID)
	//	return pHuman->GetTaskContain()->ConIndex2BagIndex(iFinalIndex);

	//iFinalIndex = pHuman->GetMatContain()->GetIndexByGUID(&guid);

	//if(iFinalIndex!=INVALID_ID)
	//	return pHuman->GetMatContain()->ConIndex2BagIndex(iFinalIndex);

	iFinalIndex = pHuman->GetBaseContain()->GetIndexByGUID( &guid );
	
	if(iFinalIndex!=INVALID_ID)
		return pHuman->GetBaseContain()->ConIndex2BagIndex(iFinalIndex);

	return INVALID_ID;
	
}

INT	HumanItemLogic::GetItemPosByType(Obj_Human* pHuman,UINT uItemIndex)
{
	Assert(uItemIndex>0);
	Assert(uItemIndex!=INVALID_ID);
	Assert(pHuman);
	
	ItemContainer* pItemContainer = GetItemContain(pHuman,uItemIndex);

	if(!pItemContainer)
	{
		Assert(FALSE);
		return INVALID_ID;
	}
	
	INT iFinalIndex = pItemContainer->GetIndexByType(uItemIndex);
	
	if(iFinalIndex!=INVALID_ID)
		return pItemContainer->ConIndex2BagIndex(iFinalIndex);

	return iFinalIndex;

}




ItemContainer*	HumanItemLogic::GetContainer(Obj_Human* pHuman,UINT uBagIndex)
{
	__ENTER_FUNCTION

	Assert(uBagIndex<MAX_BAG_SIZE);
	Assert(pHuman);

	//if(uBagIndex>=TASK_CONTAINER_OFFSET)
	//	return pHuman->GetTaskContain();
	//if(uBagIndex>=MAT_CONTAINER_OFFSET)
	//	return pHuman->GetMatContain();
	if( uBagIndex >= BASE_CONTAINER_OFFSET && uBagIndex < MAX_BAG_SIZE )
		return pHuman->GetBaseContain();
	else
		AssertEx(FALSE,"HumanItemLogic::GetItem()	uBagIndex 编号出错");

	__LEAVE_FUNCTION

	return NULL;
}

INT	HumanItemLogic::CalcBagItemCount(Obj_Human* pHuman,UINT uItemIndex)
{
	__ENTER_FUNCTION

	Assert(uItemIndex >0);
	Assert(pHuman);

	INT count = 0;
	Item*	pItem;

	ItemContainer*	pContainer = GetItemContain(pHuman,uItemIndex);

	if(!pContainer)
	{
		return 0;
	}

	for(int i=0; i<pContainer->GetContainerSize(); ++i)
	{
		pItem = pContainer->GetItem(i);
		
		if(!pItem)
		{
			Assert(pItem);
			continue;
		}
		if( pItem->GetItemTableIndex()	== uItemIndex )
		{
			count += pItem->GetLayedNum();
		}
	}

	return count;

	__LEAVE_FUNCTION

	return 0 ;
}

BOOL	HumanItemLogic::SetBagItemDur(Obj_Human* pHuman,UINT uBagIndex, INT iDur)
{
	__ENTER_FUNCTION

	
		Assert(uBagIndex<MAX_BAG_SIZE);
		Assert(pHuman);
	
		ItemContainer* pContainer = GetContainer(pHuman,uBagIndex);
		if(!pContainer)
		{
			return FALSE;
		}
		
		
		return g_ItemOperator.SetItemDur(pContainer,
										 pContainer->BagIndex2ConIndex(uBagIndex),
										 iDur);
		
	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	HumanItemLogic::SetBagItemIdent(Obj_Human* pHuman,UINT uBagIndex)
{
	__ENTER_FUNCTION


	Assert(uBagIndex<MAX_BAG_SIZE);
	Assert(pHuman);

	ItemContainer* pContainer = GetContainer(pHuman,uBagIndex);
	if(!pContainer)
	{
		return FALSE;
	}


	BOOL bRet = g_ItemOperator.SetItemIdent(pContainer,
									  pContainer->BagIndex2ConIndex(uBagIndex));
	if(bRet)
	{
					Item* pItem = pContainer->GetItem(pContainer->BagIndex2ConIndex(uBagIndex));
					Assert(pItem);
					GCItemInfo Msg;
					pItem->SaveValueTo(Msg.getItem());
					Msg.setID(uBagIndex);
					Msg.setIsNull(FALSE);
					pHuman->GetPlayer()->SendPacket(&Msg);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	HumanItemLogic::SetBagItemParam(Obj_Human* pHuman,UINT uBagIndex,UINT Start,INT Type,INT value)
{
	__ENTER_FUNCTION


	Assert(uBagIndex<MAX_BAG_SIZE);
	Assert(pHuman);

	ItemContainer* pContainer = GetContainer(pHuman,uBagIndex);
	if(!pContainer)
	{
		return FALSE;
	}


	BOOL bRet = g_ItemOperator.SetItemParam(pContainer,
						pContainer->BagIndex2ConIndex(uBagIndex),Start,Type,value);
	

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	HumanItemLogic::SetBagItemMaxDur(Obj_Human* pHuman,UINT uBagIndex, INT iDur)
{
	__ENTER_FUNCTION


	Assert(uBagIndex<MAX_BAG_SIZE);
	Assert(pHuman);

	ItemContainer* pContainer = GetContainer(pHuman,uBagIndex);
	if(!pContainer)
	{
		return FALSE;
	}


	return g_ItemOperator.SetItemMaxDur(pContainer,
						  pContainer->BagIndex2ConIndex(uBagIndex),
						  iDur);

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	HumanItemLogic::CreateMultiItemToBag(ITEM_LOG_PARAM* pLogParam, Obj_Human* pHuman,UINT uItemIndex,UINT uCount, BYTE nCoinType)
{
	__ENTER_FUNCTION
		
		Assert(uItemIndex!=0);
		Assert(uItemIndex!=INVALID_ID);
		Assert(uCount>0);
		Assert(pHuman);
		Assert(pLogParam);

		INT iNewCount;

		if(CalcItemSpace(pHuman,uItemIndex,uCount,iNewCount))
		{
			UINT iBagIndex,iOldBagIndex;
			UINT uLastCount = uCount;
			UINT uEachPileCount = 0;
			for(UINT iCount =0;iCount<uCount;iCount++)
			{
			
				CreateItemToBag(pLogParam,pHuman,uItemIndex,iBagIndex,nCoinType);
				
				if(iCount ==0) 
				{
					iOldBagIndex = iBagIndex;
				}

				if(iBagIndex!=iOldBagIndex)
				{
					GCNotifyEquip	 Msg;
					Msg.SetBagIndex( iOldBagIndex );
					Item* pItem = GetItem(pHuman,iOldBagIndex);
					Assert(pItem);
					pItem->SaveValueTo(Msg.GetItem());
					pHuman->GetPlayer()->SendPacket(&Msg);
					iOldBagIndex =	iBagIndex;
					
					//最后一堆的个数
					uLastCount -= uEachPileCount;
					uEachPileCount = 1;
				}
				else
				{
					uEachPileCount++;
				}
			}
			
			if(iBagIndex!=INVALID_INDEX)
			{
				Item* pChangedItem = GetItem(pHuman, iBagIndex);

				Assert(pChangedItem);
				
				if(pChangedItem->GetLayedNum()==uLastCount)
				{
					GCNotifyEquip Msg;
					Msg.SetBagIndex( iBagIndex );
					Item* pItem = GetItem(pHuman,iBagIndex);
					Assert(pItem);
					pItem->SaveValueTo(Msg.GetItem());
					pHuman->GetPlayer()->SendPacket(&Msg);
				}
				else
				{
					GCItemInfo  Msg;
					Msg.setIsNull(FALSE);
					Msg.setID(iBagIndex);
					pChangedItem->SaveValueTo(Msg.getItem());
					pHuman->GetPlayer()->SendPacket(&Msg);

				}
				
			}

			return TRUE;

		}
		return FALSE;

	__LEAVE_FUNCTION

		return FALSE;
}


BOOL	HumanItemLogic::CreateItemToBag(ITEM_LOG_PARAM* pLogParam,Obj_Human* pHuman,UINT uItemIndex,UINT& uBagIndex,BYTE nCoinType, UINT uQuality )
{
	__ENTER_FUNCTION

		Assert(pHuman);
		Assert(uItemIndex!=0);
		Assert(uItemIndex!=INVALID_ID);
		Assert(pLogParam);
		
		if(uItemIndex==0||uItemIndex==INVALID_ID)
			return FALSE;

		ItemContainer*	pContainer = GetItemContain(pHuman,uItemIndex);
		
		Assert(pContainer);

		if(!pContainer)
		{
			return FALSE;
		}

		BOOL bNewPos;
		UINT uContainerPos;

		BOOL	bRet =  g_ItemOperator.CreateItem(pLogParam,uItemIndex,uQuality,pContainer,bNewPos,uContainerPos,nCoinType);
		
		if(bRet)
		{
			pHuman->OnItemChanged( uItemIndex ); //通知脚本
			uBagIndex = pContainer->ConIndex2BagIndex(uContainerPos);
			//绑定处理
			Item* pItem = pContainer->GetItem(uContainerPos);
			Assert(pItem);
			if(pItem->IsRuler(IRL_PICKBIND))
			{
				g_ItemOperator.SetItemBind(pContainer,uContainerPos);
			}

		}
		else
		{
			uBagIndex = INVALID_INDEX;
		}

		return bRet;
		
	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	HumanItemLogic::MoveItem(ITEM_LOG_PARAM* pLogParam,Obj_Human* pHuman,ItemContainer* pContainer,UINT uIndex,UINT& uBagPos)
{
	__ENTER_FUNCTION
		
		Assert(pLogParam);
		Assert(pHuman);
		Assert(pContainer);
		Assert(uIndex<(UINT)pContainer->GetContainerSize());
		
		//得到源物品数据
		Item*	pSourItem = pContainer->GetItem(uIndex);
		Assert(pSourItem);
		
			
		if(!pSourItem)
		{
			return FALSE;
		}

		pLogParam->ItemGuid = pSourItem->GetGUID();
		pLogParam->Count	= pSourItem->GetLayedNum();
		uBagPos = INVALID_INDEX;

		UINT uItemIndex = pSourItem->GetItemTableIndex();
		
		pLogParam->ItemType	=	uItemIndex;
		//判断放置的背包位置
		ItemContainer*	pHumanContainer = 
			GetItemContain(pHuman,pSourItem->GetItemTableIndex());

		if(!pHumanContainer)
		{
			AssertEx(FALSE,"没有找到对应背包");
			return FALSE;
		}
		
		//获得目的包位置
		UINT uContainIndex = g_ItemOperator.MoveItem(pContainer,uIndex,pHumanContainer);
		if(uContainIndex<(UINT)pHumanContainer->GetContainerSize())
		{
			//转换成BagIndex
			uBagPos = pHumanContainer->ConIndex2BagIndex(uContainIndex);
			
			
			pHuman->OnItemChanged( uItemIndex); //通知脚本
			return TRUE; //成功
		}

		return FALSE;
			
		

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	HumanItemLogic::SplitItem(Obj_Human* pHuman,
								  ItemContainer* pSourContainer, 
								  UCHAR uSourIndex, 
								  UCHAR uCount, 
								  ItemContainer* pDestContainer, 
								  INT nDestIndex )
{
	__ENTER_FUNCTION

	Assert(pSourContainer);
	Assert(pDestContainer);
	Assert(uSourIndex<pSourContainer->GetContainerSize());


	Item* pSourceItem = pSourContainer->GetItem(uSourIndex);

	if(!pSourceItem)
	{
		Assert(FALSE);
		return FALSE;
	}

	if(pSourceItem->IsEmpty())
	{
		return FALSE;
	}
	
	ITEM_LOG_PARAM	ItemLogParam;
	ItemLogParam.OpType		= ITEM_CREATE_SPLIT;
	ItemLogParam.SceneID	= pHuman->getScene()->SceneID();
	ItemLogParam.XPos		= pHuman->getWorldPos()->m_fX;
	ItemLogParam.ZPos		= pHuman->getWorldPos()->m_fZ;

	if(g_ItemOperator.SplitItem(&ItemLogParam,pSourContainer,uSourIndex,uCount,pDestContainer,nDestIndex)
		==ITEMOE_SUCCESS)
	{
		SaveItemLog(&ItemLogParam);
		return TRUE;
	}

	
	
	__LEAVE_FUNCTION

		return FALSE;
}

	//在容器之间合并物品
BOOL	HumanItemLogic::SpliceItem(Obj_Human* pHuman,
									   ItemContainer* pSourContainer, 
									   UCHAR uSourIndex,
									   ItemContainer* pDestContainer,
									   UCHAR uDestIndex)
{
	__ENTER_FUNCTION
	Assert(pSourContainer);
	Assert(pDestContainer);
	Assert(uSourIndex<pSourContainer->GetContainerSize());

	Item* pSourceItem	= pSourContainer->GetItem(uSourIndex);
	Item* pSestItem		= pDestContainer->GetItem(uDestIndex);

	if(!pSourceItem || !pSestItem)
	{
		Assert(FALSE);
		return FALSE;
	}

	if(pSourceItem->IsEmpty() || pSestItem->IsEmpty())
	{
		return FALSE;
	}

	if(g_ItemOperator.SpliceItem(pSourContainer,uSourIndex,pDestContainer,uDestIndex)
		==ITEMOE_SUCCESS)
	{
		return TRUE;
	}

	__LEAVE_FUNCTION
		return FALSE;
}


Item*	HumanItemLogic::GetEquip(Obj_Human* pHuman,HUMAN_EQUIP EquipPoint)
{
	__ENTER_FUNCTION

	Assert(EquipPoint<HEQUIP_NUMBER);
	Assert(EquipPoint>=0);
	if(EquipPoint<0||EquipPoint>HEQUIP_NUMBER)
	{
		AssertEx(FALSE,"HumanItemLogic::SetBagItemDur() EquipPoint 不在合法范围 ");
		return NULL;
	}

	ItemContainer*	pEquipContainer = pHuman->GetEquipContain();

	Assert(pEquipContainer);

	if(!pEquipContainer)
	{
		return NULL;
	}
		
	return pEquipContainer->GetItem((UINT)EquipPoint);
	

	__LEAVE_FUNCTION

	return NULL;
}

INT		HumanItemLogic::CalcEquipItemCount(Obj_Human* pHuman,UINT uItemIndex)
{
	__ENTER_FUNCTION

	Assert(uItemIndex!=0);
	Assert(uItemIndex!=INVALID_ID);
	
	ItemContainer*	pEquipContainer = pHuman->GetEquipContain();

	Assert(pEquipContainer);

	if(!pEquipContainer)
	{
		return 0;
	}
	
	INT nCount = 0;

	for(int i =0;i<HEQUIP_NUMBER;i++)
	{
		Item* pEquipItem =  pEquipContainer->GetItem(i);

		if(!pEquipItem)
			continue;
		
		if(pEquipItem->GetItemTableIndex()==uItemIndex)
		{
			if(pEquipItem->GetLayedNum()>0)
			{
				nCount+=pEquipItem->GetLayedNum();
			}
		}

	}

	return nCount;
	
	__LEAVE_FUNCTION

	return 0;
}



BOOL	HumanItemLogic::SetEquipDur(Obj_Human* pHuman,HUMAN_EQUIP EquipPoint, INT iDur)
{
	__ENTER_FUNCTION

		Assert(EquipPoint<HEQUIP_NUMBER);
		Assert(EquipPoint>=0);
		if(EquipPoint<0||EquipPoint>HEQUIP_NUMBER)
		{
			AssertEx(FALSE,"HumanItemLogic::SetBagItemDur() EquipPoint 不在合法范围 ");
			return FALSE;
		}
		
		return g_ItemOperator.SetItemDur(pHuman->GetEquipContain(),
										 EquipPoint,
										 iDur);

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	HumanItemLogic::SetEquipMaxDur(Obj_Human* pHuman,HUMAN_EQUIP EquipPoint, INT iDur)
{
	__ENTER_FUNCTION

	Assert(EquipPoint<HEQUIP_NUMBER);
	Assert(EquipPoint>=0);
	if(EquipPoint<0||EquipPoint>HEQUIP_NUMBER)
	{
		AssertEx(FALSE,"HumanItemLogic::SetBagItemDur() EquipPoint 不在合法范围 ");
		return FALSE;
	}

	return g_ItemOperator.SetItemMaxDur(pHuman->GetEquipContain(),
									    EquipPoint,
										iDur);

	__LEAVE_FUNCTION

	return FALSE;
}

Item*	HumanItemLogic::GetBankItem(Obj_Human* pHuman,UINT BankIndex)
{
	__ENTER_FUNCTION

	Assert(BankIndex<MAX_BANK_SIZE);
	Assert(BankIndex>=0);
	if(BankIndex<0||BankIndex>MAX_BANK_SIZE)
	{
		AssertEx(FALSE,"HumanItemLogic::GetBankItem() BankIndex 不在合法范围 ");
		return NULL;
	}

	ItemContainer*	pBankContainer = pHuman->GetBankContain();

	Assert(pBankContainer);

	if(!pBankContainer)
	{
		return NULL;
	}

	return pBankContainer->GetItem((UINT)BankIndex);

	__LEAVE_FUNCTION

	return NULL;


}

BOOL	HumanItemLogic::RecieveItemToBag(Obj_Human* pHuman, ItemContainer* pContainer, UINT uIndex )
{
	__ENTER_FUNCTION

	Assert(pHuman);

	Item*	pItemInSource = pContainer->GetItem(uIndex);
	if(pItemInSource->IsEmpty())
	{
		return FALSE;
	}
	UINT uItemIndex = pItemInSource->GetItemTableIndex();
	UINT uCount = pItemInSource->GetLayedNum();

	ItemContainer*		pMyContainer = HumanItemLogic::GetItemContain(pHuman, uItemIndex);

	BOOL	bFlag = FALSE;
	g_ItemOperator.UnlockItem( pContainer, uIndex );

	//自动找格,支持自动叠加
	INT result = 
		g_ItemOperator.MoveItem
		(
		pContainer,
		uIndex,
		bFlag,
		pMyContainer
		);

	if(result<0)
	{//拷贝失败
		return FALSE;
	}
	Item*	pItemDest = pMyContainer->GetItem(result);
	UINT	ToIndex = pMyContainer->ConIndex2BagIndex(result);
	if(pItemDest->IsRuler(IRL_PICKBIND))
	{
		g_ItemOperator.SetItemBind(pMyContainer,result);
	}

	if(pItemDest->GetLayedNum() == uCount)
	{
		GCNotifyEquip Msg;
		Msg.SetBagIndex( ToIndex );
		pItemDest->SaveValueTo(Msg.GetItem());
		pHuman->GetPlayer()->SendPacket(&Msg);
	}
	else
	{
		GCItemInfo  Msg;
		Msg.setIsNull(FALSE);
		Msg.setID(ToIndex);
		pItemDest->SaveValueTo(Msg.getItem());
		pHuman->GetPlayer()->SendPacket(&Msg);
	}

    
	return TRUE;
	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	HumanItemLogic::CanReceiveExchangeItemList(Obj_Human* pHuman,_EXCHANGE_ITEM_LIST& List)
{
	__ENTER_FUNCTION

	ItemContainer*  pBaseContain	= pHuman->GetBaseContain();
	ItemContainer*	pPetContain		= pHuman->GetPetContain();
	Assert(pBaseContain);
	Assert(pPetContain);

	INT iBaseSize = pBaseContain->CalcExchangeItemSpace();
	INT iPetSize =  pPetContain->CalcExchangeItemSpace();

	for( INT iListIndex = 0; iListIndex < List.m_ListCount; iListIndex++ )
	{
		Item* pItem = List.m_ListItem[iListIndex];
		if( pItem->GetDataType() == IDT_PET )
		{
			iPetSize -= 1;
			if( iPetSize < 0 )
				return FALSE;
		}
		else if( pItem->GetDataType() == IDT_ITEM )
		{
			iBaseSize -= 1;
			if( iBaseSize < 0 )
				return FALSE;
		}
	}

/*	ItemContainer*  pBaseContain	= pHuman->GetBaseContain();
	ItemContainer*	pTaskContain	= pHuman->GetTaskContain();
	ItemContainer*	pMatContain		= pHuman->GetMatContain();
	ItemContainer*	pPetContain		= pHuman->GetPetContain();


	Assert(pBaseContain);
	Assert(pTaskContain);
	Assert(pMatContain);
	Assert(pPetContain);

	INT iBaseSize = pBaseContain->CalcExchangeItemSpace();
	INT iTaskSize = pTaskContain->CalcExchangeItemSpace();
	INT iMatSize =  pMatContain->CalcExchangeItemSpace();
	INT iPetSize =  pPetContain->CalcExchangeItemSpace();

	for(INT iListIndex =0;iListIndex<List.m_ListCount;iListIndex++ )
	{	
		Item* pItem = List.m_ListItem[iListIndex];
		if(pItem->GetDataType() == IDT_PET)
		{
			iPetSize-=1;
			if(iPetSize<0)
				return FALSE;
		}
		else if(pItem->GetDataType() == IDT_ITEM)
		{
			switch(pItem->GetItemClass()) 
			{
			case ICLASS_TASKITEM:
				{
					iTaskSize-=1;
					if(iTaskSize<0)
						return FALSE;
				}
				break;
			case ICLASS_EQUIP:
			case ICLASS_COMITEM:
			case ICLASS_STOREMAP:
				{
					iBaseSize-=1;
					if(iBaseSize<0)
						return FALSE;
				}
				break;
			case ICLASS_GEM:
			case ICLASS_MATERIAL:
				{
					iMatSize-= 1;
					if(iMatSize<0)
						return FALSE;
				}
				break;
			default:
				Assert(FALSE);
				return FALSE;
				break;
			}
		}

	}*/
	
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

