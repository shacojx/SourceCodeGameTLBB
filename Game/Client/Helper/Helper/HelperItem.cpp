#include "StdAfx.h"
#include "HelperItem.h"
#include "HelperSystem.h"
#include "HelperItemSet.h"

/*************************************************************************
	Constructor
*************************************************************************/
CHelperItem::CHelperItem(const String& strName, const String& strAnchor, const String& strUrl) :
	m_strName(strName),
	m_strAnchor(strAnchor),
	m_strUrl(strUrl),
	m_nUserData(0)
{

}

CHelperItem::CHelperItem(const CHelperItem& obj)
{
	m_strName = obj.m_strName;
	m_strAnchor = obj.m_strAnchor;
	m_strUrl = obj.m_strUrl;
	m_childBuf = obj.m_childBuf;
}

CHelperItem& CHelperItem::operator=(const CHelperItem& obj)
{
	m_strName = obj.m_strName;
	m_strAnchor = obj.m_strAnchor;
	m_strUrl = obj.m_strUrl;
	m_childBuf = obj.m_childBuf;

	return *this;
}

/*************************************************************************
	Destructor
*************************************************************************/
CHelperItem::~CHelperItem()
{
}

/*************************************************************************
	Add a new child item for this HelperItem.
*************************************************************************/
CHelperItem* CHelperItem::AddChildItem(const CHelperItem& childItem)
{
	m_childBuf.push_back(childItem);

	CHelperItem* pReturn = &(*(m_childBuf.rbegin()));

	// insert to anchor map
	if(!childItem.m_strAnchor.empty())
	{
		CHelperSystem::GetMe()->GetHelpItemSet()->InsertAnchorItem(
			childItem.m_strAnchor, pReturn);
	}

	return pReturn;
}
