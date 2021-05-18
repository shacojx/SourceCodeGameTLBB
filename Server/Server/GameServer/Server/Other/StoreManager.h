/********************************************************************
	创建日期:	2005年11月1日
	创建时间:	13:10
	文件名称:	StoreManager.h
	文件路径:	d:\Prj\Server\Server\Obj\StoreManager.h
	
	文件功能： 藏宝点管理器
			   每个场景携带一个藏宝点管理器
	修改纪录：
	
*********************************************************************/

#ifndef _STORE_MANAGER_H_
#define _STORE_MANAGER_H_
#include "Type.h"



//每个场景最大管理藏宝点个数
#define MAX_MANAGERSTOREPOINT 10


//藏宝点数据
struct STORE_POINT
{
	WORLD_POS	m_Pos;				//藏宝点世界坐标
	INT			m_Type;				//藏宝点类型----〉
};

class	StoreManager
{
	
	INT						m_Count;								//藏宝点个数
	STORE_POINT				m_StorePoint[MAX_MANAGERSTOREPOINT];	//藏宝点数据


public:
	StoreManager();
	
	/*
	 *	添加藏宝点
	 *
	 *	入口：
	 *		const STORE_POINT* point		藏宝点指针
	 *	返回：
	 *		BOOL							0 代表失败		1 表示成功
	 */
	BOOL					AddStorePoint(const STORE_POINT* point);
	 /*
	  *	删除藏宝点
	  *
	  *		参数说明：
	  *			INT iIndex	藏宝点编号
	  *		
	  *		返回	 
				BOOL		0表示失败	1表示成功
	  *
	  *
	  */
	BOOL					DelStorePoint(INT iIndex);

	/*	查找藏宝点
	 *	
	 *	 参数说明：
	 *
	 *		WORLD_POS& pos			世界坐标
	 *	
	 *		INT& index				找到后返回的编号,没找到返回-1
	 *		
	 *	 返回
	 *		STORE_POINT*			
	 *								找到了返回藏宝点指针，没找到返回NULL
	 *
	 *
	 */
	STORE_POINT*			FindStorePointByPos(WORLD_POS& pos,INT& index);

	/*	
	 *	带误差的查找藏宝点
	 *	
	 *	 参数说明：
	 *
	 *		WORLD_POS& pos			世界坐标
	 *	
	 *		INT& index				找到后返回的编号,没找到返回-1
	 *		
	 *		FLOAT	fRange			误差范围设置
	 *		
	 *	 返回
	 *		STORE_POINT*			
	 *								找到了返回藏宝点指针，没找到返回NULL
	 *
	 *
	 */

	BOOL					FindStorePointByPosRange(WORLD_POS& pos,INT& index,FLOAT fRange);
	/*
	 *	获得藏宝点类型
	 *	
	 *		INT		index			藏宝点编号
	 *
	 *		返回：
	 *		INT						类型
	 */
	INT						GetStorePointType(INT index);

	/*
	 *  获得藏宝点位置
	 *
	 */
	WORLD_POS				GetStorePointPos(INT index);

};







#endif