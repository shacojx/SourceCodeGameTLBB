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
OgreEventTarget.h  -  
	This is an abstract class that is the base class of all Targets.

	All targets can processEvents. This function should call the respective
		event process method depending on what type of event is sent. 

-------------------
begin                : Nov 19 2002
copyright            : (C) 2002 by Kenny Sabir
email                : kenny@sparksuit.com
***************************************************************************/

#ifndef __EventTarget_H__
#define __EventTarget_H__

#include "OgrePrerequisites.h"

namespace Ogre {

	/**	This is an abstract class that is the base class of all consumers of InputEvent instances.
	@remarks
		This class is part of the set of classes handling buffered input. 
	*/
    class _OgreExport EventTarget
    {
	public:
		virtual void processEvent(InputEvent* e) = 0;
		virtual ~EventTarget() {}
    };
}


#endif //__EventTarget_H__
