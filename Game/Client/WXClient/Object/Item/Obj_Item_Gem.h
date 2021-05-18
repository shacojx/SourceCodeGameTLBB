#pragma once
#include "Obj_Item.h"
#include "GameDefine2.h"
#include "Obj_Item_Equip.h"
#include "TDDBC_Struct.h"
#include "../ObjectDef.h"
#include "GameStruct_Item.h"

/*
|
|	宝石类物品
|
*/
class CObject_Item_Gem : public CObject_Item
{
public:



	//得到物品的类别
	virtual ITEM_CLASS		GetItemClass(VOID) const { return (ITEM_CLASS)m_theBaseDef->nClass; }
	//物品的名称
	virtual LPCTSTR			GetName(VOID) const			{ return m_theBaseDef->szName; }
	//物品解释
	virtual LPCTSTR			GetDesc(VOID) const			{ return m_theBaseDef->szDesc; }
	//图标名
	virtual LPCTSTR			GetIconName(VOID) const;
	//详细解释(可能需要服务器)
	virtual LPCTSTR			GetExtraDesc(VOID);
	//设置详细解释
	virtual VOID			SetExtraInfo(const _ITEM * pItemInfo);
	//克隆详细信息
	virtual VOID			Clone(const CObject_Item * pItemSource);
	//宝石级别
	INT						GetQuality(VOID) const			{ return m_theBaseDef->nQuality; }
	//宝石类型
	INT						GetGemType(VOID) const			{ return m_theBaseDef->nType; }	
	//获得物品的最大叠加数量
	virtual	INT				GetMaxOverLay(VOID) const	{return 1;};

	
	// 得到物品卖给npc的价格
	virtual INT				GetItemPrice();
	// 得到宝石的等级
	INT						GetGemLevel();

	// 得到宝石属性信息
	LPCTSTR					GetGemArribInfo();

	// 2006-3-30;
	// 得到quality信息
	virtual INT				GetItemTableQuality(VOID);
	
	// 得到type信息
	virtual INT				GetItemTableType(VOID);

	// 得到物品的类型描述2006-4-28
	LPCTSTR	GetItemTableTypeDesc();
/*
	CObject_Item*			GetHost(VOID) const				{ return m_HostOfAppend; }

	VOID					SetHost(CObject_Item* HoseItem) { m_HostOfAppend = HoseItem;}
*/
public:
	CObject_Item_Gem(INT id);
	virtual ~CObject_Item_Gem();
	//生成一个宝石
	VOID					AsGem(const _DBC_ITEM_GEM* pGemDefine);

protected:
	//--------------------------------------------------------------
	//基本属性
	const _DBC_ITEM_GEM*	m_theBaseDef;			//基本属性定义(数据表)

	STRING					m_strExtraDesc;

//	CObject_Item*			m_HostOfAppend;			//附加在哪件装备上。
};
