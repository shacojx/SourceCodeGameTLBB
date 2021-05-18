/****************************************\
*										*
* 			   场景区域Zone				*
*										*
\****************************************/
#pragma once
#include "Region.h"
#include "..\Object\Object.h"
#include "TDObjectSystem.h"
/*
|
|	o 将整个游戏场景分割成固定大小的网格，每个网格单位为一个Zone，
|     Zone的大小跟一个屏幕的大小类似
|
|   o 继承于MapObject的物体都会注册到自己所在的Zone,每个Zone都有
|     一个链表，保存注册到该网格的物体
|
|   o 在Zone中注册的数据还有"不可行走区域" Region，这样从
|     Zone可以快速找到附近的Region
|
|
*/

class CObject_Map;
class CZone
{
public:
	CZone();
	virtual ~CZone();

	//Zone初始化，用于所属的Map初始化时调用
	VOID		Initial(VOID);
	//将某一Object注册到自身，该Object必须继承于Object_Map
	VOID		RegisterObject(CObject_Map* pObject);
	//将某一Object反注册
	VOID		UnRegisterObject(CObject_Map* pObject);

public:
	std::set< INT >* GetRigisterObject(){ return &m_setRegisterObject; };
	std::list< INT >* GetListRegion(){ return &m_listRegion; };
protected:
	//注册到该Zone上的Object
	std::set< INT >		m_setRegisterObject;

	//经过该Zone的Region
	std::list< INT >	m_listRegion;
	
	VOID AddRegion(INT id);

	friend class CScene;
	friend class CPath;
	friend class CObject;
	friend class CAI_MySelf;
	
};