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
#ifndef __OgreGLContext_H__
#define __OgreGLContext_H__

#include "OgreGLPrerequisites.h"

namespace Ogre {

    /**
     * Class that encapsulates an GL context. (IE a window/pbuffer). This is a 
     * virtual base class which should be implemented in a GLSupport.
     * This object can also be used to cache renderstate if we decide to do so
     * in the future.
     */
    class GLContext
    {
    public:
        GLContext();
        virtual ~GLContext();

        /**
         * Enable the context. All subsequent rendering commands will go here.
         */
        virtual void setCurrent() = 0;
        /**
         * This is called before another context is made current. By default,
         * nothing is done here.
         */
        virtual void endCurrent();
        
        bool getInitialized() { return initialized; };
        void setInitialized() { initialized = true; };
    protected:
        bool initialized;
    };
}

#endif
