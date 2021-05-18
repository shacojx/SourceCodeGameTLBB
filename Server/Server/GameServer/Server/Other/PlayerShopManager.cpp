#include "stdafx.h"
/************************************************************************/
/*
创建日期:	2006年2月13日
创建时间:	11:54
文件名称:	PlayerShop.h
文件路径:	d:\Prj\Server\Server\Other\PlayerShopManager.cpp

文件功能：	玩家商店管理器
修改纪录：
*/
/************************************************************************/

#include "PlayerShopManager.h"
#include "PlayerShopPool.h"
#include "Scene.h"


VOID PlayerShopManager::Init(Scene* pScene)
{
	m_CommerceFactor = 0.5;
	CleanUp();
	m_pScene	=	pScene;
	for(UINT i = 0; i<MAX_SHOP_NUM_PER_SCENE; i++)
		m_pFreeItemID[i]	=	i;
}

VOID PlayerShopManager::CleanUp()
{
	memset(m_pPlayerShopList, 0, sizeof(PlayerShop*)*MAX_SHOP_NUM_PER_SCENE);
	m_nPlayerShopNum	=	0;
}

INT	PlayerShopManager::NewPlayerShop()
{
	__ENTER_FUNCTION
	PlayerShop* pPlayerShop = g_pPlayerShopPool->NewPlayerShop(m_pScene->SceneID());
	if(pPlayerShop!= NULL)
	{
		return AddPlayerShop(pPlayerShop);
	}
	__LEAVE_FUNCTION
		return -1;
}

INT PlayerShopManager::AddPlayerShop(PlayerShop* pPlayerShop)
{
	__ENTER_FUNCTION
	if(pPlayerShop == NULL)
		return -1;
	if(m_nPlayerShopNum == MAX_SHOP_NUM_PER_SCENE-1)
		return -1;

	m_pPlayerShopList[m_pFreeItemID[m_nPlayerShopNum]] = pPlayerShop;
	m_nPlayerShopNum++;
	UpdateComFactor();
	return m_pFreeItemID[m_nPlayerShopNum-1];
	__LEAVE_FUNCTION
	return -1;
}

BOOL PlayerShopManager::RemovePlayerShop(INT iShopIndex)
{
	__ENTER_FUNCTION

	if( iShopIndex<0 || iShopIndex>=MAX_SHOP_NUM_PER_SCENE )
	{
		Assert(0);
		return FALSE;
	}

	m_nPlayerShopNum--;
	Assert(m_nPlayerShopNum>=0);
	UpdateComFactor();

	m_pFreeItemID[m_nPlayerShopNum]	=	iShopIndex;

	//在池中回收
	g_pPlayerShopPool->DeleteObj(m_pPlayerShopList[iShopIndex]);
	m_pPlayerShopList[iShopIndex] = NULL;

	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

PlayerShop*	PlayerShopManager::GetPlayerShopByIndex(INT iShopIndex)
{
	__ENTER_FUNCTION
	if(iShopIndex>=0 && (UINT)iShopIndex<MAX_SHOP_NUM_PER_SCENE)
	{
		return m_pPlayerShopList[iShopIndex];
	}
	return NULL;
	__LEAVE_FUNCTION
	return NULL;
}

PlayerShop*	PlayerShopManager::GetPlayerShopByGUID(_PLAYERSHOP_GUID ShopGuid)
{
	__ENTER_FUNCTION
	for(UINT i = 0; i<MAX_SHOP_NUM_PER_SCENE; i++)
	{
		if(!m_pPlayerShopList[i])
		{
			continue;
		}
		if(m_pPlayerShopList[i]->GetShopGUID() == ShopGuid)
		{
			return m_pPlayerShopList[i];
		}
	}
	return NULL;
	__LEAVE_FUNCTION
	return NULL;

}

BOOL PlayerShopManager::RemovePlayerShopByGuid(_PLAYERSHOP_GUID ShopGuid)
{
	__ENTER_FUNCTION
	for(UINT i = 0; i<MAX_SHOP_NUM_PER_SCENE; i++)
	{
		if(!m_pPlayerShopList[i])
		{
			continue;
		}
		if(m_pPlayerShopList[i]->GetShopGUID() == ShopGuid)
		{
			return RemovePlayerShop(i);
		}
	}
	return NULL;
	__LEAVE_FUNCTION
	return NULL;
}

BOOL PlayerShopManager::HeartBeat(UINT uTime)
{
	__ENTER_FUNCTION
	//定时扣除营业税
	for(UINT i = 0; i<MAX_SHOP_NUM_PER_SCENE; i++ )
	{
		if(m_pPlayerShopList[i] != NULL)
		{
			_MY_TRY
			{
				if(m_pPlayerShopList[i]->GetShopStatus() == PLAYER_SHOP::STATUS_PLAYER_SHOP_OPEN)
				{//开张的商店，跑心跳
					m_pPlayerShopList[i]->HeartBeat(uTime, m_CommerceFactor);
				}
				else if(m_pPlayerShopList[i]->GetShopStatus() == PLAYER_SHOP::STATUS_PLAYER_SHOP_CLOSE)
				{//打烊的

				}
				else if(m_pPlayerShopList[i]->GetShopStatus() == PLAYER_SHOP::STATUS_PLAYER_SHOP_SHUT_DOWN)
				{//已被系统关闭的商店，系统回收
					RemovePlayerShop(i);
				}
				else if(m_pPlayerShopList[i]->GetShopStatus() == PLAYER_SHOP::STATUS_PLAYER_SHOP_ON_SALE)
				{//已盘出的商店
					m_pPlayerShopList[i]->HeartBeat(uTime, m_CommerceFactor);
				}
			}
			_MY_CATCH
			{
				SaveCodeLog( ) ;
			}
		}
	}

	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;
}

FLOAT PlayerShopManager::UpdateComFactor()
{
	__ENTER_FUNCTION
	FLOAT CurComFact = 0.5;
	FLOAT fPlayerShopNum = (FLOAT)m_nPlayerShopNum;
	if(fPlayerShopNum<200)
	{
		CurComFact += (FLOAT)(fPlayerShopNum*0.002);
	}
	else if(fPlayerShopNum>=200 && fPlayerShopNum<250)
	{
		CurComFact += (FLOAT)(200*0.002);
		CurComFact += (FLOAT)((fPlayerShopNum-200)*0.003);
	}
	else if(fPlayerShopNum>=250)
	{
		CurComFact += (FLOAT)(200*0.002);
		CurComFact += (FLOAT)(50*0.003);
		CurComFact += (FLOAT)((fPlayerShopNum-250)*0.004);
	}

	m_CommerceFactor = CurComFact;
	return m_CommerceFactor;
	__LEAVE_FUNCTION
	return -1;
}


VOID PlayerShopManager::ClampComFactor()
{
	__ENTER_FUNCTION

	INT iComFactor		= (INT)(m_CommerceFactor*100);
	m_CommerceFactor	= ((FLOAT)iComFactor)/100;

	__LEAVE_FUNCTION
}
