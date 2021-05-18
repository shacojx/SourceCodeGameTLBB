#include "stdafx.h"
/************************************************************************/
/*
创建日期:	2006年2月13日
创建时间:	11:54
文件名称:	PlayerShop.h
文件路径:	d:\Prj\Server\Server\Other\PlayerShop.cpp

文件功能：	玩家商店
修改纪录：
*/
/************************************************************************/

#include "PlayerShop.h"
#include "TimeManager.h"//g_pTimeManager
#include "SMUManager.h"
#include "ShareMemManager.h"
#include "ServerManager.h"
#include "RecordOpt.h"
#include "GUIDManager.h"
#include "Obj_Human.h"

PlayerShop::PlayerShop()
{
	__ENTER_FUNCTION
		
		m_pPlayerShopDB = new PlayerShopDB_t;
		BOOL bEnableShareMem = 
			g_pServerManager->GetCurrentServerInfo()->m_EnableShareMem;	
		if(bEnableShareMem)
		{
			PlayerShopSM* pSMU = g_PlayerShopSMUPool.NewObj();
			Assert(pSMU);
			m_pPlayerShopSM = pSMU;
		}
		else
		{
			m_pPlayerShopSM = NULL;
		}
		m_PlayerShopRT.m_PlayerShopChanged = FALSE;

	__LEAVE_FUNCTION

}
PlayerShop::~PlayerShop()
{
	__ENTER_FUNCTION
	SAFE_DELETE(m_pPlayerShopDB);
	__LEAVE_FUNCTION
}

VOID PlayerShop::Init()
{
	__ENTER_FUNCTION

	CleanUp();

	if(RecordOpt::InitRecordContainer(&(m_pPlayerShopDB->m_ExchangeRecord), MAX_EXCHANGE_RECORD) == FALSE)
	{
		Assert(0);
		return;
	}
	if(RecordOpt::InitRecordContainer(&(m_pPlayerShopDB->m_ManagerRecord), MAX_MANAGER_RECORD) == FALSE)
	{
		Assert(0);
		return;
	}

	BOOL bEnableShareMem = 
		g_pServerManager->GetCurrentServerInfo()->m_EnableShareMem;	
	if(bEnableShareMem)
	{
		*m_pPlayerShopDB = m_pPlayerShopSM->m_PlayerShop;
	}

	//连接到运行时数据
	for(UINT i = 0; i<MAX_STALL_NUM_PER_SHOP; i++)
	{
		m_PlayerShopRT.m_StallList[i].Init(&m_pPlayerShopDB->m_StallListData[i], this);		//柜台列表
	}
	
	__LEAVE_FUNCTION
}

VOID PlayerShop::CleanUp()
{
	__ENTER_FUNCTION

	m_pPlayerShopDB->CleanUp();

	//初始化摊位名，广告语
	sprintf(m_pPlayerShopDB->m_szShopName, "杂货摊位");

	m_PlayerShopRT.m_bSerial = 0;
	DataChanged();
	__LEAVE_FUNCTION
}

PlayerStallBox*	PlayerShop::GetPlayerStallBoxByIndex(UINT uIndex)
{
	__ENTER_FUNCTION
	if( uIndex>=0 && uIndex<MAX_STALL_NUM_PER_SHOP )
	{
		return &m_PlayerShopRT.m_StallList[uIndex];
	}
	return NULL;
	__LEAVE_FUNCTION
	return NULL;
}

BOOL PlayerShop::HeartBeat(UINT uTime, FLOAT fComFactor)
{
	__ENTER_FUNCTION
	BYTE	nDay		=	(BYTE)g_pTimeManager->GetDay();
	BYTE	nHour		=	(BYTE)g_pTimeManager->GetHour();

	FLOAT		fPayMent	= 0.0;
	static BYTE	CurrentDay	= 0;
	static BYTE	CurrentHour	= 0;

	if(CurrentDay == 0 && CurrentHour == 0)
	{//第一次初始化为当前时间
		CurrentDay	= nDay;
		CurrentHour = nHour;
		return TRUE;
	}

	if(CurrentDay != nDay)
	{//午夜0时
		CurrentDay = nDay;
		fPayMent = 50000*fComFactor*m_pPlayerShopDB->m_NumStallOpened;
		if(MinusMoney((UINT)fPayMent))
		{//成功

		}
		else
		{//倒闭
			//下一个逻辑桢清空所有被关闭的商店
			SetShopStatus(PLAYER_SHOP::STATUS_PLAYER_SHOP_SHUT_DOWN);
		}
	}

	if(CurrentHour != nHour)
	{//每小时
		CurrentHour = nHour;
		fPayMent = 2000*fComFactor*m_pPlayerShopDB->m_NumStallOnSale;
		if(MinusMoney((UINT)fPayMent))
		{//成功

		}
		else
		{//倒闭
			//下一个逻辑桢清空所有被关闭的商店
			SetShopStatus(PLAYER_SHOP::STATUS_PLAYER_SHOP_SHUT_DOWN);
		}
	}

	if(m_PlayerShopRT.m_PlayerShopChanged)
	{//写入sharememory
		if(m_pPlayerShopSM)
		{
			m_pPlayerShopSM->Lock(SM_S_WRITE);
			memcpy(&(m_pPlayerShopSM->m_PlayerShop),m_pPlayerShopDB,sizeof(PlayerShopDB_t));
			m_pPlayerShopSM->UnLock(SM_S_WRITE);
		}
		m_PlayerShopRT.m_PlayerShopChanged = FALSE;
	}

	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

BOOL PlayerShop::MinusMoney(UINT uMoney)
{
	if(m_pPlayerShopDB->m_ProfitMoney>uMoney)
	{
		m_pPlayerShopDB->m_ProfitMoney -= uMoney;
		DataChanged();
		return TRUE;
	}
	else if(m_pPlayerShopDB->m_ProfitMoney + m_pPlayerShopDB->m_BaseMoney>uMoney)
	{
		m_pPlayerShopDB->m_BaseMoney -= uMoney-m_pPlayerShopDB->m_ProfitMoney;
		m_pPlayerShopDB->m_ProfitMoney = 0;
		UpdateMoneyEvent();
		DataChanged();
		return TRUE;
	}
	else//倒闭了
	{
		UpdateMoneyEvent();
		DataChanged();
		return FALSE;
	}
}

VOID PlayerShop::AddMoney(UINT uMoney)
{
	UINT fBaseMoney		= GetBaseMoney();
	UINT fMaxBaseMoney	= GetMaxBaseMoney();

	if( fBaseMoney < fMaxBaseMoney )
	{//冲入本金
		if((fMaxBaseMoney-fBaseMoney)<uMoney)
		{//分两部分
			SetBaseMoney((UINT)fMaxBaseMoney);
			SetProfitMoney((UINT)(GetProfitMoney() + (uMoney - (fMaxBaseMoney-fBaseMoney))));
		}
		else
		{
			SetBaseMoney((UINT)(fBaseMoney+uMoney));
		}
	}
	else
	{//直接冲入盈利金
		SetProfitMoney((UINT)(GetProfitMoney() + uMoney) );
	}
}

VOID PlayerShop::UpdateMoneyEvent()
{
	if((FLOAT)GetBaseMoney()<((FLOAT)GetMaxBaseMoney())/2)
	{//通知客户端本金不够了

	}
}

UINT PlayerShop::UpdateMaxBaseMoney(FLOAT fComFactor)
{
	m_pPlayerShopDB->m_MaxBaseMoney = (UINT)(300000*fComFactor*GetNumStallOpened());
	DataChanged();
	return m_pPlayerShopDB->m_MaxBaseMoney;
}

//-------------------------------------------------------------------------------------------------
//添加合作伙伴
RET_TYPE_PARTNER PlayerShop::AddPartner(GUID_t Guid)
{
	__ENTER_FUNCTION
	Partner_t* PartnerList = m_pPlayerShopDB->m_PartnerList;

	if(m_pPlayerShopDB->m_PartnerNum == MAX_PARTNER_PER_SHOP)
	{
		return RET_TYPE_LIST_FULL;
	}

	for(INT i = 0; i<MAX_PARTNER_PER_SHOP; i++)
	{
		if(PartnerList[i].m_Guid == Guid)
		{
			return RET_TYPE_ALREADY_IN_LIST;
		}
	}

	Obj_Human* pSearchHuman = (Obj_Human*)g_pGUIDManager->Get(Guid) ;

	if(pSearchHuman == NULL)
	{
		return RET_TYPE_NOT_FIND_IN_WORLD;
	}

	for(INT i = 0; i<MAX_PARTNER_PER_SHOP; i++)
	{
		if(PartnerList[i].m_Guid == INVALID_GUID)
		{
			PartnerList[i].m_Guid = Guid;
			memset(PartnerList[i].m_szName, 0, MAX_CHARACTER_NAME);
			memcpy(PartnerList[i].m_szName, pSearchHuman->GetName(), MAX_CHARACTER_NAME);
			m_pPlayerShopDB->m_PartnerNum++;
			break;
		}
	}
	return RET_TYPE_SUCCESS;
	__LEAVE_FUNCTION
	return RET_TYPE_INVALID;
}

//-------------------------------------------------------------------------------------------------
//删除合作伙伴
RET_TYPE_PARTNER PlayerShop::RemovePartner(GUID_t Guid)
{
	__ENTER_FUNCTION
	Partner_t* PartnerList = m_pPlayerShopDB->m_PartnerList;

	if(m_pPlayerShopDB->m_PartnerNum == 0)
	{
		return RET_TYPE_LIST_EMPTY;
	}

	INT	iPartnerIndex = -1;
	INT i;
	for( i = 0; i<MAX_PARTNER_PER_SHOP; i++)
	{
		if(PartnerList[i].m_Guid == Guid)
		{
			iPartnerIndex = i;
			break;
		}
	}

	if(iPartnerIndex == -1)
	{
		return RET_TYPE_NOT_FIND_IN_LIST;
	}

	PartnerList[iPartnerIndex].m_Guid = INVALID_GUID;
	m_pPlayerShopDB->m_PartnerNum--;
	return RET_TYPE_SUCCESS;

	__LEAVE_FUNCTION
	return RET_TYPE_INVALID;
}

//-------------------------------------------------------------------------------------------------
//是否是合作伙伴
BOOL PlayerShop::IsPartner(GUID_t Guid)
{
	Partner_t* PartnerList = m_pPlayerShopDB->m_PartnerList;
	for(INT i = 0; i<MAX_PARTNER_PER_SHOP; i++)
	{
		if(PartnerList[i].m_Guid == Guid)
		{
			return TRUE;
		}
	}
	return FALSE;
}
