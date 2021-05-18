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
OgrePositionTarget.h  -  
	A position target is a component that can be targeted by any mouse click or 
	event that relies on an xy position.

	It supports nesting, so the left and top are relative to its parent.

-------------------
begin                : Nov 19 2002
copyright            : (C) 2002 by Kenny Sabir
email                : kenny@sparksuit.com
***************************************************************************/


#ifndef __PositionTarget_H__
#define __PositionTarget_H__

#include "OgrePrerequisites.h"
#include "OgreEventTarget.h"

namespace Ogre {

    class _OgreExport PositionTarget : public EventTarget
    {
	public:
		virtual ~PositionTarget() {}
        /** Gets the left of this element in relation to the screen (where 0 = far left, 1.0 = far right)  */
        virtual Real getLeft(void) const = 0;

        /** Gets the top of this element in relation to the screen (where 0 = top, 1.0 = bottom)  */
        virtual Real getTop(void) const = 0;

		virtual PositionTarget* getPositionTargetParent() const = 0;

		virtual bool isKeyEnabled() const = 0;

    };
}


#endif //__EventTarget_H__
