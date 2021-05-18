/************************************************************************/
/*
	创建日期:	2006年2月13日
	创建时间:	11:54
	文件名称:	PlayerShop.h
	文件路径:	d:\Prj\Server\Server\Other\PlayerShop.h

	文件功能：	玩家商店
	修改纪录：
*/
/************************************************************************/

#ifndef _PLAYER_SHOP_H_
#define _PLAYER_SHOP_H_

#include "PlayerShopDefine.h"
#include "PlayerStallBox.h"
/*
玩家商店
*/
using namespace PLAYER_SHOP;
class PlayerShop
{
	friend class PlayerShopPool;
public:
	//-------------------------------------------------------------------------------------------------
	//运行时结构
	struct PlayerShopRT_t
	{
		PlayerStallBox					m_StallList[MAX_STALL_NUM_PER_SHOP];		//运行时柜台列表
		BYTE							m_bSerial;									//商店自己的序列号,用于商店的交易
		BOOL							m_PlayerShopChanged;						//商店数据改变

	};
public:
	PlayerShop();
	~PlayerShop();
public:
	//-------------------------------------------------------------------------------------------------
	//初始化,清空玩家商店
	VOID Init();

	VOID CleanUp();
public:
	//-------------------------------------------------------------------------------------------------
	//一个商店的唯一ID
	INT					GetID(){return GetPoolPos();}
	SceneID_t			GetSceneID(){return m_pPlayerShopDB->m_ShopGUID.m_Scene;}

	STATUS_PLAYER_SHOP	GetShopStatus(){return (STATUS_PLAYER_SHOP)m_pPlayerShopDB->m_ShopStatus;}
	VOID				SetShopStatus(const STATUS_PLAYER_SHOP ShopStatus)
	{
		m_pPlayerShopDB->m_ShopStatus = (BYTE)ShopStatus;
		DataChanged();
	}

	TYPE_PLAYER_SHOP	GetShopType(){return (TYPE_PLAYER_SHOP)m_pPlayerShopDB->m_ShopType;}
	VOID				SetShopType(const TYPE_PLAYER_SHOP ShopType)
	{
		m_pPlayerShopDB->m_ShopType = (BYTE)ShopType;
		DataChanged();
	}

	_PLAYERSHOP_GUID	GetShopGUID(){return m_pPlayerShopDB->m_ShopGUID;}
	VOID				SetShopGUID(const _PLAYERSHOP_GUID ShopGUID)
	{
		m_pPlayerShopDB->m_ShopGUID = ShopGUID;
		DataChanged();
	}

	UINT				GetMaxBaseMoney(){return m_pPlayerShopDB->m_MaxBaseMoney;}
	VOID				SetMaxBaseMoney(UINT MaxBaseMoney)
	{
		m_pPlayerShopDB->m_MaxBaseMoney = MaxBaseMoney;
		DataChanged();
	}
	UINT				UpdateMaxBaseMoney(FLOAT fComFactor);

	BOOL				MinusMoney(UINT uMoney);
	VOID				AddMoney(UINT uMoney);
	VOID				UpdateMoneyEvent();

	UINT				GetBaseMoney(){return m_pPlayerShopDB->m_BaseMoney;}
	VOID				SetBaseMoney(UINT BaseMoney)
	{
		m_pPlayerShopDB->m_BaseMoney = BaseMoney;
		DataChanged();
	}

	UINT				GetProfitMoney(){return m_pPlayerShopDB->m_ProfitMoney;}
	VOID				SetProfitMoney(UINT ProfitMoney)
	{
		m_pPlayerShopDB->m_ProfitMoney = ProfitMoney;
		DataChanged();
	}

	CHAR*				GetShopName(){return m_pPlayerShopDB->m_szShopName;}
	VOID				SetShopName(CHAR* pszShopName, UINT uSize)
	{
		memset(m_pPlayerShopDB->m_szShopName, 0, MAX_SHOP_NAME_SIZE);
		memcpy(m_pPlayerShopDB->m_szShopName, pszShopName, uSize);
		DataChanged();
	}

	CHAR*				GetShopDesc(){return m_pPlayerShopDB->m_szShopDesc;}
	VOID				SetShopDesc(CHAR* pszShopDesc, UINT uSize)
	{
		memset(m_pPlayerShopDB->m_szShopDesc, 0, MAX_SHOP_DESC_SIZE);
		memcpy(m_pPlayerShopDB->m_szShopDesc, pszShopDesc, uSize);
		DataChanged();
	}

	CHAR*				GetOwnerName(){return m_pPlayerShopDB->m_szOwnerName;}
	VOID				SetOwnerName(const CHAR* pszOwnerName)
	{
		memset(m_pPlayerShopDB->m_szOwnerName, 0, MAX_CHARACTER_NAME);
		memcpy(m_pPlayerShopDB->m_szOwnerName, pszOwnerName, strlen(pszOwnerName));
		DataChanged();
	}

	GUID_t				GetOwnerGuid(){ return m_pPlayerShopDB->m_OwnerGuid; }
	VOID				SetOwnerGuid(GUID_t OwnerGuid)
	{
		m_pPlayerShopDB->m_OwnerGuid = OwnerGuid; 
		DataChanged();
	}
	
	//-------------------------------------------------------------------------------------------------
	//打开的,开张的柜台
	BYTE				GetNumStallOpened(){ return m_pPlayerShopDB->m_NumStallOpened; }
	VOID				SetNumStallOpened(BYTE NumStallOpened)
	{
		m_pPlayerShopDB->m_NumStallOpened = NumStallOpened; 
		DataChanged();
	}

	BYTE				GetNumStallOnSale(){ return m_pPlayerShopDB->m_NumStallOnSale; }
	VOID				SetNumStallOnSale(BYTE NumStallOnSale)
	{
		m_pPlayerShopDB->m_NumStallOnSale = NumStallOnSale; 
		DataChanged();
	}

	UINT				GetSaleOutPrice(){ return m_pPlayerShopDB->m_uSaleOutPrice; }
	VOID				SetSaleOutPrice(UINT uSaleOutPrice)
	{
		m_pPlayerShopDB->m_uSaleOutPrice = uSaleOutPrice; 
		DataChanged();
	}

	//-------------------------------------------------------------------------------------------------
	//建店时间
	UINT				GetFoundedYear(){ return m_pPlayerShopDB->m_FoundedYear; }
	VOID				SetFoundedYear(UINT nFoundedYear)
	{
		m_pPlayerShopDB->m_FoundedYear = nFoundedYear; 
		DataChanged();
	}

	BYTE				GetFoundedMonth(){ return m_pPlayerShopDB->m_FoundedMonth; }
	VOID				SetFoundedMonth(BYTE nFoundedMonth)
	{
		m_pPlayerShopDB->m_FoundedMonth = nFoundedMonth; 
		DataChanged();
	}

	BYTE				GetFoundedDay(){ return m_pPlayerShopDB->m_FoundedDay; }
	VOID				SetFoundedDay(BYTE nFoundedDay)
	{
		m_pPlayerShopDB->m_FoundedDay = nFoundedDay; 
		DataChanged();
	}

	BYTE				GetFoundedHour(){ return m_pPlayerShopDB->m_FoundedHour; }
	VOID				SetFoundedHour(BYTE nFoundedHour)
	{
		m_pPlayerShopDB->m_FoundedHour = nFoundedHour; 
		DataChanged();
	}

	BYTE				GetFoundedMin(){ return m_pPlayerShopDB->m_FoundedMin; }
	VOID				SetFoundedMin(BYTE nFoundedMin)
	{
		m_pPlayerShopDB->m_FoundedMin = nFoundedMin; 
		DataChanged();
	}

	//-------------------------------------------------------------------------------------------------
	//序列号
	BYTE				GetSerial(){ return m_PlayerShopRT.m_bSerial; }
	VOID				SetSerial(BYTE nSerial){ m_PlayerShopRT.m_bSerial = nSerial; }
	BYTE				IncSerial(){return ++m_PlayerShopRT.m_bSerial;}

	//-------------------------------------------------------------------------------------------------
	//获得交易记录
	RecordContainer_t*	GetExchangeRecord() {return &m_pPlayerShopDB->m_ExchangeRecord;}

	//-------------------------------------------------------------------------------------------------
	//获得管理记录
	RecordContainer_t*	GetManagerRecord() {return &m_pPlayerShopDB->m_ManagerRecord;}

	//-------------------------------------------------------------------------------------------------
	//添加合作伙伴
	RET_TYPE_PARTNER	AddPartner( GUID_t Guid);

	//-------------------------------------------------------------------------------------------------
	//删除合作伙伴
	RET_TYPE_PARTNER	RemovePartner( GUID_t Guid );

	//-------------------------------------------------------------------------------------------------
	//合作伙伴列表
	Partner_t*			GetPartnerList(){return m_pPlayerShopDB->m_PartnerList;}

	//-------------------------------------------------------------------------------------------------
	//是否是合作伙伴
	BOOL				IsPartner(GUID_t Guid);


	//-------------------------------------------------------------------------------------------------
	//心跳
	BOOL				HeartBeat(UINT uTime, FLOAT	fComFactor);
    
	PlayerStallBox*		GetPlayerStallBoxByIndex(UINT uIndex);

	/*ServerBBS*			GetServerBBS(){return &m_pPlayerShopDB->m_StallBBS;}*/

	//-------------------------------------------------------------------------------------------------
	//数据改变需要存盘
	VOID				DataChanged(){	m_PlayerShopRT.m_PlayerShopChanged = TRUE;}

private:
	//-------------------------------------------------------------------------------------------------
	//池位置,一个商店的唯一ID
	INT					GetPoolPos(){ return m_pPlayerShopDB->m_ShopGUID.m_PoolPos;}
	VOID				SetPoolPos(INT	nPoolPos){m_pPlayerShopDB->m_ShopGUID.m_PoolPos = nPoolPos;}

private:
	//-------------------------------------------------------------------------------------------------
	//需要存储数据库的信息
	PlayerShopDB_t*		m_pPlayerShopDB;
	//-------------------------------------------------------------------------------------------------
	//ShareMemory 数据
	PlayerShopSM*		m_pPlayerShopSM;
	//-------------------------------------------------------------------------------------------------
	//运行时信息
	PlayerShopRT_t		m_PlayerShopRT;
};

#endif