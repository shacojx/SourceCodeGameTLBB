#ifndef		__CHAIN_H__
#define		__CHAIN_H__

#include "Type.h"

class CChainHeader;
class CChainItem
{
private:
	CChainHeader*		m_pHead;
	CChainItem*			m_pNext;
	CChainItem*			m_pPrevious;
public:
	CChainItem();
	virtual ~CChainItem(){;}
	VOID				SetPrevious(CChainItem *pItem){
		m_pPrevious = pItem;
	}
	VOID				SetNext(CChainItem *pItem){
		m_pNext = pItem;
	}
	VOID				SetHeader(CChainHeader *pHeader){
		m_pHead = pHeader;
	}
	CChainHeader*		GetHeader(){
		return m_pHead;
	}
	CChainItem*			GetNext(){
		return m_pNext;
	}
	CChainItem*			GetPrevious(){
		return m_pPrevious;
	}
	VOID				DisConnect();
};

class CChainHeader : public CChainItem
{
friend class CChainItem;
private:
	UINT				m_uNum;
	CChainItem*			m_pEnd;
	CChainItem*			m_pHead;
	CChainItem*			m_pCurrent;

public:
	CChainHeader();
	virtual ~CChainHeader();

	CChainItem*			MoveTo(INT i);
	BOOL				Add(CChainItem* pItem);
	BOOL				Insert(CChainItem* pItem);
	BOOL				InsertBeforeCurrent(CChainItem* pItem);
	BOOL				InsertAfterCurrent(CChainItem* pItem);
	CChainItem*			Remove();
	CChainItem*			OutChain();
	BOOL				RemoveAll();
	BOOL				OutAll();

	UINT				GetNum(){
		return m_uNum;
	}
	VOID				SetNum(UINT uNum){
		m_uNum = uNum;
	}
	CChainItem*			First(){
		m_pCurrent = m_pHead;
		return m_pCurrent;
	}
	CChainItem*			Next(){
		if (!m_pCurrent)
			return NULL;
		m_pCurrent = m_pCurrent->GetNext(); 
		return m_pCurrent;	
	}

	CChainItem*			NextCycle(){
		if (!m_pCurrent)
			m_pCurrent = m_pHead;
		else if (m_pCurrent == m_pEnd)
			m_pCurrent = m_pHead;
		else
     		m_pCurrent = m_pCurrent->GetNext(); 
		
		return m_pCurrent;
	}
	CChainItem*			ViewFirst(){
		return m_pHead;
	}
	CChainItem*			ViewNext(){
		if (!m_pCurrent)
			return NULL;
		return m_pCurrent->GetNext(); 
	}

	CChainItem*			Last(){
		m_pCurrent = m_pEnd; 
		return m_pCurrent;
	}
	CChainItem*			Previous();
	CChainItem*			PreviousCycle();
	CChainItem*			ViewPrevious();

	 CChainItem*		Current(){
		 return m_pCurrent;
	 }

	 VOID				SetCurrent(CChainItem *pItem){
		m_pCurrent = pItem;
	 }

	 INT				Get(CChainItem *pItem);
};






#endif