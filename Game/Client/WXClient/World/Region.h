#ifndef __REGION_H__
#define __REGION_H__

#pragma once

#include <vector>
#include "TDMath.h"

class tEntityNode;

class CRegion
{
public:
	CRegion( VOID );
	~CRegion( VOID );

	BOOL AddPoint( const fVector2 *pfvPoint );
	BOOL IsBulge( INT nIndex )const;
	BOOL JudgeBulge( INT nIndex );
	VOID JudgeAllBulge( VOID );

	//得到一个顶点的序号,如果这个顶点不在多边形上,返回-1
	INT GetIndex( const fVector2 *pfvPoint )const;

	// 得到相邻的凸点
	INT GetNeighborBulgeIndex( INT nIndex, BOOL bCountSelf, BOOL bClockwise )const;
	INT GetNeighborBulgeIndex( const fVector2 *pfvPoint, BOOL bCountSelf, BOOL bClockwise )const;
	const fVector2* GetNeighborBulge( INT nIndex, BOOL bCountSelf, BOOL bClockwise )const;
	const fVector2* GetNeighborBulge( const fVector2 *pfvPoint, BOOL bCountSelf, BOOL bClockwise )const;

	const fVector2* GetPoint( INT nIndex, BOOL bLoop )const;
	const fVector2* GetPoint( INT nIndex )const;

public:
	INT						m_RegionID;
	INT						m_nCount;
	INT						m_nPassRegionLevel;// 可通过区域的 行走等级 fujia 2007.12.27
	std::vector<fVector2>	m_vfPoints;
	std::vector<BOOL>		m_vbBulge;
};

#endif 
