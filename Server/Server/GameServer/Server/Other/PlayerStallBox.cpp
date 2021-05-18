#include "stdafx.h"
/************************************************************************/
/*
创建日期:	2006年2月13日
创建时间:	11:54
文件名称:	PlayerStallBox.h
文件路径:	d:\Prj\Server\Server\Other\PlayerStallBox.cpp

文件功能：	玩家商店的一个柜台
修改纪录：
*/
/************************************************************************/

#include "PlayerStallBox.h"
#include "PlayerShop.h"
VOID PlayerStallBox::Init(StallBoxDB_t* pStallBoxDB, PlayerShop* pShop)
{
	__ENTER_FUNCTION

	if(pStallBoxDB == NULL)
		return;
	m_pStallBoxDB			= pStallBoxDB;

	//连接到运行时数据
	m_StallBoxRT.m_pShop	=	pShop;

	//初始化container
	_ITEMCONTAINER_INIT	containInit;
	containInit.m_nContainerSize	=	STALL_BOX_SIZE;
	m_StallBoxRT.m_Container.Init(&containInit);

	for(INT i=0;i<STALL_BOX_SIZE;i++)
	{
		_ITEM_INIT	itemInit(&m_pStallBoxDB->m_ItemList[i]);
		m_StallBoxRT.m_Container.GetItem(i)->Init(&itemInit);
		m_StallBoxRT.m_Serial[i] = 0;
	}

	//初始化container
	_ITEMCONTAINER_INIT	petcontainInit;
	petcontainInit.m_nContainerSize	=	STALL_PET_BOX_SIZE;
	m_StallBoxRT.m_PetContainer.Init(&petcontainInit);

	for(INT i=0;i<STALL_PET_BOX_SIZE;i++)
	{
		_ITEM_INIT	petInit(&m_pStallBoxDB->m_PetList[i]);
		m_StallBoxRT.m_PetContainer.GetItem(i)->Init(&petInit);
		m_StallBoxRT.m_Serial[i] = 0;
	}

	__LEAVE_FUNCTION
}

VOID PlayerStallBox::CleanUp()
{
	__ENTER_FUNCTION
	
	m_pStallBoxDB->CleanUp();
	for(INT i=0;i<STALL_BOX_SIZE;i++)
	{
		m_StallBoxRT.m_Serial[i] = 0;
	}


	__LEAVE_FUNCTION
}


BOOL PlayerStallBox::CanSale(UINT uIndex)
{
	__ENTER_FUNCTION

	if( m_pStallBoxDB && uIndex>=0 && uIndex<STALL_BOX_SIZE )
	{
		return m_pStallBoxDB->m_bItemIsOnSale[uIndex];
	}
	__LEAVE_FUNCTION

	return FALSE;
}
VOID PlayerStallBox::SetCanSale(UINT uIndex, BOOL bCan)
{
	__ENTER_FUNCTION

	if( m_pStallBoxDB && uIndex>=0 && uIndex<STALL_BOX_SIZE )
	{
		m_pStallBoxDB->m_bItemIsOnSale[uIndex] = bCan;
		m_StallBoxRT.m_pShop->DataChanged();
	}
	__LEAVE_FUNCTION

}

UINT PlayerStallBox::GetPriceByIndex(UINT uIndex)
{
	__ENTER_FUNCTION
	if( m_pStallBoxDB && uIndex>=0 && uIndex<STALL_BOX_SIZE )
	{
		return m_pStallBoxDB->m_ItemPrice[uIndex];
	}
	return 0;
	__LEAVE_FUNCTION
	return 0;
}

VOID PlayerStallBox::SetPriceByIndex(UINT uIndex, UINT nPrice)
{
	__ENTER_FUNCTION

	if( m_pStallBoxDB && uIndex>=0 && uIndex<STALL_BOX_SIZE )
	{
		m_pStallBoxDB->m_ItemPrice[uIndex] = nPrice;
		m_StallBoxRT.m_pShop->DataChanged();
	}
	__LEAVE_FUNCTION
}

BYTE PlayerStallBox::GetSerialByIndex(UINT uIndex)
{
	__ENTER_FUNCTION
	if( m_pStallBoxDB && uIndex>=0 && uIndex<STALL_BOX_SIZE )
	{
		 return m_StallBoxRT.m_Serial[uIndex];
	}
	return 0;
	__LEAVE_FUNCTION
	return 0;
}

VOID PlayerStallBox::SetSerialByIndex(UINT uIndex, BYTE uSerial)
{
	__ENTER_FUNCTION
	if( m_pStallBoxDB && uIndex>=0 && uIndex<STALL_BOX_SIZE )
	{
		m_StallBoxRT.m_Serial[uIndex] = uSerial;
	}
	__LEAVE_FUNCTION
}

BYTE PlayerStallBox::IncSerialByIndex(UINT uIndex)
{
	__ENTER_FUNCTION
	if( m_pStallBoxDB && uIndex>=0 && uIndex<STALL_BOX_SIZE )
	{
		m_StallBoxRT.m_Serial[uIndex]++;
		return m_StallBoxRT.m_Serial[uIndex];
	}
	return 0;
	__LEAVE_FUNCTION
	return 0;
}

VOID PlayerStallBox::SetStallStatus(const STATUS_STALL stallstatus)
{
	m_pStallBoxDB->m_StallStatus = (BYTE)stallstatus;
	m_StallBoxRT.m_pShop->DataChanged();
}

VOID PlayerStallBox::SetStallType(const TYPE_STALL stalltype)
{
	m_pStallBoxDB->m_StallType = (BYTE)stalltype;
	m_StallBoxRT.m_pShop->DataChanged();
}
