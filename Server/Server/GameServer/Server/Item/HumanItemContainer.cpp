#include "stdafx.h"
#include "HumanItemContainer.h"
#include "HumanDB.h"

BOOL HumanItemContainer::Init(const _ITEMCONTAINER_INIT* pInit )
{
	__ENTER_FUNCTION

	if(!ItemContainer::Init(pInit))
		return FALSE;

	const _HUMAN_ITEMCONTAINER_INIT*	pHICInit = 
		static_cast<const _HUMAN_ITEMCONTAINER_INIT*>(pInit);

	if(pHICInit==NULL)
		return FALSE;

	Assert(pHICInit->m_pHumanDB);

	m_ICTType	=	pHICInit->m_ICTType;
	m_pHumanDB	=	pHICInit->m_pHumanDB;
	m_DBOffSet	=	pHICInit->m_DBOffSet;
	return TRUE;
	
	__LEAVE_FUNCTION

	return	FALSE;
}

UINT	HumanItemContainer::ConIndex2BagIndex(UINT uConIndex)
{
	__ENTER_FUNCTION

	Assert(uConIndex!=INVALID_INDEX);

	return m_DBOffSet+uConIndex;

	__LEAVE_FUNCTION

	return INVALID_INDEX;
}

UINT	HumanItemContainer::BagIndex2ConIndex(UINT uBagIndex)
{
	__ENTER_FUNCTION

		Assert(uBagIndex>=m_DBOffSet);
		Assert(uBagIndex<m_DBOffSet+m_nContainerSize);

		return	uBagIndex-m_DBOffSet;

	__LEAVE_FUNCTION

		return INVALID_INDEX;
}
BOOL	HumanItemContainer::IsInContainer(UINT uBagIndex)
{
	__ENTER_FUNCTION
		if(uBagIndex >= m_DBOffSet && uBagIndex< m_DBOffSet+ GetContainerSize() )
			return TRUE;
		else
			return FALSE;

	__LEAVE_FUNCTION
		return FALSE;
}

BOOL	HumanItemContainer::SetItem(const INT nIndex, const Item* pItem )
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);
	BOOL	bRet = ItemContainer::SetItem(nIndex,pItem);
	
	if(bRet)
	{
		SetDBDirty(nIndex);
	}
	return	bRet;
	__LEAVE_FUNCTION
	
	return  FALSE;
}

//设置物品重叠数量
VOID		HumanItemContainer::SetItemLayCount(INT nIndex, INT nCount) 
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);
	
	
	ItemContainer::SetItemLayCount(nIndex,nCount);
	
	SetDBDirty(nIndex);

	__LEAVE_FUNCTION

}
//减少物品重叠数量
BOOL		HumanItemContainer::DecItemLayCount(INT nIndex, INT nCount)
{
	__ENTER_FUNCTION

	BOOL	bRet	=	ItemContainer::DecItemLayCount(nIndex,nCount);
	
	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}
//增加物品重叠数量
BOOL		HumanItemContainer::IncItemLayCount(INT nIndex, INT nCount)
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);


	BOOL	bRet  = ItemContainer::IncItemLayCount(nIndex,nCount);
	
	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}

//删除物品
BOOL		HumanItemContainer::EraseItem(UINT uIndex)
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);

	BOOL	bRet	=	ItemContainer::EraseItem(uIndex);
	
	if(bRet)
	{
		SetDBDirty(uIndex);
	}

	return bRet;
	
	__LEAVE_FUNCTION
	
	return FALSE;
}

//设置物品耐久度
BOOL			HumanItemContainer::SetItemDur(INT nIndex, INT nDur)
{
	__ENTER_FUNCTION
	
		Assert(m_pHumanDB);
		
		BOOL	bRet	=	ItemContainer::SetItemDur(nIndex,nDur);
	
		if(bRet)
		{
			SetDBDirty(nIndex);
		}

		return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}
//设置物品损伤点
BOOL		HumanItemContainer::SetItemDamagePoint(INT nIndex, INT nPoint)
{
	__ENTER_FUNCTION
	
		Assert(m_pHumanDB);
		
		BOOL	bRet	=	ItemContainer::SetItemDamagePoint(nIndex,nPoint);
	
		if(bRet)
		{
			SetDBDirty(nIndex);
		}

		return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}


//设置物品最大耐久度
BOOL			HumanItemContainer::SetItemMaxDur(INT nIndex, INT nDur)
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);


	BOOL	bRet	=	ItemContainer::SetItemMaxDur(nIndex,nDur);
	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL		HumanItemContainer::SetItemValue(INT nIndex,_ITEM* pItem)
{
	__ENTER_FUNCTION
	
	Assert(m_pHumanDB);
	Assert(pItem);
	
	BOOL bRet	=	ItemContainer::SetItemValue(nIndex,pItem);
	
	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}


BOOL		HumanItemContainer::SetItemBind(INT nIndex)
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);


	BOOL bRet = ItemContainer::SetItemBind(nIndex);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL		HumanItemContainer::SetItemIdent(INT nIndex)
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);
	BOOL bRet = ItemContainer::SetItemIdent(nIndex);
	if(bRet)
	{
		SetDBDirty(nIndex);
	}
	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL HumanItemContainer::SetItemCreator(INT nIndex,const CHAR* CreatorName)
{
	__ENTER_FUNCTION

	Assert(m_pHumanDB);
	BOOL bRet = ItemContainer::SetItemCreator(nIndex,CreatorName);
	if(bRet)
	{
		SetDBDirty(nIndex);
	}
	return bRet;

	__LEAVE_FUNCTION

	return FALSE;

}


//增加物品属性
BOOL		HumanItemContainer::AddItemAttr(INT nIndex,_ITEM_ATTR iA)
{
__ENTER_FUNCTION

	Assert(m_pHumanDB);


	BOOL bRet = ItemContainer::AddItemAttr(nIndex, iA);
	
	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

__LEAVE_FUNCTION

	return FALSE;
}

//删除物品属性
BOOL		HumanItemContainer::DelItemAttr(INT nIndex,_ITEM_ATTR iA)
{
__ENTER_FUNCTION

	Assert(m_pHumanDB);


	BOOL bRet = ItemContainer::DelItemAttr(nIndex, iA);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

__LEAVE_FUNCTION

	return FALSE;
}

//删除宝石信息
BOOL		HumanItemContainer::DelGemInfo(INT nIndex,INT GemIndex)
{
__ENTER_FUNCTION

	Assert(m_pHumanDB);


	BOOL bRet = ItemContainer::DelGemInfo(nIndex, GemIndex);

	if(bRet)
	{
		SetDBDirty(nIndex);
	}

	return bRet;

__LEAVE_FUNCTION

	return FALSE;
}

//添加宝石信息
BOOL		HumanItemContainer::AddGemInfo(INT nIndex,INT& GemIndex,UINT GemType)
{
__ENTER_FUNCTION




	BOOL bRet = ItemContainer::AddGemInfo(nIndex, GemIndex, GemType);
	
	if(bRet)
	{
		SetDBDirty(nIndex);
	}
	return bRet;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL	HumanItemContainer::SetDBDirty(INT OffSet)
{
	__ENTER_FUNCTION


	Assert(m_pHumanDB);
	Assert(OffSet>=0);
	Assert(OffSet<m_nContainerSize);

	switch(m_ICTType)
	{
	case ICT_BASE_CONTAINER:
		{
			m_pHumanDB->SetDBEnumAttrDirty((CHAR_ATTR_DB)(CATTR_DB_BAG_BEGIN+BASE_CONTAINER_OFFSET+OffSet)); 
		}
		break;
	//case ICT_MAT_CONTAINER:
	//	{
	//		m_pHumanDB->SetDBEnumAttrDirty((CHAR_ATTR_DB)(CATTR_DB_BAG_BEGIN+MAT_CONTAINER_OFFSET+OffSet)); 
	//	}
	//	break;
	//case ICT_TASK_CONTAINER:
	//	{
	//		m_pHumanDB->SetDBEnumAttrDirty((CHAR_ATTR_DB)(CATTR_DB_BAG_BEGIN+TASK_CONTAINER_OFFSET+OffSet)); 
	//	}
	//	break;
	case ICT_EQUIP_CONTAINER:
		{
			m_pHumanDB->SetDBEnumAttrDirty((CHAR_ATTR_DB)(CATTR_DB_BAG_BEGIN+EQUIP_CONTAINER_OFFSET+OffSet)); 
		}
		break;
	case ICT_BANK_CONTAINER:
		{
			m_pHumanDB->SetDBEnumAttrDirty((CHAR_ATTR_DB)(CATTR_DB_BAG_BEGIN+BANK_CONTAINER_OFFSET+OffSet)); 
		}
		break;

	default:
		{
			AssertEx(FALSE,"无效容器类型");
			return FALSE;
		}
	}
	
	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}


