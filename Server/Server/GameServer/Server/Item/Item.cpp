#include "stdafx.h"

#include "Item.h"
#include "ItemRuler.h"
#include "DB_Struct.h"


Item::Item( )
{
__ENTER_FUNCTION

	m_pItem = NULL ;
	m_pPet	= NULL;
	m_bLock = FALSE ;
	m_bIsInExchange = FALSE;
	m_nCoinType = 0;

__LEAVE_FUNCTION
}


Item::~Item( )
{
__ENTER_FUNCTION

	m_pItem = NULL ;
	m_pPet  = NULL;
	m_bLock = FALSE ;
	m_bIsInExchange = FALSE;

__LEAVE_FUNCTION
}


BOOL Item::Init( const _ITEM_INIT* pInit )
{
__ENTER_FUNCTION

	Assert( pInit ) ;
	
	m_pItem = const_cast<_ITEM*>(pInit->m_pItem);
	m_pPet	= const_cast<_PET_DB_LOAD*>(pInit->m_pPet);
	
	Assert((m_pItem!=0)||(m_pPet!=0));
	Assert(!(m_pItem==0 && m_pPet == 0));
	Assert(!(m_pItem!=0 && m_pPet!= 0));

	if(m_pItem) m_Type = IDT_ITEM;
	if(m_pPet)  m_Type = IDT_PET;

	//Assert( m_pItem ) ;
	

	m_bLock = FALSE ;

__LEAVE_FUNCTION

	return TRUE ;
}


VOID Item::CleanUp( )
{
__ENTER_FUNCTION

	if( m_pItem )
	{
		memset( m_pItem, 0, sizeof(_ITEM) ) ;
	}

	if(m_pPet)
	{
		m_pPet->CleanUp();
		//memset(m_pPet,0,sizeof(_PET_DB_LOAD));
	}
	m_bLock = FALSE ;

__LEAVE_FUNCTION
}

BOOL Item::IsEmpty( )
{
__ENTER_FUNCTION

	
	
	switch(m_Type)
	{
	case IDT_ITEM:
		{
			if(m_pItem==NULL)
			{
				Assert(FALSE);
				return TRUE ;
			}

			if( m_pItem->IsNullType() )
			return TRUE ;
		}
		break;
	case IDT_PET:
		{
			if(m_pPet == NULL)
			{
				Assert(FALSE);
				return TRUE ;
			}
			if( TRUE == m_pPet->m_GUID.IsNull() )
				return TRUE;
		}
		break;
	default:
		{
			Assert(FALSE);
			return TRUE;
		}
	}

	return FALSE ;

__LEAVE_FUNCTION

	return TRUE ;
}

//判断此物品是否为一件装备
BOOL	Item::IsEquip( ) const
{
__ENTER_FUNCTION

	Assert(m_pItem);

	BYTE bClass = m_pItem->ItemClass();
	if ( bClass == ICLASS_EQUIP ) return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL	Item::IsCanLay()	const //是否可以叠放的判断
{
	__ENTER_FUNCTION
	
	switch(m_Type)
	{
		case IDT_ITEM:
			{
				Assert(m_pItem);
				return	ItemRulerCheck::CheckRuler(IRL_TILE,m_pItem->m_RulerID);
			}
			break;
		case IDT_PET:
			{
				Assert(m_pPet);
				return FALSE;
			}
			break;
		default:
			{
				Assert(FALSE);
				return FALSE;
			}
	}
	
	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	Item::IsCanEquipBind()	const
{
	__ENTER_FUNCTION

	Assert(m_pItem);
	return ItemRulerCheck::CheckRuler(IRL_EQUIPBIND,m_pItem->m_RulerID)	;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	Item::IsRuler(ITEM_RULER_LIST ruler) const
{
	__ENTER_FUNCTION

	Assert(m_pItem);
	return ItemRulerCheck::CheckRuler(ruler,m_pItem->m_RulerID);

	__LEAVE_FUNCTION

	return FALSE;
}



VOID	Item::SaveValueTo(_ITEM* pItem)
{
	__ENTER_FUNCTION
	
	 Assert(pItem);
	 Assert(m_pItem);
	 if(m_pItem->ItemClass() == ICLASS_EQUIP && GetSerialQual(m_pItem->m_ItemIndex)== EQUALITY_BLUE)
	 {
		 if(m_pItem->GetItemIdent())
		 {
			memcpy(pItem,m_pItem,sizeof(_ITEM));
		 }
		 else
		 {
			pItem->m_ItemGUID	= m_pItem->m_ItemGUID;
			pItem->m_ItemIndex	= m_pItem->m_ItemIndex;
			pItem->m_nsBind		= m_pItem->m_nsBind;
			pItem->m_RulerID	= m_pItem->m_RulerID;
		 }
	 }
	 else
		memcpy(pItem,m_pItem,sizeof(_ITEM));
	
	__LEAVE_FUNCTION
}

VOID	Item::SaveValueTo(_PET_DB_LOAD* pPet)
{
	__ENTER_FUNCTION

	Assert(pPet);
	Assert(m_pPet);
	memcpy(pPet, m_pPet, sizeof(_PET_DB_LOAD));

	__LEAVE_FUNCTION

}


UINT	Item::GetPrice()	const
{
	__ENTER_FUNCTION

	Assert(m_pItem);
	BYTE	 bClass		= m_pItem->ItemClass();
	BYTE	 bQuality	= m_pItem->GetQual();
	
	switch(bClass)
	{
	case ICLASS_EQUIP:
		{
			return m_pItem->GetEquipData()->m_BasePrice;
		}
		break;
	case ICLASS_MATERIAL: 
	case ICLASS_COMITEM:
	case ICLASS_TASKITEM: 
		{
			return m_pItem->GetMedicData()->m_nBasePrice;
		}
		break;
	case ICLASS_GEM:
		{
			return m_pItem->GetGemData()->m_nPrice;
		}
		break;
	case ICLASS_STOREMAP:
		{
			return 0;
		}
	case ICLASS_TALISMAN: 
		Assert(FALSE);
		break;
	case ICLASS_GUILDITEM: 
		Assert(FALSE);
		break;
	default:
		Assert(FALSE);
		break;
	}
	Assert(FALSE);
	return -1;

	__LEAVE_FUNCTION

	return -1;
}

INT		Item::GetItemParam(UINT start,ItemParamValue& ipv)
{
	__ENTER_FUNCTION
		
		Assert(m_pItem);
		return m_pItem->GetItemParamValue(start,ipv);	

	__LEAVE_FUNCTION

		return -1;
}

VOID	Item::SetItemValue(_ITEM* pItem)
{	
	__ENTER_FUNCTION

	Assert(pItem);
	Assert(m_pItem);
	*m_pItem = *pItem;

	__LEAVE_FUNCTION
}
