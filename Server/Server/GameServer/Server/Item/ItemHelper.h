/********************************************************************
	创建日期:	2005年11月1日
	创建时间:	13:48
	文件名称:	ItemHelper.h
	文件路径:	d:\Prj\Server\Server\Item\ItemHelper.h
	
	文件功能： 物品系统辅助类
	修改纪录：
	
*********************************************************************/



#ifndef _ITEM_HELPER_H_
#define _ITEM_HELPER_H_
#include "Type.h"


//序列号帮助类
struct TSerialHelper {

	uint	m_SN;

	TSerialHelper(){m_SN = 0;};
	TSerialHelper(uint uiSN){m_SN = uiSN;};
	TSerialHelper(CHAR* pStr)
	{
		__ENTER_FUNCTION

			Assert(pStr!=NULL);

			m_SN = atoi(pStr);

		__LEAVE_FUNCTION
	}

	BOOL		GetItemTypeStruct( _ITEM_TYPE& it )
	{
		it.m_Class		=	GetItemClass();
		it.m_Index		=	GetItemIndex();
		it.m_Quality	=	GetItemQual();
		it.m_Type		=	GetItemType();

		return TRUE;
	}

	_ITEM_TYPE	GetItemTypeStruct()
	{
		_ITEM_TYPE it;

		it.m_Class		=	GetItemClass();
		it.m_Index		=	GetItemIndex();
		it.m_Quality	=	GetItemQual();
		it.m_Type		=	GetItemType();

		return it;
	}
	ITEM_CLASS	GetItemClass()
	{
		return	(ITEM_CLASS)GetSerialClass(m_SN);
	}
	VOID		SetClass(ITEM_CLASS rh)
	{
		m_SN = rh*1000000+m_SN%1000000;
	}
	uint		GetItemQual()
	{
		return	GetSerialQual(m_SN);
	}

	VOID		SetItemQual(uint uiQual)
	{
		m_SN = m_SN + (uiQual-GetItemQual())*10000;
	}

	uint		GetItemType()
	{
		return GetSerialType(m_SN);
	}

	VOID		SetItemType(uint	uiType)
	{
		m_SN  = m_SN + (uiType-GetItemType())*100;
	}

	uint		GetItemIndex()
	{
		return	GetSerialIndex(m_SN);
	}
	VOID		SetItemIndex(uint	uiIndex)
	{
		m_SN	=	m_SN +(uiIndex-GetItemIndex());
	}

};

//物品随机数产生器
class ItemRander
{
public:
	static	double		DoubleRand();
	static	INT			IntRand();
};

#endif