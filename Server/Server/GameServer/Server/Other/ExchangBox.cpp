#include "stdafx.h"
/*
服务器段交易盒
*/
#include "ExchangBox.h"
#include "Obj_Human.h"
#include "HumanItemLogic.h"
#include "ItemOperator.h"

VOID ServerExchangeBox::Init(Obj_Human* pHuman)
{
	__ENTER_FUNCTION
	m_pMySelf = pHuman;

	//清空物品数据
	memset(m_ItemList, 0, sizeof(_ITEM)*EXCHANGE_BOX_SIZE);
	memset(m_PetItemList, 0, sizeof(_PET_DB_LOAD)*EXCHANGE_PET_BOX_SIZE);

	//初始化物品container
	_ITEMCONTAINER_INIT	containInit;
	containInit.m_nContainerSize	=	EXCHANGE_BOX_SIZE;
	m_Container.Init(&containInit);

	for(UINT i=0;i<EXCHANGE_BOX_SIZE;i++)
	{
		_ITEM_INIT	itemInit(&m_ItemList[i]);
		m_Container.GetItem(i)->Init(&itemInit);
	}
 
	//初始化宠物container
	_ITEMCONTAINER_INIT	PetContainInit;
	PetContainInit.m_nContainerSize		=	EXCHANGE_PET_BOX_SIZE;
	m_PetContainer.Init(&PetContainInit);

	for(UINT i = 0 ;i<EXCHANGE_PET_BOX_SIZE;i++)
	{
		_ITEM_INIT itemInit(&m_PetItemList[i]);
		m_PetContainer.GetItem(i)->Init(&itemInit);
	}

	CleanUp();
	__LEAVE_FUNCTION
}

VOID ServerExchangeBox::CleanUp()
{
	__ENTER_FUNCTION
	//解所有锁
	UnLockAllItem();

	//清空所有状态
	m_Status		= EXCHANGE_NONE;
	m_ObjID			= INVALID_ID;
	m_IsLocked		= FALSE;
	m_CanConform	= FALSE;
	m_Money			= 0;

	//清空物品数据
	memset(m_ItemList, 0, sizeof(_ITEM)*EXCHANGE_BOX_SIZE);
	memset(m_PetItemList, 0, sizeof(_PET_DB_LOAD)*EXCHANGE_PET_BOX_SIZE);
	__LEAVE_FUNCTION
}

VOID ServerExchangeBox::UnLockAllItem()
{
	__ENTER_FUNCTION
	if(m_pMySelf == NULL)
		return;

	for(INT i = 0; i<EXCHANGE_BOX_SIZE; i++)
	{
		if(m_Container.GetItem(i)->IsEmpty() == FALSE)
		{
			Item* pItemRef = m_Container.GetItem(i);

			ItemContainer*	pMyBagContainer = HumanItemLogic::GetItemContain(m_pMySelf, pItemRef->GetItemTableIndex());
			INT	BagIndex = pMyBagContainer->GetIndexByGUID(&pItemRef->GetGUID());

			if(BagIndex != INVALID_INDEX && pMyBagContainer != NULL)
				g_ItemOperator.UnlockItem( pMyBagContainer, BagIndex );
		}
	}

	for(INT i = 0; i<EXCHANGE_PET_BOX_SIZE; i++)
	{
		if( m_PetContainer.GetItem(i)->IsEmpty() == FALSE )
		{
			Item* pItemRef = m_PetContainer.GetItem(i);
			ItemContainer*	pPetContainer = m_pMySelf->GetPetContain();
			INT	iPetIndex = pPetContainer->GetIndexByGUID(&pItemRef->GetPetGUID());

			if(iPetIndex != INVALID_INDEX && pPetContainer != NULL)
				g_ItemOperator.UnlockItem( pPetContainer, iPetIndex );
		}
	}
	__LEAVE_FUNCTION
}
