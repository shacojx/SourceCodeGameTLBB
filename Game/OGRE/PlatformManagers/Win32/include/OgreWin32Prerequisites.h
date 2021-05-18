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
#ifndef _Win32Prerequisites_H__
#define _Win32Prerequisites_H__

#include "OgrePrerequisites.h"


#define WIN32_LEAN_AND_MEAN
#include "windows.h"


/* If you don't want any DirectX dependency at all
 */

//#define OGRE_NO_DX_INPUT


/* If you don't whant your build to request dxinput8, 
    for example in case u whant to run on a DirectX7 machine.
	This is stil request  DirectX 8 sdk for building, but the binary requires
    only DirectX 7
*/
//#define DX7INPUTONLY 

#ifndef OGRE_NO_DX_INPUT

#ifdef DX7INPUTONLY 
#define DIRECTINPUT_VERSION 0x0700

#else 
/**
    This has to be done in order to make sure the file compiles correctly under DirectX 8
*/
#define DIRECTINPUT_VERSION 0x0800
#endif 

#endif // OGRE_NO_DX_INPUT

namespace Ogre {

    // Predeclare classes 
    class Win32ConfigDialog;
    class Win32ErrorDialog;
    class Win32Window;


}


#endif
