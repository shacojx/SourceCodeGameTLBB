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
#ifndef __SfxPluginPrerequisites_H__
#define __SfxPluginPrerequisites_H__

#include "OgrePrerequisites.h"

//-----------------------------------------------------------------------
// Windows Settings
//-----------------------------------------------------------------------
//#if (OGRE_PLATFORM == OGRE_PLATFORM_WIN32) && !defined(__MINGW32__)
//#   if (OGRE_VERSION >= 0x010300 && defined(OGRE_STATIC_LIB)) ||    \
//       (OGRE_VERSION < 0x010300 && OGRE_DYNAMIC_LINKAGE == 0)
//#       pragma message "No dynamic linkage"
#       define _OgreParticleFX2Export
//#   else
//#       ifdef PLUGIN_ParticleFX2_EXPORTS
//#           define _OgreParticleFX2Export __declspec(dllexport)
//#       else
//#           define _OgreParticleFX2Export __declspec(dllimport)
//#       endif
//#   endif
//#else
//#   define _OgreParticleFX2Export
//#endif


namespace Ogre {

    // Predeclare classes
}


#endif
