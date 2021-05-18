#ifndef __BUILDINGCOLLISIONMNG_H__
#define __BUILDINGCOLLISIONMNG_H__
#pragma once

#include <map>
#include <string>
#include <vector>

typedef struct _POINT_2D
{
	INT iX;
	INT iY;
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
	_POINT_3D()
	{
	
		fx = 0;
		fy = 0;
		fz = 0;
	}
	FLOAT fx;
	FLOAT fy;
	FLOAT fz;

}POINT_3D, *PPOINT_3D;


typedef struct _TRI_INFO
{
	POINT_3D v1;
	POINT_3D v2;
	POINT_3D v3;

}TRI_INFO, *PTRI_INFO;


// 三角形列表信息。
typedef std::vector<TRI_INFO> TRI_INFO_VECTOR;

// 地图中注册的三角形列表与位置映射表。
typedef std::map<POINT_2D, TRI_INFO_VECTOR, Point_2D_less> TRI_INFO_MAP;



class CBuildingCollisionMng
{
public:
	CBuildingCollisionMng(VOID);
	~CBuildingCollisionMng(VOID);

public:

	// 在地图中注册的三角形映射表。
	TRI_INFO_MAP m_triInfoMap;

	//
	// 从行走面数据文件中读取数据。
	//
	// 参数：行走面数据文件， 包含路径。
	UINT LoadCollisionTriInfoFromFile(const char* szFileName);

	//
	// 通过传入的 平面fx, fz位置得到高度。 
	// 
	// 参数1： FLOAT fx    游戏逻辑坐标。
	//
	// 参数2： FLOAT fz    游戏逻辑坐标。
	//
	// 参数3： FLOAT& fy   游戏逻辑坐标  返回的高度。
	//
	// return： 1 － 成功， fy有意义。
	//		    0 － 失败， fy无意义， 表明此处无建筑物行走面数据。
	BOOL GetHeight(FLOAT fx, FLOAT fz, FLOAT& fy);


	//
	// 通过传入的 平面fx, fz位置得到高度。 
	// 
	// 参数1： FLOAT fx    游戏逻辑坐标。
	//
	// 参数2： FLOAT fz    游戏逻辑坐标。
	//
	// 参数3： FLOAT& fy   游戏逻辑坐标  返回的高度。
	//
	// return： 1 － 成功， fy有意义。
	//		    0 － 失败， fy无意义， 表明此处无建筑物行走面数据。
	BOOL SelBuilding(
						FLOAT fOrigx, FLOAT fOrigy, FLOAT fOrigz,	// 射线的原点。
						FLOAT fDirx,  FLOAT fDiry,  FLOAT fDirz,	// 射线的方向。
						INT	  ix, INT iz,							// 选中地图的位置
						FLOAT& fPosx, FLOAT& fPosy, FLOAT& fPosz	// 返回建筑物的行走面位置。
						);


	BOOL IntersectTriangle(  
							POINT_3D Orig,	// 射线的原点。
							POINT_3D Dir,	// 射线的方向。
							POINT_3D v1,
							POINT_3D v2,
							POINT_3D v3,
							FLOAT* t, 
							FLOAT* u, 
							FLOAT* v 
							);


	//
	//  清空数据。
	//
	VOID ClearData();


};

#endif //__BUILDINGCOLLISIONMNG_H__