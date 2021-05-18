#include "stdafx.h"
#include "StoreManager.h"


StoreManager::StoreManager()
{
		m_Count	=	0;
}

BOOL	StoreManager::AddStorePoint(const STORE_POINT* point)
{
		
	__ENTER_FUNCTION

		Assert(point);
		Assert(m_Count+1<MAX_MANAGERSTOREPOINT);
		m_StorePoint[m_Count++]	=	*point;
		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}


BOOL	StoreManager::DelStorePoint(INT iIndex)
{
	__ENTER_FUNCTION

		//Assert((0<=iIndex)&&(iIndex<m_Count));
		if(!((0<=iIndex)&&(iIndex<m_Count)))
		{
			Assert(FALSE);
			return FALSE;
		}
		m_StorePoint[iIndex]=m_StorePoint[m_Count-1];
		m_Count--;
		return TRUE;

	__LEAVE_FUNCTION
		
		return	FALSE;
}


STORE_POINT*	StoreManager::FindStorePointByPos(WORLD_POS& pos,INT& index)
{
	__ENTER_FUNCTION

	for(INT i = 0;i<m_Count;i++)
	{
		if(m_StorePoint[i].m_Pos==pos)
		{
			index = i;
			return &m_StorePoint[i];
		}
	}
	index = -1;
	return NULL;

	__LEAVE_FUNCTION

	return NULL;
}

BOOL	StoreManager::FindStorePointByPosRange(WORLD_POS& pos,INT& index,FLOAT fRange)
{
	__ENTER_FUNCTION

	for(INT i = 0;i<m_Count;i++)
	{
		if(fabs(m_StorePoint[i].m_Pos.m_fX-pos.m_fX)<fRange
			&&fabs(m_StorePoint[i].m_Pos.m_fZ-pos.m_fZ)<fRange)
		{
			index = i;
			return TRUE;
		}
	}
	index = -1;
	return FALSE;

	__LEAVE_FUNCTION

	return FALSE;
	
}

INT		StoreManager::GetStorePointType(INT	index)
{
	__ENTER_FUNCTION

	Assert(index>=0);
	Assert(index<m_Count);
	return m_StorePoint[index].m_Type;
	
	__LEAVE_FUNCTION

	return -1;
}

WORLD_POS	StoreManager::GetStorePointPos(INT index)
{
	__ENTER_FUNCTION
	
	Assert(index>=0);
	Assert(index<m_Count);
	return m_StorePoint[index].m_Pos;
	__LEAVE_FUNCTION
	WORLD_POS pos;
	return pos;
}
