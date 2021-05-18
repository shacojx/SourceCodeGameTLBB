#include "StdAfx.h"
#include "HelperItemSet.h"
#include "HelperItem.h"
#include "HelperItemSet_XmlHandler.h"
#include "HelperSystem.h"
#include "HelperXMLParser.h"

CHelperItemSet::CHelperItemSet() :
	m_rootItem(0)
{
	// create root item.
	m_rootItem = new CHelperItem("root");
}

CHelperItemSet::~CHelperItemSet()
{
	delete m_rootItem; m_rootItem = 0;
}

/*************************************************************************
	Load all items from itemset file.
*************************************************************************/
void CHelperItemSet::LoadItemSet(const String& strFileName)
{
	if (strFileName.empty() || (strFileName == ""))
	{
		throw std::exception("CHelperItemSet::Load - Filename supplied for ItemSet loading must be valid");
	}

    // create handler object
    CHelperItemSet_xmlHandler handler(this);

    // do parse (which uses handler to create actual data)
	CHelperSystem::GetMe()->GetXMLParser()->parseXMLFile(handler, strFileName, "");
}

/*************************************************************************
	Add a anchor item( userd by CHelpItem::AddChildItem);
*************************************************************************/
void CHelperItemSet::InsertAnchorItem(const String& strName, CHelperItem* pItem)
{
	if(strName.empty()) return;

	m_anchorMap.insert(std::make_pair(strName, pItem));
}

/*************************************************************************
	Search a item by anchor name
*************************************************************************/
CHelperItem* CHelperItemSet::FindItem_Anchor(const String& strAnchor)
{
	AnchorMap::iterator itFind = m_anchorMap.find(strAnchor);
	if(itFind == m_anchorMap.end()) return 0;

	return itFind->second;
}
