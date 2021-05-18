/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General  License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General  License for more details.

You should have received a copy of the GNU Lesser General  License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.

-----------------------------------------------------------------------------
*/

/***************************************************************************
TargetManager.h  -  
	an abstract interface.
	Implemented by OverlayManager, to return a GuiElement at a given x,y 
	position. 

	Custom TargetManagers can be written to get a 3D element from an x,y 
	position so you can write mouse listeners for your 3D objects.

-------------------
begin                : Dec 02 2002
copyright            : (C) 2002 by Kenny Sabir
email                : kenny@sparksuit.com
***************************************************************************/


#ifndef __TargetManager_H__
#define __TargetManager_H__

#include "OgrePrerequisites.h"

namespace Ogre {

	/** An abstract interface, implemented by OverlayManager, to return a GuiElement at a given x,y 
		position. 
	@remarks
		Custom TargetManagers can be written to get a 3D element from an x,y 
		position so you can write mouse listeners for your 3D objects.
	*/
	class _OgreExport TargetManager 
    {
	public:
		virtual PositionTarget* getPositionTargetAt(Real x, Real y) = 0;
    };


}
#endif 

