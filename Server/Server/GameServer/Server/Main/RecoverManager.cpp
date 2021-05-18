#include "stdafx.h"

#include "RecoverManager.h"
#include "GUIDManager.h"
#include "PlayerPool.h"
#include "Obj_Human.h"



RecoverManager g_RecoverManager;


BOOL RecoverManager::Init()
{

	__ENTER_FUNCTION

	m_Lock.Lock();

	for(uint i=0;i<MAX_POOL_SIZE;i++)
	{
		GamePlayer* pPlayer = g_pPlayerPool->GetPlayer(i);
		
		if(pPlayer->GetShareMemStatus())
		{
			
			g_pPlayerPool->NewPlayer(i);
			AddPlayer(pPlayer);
		}

	}
	
	m_Lock.Unlock();
	return TRUE;

	__LEAVE_FUNCTION

	m_Lock.Unlock();
	return FALSE;
}



BOOL RecoverManager::Tick()	
{
	__ENTER_FUNCTION

	MySleep(100) ;


	if(m_Count==0)
		return TRUE;

	

	for(uint i=0;i<m_Count;i++)
	{
		GamePlayer* pPlayer = m_PlayerPtr[i];
		Assert(pPlayer);

		if(pPlayer->ShareMemSafe()) //数据库保存完毕
		{
			

		}

	}

	__LEAVE_FUNCTION

	return TRUE;
}

BOOL RecoverManager::AddPlayer(GamePlayer* pPlayer)
{
	__ENTER_FUNCTION

	m_Lock.Lock();

	Obj_Human* pHuman =pPlayer->GetHuman();
	Assert(pHuman);
	pPlayer->m_HumanGUID	 = pHuman->GetDB()->GetHumanDB()->m_GUID;
	m_PlayerPtr[m_Count] = pPlayer;
	m_Count++;
	m_Lock.Unlock();
	return TRUE;
	__LEAVE_FUNCTION
	m_Lock.Unlock();
	return FALSE;
}

BOOL	RecoverManager::RemovePlayer(GUID_t guid)
{
	__ENTER_FUNCTION
	m_Lock.Lock();
	
	for(uint i=0;i<m_Count;i++)
	{
		GamePlayer* pPlayer = m_PlayerPtr[i];
		Assert(pPlayer);

		if(pPlayer->m_HumanGUID == guid)
		{
			
			m_PlayerPtr[i]=m_PlayerPtr[m_Count-1];
			m_PlayerPtr[m_Count-1] = 0;
			m_Count--;
			m_Lock.Unlock();
			return TRUE;
		}
	}

	m_Lock.Unlock();
	return FALSE;
	__LEAVE_FUNCTION
	m_Lock.Unlock();
	return FALSE;
}