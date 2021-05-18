#include "stdafx.h"
/*
摊位信息管理器，每个场景挂一个。
负责摊位信息文件的读取。
摊位费用的管理，包括第一次摆摊的摊位费，每次交易的交易税，此地是否可以摆摊。
*/

#include "StallInfoManager.h"
#define MAX_STALL_MAP_SIZE	(512)

StallInfoManager::StallInfoManager()
{
	__ENTER_FUNCTION
	m_StallMap		=	NULL;
	m_nMapHeight	=	0;
	m_nMapWidth		=	0;
	__LEAVE_FUNCTION
}

StallInfoManager::~StallInfoManager()
{
	__ENTER_FUNCTION
	CleanUp();
	__LEAVE_FUNCTION
}

BOOL		StallInfoManager::Init(UINT uMapWidth, UINT uMapHeight)
{
	__ENTER_FUNCTION

	if( uMapWidth>MAX_STALL_MAP_SIZE || uMapHeight>MAX_STALL_MAP_SIZE )
		return FALSE;

	//都等于0代表所有清空
    if(uMapWidth == 0 && uMapHeight == 0)
	{
		return TRUE;
	}

	//非零，new内存
	m_nMapHeight	=	uMapHeight;
	m_nMapWidth		=	uMapWidth;

	m_StallMap = new StallInfoData_t*[uMapHeight];
	if( m_StallMap == NULL )
	{
		Assert(0);
		return FALSE;
	}
	for(UINT i = 0; i<uMapHeight; i++)
	{
		m_StallMap[i] = new StallInfoData_t[uMapWidth];
		if ( m_StallMap[i] == NULL )
		{
			Assert(0);
			return FALSE;
		}
		memset(m_StallMap[i], 0, uMapWidth*sizeof(StallInfoData_t));
	}

	return TRUE;
	__LEAVE_FUNCTION
	return FALSE;

}

VOID		StallInfoManager::CleanUp()
{
	__ENTER_FUNCTION
	
	if(m_StallMap == NULL || m_nMapHeight == 0 || m_nMapWidth == 0)
		return;

	for(UINT i =0; i<m_nMapHeight; i++)
	{
		SAFE_DELETE_ARRAY(m_StallMap[i])
	}
	SAFE_DELETE_ARRAY(m_StallMap);
	m_nMapHeight = 0;
	m_nMapWidth  = 0;

	__LEAVE_FUNCTION
}

BOOL		StallInfoManager::Load( CHAR* filename ) 
{
	__ENTER_FUNCTION

	UINT	CurVersion = 1;
	FILE*	pFile = NULL;

	UINT	dwVersion = 0;
	UINT	dwStallWidth = 0;
	UINT	dwStallHeight = 0;

	pFile = ::fopen(filename, "r");
	if(NULL == pFile)
	{
		return FALSE;
	}

	// 读取版本号
	::fread(&dwVersion, sizeof(dwVersion), 1, pFile);
	// 版本不对
	if(dwVersion!=CurVersion)
	{
		::fclose(pFile);
		return FALSE;
	}

	// 摊位的大小
	::fread(&dwStallWidth,  sizeof(dwStallWidth),  1, pFile);
	::fread(&dwStallHeight, sizeof(dwStallHeight), 1, pFile);

	if(dwStallWidth != m_nMapWidth || dwStallHeight != m_nMapHeight)
	{//摊位文件的地图大小与实际地图大小不一样。
		Assert(0);
		::fclose(pFile);
		return FALSE;
	}

    //地图没有被初始化
	if(NULL == m_StallMap)
	{
		::fclose(pFile);
		return FALSE;
	}

	for(UINT i = 0; i < dwStallHeight; i++)
	{
		for(UINT j = 0; j < dwStallWidth; j++)
		{
			StallInfoData_t&	stallInfo =  m_StallMap[j][i];
			::fread(&(stallInfo.m_bCanStall),  sizeof(stallInfo.m_bCanStall),  1, pFile);
			::fread(&(stallInfo.m_iTradeTax),  sizeof(stallInfo.m_iTradeTax),  1, pFile);
			::fread(&(stallInfo.m_dwPosTax),   sizeof(stallInfo.m_dwPosTax),   1, pFile);
			::fread(&(stallInfo.m_iExtraInfo), sizeof(stallInfo.m_iExtraInfo), 1, pFile);
		}
	}
	::fclose(pFile);
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL		StallInfoManager::CanStall(UINT pos_x, UINT pos_y)
{
	__ENTER_FUNCTION

	if(m_StallMap == NULL || m_nMapHeight == 0 || m_nMapWidth == 0)
		return FALSE;

	if(pos_x < m_nMapHeight && pos_y < m_nMapWidth)
	{
		return m_StallMap[pos_x][pos_y].m_bCanStall;
	}
	__LEAVE_FUNCTION
	return FALSE;

}

BYTE		StallInfoManager::StallExchangeTax(UINT pos_x, UINT pos_y)
{
	__ENTER_FUNCTION

	if(m_StallMap == NULL || m_nMapHeight == 0 || m_nMapWidth == 0)
		return 0;

	if(pos_x < m_nMapHeight && pos_y < m_nMapWidth)
	{
		return m_StallMap[pos_x][pos_y].m_iTradeTax;
	}
	__LEAVE_FUNCTION
	return 0;
}

UINT		StallInfoManager::StallPosPayment(UINT pos_x, UINT pos_y)
{
	__ENTER_FUNCTION
	if(m_StallMap == NULL || m_nMapHeight == 0 || m_nMapWidth == 0)
		return 0;

	if(pos_x < m_nMapHeight && pos_y < m_nMapWidth)
	{
		return m_StallMap[pos_x][pos_y].m_dwPosTax;
	}
	__LEAVE_FUNCTION
	return 0;
}

BYTE		StallInfoManager::StallExtraInfo(UINT pos_x, UINT pos_y)
{
	__ENTER_FUNCTION
	if(m_StallMap == NULL || m_nMapHeight == 0 || m_nMapWidth == 0)
		return 0;

	if(pos_x < m_nMapHeight && pos_y < m_nMapWidth)
	{
		return m_StallMap[pos_x][pos_y].m_iExtraInfo;
	}
	__LEAVE_FUNCTION
	return 0;
}

VOID		StallInfoManager::SetCanStall(UINT pos_x, UINT pos_y, BOOL bIsCanStall)
{
	__ENTER_FUNCTION
	if(m_StallMap == NULL || m_nMapHeight == 0 || m_nMapWidth == 0)
		return;

	if(pos_x < m_nMapHeight && pos_y < m_nMapWidth)
	{
		m_StallMap[pos_x][pos_y].m_bCanStall = bIsCanStall;
	}
	__LEAVE_FUNCTION

}