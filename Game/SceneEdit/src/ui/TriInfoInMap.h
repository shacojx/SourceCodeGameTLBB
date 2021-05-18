#ifndef __TRIINFOINMAP_H__
#define __TRIINFOINMAP_H__
#pragma once

#include <map>
#include <string>
#include <vector>

typedef struct _POINT_2D
{
	int iX;
	int iY;
}POINT_2D, *PPOINT_2D;

class Point_2D_less 
{
public:
	bool operator() ( const POINT_2D& point1, const POINT_2D& point2 ) const
	{
		if(point1.iX < point2.iX)
		{
			return true;
		}
		else if(point1.iX == point2.iX)
		{
			if(point1.iY < point2.iY)
			{
				return true;	
			}
		}

		return false;
	}
};

typedef struct _POINT_3D
{
	float fx;
	float fy;
	float fz;

}POINT_3D, *PPOINT_3D;


typedef struct _POINT_2D_FLOAT
{
	float fx;
	float fy;
	
}POINT_2D_FLOAT, *PPOINT_2D_FLOAT;

typedef struct _SCANLINE_INFO
{
	int iStart;
	int iEnd;
	int iScanLine;

}SCANLINE_INFO, *PSCANLINE_INFO;

typedef struct _TRI_INFO
{
	float f1x;
	float f1y;
	float f1z;

	float f2x;
	float f2y;
	float f2z;

	float f3x;
	float f3y;
	float f3z;

}TRI_INFO, *PTRI_INFO;


// 三角形列表信息。
typedef std::vector<TRI_INFO> TRI_INFO_VECTOR;

// 地图中注册的三角形列表与位置映射表。
typedef std::map<POINT_2D, TRI_INFO_VECTOR, Point_2D_less> TRI_INFO_MAP;


class CTriInfoInMap
{
public:
	CTriInfoInMap(void);
	~CTriInfoInMap(void);

public:

	// 注册在地图中的三角形信息与位置的映射表.
	TRI_INFO_MAP m_triInfoInMap;
};

#endif //__TRIINFOINMAP_H__