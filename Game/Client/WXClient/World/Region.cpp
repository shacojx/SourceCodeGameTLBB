// Region.cpp
// 
//////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Region.h"
#include "Path.h"
#include "TDUtil.h"
#include "Type.h"
#include <math.h>


CRegion::CRegion( VOID )
{
	m_nCount		= 0;
	m_RegionID		= INVALID_ID;
	m_nPassRegionLevel = 0;
}

CRegion::~CRegion( VOID )
{
}

BOOL CRegion::AddPoint( const fVector2 *pfvPoint )
{
	m_vfPoints.push_back( *pfvPoint );
	m_nCount = (INT)m_vfPoints.size();	
	return true;
}

BOOL CRegion::IsBulge( INT nIndex )const
{
	if ( ( nIndex < 0 ) || ( nIndex >= (INT)m_vbBulge.size() ) )
	{
		return FALSE;
	}
	return m_vbBulge[nIndex];
}

BOOL CRegion::JudgeBulge( INT nIndex )
{
	if ( nIndex >= m_nCount )
	{
		return FALSE;
	}
	if( m_nCount < 3 )
	{
		//多边形不合法
		return FALSE;
	}
	if ( m_nCount == 3 )
	{
		//三角形情况
		return true;
	}

	INT nIndex1, nIndex2, nIndex3;
	nIndex1 = nIndex - 1;
	nIndex2 = nIndex;
	nIndex3 = nIndex + 1;
	if ( nIndex1 < 0 )
	{
		nIndex1 = (INT)(m_vfPoints.size()) - 1;
	}
	if ( nIndex3 >= (INT)(m_vfPoints.size()) )
	{
		nIndex3 = 0;
	}

	fVector2 fvLine1, fvLine2;
	fvLine1.x = m_vfPoints[nIndex1].x - m_vfPoints[nIndex2].x;
	fvLine1.y = m_vfPoints[nIndex1].y - m_vfPoints[nIndex2].y;
	fvLine2.x = m_vfPoints[nIndex3].x - m_vfPoints[nIndex2].x;
	fvLine2.y = m_vfPoints[nIndex3].y - m_vfPoints[nIndex2].y;
	FLOAT fAngle1 = atan( (-fvLine1.y) / fvLine1.x );
	FLOAT fAngle2 = atan( (-fvLine2.y) / fvLine2.x );

	if ( fvLine1.x < 0.f )
	{
		fAngle1 += (FLOAT)TDU_PI;
	}

	if ( fvLine2.x < 0.f )
	{
		fAngle2 += (FLOAT)TDU_PI;
	}

	if ( fAngle1 < 0.f )
		fAngle1 = (FLOAT)TDU_PI * 2 + fAngle1;

	if ( fAngle2 < 0.f )
		fAngle2 = (FLOAT)TDU_PI * 2 + fAngle2;

	FLOAT fAngle = fAngle2 - fAngle1;
	if ( fAngle >= 0.f )
	{
		if ( fAngle < (FLOAT)TDU_PI )
			return true;
		else
			return FALSE;
	}
	else
	{
		if ( fAngle < (FLOAT)(-TDU_PI) )
			return true;
		else
			return FALSE;
	}

	return FALSE;
}

//BOOL CRegion::JudgeInside( const fVector2 *pfvPoint )
//{
//	FLOAT fAxisX = pfvPoint->x;
//	FLOAT fAxisY = pfvPoint->y;	
//	fVector2 sp,ep;
//	INT i, nPassBy;
//	nPassBy = 0;
//	for ( INT i = 0; i < m_nCount; i++ )
//	{
//		sp = m_vfPoints[i];
//		ep = m_vfPoints[(i+1)%m_nCount];
//	}
//	if ( nPassBy % 2 == 0)
//	{		
//		return FALSE;
//	}	
//	return true;
//}

VOID CRegion::JudgeAllBulge( VOID )
{
	m_vbBulge.clear();
	INT i;
	BOOL bBulge;
	for ( i = 0; i < (INT)m_vfPoints.size(); i++ )
	{
		bBulge = JudgeBulge( i );
		m_vbBulge.push_back( bBulge );
	}
}

INT CRegion::GetIndex( const fVector2 *pfvPoint )const
{
	for ( INT i = 0; i < (INT)m_vfPoints.size(); i++ )
	{
		if ( ( abs( pfvPoint->x - m_vfPoints[i].x ) < 1e-10 )
			&& ( abs( pfvPoint->y - m_vfPoints[i].y ) < 1e-10 ) )
		{
			return i;
		}
	}
	return -1;
}
INT CRegion::GetNeighborBulgeIndex( INT nIndex, BOOL bCountSelf, BOOL bClockwise )const
{
	if ( ( m_vfPoints.empty() ) || ( m_vbBulge.empty() ) )
	{
		return -1;
	}

	if ( ( nIndex < 0 ) || ( nIndex >= (INT)m_vfPoints.size() ) )
	{
		return -1;
	}

	INT ret = (bCountSelf)?(nIndex):( (bClockwise)?(nIndex+1):(nIndex-1) );

	INT i, nSize;
	nSize = (INT)m_vfPoints.size();
	for ( i = 0; i < nSize; i++ )
	{
		if ( ret >= nSize )
		{
			ret = 0;
		}
		if ( ret < 0 )
		{
			ret = nSize - 1;
		}
		if ( m_vbBulge[ret] )
		{
			return ret;
		}
		(bClockwise)?(ret++):(ret--);

	}
	return -1;
}

INT CRegion::GetNeighborBulgeIndex( const fVector2 *pfvPoint, BOOL bCountSelf, BOOL bClockwise )const
{
	INT nIndex = GetIndex( pfvPoint );
	if ( nIndex >= 0 )
	{
		return GetNeighborBulgeIndex( nIndex, bCountSelf, bClockwise );
	}
	return -1;
}

const fVector2* CRegion::GetNeighborBulge( INT nIndex, BOOL bCountSelf, BOOL bClockwise )const
{
	INT ret = GetNeighborBulgeIndex( nIndex, bCountSelf, bClockwise );
	if ( ret >= 0 )
	{
		return &m_vfPoints[ret];
	}
	return NULL;	
}

const fVector2* CRegion::GetNeighborBulge( const fVector2 *pfvPoint, BOOL bCountSelf, BOOL bClockwise )const
{
	INT nIndex = GetIndex( pfvPoint );
	if ( nIndex >= 0 )
	{
		return GetNeighborBulge( nIndex, bCountSelf, bClockwise );
	}
	return NULL;
}

const fVector2* CRegion::GetPoint( INT nIndex, BOOL bLoop )const
{
	if ( m_vfPoints.empty() )
	{
		return NULL;
	}
	if ( bLoop )
	{
		nIndex = nIndex % (INT)m_vfPoints.size();
	}
	else if ( nIndex >= (INT)m_vfPoints.size() )
	{
		return NULL;
	}
	return &m_vfPoints[nIndex];
}

const fVector2* CRegion::GetPoint( INT nIndex )const
{
	return GetPoint( nIndex, true );
}


