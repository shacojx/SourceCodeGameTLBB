#include "stdafx.h"
/////////////////////////////////////////////////////////////////////////////////
//

#include "ItemContainer.h"



ItemContainer::ItemContainer( )
{
__ENTER_FUNCTION

	m_nContainerSize = 0 ;

__LEAVE_FUNCTION
}

ItemContainer::~ItemContainer( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

__LEAVE_FUNCTION
}

BOOL ItemContainer::Init( const _ITEMCONTAINER_INIT* pInit )
{
__ENTER_FUNCTION

	Assert( pInit ) ;

	m_nContainerSize = pInit->m_nContainerSize ;
	
	Assert( m_nContainerSize>0 && m_nContainerSize<=MAX_ITEMCONTAINER_SIZE ) ;

__LEAVE_FUNCTION

	return TRUE ;
}

VOID ItemContainer::CleanUp( )
{
__ENTER_FUNCTION

	for( INT i=0; i<MAX_ITEMCONTAINER_SIZE; i++ )
	{
		m_aItem[i].CleanUp();
	}

__LEAVE_FUNCTION
}

INT ItemContainer::GetIndexByGUID( const _ITEM_GUID* pGuid )
{
__ENTER_FUNCTION

	for( INT i=0; i<m_nContainerSize; i++ )
	{
		if( m_aItem[i].IsEmpty() )
		{
			continue ;
		}
		_ITEM_GUID guid = m_aItem[i].GetGUID();
		if( (*pGuid)== guid)
		{
			return i ;
		}
	}

__LEAVE_FUNCTION

	return INVALID_INDEX ;
}

INT ItemContainer::GetIndexByGUID( const PET_GUID_t* pGuid )
{
	__ENTER_FUNCTION

		for( INT i=0; i<m_nContainerSize; i++ )
		{
			if( m_aItem[i].IsEmpty() )
			{
				continue ;
			}

			if( (*pGuid)==m_aItem[i].GetPetGUID() )
			{
				return i ;
			}
		}

	__LEAVE_FUNCTION

	return INVALID_INDEX ;
}

INT ItemContainer::GetIndexByType( const UINT uType )
{
__ENTER_FUNCTION

	for( INT i=0; i<m_nContainerSize; i++ )
	{
		if( m_aItem[i].IsEmpty() )
		{
			continue ;
		}

		if( uType==m_aItem[i].GetItemTableIndex())
		{
			return i ;
		}
	}

__LEAVE_FUNCTION

	return INVALID_INDEX ;
}

INT ItemContainer::GetIndexByType( const UINT uType ,UINT emptyCount)
{
__ENTER_FUNCTION

	for( INT i=0; i<m_nContainerSize; i++ )
	{
		if( m_aItem[i].IsEmpty() )
		{
			continue ;
		}

		if( uType==m_aItem[i].GetItemTableIndex()
			&&m_aItem[i].GetMaxLayedNum()-m_aItem[i].GetLayedNum()>=(INT)emptyCount)
		{
			return i ;
		}
	}

__LEAVE_FUNCTION

	return INVALID_INDEX ;
}

UINT	ItemContainer::CalcItemSpace()
{
	__ENTER_FUNCTION
		
		UINT uCount =	0;

		for( INT i=0; i<m_nContainerSize; i++ )
		{
			if( m_aItem[i].IsEmpty() )
			{
				uCount++ ;
			}
		}
		
		return uCount;

		__LEAVE_FUNCTION

		return 0;
}

UINT	ItemContainer::CalcExchangeItemSpace()
{
	__ENTER_FUNCTION

	UINT uCount =	0;

	for( INT i=0; i<m_nContainerSize; i++ )
	{
		//对于已经换出的物品,可以当作空格来计算
		if( m_aItem[i].IsEmpty() || m_aItem[i].IsInExchange())
		{
			uCount++ ;
		}
	}

	return uCount;

	__LEAVE_FUNCTION

	return 0;
}



Item* ItemContainer::GetItem( const INT nIndex )
{
__ENTER_FUNCTION

	Assert( nIndex>=0 && nIndex<m_nContainerSize ) ;

	return &(m_aItem[nIndex]) ;

__LEAVE_FUNCTION

	return NULL ;
}

BOOL ItemContainer::SetItem( const INT nIndex, const Item* pItem )
{
__ENTER_FUNCTION

	Assert( pItem ) ;

	if( nIndex>=0 && nIndex<m_nContainerSize )
	{
		switch(pItem->m_Type) {
		case IDT_ITEM:
			{
				*m_aItem[nIndex].m_pItem = *pItem->m_pItem ;
				m_aItem[nIndex].m_bLock = FALSE ;
			}
			break;
		case IDT_PET:
			{
				*m_aItem[nIndex].m_pPet = *pItem->m_pPet;
				m_aItem[nIndex].m_bLock = FALSE ;
			}
			break;
		default:
			Assert(FALSE);
		}
		

		return TRUE ;
	}

__LEAVE_FUNCTION

	return FALSE ;
}

INT ItemContainer::GetEmptyItemIndex( )
{
__ENTER_FUNCTION

	for( INT i=0; i<m_nContainerSize; i++ )
	{
		if( m_aItem[i].IsEmpty() )
			return i ;
	}
	
	return INVALID_INDEX;

__LEAVE_FUNCTION

	return INVALID_INDEX ;
}

INT ItemContainer::GetNoFullItemIndex(UINT uItemIndex)
{
	__ENTER_FUNCTION

	 for(INT i = 0 ;i<m_nContainerSize;i++)
	 {
		 if((m_aItem[i].GetItemTableIndex()==uItemIndex)&&
			 (m_aItem[i].IsCanLay()))
		 {
			 if(!m_aItem[i].IsFull())
				 return i;
		 }
	 }

	__LEAVE_FUNCTION

	return -1;
}

BOOL	ItemContainer::ChangeSize(const INT nSize)
{
	__ENTER_FUNCTION
	
		Assert(nSize>0);
		
		if(nSize<=0)
			return FALSE;

		for(INT i =0;i<m_nContainerSize;i++)
		{
			if(!m_aItem[i].IsEmpty())
				return FALSE;
		}


		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}
VOID	ItemContainer::SetItemLayCount(INT nIndex, INT nCount) 
{

	__ENTER_FUNCTION

		Assert(nCount>=0);

		Item* pItem = GetItem(nIndex);
		if(!pItem)
		{
			Assert(FALSE);
			return;
		}
		pItem->m_pItem->SetItemCount(nCount);

	__LEAVE_FUNCTION

}
//减少物品重叠数量
BOOL	ItemContainer::DecItemLayCount( INT nIndex,INT nCount) 
{
	__ENTER_FUNCTION
		
		Item* pItem = GetItem(nIndex);

		if(!pItem)
		{
			Assert(FALSE);
			return	FALSE;
		}
		pItem->m_pItem->DecCount(nCount);	

		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;

}
//增加物品重叠数量
BOOL	ItemContainer::IncItemLayCount(INT nIndex, INT nCount/* =1 */)
{
	__ENTER_FUNCTION
	
		Item* pItem = GetItem(nIndex);

		if(!pItem)
		{
			Assert(FALSE);
			return	FALSE;
		}
		pItem->m_pItem->IncCount(nCount);	

		return TRUE;


	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	ItemContainer::EraseItem(UINT uIndex)
{
	__ENTER_FUNCTION
		
		Item* pItem = GetItem(uIndex);

		if(!pItem)
		{
			Assert(FALSE);
			return FALSE;
		}
		
		pItem->CleanUp();
	
		return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	ItemContainer::SetItemDur(INT nIndex, INT nDur)
{
	__ENTER_FUNCTION

	Item* pItem = GetItem(nIndex);

	if(!pItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	
	if(pItem->GetItemClass()!=ICLASS_EQUIP)
	{
		AssertEx(pItem->GetItemClass()==ICLASS_EQUIP,"非装备不能设置耐久度");
		return FALSE;
	}
	pItem->m_pItem->GetEquipData()->m_CurDurPoint	=	nDur;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	ItemContainer::SetItemDamagePoint(INT nIndex, INT nPoint)
{
	__ENTER_FUNCTION

		Item* pItem = GetItem(nIndex);

	if(!pItem)
	{
		Assert(FALSE);
		return FALSE;
	}

	if(pItem->GetItemClass()!=ICLASS_EQUIP)
	{
		AssertEx(pItem->GetItemClass()==ICLASS_EQUIP,"非装备不能设置损伤度");
		return FALSE;
	}
	pItem->m_pItem->GetEquipData()->m_CurDamagePoint	=	nPoint;

	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	ItemContainer::SetItemMaxDur(INT nIndex, INT nDur)
{
	__ENTER_FUNCTION

	Item* pItem = GetItem(nIndex);

	if(!pItem)
	{
		Assert(FALSE);
		return FALSE;
	}

	if(pItem->GetItemClass()!=ICLASS_EQUIP)
	{
		AssertEx(pItem->GetItemClass()==ICLASS_EQUIP,"非装备不能设置耐久度");
		return FALSE;
	}
	AssertEx(nDur<=255,"装备最大耐久度255");
	pItem->m_pItem->GetEquipData()->m_MaxDurPoint	=	nDur;

	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	ItemContainer::SetItemValue(INT nIndex,_ITEM* pItem)
{
	__ENTER_FUNCTION
		
	Assert(pItem);
	Assert(!pItem->IsNullType());

	Item*	pDestItem = GetItem(nIndex);
	
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	
	Assert(pDestItem->m_Type == IDT_ITEM);

	*pDestItem->m_pItem	= *pItem;
	
	return	 TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	ItemContainer::SetItemValue(INT nIndex,const _PET_DB_LOAD* pPet)
{
	__ENTER_FUNCTION

	Assert(pPet);
	//Assert(FALSE == pPet->m_GUID.IsNull());


	Item*	pDestItem = GetItem(nIndex);
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}

	
	Assert(pDestItem->m_Type == IDT_PET);
	*pDestItem->m_pPet	= *pPet;

	return	 TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL	ItemContainer::SetItemBind(INT nIndex)
{
	__ENTER_FUNCTION
		
	Item*	pDestItem = GetItem(nIndex);

	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}

	pDestItem->m_pItem->SetItemBind(TRUE);

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	ItemContainer::SetItemIdent(INT nIndex)
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	pDestItem->m_pItem->SetItemIdent(TRUE);

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

//设置物品修理次数
BOOL	ItemContainer::SetItemFailTimes(INT nIndex, INT nTimes)
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	pDestItem->m_pItem->SetItemFailTimes(nTimes);

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}



BOOL	ItemContainer::AddItemAttr(INT nIndex,_ITEM_ATTR iA)
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);

	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	
	INT AttrCount = pDestItem->m_pItem->GetEquipData()->m_AttrCount;
	
	pDestItem->m_pItem->GetEquipData()->m_pAttr[AttrCount]	=	iA;

	pDestItem->m_pItem->GetEquipData()->m_AttrCount++;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	ItemContainer::DelItemAttr(INT nIndex,_ITEM_ATTR iA)
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);

	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}

	INT AttrCount = 
		pDestItem->m_pItem->GetEquipData()->m_AttrCount;

	for(INT iAttrIndex=0;iAttrIndex<AttrCount;iAttrIndex++ )
	{
		_ITEM_ATTR EquipAttr = pDestItem->m_pItem->GetEquipData()->m_pAttr[iAttrIndex];
		if(EquipAttr ==	iA)
		{
			pDestItem->m_pItem->GetEquipData()->m_pAttr[iAttrIndex] =
				pDestItem->m_pItem->GetEquipData()->m_pAttr[AttrCount-1];
			pDestItem->m_pItem->GetEquipData()->m_pAttr[AttrCount-1].CleanUp();
		}

	}
	pDestItem->m_pItem->GetEquipData()->m_AttrCount--;
	
	return TRUE;
	
	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	ItemContainer::DelGemInfo(INT nIndex,INT GemIndex)
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);

	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	
	if(GemIndex>=pDestItem->GetEquipGemCount())
	{
		Assert(GemIndex<pDestItem->GetEquipGemCount());
		return FALSE;
	}
	pDestItem->m_pItem->GetEquipData()->m_pGemInfo[GemIndex].m_GemType = 0;
	--(pDestItem->m_pItem->GetEquipData()->m_StoneCount);

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	ItemContainer::AddGemInfo(INT nIndex,INT& GemIndex,UINT GemType)
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);

	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	BOOL bRet	= FALSE;

	if ( pDestItem->GetEquipGemCount() < 1 )
	{
		pDestItem->m_pItem->GetEquipData()->m_pGemInfo[0].m_GemType = GemType;
		GemIndex	=	0;
		pDestItem->m_pItem->GetEquipData()->m_StoneCount = 1;
		bRet		=	TRUE;
	}
	else
	{
		for( INT i=0; i<pDestItem->GetGemSlotMax(); ++i )
		{
			if( pDestItem->GetEquipGemInfo(i).m_GemType == 0 )
			{
				pDestItem->m_pItem->GetEquipData()->m_pGemInfo[i].m_GemType = GemType;
				GemIndex	=	i;
				++(pDestItem->m_pItem->GetEquipData()->m_StoneCount);
				bRet		=	TRUE;
				break;
			}
		}
	}

	return bRet;

	__LEAVE_FUNCTION

	return FALSE;
}


BOOL ItemContainer::SetItemGuid(INT nIndex,_ITEM_GUID* pGUID)
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);

	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	
	if(!pGUID)
	{
		Assert(pGUID);
		return FALSE;
	}

	pDestItem->m_pItem->m_ItemGUID = *pGUID;

	return TRUE;

	__LEAVE_FUNCTION
	
	return FALSE;
}

BOOL ItemContainer::SetItemGuid(INT nIndex,PET_GUID_t* pGUID)
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);

	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}

	if(!pGUID)
	{
		Assert(pGUID);
		return FALSE;
	}
	
	Assert(pDestItem->m_Type == IDT_PET);

	pDestItem->m_pPet->m_GUID = *pGUID;

	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL  ItemContainer::SetPetGUID(INT nIndex,PET_GUID_t GUID) 				//设置宠物GUID
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);

	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}

	Assert(pDestItem->m_Type == IDT_PET);

	pDestItem->m_pPet->m_GUID = GUID;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL  ItemContainer::SetSpouseGUID(INT nIndex,PET_GUID_t GUID) 				//设置宠物GUID
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);

	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}

	Assert(pDestItem->m_Type == IDT_PET);

	pDestItem->m_pPet->m_SpouseGUID = GUID;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  ItemContainer::SetDataID(INT nIndex,INT ID)							//设置宠物模型		
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);

	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}

	Assert(pDestItem->m_Type == IDT_PET);

	pDestItem->m_pPet->m_nDataID = ID;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  ItemContainer::SetName(INT nIndex,const CHAR* pName)						//设置名字
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);

	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}

	Assert(pDestItem->m_Type == IDT_PET);
	Assert(pName);
	strncpy(pDestItem->m_pPet->m_szName,pName,MAX_CHARACTER_NAME);

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  ItemContainer::SetNick(INT nIndex,const CHAR* pNick)						//设置昵称
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	Assert(pDestItem->m_Type == IDT_PET);
	Assert(pNick);
	strncpy(pDestItem->m_pPet->m_szNick,pNick,MAX_NICK_NAME);

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  ItemContainer::SetLevel(INT nIndex,INT level)							//设置等级
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	Assert(pDestItem->m_Type == IDT_PET);
	
	pDestItem->m_pPet->m_nLevel = level;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL  ItemContainer::SetTakeLevel(INT nIndex,INT takeLevel)					//设置携带等级
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	Assert(pDestItem->m_Type == IDT_PET);

	pDestItem->m_pPet->m_nTakeLevel = takeLevel;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  ItemContainer::SetAttackType(INT nIndex,INT attackType)				//设置进攻类型（物/法）
{
	__ENTER_FUNCTION

		Item*	pDestItem = GetItem(nIndex);
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	Assert(pDestItem->m_Type == IDT_PET);

	pDestItem->m_pPet->m_nAttackType = attackType;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  ItemContainer::SetAIType(INT nIndex,INT AIType)						//设置AI类型
{
	__ENTER_FUNCTION

		Item*	pDestItem = GetItem(nIndex);
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	Assert(pDestItem->m_Type == IDT_PET);

	pDestItem->m_pPet->m_AIType = AIType;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  ItemContainer::SetCampData(INT nIndex,const _CAMP_DATA* pCamp)		//设置阵营
{
	__ENTER_FUNCTION

		Item*	pDestItem = GetItem(nIndex);
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	Assert(pDestItem->m_Type == IDT_PET);
	Assert(pCamp);
	pDestItem->m_pPet->m_CampData = *pCamp;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  ItemContainer::SetHP(INT nIndex,INT hp)								//设置生命值
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	Assert(pDestItem->m_Type == IDT_PET);
	
	pDestItem->m_pPet->m_nHP = hp;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL  ItemContainer::SetLife(INT nIndex,INT Life)							//设置当前寿命
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	Assert(pDestItem->m_Type == IDT_PET);
	Life < 0 ? Life = 0 : NULL;
	Life > 99999 ? Life = 99999 : NULL;
	pDestItem->m_pPet->m_nLife	= Life;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  ItemContainer::SetPetType(INT nIndex,BYTE PetType)					//宝宝，变异，野生
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	Assert(pDestItem->m_Type == IDT_PET);
	pDestItem->m_pPet->m_byPetType	= PetType;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  ItemContainer::SetGeneration(INT nIndex,BYTE Gen)						//几代宠
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	Assert(pDestItem->m_Type == IDT_PET);
	pDestItem->m_pPet->m_byGeneration	= Gen;

	return TRUE;

	__LEAVE_FUNCTION

		return FALSE;
}
BOOL  ItemContainer::SetHappiness(INT nIndex,BYTE byHappiness)						//快乐度		
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	Assert(pDestItem->m_Type == IDT_PET);
	pDestItem->m_pPet->m_byHappiness	= byHappiness;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  ItemContainer::SetStrPer(INT nIndex,INT strper)						//力量资质	
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	Assert(pDestItem->m_Type == IDT_PET);
	pDestItem->m_pPet->m_nStrPerception	= strper;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  ItemContainer::SetConPer(INT nIndex,INT conper)						//体力资质	
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	Assert(pDestItem->m_Type == IDT_PET);
	pDestItem->m_pPet->m_nConPerception	= conper;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  ItemContainer::SetDexPer(INT nIndex,INT dexper)						//身法资质
{
	__ENTER_FUNCTION

		Item*	pDestItem = GetItem(nIndex);
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	Assert(pDestItem->m_Type == IDT_PET);
	pDestItem->m_pPet->m_nDexPerception	= dexper;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  ItemContainer::SetSprPer(INT nIndex,INT sprper)						//灵气资质
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	Assert(pDestItem->m_Type == IDT_PET);
	pDestItem->m_pPet->m_nSprPerception	= sprper;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  ItemContainer::SetIntPer(INT nIndex,INT intper)						//定力资质
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	Assert(pDestItem->m_Type == IDT_PET);
	pDestItem->m_pPet->m_nIntPerception	= intper;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  ItemContainer::SetGenGu(INT nIndex,INT gengu)							//根骨
{
	__ENTER_FUNCTION

		Item*	pDestItem = GetItem(nIndex);
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	Assert(pDestItem->m_Type == IDT_PET);
	pDestItem->m_pPet->m_nGenGu	= gengu;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  ItemContainer::SetGrowRate(INT nIndex,FLOAT rate)						//成长率
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	Assert(pDestItem->m_Type == IDT_PET);
	pDestItem->m_pPet->m_fGrowRate	= rate;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  ItemContainer::SetRemainPoint(INT nIndex,INT rPoint)					//一级属性剩余点数
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	Assert(pDestItem->m_Type == IDT_PET);
	pDestItem->m_pPet->m_nRemainPoint	= rPoint;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  ItemContainer::SetExp(INT nIndex,INT exp)							//经验值
{	
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	Assert(pDestItem->m_Type == IDT_PET);
	pDestItem->m_pPet->m_nExp	= exp;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  ItemContainer::SetLvl1Attr(INT nIndex,CHAR_ATTR_LEVEL1 type,INT value)//基础一级战斗属性（不包括技能和装备增加的部分）
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	Assert(pDestItem->m_Type == IDT_PET);
	pDestItem->m_pPet->m_BaseAttrLevel1.Set(type,value);

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL  ItemContainer::SetSkill(INT nIndex,UINT SkillIndex,_PET_SKILL skill)	//宠物技能
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	Assert(pDestItem->m_Type == IDT_PET);
	Assert(SkillIndex<PET_MAX_SKILL_COUNT);
	pDestItem->m_pPet->m_SkillList[SkillIndex] = skill;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}
BOOL ItemContainer::IsInContainer(UINT uBagIndex)
{
	__ENTER_FUNCTION
	if(uBagIndex>=0 && uBagIndex < (UINT)m_nContainerSize)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	__LEAVE_FUNCTION
	return FALSE;

}

BOOL ItemContainer::SetItemPWLock(INT nIndex,BOOL bLock)
{
	__ENTER_FUNCTION
	
	Item*	pDestItem = GetItem(nIndex);
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	if(pDestItem->m_Type == IDT_PET)
	{
		return TRUE;
	}
	else if(pDestItem->m_Type == IDT_ITEM)
	{
		pDestItem->m_pItem->SetItemPLock(bLock);
		return TRUE;
	}

	__LEAVE_FUNCTION

		return FALSE;
}

BOOL ItemContainer::SetItemCreator(INT nIndex,const CHAR* CreatorName)
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	if(pDestItem->m_Type == IDT_PET)
	{
		return TRUE;
	}
	else if(pDestItem->m_Type == IDT_ITEM)
	{
		pDestItem->m_pItem->SetCreatorVar(TRUE);
		strncpy(pDestItem->m_pItem->m_Creator,CreatorName,MAX_ITEM_CREATOR_NAME);
		pDestItem->m_pItem->m_Creator[MAX_ITEM_CREATOR_NAME-1] = '\0';
		return TRUE;
	}



	__LEAVE_FUNCTION

		return FALSE;
}

BOOL ItemContainer::SetItemParam(INT nIndex,UINT start,INT type,INT value)
{
	__ENTER_FUNCTION

	Item*	pDestItem = GetItem(nIndex);
	if(!pDestItem)
	{
		Assert(FALSE);
		return FALSE;
	}
	if(pDestItem->m_Type == IDT_PET)
	{
		return TRUE;
	}
	else if(pDestItem->m_Type == IDT_ITEM)
	{
		ItemParamValue	ipv = (ItemParamValue)type;	
		pDestItem->m_pItem->SetItemParam(start,ipv,value);
		return TRUE;
	}



	__LEAVE_FUNCTION

		return FALSE;
}