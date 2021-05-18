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

#ifndef __GLXWindowInterface_H__
#define __GLXWindowInterface_H__

#include "OgreRenderWindow.h"

namespace Ogre {

/** Interface that provides some GLX specific notification to an Ogre GLX window. Use if this
    is mandatory when you provide your own input system instead of the Ogre input system.
 */
class GLXWindowInterface
{
public:
    virtual ~GLXWindowInterface() = 0;
    
    /// Call this with true when the window is mapped/visible, false when the window is unmapped/invisible
    virtual void exposed(bool active) = 0;
    
    /// Call this to notify the window was resized
    virtual void resized(size_t width, size_t height) = 0;
};

};

#endif
