/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/
/***************************************************************************
OgreMouseTarget.h  -  
	Handles the adding and removing of MouseListeners.

	Components that process the Mouse Event should subclass this class 
	and call processMouseEvent when that event is identified.

-------------------
begin                : Nov 19 2002
copyright            : (C) 2002 by Kenny Sabir
email                : kenny@sparksuit.com
***************************************************************************/

#ifndef __MouseTarget_H__
#define __MouseTarget_H__

#include "OgrePrerequisites.h"
#include "OgrePositionTarget.h"

namespace Ogre {

	
	/** Handles the adding and removing of MouseListeners.
	@remarks
		Components that process the Mouse Event should subclass this class 
		and call processMouseEvent when that event is identified.
	*/
	class _OgreExport MouseTarget : public PositionTarget
    {
    private:
        std::set<MouseListener*> mRemovedListeners;
    
    protected:
        std::set<MouseListener*> mMouseListeners;

		// is mouse inside the object
		bool mMouseWithin;

    public:
		MouseTarget();
        virtual ~MouseTarget() { }

		void processMouseEvent(MouseEvent* e) ;
		void addMouseListener(MouseListener* l) ;
		void removeMouseListener(MouseListener* l) ;
		bool isMouseWithin() const;
    };



}


#endif //__MouseTarget_H__
