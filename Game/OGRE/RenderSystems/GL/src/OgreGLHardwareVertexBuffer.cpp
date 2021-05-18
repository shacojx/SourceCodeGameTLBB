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
#include "OgreGLHardwareBufferManager.h"
#include "OgreGLHardwareVertexBuffer.h"
#include "OgreException.h"
#include "OgreLogManager.h"

namespace Ogre {

	//---------------------------------------------------------------------
    GLHardwareVertexBuffer::GLHardwareVertexBuffer(size_t vertexSize, 
        size_t numVertices, HardwareBuffer::Usage usage, bool useShadowBuffer)
        : HardwareVertexBuffer(vertexSize, numVertices, usage, false, useShadowBuffer)
    {
        glGenBuffersARB( 1, &mBufferId );

        if (!mBufferId)
        {
            OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR, 
                "Cannot create GL vertex buffer", 
                "GLHardwareVertexBuffer::GLHardwareVertexBuffer");
        }

        glBindBufferARB(GL_ARRAY_BUFFER_ARB, mBufferId);

        // Initialise mapped buffer and set usage
        glBufferDataARB(GL_ARRAY_BUFFER_ARB, mSizeInBytes, NULL, 
            GLHardwareBufferManager::getGLUsage(usage));

        //std::cerr << "creating vertex buffer = " << mBufferId << std::endl;
    }
	//---------------------------------------------------------------------
    GLHardwareVertexBuffer::~GLHardwareVertexBuffer()
    {
        glDeleteBuffersARB(1, &mBufferId);
    }
	//---------------------------------------------------------------------
    void* GLHardwareVertexBuffer::lockImpl(size_t offset, 
        size_t length, LockOptions options)
    {
        GLenum access = 0;

        if(mIsLocked)
        {
            OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR,
                "Invalid attempt to lock an index buffer that has already been locked",
                "GLHardwareIndexBuffer::lock");
        }

        glBindBufferARB(GL_ARRAY_BUFFER_ARB, mBufferId);

        if(options == HBL_DISCARD)
        {
            //TODO: really we should use this to indicate our discard of the buffer
            //However it makes no difference to fps on nVidia, and can crash some ATI
            //glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mSizeInBytes, NULL, 
            //    GLHardwareBufferManager::getGLUsage(mUsage));

            access = (mUsage == HBU_DYNAMIC || mUsage == HBU_STATIC) ? 
                GL_READ_WRITE_ARB : GL_WRITE_ONLY_ARB;

        }
        else if(options == HBL_READ_ONLY)
        {
            if(mUsage == HBU_WRITE_ONLY)
            {
                LogManager::getSingleton().logMessage(
                    "GLHardwareVertexBuffer: Locking a write-only vertex "
                    "buffer for reading, performance warning.");
            }
            access = GL_READ_ONLY_ARB;
        }
        else if(options == HBL_NORMAL || options == HBL_NO_OVERWRITE)
        {
            // TODO: we should be using the below implementation, but nVidia cards
            // choke on it and perform terribly - for investigation with nVidia
            //access = (mUsage == HBU_DYNAMIC || mUsage == HBU_STATIC) ? 
            //    GL_READ_WRITE_ARB : GL_WRITE_ONLY_ARB;
            access = GL_READ_WRITE;
        }
        else
        {
            OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR, 
                "Invalid locking option set", "GLHardwareVertexBuffer::lock");
        }

        void* pBuffer = glMapBufferARB( GL_ARRAY_BUFFER_ARB, access);

        if(pBuffer == 0)
        {
            OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR, 
                "Vertex Buffer: Out of memory", "GLHardwareVertexBuffer::lock");
        }

        mIsLocked = true;
        // return offsetted
        return static_cast<void*>(
            static_cast<unsigned char*>(pBuffer) + offset);
    }
	//---------------------------------------------------------------------
	void GLHardwareVertexBuffer::unlockImpl(void)
    {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, mBufferId);

        if(!glUnmapBufferARB( GL_ARRAY_BUFFER_ARB ))
        {
            OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR, 
                "Buffer data corrupted, please reload", 
                "GLHardwareVertexBuffer::unlock");
        }

        mIsLocked = false;
    }
	//---------------------------------------------------------------------
    void GLHardwareVertexBuffer::readData(size_t offset, size_t length, 
        void* pDest)
    {
        if(mUseShadowBuffer)
        {
            // get data from the shadow buffer
            void* srcData = mpShadowBuffer->lock(offset, length, HBL_READ_ONLY);
            memcpy(pDest, srcData, length);
            mpShadowBuffer->unlock();
        }
        else
        {
            // get data from the real buffer
            glBindBufferARB(GL_ARRAY_BUFFER_ARB, mBufferId);
        
            glGetBufferSubDataARB(GL_ARRAY_BUFFER_ARB, offset, length, pDest);
        }
    }
	//---------------------------------------------------------------------
    void GLHardwareVertexBuffer::writeData(size_t offset, size_t length, 
            const void* pSource, bool discardWholeBuffer)
    {
        glBindBufferARB(GL_ARRAY_BUFFER_ARB, mBufferId);

        // Update the shadow buffer
        if(mUseShadowBuffer)
        {
            void* destData = mpShadowBuffer->lock(offset, length, 
                discardWholeBuffer ? HBL_DISCARD : HBL_NORMAL);
            memcpy(destData, pSource, length);
            mpShadowBuffer->unlock();
        }

        if(discardWholeBuffer)
        {
            glBufferDataARB(GL_ARRAY_BUFFER_ARB, mSizeInBytes, NULL, 
                GLHardwareBufferManager::getGLUsage(mUsage));
        }

        // Now update the real buffer
        glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, offset, length, pSource); 
    }
	//---------------------------------------------------------------------

}
