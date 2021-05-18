#include "StdAfx.h"
#include "HelperItemSet_XmlHandler.h"
#include "HelperItemSet.h"
#include "HelperItem.h"
#include "HelperXMLAttributes.h"
#include "HelperSystem.h"

/*************************************************************************
Definition of constant data for Imageset (and sub-classes)
*************************************************************************/
// Declared in Imageset::xmlHandler
const String CHelperItemSet_xmlHandler::HelperItemSetElement("HelperItemSet");
const String CHelperItemSet_xmlHandler::ItemElement("Item");

const char	CHelperItemSet_xmlHandler::ItemNameAttribute[]		= "name";
const char	CHelperItemSet_xmlHandler::ItemAnchorAttribute[]	= "anchor";
const char	CHelperItemSet_xmlHandler::ItemUrlAttribute[]		= "url";


CHelperItemSet_xmlHandler::CHelperItemSet_xmlHandler(CHelperItemSet* itemset) : 
	m_pItemSet(itemset) 
{
	m_root = m_pItemSet->GetRootItem();

	m_stack.push_back(m_root);
}

/*************************************************************************
SAX2 Handler methods
*************************************************************************/
void CHelperItemSet_xmlHandler::elementStart(const String& element, const CHelperXMLAttributes& attributes)
{
	// handle an ItemSet element.
	if (element == HelperItemSetElement)
	{
		// do nothing...
	}
	// handle an item element
	else if(element == ItemElement)
	{
		// get name of item.
        String itemName(attributes.getValueAsString(ItemNameAttribute));

		// get anchor for new item
        String itemAnchor(attributes.getValueAsString(ItemAnchorAttribute));
		
		// get url for new item
        String itemUrl(attributes.getValueAsString(ItemUrlAttribute));

		// url convert(MFC)
		if(!itemUrl.empty())
		{
			DWORD dwServiceType;
			CString strServer, strObject;
			INTERNET_PORT nPort;

			String strFullURL;
			AfxParseURL(itemUrl.c_str(), dwServiceType, strServer, strObject, nPort);
			if(AFX_INET_SERVICE_UNK == dwServiceType)
			{
				//local file
				itemUrl = CHelperSystem::GetMe()->GetWorkDictory() + itemUrl;
			}
			else
			{
				itemUrl = itemUrl;
			}
		}

		// create new item
		CHelperItem item(itemName, itemAnchor, itemUrl);

		// add this item to the current parent (if any)
		if (!m_stack.empty())
		{
			m_stack.push_back(m_stack.back()->AddChildItem(item));
		}
		else
		{
			throw std::exception("HelperItemSet::xmlHandler::startElement item hiberarchy error!");
		}
	}
	else
	{
		char szException[MAX_PATH];
		_snprintf(szException, MAX_PATH, 
			"HelperItemSet::xmlHandler::startElement - Unexpected data was found while parsing the Imageset file: '%s' is unknown.", 
			element.c_str());

		throw std::exception(szException);
	}
}

void CHelperItemSet_xmlHandler::elementEnd(const String& element)
{
	// handle root HelperItemSet Element
	if (element == HelperItemSetElement)
	{
		// do nothing...
	}
	// handle Item Element
	else if (element == ItemElement)
	{
		// pop a item from the item stack
		if (!m_stack.empty())
		{
			m_stack.pop_back();
		}
		else
		{
			throw std::exception("HelperItemSet::xmlHandler::startElement item hiberarchy error!");
		}
	}
}
