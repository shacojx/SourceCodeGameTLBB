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
#ifndef _CEGUIHyperLinkManager_h_
#define _CEGUIHyperLinkManager_h_

#include "CEGUIBase.h"
#include "CEGUISingleton.h"
#include "CEGUIHyperLink.h"
//#include <vector>
#include <map>

// Start of CEGUI namespace section
namespace CEGUI
{

class CEGUIEXPORT HyperLinkManager : public Singleton<HyperLinkManager>, public EventSet
{
public:
	/*!
	\brief
		Constructor for HyperLinkManager objects
	*/
	HyperLinkManager(void);


	/*!
	\brief
		Destructor for HyperLinkManager objects
	*/
	~HyperLinkManager(void);

	/*!
	\brief
		Return singleton HyperLinkManager object

	\return
		Singleton HyperLinkManager object
	*/
	static	HyperLinkManager&	getSingleton(void);


	/*!
	\brief
		Return pointer to singleton HyperLinkManager object

	\return
		Pointer to singleton HyperLinkManager object
	*/
	static	HyperLinkManager*	getSingletonPtr(void);

public:
		/*************************************************************************
            Constants
        *************************************************************************/
		static const String EventNamespace;						//!< Namespace for global events

		static const String EventHyperLinkActive;             //!< Event fired when the HyperLink is about to get activated.
		static const String EventHyperLinkLeftActive;			//!< Event fired by MouseLeft Active
		static const String EventHyperLinkRightActive;			//!< Event fired by MouseRight Active
        static const String EventHyperLinkInactive;           //!< Event fired when the HyperLink has been deactivated.
		/*!
		\brief
			Clears all hyperlink.
		
		\return
			Nothing
		*/
//		void clearHyperLinkList(void);
		void destroyAllHyperLink(void);
		void destroyHyperLink(const String name);

		void addHyperLink(const Window* OwnerWindow, const String name, const Rect hyper_rect);
		bool isHyperLinkRange(const Window* pOwnerWindow,const Point& position);
		bool doHyperLink(const Window* pOwnerWindow,const Point& position);

		//add for MouseLeft Click & MouseRight Click on HyperLink
		bool doHyperLinkByMouseLeft(const Window* pOwnerWindow,const Point& position);
		bool doHyperLinkByMouseRight(const Window* pOwnerWindow,const Point& position);

		//get HyperLink content from Window and Position
		String getHyperLinkContex(const Window* pOwnerWindow, const Point& position);
private:

	typedef std::map< String, HyperLink > HyperLinkNameRegistry;
	//All HyperLink
	HyperLinkNameRegistry	d_HyperLinkNameMap;

//	typedef std::vector< HyperLink > HyperLinkList;
//	HyperLinkList			d_HyperLinkList;

};

}// End of  CEGUI namespace section

#endif	// end of guard _CEGUIHyperLinkManager_h_