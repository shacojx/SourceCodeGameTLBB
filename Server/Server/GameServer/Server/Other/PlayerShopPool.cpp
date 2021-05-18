#include "stdafx.h"
/************************************************************************/
/*
创建日期:	2006年2月13日
创建时间:	11:54
文件名称:	PlayerShopPool.h
文件路径:	d:\Prj\Server\Server\Other\PlayerShopPool.cpp

文件功能：	玩家商店池
修改纪录：
*/
/************************************************************************/

#include "PlayerShopPool.h"
#include "SceneManager.h"
#include "WorldManager.h"
#include "ServerManager.h"

PlayerShopPool		*g_pPlayerShopPool = NULL;

PlayerShopPool::PlayerShopPool()
{
	m_nFreePosition = 0;
	m_pPlayerShop	= NULL ;
	m_pFreeItemID	= NULL ;
	m_WorldId		= INVALID_ID;
	m_ServerId		= INVALID_ID;
}

PlayerShopPool::~PlayerShopPool()
{
	SAFE_DELETE_ARRAY( m_pPlayerShop );
	SAFE_DELETE_ARRAY( m_pFreeItemID );
}

BOOL PlayerShopPool::Init(UINT nPoolSize)
{
	__ENTER_FUNCTION

	m_WorldId	=	g_pWorldManager->GetWorldID();
	m_ServerId	=	g_pServerManager->GetServerID();

	if(nPoolSize > MAX_PLAYER_SHOP_POOL_PER_SERVER)
		return FALSE;

	m_PoolSize = nPoolSize;

	m_pPlayerShop	=	new	PlayerShop[m_PoolSize];

	Assert(m_pPlayerShop);
	for(INT i = 0;i<(INT)m_PoolSize;i++)
	{
		//初始化池中每个商店
		m_pPlayerShop[i].Init();
	}

	m_pFreeItemID	= new UINT[m_PoolSize];
	Assert(m_pFreeItemID);

	for(UINT i = 0;i<(INT)m_PoolSize;i++)
		m_pFreeItemID[i]	=	i;
	
	//初始化分配数组
	INT iMaxIndex = 0;
	INT	iTempIndex = 0;
	INT	TempShopIndex[MAX_PLAYER_SHOP_POOL_PER_SERVER] = {0};
	for(INT i = 0; i<(INT)m_PoolSize; i++ )
	{
		if(!m_pPlayerShop[i].GetShopGUID().isNull())
		{
			iMaxIndex = i+1;
		}
		else
		{
			TempShopIndex[iTempIndex++] = i;
		}
	}
	for(INT i = 0; i<(INT)iTempIndex; i++ )
	{
		if(TempShopIndex[i]<=iMaxIndex)
		{
			m_pFreeItemID[iMaxIndex-i-1] = TempShopIndex[i];
			m_nFreePosition = iMaxIndex-i;
		}
	}

	//
	//sharemem导入m_pPlayerShop[]，m_pFreeItemID[]，m_nFreePosition
	//

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

VOID PlayerShopPool::CleanUp()
{
	return;
}

PlayerShop*	PlayerShopPool::NewPlayerShop(ID_t idScene)
{
	__ENTER_FUNCTION

	m_Lock.Lock();
	Assert(m_nFreePosition<m_PoolSize);
	PlayerShop& PS = m_pPlayerShop[m_pFreeItemID[m_nFreePosition]];

	PS.CleanUp();

	//唯一能生成商店GUID的地方
	_PLAYERSHOP_GUID	NewGUID;
	NewGUID.m_World		= m_WorldId;
	NewGUID.m_Server	= m_ServerId;
	NewGUID.m_Scene		= idScene;
	NewGUID.m_PoolPos	= m_pFreeItemID[m_nFreePosition];
	PS.SetShopGUID(NewGUID);

	m_nFreePosition++;

	m_Lock.Unlock();

	return &PS;

	__LEAVE_FUNCTION

	m_Lock.Unlock();
	return NULL ;
}

VOID PlayerShopPool::DelPlayerShop(UINT uShopPos)
{
	__ENTER_FUNCTION

	m_Lock.Lock();

	Assert(	uShopPos < m_PoolSize);

	PlayerShop* pPlayerShop = &m_pPlayerShop[uShopPos];

	m_nFreePosition--;
	Assert(m_nFreePosition>=0);

	m_pFreeItemID[m_nFreePosition]	=	uShopPos;
	pPlayerShop->CleanUp();
	m_Lock.Unlock();
	return ;
	__LEAVE_FUNCTION
	m_Lock.Unlock();
}

VOID PlayerShopPool::DeleteObj(PlayerShop* pPlayerShop)
{
	__ENTER_FUNCTION

	m_Lock.Lock();
	INT nPoolPos = pPlayerShop->GetPoolPos();
	Assert(	 nPoolPos < (INT)m_PoolSize && nPoolPos >= 0 );
	m_nFreePosition--;
	Assert(m_nFreePosition>=0);
	m_pFreeItemID[m_nFreePosition]	= nPoolPos;
	pPlayerShop->CleanUp();
	m_Lock.Unlock();
	return ;

	__LEAVE_FUNCTION
	m_Lock.Unlock();
}


BOOL PlayerShopPool::ConnectTo(SceneManager*	pSceneManager)
{
	__ENTER_FUNCTION

	//单线称调用，不用锁
	PlayerShop* pShop = NULL;
	Scene* pScene = NULL;
	SceneID_t sceneid;

	for(UINT i = 0; i<m_PoolSize; i++)
	{
		pShop	 = &m_pPlayerShop[i];
		sceneid	 = pShop->GetSceneID();

		if( sceneid != INVALID_ID)
		{//有主了
			pScene = pSceneManager->GetScene(sceneid);
			if (!pScene)
			{
				continue;
			}
			pScene->GetPlayerShopManager()->AddPlayerShop(pShop);
		}
	}

	return TRUE;

	__LEAVE_FUNCTION
	return FALSE;
}

