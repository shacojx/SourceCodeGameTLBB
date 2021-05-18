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
#include "OgreGLHardwareIndexBuffer.h"
#include "OgreHardwareBuffer.h"

namespace Ogre {
    //-----------------------------------------------------------------------
    GLHardwareBufferManager::GLHardwareBufferManager()
    {
    }
    //-----------------------------------------------------------------------
    GLHardwareBufferManager::~GLHardwareBufferManager()
    {
        destroyAllDeclarations();
        destroyAllBindings();
    }
    //-----------------------------------------------------------------------
    HardwareVertexBufferSharedPtr GLHardwareBufferManager::createVertexBuffer(
        size_t vertexSize, size_t numVerts, HardwareBuffer::Usage usage, bool useShadowBuffer)
    {
		GLHardwareVertexBuffer* buf = 
			new GLHardwareVertexBuffer(vertexSize, numVerts, usage, useShadowBuffer);
		mVertexBuffers.insert(buf);
		return HardwareVertexBufferSharedPtr(buf);
    }
    //-----------------------------------------------------------------------
    HardwareIndexBufferSharedPtr 
    GLHardwareBufferManager::createIndexBuffer(
        HardwareIndexBuffer::IndexType itype, size_t numIndexes, 
        HardwareBuffer::Usage usage, bool useShadowBuffer)
    {
		GLHardwareIndexBuffer* buf = 
			new GLHardwareIndexBuffer(itype, numIndexes, usage, useShadowBuffer);
		mIndexBuffers.insert(buf);
		return HardwareIndexBufferSharedPtr(buf);
    }
    //---------------------------------------------------------------------
    GLenum GLHardwareBufferManager::getGLUsage(unsigned int usage)
    {
        switch(usage)
        {
        case HardwareBuffer::HBU_STATIC:
        case HardwareBuffer::HBU_STATIC_WRITE_ONLY:
            return GL_STATIC_DRAW_ARB;
        case HardwareBuffer::HBU_DYNAMIC:
        case HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY:
            return GL_DYNAMIC_DRAW_ARB;
        case HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE:
            return GL_STREAM_DRAW_ARB;
        default:
            return GL_DYNAMIC_DRAW_ARB;
        };
    }
    //---------------------------------------------------------------------
    GLenum GLHardwareBufferManager::getGLType(unsigned int type)
    {
        switch(type)
        {
            case VET_FLOAT1:
            case VET_FLOAT2:
            case VET_FLOAT3:
            case VET_FLOAT4:
                return GL_FLOAT;
            case VET_SHORT1:
            case VET_SHORT2:
            case VET_SHORT3:
            case VET_SHORT4:
                return GL_SHORT;
            case VET_COLOUR:
			case VET_COLOUR_ABGR:
			case VET_COLOUR_ARGB:
            case VET_UBYTE4:
                return GL_UNSIGNED_BYTE;
            default:
                return 0;
        };
    }
}
