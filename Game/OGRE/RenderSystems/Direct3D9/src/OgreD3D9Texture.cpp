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
#include "OgreD3D9Texture.h"
#include "OgreD3D9HardwarePixelBuffer.h"
#include "OgreException.h"
#include "OgreLogManager.h"
#include "OgreStringConverter.h"
#include "OgreBitwise.h"
#include "OgreD3D9Mappings.h"
#include "OgreD3D9RenderSystem.h"
#include "OgreD3D9Driver.h"
#include "OgreRoot.h"

#include "OgreNoMemoryMacros.h"
#include <d3dx9.h>
#include <dxerr9.h>
#include "OgreMemoryMacros.h"

namespace Ogre 
{
    static FORCEINLINE D3D9RenderSystem* _getD3D9RenderSystem(void)
    {
        return static_cast<D3D9RenderSystem*>(
            Root::getSingleton().getRenderSystem());
    }
	/****************************************************************************************/
    D3D9Texture::D3D9Texture(ResourceManager* creator, const String& name, 
        ResourceHandle handle, const String& group, bool isManual, 
        ManualResourceLoader* loader, IDirect3DDevice9 *pD3DDevice)
        :Texture(creator, name, handle, group, isManual, loader),
        mpDev(pD3DDevice), 
        mpD3D(NULL), 
        mpNormTex(NULL),
        mpCubeTex(NULL),
		mpVolumeTex(NULL),
        mpTex(NULL),
        mBBPixelFormat(D3DFMT_UNKNOWN),
        mD3DPool(D3DPOOL_MANAGED),
		mDynamicTextures(false)
	{
        _initDevice();
	}
	/****************************************************************************************/
	D3D9Texture::~D3D9Texture()
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
	/****************************************************************************************/
	void D3D9Texture::copyToTexture(TexturePtr& target)
	{
        // check if this & target are the same format and type
		// blitting from or to cube textures is not supported yet
		if (target->getUsage() != this->getUsage() ||
			target->getTextureType() != this->getTextureType())
		{
			OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, 
					"Src. and dest. textures must be of same type and must have the same usage !!!", 
					"D3D9Texture::copyToTexture" );
		}

        HRESULT hr;
        D3D9Texture *other;
		// get the target
		other = reinterpret_cast< D3D9Texture * >( target.get() );
		// target rectangle (whole surface)
		RECT dstRC = {0, 0, other->getWidth(), other->getHeight()};

		// do it plain for normal texture
		if (this->getTextureType() == TEX_TYPE_2D)
		{
			// get our source surface
			IDirect3DSurface9 *pSrcSurface = 0;
			if( FAILED( hr = mpNormTex->GetSurfaceLevel(0, &pSrcSurface) ) )
			{
				String msg = DXGetErrorDescription9(hr);
				OGRE_EXCEPT( hr, "Couldn't blit : " + msg, "D3D9Texture::copyToTexture" );
			}

			// get our target surface
			IDirect3DSurface9 *pDstSurface = 0;
			IDirect3DTexture9 *pOthTex = other->getNormTexture();
			if( FAILED( hr = pOthTex->GetSurfaceLevel(0, &pDstSurface) ) )
			{
				String msg = DXGetErrorDescription9(hr);
				SAFE_RELEASE(pSrcSurface);
				OGRE_EXCEPT( hr, "Couldn't blit : " + msg, "D3D9Texture::copyToTexture" );
			}

			// do the blit, it's called StretchRect in D3D9 :)
			if( FAILED( hr = mpDev->StretchRect( pSrcSurface, NULL, pDstSurface, &dstRC, D3DTEXF_NONE) ) )
			{
				String msg = DXGetErrorDescription9(hr);
				SAFE_RELEASE(pSrcSurface);
				SAFE_RELEASE(pDstSurface);
				OGRE_EXCEPT( hr, "Couldn't blit : " + msg, "D3D9Texture::copyToTexture" );
			}

			// release temp. surfaces
			SAFE_RELEASE(pSrcSurface);
			SAFE_RELEASE(pDstSurface);
		}
		else if (this->getTextureType() == TEX_TYPE_CUBE_MAP)
		{
			// get the target cube texture
			IDirect3DCubeTexture9 *pOthTex = other->getCubeTexture();
			// blit to 6 cube faces
			for (size_t face = 0; face < 6; face++)
			{
				// get our source surface
				IDirect3DSurface9 *pSrcSurface = 0;
				if( FAILED( hr = mpCubeTex->GetCubeMapSurface((D3DCUBEMAP_FACES)face, 0, &pSrcSurface) ) )
				{
					String msg = DXGetErrorDescription9(hr);
					OGRE_EXCEPT( hr, "Couldn't blit : " + msg, "D3D9Texture::copyToTexture" );
				}

				// get our target surface
				IDirect3DSurface9 *pDstSurface = 0;
				if( FAILED( hr = pOthTex->GetCubeMapSurface((D3DCUBEMAP_FACES)face, 0, &pDstSurface) ) )
				{
					String msg = DXGetErrorDescription9(hr);
					SAFE_RELEASE(pSrcSurface);
					OGRE_EXCEPT( hr, "Couldn't blit : " + msg, "D3D9Texture::copyToTexture" );
				}

				// do the blit, it's called StretchRect in D3D9 :)
				if( FAILED( hr = mpDev->StretchRect( pSrcSurface, NULL, pDstSurface, &dstRC, D3DTEXF_NONE) ) )
				{
					String msg = DXGetErrorDescription9(hr);
					SAFE_RELEASE(pSrcSurface);
					SAFE_RELEASE(pDstSurface);
					OGRE_EXCEPT( hr, "Couldn't blit : " + msg, "D3D9Texture::copyToTexture" );
				}

				// release temp. surfaces
				SAFE_RELEASE(pSrcSurface);
				SAFE_RELEASE(pDstSurface);
			}
		}
		else
		{
			OGRE_EXCEPT( Exception::UNIMPLEMENTED_FEATURE, 
					"Copy to texture is implemented only for 2D and cube textures !!!", 
					"D3D9Texture::copyToTexture" );
		}
	}
	/****************************************************************************************/
	void D3D9Texture::loadImage( const Image &img )
	{
		// Use OGRE its own codecs
		std::vector<const Image*> imagePtrs;
		imagePtrs.push_back(&img);
		_loadImages( imagePtrs );
	}
	/****************************************************************************************/
	void D3D9Texture::loadImpl()
	{
		if (mUsage & TU_RENDERTARGET)
		{
			createInternalResources();
			mIsLoaded = true;
			return;
		}

        if (!mInternalResourcesCreated)
        {
            // NB: Need to initialise pool to some value other than D3DPOOL_DEFAULT,
            // otherwise, if the texture loading failed, it might re-create as empty
            // texture when device lost/restore. The actual pool will determine later.
            mD3DPool = D3DPOOL_MANAGED;
        }

		// load based on tex.type
		switch (this->getTextureType())
		{
		case TEX_TYPE_1D:
		case TEX_TYPE_2D:
			this->_loadNormTex();
			break;
		case TEX_TYPE_3D:
            this->_loadVolumeTex();
            break;
		case TEX_TYPE_CUBE_MAP:
			this->_loadCubeTex();
			break;
		default:
			OGRE_EXCEPT( Exception::ERR_INTERNAL_ERROR, "Unknown texture type", "D3D9Texture::loadImpl" );
		}

	}
	/****************************************************************************************/
	void D3D9Texture::freeInternalResourcesImpl()
	{
		SAFE_RELEASE(mpTex);
		SAFE_RELEASE(mpNormTex);
		SAFE_RELEASE(mpCubeTex);
		SAFE_RELEASE(mpVolumeTex);
	}
	/****************************************************************************************/
	void D3D9Texture::_loadCubeTex()
	{
		assert(this->getTextureType() == TEX_TYPE_CUBE_MAP);

        // DDS load?
		if (StringUtil::endsWith(getName(), ".dds"))
        {
            // find & load resource data
			DataStreamPtr dstream = 
				ResourceGroupManager::getSingleton().openResource(
					mName, mGroup, true, this);
            MemoryDataStream stream( dstream );

			DWORD usage = 0;
			UINT numMips = mNumRequestedMipmaps + 1;
			// check if mip map volume textures are supported
			if (!(_getD3D9RenderSystem()->_getDeviceCaps().TextureCaps & D3DPTEXTURECAPS_MIPCUBEMAP))
			{
				// no mip map support for this kind of textures :(
				mNumMipmaps = 0;
				numMips = 1;
			}

            // Determine D3D pool to use
            D3DPOOL pool;
            if (mUsage & TU_DYNAMIC)
            {
                pool = D3DPOOL_DEFAULT;
            }
            else
            {
                pool = D3DPOOL_MANAGED;
            }

			HRESULT hr = D3DXCreateCubeTextureFromFileInMemoryEx(
				mpDev,
				stream.getPtr(),
				stream.size(),
				D3DX_DEFAULT, // dims (square)
				numMips,
				usage,
				D3DFMT_UNKNOWN,
				pool,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				0,  // colour key
				NULL, // src box
				NULL, // palette
				&mpCubeTex); 

            if (FAILED(hr))
		    {
				this->freeInternalResources();
			    OGRE_EXCEPT( hr, "Can't create cube texture", "D3D9Texture::_loadCubeTex" );
		    }

            hr = mpCubeTex->QueryInterface(IID_IDirect3DBaseTexture9, (void **)&mpTex);

            if (FAILED(hr))
		    {
				this->freeInternalResources();
			    OGRE_EXCEPT( hr, "Can't get base texture", "D3D9Texture::_loadCubeTex" );
		    }

            D3DSURFACE_DESC texDesc;
            mpCubeTex->GetLevelDesc(0, &texDesc);
            mD3DPool = texDesc.Pool;
            // set src and dest attributes to the same, we can't know
            _setSrcAttributes(texDesc.Width, texDesc.Height, 1, D3D9Mappings::_getPF(texDesc.Format));
            _setFinalAttributes(texDesc.Width, texDesc.Height, 1,  D3D9Mappings::_getPF(texDesc.Format));
			mIsLoaded = true;
			mInternalResourcesCreated = true;
        }
        else
        {
			// Load from 6 separate files
			// Use OGRE its own codecs
			String baseName, ext;
			size_t pos = mName.find_last_of(".");
			baseName = mName.substr(0, pos);
			ext = mName.substr(pos+1);
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
	/****************************************************************************************/
	void D3D9Texture::_loadVolumeTex()
	{
		assert(this->getTextureType() == TEX_TYPE_3D);
		// DDS load?
		if (StringUtil::endsWith(getName(), ".dds"))
		{
			// find & load resource data
			DataStreamPtr dstream = 
				ResourceGroupManager::getSingleton().openResource(
					mName, mGroup, true, this);
			MemoryDataStream stream(dstream);
	
			DWORD usage = 0;
			UINT numMips = mNumRequestedMipmaps + 1;
			// check if mip map volume textures are supported
			if (!(_getD3D9RenderSystem()->_getDeviceCaps().TextureCaps & D3DPTEXTURECAPS_MIPVOLUMEMAP))
			{
				// no mip map support for this kind of textures :(
				mNumMipmaps = 0;
				numMips = 1;
			}

            // Determine D3D pool to use
            D3DPOOL pool;
            if (mUsage & TU_DYNAMIC)
            {
                pool = D3DPOOL_DEFAULT;
            }
            else
            {
                pool = D3DPOOL_MANAGED;
            }

			HRESULT hr = D3DXCreateVolumeTextureFromFileInMemoryEx(
				mpDev,
				stream.getPtr(),
				stream.size(),
				D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, // dims
				numMips,
				usage,
				D3DFMT_UNKNOWN,
				pool,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				0,  // colour key
				NULL, // src box
				NULL, // palette
				&mpVolumeTex); 
	
			if (FAILED(hr))
			{
				OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR, 
					"Unable to load volume texture from " + this->getName(),
					"D3D9Texture::_loadVolumeTex");
			}
	
			hr = mpVolumeTex->QueryInterface(IID_IDirect3DBaseTexture9, (void **)&mpTex);
	
			if (FAILED(hr))
			{
				this->freeInternalResources();
				OGRE_EXCEPT( hr, "Can't get base texture", "D3D9Texture::_loadVolumeTex" );
			}
	
			D3DVOLUME_DESC texDesc;
			hr = mpVolumeTex->GetLevelDesc(0, &texDesc);
            mD3DPool = texDesc.Pool;
			// set src and dest attributes to the same, we can't know
			_setSrcAttributes(texDesc.Width, texDesc.Height, texDesc.Depth, D3D9Mappings::_getPF(texDesc.Format));
			_setFinalAttributes(texDesc.Width, texDesc.Height, texDesc.Depth, D3D9Mappings::_getPF(texDesc.Format));
			mIsLoaded = true;
			mInternalResourcesCreated = true;
        }
		else
		{
			Image img;
           	// find & load resource data intro stream to allow resource
			// group changes if required
			DataStreamPtr dstream = 
				ResourceGroupManager::getSingleton().openResource(
					mName, mGroup, true, this);
			size_t pos = mName.find_last_of(".");
			String ext = mName.substr(pos+1);
	
			img.load(dstream, ext);
			loadImage(img);
		}
    }
	/****************************************************************************************/
	void D3D9Texture::_loadNormTex()
	{
		assert(this->getTextureType() == TEX_TYPE_1D || this->getTextureType() == TEX_TYPE_2D);
		// DDS load?
		if (StringUtil::endsWith(getName(), ".dds"))
		{
			// Use D3DX
			// find & load resource data
			DataStreamPtr dstream = 
				ResourceGroupManager::getSingleton().openResource(
					mName, mGroup, true, this);
			MemoryDataStream stream(dstream);
	
			DWORD usage = 0;
			UINT numMips = mNumRequestedMipmaps + 1;
			// check if mip map volume textures are supported
			if (!(_getD3D9RenderSystem()->_getDeviceCaps().TextureCaps & D3DPTEXTURECAPS_MIPMAP))
			{
				// no mip map support for this kind of textures :(
				mNumMipmaps = 0;
				numMips = 1;
			}

            // Determine D3D pool to use
            D3DPOOL pool;
            if (mUsage & TU_DYNAMIC)
            {
                pool = D3DPOOL_DEFAULT;
            }
            else
            {
                pool = D3DPOOL_MANAGED;
            }

			HRESULT hr = D3DXCreateTextureFromFileInMemoryEx(
				mpDev,
				stream.getPtr(),
				stream.size(),
				D3DX_DEFAULT, D3DX_DEFAULT, // dims
				numMips,
				usage,
				D3DFMT_UNKNOWN,
				pool,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				0,  // colour key
				NULL, // src box
				NULL, // palette
				&mpNormTex); 
	
			if (FAILED(hr))
			{
				OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR, 
					"Unable to load texture from " + this->getName(),
					"D3D9Texture::_loadNormTex");
			}
	
			hr = mpNormTex->QueryInterface(IID_IDirect3DBaseTexture9, (void **)&mpTex);
	
			if (FAILED(hr))
			{
				this->freeInternalResources();
				OGRE_EXCEPT( hr, "Can't get base texture", "D3D9Texture::_loadNormTex" );
			}
	
			D3DSURFACE_DESC texDesc;
			mpNormTex->GetLevelDesc(0, &texDesc);
            mD3DPool = texDesc.Pool;
			// set src and dest attributes to the same, we can't know
			_setSrcAttributes(texDesc.Width, texDesc.Height, 1, D3D9Mappings::_getPF(texDesc.Format));
			_setFinalAttributes(texDesc.Width, texDesc.Height, 1, D3D9Mappings::_getPF(texDesc.Format));
			mIsLoaded = true;
			mInternalResourcesCreated = true;
        }
		else
		{
			Image img;
           	// find & load resource data intro stream to allow resource
			// group changes if required
			DataStreamPtr dstream = 
				ResourceGroupManager::getSingleton().openResource(
					mName, mGroup, true, this);
	
			size_t pos = mName.find_last_of(".");
			String ext = mName.substr(pos+1);
			
			img.load(dstream, ext);
			loadImage(img);
		}
	}
	/****************************************************************************************/
    void D3D9Texture::createInternalResourcesImpl(void)
	{
		// If mSrcWidth and mSrcHeight are zero, the requested extents have probably been set
		// through setWidth and setHeight, which set mWidth and mHeight. Take those values.
		if(mSrcWidth == 0 || mSrcHeight == 0) {
			mSrcWidth = mWidth;
			mSrcHeight = mHeight;
		}
		
		// Determine D3D pool to use
		// Use managed unless we're a render target or user has asked for 
		// a dynamic texture
		if ((mUsage & TU_RENDERTARGET) ||
			(mUsage & TU_DYNAMIC))
		{
			mD3DPool = D3DPOOL_DEFAULT;
		}
		else
		{
			mD3DPool = D3DPOOL_MANAGED;
		}
		// load based on tex.type
		switch (this->getTextureType())
		{
		case TEX_TYPE_1D:
		case TEX_TYPE_2D:
			this->_createNormTex();
			break;
		case TEX_TYPE_CUBE_MAP:
			this->_createCubeTex();
			break;
		case TEX_TYPE_3D:
			this->_createVolumeTex();
			break;
		default:
			this->freeInternalResources();
			OGRE_EXCEPT( Exception::ERR_INTERNAL_ERROR, "Unknown texture type", "D3D9Texture::createInternalResources" );
		}
	}
	/****************************************************************************************/
	void D3D9Texture::_createNormTex()
	{
		// we must have those defined here
		assert(mSrcWidth > 0 || mSrcHeight > 0);

		// determine wich D3D9 pixel format we'll use
		HRESULT hr;
		D3DFORMAT d3dPF = this->_chooseD3DFormat();
		// let's D3DX check the corrected pixel format
		hr = D3DXCheckTextureRequirements(mpDev, NULL, NULL, NULL, 0, &d3dPF, mD3DPool);

		// Use D3DX to help us create the texture, this way it can adjust any relevant sizes
		DWORD usage = (mUsage & TU_RENDERTARGET) ? D3DUSAGE_RENDERTARGET : 0;
		UINT numMips = mNumRequestedMipmaps + 1;
		// Check dynamic textures
		if (mUsage & TU_DYNAMIC)
		{
			if (_canUseDynamicTextures(usage, D3DRTYPE_TEXTURE, d3dPF))
			{
				usage |= D3DUSAGE_DYNAMIC;
				mDynamicTextures = true;
			}
			else
			{
				mDynamicTextures = false;
			}
		}
		// check if mip maps are supported on hardware
		mMipmapsHardwareGenerated = false;
		if (_getD3D9RenderSystem()->_getDeviceCaps().TextureCaps & D3DPTEXTURECAPS_MIPMAP)
		{
			if (mUsage & TU_AUTOMIPMAP && mNumRequestedMipmaps != 0)
			{
				// use auto.gen. if available, and if desired
				mMipmapsHardwareGenerated = this->_canAutoGenMipmaps(usage, D3DRTYPE_TEXTURE, d3dPF);
				if (mMipmapsHardwareGenerated)
				{
					usage |= D3DUSAGE_AUTOGENMIPMAP;
					numMips = 0;
				}
			}
		}
		else
		{
			// no mip map support for this kind of textures :(
			mNumMipmaps = 0;
			numMips = 1;
		}

		// create the texture
		hr = D3DXCreateTexture(	
				mpDev,								// device
				mSrcWidth,							// width
				mSrcHeight,							// height
				numMips,							// number of mip map levels
				usage,								// usage
				d3dPF,								// pixel format
				mD3DPool,
				&mpNormTex);						// data pointer
		// check result and except if failed
		if (FAILED(hr))
		{
			this->freeInternalResources();
			OGRE_EXCEPT( hr, "Error creating texture", "D3D9Texture::_createNormTex" );
		}
		
		// set the base texture we'll use in the render system
		hr = mpNormTex->QueryInterface(IID_IDirect3DBaseTexture9, (void **)&mpTex);
		if (FAILED(hr))
		{
			this->freeInternalResources();
			OGRE_EXCEPT( hr, "Can't get base texture", "D3D9Texture::_createNormTex" );
		}
		
		// set final tex. attributes from tex. description
		// they may differ from the source image !!!
		D3DSURFACE_DESC desc;
		hr = mpNormTex->GetLevelDesc(0, &desc);
		if (FAILED(hr))
		{
			this->freeInternalResources();
			OGRE_EXCEPT( hr, "Can't get texture description", "D3D9Texture::_createNormTex" );
		}
		this->_setFinalAttributes(desc.Width, desc.Height, 1, D3D9Mappings::_getPF(desc.Format));
		
		// Set best filter type
		if(mMipmapsHardwareGenerated)
		{
			hr = mpTex->SetAutoGenFilterType(_getBestFilterMethod());
			if(FAILED(hr))
			{
				OGRE_EXCEPT( hr, "Could not set best autogen filter type", "D3D9Texture::_createNormTex" );
			}
		}
	}
	/****************************************************************************************/
	void D3D9Texture::_createCubeTex()
	{
		// we must have those defined here
		assert(mSrcWidth > 0 || mSrcHeight > 0);

		// determine wich D3D9 pixel format we'll use
		HRESULT hr;
		D3DFORMAT d3dPF = this->_chooseD3DFormat();
		// let's D3DX check the corrected pixel format
		hr = D3DXCheckCubeTextureRequirements(mpDev, NULL, NULL, 0, &d3dPF, mD3DPool);

		// Use D3DX to help us create the texture, this way it can adjust any relevant sizes
		DWORD usage = (mUsage & TU_RENDERTARGET) ? D3DUSAGE_RENDERTARGET : 0;
		UINT numMips = mNumRequestedMipmaps + 1;
		// Check dynamic textures
		if (mUsage & TU_DYNAMIC)
		{
			if (_canUseDynamicTextures(usage, D3DRTYPE_CUBETEXTURE, d3dPF))
			{
				usage |= D3DUSAGE_DYNAMIC;
				mDynamicTextures = true;
			}
			else
			{
				mDynamicTextures = false;
			}
		}
		// check if mip map cube textures are supported
		mMipmapsHardwareGenerated = false;
		if (_getD3D9RenderSystem()->_getDeviceCaps().TextureCaps & D3DPTEXTURECAPS_MIPCUBEMAP)
		{
			if (mUsage & TU_AUTOMIPMAP && mNumRequestedMipmaps != 0)
			{
				// use auto.gen. if available
				mMipmapsHardwareGenerated = this->_canAutoGenMipmaps(usage, D3DRTYPE_CUBETEXTURE, d3dPF);
				if (mMipmapsHardwareGenerated)
				{
					usage |= D3DUSAGE_AUTOGENMIPMAP;
					numMips = 0;
				}
			}
		}
		else
		{
			// no mip map support for this kind of textures :(
			mNumMipmaps = 0;
			numMips = 1;
		}

		// create the texture
		hr = D3DXCreateCubeTexture(	
				mpDev,								// device
				mSrcWidth,							// dimension
				numMips,							// number of mip map levels
				usage,								// usage
				d3dPF,								// pixel format
				mD3DPool,
				&mpCubeTex);						// data pointer
		// check result and except if failed
		if (FAILED(hr))
		{
			this->freeInternalResources();
			OGRE_EXCEPT( hr, "Error creating texture", "D3D9Texture::_createCubeTex" );
		}

		// set the base texture we'll use in the render system
		hr = mpCubeTex->QueryInterface(IID_IDirect3DBaseTexture9, (void **)&mpTex);
		if (FAILED(hr))
		{
			this->freeInternalResources();
			OGRE_EXCEPT( hr, "Can't get base texture", "D3D9Texture::_createCubeTex" );
		}
		
		// set final tex. attributes from tex. description
		// they may differ from the source image !!!
		D3DSURFACE_DESC desc;
		hr = mpCubeTex->GetLevelDesc(0, &desc);
		if (FAILED(hr))
		{
			this->freeInternalResources();
			OGRE_EXCEPT( hr, "Can't get texture description", "D3D9Texture::_createCubeTex" );
		}
		this->_setFinalAttributes(desc.Width, desc.Height, 1, D3D9Mappings::_getPF(desc.Format));

		// Set best filter type
		if(mMipmapsHardwareGenerated)
		{
			hr = mpTex->SetAutoGenFilterType(_getBestFilterMethod());
			if(FAILED(hr))
			{
				OGRE_EXCEPT( hr, "Could not set best autogen filter type", "D3D9Texture::_createCubeTex" );
			}
		}
	}
	/****************************************************************************************/
	void D3D9Texture::_createVolumeTex()
	{
		// we must have those defined here
		assert(mWidth > 0 && mHeight > 0 && mDepth>0);

		// determine wich D3D9 pixel format we'll use
		HRESULT hr;
		D3DFORMAT d3dPF = this->_chooseD3DFormat();
		// let's D3DX check the corrected pixel format
		hr = D3DXCheckVolumeTextureRequirements(mpDev, NULL, NULL, NULL, NULL, 0, &d3dPF, mD3DPool);

		// Use D3DX to help us create the texture, this way it can adjust any relevant sizes
		DWORD usage = (mUsage & TU_RENDERTARGET) ? D3DUSAGE_RENDERTARGET : 0;
		UINT numMips = mNumRequestedMipmaps + 1;
		// Check dynamic textures
		if (mUsage & TU_DYNAMIC)
		{
			if (_canUseDynamicTextures(usage, D3DRTYPE_VOLUMETEXTURE, d3dPF))
			{
				usage |= D3DUSAGE_DYNAMIC;
				mDynamicTextures = true;
			}
			else
			{
				mDynamicTextures = false;
			}
		}
		// check if mip map volume textures are supported
		mMipmapsHardwareGenerated = false;
		if (_getD3D9RenderSystem()->_getDeviceCaps().TextureCaps & D3DPTEXTURECAPS_MIPVOLUMEMAP)
		{
			if (mUsage & TU_AUTOMIPMAP && mNumRequestedMipmaps != 0)
			{
				// use auto.gen. if available
				mMipmapsHardwareGenerated = this->_canAutoGenMipmaps(usage, D3DRTYPE_VOLUMETEXTURE, d3dPF);
				if (mMipmapsHardwareGenerated)
				{
					usage |= D3DUSAGE_AUTOGENMIPMAP;
					numMips = 0;
				}
			}
		}
		else
		{
			// no mip map support for this kind of textures :(
			mNumMipmaps = 0;
			numMips = 1;
		}

		// create the texture
		hr = D3DXCreateVolumeTexture(	
				mpDev,								// device
				mWidth,								// dimension
				mHeight,
				mDepth,
				numMips,							// number of mip map levels
				usage,								// usage
				d3dPF,								// pixel format
				mD3DPool,
				&mpVolumeTex);						// data pointer
		// check result and except if failed
		if (FAILED(hr))
		{
			this->freeInternalResources();
			OGRE_EXCEPT( hr, "Error creating texture", "D3D9Texture::_createVolumeTex" );
		}

		// set the base texture we'll use in the render system
		hr = mpVolumeTex->QueryInterface(IID_IDirect3DBaseTexture9, (void **)&mpTex);
		if (FAILED(hr))
		{
			this->freeInternalResources();
			OGRE_EXCEPT( hr, "Can't get base texture", "D3D9Texture::_createVolumeTex" );
		}
		
		// set final tex. attributes from tex. description
		// they may differ from the source image !!!
		D3DVOLUME_DESC desc;
		hr = mpVolumeTex->GetLevelDesc(0, &desc);
		if (FAILED(hr))
		{
			this->freeInternalResources();
			OGRE_EXCEPT( hr, "Can't get texture description", "D3D9Texture::_createVolumeTex" );
		}
		this->_setFinalAttributes(desc.Width, desc.Height, desc.Depth, D3D9Mappings::_getPF(desc.Format));
		
		// Set best filter type
		if(mMipmapsHardwareGenerated)
		{
			hr = mpTex->SetAutoGenFilterType(_getBestFilterMethod());
			if(FAILED(hr))
			{
				OGRE_EXCEPT( hr, "Could not set best autogen filter type", "D3D9Texture::_createCubeTex" );
			}
		}
	}
	/****************************************************************************************/
	void D3D9Texture::_initDevice(void)
	{ 
		assert(mpDev);
		HRESULT hr;

		// get D3D pointer
		hr = mpDev->GetDirect3D(&mpD3D);
		if (FAILED(hr))
			OGRE_EXCEPT( hr, "Failed to get D3D9 pointer", "D3D9Texture::_initDevice" );
		// decrement reference count
		mpD3D->Release();

		// get our back buffer pixel format
		IDirect3DSurface9 *pSrf;
		hr = mpDev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pSrf);
		if (FAILED(hr))
			OGRE_EXCEPT( hr, "Failed to get D3D9 device pixel format", "D3D9Texture::_initDevice" );
		// decrement reference count
		pSrf->Release();

		D3DSURFACE_DESC srfDesc;
		hr = pSrf->GetDesc(&srfDesc);
		if (FAILED(hr))
		{
			OGRE_EXCEPT( hr, "Failed to get D3D9 device pixel format", "D3D9Texture::_initDevice" );
		}

		mBBPixelFormat = srfDesc.Format;
	}
	/****************************************************************************************/
	void D3D9Texture::_setFinalAttributes(unsigned long width, unsigned long height, 
        unsigned long depth, PixelFormat format)
	{ 
		// set target texture attributes
		mHeight = height; 
		mWidth = width; 
        mDepth = depth;
		mFormat = format; 

		// Update size (the final size, including temp space because in consumed memory)
		// this is needed in Resource class
		mSize = calculateSize();

		// say to the world what we are doing
		if (mWidth != mSrcWidth ||
			mHeight != mSrcHeight)
		{
			LogManager::getSingleton().logMessage("D3D9 : ***** Dimensions altered by the render system");
			LogManager::getSingleton().logMessage("D3D9 : ***** Source image dimensions : " + StringConverter::toString(mSrcWidth) + "x" + StringConverter::toString(mSrcHeight));
			LogManager::getSingleton().logMessage("D3D9 : ***** Texture dimensions : " + StringConverter::toString(mWidth) + "x" + StringConverter::toString(mHeight));
		}
		
		// Create list of subsurfaces for getBuffer()
		_createSurfaceList();
	}
	/****************************************************************************************/
	void D3D9Texture::_setSrcAttributes(unsigned long width, unsigned long height, 
        unsigned long depth, PixelFormat format)
	{ 
		// set source image attributes
		mSrcWidth = width; 
		mSrcHeight = height; 
		mSrcDepth = depth;
        mSrcFormat = format;
		// say to the world what we are doing
		switch (this->getTextureType())
		{
		case TEX_TYPE_1D:
			if (mUsage & TU_RENDERTARGET)
				LogManager::getSingleton().logMessage("D3D9 : Creating 1D RenderTarget, name : '" + this->getName() + "' with " + StringConverter::toString(mNumMipmaps) + " mip map levels");
			else
				LogManager::getSingleton().logMessage("D3D9 : Loading 1D Texture, image name : '" + this->getName() + "' with " + StringConverter::toString(mNumMipmaps) + " mip map levels");
			break;
		case TEX_TYPE_2D:
			if (mUsage & TU_RENDERTARGET)
				LogManager::getSingleton().logMessage("D3D9 : Creating 2D RenderTarget, name : '" + this->getName() + "' with " + StringConverter::toString(mNumMipmaps) + " mip map levels");
			else
				LogManager::getSingleton().logMessage("D3D9 : Loading 2D Texture, image name : '" + this->getName() + "' with " + StringConverter::toString(mNumMipmaps) + " mip map levels");
			break;
		case TEX_TYPE_3D:
			if (mUsage & TU_RENDERTARGET)
				LogManager::getSingleton().logMessage("D3D9 : Creating 3D RenderTarget, name : '" + this->getName() + "' with " + StringConverter::toString(mNumMipmaps) + " mip map levels");
			else
				LogManager::getSingleton().logMessage("D3D9 : Loading 3D Texture, image name : '" + this->getName() + "' with " + StringConverter::toString(mNumMipmaps) + " mip map levels");
			break;
		case TEX_TYPE_CUBE_MAP:
			if (mUsage & TU_RENDERTARGET)
				LogManager::getSingleton().logMessage("D3D9 : Creating Cube map RenderTarget, name : '" + this->getName() + "' with " + StringConverter::toString(mNumMipmaps) + " mip map levels");
			else
				LogManager::getSingleton().logMessage("D3D9 : Loading Cube Texture, base image name : '" + this->getName() + "' with " + StringConverter::toString(mNumMipmaps) + " mip map levels");
			break;
		default:
			this->freeInternalResources();
			OGRE_EXCEPT( Exception::ERR_INTERNAL_ERROR, "Unknown texture type", "D3D9Texture::_setSrcAttributes" );
		}
	}
	/****************************************************************************************/
	D3DTEXTUREFILTERTYPE D3D9Texture::_getBestFilterMethod()
	{
		// those MUST be initialized !!!
		assert(mpDev);
		assert(mpD3D);
		assert(mpTex);
		
		DWORD filterCaps = 0;
		// Minification filter is used for mipmap generation
		// Pick the best one supported for this tex type
		switch (this->getTextureType())
		{
		case TEX_TYPE_1D:		// Same as 2D
		case TEX_TYPE_2D:		filterCaps = _getD3D9RenderSystem()->_getDeviceCaps().TextureFilterCaps;		break;
		case TEX_TYPE_3D:		filterCaps = _getD3D9RenderSystem()->_getDeviceCaps().VolumeTextureFilterCaps;	break;
		case TEX_TYPE_CUBE_MAP:	filterCaps = _getD3D9RenderSystem()->_getDeviceCaps().CubeTextureFilterCaps;	break;
		}
		if(filterCaps & D3DPTFILTERCAPS_MINFGAUSSIANQUAD)
			return D3DTEXF_GAUSSIANQUAD;
		
		if(filterCaps & D3DPTFILTERCAPS_MINFPYRAMIDALQUAD)
			return D3DTEXF_PYRAMIDALQUAD;
		
		if(filterCaps & D3DPTFILTERCAPS_MINFANISOTROPIC)
			return D3DTEXF_ANISOTROPIC;
		
		if(filterCaps & D3DPTFILTERCAPS_MINFLINEAR)
			return D3DTEXF_LINEAR;
		
		if(filterCaps & D3DPTFILTERCAPS_MINFPOINT)
			return D3DTEXF_POINT;
		
		return D3DTEXF_POINT;
	}
	/****************************************************************************************/
	bool D3D9Texture::_canUseDynamicTextures(DWORD srcUsage, D3DRESOURCETYPE srcType, D3DFORMAT srcFormat)
	{
		// those MUST be initialized !!!
		assert(mpDev);
		assert(mpD3D);

		// Check for dynamic texture support
		HRESULT hr;
		// check for auto gen. mip maps support
		hr = mpD3D->CheckDeviceFormat(
			_getD3D9RenderSystem()->_getD3DDriver()->getAdapterNumber(),
			D3DDEVTYPE_HAL,
			mBBPixelFormat, 
			srcUsage | D3DUSAGE_DYNAMIC,
			srcType,
			srcFormat);
		if (hr == D3D_OK)
			return true;
		else
			return false;
	}
	/****************************************************************************************/
	bool D3D9Texture::_canAutoGenMipmaps(DWORD srcUsage, D3DRESOURCETYPE srcType, D3DFORMAT srcFormat)
	{
		// those MUST be initialized !!!
		assert(mpDev);
		assert(mpD3D);

		// Hacky override - many (all?) cards seem to not be able to autogen on 
		// textures which are not a power of two
		// Can we even mipmap on 3D textures? Well
		if ((mWidth & mWidth-1) || (mHeight & mHeight-1) || (mDepth & mDepth-1))
			return false;

		if (_getD3D9RenderSystem()->_getDeviceCaps().Caps2 & D3DCAPS2_CANAUTOGENMIPMAP)
		{
			HRESULT hr;
			// check for auto gen. mip maps support
			hr = mpD3D->CheckDeviceFormat(
                    _getD3D9RenderSystem()->_getD3DDriver()->getAdapterNumber(),
                    D3DDEVTYPE_HAL,
					mBBPixelFormat, 
					srcUsage | D3DUSAGE_AUTOGENMIPMAP,
					srcType,
					srcFormat);
			// this HR could a SUCCES
			// but mip maps will not be generated
			if (hr == D3D_OK)
				return true;
			else
				return false;
		}
		else
			return false;
	}
	/****************************************************************************************/
	D3DFORMAT D3D9Texture::_chooseD3DFormat()
	{
		// Choose frame buffer pixel format in case PF_UNKNOWN was requested
		if(mFormat == PF_UNKNOWN)
			return mBBPixelFormat;
		// Choose closest supported D3D format as a D3D format
		return D3D9Mappings::_getPF(D3D9Mappings::_getClosestSupportedPF(mFormat));

	}
	/****************************************************************************************/
	// Macro to hide ugly cast
	#define GETLEVEL(face,mip) \
	 	static_cast<D3D9HardwarePixelBuffer*>(mSurfaceList[face*(mNumMipmaps+1)+mip].get())
	void D3D9Texture::_createSurfaceList(void)
	{
		IDirect3DSurface9 *surface;
		IDirect3DVolume9 *volume;
		D3D9HardwarePixelBuffer *buffer;
		size_t mip, face;
		assert(mpTex);
		// Make sure number of mips is right
		mNumMipmaps = mpTex->GetLevelCount() - 1;
		// Need to know static / dynamic
		unsigned int bufusage;
		if ((mUsage & TU_DYNAMIC) && mDynamicTextures)
		{
			bufusage = HardwareBuffer::HBU_DYNAMIC;
		}
		else
		{
			bufusage = HardwareBuffer::HBU_STATIC;
		}
		if (mUsage & TU_RENDERTARGET)
		{
			bufusage |= TU_RENDERTARGET;
		}
		
		bool updateOldList = mSurfaceList.size() == (getNumFaces() * (mNumMipmaps + 1));
		if(!updateOldList)
		{
			// Create new list of surfaces
			mSurfaceList.clear();
			for(size_t face=0; face<getNumFaces(); ++face)
			{
				for(size_t mip=0; mip<=mNumMipmaps; ++mip)
				{
					buffer = new D3D9HardwarePixelBuffer((HardwareBuffer::Usage)bufusage);
					mSurfaceList.push_back(
						HardwarePixelBufferSharedPtr(buffer)
					);
				}
			}
		}

		switch(getTextureType()) {
		case TEX_TYPE_2D:
		case TEX_TYPE_1D:
			assert(mpNormTex);
			// For all mipmaps, store surfaces as HardwarePixelBufferSharedPtr
			for(mip=0; mip<=mNumMipmaps; ++mip)
			{
				if(mpNormTex->GetSurfaceLevel(mip, &surface) != D3D_OK)
					OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, "Get surface level failed",
		 				"D3D9Texture::_createSurfaceList");
				// decrement reference count, the GetSurfaceLevel call increments this
				// this is safe because the texture keeps a reference as well
				surface->Release();

				GETLEVEL(0, mip)->bind(mpDev, surface, updateOldList);
			}
			break;
		case TEX_TYPE_CUBE_MAP:
			assert(mpCubeTex);
			// For all faces and mipmaps, store surfaces as HardwarePixelBufferSharedPtr
			for(face=0; face<6; ++face)
			{
				for(mip=0; mip<=mNumMipmaps; ++mip)
				{
					if(mpCubeTex->GetCubeMapSurface((D3DCUBEMAP_FACES)face, mip, &surface) != D3D_OK)
						OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, "Get cubemap surface failed",
		 				"D3D9Texture::getBuffer");
					// decrement reference count, the GetSurfaceLevel call increments this
					// this is safe because the texture keeps a reference as well
					surface->Release();
					
					GETLEVEL(face, mip)->bind(mpDev, surface, updateOldList);
				}
			}
			break;
		case TEX_TYPE_3D:
			assert(mpVolumeTex);
			// For all mipmaps, store surfaces as HardwarePixelBufferSharedPtr
			for(mip=0; mip<=mNumMipmaps; ++mip)
			{
				if(mpVolumeTex->GetVolumeLevel(mip, &volume) != D3D_OK)
					OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, "Get volume level failed",
		 				"D3D9Texture::getBuffer");	
				// decrement reference count, the GetSurfaceLevel call increments this
				// this is safe because the texture keeps a reference as well
				volume->Release();
						
				GETLEVEL(0, mip)->bind(mpDev, volume, updateOldList);
			}
			break;
		};
		
		// Set autogeneration of mipmaps for each face of the texture, if it is enabled
		if(mNumRequestedMipmaps != 0 && (mUsage & TU_AUTOMIPMAP)) 
		{
			for(face=0; face<getNumFaces(); ++face)
			{
				GETLEVEL(face, 0)->_setMipmapping(true, mMipmapsHardwareGenerated, mpTex);
			}
		}
	}
	#undef GETLEVEL
	/****************************************************************************************/
	HardwarePixelBufferSharedPtr D3D9Texture::getBuffer(size_t face, size_t mipmap) 
	{
		if(face >= getNumFaces())
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "A three dimensional cube has six faces",
					"D3D9Texture::getBuffer");
		if(mipmap > mNumMipmaps)
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Mipmap index out of range",
					"D3D9Texture::getBuffer");
		size_t idx = face*(mNumMipmaps+1) + mipmap;
		assert(idx < mSurfaceList.size());
		return mSurfaceList[idx];
	}
	
	
	/****************************************************************************************/
	bool D3D9Texture::releaseIfDefaultPool(void)
	{
		if(mD3DPool == D3DPOOL_DEFAULT)
		{
			LogManager::getSingleton().logMessage(
				"Releasing D3D9 default pool texture: " + mName);
			// Just free any internal resources, don't call unload() here
			// because we want the un-touched resource to keep its unloaded status
			// after device reset.
			freeInternalResources();
			LogManager::getSingleton().logMessage(
				"Released D3D9 default pool texture: " + mName);
			return true;
		}
		return false;
	}
	/****************************************************************************************/
	bool D3D9Texture::recreateIfDefaultPool(LPDIRECT3DDEVICE9 pDev)
	{
		bool ret = false;
		if(mD3DPool == D3DPOOL_DEFAULT)
		{
			ret = true;
			LogManager::getSingleton().logMessage(
				"Recreating D3D9 default pool texture: " + mName);
			// We just want to create the texture resources if:
			// 1. This is a render texture, or
			// 2. This is a manual texture with no loader, or
			if ((mIsManual && !mLoader) || (mUsage & TU_RENDERTARGET))
			{
				// just recreate any internal resources
				createInternalResources();
			}
			// This was an unloaded regular texture, or a manual texture with a loader
			else if (!mIsLoaded)
            {
                // Don't create internal resources here, and preserve unloaded state,
                // they will create/load on demand
            }
			// Otherwise, this is a regular loaded texture, or a manual texture with a loader
			else
			{
				// The internal resources already freed, need unload/load here:
				// 1. Make sure resource memory usage statistic correction.
				// 2. Don't call unload() in releaseIfDefaultPool() because we want
				//    the un-touched resource keep unload status after device reset.
				unload();
				// if manual, we need to recreate internal resources since load() won't do that
				if (mIsManual)
					createInternalResources();
				load();
			}
			LogManager::getSingleton().logMessage(
				"Recreated D3D9 default pool texture: " + mName);
		}

		return ret;

	}


	/****************************************************************************************/
    void D3D9RenderTexture::update(void)
    {
        D3D9RenderSystem* rs = static_cast<D3D9RenderSystem*>(
            Root::getSingleton().getRenderSystem());
        if (rs->isDeviceLost())
            return;

        RenderTexture::update();
    }
}
