// 增加了处理宝石镶嵌以及物品属性加减的函数
#ifndef _HUMAN_ITEM_CONTAINER_H_
#define _HUMAN_ITEM_CONTAINER_H_
#include "ItemContainer.h"


class HumanDB;

struct _HUMAN_ITEMCONTAINER_INIT: 
	public _ITEMCONTAINER_INIT
{
	HumanDB*				m_pHumanDB;
	ITEM_CONTAINER_TYPE		m_ICTType;
	UINT					m_DBOffSet;

};

class Obj_Human;

class HumanItemContainer:public ItemContainer
{
	HumanDB*				m_pHumanDB;
	ITEM_CONTAINER_TYPE		m_ICTType;
	UINT					m_DBOffSet;

public:
	virtual		BOOL		Init( const _ITEMCONTAINER_INIT* pInit ) ;//初始化容器
	//容器编号到Bag编号
	virtual		UINT		ConIndex2BagIndex(UINT uConIndex);
	//背包编号到容器编号
	virtual		UINT		BagIndex2ConIndex(UINT uBagIndex);
	//测试传入的一个值是不是能够在这个Con的范围之中
	virtual		BOOL		IsInContainer(UINT uBagIndex);

protected :
	//设置物品的属性
	virtual		BOOL		SetItem( const INT nIndex, const Item* pItem );
	//设置物品重叠数量
	virtual		VOID		SetItemLayCount(INT nIndex, INT nCount) ;
	//减少物品重叠数量
	virtual		BOOL		DecItemLayCount(INT nIndex, INT nCount=1) ;
	//增加物品重叠数量
	virtual		BOOL		IncItemLayCount(INT nIndex, INT nCount=1);
	//删除物品
	virtual		BOOL		EraseItem(UINT uIndex);
	//设置物品耐久度
	virtual	BOOL			SetItemDur(INT nIndex, INT nDur);
	//设置物品损伤点
	virtual	BOOL			SetItemDamagePoint(INT nIndex, INT nPoint);
	//设置物品最大耐久度
	virtual	BOOL			SetItemMaxDur(INT nIndex, INT nDur);

	//设置物品属性
	virtual	BOOL			SetItemValue(INT nIndex,_ITEM*	pItem);
	//设置物品帮定
	virtual	BOOL			SetItemBind(INT nIndex);
	//设置物品鉴定信息
	virtual BOOL			SetItemIdent(INT nIndex);
	//设置制造者信息
	virtual	BOOL			SetItemCreator(INT nIndex,const CHAR* CreatorName);

	//增加物品属性
	virtual BOOL			AddItemAttr(INT nIndex,_ITEM_ATTR iA);
	//删除物品属性
	virtual	BOOL			DelItemAttr(INT nIndex,_ITEM_ATTR iA);
	//删除宝石信息
	virtual BOOL			DelGemInfo(INT nIndex,INT GemIndex);
	//添加宝石信息
	virtual	BOOL			AddGemInfo(INT nIndex,INT& GemIndex,UINT GemType);
private:
	BOOL					SetDBDirty(INT OffSet);

	
};



#endif