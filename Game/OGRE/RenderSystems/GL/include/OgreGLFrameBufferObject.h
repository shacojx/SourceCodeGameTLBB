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
#ifndef __OgreGLFBO_H__
#define __OgreGLFBO_H__

#include "OgreGLRenderTexture.h"
#include "OgreGLContext.h"

namespace Ogre {
    
    /** Frame Buffer Object abstraction.
    */
    class GLFrameBufferObject
    {
    public:
        GLFrameBufferObject(GLFBOManager *manager);
        ~GLFrameBufferObject();
        //void bindSurface(size_t attachment, RenderTarget *target);
        /** Bind a surface to a certain attachment point.
            attachment: 0..OGRE_MAX_MULTIPLE_RENDER_TARGETS-1
        */
        void bindSurface(size_t attachment, const GLSurfaceDesc &target);
        /** Unbind attachment
        */
        void unbindSurface(size_t attachment);
        
        /** Bind FrameBufferObject
        */
        void bind();
        
        /// Accessors
        size_t getWidth();
        size_t getHeight();
        PixelFormat getFormat();
        
        GLFBOManager *getManager() { return mManager; }
		const GLSurfaceDesc &getSurface(size_t attachment) { return mColour[attachment]; }
    private:
        GLFBOManager *mManager;
        GLuint mFB;
        GLSurfaceDesc mDepth;
        GLSurfaceDesc mStencil;
        // Arbitrary number of texture surfaces
        GLSurfaceDesc mColour[OGRE_MAX_MULTIPLE_RENDER_TARGETS];

		/** Initialise object (find suitable depth and stencil format).
            Must be called every time the bindings change.
            It fails with an exception (ERR_INVALIDPARAMS) if:
            - Attachment point 0 has no binding
            - Not all bound surfaces have the same size
            - Not all bound surfaces have the same internal format
        */
        void initialise();
    };

}

#endif
