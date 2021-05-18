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

#include "OgreGLFrameBufferObject.h"
#include "OgreGLPixelFormat.h"
#include "OgreLogManager.h"
#include "OgreStringConverter.h"
#include "OgreRoot.h"
#include "OgreGLHardwarePixelBuffer.h"
#include "OgreGLFBORenderTexture.h"

namespace Ogre {

//-----------------------------------------------------------------------------
    GLFrameBufferObject::GLFrameBufferObject(GLFBOManager *manager):
        mManager(manager)
    {
        /// Generate framebuffer object
        glGenFramebuffersEXT(1, &mFB);
        /// Initialise state
        mDepth.buffer=0;
        mStencil.buffer=0;
        for(size_t x=0; x<OGRE_MAX_MULTIPLE_RENDER_TARGETS; ++x)
        {
            mColour[x].buffer=0;
        }
    }
    GLFrameBufferObject::~GLFrameBufferObject()
    {
        mManager->releaseRenderBuffer(mDepth);
        mManager->releaseRenderBuffer(mStencil);
        /// Delete framebuffer object
        glDeleteFramebuffersEXT(1, &mFB);        
    }
    void GLFrameBufferObject::bindSurface(size_t attachment, const GLSurfaceDesc &target)
    {
        assert(attachment < OGRE_MAX_MULTIPLE_RENDER_TARGETS);
        mColour[attachment] = target;
		// Re-initialise
		if(mColour[0].buffer)
			initialise();
    }
    void GLFrameBufferObject::unbindSurface(size_t attachment)
    {
        assert(attachment < OGRE_MAX_MULTIPLE_RENDER_TARGETS);
        mColour[attachment].buffer = 0;
		// Re-initialise if buffer 0 still bound
		if(mColour[0].buffer)
		{
			initialise();
		}
    }
    void GLFrameBufferObject::initialise()
    {
		// Release depth and stencil, if they were bound
        mManager->releaseRenderBuffer(mDepth);
        mManager->releaseRenderBuffer(mStencil);
        /// First buffer must be bound
        if(!mColour[0].buffer)
        {
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
            "Attachment 0 must have surface attached",
		 	"GLFrameBufferObject::initialise");
        }
        /// Bind FBO to frame buffer
        bind();
        /// Store basic stats
        size_t width = mColour[0].buffer->getWidth();
        size_t height = mColour[0].buffer->getHeight();
        GLuint format = mColour[0].buffer->getGLFormat();
        PixelFormat ogreFormat = mColour[0].buffer->getFormat();
        /// Bind all attachment points to frame buffer
        for(size_t x=0; x<OGRE_MAX_MULTIPLE_RENDER_TARGETS; ++x)
        {
            if(mColour[x].buffer)
            {
                if(mColour[x].buffer->getWidth() != width || mColour[x].buffer->getHeight() != height)
                {
                    std::stringstream ss;
                    ss << "Attachment " << x << " has incompatible size ";
                    ss << mColour[x].buffer->getWidth() << "x" << mColour[x].buffer->getHeight();
                    ss << ". It must be of the same as the size of surface 0, ";
                    ss << width << "x" << height;
                    ss << ".";
                    OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, ss.str(), "GLFrameBufferObject::initialise");
                }
                if(mColour[x].buffer->getGLFormat() != format)
                {
                    std::stringstream ss;
                    ss << "Attachment " << x << " has incompatible format.";
                    OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, ss.str(), "GLFrameBufferObject::initialise");
                }
	            mColour[x].buffer->bindToFramebuffer(GL_COLOR_ATTACHMENT0_EXT+x, mColour[x].zoffset);
            }
            else
            {
                // Detach
                glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT+x,
                    GL_RENDERBUFFER_EXT, 0);
            }
        }
        /// Find suitable depth and stencil format that is compatible with colour format
        GLenum depthFormat, stencilFormat;
        mManager->getBestDepthStencil(ogreFormat, &depthFormat, &stencilFormat);
        
        /// Request surfaces
        mDepth = mManager->requestRenderBuffer(depthFormat, width, height);
		if (depthFormat == GL_DEPTH24_STENCIL8_EXT)
		{
			// bind same buffer to depth and stencil attachments
            mManager->requestRenderBuffer(mDepth);
			mStencil = mDepth;
		}
		else
		{
			// separate stencil
			mStencil = mManager->requestRenderBuffer(stencilFormat, width, height);
		}
        
        /// Attach/detach surfaces
        if(mDepth.buffer)
        {
            mDepth.buffer->bindToFramebuffer(GL_DEPTH_ATTACHMENT_EXT, mDepth.zoffset);
        }
        else
        {
            glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
                GL_RENDERBUFFER_EXT, 0);
        }
        if(mStencil.buffer)
        {
            mStencil.buffer->bindToFramebuffer(GL_STENCIL_ATTACHMENT_EXT, mStencil.zoffset);
        }
        else
        {
            glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT,
                GL_RENDERBUFFER_EXT, 0);
        }

		/// Do glDrawBuffer calls
		GLenum bufs[OGRE_MAX_MULTIPLE_RENDER_TARGETS];
		GLsizei n=0;
		for(size_t x=0; x<OGRE_MAX_MULTIPLE_RENDER_TARGETS; ++x)
		{
			// Fill attached colour buffers
			if(mColour[x].buffer)
			{
				bufs[x] = GL_COLOR_ATTACHMENT0_EXT + x;
				// Keep highest used buffer + 1
				n = x+1;
			}
			else
			{
				bufs[x] = GL_NONE;
			}
		}
		if(glDrawBuffers)
		{
			/// Drawbuffer extension supported, use it
			glDrawBuffers(n, bufs);
		}
		else
		{
			/// In this case, the capabilities will not show more than 1 simultaneaous render target.
			glDrawBuffer(bufs[0]);
		}
		/// No read buffer, by default, if we want to read anyway we must not forget to set this.
		glReadBuffer(GL_NONE);
        
        /// Check status
        GLuint status;
        status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
        
        /// Bind main buffer
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        
        switch(status)
        {
        case GL_FRAMEBUFFER_COMPLETE_EXT:
            // All is good
            break;
        case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
            "All framebuffer formats with this texture internal format unsupported",
		 	"GLFrameBufferObject::initialise");
        default:
            OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
            "Framebuffer incomplete or other FBO status error",
		 	"GLFrameBufferObject::initialise");
        }
        
    }
    void GLFrameBufferObject::bind()
    {
        /// Bind it to FBO
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, mFB);
    }
    size_t GLFrameBufferObject::getWidth()
    {
        assert(mColour[0].buffer);
        return mColour[0].buffer->getWidth();
    }
    size_t GLFrameBufferObject::getHeight()
    {
        assert(mColour[0].buffer);
        return mColour[0].buffer->getHeight();
    }
    PixelFormat GLFrameBufferObject::getFormat()
    {
        assert(mColour[0].buffer);
        return mColour[0].buffer->getFormat();
    }
//-----------------------------------------------------------------------------
}
