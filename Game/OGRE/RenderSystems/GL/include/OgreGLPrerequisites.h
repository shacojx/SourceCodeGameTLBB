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
#ifndef __GLPrerequisites_H__
#define __GLPrerequisites_H__

#include "OgrePrerequisites.h"

namespace Ogre {
    // Forward declarations
    class GLSupport;
    class GLRenderSystem;
    class GLTexture;
    class GLTextureManager;
    class GLGpuProgram;
    class GLContext;
    class GLRTTManager;
    class GLFBOManager;
    class GLHardwarePixelBuffer;
    class GLRenderBuffer;
}

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#if !defined( __MINGW32__ )
#   define NOMINMAX // required to stop windows.h messing up std::min
#endif
#   include <windows.h>
#   include <wingdi.h>
#   include <GL/glew.h>
#   include <GL/wglew.h>
#   include <GL/glu.h>
#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX
#   include <GL/glew.h>
#   include <GL/glu.h>
#   define GL_GLEXT_PROTOTYPES
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#   include <GL/glew.h>
#   include <OpenGL/glu.h>
#endif




#ifdef  OGRE_DEBUG_MODE

#ifndef GL_ERROR_EXCEPT

#define OGRE_GL_GETERROR(ERROR_MSG) {const GLubyte *errString; \
    GLenum errCode = glGetError(); \
    if (errCode != GL_NO_ERROR) {  \
    errString = gluErrorString (errCode);  \
    LogManager::getSingleton().logMessage  ("[GL] :" + Ogre::String(ERROR_MSG) +  \
    " : " + Ogre::String( (const char*) errString)); \
        } \
    }

#else //GL_ERROR_EXCEPT

#define OGRE_GL_GETERROR(ERROR_MSG) {const GLubyte *errString; \
    GLenum errCode = glGetError(); \
    if (errCode != GL_NO_ERROR) {  \
    errString = gluErrorString (errCode);  \
    OGRE_EXCEPT (Exception::ERR_INTERNAL_ERROR,  \
    Ogre::String(ERROR_MSG) +  \
    " : " + Ogre::String( (const char*) errString), StringUtil::BLANK); \
        } \
    }

#endif //GL_ERROR_EXCEPT

#else //OGRE_DEBUG_MODE

#define OGRE_GL_GETERROR()

#endif //OGRE_DEBUG_MODE

#endif
