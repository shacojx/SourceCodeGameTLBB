#ifndef __PATH_H__
#define __PATH_H__

#pragma once

#include <vector>
#include <map>
#include "TDMath.h"


class CScene;
class CRegion;
class CPath
{
public:
	struct PathUnit
	{
		fVector2	fvStart;		//起始点
		fVector2	fvTarget;		//目标点
		INT			dwRequestIndex;	//发往服务器的路径编号，用于和服务器之间交流的标示
	};

	
protected:
	// 递归查找行走路径。
    BOOL FindPath(
					fVector2 fvStart,			 // 当前查找的开始点
					BOOL bBegin         = FALSE, // 是否开始查找
					INT iPreRegionIndex = -1,	 // 前一次调用的region索引
					INT iPreLineIndex   = -1,	 // 前一次调用的阻挡线索引
					INT iPreVertexIndex = -1	 // 前一次找到的点。
					);

	// 是否到达终点。
	BOOL IsReachDes(fVector2 fvStart,int level);

	
	// 查找所有区域得到最近的点
	BOOL GetMinCrossPos(
						fVector2 fvStart,		// 直线的开始的点。
						fVector2 fvEnd,			// 直线的结束的点。
						FLOAT& fFindx,			// 找到的相交的点x 
						FLOAT& fFindy,			// 找到的相交点y
						INT& iRegionIndex,      // 找到的区域索引
						INT& iLineIndex,		// 找到的的线的索引
						INT iExcludeRegion = -1 // 不查找的区域
						);

	// 查找两个直线的交点， 并求交点。
	BOOL CheckLineCross(
						fVector2		fv1Start,		// 直线1的开始点。
						fVector2		fv1End,			// 直线1的结束点。
						fVector2		fv2Start,		// 直线2的开始点。
						fVector2		fv2End,			// 直线2的结束点。
						FLOAT&			fcrossx,			// 找到的交点x。
						FLOAT&			fcrossy			// 找到的交点y
						);

	INT	 BuildDijkPath(const fVector2& fvCurrent, const fVector2& fvTarget, int nPassLevel);

	std::vector<PathUnit>		m_vPosStack;		//关键点的集合，每两个点之间是一条路径单位
	fVector2					m_TargetPos;		// 走路的终点
	std::vector<INT>			m_PassVector;		// 起点和终点所经过的所有区域
	INT							m_iScene;
	std::vector<PathUnit>		m_vPosStack1;		//关键点的集合，每两个点之间是一条路径单位

	// 当从一个region走到另一个region时， 下面两个变量记录的是前一个region和阻挡线的索引。
	INT							m_iChangeRegion;
	INT							m_iChangeLine;


public:
	CPath();
	virtual ~CPath();

	static INT			RequestIndexFactory(VOID);
	static BOOL			CheckCross(fVector2 fvmStart, fVector2 fvmEnd,const CRegion* pcr);
	static BOOL			CheckCross(fVector2 fvmStart, fVector2 fvmEnd,fVector2 fvnStart, fVector2 fvnEnd);
	static fVector2		fvSub(fVector2 fv1, fVector2 fv2);
	static FLOAT		fvMulti(fVector2 fv1, fVector2 fv2);
	static BOOL			fvEuqual(const fVector2& fv1, const fVector2& fv2);
	static BOOL			IsPointInUnreachRegion(const fVector2& fvTarget); // 当前点是否在不可行走区域之内

	// 计算从开始点到结束点的路径。
	BOOL				CreateMovePath(const fVector2& fvCurrent, const fVector2& fvTarget);

	//  直接查找目的地。
	BOOL                GetPathDirect(const fVector2& fvCurrent, const fVector2& fvTarget);

	// 查找当前的直线与一个区域是否相交，如果相交返回交点和区域中最近相交直线的索引值
	BOOL	CheckCross(
		fVector2 fvStart,		// 直线的开始点。
		fVector2 fvEnd,			// 直线的结束点。
		const CRegion* pcr,		// 要查找的直线。
		FLOAT& fFindx,			// 找到的交点x。
		FLOAT& fFindY,			// 找到的交点y
		FLOAT& fFindDis,		// 找到的直线的距离
		INT& iFindLineIndex		// 找到的直线的索引
		);

	//取得路径段的数目
	INT					GetUnitNum(VOID)const { return (INT)m_vPosStack.size(); }

	const PathUnit&		GetConstPathUnit(INT nIndex)const;


};

#endif 

