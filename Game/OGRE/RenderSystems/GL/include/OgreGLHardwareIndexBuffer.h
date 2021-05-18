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
#ifndef __GLHARDWAREINDEXBUFFER_H__
#define __GLHARDWAREINDEXBUFFER_H__

#include "OgreGLPrerequisites.h"
#include "OgreHardwareIndexBuffer.h"

namespace Ogre { 


    class GLHardwareIndexBuffer : public HardwareIndexBuffer
    {
    private:
        GLuint mBufferId;
    protected:
        /** See HardwareBuffer. */
        void* lockImpl(size_t offset, size_t length, LockOptions options);
        /** See HardwareBuffer. */
        void unlockImpl(void);
    public:
        GLHardwareIndexBuffer(IndexType idxType, size_t numIndexes, 
            HardwareBuffer::Usage usage, bool useShadowBuffer); 
        ~GLHardwareIndexBuffer();
        /** See HardwareBuffer. */
        void readData(size_t offset, size_t length, void* pDest);
        /** See HardwareBuffer. */
        void writeData(size_t offset, size_t length, 
            const void* pSource, bool discardWholeBuffer = false);

        GLuint getGLBufferId(void) const { return mBufferId; }
    };

}

#endif // __GLHARDWAREINDEXBUFFER_H__

