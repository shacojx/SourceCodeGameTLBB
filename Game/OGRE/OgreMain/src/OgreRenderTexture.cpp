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
#include "OgreStableHeaders.h"

#include "OgreRenderTexture.h"
#include "OgreException.h"
#include "OgreHardwarePixelBuffer.h"
#include "OgreImage.h"
#include "OgreImageCodec.h"

namespace Ogre
{

    //-----------------------------------------------------------------------------
	RenderTexture::RenderTexture(HardwarePixelBuffer *buffer, size_t zoffset):
		mBuffer(buffer), mZOffset(zoffset)
    {
        mPriority = OGRE_REND_TO_TEX_RT_GROUP;
		mWidth = mBuffer->getWidth();
		mHeight = mBuffer->getHeight();
        mColourDepth = Ogre::PixelUtil::getNumElemBits(mBuffer->getFormat());
    }
    RenderTexture::~RenderTexture()
    {
		mBuffer->_clearSliceRTT(0);
    }

	void RenderTexture::writeContentsToFile( const String & filename )
    {
		// copyToMemory
        ImageCodec::ImageData *imgData = new ImageCodec::ImageData();
        
        imgData->width = mWidth;
        imgData->height = mHeight;
		imgData->depth = 1;
        imgData->format = PF_BYTE_RGBA;
		size_t size = imgData->width * imgData->height * 4;

        // Allocate buffer 
        uchar* pBuffer = new uchar[size];

        // Read pixels
        mBuffer->blitToMemory(
			Box(0,0,mZOffset,mWidth,mHeight,mZOffset+1), 
			PixelBox(mWidth, mHeight, 1, imgData->format, pBuffer)
		);

        // Wrap buffer in a chunk
        MemoryDataStreamPtr stream(new MemoryDataStream(
            pBuffer, size, false));

        // Get codec 
        size_t pos = filename.find_last_of(".");
            String extension;
        if( pos == String::npos )
            OGRE_EXCEPT(
                Exception::ERR_INVALIDPARAMS, 
            "Unable to determine image type for '" + filename + "' - invalid extension.",
                "GLRenderTexture::writeContentsToFile" );

        while( pos != filename.length() - 1 )
            extension += filename[++pos];

        // Get the codec
        Codec * pCodec = Codec::getCodec(extension);

        // Write out
        Codec::CodecDataPtr codecDataPtr(imgData);
        pCodec->codeToFile(stream, filename, codecDataPtr);

		delete [] pBuffer;
    }
	//-----------------------------------------------------------------------------
	MultiRenderTarget::MultiRenderTarget(const String &name)
    {
        mPriority = OGRE_REND_TO_TEX_RT_GROUP;
		mName = name;
		/// Width and height is unknown with no targets attached
		mWidth = mHeight = 0;
    }
	//-----------------------------------------------------------------------------
	void MultiRenderTarget::writeContentsToFile( const String & filename )
	{
		OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
				"Cannot write MultiRenderTargets to disk",
                "MultiRenderTarget::writeContentsToFile");
	}

}
