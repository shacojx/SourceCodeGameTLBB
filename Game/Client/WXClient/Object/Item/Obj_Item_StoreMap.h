#pragma once
#include "Obj_Item.h"
#include "GameDefine2.h"
#include "Obj_Item_Equip.h"
#include "TDDBC_Struct.h"
#include "../ObjectDef.h"
#include "GameStruct_Item.h"

/*
|
|	地图类物品
|
*/
class CObject_Item_StoreMap : public CObject_Item
{
public:
	//得到物品的类别
	virtual ITEM_CLASS		GetItemClass(VOID) const { return (ITEM_CLASS)m_theBaseDef->nClass; }

	// 2006-3-30;
	// 得到quality信息
	virtual INT				GetItemTableQuality(VOID);
	
	// 得到type信息
	virtual INT				GetItemTableType(VOID);


	//物品的名称
	virtual LPCTSTR			GetName(VOID) const			{ return m_theBaseDef->szName; }
	//物品解释
	virtual LPCTSTR			GetDesc(VOID) const			{ return m_theBaseDef->szDesc; }
	//物品的详细编号，不是client用的index
	virtual INT				GetParticularID(VOID) const	{ return m_nParticularID; }
	//图标名
	virtual LPCTSTR			GetIconName(VOID) const;
	//详细解释(可能需要服务器)
	virtual LPCTSTR			GetExtraDesc(VOID);
	//设置详细解释
	virtual VOID			SetExtraInfo(const _ITEM * pItemInfo);
	//克隆详细信息
	virtual VOID			Clone(const CObject_Item * pItemSource);
	//获得物品的最大叠加数量
	virtual	INT				GetMaxOverLay(VOID) const	{return 1;};


public:
	CObject_Item_StoreMap(INT id);
	virtual ~CObject_Item_StoreMap();
	//生成一个地图
	VOID					AsStoreMap(const _DBC_ITEM_STOREMAP* pStoreMapDefine);

protected:
	//--------------------------------------------------------------
	//基本属性
	const _DBC_ITEM_STOREMAP*	m_theBaseDef;			//基本属性定义(数据表)

	STRING						m_strExtraDesc;
	//物品的详细编号
	INT							m_nParticularID;
};
