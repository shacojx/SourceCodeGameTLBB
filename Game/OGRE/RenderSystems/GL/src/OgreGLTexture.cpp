/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://ogre.sourceforge.net/

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

#include "OgreGLTexture.h"
#include "OgreGLSupport.h"
#include "OgreGLPixelFormat.h"
#include "OgreGLHardwarePixelBuffer.h"

#include "OgreTextureManager.h"
#include "OgreImage.h"
#include "OgreLogManager.h"
#include "OgreCamera.h"
#include "OgreException.h"
#include "OgreRoot.h"
#include "OgreCodec.h"
#include "OgreImageCodec.h"
#include "OgreStringConverter.h"
#include "OgreBitwise.h"

#include "OgreGLFBORenderTexture.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#   include <windows.h>
#   include <wingdi.h>
#endif

namespace Ogre {



    GLTexture::GLTexture(ResourceManager* creator, const String& name, 
        ResourceHandle handle, const String& group, bool isManual, 
        ManualResourceLoader* loader, GLSupport& support) 
        : Texture(creator, name, handle, group, isManual, loader),
        mTextureID(0), mGLSupport(support)
    {
    }


    GLTexture::~GLTexture()
    {
        // have to call this here reather than in Resource destructor
        // since calling virtual methods in base destructors causes crash
		if (mIsLoaded)
		{
			unload(); 
		}
		else
		{
			freeInternalResources();
		}
    }

    GLenum GLTexture::getGLTextureTarget(void) const
    {
        switch(mTextureType)
        {
            case TEX_TYPE_1D:
                return GL_TEXTURE_1D;
            case TEX_TYPE_2D:
                return GL_TEXTURE_2D;
            case TEX_TYPE_3D:
                return GL_TEXTURE_3D;
            case TEX_TYPE_CUBE_MAP:
                return GL_TEXTURE_CUBE_MAP;
            default:
                return 0;
        };
    }

	//* Creation / loading methods ********************************************
	void GLTexture::createInternalResourcesImpl(void)
    {
		// Convert to nearest power-of-two size if required
        mWidth = GLPixelUtil::optionalPO2(mWidth);      
        mHeight = GLPixelUtil::optionalPO2(mHeight);
        mDepth = GLPixelUtil::optionalPO2(mDepth);
		

		// Adjust format if required
		mFormat = TextureManager::getSingleton().getNativeFormat(mTextureType, mFormat, mUsage);
		
		// Check requested number of mipmaps
		size_t maxMips = GLPixelUtil::getMaxMipmaps(mWidth, mHeight, mDepth, mFormat);
		mNumMipmaps = mNumRequestedMipmaps;
		if(mNumMipmaps>maxMips)
			mNumMipmaps = maxMips;
		
		// Generate texture name
        glGenTextures( 1, &mTextureID );
		
		// Set texture type
		glBindTexture( getGLTextureTarget(), mTextureID );
        
		// This needs to be set otherwise the texture doesn't get rendered
        glTexParameteri( getGLTextureTarget(), GL_TEXTURE_MAX_LEVEL, mNumMipmaps );
        
        // Set some misc default parameters so NVidia won't complain, these can of course be changed later
        glTexParameteri(getGLTextureTarget(), GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(getGLTextureTarget(), GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(getGLTextureTarget(), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(getGLTextureTarget(), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		// If we can do automip generation and the user desires this, do so
		mMipmapsHardwareGenerated = 
			Root::getSingleton().getRenderSystem()->getCapabilities()->hasCapability(RSC_AUTOMIPMAP);
		if((mUsage & TU_AUTOMIPMAP) &&
		    mNumRequestedMipmaps && mMipmapsHardwareGenerated)
        {
            glTexParameteri( getGLTextureTarget(), GL_GENERATE_MIPMAP, GL_TRUE );
        }
		
		// Allocate internal buffer so that glTexSubImageXD can be used
		// Internal format
		GLenum format = GLPixelUtil::getClosestGLInternalFormat(mFormat);
		size_t width = mWidth;
		size_t height = mHeight;
		size_t depth = mDepth;
#if 0
        /** Luckily, this hack seems not to be needed; empty compressed textures can very well be created the 
            conventional way with glTexImageXD.
        */
		if(PixelUtil::isCompressed(mFormat))
		{
			// Compressed formats
			size_t size = PixelUtil::getMemorySize(mWidth, mHeight, mDepth, mFormat);
			// Provide temporary buffer filled with zeroes as glCompressedTexImageXD does not
			// accept a 0 pointer like normal glTexImageXD
			// Run through this process for every mipmap to pregenerate mipmap piramid
			uint8 *tmpdata = new uint8[size];
			memset(tmpdata, 0, size);
			
			for(size_t mip=0; mip<=mNumMipmaps; mip++)
			{
				size = PixelUtil::getMemorySize(width, height, depth, mFormat);
				switch(mTextureType)
				{
					case TEX_TYPE_1D:
						glCompressedTexImage1DARB(GL_TEXTURE_1D, mip, format, 
							width, 0, 
							size, tmpdata);
						break;
					case TEX_TYPE_2D:
						glCompressedTexImage2DARB(GL_TEXTURE_2D, mip, format,
							width, height, 0, 
							size, tmpdata);
						break;
					case TEX_TYPE_3D:
						glCompressedTexImage3DARB(GL_TEXTURE_3D, mip, format,
							width, height, depth, 0, 
							size, tmpdata);
						break;
					case TEX_TYPE_CUBE_MAP:
						for(int face=0; face<6; face++) {
							glCompressedTexImage2DARB(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, mip, format,
								width, height, 0, 
								size, tmpdata);
						}
						break;
				};
				if(width>1)		width = width/2;
				if(height>1)	height = height/2;
				if(depth>1)		depth = depth/2;
			}
			delete [] tmpdata;
		}
		else
#endif
		{
			// Run through this process to pregenerate mipmap piramid
			for(size_t mip=0; mip<=mNumMipmaps; mip++)
			{
				// Normal formats
				switch(mTextureType)
				{
					case TEX_TYPE_1D:
						glTexImage1D(GL_TEXTURE_1D, mip, format,
							width, 0, 
							GL_RGBA, GL_UNSIGNED_BYTE, 0);
	
						break;
					case TEX_TYPE_2D:
						glTexImage2D(GL_TEXTURE_2D, mip, format,
							width, height, 0, 
							GL_RGBA, GL_UNSIGNED_BYTE, 0);
						break;
					case TEX_TYPE_3D:
						glTexImage3D(GL_TEXTURE_3D, mip, format,
							width, height, depth, 0, 
							GL_RGBA, GL_UNSIGNED_BYTE, 0);
						break;
					case TEX_TYPE_CUBE_MAP:
						for(int face=0; face<6; face++) {
							glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, mip, format,
								width, height, 0, 
								GL_RGBA, GL_UNSIGNED_BYTE, 0);
						}
						break;
				};
				if(width>1)		width = width/2;
				if(height>1)	height = height/2;
				if(depth>1)		depth = depth/2;
			}
		}
		_createSurfaceList();
		// Get final internal format
		mFormat = getBuffer(0,0)->getFormat();
	}
	
    void GLTexture::createRenderTexture(void)
    {
        // Create the GL texture
		// This already does everything neccessary
        createInternalResources();
    }
	
	void GLTexture::loadImage( const Image& img )
    {
        std::vector<const Image*> images;
		
        images.push_back(&img);
        _loadImages(images);
        images.clear();
    }

    void GLTexture::loadImpl()
    {
        if( mUsage & TU_RENDERTARGET )
        {
            createRenderTexture();
            mIsLoaded = true;     
        }
        else
        {
			String baseName, ext;
			size_t pos = mName.find_last_of(".");
			if( pos == String::npos )
				OGRE_EXCEPT(
					Exception::ERR_INVALIDPARAMS, 
					"Unable to load image file '"+ mName + "' - invalid extension.",
					"GLTexture::loadImpl" );

			baseName = mName.substr(0, pos);
			ext = mName.substr(pos+1);
    
			if(mTextureType == TEX_TYPE_1D || mTextureType == TEX_TYPE_2D || 
                mTextureType == TEX_TYPE_3D)
            {
                Image img;
	            // find & load resource data intro stream to allow resource
				// group changes if required
				DataStreamPtr dstream = 
					ResourceGroupManager::getSingleton().openResource(
						mName, mGroup, true, this);

                img.load(dstream, ext);

				// If this is a cube map, set the texture type flag accordingly.
                if (img.hasFlag(IF_CUBEMAP))
					mTextureType = TEX_TYPE_CUBE_MAP;
				// If this is a volumetric texture set the texture type flag accordingly.
				if(img.getDepth() > 1)
					mTextureType = TEX_TYPE_3D;

				loadImage( img );
            }
            else if (mTextureType == TEX_TYPE_CUBE_MAP)
            {
				if(StringUtil::endsWith(getName(), ".dds"))
				{
					// XX HACK there should be a better way to specify whether 
					// all faces are in the same file or not
					Image img;
	            	// find & load resource data intro stream to allow resource
					// group changes if required
					DataStreamPtr dstream = 
						ResourceGroupManager::getSingleton().openResource(
							mName, mGroup, true, this);

	                img.load(dstream, ext);
					loadImage( img );
				}
				else
				{
					std::vector<Image> images(6);
					std::vector<const Image*> imagePtrs;
					static const String suffixes[6] = {"_rt", "_lf", "_up", "_dn", "_fr", "_bk"};
	
					for(size_t i = 0; i < 6; i++)
					{
						String fullName = baseName + suffixes[i] + "." + ext;
		            	// find & load resource data intro stream to allow resource
						// group changes if required
						DataStreamPtr dstream = 
							ResourceGroupManager::getSingleton().openResource(
								fullName, mGroup, true, this);
	
						images[i].load(dstream, ext);
						imagePtrs.push_back(&images[i]);
					}
	
					_loadImages( imagePtrs );
				}
            }
            else
                OGRE_EXCEPT( Exception::UNIMPLEMENTED_FEATURE, "**** Unknown texture type ****", "GLTexture::load" );
        }
    }
	
	//*************************************************************************
    
    void GLTexture::freeInternalResourcesImpl()
    {
		mSurfaceList.clear();
        glDeleteTextures( 1, &mTextureID );
    }

	
	//---------------------------------------------------------------------------------------------
	void GLTexture::_createSurfaceList()
	{
		mSurfaceList.clear();
		
		// For all faces and mipmaps, store surfaces as HardwarePixelBufferSharedPtr
		bool wantGeneratedMips = (mUsage & TU_AUTOMIPMAP)!=0;
		
		// Do mipmapping in software? (uses GLU) For some cards, this is still needed. Of course,
		// only when mipmap generation is desired.
		bool doSoftware = wantGeneratedMips && !mMipmapsHardwareGenerated && getNumMipmaps(); 
		
		for(size_t face=0; face<getNumFaces(); face++)
		{
			for(size_t mip=0; mip<=getNumMipmaps(); mip++)
			{
                GLHardwarePixelBuffer *buf = new GLTextureBuffer(mName, getGLTextureTarget(), mTextureID, face, mip,
						static_cast<HardwareBuffer::Usage>(mUsage), doSoftware && mip==0);
				mSurfaceList.push_back(HardwarePixelBufferSharedPtr(buf));
                
                /// Check for error
                if(buf->getWidth()==0 || buf->getHeight()==0 || buf->getDepth()==0)
                {
                    OGRE_EXCEPT(
                        Exception::ERR_RENDERINGAPI_ERROR, 
                        "Zero sized texture surface on texture "+getName()+
                            " face "+StringConverter::toString(face)+
                            " mipmap "+StringConverter::toString(mip)+
                            ". Probably, the GL driver refused to create the texture.", 
                            "GLTexture::_createSurfaceList");
                }
			}
		}
	}
	
	//---------------------------------------------------------------------------------------------
	HardwarePixelBufferSharedPtr GLTexture::getBuffer(size_t face, size_t mipmap)
	{
		if(face >= getNumFaces())
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Face index out of range",
					"GLTexture::getBuffer");
		if(mipmap > mNumMipmaps)
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Mipmap index out of range",
					"GLTexture::getBuffer");
		unsigned int idx = face*(mNumMipmaps+1) + mipmap;
		assert(idx < mSurfaceList.size());
		return mSurfaceList[idx];
	}
	
}

