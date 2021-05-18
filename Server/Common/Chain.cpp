#include "stdafx.h"

//-----------------------------------------------------------------------------
// ×îµ×²ãµÄÁ´±í
//-----------------------------------------------------------------------------
#include "stdafx.h"
#include	"Chain.h"



CChainItem::CChainItem()
{
	m_pHead     = NULL;
	m_pNext     = NULL;
	m_pPrevious = NULL;
}

VOID CChainItem::DisConnect()
{
	if (m_pHead)
	{
   		m_pHead->SetNum(m_pHead->GetNum() - 1);
		if (m_pHead->GetNum() == 0)
		{
			m_pHead->m_pHead    = NULL;
			m_pHead->m_pEnd     = NULL;
			m_pHead->m_pCurrent = NULL;
		}else
		{
			//if first item
   			if ( m_pHead->m_pHead == this )
			{
				m_pHead->m_pHead    = m_pNext;
				m_pHead->m_pCurrent = m_pNext;
			}

			//if last item
			if ( m_pHead->m_pEnd == this )
			{
				m_pHead->m_pEnd = m_pPrevious;
				m_pHead->m_pCurrent = m_pPrevious;
			}

		    //else 
		    if ( m_pHead->m_pCurrent == this )
			{
			    /*******************************************
			    !!! IMPORTANT !!!  RULE
			    current pointer is always moving to Next if possible
			    *******************************************/
				m_pHead->m_pCurrent = m_pNext;
			}
		}
	}

	//connect previous item and next item
	if (m_pNext)	
		m_pNext->SetPrevious(m_pPrevious);
	if (m_pPrevious)
		m_pPrevious->SetNext(m_pNext);
   
	m_pNext = NULL;
	m_pPrevious = NULL;
	m_pHead = NULL;
}

CChainHeader::CChainHeader()
{
	m_uNum    = 0;
	m_pEnd     = NULL;
	m_pHead    = NULL;
	m_pCurrent = NULL;
}

CChainHeader::~CChainHeader()
{
	RemoveAll();
}

CChainItem*	CChainHeader::MoveTo(INT i)
{
	m_pCurrent = m_pHead;
	for(INT j = 0; j < i; j ++)
	{
		if (m_pCurrent->GetNext())
		{
			m_pCurrent = m_pCurrent->GetNext();
		}else
		{
			return NULL;
		}
	}
	return  m_pCurrent;
}

BOOL CChainHeader::Add(CChainItem* pItem)
{
	pItem->DisConnect();

	// first note
	if (!m_uNum)
	{
		m_pEnd     = pItem;
		m_pHead    = pItem;
		m_pCurrent = pItem;
		SetNext(pItem);
		pItem->SetPrevious((CChainItem *)this);
		pItem->SetNext(NULL);
		m_uNum ++;

	}else{

		m_pEnd->SetNext(pItem);
		pItem->SetPrevious(m_pEnd);
		pItem->SetNext(NULL);
		m_pEnd = pItem;
		m_uNum ++;
	}

	pItem->SetHeader(this);
	return TRUE;
}

BOOL CChainHeader::Insert(CChainItem* pItem)
{
	if (!m_uNum)
	{
		m_pEnd     = pItem;
		m_pHead    = pItem;
		m_pCurrent = pItem;
		SetNext(pItem);
		pItem->SetPrevious((CChainItem *)this);
		pItem->SetNext(NULL);
		m_uNum ++;

	}else{

		SetNext(pItem);							 
		pItem->SetPrevious((CChainItem *)this);
		pItem->SetNext(m_pHead);
		m_pHead->SetPrevious(pItem);
		m_pHead = pItem;
		m_uNum ++;

	}

	pItem->SetHeader(this);
	return TRUE;
}

BOOL CChainHeader::InsertBeforeCurrent(CChainItem *pItem)
{
	if (!m_uNum)
	{
		m_pEnd     = pItem;
		m_pHead    = pItem;
		m_pCurrent = pItem;
		SetNext(pItem);
		pItem->SetPrevious((CChainItem *)this);
		pItem->SetNext(NULL);
		m_uNum ++;

	}else{

		if (m_pCurrent == m_pHead)
		{
			m_pHead = pItem;
		}

		pItem->SetNext(m_pCurrent);
		pItem->SetPrevious(m_pCurrent->GetPrevious());
		m_pCurrent->GetPrevious()->SetNext(pItem);
		m_pCurrent->SetPrevious(pItem);

		m_uNum ++;
	}

	pItem->SetHeader(this);
	return TRUE;
}

BOOL CChainHeader::InsertAfterCurrent(CChainItem* pItem)
{
	CChainItem *pNext = GetNext();
	if( pNext )
	{
		InsertBeforeCurrent( pItem );
	}else 
	{
		Add( pItem );
	}

	return TRUE;
}

CChainItem*	CChainHeader::Remove()
{
	CChainItem*	pPreviousCurrent = m_pCurrent;
	  
	if (m_pCurrent)
	{
		CChainItem*  pReturn = m_pCurrent->GetNext();
  		/***********************************
		IMPORTANT!!!
		After call DisConnect() pCurrent will be chged
		************************************/
		m_pCurrent->DisConnect();
		delete pPreviousCurrent;
		return  pReturn;
	}else
	{
		return   NULL;
	}
}

CChainItem*	CChainHeader::OutChain()
{
	CChainItem*	pReturn = m_pCurrent;

	if (m_pCurrent)
	{
		m_pCurrent->DisConnect();
	}

	return pReturn;
}

BOOL CChainHeader::RemoveAll()
{
	First();
	while(Remove())
	{
	  ;
	}

	m_uNum = 0;

	return TRUE;
}

BOOL CChainHeader::OutAll()
{
	Last();
	while(m_uNum > 0)
	{
		OutChain();
	}

	m_uNum = 0;

	return TRUE;

}


INT	CChainHeader::Get(CChainItem *pItem)
{
	 UINT	uNum = 0;

	 CChainItem *temp = First();

	 while(temp)
	 {
		if(pItem == temp)
			return uNum;

		temp = Next();

		uNum ++;
	 }

	 return -1;
}






