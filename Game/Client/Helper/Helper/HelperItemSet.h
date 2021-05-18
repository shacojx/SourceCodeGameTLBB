/****************************************\
*										*
* 			   HelperItem				*
*										*
*						 by jinchao		*
\****************************************/

#pragma once

class CHelperItem;
class CHelperItemSet
{
public:
	//	Get the root item
	CHelperItem*	GetRootItem(void) { return m_rootItem; }

	// Load all items from itemset file.
	void			LoadItemSet(const String& strFileName);

public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	CHelperItemSet();
	virtual ~CHelperItemSet();

	// add a anchor item( userd by CHelpItem::AddChildItem);
	void	InsertAnchorItem(const String& strName, CHelperItem* pItem);
	
	// search a item by anchor name
	CHelperItem* FindItem_Anchor(const String& strAnchor);

protected:

	/*************************************************************************
		Implementation Data
	*************************************************************************/
	CHelperItem*	m_rootItem;		// Root item that hold all items.

	// Anchor map
	typedef std::map< String, CHelperItem* > AnchorMap;
	AnchorMap		m_anchorMap;
	
private:
    // unimplemented constructors / assignment
    CHelperItemSet(const CHelperItemSet& obj);
    CHelperItemSet& operator=(const CHelperItemSet& obj);
};