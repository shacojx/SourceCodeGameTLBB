#include "stdafx.h"


#include "ItemOperator.h"
#include "ItemManager.h"
#include "HumanDB.h"
#include "ItemRuler.h"
#include "Log.h"


ItemOperator	g_ItemOperator;


ItemOperator::ItemOperator( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

ItemOperator::~ItemOperator( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

__LEAVE_FUNCTION
}

BOOL ItemOperator::Init( _ITEMOPERATOR_INIT* pInit )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION

	return TRUE ;
}

VOID ItemOperator::CleanUp( )
{
__ENTER_FUNCTION


__LEAVE_FUNCTION
}

INT ItemOperator::CreateItem( ITEM_LOG_PARAM*	pLogParam,
							  Item* pTemplateItem ,
							  ItemContainer* pDestContainer,
							  INT nDestIndex )
{
__ENTER_FUNCTION

	if(		pTemplateItem==NULL 
		||  pDestContainer==NULL 
		||  (nDestIndex<INVALID_INDEX 
		||  nDestIndex>=pDestContainer->GetContainerSize())
	  )	 
	{
		return ITEMOE_UNKNOW ;
	}

	Item* pSourItem = pTemplateItem ;
	Item* pDestItem = NULL ;

	if( pSourItem->IsEmpty() )
	{
		return ITEMOE_SOUROPERATOR_EMPTY ;
	}
	else if( pSourItem->IsLock() )
	{
		return ITEMOE_SOUROPERATOR_LOCK ;
	}

	INT nFinalDestIndex = nDestIndex ;
	if( nFinalDestIndex == INVALID_INDEX )
	{//随机找个地方生成物品
		
		if(pSourItem->m_Type != IDT_PET)
		{
			INT nSourLayCount = pSourItem->GetLayedNum();
			INT nRepIndex = pDestContainer->GetIndexByType( pSourItem->GetItemTableIndex(),pSourItem->GetLayedNum() ) ;
			if( nRepIndex!=INVALID_INDEX )
			{//已经有同类物品了
				pDestItem = pDestContainer->GetItem( nRepIndex ) ;
				if( !pDestItem->IsLock() && pDestItem->IsCanLay() )
				{//物品能够重叠放置
					INT nDestLayCount = pDestItem->GetLayedNum() ;
					INT nDestMaxLayCount = pDestItem->GetMaxLayedNum() ;

					if( nSourLayCount+nDestLayCount<=nDestMaxLayCount )
					{
						pLogParam->ItemType		=	pDestItem->GetItemTableIndex();
						pLogParam->BagPos		=	pDestContainer->ConIndex2BagIndex(nRepIndex);
						pLogParam->SecOpType	=	ITEM_CREATE_TILED;
						pLogParam->ItemGuid		=	pDestContainer->GetItem(nRepIndex)->GetGUID();
						pLogParam->Count		=	nSourLayCount+nDestLayCount;
						SetItemLayCount( pDestContainer, nRepIndex, nSourLayCount+nDestLayCount ) ;
						return nRepIndex ;
					}
				}
			}
		}
		

		nFinalDestIndex = pDestContainer->GetEmptyItemIndex() ;
		if( nFinalDestIndex == INVALID_INDEX )
		{
			return ITEMOE_DESTOPERATOR_FULL ;
		}
	}
	else 
	{
		pDestItem = pDestContainer->GetItem( nDestIndex ) ;
		Assert( pDestItem ) ;

		if( !pDestItem->IsEmpty() )
		{//目标位置有物品
			return ITEMOE_DESTOPERATOR_HASITEM ;
		}
	}
	
	switch(pSourItem->m_Type )
	{
	case IDT_ITEM:
		{
			//新create出来的item需要新的guid
			_ITEM_GUID NewGUID;
			if(!GenItemGUID(NewGUID))
			{
				return ITEMOE_CREATEGUID_FAIL;
			}

			pDestContainer->SetItem( nFinalDestIndex, pSourItem ) ;
			SetItemGUID( pDestContainer, nFinalDestIndex, &NewGUID ) ;
			pDestItem = pDestContainer->GetItem(nFinalDestIndex);
			pLogParam->ItemType		=	pDestItem->GetItemTableIndex();
			pLogParam->BagPos		=	pDestContainer->ConIndex2BagIndex(nFinalDestIndex);
			pLogParam->ItemGuid		=	pDestItem->GetGUID();
			pLogParam->Count		=	pDestItem->GetLayedNum();
			
		}
		break;
	case IDT_PET:
		{
			PET_GUID_t	NewGUID;
			if(!GenItemGUID(NewGUID))
			{
				return ITEMOE_CREATEGUID_FAIL;
			}
			pDestContainer->SetItem( nFinalDestIndex, pSourItem ) ;
			SetItemGUID( pDestContainer, nFinalDestIndex, &NewGUID ) ;

		}
		break;
	default:
		{
			return ITEMOE_CREATEGUID_FAIL;
			Assert(FALSE);
		}
	}

	return ITEMOE_SUCCESS;


__LEAVE_FUNCTION

	return ITEMOE_UNKNOW ;
}


BOOL	ItemOperator::CreateItem(ITEM_LOG_PARAM* pLogParam,
								UINT uItemIndex,
								UINT uQuality,
								ItemContainer* pDestContainer,
								BOOL&	bNewEmptyPos,
								UINT&	uConIndex,
								BYTE	nCoinType,
								INT nDestIndex 
								)
{

	__ENTER_FUNCTION

	Assert(pDestContainer);
	Assert(uItemIndex!=0);
	Assert(uItemIndex!=INVALID_ID);
	Assert(pLogParam);

	if(uItemIndex==0||uItemIndex==INVALID_ID)
		return FALSE;
	
	uConIndex		=	INVALID_INDEX;
	bNewEmptyPos	=	FALSE;
	

	//如果可叠加物品
	if(ItemRulerCheck::CheckTypeRuler(IRL_TILE,uItemIndex))
	{
		//只加一个可以用这个函数
		INT iNoFullBagIndex =	pDestContainer->GetNoFullItemIndex(uItemIndex);

		if(iNoFullBagIndex != INVALID_INDEX)
		{
			uConIndex				=	iNoFullBagIndex;
			bNewEmptyPos			=	FALSE;
			pLogParam->ItemType		=	uItemIndex;
			pLogParam->BagPos		=	pDestContainer->ConIndex2BagIndex(iNoFullBagIndex);
			pLogParam->SecOpType	=	ITEM_CREATE_TILED;
			pLogParam->ItemGuid		=	pDestContainer->GetItem(iNoFullBagIndex)->GetGUID();
			pLogParam->Count		=	pDestContainer->GetItem(iNoFullBagIndex)->GetLayedNum()+1;
			return IncItemLayCount(pDestContainer,iNoFullBagIndex);
		}
	}

	//否则一定要new物品
	INT iEmptyBagIndex	= pDestContainer->GetEmptyItemIndex();

	if(iEmptyBagIndex != INVALID_INDEX)
	{

		_ITEM	createItem;
		
		g_pItemManager->CreateItem(pLogParam,uItemIndex,createItem,uQuality);
		
		if(createItem.IsNullType())
		{
			return FALSE;
		}
		
		pLogParam->ItemType = uItemIndex;
		pLogParam->BagPos   = pDestContainer->ConIndex2BagIndex(iEmptyBagIndex);
		pLogParam->ItemGuid	= createItem.m_ItemGUID;
		
		Item*	pDestItem =	pDestContainer->GetItem(iEmptyBagIndex);
		pDestItem->SetCoinType( nCoinType );
		if(pDestItem->IsEmpty())
		{
			pDestContainer->SetItemValue(iEmptyBagIndex,&createItem);
			bNewEmptyPos	= TRUE;	
			uConIndex		= iEmptyBagIndex;
			return TRUE;
		}
		else
		{
			Assert(FALSE);//不能出现的情况
		}
	}
	return FALSE;

	__LEAVE_FUNCTION

	return FALSE;
}





INT ItemOperator::CopyItem( ItemContainer* pSourContainer,
							INT			   nSourIndex ,
							ItemContainer* pDestContainer,
							INT nDestIndex )
{
__ENTER_FUNCTION

	if( pSourContainer==NULL 
		|| (nSourIndex<0 || nSourIndex>=pSourContainer->GetContainerSize())
		|| pDestContainer==NULL 
		|| (nDestIndex<INVALID_INDEX || nDestIndex>=pDestContainer->GetContainerSize())
		)
	{
		return ITEMOE_UNKNOW ;
	}

	Item* pSourItem = NULL ;
	Item* pDestItem = NULL ;

	//验证
	pSourItem = pSourContainer->GetItem( nSourIndex ) ;
	if( pSourItem==NULL )
	{
		Assert(FALSE) ;
		return ITEMOE_UNKNOW ;
	}
	if( pSourItem->IsEmpty() )
	{
		return ITEMOE_SOUROPERATOR_EMPTY ;
	}
	else if( pSourItem->IsLock() )
	{
		return ITEMOE_SOUROPERATOR_LOCK ;
	}

	INT nFinalDestIndex = nDestIndex ;
	
	if( nFinalDestIndex == INVALID_INDEX )
	{//自动寻找空格，不自动填充到可叠加的格子里了？
		nFinalDestIndex = pDestContainer->GetEmptyItemIndex() ;
		if( nFinalDestIndex == INVALID_INDEX )
		{
			return ITEMOE_DESTOPERATOR_FULL ;
		}
	}
	else
	{//指定格子
		pDestItem = pDestContainer->GetItem( nDestIndex ) ;
		if( pDestItem==NULL )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}
		if( !pDestItem->IsEmpty() )
		{//目标位置有物品
			return ITEMOE_DESTOPERATOR_HASITEM ;
		}
		if( pDestItem->IsLock() )
		{//目标位置已被锁
			return ITEMOE_DESTOPERATOR_LOCK;
		}
	}

	//拷贝过去，自动解锁
	pDestContainer->SetItem( nFinalDestIndex, pSourItem ) ;

	return nFinalDestIndex ;

__LEAVE_FUNCTION

	return ITEMOE_UNKNOW ;
}



INT	ItemOperator::CopyItem(ItemBoxContaner* pSourceContainer,
						   INT				nSourceIndex,
						   BOOL&		    bNewPos,
						   ItemContainer*   pDestContainer,
						   INT nDestIndex/* =INVALID_INDEX */)
{
	__ENTER_FUNCTION

		if( pSourceContainer==NULL 
			|| (nSourceIndex<0 
			|| nSourceIndex>=MAX_BOXITEM_NUMBER)
			|| pDestContainer==NULL 
			|| (nDestIndex<INVALID_INDEX 
			|| nDestIndex>=pDestContainer->GetContainerSize())
		  )
		{return ITEMOE_UNKNOW ;	}

	   _ITEM* pSourItem = &(pSourceContainer->m_Item[nSourceIndex]);

		if( pSourItem==NULL )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}
		
		Item* pDestItem = NULL ;

		INT nFinalDestIndex = nDestIndex ;
		if( nFinalDestIndex == INVALID_INDEX )
		{
			INT nRepIndex = pDestContainer->GetIndexByType( pSourItem->m_ItemIndex, pSourItem->GetItemCount() ) ;
			
			if( nRepIndex!=INVALID_INDEX ) 
			{//同类物品处理
				
				pDestItem = pDestContainer->GetItem( nRepIndex ) ;
				Assert(pDestItem);

				if(!pDestItem)
				{
					return ITEMOE_UNKNOW;
				}
				if( !pDestItem->IsEmpty() && !pDestItem->IsLock() && pDestItem->IsCanLay() )
				{//物品能够重叠放置
					INT nDestLayCount = pDestItem->GetLayedNum() ;
					INT nDestMaxLayCount = pDestItem->GetMaxLayedNum() ;
					INT nSourLayCount = pSourItem->GetItemCount();
					if( nSourLayCount+nDestLayCount<nDestMaxLayCount )
					{
						SetItemLayCount( pDestContainer, nRepIndex, nSourLayCount+nDestLayCount ) ;
						return nRepIndex ;
					}
				}
			}
			nFinalDestIndex = pDestContainer->GetEmptyItemIndex() ;
			if( nFinalDestIndex == INVALID_INDEX )
			{
				return ITEMOE_DESTOPERATOR_FULL ;
			}
			
			pDestItem	=	pDestContainer->GetItem(nFinalDestIndex);
			
		}
		else 
		{
			pDestItem = pDestContainer->GetItem( nDestIndex ) ;
			if( pDestItem==NULL )
			{
				Assert(FALSE) ;
				return ITEMOE_UNKNOW ;
			}

			if( !pDestItem->IsEmpty() )
			{//目标位置有物品
				return ITEMOE_DESTOPERATOR_HASITEM ;
			}
			
		}
		
		pDestItem->SetItemValue(pSourItem);
		bNewPos = TRUE;

		return	nFinalDestIndex;

	__LEAVE_FUNCTION

		return ITEMOE_UNKNOW ;

}

INT ItemOperator::MoveItem( ItemContainer* pSourContainer,
							INT nSourIndex ,
							ItemContainer* pDestContainer,
							INT nDestIndex )
{
__ENTER_FUNCTION

	if  (	pSourContainer==NULL 
		|| (nSourIndex<0 || nSourIndex>=pSourContainer->GetContainerSize())
		||  pDestContainer==NULL 
		|| (nDestIndex<INVALID_INDEX || nDestIndex>=pDestContainer->GetContainerSize())
		)
	{
		return ITEMOE_UNKNOW ;
	}

	Item* pSourItem = pSourContainer->GetItem(nSourIndex) ;
	Item* pDestItem = NULL ;

	if( pSourItem==NULL )
	{
		Assert(FALSE) ;
		return ITEMOE_UNKNOW ;
	}
	else if( pSourItem->IsEmpty() )
	{
		return ITEMOE_SOUROPERATOR_EMPTY ;
	}
	else if( pSourItem->IsLock() )
	{
		return ITEMOE_SOUROPERATOR_LOCK ;
	}

	INT nFinalDestIndex = nDestIndex ;
	if( nFinalDestIndex == INVALID_INDEX )
	{//随机找个地方生成物品
		if(pSourItem->m_Type != IDT_PET)
		{
			INT nSourLayCount = pSourItem->GetLayedNum();
			INT nRepIndex = pDestContainer->GetIndexByType( pSourItem->GetItemTableIndex(),pSourItem->GetLayedNum() ) ;
			if( nRepIndex!=INVALID_INDEX )
			{//已经有同类物品了
				pDestItem = pDestContainer->GetItem( nRepIndex );
				if( !pDestItem->IsLock() && pDestItem->IsCanLay() )
				{//物品能够重叠放置
					INT nDestLayCount = pDestItem->GetLayedNum() ;
					INT nDestMaxLayCount = pDestItem->GetMaxLayedNum() ;
					INT nSourLayCount = pSourItem->GetLayedNum() ;
					if( nSourLayCount+nDestLayCount<=nDestMaxLayCount )
					{
						SetItemLayCount( pDestContainer, nRepIndex, nSourLayCount+nDestLayCount ) ;
						pSourContainer->EraseItem(nSourIndex);//复制！！！
						return nRepIndex ;
					}
				}
			}
		}

		nFinalDestIndex = pDestContainer->GetEmptyItemIndex() ;
		if( nFinalDestIndex == INVALID_INDEX )
		{
			return ITEMOE_DESTOPERATOR_FULL ;
		}
	}
	else 
	{
		pDestItem = pDestContainer->GetItem( nDestIndex ) ;
		if( pDestItem==NULL )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}

		if( !pDestItem->IsEmpty() )
		{//目标位置有物品
			return ITEMOE_DESTOPERATOR_HASITEM ;
		}
		Assert( !pDestItem->IsLock() ) ;
	}

	pDestContainer->SetItem( nFinalDestIndex, pSourItem ) ;
	pSourContainer->EraseItem(nSourIndex);
	

	return nFinalDestIndex;

__LEAVE_FUNCTION

	return ITEMOE_UNKNOW ;
}





INT	ItemOperator::MoveItem( ItemContainer* pSourContainer,
							 INT			 nSourIndex ,
							 BOOL&			 bNewPos,
							 ItemContainer* pDestContainer,
							 INT			 nDestIndex )
{
	__ENTER_FUNCTION

		if  (	pSourContainer==NULL 
			|| (nSourIndex<0 || nSourIndex>=pSourContainer->GetContainerSize())
			||  pDestContainer==NULL 
			|| (nDestIndex<INVALID_INDEX || nDestIndex>=pDestContainer->GetContainerSize())
			)
		{
			return ITEMOE_UNKNOW ;
		}

		Item* pSourItem = pSourContainer->GetItem(nSourIndex) ;
		Item* pDestItem = NULL ;
		bNewPos = FALSE;

		if( pSourItem==NULL )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}
		else if( pSourItem->IsEmpty() )
		{
			return ITEMOE_SOUROPERATOR_EMPTY ;
		}
		else if( pSourItem->IsLock() )
		{
			return ITEMOE_SOUROPERATOR_LOCK ;
		}


		INT nFinalDestIndex = nDestIndex ;
		if( nFinalDestIndex == INVALID_INDEX )
		{//随机找个地方生成物品
			INT nRepIndex = pDestContainer->GetIndexByType( pSourItem->GetItemTableIndex(),pSourItem->GetLayedNum() ) ;
			if( nRepIndex!=INVALID_INDEX )
			{//已经有同类物品了
				pDestItem = pDestContainer->GetItem( nRepIndex );
				if( !pDestItem->IsLock() && pDestItem->IsCanLay() )
				{//物品能够重叠放置
					INT nDestLayCount = pDestItem->GetLayedNum() ;
					INT nDestMaxLayCount = pDestItem->GetMaxLayedNum() ;
					INT nSourLayCount = pSourItem->GetLayedNum() ;
					if( nSourLayCount+nDestLayCount<=nDestMaxLayCount )
					{
						SetItemLayCount( pDestContainer, nRepIndex, nSourLayCount+nDestLayCount ) ;
						pSourContainer->EraseItem(nSourIndex);//复制！！！
						return nRepIndex ;
					}
				}
			}
			nFinalDestIndex = pDestContainer->GetEmptyItemIndex() ;
			if( nFinalDestIndex == INVALID_INDEX )
			{
				return ITEMOE_DESTOPERATOR_FULL ;
			}
			bNewPos = TRUE;
		}
		else 
		{
			pDestItem = pDestContainer->GetItem( nDestIndex ) ;
			if( pDestItem==NULL )
			{
				Assert(FALSE) ;
				return ITEMOE_UNKNOW ;
			}
			else if( !pDestItem->IsEmpty() )
			{//目标位置有物品
				return ITEMOE_DESTOPERATOR_HASITEM ;
			}
			Assert( !pDestItem->IsLock() ) ;
		}

		pDestContainer->SetItem( nFinalDestIndex, pSourItem ) ;
		pSourContainer->EraseItem( nSourIndex );

		return nFinalDestIndex;

		__LEAVE_FUNCTION

		return ITEMOE_UNKNOW ;

}


INT ItemOperator::MoveItem( ItemContainer* pSourContainer,
						   INT nSourIndex,
						   INT nDestIndex)
{
	__ENTER_FUNCTION

		if  (	pSourContainer==NULL 
			|| (nSourIndex<0 || nSourIndex>=pSourContainer->GetContainerSize())
			|| (nDestIndex<INVALID_INDEX ))
		{
			return ITEMOE_UNKNOW ;
		}

		Item* pSourItem = pSourContainer->GetItem(nSourIndex) ;
		Item* pDestItem = NULL ;

		if( pSourItem==NULL )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}

		INT nFinalDestIndex = nDestIndex ;
	
		pDestItem = pSourContainer->GetItem( nDestIndex ) ;
		if( pDestItem==NULL )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}

		Assert( !pDestItem->IsLock() ) ;
		
		if( pSourItem->IsEmpty() )
		{
			return ITEMOE_SOUROPERATOR_EMPTY ;
		}
		else if( pSourItem->IsLock() )
		{
			return ITEMOE_SOUROPERATOR_LOCK ;
		}


		if( !pDestItem->IsEmpty() )
		{//目标位置有物品
			
			Assert( pSourItem->GetItemTableIndex() == pDestItem->GetItemTableIndex() );
			INT nDestLayCount = pDestItem->GetLayedNum() ;
			INT nDestMaxLayCount = pDestItem->GetMaxLayedNum() ;
			INT nSourLayCount = pSourItem->GetLayedNum() ;
			if( nSourLayCount+nDestLayCount <= nDestMaxLayCount )
			{
				SetItemLayCount( pSourContainer, nFinalDestIndex, nSourLayCount+nDestLayCount ) ;
				BOOL bRet = pSourContainer->EraseItem(nSourIndex);
				Assert(bRet);
				return nFinalDestIndex ;
			}
			else
			{
				SetItemLayCount(pSourContainer,nFinalDestIndex,nDestMaxLayCount);
				SetItemLayCount(pSourContainer,nSourIndex,nSourLayCount+nDestLayCount-nDestMaxLayCount);
				return nFinalDestIndex ;
			}
		}

		return ITEMOE_DESTOPERATOR_EMPTY;

		__LEAVE_FUNCTION

			return ITEMOE_UNKNOW ;
}





INT ItemOperator::ExchangeItem( ItemContainer* pSourContainer,
								INT nSourIndex ,
								ItemContainer* pDestContainer,
								INT nDestIndex )
{
__ENTER_FUNCTION

	if(		pSourContainer==NULL 
		|| (nSourIndex<0 || nSourIndex>=pSourContainer->GetContainerSize())
		||  pDestContainer==NULL 
		|| (nDestIndex<INVALID_INDEX || nDestIndex>=pDestContainer->GetContainerSize())
		)
	{
		return ITEMOE_UNKNOW ;
	}

	Item* pSourItem = NULL ;
	Item* pDestItem = NULL ;

	pDestItem = pDestContainer->GetItem( nDestIndex ) ;
	if( pDestItem==NULL )
	{
		Assert(FALSE) ;
		return ITEMOE_DESTOPERATOR_EMPTY ;
	}
	else if( pDestItem->IsEmpty() )
	{
		return ITEMOE_DESTOPERATOR_EMPTY ;
	}
	else if( pDestItem->IsLock() )
	{
		return ITEMOE_DESTOPERATOR_LOCK ;
	}

	pSourItem = pSourContainer->GetItem( nSourIndex ) ;
	if( pSourItem==NULL )
	{
		Assert(FALSE) ;
		return ITEMOE_SOUROPERATOR_EMPTY ;
	}
	else if( pSourItem->IsEmpty() )
	{
		return ITEMOE_SOUROPERATOR_EMPTY ;
	}
	else if( pSourItem->IsLock() )
	{
		return ITEMOE_SOUROPERATOR_LOCK ;
	}
	
	if(pSourItem->m_Type != pDestItem->m_Type)
	{
		return ITEMOE_DIFF_ITEM_DATA;
	}
	
	switch(pSourItem->m_Type) 
	{
	case IDT_ITEM:
		{
			_ITEM		TmpITEM ;
			const _ITEM_INIT	Init(&TmpITEM) ;

			Item		tempItem;
			tempItem.Init(&Init);
			TmpITEM = *pSourItem->m_pItem ;

			pSourContainer->SetItem( nSourIndex, pDestItem) ;
			pDestContainer->SetItem( nDestIndex, &tempItem) ;
		}
		break;
	case IDT_PET:
		{
			_PET_DB_LOAD	TmpPet ;
			const _ITEM_INIT	Init(&TmpPet) ;

			Item		tempItem;
			tempItem.Init(&Init);
			TmpPet = *pSourItem->m_pPet ;
			pSourContainer->SetItem( nSourIndex, pDestItem) ;
			pDestContainer->SetItem( nDestIndex, &tempItem) ;
		}
		break;
	default:
		{
			Assert(FALSE);
			return ITEMOE_UNKNOW ;
		}
	}



	return ITEMOE_SUCCESS;
__LEAVE_FUNCTION

	return ITEMOE_UNKNOW ;
}

BOOL	ItemOperator::EraseItem(ItemContainer* pDestContainer, UCHAR uIndex)
{
	__ENTER_FUNCTION
		
		if(		pDestContainer==NULL 
			||  (uIndex<0 || uIndex>=pDestContainer->GetContainerSize())
		   )
		{
			return FALSE ;
		}

		Item* pDestItem = pDestContainer->GetItem(uIndex);
		
		if( pDestItem==NULL )
		{
			Assert(FALSE) ;
			return FALSE;
		}
		else if( pDestItem->IsEmpty() )
		{
			return FALSE ;
		}
		else if( pDestItem->IsLock() )
		{
			return FALSE;
		}
		
		return pDestContainer->EraseItem(uIndex);

	__LEAVE_FUNCTION

		return	FALSE;
}

BOOL	ItemOperator::SplitItem(ITEM_LOG_PARAM* pLogParam,
								ItemContainer* pSourContainer, 
								UCHAR uSourIndex,  
								UCHAR uCount,  
								ItemContainer* pDestContainer,
								INT nDestIndex /* =INVALID_INDEX  */)
{
	__ENTER_FUNCTION
		
		if  (	pSourContainer==NULL 
			|| (uSourIndex<0 || uSourIndex>=pSourContainer->GetContainerSize())
			|| pDestContainer ==NULL
			|| (nDestIndex<INVALID_INDEX ))
		{
			return ITEMOE_UNKNOW ;
		}

		Assert(pLogParam);
		Item* pSourItem = pSourContainer->GetItem(uSourIndex) ;

		if( pSourItem==NULL )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}
		
		if(pSourItem->GetLayedNum()<=uCount 
		  || uCount == 0)
		{
			return ITEMOE_UNKNOW ;	
		}

		if( pSourItem->IsEmpty() )
		{
			return ITEMOE_SOUROPERATOR_EMPTY ;
		}
		else if( pSourItem->IsLock() )
		{
			return ITEMOE_SOUROPERATOR_LOCK ;
		}
		
		INT nFinalDestIndex = nDestIndex ;
		if( nFinalDestIndex == INVALID_INDEX )
		{  //随机找个地方生成物品
			nFinalDestIndex = pDestContainer->GetEmptyItemIndex() ;
			if( nFinalDestIndex == INVALID_INDEX )
			{
				return ITEMOE_DESTOPERATOR_FULL ;
			}
		}
		
		if(CreateItem(pLogParam,pSourItem,pDestContainer,nFinalDestIndex)!=0)
		{
			return ITEMOE_UNKNOW;
		}

		SetItemLayCount(pDestContainer,nFinalDestIndex,uCount);
		SetItemLayCount(pSourContainer,uSourIndex,pSourItem->GetLayedNum()-uCount);

		return ITEMOE_SUCCESS;
		

	__LEAVE_FUNCTION

		return ITEMOE_UNKNOW;
}

INT	ItemOperator::SpliceItem(ItemContainer* pSourContainer,			//原容器
									UCHAR uSourIndex,				//原容器中位置
									ItemContainer* pDestContainer,	//目标容器
									INT nDestIndex)					//目标位置
{
	__ENTER_FUNCTION

		if  (	pSourContainer==NULL 
			|| (uSourIndex<0 || uSourIndex>=pSourContainer->GetContainerSize())
			|| pDestContainer ==NULL
			|| (nDestIndex<INVALID_INDEX ))
		{
			return ITEMOE_UNKNOW ;
		}

		Item* pSourItem = pSourContainer->GetItem(uSourIndex) ;
		if( NULL==pSourItem )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}

		Item* pDestItem = pDestContainer->GetItem(nDestIndex);
		if( NULL==pDestItem )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}
		if( pSourItem->GetItemIndex() != pDestItem->GetItemIndex() )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}
		if( pSourItem->IsEmpty() || pDestItem->IsEmpty() )
		{
			return ITEMOE_SOUROPERATOR_EMPTY ;
		}
		if( pSourItem->IsLock() || pDestItem->IsLock() )
		{
			return ITEMOE_SOUROPERATOR_LOCK ;
		}
		if( !pSourItem->IsCanLay() ||
			!pDestItem->IsCanLay() ||
			pSourItem->GetLayedNum()+pDestItem->GetLayedNum() > pDestItem->GetMaxLayedNum() )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}

		//到这里，已经完全验证了物品能够叠加
		
        //修改目标物品的内容
		INT nSourLayCount = pSourItem->GetLayedNum();
		INT nDestLayCount = pDestItem->GetLayedNum();
		SetItemLayCount( pDestContainer, nDestIndex, nSourLayCount+nDestLayCount );

		//删除原来的物品
		BOOL bRet	= pSourContainer->EraseItem(uSourIndex);
		Assert(bRet);

		return ITEMOE_SUCCESS;

	__LEAVE_FUNCTION
		return ITEMOE_UNKNOW;

}

INT ItemOperator::MoveSpliceItem(ItemContainer* pSourContainer,		//原容器
									UCHAR uSourIndex,				//原容器中位置
									ItemContainer* pDestContainer,	//目标容器
									INT nDestIndex)					//目标位置
{
	__ENTER_FUNCTION

		if  (	pSourContainer==NULL 
			|| (uSourIndex<0 || uSourIndex>=pSourContainer->GetContainerSize())
			|| pDestContainer ==NULL
			|| (nDestIndex<INVALID_INDEX ))
		{
			return ITEMOE_UNKNOW ;
		}

		Item* pSourItem = pSourContainer->GetItem(uSourIndex);
		if( NULL==pSourItem )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}

		Item* pDestItem = pDestContainer->GetItem(nDestIndex);
		if( NULL==pDestItem )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}
		if( pSourItem->GetItemIndex() != pDestItem->GetItemIndex() )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}
		if( !pSourItem->IsCanLay() )
		{
			Assert(FALSE) ;
			return ITEMOE_UNKNOW ;
		}
		if( pSourItem->IsEmpty() || pDestItem->IsEmpty() )
		{
			return ITEMOE_SOUROPERATOR_EMPTY ;
		}
		if( pSourItem->IsLock() || pDestItem->IsLock() )
		{
			return ITEMOE_SOUROPERATOR_LOCK ;
		}

		//到这里，已经完全验证了物品能够叠加

        //修改目标物品的内容
		INT nSourLayCount = pSourItem->GetLayedNum();
		INT nDestLayCount = pDestItem->GetLayedNum();
		INT nMaxLayCount  = pDestItem->GetMaxLayedNum();
		
		if( nMaxLayCount >= nSourLayCount + nDestLayCount )
		{
			SetItemLayCount( pDestContainer, nDestIndex, nSourLayCount+nDestLayCount );

			//删除原来的物品
			BOOL bRet = pSourContainer->EraseItem(uSourIndex);
			Assert(bRet);
		}
		else
		{
			SetItemLayCount( pDestContainer, nDestIndex, nMaxLayCount );
			SetItemLayCount( pSourContainer, uSourIndex, nSourLayCount+nDestLayCount - nMaxLayCount );
		}

		return ITEMOE_SUCCESS;

	__LEAVE_FUNCTION
		return ITEMOE_UNKNOW;
}



VOID ItemOperator::LockItem( ItemContainer* pContainer, INT nIndex )
{
__ENTER_FUNCTION

	if(		pContainer==NULL 
		||  nIndex<0 
		||  nIndex>=pContainer->GetContainerSize() )
	{
		AssertEx( FALSE, "ItemOperator::LockItem" ) ;
		return ;
	}

	Item* pItem = pContainer->GetItem( nIndex ) ;

	pItem->Lock( ) ;

__LEAVE_FUNCTION
}

VOID ItemOperator::UnlockItem( ItemContainer* pContainer, INT nIndex )
{
__ENTER_FUNCTION

	if(		pContainer==NULL 
		||  nIndex<0 
		||  nIndex>=pContainer->GetContainerSize() )
	{
		AssertEx( FALSE, "ItemOperator::UnlockItem" ) ;
		return ;
	}

	Item* pItem = pContainer->GetItem( nIndex ) ;

	pItem->Unlock( ) ;
	pItem->SetInExchange(FALSE);

__LEAVE_FUNCTION
}

VOID ItemOperator::SetItemLayCount( ItemContainer* pContainer, INT nIndex, INT nCount )
{
__ENTER_FUNCTION

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetItemLayCount get invalid");
		return;
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)return;

	if(pDestItem->IsEmpty())
	{
		return;
	}

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return;
	}

	pContainer->SetItemLayCount(nIndex,nCount);

	
__LEAVE_FUNCTION
}

BOOL ItemOperator::DecItemLayCount(ItemContainer* pContainer,INT nIndex, INT nCount)
{
	__ENTER_FUNCTION
		
		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::DecItemLayCount get invalid");
			return	FALSE;
		}
		
		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)return	FALSE;

		if(pDestItem->IsEmpty())
		{
			return FALSE;
		}

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}

		return pContainer->DecItemLayCount(nIndex,nCount);

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	ItemOperator::IncItemLayCount(ItemContainer* pContainer,INT nIndex, INT nCount)
{
	__ENTER_FUNCTION
		
		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::IncItemLayCount get invalid");
			return	FALSE;
		}

		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)return	FALSE;

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}

		return pContainer->IncItemLayCount(nIndex,nCount);


	__LEAVE_FUNCTION

		return	FALSE;
}


BOOL	ItemOperator::SetItemDur(ItemContainer* pContainer,INT nIndex, INT nDur)
{
	__ENTER_FUNCTION

		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::SetItemDur get Invalid");
			return	FALSE;
		}
		
		
		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)return	FALSE;

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}

		return pContainer->SetItemDur(nIndex,nDur);


	__LEAVE_FUNCTION

		return	FALSE;
}

BOOL	ItemOperator::SetItemIdent(ItemContainer* pContainer,INT nIndex)
{
	__ENTER_FUNCTION

		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::SetItemIdent get Invalid");
			return	FALSE;
		}

		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)return	FALSE;

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}

		return pContainer->SetItemIdent(nIndex);


		__LEAVE_FUNCTION

		return	FALSE;
}

BOOL	ItemOperator::SetItemDamagePoint(ItemContainer* pContainer,INT nIndex, INT nPoint)
{
	__ENTER_FUNCTION

		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::SetItemDamagePoint get Invalid");
			return	FALSE;
		}
		
		
		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)return	FALSE;

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}

		return pContainer->SetItemDamagePoint(nIndex,nPoint);


	__LEAVE_FUNCTION

		return	FALSE;
}

BOOL	ItemOperator::SetItemMaxDur(ItemContainer* pContainer,INT nIndex, INT nDur)
{
	__ENTER_FUNCTION

		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::SetItemDur get Invalid");
			return	FALSE;
		}


		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)return	FALSE;

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}

		return pContainer->SetItemMaxDur(nIndex,nDur);


		__LEAVE_FUNCTION

			return	FALSE;
}


BOOL	ItemOperator::SetItemBind(ItemContainer* pContainer,INT nIndex)
{
	__ENTER_FUNCTION
		
		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::SetItemBind get Invalid");
			return	FALSE;
		}
		
		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)return	FALSE;

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}

		return pContainer->SetItemBind(nIndex);


	__LEAVE_FUNCTION

		return FALSE;
}

//设置物品修理次数
BOOL ItemOperator::SetItemFailTimes(ItemContainer*	pContainer,INT nIndex, INT nTimes)
{
	__ENTER_FUNCTION

		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::SetItemRepairTimes get Invalid");
			return	FALSE;
		}

		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)return	FALSE;

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}

		return pContainer->SetItemFailTimes(nIndex, nTimes);


		__LEAVE_FUNCTION

			return FALSE;

}

BOOL ItemOperator::SetItemPWLock(ItemContainer*	pContainer,INT nIndex, BOOL bLock)
{
	__ENTER_FUNCTION

		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::SetItemPWLock get Invalid");
			return	FALSE;
		}

		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)return	FALSE;

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}

		return pContainer->SetItemPWLock(nIndex, bLock);


		__LEAVE_FUNCTION

			return FALSE;

}

BOOL ItemOperator::SetItemCreator(ItemContainer* pContainer,INT nIndex,const CHAR* CreatorName)
{
	__ENTER_FUNCTION

		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::SetItemCreator get Invalid");
			return	FALSE;
		}

		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)return	FALSE;

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}

		return pContainer->SetItemCreator(nIndex, CreatorName);


		__LEAVE_FUNCTION

			return FALSE;

}

BOOL ItemOperator::SetItemParam(ItemContainer* pContainer,INT nIndex,UINT start,INT type,INT value)
{
	__ENTER_FUNCTION

		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::SetItemParam get Invalid");
			return	FALSE;
		}

		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)return	FALSE;

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}

		return pContainer->SetItemParam(nIndex,start,type,value);


		__LEAVE_FUNCTION

			return FALSE;

}






VOID ItemOperator::SetItemGUID( ItemContainer* pContainer, INT nIndex, _ITEM_GUID* pGuid )
{
__ENTER_FUNCTION
	
	Assert(pContainer);
	Assert(pGuid);
	
	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetItemGUID() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)
	{
		return;
	}

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return;
	}
	
	pContainer->SetItemGuid(nIndex,pGuid);


	__LEAVE_FUNCTION

}


VOID ItemOperator::SetItemGUID( ItemContainer* pContainer, INT nIndex, PET_GUID_t* pGuid ) 
{
	__ENTER_FUNCTION

	Assert(pContainer);
	Assert(pGuid);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetItemGUID() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)
	{
		return;
	}

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return;
	}

	pContainer->SetItemGuid(nIndex,pGuid);


	__LEAVE_FUNCTION

}

BOOL ItemOperator::GenItemGUID(_ITEM_GUID& ig)
{
__ENTER_FUNCTION

	return	g_pItemManager->GenItemGuid(ig);

__LEAVE_FUNCTION

	return FALSE;
}

BOOL ItemOperator::GenItemGUID(PET_GUID_t& ig)
{
	__ENTER_FUNCTION
	
		//todo
		return TRUE;


	__LEAVE_FUNCTION
		
		return FALSE;
}

BOOL	ItemOperator::AddItemAttr(ItemContainer* pContainer,INT nIndex,_ITEM_ATTR iA)
{
	__ENTER_FUNCTION

		Assert(pContainer);

		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::AddItemAttr() Get Invalid");
		}

		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)return	FALSE;

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}
		return pContainer->AddItemAttr(nIndex,iA);

	__LEAVE_FUNCTION

		return	FALSE;
}

BOOL	ItemOperator::DelItemAttr(ItemContainer* pContainer,INT nIndex,_ITEM_ATTR iA)
{
	__ENTER_FUNCTION

		Assert(pContainer);

		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::DelItemAttr() Get Invalid");
		}

		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)	return	FALSE;

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}
		return pContainer->DelItemAttr(nIndex,iA);

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	ItemOperator::DelGemInfo(ItemContainer* pContainer,INT nIndex,INT GemIndex)
{
	__ENTER_FUNCTION

		Assert(pContainer);
		
		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::DelItemInfo() Get Invalid");
		}

		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)	return	FALSE;

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}
		return pContainer->DelGemInfo(nIndex,GemIndex);

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	ItemOperator::AddGemInfo(ItemContainer* pContainer,INT nIndex,INT& GemIndex,UINT GemType)
{
	__ENTER_FUNCTION

		Assert(pContainer);

		if(!pContainer)
		{
			AssertEx(FALSE,"ItemOperator::AddItemInfo() Get Invalid");
		}

		Item* pDestItem = pContainer->GetItem(nIndex);
		Assert(pDestItem);
		if(!pDestItem)	return	FALSE;

		if(pDestItem->IsLock())
		{	
			Assert(FALSE);
			return	FALSE;
		}
		return pContainer->AddGemInfo(nIndex,GemIndex,GemType);

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL  	ItemOperator::SetItemValue(ItemContainer*	pContainer,INT nIndex,const _PET_DB_LOAD*	pPet)
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetItemValue Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}
	return pContainer->SetItemValue(nIndex,pPet);

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL  	ItemOperator::SetPetGUID(ItemContainer*	pContainer,INT nIndex,PET_GUID_t GUID)				//设置宠物GUID
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetPetGUID() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}
	return pContainer->SetPetGUID(nIndex,GUID);

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL  	ItemOperator::SetSpouseGUID(ItemContainer*	pContainer,INT nIndex,PET_GUID_t GUID)
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetSpouseGUID() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}
	return pContainer->SetSpouseGUID(nIndex,GUID);

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL	ItemOperator::SetDataID(ItemContainer*	pContainer,INT nIndex,INT ID)							//设置宠物模型		
{
	__ENTER_FUNCTION

		Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetDataID() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}
	return pContainer->SetDataID(nIndex,ID);

	__LEAVE_FUNCTION

		return FALSE;
}
BOOL 	ItemOperator::SetName(ItemContainer*	pContainer,INT nIndex,const CHAR* pName)						//设置名字
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetName() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	Assert(pName);

	return pContainer->SetName(nIndex,pName);

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL   	ItemOperator::SetNick(ItemContainer*	pContainer,INT nIndex,const CHAR* pNick)						//设置昵称
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetNick() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	Assert(pNick);

	return pContainer->SetNick(nIndex,pNick);

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	ItemOperator::SetLevel(ItemContainer*	pContainer,INT nIndex,INT level)						//设置等级
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetLevel() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetLevel(nIndex,level);

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	ItemOperator::SetTakeLevel(ItemContainer*	pContainer,INT nIndex,INT takeLevel)				//设置携带等级
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetTakeLevel() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetTakeLevel(nIndex,takeLevel);

	__LEAVE_FUNCTION

	return FALSE;
}


BOOL  	ItemOperator::SetAttackType(ItemContainer*	pContainer,INT nIndex,INT attackType)				//设置进攻类型（物/法）
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetAttackType() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetAttackType(nIndex,attackType);

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	ItemOperator::SetAIType(ItemContainer*	pContainer,INT nIndex,INT AIType)						//设置AI类型
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetAIType() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetAIType(nIndex,AIType);

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	ItemOperator::SetCampData(ItemContainer*	pContainer,INT nIndex,const _CAMP_DATA* pCamp)		//设置阵营
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetCampData() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetCampData(nIndex,pCamp);

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	ItemOperator::SetHP(ItemContainer*	pContainer,INT nIndex,INT hp)								//设置生命值
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetHP() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetHP(nIndex,hp);

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL  	ItemOperator::SetLife(ItemContainer*	pContainer,INT nIndex,INT Life)						//设置当前寿命
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetLife() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetLife(nIndex,Life);

	__LEAVE_FUNCTION

	return FALSE;

}
BOOL  	ItemOperator::SetPetType(ItemContainer*	pContainer,INT nIndex,BYTE PetType)					//宝宝，变异，野生
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetPetType() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetPetType(nIndex,PetType);

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	ItemOperator::SetGeneration(ItemContainer*	pContainer,INT nIndex,BYTE Gen)					//几代宠
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetGeneration() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetGeneration(nIndex,Gen);

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	ItemOperator::SetHappiness(ItemContainer*	pContainer,INT nIndex,BYTE byHappiness)						//快乐度		
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetHappiness() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetHappiness(nIndex,byHappiness);

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	ItemOperator::SetStrPer(ItemContainer*	pContainer,INT nIndex,INT strper)						//力量资质	
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetStrPer() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetStrPer(nIndex,strper);

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	ItemOperator::SetConPer(ItemContainer*	pContainer,INT nIndex,INT conper)						//体力资质	
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetConPer() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetConPer(nIndex,conper);

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL   	ItemOperator::SetDexPer(ItemContainer*	pContainer,INT nIndex,INT dexper)						//身法资质
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetDexPer() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetDexPer(nIndex,dexper);

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	ItemOperator::SetSprPer(ItemContainer*	pContainer,INT nIndex,INT sprper)						//灵气资质
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetSprPer() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetSprPer(nIndex,sprper);

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL   	ItemOperator::SetIntPer(ItemContainer*	pContainer,INT nIndex,INT intper)						//定力资质
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetIntPer() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetIntPer(nIndex,intper);

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL	ItemOperator::SetGenGu(ItemContainer*	pContainer,INT nIndex,INT gengu)						//根骨
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetGenGu() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetGenGu(nIndex,gengu);

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	ItemOperator::SetGrowRate(ItemContainer*	pContainer,INT nIndex,FLOAT rate)					//成长率
{
	__ENTER_FUNCTION

		Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetGrowRate() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetGrowRate(nIndex,rate);

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	ItemOperator::SetRemainPoint(ItemContainer*	pContainer,INT nIndex,INT rPoint)					//一级属性剩余点数
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetRemainPoint() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetRemainPoint(nIndex,rPoint);

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	ItemOperator::SetExp(ItemContainer*	pContainer,INT nIndex,INT exp)						//经验值
{
	__ENTER_FUNCTION

	Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetExp() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetExp(nIndex,exp);

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	ItemOperator::SetLvl1Attr(ItemContainer*	pContainer,INT nIndex,CHAR_ATTR_LEVEL1 type,INT value)//基础一级战斗属性（不包括技能和装备增加的部分）
{
	__ENTER_FUNCTION

		Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetLvl1Attr() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetLvl1Attr(nIndex,type,value);

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  	ItemOperator::SetSkill(ItemContainer*	pContainer,INT nIndex,UINT SkillIndex,_PET_SKILL skill)	 //宠物技能
{	
	__ENTER_FUNCTION

		Assert(pContainer);

	if(!pContainer)
	{
		AssertEx(FALSE,"ItemOperator::SetSkill() Get Invalid");
	}

	Item* pDestItem = pContainer->GetItem(nIndex);
	Assert(pDestItem);
	if(!pDestItem)	return	FALSE;

	if(pDestItem->IsLock())
	{	
		Assert(FALSE);
		return	FALSE;
	}

	return pContainer->SetSkill(nIndex,SkillIndex,skill);

	__LEAVE_FUNCTION

	return FALSE;
}
