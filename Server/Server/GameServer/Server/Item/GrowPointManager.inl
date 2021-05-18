/********************************************************************
	创建日期:	2005年11月1日
	创建时间:	10:37
	文件名称:	GrowPointManager.inl
	文件路径:	d:\Prj\Server\Server\Item\GrowPointManager.inl
	
	文件功能： GrowPointManager 内联实现
	修改纪录：
	
*********************************************************************/

#ifndef _GROWPOINT_MANAGER_INL_
#define _GROWPOINT_MANAGER_INL_



inline	VOID	TypeGrowPointManager::AddData(GROW_POINT_GROUP_SCENE_DATA& RefData)
{
	if(!m_pData)
		m_pData = new GROW_POINT_GROUP_SCENE_DATA[m_TypeCount];
	//最大数量小于	m_TypeCount

	Assert(m_AddOffSet<m_TypeCount);

	m_pData[m_AddOffSet] = RefData;
	m_AddOffSet++;
}


inline	BOOL	TypeGrowPointManager::CreateGrowPointPos(FLOAT& x,FLOAT& y)
{

	if(m_CurrentCount>=m_MaxApperaCount)
		return FALSE;
	
	for(int i = 0;i<m_MaxApperaCount;i++)
	{
		if(!m_pData[m_TypeOffSet].Used)
		{	
			x = m_pData[m_TypeOffSet].GrowPointX;
			y = m_pData[m_TypeOffSet].GrowPointY;

			m_pData[m_TypeOffSet].Used	 = TRUE;
			IncCurrentCount();

			m_TypeOffSet++;
			if(m_TypeOffSet==m_TypeCount)
			{
				m_TypeOffSet = 0;
			}
			return TRUE;
		}
		else
		{
			m_TypeOffSet++;
			if(m_TypeOffSet==m_TypeCount)
			{
				m_TypeOffSet = 0;
			}
		}
	}

	return FALSE;
	
	

}

inline BOOL	TypeGrowPointManager::ReleaseGrowPointPos(FLOAT x,FLOAT y)
{
	
	for(int i=0;i<m_TypeCount;i++)
	{
		if((fabs(m_pData[i].GrowPointX-x)<0.01f)
			&& (fabs(m_pData[i].GrowPointY-y)<0.01f))

		{
			m_pData[i].Used = FALSE;
			DecCurrentCount();
			return TRUE;

		}
	}

	return FALSE;	
}

inline	VOID			TypeGrowPointManager::SetCount(INT count) 
{
		m_TypeCount = count;
}
inline	INT				TypeGrowPointManager::GetCount()
{
		return	m_TypeCount;
}

inline	VOID		TypeGrowPointManager::IncCount()
{
		m_TypeCount++;
}

inline	VOID		TypeGrowPointManager::IncCurrentCount()
{
		m_CurrentCount++;
		Assert(m_CurrentCount<=m_MaxApperaCount);
}

inline	VOID		TypeGrowPointManager::DecCurrentCount()
{
		m_CurrentCount--;
		Assert(m_CurrentCount>=0);
}

inline	WORD		TypeGrowPointManager::GetGrowPointType()
{
	return		m_GrowPointType;
}

inline	VOID		TypeGrowPointManager::SetGrowPointType(WORD gpType)
{
	m_GrowPointType = gpType;
}

inline	VOID		TypeGrowPointManager::SetIntervalTime(INT iInterval)
{
	m_IntervalPerSeed = iInterval;
}

inline	BOOL		TypeGrowPointManager::DoTicks(UINT uTime)
{
	

	if(!m_bEnable)
		return FALSE;

	if(m_bStartCount)
	{
		if(m_CurrentCount==m_MaxApperaCount)
		{
			m_bStartCount = FALSE;
			m_CurrentElapse = 0;
		}
		else
		{
			m_CurrentElapse=m_CurrentElapse+ (uTime -m_LastUpdateTime);
			m_LastUpdateTime =uTime;
		}
	}
	else
	{
		if(m_CurrentCount<m_MaxApperaCount)
		{
			m_bStartCount		=	TRUE;
			m_LastUpdateTime	=	uTime;
			m_CurrentElapse		=	0;
		}

		return FALSE;
	}

	if(m_CurrentElapse>m_IntervalPerSeed)
	{
		m_CurrentElapse = 0;
		return TRUE;
	}
	return FALSE;
}


inline	TypeGrowPointManager*		GrowPointManager::GetGpManagerByGrowPointType(INT GrowPointType)
{
	for(INT i =0;i<m_GrowPointTypeCount;i++)
	{
		if(m_GrowPointMgr[i])
		{
			if(m_GrowPointMgr[i]->GetGrowPointType()== GrowPointType)
			{
				return m_GrowPointMgr[i];
			}
		}
		else
			return NULL;
	}
	return NULL;
}

#endif