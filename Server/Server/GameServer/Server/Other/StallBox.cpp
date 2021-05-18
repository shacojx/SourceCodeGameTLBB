#include "stdafx.h"
#include "StallBox.h"

#include "ItemContainer.h"
#include "Obj_Human.h"
#include "HumanItemLogic.h"
#include "ItemOperator.h"
#include "Scene.h"
#include "StallInfoManager.h"

VOID ServerStallBox::Init(Obj_Human* pHuman)
{
	__ENTER_FUNCTION
	m_pMySelf = pHuman;

//	Assert(pHuman->getScene());
//	m_pStallInfoMgr = pHuman->getScene()->GetStallInfoManager();

	//清空物品数据
	memset(m_ItemList, 0, sizeof(_ITEM)*STALL_BOX_SIZE);
	memset(m_PetItemList, 0, sizeof(_PET_DB_LOAD)*STALL_PET_BOX_SIZE);

	//初始化container
	_ITEMCONTAINER_INIT	containInit;
	containInit.m_nContainerSize	=	STALL_BOX_SIZE;
	m_Container.Init(&containInit);

	for(int i=0;i<STALL_BOX_SIZE;i++)
	{
		_ITEM_INIT	itemInit(&m_ItemList[i]);
		m_Container.GetItem(i)->Init(&itemInit);
	}

	//初始化宠物container
	_ITEMCONTAINER_INIT	PetContainInit;
	PetContainInit.m_nContainerSize		=	STALL_PET_BOX_SIZE;
	m_PetContainer.Init(&PetContainInit);

	for(UINT i = 0 ;i<STALL_PET_BOX_SIZE;i++)
	{
		_ITEM_INIT itemInit(&m_PetItemList[i]);
		m_PetContainer.GetItem(i)->Init(&itemInit);
	}

	m_nFirstPage = 0;

	CleanUp();

	__LEAVE_FUNCTION
}

VOID ServerStallBox::CleanUp()
{
	__ENTER_FUNCTION
	//解所有锁
	UnLockAllItem();

	//释放当前位置
	UnLockMapPos();

	//清空所有状态
	m_StallStatus	=	STALL_CLOSE;
	m_IsOpen		=	FALSE;
	m_PosTax		=	0;
	m_TradeTax		=	0;
	memset(m_szStallName, 0, MAX_STALL_NAME);

	m_SerialIncStep	=	1;
	memset(m_ItemSerial, 0, STALL_BOX_SIZE*sizeof(UINT));
	memset(m_ItemPrice, 0, STALL_BOX_SIZE*sizeof(UINT));

	memset(m_PetSerial, 0, STALL_PET_BOX_SIZE*sizeof(UINT));
	memset(m_PetPrice, 0, STALL_PET_BOX_SIZE*sizeof(UINT));

	//清空物品数据
	memset(m_ItemList, 0, sizeof(_ITEM)*STALL_BOX_SIZE);
	memset(m_PetItemList, 0, sizeof(_PET_DB_LOAD)*STALL_PET_BOX_SIZE);

	//清空摊位BBS
	m_StallBBS.CleanUp();

	//初始化摊位名，广告语
	memset(m_szStallName, 0, MAX_STALL_NAME);
	sprintf(m_szStallName, "杂货摊位");
	
	CHAR szBBSTitle[MAX_BBS_MESSAGE_LENGTH];
	memset(szBBSTitle, 0, MAX_BBS_MESSAGE_LENGTH);
	sprintf(szBBSTitle, "欢迎你来到%s的摊位",m_pMySelf->GetName());
	m_StallBBS.SetBBSTitle(szBBSTitle,(INT)strlen(szBBSTitle));
	__LEAVE_FUNCTION
}

VOID ServerStallBox::UnLockAllItem()
{
	__ENTER_FUNCTION
	if(m_pMySelf == NULL)
		return;

	for(INT i = 0; i<STALL_BOX_SIZE; i++)
	{
		if(m_Container.GetItem(i)->IsEmpty() == FALSE)
		{
			Item* pItemRef = m_Container.GetItem(i);

			ItemContainer*	pMyBagContainer = HumanItemLogic::GetItemContain(m_pMySelf, pItemRef->GetItemTableIndex());
			INT	BagIndex = pMyBagContainer->GetIndexByGUID(&pItemRef->GetGUID());

			if(BagIndex != INVALID_INDEX && pMyBagContainer != NULL)
				g_ItemOperator.UnlockItem( pMyBagContainer, BagIndex );
			else
				Assert(FALSE);
		}
	}

	for(INT i = 0; i<STALL_PET_BOX_SIZE; i++)
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

VOID	ServerStallBox::UnLockMapPos()
{
	__ENTER_FUNCTION
	if(m_pMySelf == NULL)
		return;
	
	if(!m_pMySelf->getScene())
		return;

	m_pStallInfoMgr = m_pMySelf->getScene()->GetStallInfoManager();

	//释放这个位置
	m_pStallInfoMgr->SetCanStall(m_PosX, m_PosZ, TRUE);
	m_PosX = 0;
	m_PosZ = 0;

	__LEAVE_FUNCTION
}

VOID	ServerStallBox::SetStallName(const CHAR *pszTitle, BYTE nSize )
{
	__ENTER_FUNCTION
	memset(m_szStallName, 0, MAX_STALL_NAME);
	memcpy(m_szStallName, pszTitle, nSize);
	m_pMySelf->StallNameChanged();//摊位名变化刷到其他客户端
	__LEAVE_FUNCTION
}