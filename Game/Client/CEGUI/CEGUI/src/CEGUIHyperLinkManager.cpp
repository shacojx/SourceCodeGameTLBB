/*************************************************************************
    Crazy Eddie's GUI System (http://www.cegui.org.uk)
    Copyright (C)2004 - 2005 Paul D Turner (paul@cegui.org.uk)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*************************************************************************/

#include "CEGUIHyperLinkManager.h"
#include "CEGUIHyperLink.h"
#include "CEGUILogger.h"

// Start of CEGUI namespace section
namespace CEGUI
{
	const String HyperLinkManager::EventNamespace("HyperLinkManager");

    //////////////////////////////////////////////////////////////////////////
    // Event name constants
    const String HyperLinkManager::EventHyperLinkActive("HyperLinkActive");
	const String HyperLinkManager::EventHyperLinkLeftActive("HyperLinkLeftActive");
	const String HyperLinkManager::EventHyperLinkRightActive("HyperLinkRightActive");
    const String HyperLinkManager::EventHyperLinkInactive("HyperLinkInactive");
	//////////////////////////////////////////////////////////////////////////

/*************************************************************************
	Static Data Definitions
*************************************************************************/
// singleton instance pointer
template<> HyperLinkManager* Singleton<HyperLinkManager>::ms_Singleton	= NULL;



/*************************************************************************
	constructor
*************************************************************************/
HyperLinkManager::HyperLinkManager(void)
{
	Logger::getSingleton().logEvent((utf8*)"CEGUI::HyperLinkManager singleton created.");
}

/*************************************************************************
	Destructor
*************************************************************************/
HyperLinkManager::~HyperLinkManager(void)
{
	Logger::getSingleton().logEvent((utf8*)"CEGUI::FontManager singleton destroyed.");
}

HyperLinkManager& HyperLinkManager::getSingleton(void)
{
	return Singleton<HyperLinkManager>::getSingleton();
}


HyperLinkManager* HyperLinkManager::getSingletonPtr(void)
{
	return Singleton<HyperLinkManager>::getSingletonPtr();
}
/*
void HyperLinkManager::clearHyperLinkList(void)
{
	d_HyperLinkList.clear();
	d_HyperLinkNameMap.clear();
}
*/

/*************************************************************************
	Destroys the Animate with the specified name
*************************************************************************/
	void HyperLinkManager::destroyHyperLink(const String name)
	{
		HyperLinkNameRegistry::iterator pos = d_HyperLinkNameMap.find(name);

		if (pos != d_HyperLinkNameMap.end())
		{
			pos->second.CleanUp();

			d_HyperLinkNameMap.erase(pos);
		}
	}


/*************************************************************************
	Destroys all Animate objects registered in the system
*************************************************************************/
	void HyperLinkManager::destroyAllHyperLink(void)
	{
		while (!d_HyperLinkNameMap.empty())
		{
			destroyHyperLink(d_HyperLinkNameMap.begin()->first);
		}
	}


void HyperLinkManager::addHyperLink(const Window* OwnerWindow,const String name, const Rect hyper_rect )
{
	// first ensure name uniqueness

	HyperLinkNameRegistry::iterator pos = d_HyperLinkNameMap.find(name);

	if (pos != d_HyperLinkNameMap.end())
	{
		pos->second.addNewRect(OwnerWindow,hyper_rect);
	}
	else
	{
		HyperLink newHyperLink;
		newHyperLink.CleanUp();
		newHyperLink.d_name = name;
		newHyperLink.addNewRect(OwnerWindow,hyper_rect);

		d_HyperLinkNameMap[name] = newHyperLink;
	}
		return;
}

bool HyperLinkManager::isHyperLinkRange(const Window* pOwnerWindow, const Point& position)
{
	HyperLinkNameRegistry::const_iterator itCur, itEnd;
	HyperLink nHyperLink;

	itCur		= d_HyperLinkNameMap.begin();
	itEnd		= d_HyperLinkNameMap.end();

	while ( itCur != itEnd )
	{
		nHyperLink = itCur->second;

		if(nHyperLink.isInRange(pOwnerWindow,position))
		{
			HyperLinkEventArgs e(pOwnerWindow->getName(), nHyperLink.d_name);

			fireEvent(EventHyperLinkInactive, e, EventNamespace);		
			return true;
		}

		itCur++;
	}
	return false;
}

String HyperLinkManager::getHyperLinkContex(const Window* pOwnerWindow, const Point& position)
{
	HyperLinkNameRegistry::const_iterator itCur, itEnd;
	HyperLink nHyperLink;

	itCur		= d_HyperLinkNameMap.begin();
	itEnd		= d_HyperLinkNameMap.end();

	while ( itCur != itEnd )
	{
		nHyperLink = itCur->second;

		if(nHyperLink.isInRange(pOwnerWindow,position))
		{
			HyperLinkEventArgs e(pOwnerWindow->getName(), nHyperLink.d_name);
			return nHyperLink.d_name;
		}

		itCur++;
	}
	return "";
}

bool HyperLinkManager::doHyperLink(const Window* pOwnerWindow,const Point& position)
{

	HyperLinkNameRegistry::const_iterator itCur, itEnd;
	HyperLink nHyperLink;

	itCur		= d_HyperLinkNameMap.begin();
	itEnd		= d_HyperLinkNameMap.end();

	while ( itCur != itEnd )
	{
		nHyperLink = itCur->second;

		if(nHyperLink.isInRange(pOwnerWindow,position))
		{
			HyperLinkEventArgs e(pOwnerWindow->getName(), nHyperLink.d_name);

			fireEvent(EventHyperLinkActive, e, EventNamespace);
			return true;
		}

		itCur++;
	}
	return false;

}

bool HyperLinkManager::doHyperLinkByMouseLeft(const Window* pOwnerWindow,const Point& position)
{

	HyperLinkNameRegistry::const_iterator itCur, itEnd;
	HyperLink nHyperLink;

	itCur		= d_HyperLinkNameMap.begin();
	itEnd		= d_HyperLinkNameMap.end();

	while ( itCur != itEnd )
	{
		nHyperLink = itCur->second;

		if(nHyperLink.isInRange(pOwnerWindow,position))
		{
			HyperLinkEventArgs e(pOwnerWindow->getName(), nHyperLink.d_name);

			fireEvent(EventHyperLinkLeftActive, e, EventNamespace);
			return true;
		}

		itCur++;
	}

	return false;
}

bool HyperLinkManager::doHyperLinkByMouseRight(const Window* pOwnerWindow,const Point& position)
{

	HyperLinkNameRegistry::const_iterator itCur, itEnd;
	HyperLink nHyperLink;

	itCur		= d_HyperLinkNameMap.begin();
	itEnd		= d_HyperLinkNameMap.end();

	while ( itCur != itEnd )
	{
		nHyperLink = itCur->second;

		if(nHyperLink.isInRange(pOwnerWindow,position))
		{
			HyperLinkEventArgs e(pOwnerWindow->getName(), nHyperLink.d_name);

			fireEvent(EventHyperLinkRightActive, e, EventNamespace);
			return true;
		}

		itCur++;
	}
	return false;

}

}// End of  CEGUI namespace section

