/****************************************\
*										*
*										*
*										*
* 		HelperItemSet_xmlHandler		*
*										*
*						 by jinchao		*
*						ref from CEGUI	*
\****************************************/

#pragma once

#include "HelperXMLHandler.h"

class CHelperItemSet;
class CHelperItem;
class CHelperItemSet_xmlHandler : public CHelperXMLHandler
{
public:
	/*************************************************************************
	Construction & Destruction
	*************************************************************************/
	/*!
	\brief
	Constructor for HelpItemSet:xmlHandler objects

	\param imageset
	Pointer to the HelpItemSet object creating this xmlHandler object
	*/
	CHelperItemSet_xmlHandler(CHelperItemSet* itemset);

	/*!
	\brief
	Destructor for HelpItemSet:xmlHandler objects
	*/
	virtual ~CHelperItemSet_xmlHandler(void) {}

	/*************************************************************************
	SAX2 Handler overrides
	*************************************************************************/ 
	/*!
	\brief
	document processing (only care about elements, schema validates format)
	*/
	virtual void elementStart(const String& element, const CHelperXMLAttributes& attributes);
	virtual void elementEnd(const String& element);

	/*************************************************************************
	Functions used by our implementation
	*************************************************************************/
	CHelperItemSet*	getHelpItemset(void) const				{return m_pItemSet;}

private:
	/*************************************************************************
	Implementation Constants
	*************************************************************************/
	static const String HelperItemSetElement;		//!< Tag name for ItemSet elements.
	static const String ItemElement;				//!< Tag name for Item elements.
	static const char	ItemNameAttribute[];		//!< Attribute name that stores the name of the item
	static const char	ItemAnchorAttribute[];		//!< Attribute name that stores the anchor for the item.
	static const char	ItemUrlAttribute[];			//!< Attribute name that stores the url for the item.

	/*************************************************************************
	Implementation Data
	*************************************************************************/
	CHelperItemSet*	m_pItemSet;			//!< Holds a pointer to the HelperItemSet that created the handler object

	typedef std::vector< CHelperItem* >	ItemStack;	
	CHelperItem*	m_root;				//!< Will point to first item(created by itemset!)

	ItemStack	m_stack;				//!< Stack used to keep track of what we're doing to which item.
};