/************************************************************************/
/*
	创建日期:	2006年2月13日
	创建时间:	11:54
	文件名称:	PlayerStallBox.h
	文件路径:	d:\Prj\Server\Server\Other\PlayerStallBox.h

	文件功能：	玩家商店的一个柜台
	修改纪录：
*/
/************************************************************************/

#ifndef _PLAYER_STALL_BOX_H_
#define _PLAYER_STALL_BOX_H_

#include "PlayerShopDefine.h"
#include "ItemContainer.h"
/*
服务器端每个柜台的结构.
*/
class PlayerShop;
using namespace PLAYER_SHOP;
class PlayerStallBox
{
public:
	//运行时结构
	struct StallBoxRT_t
	{
		ItemContainer	m_Container;							//ITEM_CONTAINER
		ItemContainer	m_PetContainer;							//PET_CONTAINER
		BYTE			m_Serial[STALL_BOX_SIZE];				//序列号数组
		PlayerShop*		m_pShop;
	};
public:
	PlayerStallBox()
	{
		m_pStallBoxDB = NULL;
	};
	~PlayerStallBox()
	{

	};
public:
	//-------------------------------------------------------------------------------------------------
	//初始化,清空玩家商店的柜台
	VOID Init(StallBoxDB_t* pStallBoxDB, PlayerShop* pShop);

	VOID CleanUp();
public:
	//-------------------------------------------------------------------------------------------------
	//操作
	STATUS_STALL	GetStallStatus(){return (STATUS_STALL)m_pStallBoxDB->m_StallStatus;}
	VOID			SetStallStatus(const STATUS_STALL stallstatus);

	TYPE_STALL		GetStallType(){return (TYPE_STALL)m_pStallBoxDB->m_StallType;}
	VOID			SetStallType(const TYPE_STALL stalltype);

	BOOL			CanSale(UINT uIndex);
	VOID			SetCanSale(UINT uIndex, BOOL bCan);

	UINT			GetPriceByIndex(UINT uIndex);
	VOID			SetPriceByIndex(UINT uIndex, UINT nPrice);

	BYTE			GetSerialByIndex(UINT uIndex);
	VOID			SetSerialByIndex(UINT uIndex, BYTE uSerial);
	BYTE			IncSerialByIndex(UINT uIndex);

	ItemContainer*	GetContainer()
	{
		return &m_StallBoxRT.m_Container;
	}

	ItemContainer*	GetPetContainer()
	{
		return &m_StallBoxRT.m_PetContainer;
	}

private:
	//-------------------------------------------------------------------------------------------------
	//需要存储数据库的信息
	StallBoxDB_t*	m_pStallBoxDB;
	//-------------------------------------------------------------------------------------------------
	//运行时信息
	StallBoxRT_t	m_StallBoxRT;
};
#endif