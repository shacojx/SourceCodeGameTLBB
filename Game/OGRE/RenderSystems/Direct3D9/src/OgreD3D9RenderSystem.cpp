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
#include "OgreD3D9RenderSystem.h"
#include "OgreD3D9Prerequisites.h"
#include "OgreD3D9DriverList.h"
#include "OgreD3D9Driver.h"
#include "OgreD3D9VideoModeList.h"
#include "OgreD3D9VideoMode.h"
#include "OgreD3D9RenderWindow.h"
#include "OgreD3D9TextureManager.h"
#include "OgreD3D9Texture.h"
#include "OgreLogManager.h"
#include "OgreLight.h"
#include "OgreMath.h"
#include "OgreD3D9HardwareBufferManager.h"
#include "OgreD3D9HardwareIndexBuffer.h"
#include "OgreD3D9HardwareVertexBuffer.h"
#include "OgreD3D9VertexDeclaration.h"
#include "OgreD3D9GpuProgram.h"
#include "OgreD3D9GpuProgramManager.h"
//#include "OgreD3D9HLSLProgramFactory.h"
#include "OgreHighLevelGpuProgramManager.h"
#include "OgreD3D9HardwareOcclusionQuery.h"
#include "OgreFrustum.h"
#include "OgreD3D9MultiRenderTarget.h"


namespace Ogre 
{

	//---------------------------------------------------------------------
	D3D9RenderSystem::D3D9RenderSystem( HINSTANCE hInstance )
	{
		OgreGuard( "D3D9RenderSystem::D3D9RenderSystem" );
		LogManager::getSingleton().logMessage( "D3D9 : " + getName() + " created." );

		// set the instance being passed 
		mhInstance = hInstance;

		// set pointers to NULL
		mpD3D = NULL;
		mpD3DDevice = NULL;
		mDriverList = NULL;
		mActiveD3DDriver = NULL;
        mTextureManager = NULL;
        mHardwareBufferManager = NULL;
		mGpuProgramManager = NULL;
		mPrimaryWindow = NULL;
		mDeviceLost = false;
		mBasicStatesInitialised = false;
		mUseNVPerfHUD = false;
        //mHLSLProgramFactory = NULL;

		// init lights
		for(int i = 0; i < MAX_LIGHTS; i++ )
			mLights[i] = 0;

		// Create our Direct3D object
		if( NULL == (mpD3D = Direct3DCreate9(D3D_SDK_VERSION)) )
			OGRE_EXCEPT( Exception::ERR_INTERNAL_ERROR, "Failed to create Direct3D9 object", "D3D9RenderSystem::D3D9RenderSystem" );

		// set config options defaults
		initConfigOptions();

		// fsaa options
		mFSAAType = D3DMULTISAMPLE_NONE;
		mFSAAQuality = 0;

		// set stages desc. to defaults
		for (size_t n = 0; n < OGRE_MAX_TEXTURE_LAYERS; n++)
		{
			mTexStageDesc[n].autoTexCoordType = TEXCALC_NONE;
			mTexStageDesc[n].coordIndex = 0;
			mTexStageDesc[n].texType = TEX_TYPE_2D;
			mTexStageDesc[n].pTex = 0;
		}

		mLastVertexSourceCount = 0;

        mCurrentLights = 0;

		// Enumerate events
		mEventNames.push_back("DeviceLost");
		mEventNames.push_back("DeviceRestored");


		OgreUnguard();
	}
	//---------------------------------------------------------------------
	D3D9RenderSystem::~D3D9RenderSystem()
	{
		OgreGuard( "D3D9RenderSystem::~D3D9RenderSystem" );
        shutdown();

		
		
		SAFE_DELETE( mDriverList );
		SAFE_DELETE( mTextureManager );
        SAFE_DELETE(mHardwareBufferManager);
        //SAFE_DELETE(mHLSLProgramFactory);
		SAFE_DELETE(mGpuProgramManager);
		SAFE_RELEASE( mpD3D );

		LogManager::getSingleton().logMessage( "D3D9 : " + getName() + " destroyed." );
		OgreUnguard();
	}
	//---------------------------------------------------------------------
	const String& D3D9RenderSystem::getName() const
	{
		static String strName( "Direct3D9 Rendering Subsystem");
		return strName;
	}
	//---------------------------------------------------------------------
	D3D9DriverList* D3D9RenderSystem::getDirect3DDrivers()
	{
		if( !mDriverList )
			mDriverList = new D3D9DriverList( mpD3D );

		return mDriverList;
	}
	//---------------------------------------------------------------------
	bool D3D9RenderSystem::_checkMultiSampleQuality(D3DMULTISAMPLE_TYPE type, DWORD *outQuality, D3DFORMAT format, UINT adapterNum, D3DDEVTYPE deviceType, BOOL fullScreen)
	{
		HRESULT hr;
		hr = mpD3D->CheckDeviceMultiSampleType( 
				adapterNum, 
				deviceType, 
				format, 
				fullScreen, 
				type, 
				outQuality);

		if (SUCCEEDED(hr))
			return true;
		else
			return false;
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::initConfigOptions()
	{
		OgreGuard( "D3D9RenderSystem::initConfigOptions" );

		D3D9DriverList* driverList;
		D3D9Driver* driver;

		ConfigOption optDevice;
		ConfigOption optVideoMode;
		ConfigOption optFullScreen;
		ConfigOption optVSync;
		ConfigOption optAA;
		ConfigOption optFPUMode;
		ConfigOption optNVPerfHUD;
		ConfigOption optVertexProcessing;
        ConfigOption optVertexProgramEmulation;
        ConfigOption optFragmentProgramEmulation;

		driverList = this->getDirect3DDrivers();

		optDevice.name = "Rendering Device";
		optDevice.currentValue.clear();
		optDevice.possibleValues.clear();
		optDevice.immutable = false;

		optVideoMode.name = "Video Mode";
		optVideoMode.currentValue = "800 x 600 @ 32-bit colour";
		optVideoMode.immutable = false;

		optFullScreen.name = "Full Screen";
		optFullScreen.possibleValues.push_back( "Yes" );
		optFullScreen.possibleValues.push_back( "No" );
		optFullScreen.currentValue = "Yes";
		optFullScreen.immutable = false;

		for( unsigned j=0; j < driverList->count(); j++ )
		{
			driver = driverList->item(j);
			optDevice.possibleValues.push_back( driver->DriverDescription() );
			// Make first one default
			if( j==0 )
				optDevice.currentValue = driver->DriverDescription();
		}

		optVSync.name = "VSync";
		optVSync.immutable = false;
		optVSync.possibleValues.push_back( "Yes" );
		optVSync.possibleValues.push_back( "No" );
		optVSync.currentValue = "No";

		optAA.name = "Anti aliasing";
		optAA.immutable = false;
		optAA.possibleValues.push_back( "None" );
		optAA.currentValue = "None";

		optFPUMode.name = "Floating-point mode";
#if OGRE_DOUBLE_PRECISION
		optFPUMode.currentValue = "Consistent";
#else
		optFPUMode.currentValue = "Fastest";
#endif
		optFPUMode.possibleValues.clear();
		optFPUMode.possibleValues.push_back("Fastest");
		optFPUMode.possibleValues.push_back("Consistent");
		optFPUMode.immutable = false;

		optNVPerfHUD.currentValue = "No";
		optNVPerfHUD.immutable = false;
		optNVPerfHUD.name = "Allow NVPerfHUD";
		optNVPerfHUD.possibleValues.push_back( "Yes" );
		optNVPerfHUD.possibleValues.push_back( "No" );

		optVertexProcessing.name = "Vertex Processing";
		optVertexProcessing.currentValue = "Hardware";
		optVertexProcessing.possibleValues.push_back("Hardware");
		optVertexProcessing.possibleValues.push_back("Software");
        optVertexProcessing.possibleValues.push_back("Require Vertex Shader");
		optVertexProcessing.immutable = false;

        optVertexProgramEmulation.name = "Vertex Program Emulation";
		optVertexProgramEmulation.currentValue = "As is";
		optVertexProgramEmulation.possibleValues.push_back("As is");
		optVertexProgramEmulation.possibleValues.push_back("Fixed-function Only");
		optVertexProgramEmulation.immutable = false;

        optFragmentProgramEmulation.name = "Fragment Program Emulation";
		optFragmentProgramEmulation.currentValue = "As is";
		optFragmentProgramEmulation.possibleValues.push_back("As is");
		optFragmentProgramEmulation.possibleValues.push_back("Fixed-function Only");
		optFragmentProgramEmulation.immutable = false;

		mOptions[optDevice.name] = optDevice;
		mOptions[optVideoMode.name] = optVideoMode;
		mOptions[optFullScreen.name] = optFullScreen;
		mOptions[optVSync.name] = optVSync;
		mOptions[optAA.name] = optAA;
		mOptions[optFPUMode.name] = optFPUMode;
		//mOptions[optNVPerfHUD.name] = optNVPerfHUD;
		mOptions[optVertexProcessing.name] = optVertexProcessing;
		mOptions[optVertexProgramEmulation.name] = optVertexProgramEmulation;
		mOptions[optFragmentProgramEmulation.name] = optFragmentProgramEmulation;

		refreshD3DSettings();

		OgreUnguard();
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::refreshD3DSettings()
	{
		OgreGuard( "D3D9RenderSystem::refreshD3DSettings" );

		ConfigOption* optVideoMode;
		D3D9Driver* driver = 0;
		D3D9VideoMode* videoMode;

		ConfigOptionMap::iterator opt = mOptions.find( "Rendering Device" );
		if( opt != mOptions.end() )
		{
			for( unsigned j=0; j < getDirect3DDrivers()->count(); j++ )
			{
				driver = getDirect3DDrivers()->item(j);
				if( driver->DriverDescription() == opt->second.currentValue )
					break;
			}

			if (driver)
			{
				opt = mOptions.find( "Video Mode" );
				optVideoMode = &opt->second;
				optVideoMode->possibleValues.clear();
				// get vide modes for this device
				for( unsigned k=0; k < driver->getVideoModeList()->count(); k++ )
				{
					videoMode = driver->getVideoModeList()->item( k );
					optVideoMode->possibleValues.push_back( videoMode->getDescription() );
				}

                // Reset video mode to default if previous doesn't avail in new possible values
                StringVector::const_iterator itValue =
                    std::find(optVideoMode->possibleValues.begin(),
                              optVideoMode->possibleValues.end(),
                              optVideoMode->currentValue);
                if (itValue == optVideoMode->possibleValues.end())
                {
                    optVideoMode->currentValue = "800 x 600 @ 32-bit colour";
                }

                // Also refresh FSAA options
                refreshFSAAOptions();
			}
		}

		OgreUnguard();
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::setConfigOption( const String &name, const String &value )
	{
		OgreGuard( "D3D9RenderSystem::setConfigOption" );

        StringUtil::StrStreamType str;
        str << "D3D9 : RenderSystem Option: " << name << " = " << value;
		LogManager::getSingleton().logMessage(str.str());

        bool viewModeChanged = false;

		// Find option
		ConfigOptionMap::iterator it = mOptions.find( name );

		// Update
		if( it != mOptions.end() )
			it->second.currentValue = value;
		else
		{
            str.str(StringUtil::BLANK);
            str << "Option named '" << name << "' does not exist.";
			OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, str.str(), "D3D9RenderSystem::setConfigOption" );
		}

		// Refresh other options if D3DDriver changed
		if( name == "Rendering Device" )
			refreshD3DSettings();

		if( name == "Full Screen" )
		{
			// Video mode is applicable
			it = mOptions.find( "Video Mode" );
			if (it->second.currentValue.empty())
            {
				it->second.currentValue = "800 x 600 @ 32-bit colour";
                viewModeChanged = true;
            }
		}

		if( name == "Anti aliasing" )
		{
			if (value == "None")
				_setFSAA(D3DMULTISAMPLE_NONE, 0);
			else 
			{
				D3DMULTISAMPLE_TYPE fsaa = D3DMULTISAMPLE_NONE;
				DWORD level = 0;

				if (StringUtil::startsWith(value, "NonMaskable", false))
				{
					fsaa = D3DMULTISAMPLE_NONMASKABLE;
					size_t pos = value.find_last_of(" ");
					String sNum = value.substr(pos + 1);
					level = StringConverter::parseInt(sNum);
					level -= 1;
				}
				else if (StringUtil::startsWith(value, "Level", false))
				{
					size_t pos = value.find_last_of(" ");
					String sNum = value.substr(pos + 1);
					fsaa = (D3DMULTISAMPLE_TYPE)StringConverter::parseInt(sNum);
				}

				_setFSAA(fsaa, level);
			}
		}

		if( name == "VSync" )
		{
			if (value == "Yes")
				mVSync = true;
			else
				mVSync = false;
		}

		if( name == "Allow NVPerfHUD" )
		{
			if (value == "Yes")
				mUseNVPerfHUD = true;
			else
				mUseNVPerfHUD = false;
		}

		if (viewModeChanged || name == "Video Mode")
		{
            refreshFSAAOptions();
		}

		OgreUnguard();
	}
	//---------------------------------------------------------------------
    void D3D9RenderSystem::refreshFSAAOptions(void)
    {
        OgreGuard( "D3D9RenderSystem::refreshFSAAOptions" );

        ConfigOptionMap::iterator it = mOptions.find( "Anti aliasing" );
        ConfigOption* optFSAA = &it->second;
        optFSAA->possibleValues.clear();
        optFSAA->possibleValues.push_back("None");

        it = mOptions.find("Rendering Device");
        D3D9Driver *driver = getDirect3DDrivers()->item(it->second.currentValue);
        if (driver)
        {
            it = mOptions.find("Video Mode");
            D3D9VideoMode *videoMode = driver->getVideoModeList()->item(it->second.currentValue);
            if (videoMode)
            {
                // get non maskable FSAA for this VMODE
                DWORD numLevels = 0;
                bool bOK = this->_checkMultiSampleQuality(
                    D3DMULTISAMPLE_NONMASKABLE, 
                    &numLevels, 
                    videoMode->getFormat(), 
                    driver->getAdapterNumber(),
                    D3DDEVTYPE_HAL,
                    TRUE);
                if (bOK && numLevels > 0)
                {
                    for (DWORD n = 0; n < numLevels; n++)
                        optFSAA->possibleValues.push_back("NonMaskable " + StringConverter::toString(n + 1));
                }

                // set maskable levels supported
                for (unsigned int n = 2; n < 17; n++)
                {
                    bOK = this->_checkMultiSampleQuality(
                        (D3DMULTISAMPLE_TYPE)n, 
                        &numLevels, 
                        videoMode->getFormat(), 
                        driver->getAdapterNumber(),
                        D3DDEVTYPE_HAL,
                        TRUE);
                    if (bOK)
                        optFSAA->possibleValues.push_back("Level " + StringConverter::toString(n));
                }
            }
        }

        // Reset FSAA to none if previous doesn't avail in new possible values
        StringVector::const_iterator itValue =
            std::find(optFSAA->possibleValues.begin(),
                      optFSAA->possibleValues.end(),
                      optFSAA->currentValue);
        if (itValue == optFSAA->possibleValues.end())
        {
            optFSAA->currentValue = "None";
        }

        OgreUnguard();
    }
	//---------------------------------------------------------------------
	String D3D9RenderSystem::validateConfigOptions()
	{
		ConfigOptionMap::iterator it;
		
		// check if video mode is selected
		it = mOptions.find( "Video Mode" );
		if (it->second.currentValue.empty())
			return "A video mode must be selected.";

		it = mOptions.find( "Rendering Device" );
		bool foundDriver = false;
		D3D9DriverList* driverList = getDirect3DDrivers();
		for( ushort j=0; j < driverList->count(); j++ )
		{
			if( driverList->item(j)->DriverDescription() == it->second.currentValue )
			{
				foundDriver = true;
				break;
			}
		}

		if (!foundDriver)
		{
			// Just pick the first driver
			setConfigOption("Rendering Device", driverList->item(0)->DriverDescription());
			return "Your DirectX driver name has changed since the last time you ran OGRE; "
				"the 'Rendering Device' has been changed.";
		}

        it = mOptions.find( "VSync" );
		if( it->second.currentValue == "Yes" )
			mVSync = true;
		else
			mVSync = false;

        return StringUtil::BLANK;
	}
	//---------------------------------------------------------------------
	ConfigOptionMap& D3D9RenderSystem::getConfigOptions()
	{
		// return a COPY of the current config options
		return mOptions;
	}
	//---------------------------------------------------------------------
	RenderWindow* D3D9RenderSystem::initialise( bool autoCreateWindow, const String& windowTitle )
	{
		RenderWindow* autoWindow = NULL;
		LogManager::getSingleton().logMessage( "D3D9 : Subsystem Initialising" );

		// Init using current settings
		mActiveD3DDriver = NULL;
		ConfigOptionMap::iterator opt = mOptions.find( "Rendering Device" );
		for( unsigned j=0; j < getDirect3DDrivers()->count(); j++ )
		{
			if( getDirect3DDrivers()->item(j)->DriverDescription() == opt->second.currentValue )
			{
				mActiveD3DDriver = getDirect3DDrivers()->item(j);
				break;
			}
		}

		if( !mActiveD3DDriver )
			OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, "Problems finding requested Direct3D driver!", "D3D9RenderSystem::initialise" );

		if( autoCreateWindow )
		{
			bool fullScreen;
			opt = mOptions.find( "Full Screen" );
			if( opt == mOptions.end() )
				OGRE_EXCEPT( Exception::ERR_INTERNAL_ERROR, "Can't find full screen option!", "D3D9RenderSystem::initialise" );
			fullScreen = opt->second.currentValue == "Yes";

			D3D9VideoMode* videoMode = NULL;
			unsigned int width, height;
			String temp;

			opt = mOptions.find( "Video Mode" );
			if( opt == mOptions.end() )
				OGRE_EXCEPT( Exception::ERR_INTERNAL_ERROR, "Can't find Video Mode option!", "D3D9RenderSystem::initialise" );

			for( unsigned j=0; j < mActiveD3DDriver->getVideoModeList()->count(); j++ )
			{
				temp = mActiveD3DDriver->getVideoModeList()->item(j)->getDescription();
				if( temp == opt->second.currentValue )
				{
					videoMode = mActiveD3DDriver->getVideoModeList()->item(j);
					break;
				}
			}

			if( !videoMode )
				OGRE_EXCEPT( Exception::ERR_INTERNAL_ERROR, "Can't find requested video mode.", "D3D9RenderSystem::initialise" );

			width = videoMode->getWidth();
			height = videoMode->getHeight();
			
			NameValuePairList miscParams;
			miscParams["colourDepth"] = StringConverter::toString(videoMode->getColourDepth());
			miscParams["FSAA"] = StringConverter::toString(mFSAAType);
			miscParams["FSAAQuality"] = StringConverter::toString(mFSAAQuality);
			miscParams["vsync"] = StringConverter::toString(mVSync);
			//miscParams["useNVPerfHUD"] = StringConverter::toString(mUseNVPerfHUD);

			autoWindow = this->createRenderWindow( windowTitle, width, height, 
				fullScreen, &miscParams );

            // If we have 16bit depth buffer enable w-buffering.
            assert( autoWindow );
            if ( autoWindow->getColourDepth() == 16 ) 
            { 
                mWBuffer = true;
            } 
            else 
            {
                mWBuffer = false;
            }
		}

        LogManager::getSingleton().logMessage("***************************************");

		LogManager::getSingleton().logMessage("*** D3D9 : Subsystem Initialised OK ***");
        LogManager::getSingleton().logMessage("***************************************");

		// call superclass method
		RenderSystem::initialise( autoCreateWindow );


		return autoWindow;
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::_setFSAA(D3DMULTISAMPLE_TYPE type, DWORD qualityLevel)
	{
		if (!mpD3DDevice)
		{
			mFSAAType = type;
			mFSAAQuality = qualityLevel;
		}
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::reinitialise()
	{
		LogManager::getSingleton().logMessage( "D3D9 : Reinitialising" );
		this->shutdown();
		this->initialise( true );
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::shutdown()
	{
		RenderSystem::shutdown();
		freeDevice();
		SAFE_DELETE( mDriverList );
		mActiveD3DDriver = NULL;
		mpD3DDevice = NULL;
		mBasicStatesInitialised = false;
		LogManager::getSingleton().logMessage("D3D9 : Shutting down cleanly.");
	}
	//---------------------------------------------------------------------
	RenderWindow* D3D9RenderSystem::createRenderWindow(const String &name, 
		unsigned int width, unsigned int height, bool fullScreen,
		const NameValuePairList *miscParams)
	{

		OgreGuard( "D3D9RenderSystem::createRenderWindow" );

		// Check we're not creating a secondary window when the primary
		// was fullscreen
		if (mPrimaryWindow && mPrimaryWindow->isFullScreen())
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
				"Cannot create secondary windows when the primary is full screen",
				"D3D9RenderSystem::createRenderWindow");
		}
		if (mPrimaryWindow && fullScreen)
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
				"Cannot create full screen secondary windows",
				"D3D9RenderSystem::createRenderWindow");
		}
		
		// Log a message
		std::stringstream ss;
		ss << "D3D9RenderSystem::createRenderWindow \"" << name << "\", " <<
			width << "x" << height << " ";
		if(fullScreen)
			ss << "fullscreen ";
		else
			ss << "windowed ";
		if(miscParams)
		{
			ss << " miscParams: ";
			NameValuePairList::const_iterator it;
			for(it=miscParams->begin(); it!=miscParams->end(); ++it)
			{
				ss << it->first << "=" << it->second << " ";
			}
			LogManager::getSingleton().logMessage(ss.str());
		}
		
		String msg;

		// Make sure we don't already have a render target of the 
		// sam name as the one supplied
		if( mRenderTargets.find( name ) != mRenderTargets.end() )
		{
			msg = "A render target of the same name '" + name + "' already "
				"exists.  You cannot create a new window with this name.";
			OGRE_EXCEPT( Exception::ERR_INTERNAL_ERROR, msg, "D3D9RenderSystem::createRenderWindow" );
		}

		RenderWindow* win = new D3D9RenderWindow(mhInstance, mActiveD3DDriver, 
			mPrimaryWindow ? mpD3DDevice : 0);

		win->create( name, width, height, fullScreen, miscParams);

		attachRenderTarget( *win );

		// If this is the first window, get the D3D device and create the texture manager
		if( !mPrimaryWindow )
		{
			mPrimaryWindow = (D3D9RenderWindow *)win;
			win->getCustomAttribute( "D3DDEVICE", &mpD3DDevice );

			// Create the texture manager for use by others
			mTextureManager = new D3D9TextureManager( mpD3DDevice );
            // Also create hardware buffer manager
            mHardwareBufferManager = new D3D9HardwareBufferManager(mpD3DDevice);

			// Create the GPU program manager
			mGpuProgramManager = new D3D9GpuProgramManager(mpD3DDevice);
            // create & register HLSL factory
            //mHLSLProgramFactory = new D3D9HLSLProgramFactory();
            //HighLevelGpuProgramManager::getSingleton().addFactory(mHLSLProgramFactory);
            mGpuProgramManager->_pushSyntaxCode("hlsl");


            // Initialise the capabilities structures
            initCapabilities();

		}
		else
		{
			mSecondaryWindows.push_back(static_cast<D3D9RenderWindow *>(win));
		}

		OgreUnguardRet( win );
	}
    //---------------------------------------------------------------------
    void D3D9RenderSystem::initCapabilities(void)
    {
		// get caps
		mpD3DDevice->GetDeviceCaps( &mCaps );

        // Check for hardware stencil support
		LPDIRECT3DSURFACE9 pSurf;
		D3DSURFACE_DESC surfDesc;
		mpD3DDevice->GetDepthStencilSurface(&pSurf);
		pSurf->GetDesc(&surfDesc);
		pSurf->Release();

		if (surfDesc.Format == D3DFMT_D24S8 || surfDesc.Format == D3DFMT_D24X8)
		{
			mCapabilities->setCapability(RSC_HWSTENCIL);
			// Actually, it's always 8-bit
			mCapabilities->setStencilBufferBitDepth(8);

		}

		// Set number of texture units
		mCapabilities->setNumTextureUnits(mCaps.MaxSimultaneousTextures);
        // Anisotropy?
        if (mCaps.MaxAnisotropy > 1)
            mCapabilities->setCapability(RSC_ANISOTROPY);
        // Automatic mipmap generation?
        if (mCaps.Caps2 & D3DCAPS2_CANAUTOGENMIPMAP)
            mCapabilities->setCapability(RSC_AUTOMIPMAP);
        // Blending between stages supported
        mCapabilities->setCapability(RSC_BLENDING);
        // Dot 3
        if (mCaps.TextureOpCaps & D3DTEXOPCAPS_DOTPRODUCT3)
            mCapabilities->setCapability(RSC_DOT3);
        // Cube map
        if (mCaps.TextureCaps & D3DPTEXTURECAPS_CUBEMAP)
            mCapabilities->setCapability(RSC_CUBEMAPPING);

        // We always support compression, D3DX will decompress if device does not support
        mCapabilities->setCapability(RSC_TEXTURE_COMPRESSION);
        mCapabilities->setCapability(RSC_TEXTURE_COMPRESSION_DXT);

        // We always support VBOs
        mCapabilities->setCapability(RSC_VBO);

        // Scissor test
        if (mCaps.RasterCaps & D3DPRASTERCAPS_SCISSORTEST)
            mCapabilities->setCapability(RSC_SCISSOR_TEST);

        // Two-sided stencil
        if (mCaps.StencilCaps & D3DSTENCILCAPS_TWOSIDED)
            mCapabilities->setCapability(RSC_TWO_SIDED_STENCIL);

        // stencil wrap
        if ((mCaps.StencilCaps & D3DSTENCILCAPS_INCR) && 
            (mCaps.StencilCaps & D3DSTENCILCAPS_DECR))
            mCapabilities->setCapability(RSC_STENCIL_WRAP);

        // Check for hardware occlusion support
        if ( ( mpD3DDevice->CreateQuery(D3DQUERYTYPE_OCCLUSION,  NULL ) ) == D3D_OK )	
        {
            mCapabilities->setCapability(RSC_HWOCCLUSION);
        }
        convertVertexShaderCaps();
        convertPixelShaderCaps();

		// User clip planes
        if (mCaps.MaxUserClipPlanes > 0)
		{
			mCapabilities->setCapability(RSC_USER_CLIP_PLANES);
		}

		// UBYTE4 type?
		if (mCaps.DeclTypes & D3DDTCAPS_UBYTE4)
		{
			mCapabilities->setCapability(RSC_VERTEX_FORMAT_UBYTE4);
		}

		// Infinite projection?
		// We have no capability for this, so we have to base this on our
		// experience and reports from users
		// Non-vertex program capable hardware does not appear to support it
		if (mCapabilities->hasCapability(RSC_VERTEX_PROGRAM))
		{
			// GeForce4 Ti (and presumably GeForce3) does not
			// render infinite projection properly, even though it does in GL
            // So exclude all cards prior to the FX range from doing infinite
            const D3DADAPTER_IDENTIFIER9& adapterID = mActiveD3DDriver->getAdapterIdentifier();
			if (adapterID.VendorId != 0x10DE || // not nVidia
				!((adapterID.DeviceId >= 0x200 && adapterID.DeviceId <= 0x20F) || //gf3
				  (adapterID.DeviceId >= 0x250 && adapterID.DeviceId <= 0x25F) || //gf4ti
				  (adapterID.DeviceId >= 0x280 && adapterID.DeviceId <= 0x28F) || //gf4ti
				  (adapterID.DeviceId >= 0x170 && adapterID.DeviceId <= 0x18F) || //gf4 go
				  (adapterID.DeviceId >= 0x280 && adapterID.DeviceId <= 0x28F)))  //gf4ti go
			{
				mCapabilities->setCapability(RSC_INFINITE_FAR_PLANE);
			}
			
		}
		
		// 3D textures?
		if (mCaps.TextureCaps & D3DPTEXTURECAPS_VOLUMEMAP)
		{
			mCapabilities->setCapability(RSC_TEXTURE_3D);
		}
		
        if ((mCaps.TextureCaps & D3DPTEXTURECAPS_POW2) == 0)
		{
			// unrestricted non POW2
			mCapabilities->setCapability(RSC_NON_POWER_OF_2_TEXTURES);
		}
		else if (mCaps.TextureCaps & D3DPTEXTURECAPS_NONPOW2CONDITIONAL)
		{
			// Conditional support for non POW2
			mCapabilities->setCapability(RSC_NON_POWER_OF_2_TEXTURES);
			mCapabilities->setNonPOW2TexturesLimited(true);

		}

		// We always support rendertextures bigger than the frame buffer
        mCapabilities->setCapability(RSC_HWRENDER_TO_TEXTURE);

		// Determine if any floating point texture format is supported
		D3DFORMAT floatFormats[6] = {D3DFMT_R16F, D3DFMT_G16R16F, 
			D3DFMT_A16B16G16R16F, D3DFMT_R32F, D3DFMT_G32R32F, 
			D3DFMT_A32B32G32R32F};
		LPDIRECT3DSURFACE9 bbSurf;
		mPrimaryWindow->getCustomAttribute("DDBACKBUFFER", &bbSurf);
		D3DSURFACE_DESC bbSurfDesc;
		bbSurf->GetDesc(&bbSurfDesc);
		
		for (int i = 0; i < 6; ++i)
		{
			if (SUCCEEDED(mpD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, 
				D3DDEVTYPE_HAL, bbSurfDesc.Format, 
				0, D3DRTYPE_TEXTURE, floatFormats[i])))
			{
				mCapabilities->setCapability(RSC_TEXTURE_FLOAT);
				break;
			}
			
		}
		
		// Number of render targets
		mCapabilities->setNumMultiRenderTargets(std::min((ushort)mCaps.NumSimultaneousRTs, (ushort)OGRE_MAX_MULTIPLE_RENDER_TARGETS));

		//
		if(mCaps.PrimitiveMiscCaps & D3DPMISCCAPS_MRTINDEPENDENTBITDEPTHS)
		{
			LogManager::getSingleton().logMessage("Multiple render targets with independent bit depths supported");
		}

		// Point sprites 
		if (mCaps.MaxPointSize > 1.0f)
		{
			mCapabilities->setCapability(RSC_POINT_SPRITES);
			// sprites and extended parameters go together in D3D
			mCapabilities->setCapability(RSC_POINT_EXTENDED_PARAMETERS);
			mCapabilities->setMaxPointSize(mCaps.MaxPointSize);
		}
		

		Log* defaultLog = LogManager::getSingleton().getDefaultLog();
		if (defaultLog)
		{
			mCapabilities->log(defaultLog);
		}

		// Do we support per-stage src_manual constants?
		// HACK - ATI drivers seem to be buggy and don't support per-stage constants properly?
		mPerStageConstantSupport = (mCaps.PrimitiveMiscCaps & D3DPMISCCAPS_PERSTAGECONSTANT) != 0;

    }
    //---------------------------------------------------------------------
    void D3D9RenderSystem::convertVertexShaderCaps(void)
    {
        ushort major, minor;
        major = static_cast<ushort>((mCaps.VertexShaderVersion & 0x0000FF00) >> 8);
        minor = static_cast<ushort>(mCaps.VertexShaderVersion & 0x000000FF);

        ConfigOptionMap::const_iterator opti = mOptions.find("Vertex Program Emulation");
        if (opti != mOptions.end())
        {
            if (opti->second.currentValue == "Fixed-function Only")
            {
                if (major)
                {
                    LogManager::getSingleton().logMessage(
                        "WARNING: Vertex program capabilities disabled by config option.");
                }
                return;
            }
        }

        bool vs2x = false;
        bool vs2a = false;

        // Special case detection for vs_2_x/a support
        if (major >= 2)
        {
            if ((mCaps.VS20Caps.Caps & D3DVS20CAPS_PREDICATION) &&
                (mCaps.VS20Caps.DynamicFlowControlDepth > 0) &&
                (mCaps.VS20Caps.NumTemps >= 12))
            {
                vs2x = true;
            }

            if ((mCaps.VS20Caps.Caps & D3DVS20CAPS_PREDICATION) &&
                (mCaps.VS20Caps.DynamicFlowControlDepth > 0) &&
                (mCaps.VS20Caps.NumTemps >= 13))
            {
                vs2a = true;
            }
        }

        // Populate max version & params
        switch (major)
        {
        case 1:
            mCapabilities->setMaxVertexProgramVersion("vs_1_1");
            // No boolean params allowed
            mCapabilities->setVertexProgramConstantBoolCount(0);
            // No integer params allowed
            mCapabilities->setVertexProgramConstantIntCount(0);
            // float params, always 4D
            mCapabilities->setVertexProgramConstantFloatCount(mCaps.MaxVertexShaderConst);
           
            break;
        case 2:
            if (vs2a)
            {
                mCapabilities->setMaxVertexProgramVersion("vs_2_a");
            }
            else if (vs2x)
            {
                mCapabilities->setMaxVertexProgramVersion("vs_2_x");
            }
            else
            {
                mCapabilities->setMaxVertexProgramVersion("vs_2_0");
            }
            // 16 boolean params allowed
            mCapabilities->setVertexProgramConstantBoolCount(16);
            // 16 integer params allowed, 4D
            mCapabilities->setVertexProgramConstantIntCount(16);
            // float params, always 4D
            mCapabilities->setVertexProgramConstantFloatCount(mCaps.MaxVertexShaderConst);
            break;
        case 3:
            mCapabilities->setMaxVertexProgramVersion("vs_3_0");
            // 16 boolean params allowed
            mCapabilities->setVertexProgramConstantBoolCount(16);
            // 16 integer params allowed, 4D
            mCapabilities->setVertexProgramConstantIntCount(16);
            // float params, always 4D
            mCapabilities->setVertexProgramConstantFloatCount(mCaps.MaxVertexShaderConst);
            break;
        default:
            mCapabilities->setMaxVertexProgramVersion(StringUtil::BLANK);
            break;
        }

        // populate syntax codes in program manager (no breaks in this one so it falls through)
        switch(major)
        {
        case 3:
            mGpuProgramManager->_pushSyntaxCode("vs_3_0");
        case 2:
            if (vs2x)
                mGpuProgramManager->_pushSyntaxCode("vs_2_x");
            if (vs2a)
                mGpuProgramManager->_pushSyntaxCode("vs_2_a");

            mGpuProgramManager->_pushSyntaxCode("vs_2_0");
        case 1:
            mGpuProgramManager->_pushSyntaxCode("vs_1_1");
            mCapabilities->setCapability(RSC_VERTEX_PROGRAM);
        }
    }
    //---------------------------------------------------------------------
    void D3D9RenderSystem::convertPixelShaderCaps(void)
    {
        ushort major, minor;
        major = static_cast<ushort>((mCaps.PixelShaderVersion & 0x0000FF00) >> 8);
        minor = static_cast<ushort>(mCaps.PixelShaderVersion & 0x000000FF);

        ConfigOptionMap::const_iterator opti = mOptions.find("Fragment Program Emulation");
        if (opti != mOptions.end())
        {
            if (opti->second.currentValue == "Fixed-function Only")
            {
                if (major)
                {
                    LogManager::getSingleton().logMessage(
                        "WARNING: Fragment program capabilities disabled by config option.");
                }
                return;
            }
        }

		bool ps2a = false;
        bool ps2b = false;
        bool ps2x = false;

        // Special case detection for ps_2_x/a/b support
        if (major >= 2)
        {
			if ((mCaps.PS20Caps.Caps & D3DPS20CAPS_NOTEXINSTRUCTIONLIMIT) &&
                (mCaps.PS20Caps.NumTemps >= 32))
            {
                ps2b = true;
            }

	        if ((mCaps.PS20Caps.Caps & D3DPS20CAPS_NOTEXINSTRUCTIONLIMIT) &&
                (mCaps.PS20Caps.Caps & D3DPS20CAPS_NODEPENDENTREADLIMIT) &&
                (mCaps.PS20Caps.Caps & D3DPS20CAPS_ARBITRARYSWIZZLE) &&
				(mCaps.PS20Caps.Caps & D3DPS20CAPS_GRADIENTINSTRUCTIONS) &&
				(mCaps.PS20Caps.Caps & D3DPS20CAPS_PREDICATION) &&
                (mCaps.PS20Caps.NumTemps >= 22))
	        {
                ps2a = true;
			}

            // Does this enough?
            if (ps2a || ps2b)
            {
                ps2x = true;
            }
        }

        switch (major)
        {
        case 1:
            switch(minor)
            {
            case 1:
                mCapabilities->setMaxFragmentProgramVersion("ps_1_1");
                break;
            case 2:
                mCapabilities->setMaxFragmentProgramVersion("ps_1_2");
                break;
            case 3:
                mCapabilities->setMaxFragmentProgramVersion("ps_1_3");
                break;
            case 4:
                mCapabilities->setMaxFragmentProgramVersion("ps_1_4");
                break;
            }
            // no boolean params allowed
            mCapabilities->setFragmentProgramConstantBoolCount(0);
            // no integer params allowed
            mCapabilities->setFragmentProgramConstantIntCount(0);
            // float params, always 4D
            // NB in ps_1_x these are actually stored as fixed point values,
            // but they are entered as floats
            mCapabilities->setFragmentProgramConstantFloatCount(8);
            break;
        case 2:
			if (ps2a)
            {
                mCapabilities->setMaxFragmentProgramVersion("ps_2_a");
            }
			else if (ps2b)
            {
                mCapabilities->setMaxFragmentProgramVersion("ps_2_b");
            }
			else if (ps2x)
            {
                mCapabilities->setMaxFragmentProgramVersion("ps_2_x");
            }
            else
            {
                mCapabilities->setMaxFragmentProgramVersion("ps_2_0");
            }
            // 16 boolean params allowed
            mCapabilities->setFragmentProgramConstantBoolCount(16);
            // 16 integer params allowed, 4D
            mCapabilities->setFragmentProgramConstantIntCount(16);
            // float params, always 4D
            mCapabilities->setFragmentProgramConstantFloatCount(32);
            break;
        case 3:
            if (minor > 0)
            {
                mCapabilities->setMaxFragmentProgramVersion("ps_3_x");
            }
            else
            {
                mCapabilities->setMaxFragmentProgramVersion("ps_3_0");
            }
            // 16 boolean params allowed
            mCapabilities->setFragmentProgramConstantBoolCount(16);
            // 16 integer params allowed, 4D
            mCapabilities->setFragmentProgramConstantIntCount(16);
            // float params, always 4D
            mCapabilities->setFragmentProgramConstantFloatCount(224);
            break;
        default:
            mCapabilities->setMaxFragmentProgramVersion(StringUtil::BLANK);
            break;
        }

        // populate syntax codes in program manager (no breaks in this one so it falls through)
        switch(major)
        {
        case 3:
            if (minor > 0)
                mGpuProgramManager->_pushSyntaxCode("ps_3_x");

            mGpuProgramManager->_pushSyntaxCode("ps_3_0");
        case 2:
            if (ps2x)
                mGpuProgramManager->_pushSyntaxCode("ps_2_x");
            if (ps2a)
                mGpuProgramManager->_pushSyntaxCode("ps_2_a");
            if (ps2b)
                mGpuProgramManager->_pushSyntaxCode("ps_2_b");

            mGpuProgramManager->_pushSyntaxCode("ps_2_0");
        case 1:
            if (major > 1 || minor >= 4)
                mGpuProgramManager->_pushSyntaxCode("ps_1_4");
            if (major > 1 || minor >= 3)
                mGpuProgramManager->_pushSyntaxCode("ps_1_3");
            if (major > 1 || minor >= 2)
                mGpuProgramManager->_pushSyntaxCode("ps_1_2");
            
            mGpuProgramManager->_pushSyntaxCode("ps_1_1");
            mCapabilities->setCapability(RSC_FRAGMENT_PROGRAM);
        }
    }
	//-----------------------------------------------------------------------
	MultiRenderTarget * D3D9RenderSystem::createMultiRenderTarget(const String & name)
	{
		MultiRenderTarget *retval;
		retval = new D3D9MultiRenderTarget(name);
		attachRenderTarget(*retval);

		return retval;
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::destroyRenderTarget(const String& name)
	{
		// Check in specialised lists
		if (mPrimaryWindow->getName() == name)
		{
			// We're destroying the primary window, so reset device and window
			mPrimaryWindow = 0;
		}
		else
		{
			// Check secondary windows
			SecondaryWindowList::iterator sw;
			for (sw = mSecondaryWindows.begin(); sw != mSecondaryWindows.end(); ++sw)
			{
				if ((*sw)->getName() == name)
				{
					mSecondaryWindows.erase(sw);
					break;
				}
			}
		}
		// Do the real removal
		RenderSystem::destroyRenderTarget(name);

		// Did we destroy the primary?
		if (!mPrimaryWindow)
		{
			// device is no longer valid, so free it all up
			freeDevice();
		}

	}
	//-----------------------------------------------------------------------
	void D3D9RenderSystem::freeDevice(void)
	{
		if (mpD3DDevice)
		{
			// Set all texture units to nothing to release texture surfaces
			_disableTextureUnitsFrom(0);
			// Unbind any vertex streams to avoid memory leaks
			for (unsigned int i = 0; i < mLastVertexSourceCount; ++i)
			{
				HRESULT hr = mpD3DDevice->SetStreamSource(i, NULL, 0, 0);
			}
			// Clean up depth stencil surfaces
			_cleanupDepthStencils();
			SAFE_RELEASE(mpD3DDevice);
			mActiveD3DDriver->setD3DDevice(NULL);
			mpD3DDevice = 0;

		}


	}
	//---------------------------------------------------------------------
	String D3D9RenderSystem::getErrorDescription( long errorNumber ) const
	{
		const String errMsg = DXGetErrorDescription9( errorNumber );
		return errMsg;
	}
    //---------------------------------------------------------------------
	VertexElementType D3D9RenderSystem::getColourVertexElementType(void) const
	{
		return VET_COLOUR_ARGB;
	}
   	//---------------------------------------------------------------------
	void D3D9RenderSystem::_convertProjectionMatrix(const Matrix4& matrix,
        Matrix4& dest, bool forGpuProgram)
    {
        dest = matrix;

        // Convert depth range from [-1,+1] to [0,1]
        dest[2][0] = (dest[2][0] + dest[3][0]) / 2;
        dest[2][1] = (dest[2][1] + dest[3][1]) / 2;
        dest[2][2] = (dest[2][2] + dest[3][2]) / 2;
        dest[2][3] = (dest[2][3] + dest[3][3]) / 2;

        if (!forGpuProgram)
        {
            // Convert right-handed to left-handed
            dest[0][2] = -dest[0][2];
            dest[1][2] = -dest[1][2];
            dest[2][2] = -dest[2][2];
            dest[3][2] = -dest[3][2];
        }
    }
	//---------------------------------------------------------------------
	void D3D9RenderSystem::_makeProjectionMatrix(const Radian& fovy, Real aspect, Real nearPlane, 
        Real farPlane, Matrix4& dest, bool forGpuProgram)
	{
        Radian theta ( fovy * 0.5 );
		Real h = 1 / Math::Tan(theta);
		Real w = h / aspect;
		Real q, qn;
        if (farPlane == 0)
        {
            q = 1 - Frustum::INFINITE_FAR_PLANE_ADJUST;
            qn = nearPlane * (Frustum::INFINITE_FAR_PLANE_ADJUST - 1);
        }
        else
        {
            q = farPlane / ( farPlane - nearPlane );
            qn = -q * nearPlane;
        }

		dest = Matrix4::ZERO;
		dest[0][0] = w;
		dest[1][1] = h;

        if (forGpuProgram)
        {
            dest[2][2] = -q;
		    dest[3][2] = -1.0f;
        }
        else
        {
            dest[2][2] = q;
            dest[3][2] = 1.0f;
        }

        dest[2][3] = qn;
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::_makeOrthoMatrix(const Radian& fovy, Real aspect, Real nearPlane, Real farPlane, 
		Matrix4& dest, bool forGpuProgram )
	{
        Radian thetaY (fovy / 2.0f);
        Real tanThetaY = Math::Tan(thetaY);

        //Real thetaX = thetaY * aspect;
        Real tanThetaX = tanThetaY * aspect; //Math::Tan(thetaX);
        Real half_w = tanThetaX * nearPlane;
        Real half_h = tanThetaY * nearPlane;
        Real iw = 1.0 / half_w;
        Real ih = 1.0 / half_h;
        Real q;
        if (farPlane == 0)
        {
            q = 0;
        }
        else
        {
            q = 1.0 / (farPlane - nearPlane);
        }

        dest = Matrix4::ZERO;
        dest[0][0] = iw;
        dest[1][1] = ih;
        dest[2][2] = q;
        dest[2][3] = -nearPlane / (farPlane - nearPlane);
        dest[3][3] = 1;

        if (forGpuProgram)
        {
            dest[2][2] = -dest[2][2];
        }
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::setAmbientLight( float r, float g, float b )
	{
		HRESULT hr = __SetRenderState( D3DRS_AMBIENT, D3DCOLOR_COLORVALUE( r, g, b, 1.0f ) );
		if( FAILED( hr ) )
			OGRE_EXCEPT( hr, "Failed to set render stat D3DRS_AMBIENT", "D3D9RenderSystem::setAmbientLight" );
	}
	//---------------------------------------------------------------------
    void D3D9RenderSystem::_useLights(const LightList& lights, unsigned short limit)
    {
        LightList::const_iterator i, iend;
        iend = lights.end();
        unsigned short num = 0;
        for (i = lights.begin(); i != iend && num < limit; ++i, ++num)
        {
            setD3D9Light(num, *i);
        }
        // Disable extra lights
        for (; num < mCurrentLights; ++num)
        {
            setD3D9Light(num, NULL);
        }
        mCurrentLights = std::min(limit, static_cast<unsigned short>(lights.size()));

    }
	//---------------------------------------------------------------------
	void D3D9RenderSystem::setShadingType( ShadeOptions so )
	{
		HRESULT hr = __SetRenderState( D3DRS_SHADEMODE, D3D9Mappings::get(so) );
		if( FAILED( hr ) )
			OGRE_EXCEPT( hr, "Failed to set render stat D3DRS_SHADEMODE", "D3D9RenderSystem::setShadingType" );
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::setLightingEnabled( bool enabled )
	{
		HRESULT hr;
		if( FAILED( hr = __SetRenderState( D3DRS_LIGHTING, enabled ) ) )
			OGRE_EXCEPT( hr, "Failed to set render state D3DRS_LIGHTING", "D3D9RenderSystem::setLightingEnabled" );
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::setD3D9Light( size_t index, Light* lt )
	{
		HRESULT hr;

        if (index < MAX_LIGHTS)
        {
            if (mLights[index] == lt)
                return;
            mLights[index] = lt;
        }

        if (!lt)
        {
            if( FAILED( hr = mpD3DDevice->LightEnable( index, FALSE) ) )
			    OGRE_EXCEPT( hr, "Unable to disable light", "D3D9RenderSystem::setD3D9Light" );
        }
        else
        {
            D3DLIGHT9 d3dLight;

            const Light::LightTypes lightType = lt->getType();

			switch( lightType )
			{
			case Light::LT_POINT:
				d3dLight.Type = D3DLIGHT_POINT;
				break;

			case Light::LT_DIRECTIONAL:
				d3dLight.Type = D3DLIGHT_DIRECTIONAL;
				break;

			case Light::LT_SPOTLIGHT:
				d3dLight.Type = D3DLIGHT_SPOT;
				d3dLight.Falloff = lt->getSpotlightFalloff();
				d3dLight.Theta = lt->getSpotlightInnerAngle().valueRadians();
				d3dLight.Phi = lt->getSpotlightOuterAngle().valueRadians();
				break;

            default:
                assert(false);
                return;
			}

            const ColourValue& diffuse = lt->getDiffuseColour();
            d3dLight.Diffuse.r = diffuse.r;
            d3dLight.Diffuse.g = diffuse.g;
            d3dLight.Diffuse.b = diffuse.b;
            d3dLight.Diffuse.a = diffuse.a;

            const ColourValue& specular = lt->getSpecularColour();
            d3dLight.Specular.r = specular.r;
            d3dLight.Specular.g = specular.g;
            d3dLight.Specular.b = specular.b;
            d3dLight.Specular.a = specular.a;

            d3dLight.Ambient.r = 0;
            d3dLight.Ambient.g = 0;
            d3dLight.Ambient.b = 0;
            d3dLight.Ambient.a = 1;

            if( lightType != Light::LT_DIRECTIONAL )
            {
                const Vector3& position = lt->getDerivedPosition();
                d3dLight.Position.x = position.x;
                d3dLight.Position.y = position.y;
                d3dLight.Position.z = position.z;

                d3dLight.Range = lt->getAttenuationRange();
                d3dLight.Attenuation0 = lt->getAttenuationConstant();
                d3dLight.Attenuation1 = lt->getAttenuationLinear();
                d3dLight.Attenuation2 = lt->getAttenuationQuadric();
            }

            if( lightType != Light::LT_POINT )
            {
                const Vector3& direction = lt->getDerivedDirection();
                d3dLight.Direction.x = direction.x;
                d3dLight.Direction.y = direction.y;
                d3dLight.Direction.z = direction.z;
            }

			if( FAILED( hr = mpD3DDevice->SetLight( index, &d3dLight ) ) )
				OGRE_EXCEPT( hr, "Unable to set light details", "D3D9RenderSystem::setD3D9Light" );

            if( FAILED( hr = mpD3DDevice->LightEnable( index, TRUE ) ) )
			    OGRE_EXCEPT( hr, "Unable to enable light", "D3D9RenderSystem::setD3D9Light" );
        }


	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::_setViewMatrix( const Matrix4 &m )
	{
        // save latest view matrix
        mViewMatrix = m;
        mViewMatrix[2][0] = -mViewMatrix[2][0];
        mViewMatrix[2][1] = -mViewMatrix[2][1];
        mViewMatrix[2][2] = -mViewMatrix[2][2];
        mViewMatrix[2][3] = -mViewMatrix[2][3];

        D3DXMATRIX d3dmat = D3D9Mappings::makeD3DXMatrix( mViewMatrix );

		HRESULT hr;
		if( FAILED( hr = mpD3DDevice->SetTransform( D3DTS_VIEW, &d3dmat ) ) )
			OGRE_EXCEPT( hr, "Cannot set D3D9 view matrix", "D3D9RenderSystem::_setViewMatrix" );
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::_setProjectionMatrix( const Matrix4 &m )
	{
		D3DXMATRIX d3dMat = D3D9Mappings::makeD3DXMatrix( m );

		if( mActiveRenderTarget->requiresTextureFlipping() )
        {
            // Invert transformed y
            d3dMat._12 = - d3dMat._12;
			d3dMat._22 = - d3dMat._22;
            d3dMat._32 = - d3dMat._32;
            d3dMat._42 = - d3dMat._42;
        }

		HRESULT hr;
		if( FAILED( hr = mpD3DDevice->SetTransform( D3DTS_PROJECTION, &d3dMat ) ) )
			OGRE_EXCEPT( hr, "Cannot set D3D9 projection matrix", "D3D9RenderSystem::_setProjectionMatrix" );
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::_setWorldMatrix( const Matrix4 &m )
	{
		D3DXMATRIX d3dMat = D3D9Mappings::makeD3DXMatrix( m );

		HRESULT hr;
		if( FAILED( hr = mpD3DDevice->SetTransform( D3DTS_WORLD, &d3dMat ) ) )
			OGRE_EXCEPT( hr, "Cannot set D3D9 world matrix", "D3D9RenderSystem::_setWorldMatrix" );
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::_setSurfaceParams( const ColourValue &ambient, const ColourValue &diffuse,
		const ColourValue &specular, const ColourValue &emissive, Real shininess,
        TrackVertexColourType tracking )
	{
		
		D3DMATERIAL9 material;
		material.Diffuse = D3DXCOLOR( diffuse.r, diffuse.g, diffuse.b, diffuse.a );
		material.Ambient = D3DXCOLOR( ambient.r, ambient.g, ambient.b, ambient.a );
		material.Specular = D3DXCOLOR( specular.r, specular.g, specular.b, specular.a );
		material.Emissive = D3DXCOLOR( emissive.r, emissive.g, emissive.b, emissive.a );
		material.Power = shininess;

		HRESULT hr = mpD3DDevice->SetMaterial( &material );
		if( FAILED( hr ) )
			OGRE_EXCEPT( hr, "Error setting D3D material", "D3D9RenderSystem::_setSurfaceParams" );


		if(tracking != TVC_NONE) 
        {
            __SetRenderState(D3DRS_COLORVERTEX, TRUE);
            __SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, (tracking&TVC_AMBIENT)?D3DMCS_COLOR1:D3DMCS_MATERIAL);
            __SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, (tracking&TVC_DIFFUSE)?D3DMCS_COLOR1:D3DMCS_MATERIAL);
            __SetRenderState(D3DRS_SPECULARMATERIALSOURCE, (tracking&TVC_SPECULAR)?D3DMCS_COLOR1:D3DMCS_MATERIAL);
            __SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, (tracking&TVC_EMISSIVE)?D3DMCS_COLOR1:D3DMCS_MATERIAL);
        } 
        else 
        {
            __SetRenderState(D3DRS_COLORVERTEX, FALSE);               
        }
        
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::_setPointParameters(Real size, 
		bool attenuationEnabled, Real constant, Real linear, Real quadratic,
		Real minSize, Real maxSize)
    {
		if(attenuationEnabled)
		{
			// scaling required
			__SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);
			__SetFloatRenderState(D3DRS_POINTSCALE_A, constant);
			__SetFloatRenderState(D3DRS_POINTSCALE_B, linear);
			__SetFloatRenderState(D3DRS_POINTSCALE_C, quadratic);
		}
		else
		{
			// no scaling required
			__SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
		}
		__SetFloatRenderState(D3DRS_POINTSIZE, size);
		__SetFloatRenderState(D3DRS_POINTSIZE_MIN, minSize);
		if (maxSize == 0.0f)
			maxSize = mCapabilities->getMaxPointSize();
		__SetFloatRenderState(D3DRS_POINTSIZE_MAX, maxSize);


    }
	//---------------------------------------------------------------------
	void D3D9RenderSystem::_setPointSpritesEnabled(bool enabled)
	{
		if (enabled)
		{
			__SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
		}
		else
		{
			__SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
		}
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::_setTexture( size_t stage, bool enabled, const String &texname )
	{
		HRESULT hr;

		if (enabled && !texname.empty())
		{
            D3D9TexturePtr dt = TextureManager::getSingleton().getByName(texname);
            if (!dt.isNull())
            {
                // note used
                dt->touch();

                IDirect3DBaseTexture9 *pTex = dt->getTexture();
                if (mTexStageDesc[stage].pTex != pTex)
                {
                    hr = mpD3DDevice->SetTexture(stage, pTex);
                    if( hr != S_OK )
                    {
                        String str = "Unable to set texture '" + texname + "' in D3D9";
                        OGRE_EXCEPT( hr, str, "D3D9RenderSystem::_setTexture" );
                    }

                    // set stage desc.
                    mTexStageDesc[stage].pTex = pTex;
                    mTexStageDesc[stage].texType = dt->getTextureType();
                }

                return;
			}
		}

        if (mTexStageDesc[stage].pTex != 0)
        {
            hr = mpD3DDevice->SetTexture(stage, 0);
            if( hr != S_OK )
            {
                String str = "Unable to disable texture '" + texname + "' in D3D9";
                OGRE_EXCEPT( hr, str, "D3D9RenderSystem::_setTexture" );
            }
        }

        hr = this->__SetTextureStageState(stage, D3DTSS_COLOROP, D3DTOP_DISABLE);
        if( hr != S_OK )
        {
            String str = "Unable to disable texture '" + texname + "' in D3D9";
            OGRE_EXCEPT( hr, str, "D3D9RenderSystem::_setTexture" );
        }

        // set stage desc. to defaults
        mTexStageDesc[stage].pTex = 0;
        mTexStageDesc[stage].autoTexCoordType = TEXCALC_NONE;
        mTexStageDesc[stage].coordIndex = 0;
        mTexStageDesc[stage].texType = TEX_TYPE_2D;
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::_setTextureCoordSet( size_t stage, size_t index )
	{
		HRESULT hr;
        // Record settings
        mTexStageDesc[stage].coordIndex = index;

		hr = __SetTextureStageState( stage, D3DTSS_TEXCOORDINDEX, D3D9Mappings::get(mTexStageDesc[stage].autoTexCoordType, mCaps) | index );
		if( FAILED( hr ) )
			OGRE_EXCEPT( hr, "Unable to set texture coord. set index", "D3D8RenderSystem::_setTextureCoordSet" );
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::_setTextureCoordCalculation( size_t stage, TexCoordCalcMethod m,
        const Frustum* frustum)
	{
		HRESULT hr;
		// record the stage state
		mTexStageDesc[stage].autoTexCoordType = m;
        mTexStageDesc[stage].frustum = frustum;

		hr = __SetTextureStageState( stage, D3DTSS_TEXCOORDINDEX, D3D9Mappings::get(m, mCaps) | mTexStageDesc[stage].coordIndex );
		if(FAILED(hr))
			OGRE_EXCEPT( hr, "Unable to set texture auto tex.coord. generation mode", "D3D8RenderSystem::_setTextureCoordCalculation" );
	}
    //---------------------------------------------------------------------
	void D3D9RenderSystem::_setTextureMatrix( size_t stage, const Matrix4& xForm )
	{
		HRESULT hr;
		// Cache texcoord calc method to register
		TexCoordCalcMethod autoTexCoordType = mTexStageDesc[stage].autoTexCoordType;

#if 0
        // Check for most common case first: disbale texcoord transform completely.
        if (autoTexCoordType == TEXCALC_NONE && xForm.isIdentity())
        {
            // disable all of this
            hr = __SetTextureStageState( stage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
            if (FAILED(hr))
                OGRE_EXCEPT( hr, "Unable to disable texture coordinate transform", "D3D9RenderSystem::_setTextureMatrix" );

            // Needless to sets texture transform here, it's never used at all
            return;
        }
#endif

        // The matrix used to combine user supplied transform with texgen dependent transform
        Matrix4 tmpMat;
        // Pointer to the actual used matrix we'll apply after convert to D3D format
        const Matrix4* pMat;

        switch (autoTexCoordType)
        {
        case TEXCALC_ENVIRONMENT_MAP:
            if (mCaps.VertexProcessingCaps & D3DVTXPCAPS_TEXGEN_SPHEREMAP)
            {
                /** Invert the input v texture coordinate for the spheremap */
                tmpMat[0][0] =  xForm[0][0];
                tmpMat[0][1] = -xForm[0][1];
                tmpMat[0][2] =  xForm[0][2];
                tmpMat[0][3] =  xForm[0][3];
                tmpMat[1][0] =  xForm[1][0];
                tmpMat[1][1] = -xForm[1][1];
                tmpMat[1][2] =  xForm[1][2];
                tmpMat[1][3] =  xForm[1][3];
                tmpMat[2][0] =  xForm[2][0];
                tmpMat[2][1] = -xForm[2][1];
                tmpMat[2][2] =  xForm[2][2];
                tmpMat[2][3] =  xForm[2][3];
                tmpMat[3][0] =  xForm[3][0];
                tmpMat[3][1] = -xForm[3][1];
                tmpMat[3][2] =  xForm[3][2];
                tmpMat[3][3] =  xForm[3][3];
            }
            else
            {
		        /* If envmap is applied, but device doesn't support spheremap,
		        then we have to use texture transform to make the camera space normal
		        reference the envmap properly. This isn't exactly the same as spheremap
		        (it looks nasty on flat areas because the camera space normals are the same)
		        but it's the best approximation we have in the absence of a proper spheremap */
			    // concatenate with the xForm
                tmpMat.concatenate(xForm, Matrix4::CLIPSPACE2DTOIMAGESPACE);
            }

            // need this if texture is a cube map, to invert D3D's z coord
            tmpMat[2][0] = -tmpMat[2][0];
            tmpMat[2][1] = -tmpMat[2][1];
            tmpMat[2][2] = -tmpMat[2][2];
            tmpMat[2][3] = -tmpMat[2][3];

            // Use combined matrix
            pMat = &tmpMat;
            break;

        case TEXCALC_ENVIRONMENT_MAP_REFLECTION:
            // If this is a cubic reflection, we need to modify using the view matrix
            // Concatenate with transposed 3x3 view matrix
            // We want to transpose since that will invert an orthonormal matrix ie rotation
            tmpMat.concatenate(xForm, Matrix4(
                mViewMatrix[0][0], mViewMatrix[1][0], mViewMatrix[2][0], 0.0f,
                mViewMatrix[0][1], mViewMatrix[1][1], mViewMatrix[2][1], 0.0f,
                mViewMatrix[0][2], mViewMatrix[1][2], mViewMatrix[2][2], 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f));

            // need this if texture is a cube map, to invert D3D's z coord
            tmpMat[2][0] = -tmpMat[2][0];
            tmpMat[2][1] = -tmpMat[2][1];
            tmpMat[2][2] = -tmpMat[2][2];
            tmpMat[2][3] = -tmpMat[2][3];

            // Use combined matrix
            pMat = &tmpMat;
            break;

        case TEXCALC_ENVIRONMENT_MAP_PLANAR:
        case TEXCALC_ENVIRONMENT_MAP_NORMAL:
            // need this for texture is a cube map, to invert D3D's z coord
            tmpMat[0][0] =  xForm[0][0];
            tmpMat[0][1] =  xForm[0][1];
            tmpMat[0][2] =  xForm[0][2];
            tmpMat[0][3] =  xForm[0][3];
            tmpMat[1][0] =  xForm[1][0];
            tmpMat[1][1] =  xForm[1][1];
            tmpMat[1][2] =  xForm[1][2];
            tmpMat[1][3] =  xForm[1][3];
            tmpMat[2][0] = -xForm[2][0];
            tmpMat[2][1] = -xForm[2][1];
            tmpMat[2][2] = -xForm[2][2];
            tmpMat[2][3] = -xForm[2][3];
            tmpMat[3][0] =  xForm[3][0];
            tmpMat[3][1] =  xForm[3][1];
            tmpMat[3][2] =  xForm[3][2];
            tmpMat[3][3] =  xForm[3][3];

            // Use combined matrix
            pMat = &tmpMat;
            break;

        case TEXCALC_PROJECTIVE_TEXTURE:
            // Derive camera space to projector space transform
            // To do this, we need to undo the camera view matrix, then 
            // apply the projector view & projection matrices
            tmpMat.concatenate(mTexStageDesc[stage].frustum->getViewMatrix(), mViewMatrix.inverse());
            tmpMat.concatenate(mTexStageDesc[stage].frustum->getProjectionMatrix(), tmpMat);
            tmpMat.concatenate(Matrix4::CLIPSPACE2DTOIMAGESPACE, tmpMat);
            tmpMat.concatenate(xForm, tmpMat);

            // Use combined matrix
            pMat = &tmpMat;
            break;

        default: // Make compiler happy
        case TEXCALC_NONE:
            // Use user supplied transform unmodify
            pMat = &xForm;
            break;
        }

		// set the matrix if it's not the identity
        if (!pMat->isIdentity())
		{
            // convert our matrix to D3D format
		    D3DXMATRIX d3dMat = D3D9Mappings::makeD3DXMatrix(*pMat);

            TextureType texType = mTexStageDesc[stage].texType;

            /* It's seems D3D automatically add a texture coordinate with value 1,
            and fill up the remaining texture coordinates with 0 for the input
            texture coordinates before pass to texture coordinate transformation.

               NOTE: It's difference with D3DDECLTYPE enumerated type expand in
            DirectX SDK documentation!

               So we should prepare the texcoord transform, make the transformation
            just like standardized vector expand, thus, fill w with value 1 and
            others with 0.
            */
            if (autoTexCoordType == TEXCALC_NONE)
            {
                /* FIXME: The actually input texture coordinate dimensions should
                be determine by texture coordinate vertex element. Now, just trust
                user supplied texture type matchs texture coordinate vertex element.
                */
                if (texType == TEX_TYPE_2D)
                {
                    /* It's 2D input texture coordinate:

                      texcoord in vertex buffer     D3D expanded to     We are adjusted to
                                                -->                 -->
                                (u, v)               (u, v, 1, 0)          (u, v, 0, 1)
                    */
                    std::swap(d3dMat._31, d3dMat._41);
                    std::swap(d3dMat._32, d3dMat._42);
                    std::swap(d3dMat._33, d3dMat._43);
                    std::swap(d3dMat._34, d3dMat._44);
                }
                else if (texType == TEX_TYPE_1D)
                {
                    /* It's 1D input texture coordinate:

                      texcoord in vertex buffer     D3D expanded to     We are adjusted to
                                                -->                 -->
                                (u)                  (u, 1, 0, 0)          (u, 0, 0, 1)
                    */
                    std::swap(d3dMat._21, d3dMat._41);
                    std::swap(d3dMat._22, d3dMat._42);
                    std::swap(d3dMat._23, d3dMat._43);
                    std::swap(d3dMat._24, d3dMat._44);
                }
                else
                {
                    // Nothing to do with other texture type 
                }
            }
            else if ((autoTexCoordType == TEXCALC_ENVIRONMENT_MAP ||
                      autoTexCoordType == TEXCALC_ENVIRONMENT_MAP_PLANAR) &&
                     (mCaps.VertexProcessingCaps & D3DVTXPCAPS_TEXGEN_SPHEREMAP))
            {
                // Sphere map texgen generate 2D input texture coordinates.
                std::swap(d3dMat._31, d3dMat._41);
                std::swap(d3dMat._32, d3dMat._42);
                std::swap(d3dMat._33, d3dMat._43);
                std::swap(d3dMat._34, d3dMat._44);
            }
            else
            {
                // All other texgen generate 3D input texture coordinates.
            }

			// tell D3D the dimension of tex. coord.
			int texCoordDim = D3DTTFF_COUNT2;
            if (autoTexCoordType == TEXCALC_PROJECTIVE_TEXTURE)
            {
                /* We want texcoords (u, v, w, q) always get divided by q, but D3D
                projected texcoords is divided by the last element (in the case of
                2D texcoord, is w). So we tweak the transform matrix, transform the
                texcoords with w and q swapped: (u, v, q, w), and then D3D will
                divide u, v by q. The w and q just ignored as it wasn't used by
                rasterizer.
                */
			    switch (texType)
			    {
			    case TEX_TYPE_1D:
                    std::swap(d3dMat._12, d3dMat._14);
                    std::swap(d3dMat._22, d3dMat._24);
                    std::swap(d3dMat._32, d3dMat._34);
                    std::swap(d3dMat._42, d3dMat._44);

                    texCoordDim = D3DTTFF_PROJECTED | D3DTTFF_COUNT2;
                    break;

			    case TEX_TYPE_2D:
                    std::swap(d3dMat._13, d3dMat._14);
                    std::swap(d3dMat._23, d3dMat._24);
                    std::swap(d3dMat._33, d3dMat._34);
                    std::swap(d3dMat._43, d3dMat._44);

                    texCoordDim = D3DTTFF_PROJECTED | D3DTTFF_COUNT3;
                    break;

			    case TEX_TYPE_CUBE_MAP:
			    case TEX_TYPE_3D:
                    // Yes, we support 3D projective texture.
				    texCoordDim = D3DTTFF_PROJECTED | D3DTTFF_COUNT4;
                    break;
                }
            }
            else
            {
			    switch (texType)
			    {
                case TEX_TYPE_1D:
				    texCoordDim = D3DTTFF_COUNT1;
				    break;
			    case TEX_TYPE_2D:
				    texCoordDim = D3DTTFF_COUNT2;
				    break;
			    case TEX_TYPE_CUBE_MAP:
			    case TEX_TYPE_3D:
				    texCoordDim = D3DTTFF_COUNT3;
                    break;
			    }
            }

			hr = __SetTextureStageState( stage, D3DTSS_TEXTURETRANSFORMFLAGS, texCoordDim );
			if (FAILED(hr))
				OGRE_EXCEPT( hr, "Unable to set texture coord. dimension", "D3D9RenderSystem::_setTextureMatrix" );

			hr = mpD3DDevice->SetTransform( (D3DTRANSFORMSTATETYPE)(D3DTS_TEXTURE0 + stage), &d3dMat );
			if (FAILED(hr))
				OGRE_EXCEPT( hr, "Unable to set texture matrix", "D3D9RenderSystem::_setTextureMatrix" );
		}
		else
		{
			// disable all of this
			hr = __SetTextureStageState( stage, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE );
			if( FAILED( hr ) )
				OGRE_EXCEPT( hr, "Unable to disable texture coordinate transform", "D3D9RenderSystem::_setTextureMatrix" );

			// Needless to sets texture transform here, it's never used at all
		}
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::_setTextureAddressingMode( size_t stage, 
		const TextureUnitState::UVWAddressingMode& uvw )
	{
		HRESULT hr;
		if( FAILED( hr = __SetSamplerState( stage, D3DSAMP_ADDRESSU, D3D9Mappings::get(uvw.u) ) ) )
			OGRE_EXCEPT( hr, "Failed to set texture addressing mode for U", "D3D9RenderSystem::_setTextureAddressingMode" );
		if( FAILED( hr = __SetSamplerState( stage, D3DSAMP_ADDRESSV, D3D9Mappings::get(uvw.v) ) ) )
			OGRE_EXCEPT( hr, "Failed to set texture addressing mode for V", "D3D9RenderSystem::_setTextureAddressingMode" );
		if( FAILED( hr = __SetSamplerState( stage, D3DSAMP_ADDRESSW, D3D9Mappings::get(uvw.w) ) ) )
			OGRE_EXCEPT( hr, "Failed to set texture addressing mode for W", "D3D9RenderSystem::_setTextureAddressingMode" );
	}
    //-----------------------------------------------------------------------------
    void D3D9RenderSystem::_setTextureBorderColour(size_t stage,
        const ColourValue& colour)
    {
		HRESULT hr;
		if( FAILED( hr = __SetSamplerState( stage, D3DSAMP_BORDERCOLOR, colour.getAsARGB()) ) )
			OGRE_EXCEPT( hr, "Failed to set texture border colour", "D3D9RenderSystem::_setTextureBorderColour" );
    }
	//---------------------------------------------------------------------
    void D3D9RenderSystem::_setTextureBlendMode(size_t stage,
        const LayerBlendModeEx& colourBlendMode, const LayerBlendModeEx& alphaBlendMode)
    {
        assert(colourBlendMode.blendType == LBT_COLOUR);
        assert(alphaBlendMode.blendType == LBT_ALPHA);

        HRESULT hr;

        D3DXCOLOR manual;
        bool useManual = false;

        // Gather manual alpha
        if (colourBlendMode.operation == LBX_BLEND_MANUAL)
        {
            manual.a = colourBlendMode.factor;
            useManual = true;
        }
        else if (alphaBlendMode.operation == LBX_BLEND_MANUAL)
        {
            manual.a = alphaBlendMode.factor;
            useManual = true;
        }
        else if (alphaBlendMode.source1 == LBS_MANUAL)
        {
            manual.a = alphaBlendMode.alphaArg1;
            useManual = true;
        }
        else if (alphaBlendMode.source2 == LBS_MANUAL)
        {
            manual.a = alphaBlendMode.alphaArg2;
            useManual = true;
        }
        else
        {
            manual.a = 1.0f;
        }

        // Gather manual colour
        if (colourBlendMode.source1 == LBS_MANUAL)
        {
            manual.r = colourBlendMode.colourArg1.r;
            manual.g = colourBlendMode.colourArg1.g;
            manual.b = colourBlendMode.colourArg1.b;
            useManual = true;
        }
        else if (colourBlendMode.source2 == LBS_MANUAL)
        {
            manual.r = colourBlendMode.colourArg2.r;
            manual.g = colourBlendMode.colourArg2.g;
            manual.b = colourBlendMode.colourArg2.b;
            useManual = true;
        }
        else
        {
            manual.r = 1.0f;
            manual.g = 1.0f;
            manual.b = 1.0f;
        }

		// Set manual factor if required
        if (useManual)
        {
            if (mPerStageConstantSupport)
            {
                // Per-stage state
                hr = __SetTextureStageState(stage, D3DTSS_CONSTANT, manual);
            }
            else
            {
                // Global state
			    hr = __SetRenderState(D3DRS_TEXTUREFACTOR, manual);
            }
			if (FAILED(hr))
				OGRE_EXCEPT(hr, "Failed to set manual factor", "D3D9RenderSystem::_setTextureBlendMode");
        }

		// set colour operation
		hr = __SetTextureStageState(stage, D3DTSS_COLOROP, D3D9Mappings::get(colourBlendMode.operation, mCaps));
		if (FAILED(hr))
			OGRE_EXCEPT(hr, "Failed to set colour operation", "D3D9RenderSystem::_setTextureBlendMode");
		// set colour source 1
		hr = __SetTextureStageState(stage, D3DTSS_COLORARG1, D3D9Mappings::get(colourBlendMode.source1, mPerStageConstantSupport));
		if (FAILED(hr))
			OGRE_EXCEPT(hr, "Failed to set colour source1", "D3D9RenderSystem::_setTextureBlendMode");
		// set colour source 2
		hr = __SetTextureStageState(stage, D3DTSS_COLORARG2, D3D9Mappings::get(colourBlendMode.source2, mPerStageConstantSupport));
		if (FAILED(hr))
			OGRE_EXCEPT(hr, "Failed to set colour source2", "D3D9RenderSystem::_setTextureBlendMode");

		// set alpha operation
		hr = __SetTextureStageState(stage, D3DTSS_ALPHAOP, D3D9Mappings::get(alphaBlendMode.operation, mCaps));
		if (FAILED(hr))
			OGRE_EXCEPT(hr, "Failed to set alpha operation", "D3D9RenderSystem::_setTextureBlendMode");
		// set alpha source 1
		hr = __SetTextureStageState(stage, D3DTSS_ALPHAARG1, D3D9Mappings::get(alphaBlendMode.source1, mPerStageConstantSupport));
		if (FAILED(hr))
			OGRE_EXCEPT(hr, "Failed to set alpha source1", "D3D9RenderSystem::_setTextureBlendMode");
		// set alpha source 2
		hr = __SetTextureStageState(stage, D3DTSS_ALPHAARG2, D3D9Mappings::get(alphaBlendMode.source2, mPerStageConstantSupport));
		if (FAILED(hr))
			OGRE_EXCEPT(hr, "Failed to set alpha source2", "D3D9RenderSystem::_setTextureBlendMode");

		// Set interpolation factor if lerping
		if (mCaps.TextureOpCaps & D3DTEXOPCAPS_LERP)
        {
            if (colourBlendMode.operation == LBX_BLEND_DIFFUSE_COLOUR)
            {
			    // choose colour source 0 (lerp factor)
			    hr = __SetTextureStageState(stage, D3DTSS_COLORARG0, D3DTA_DIFFUSE);
			    if (FAILED(hr))
				    OGRE_EXCEPT(hr, "Failed to set lerp colour source 0",
					    "D3D9RenderSystem::_setTextureBlendMode");
            }

            if (alphaBlendMode.operation == LBX_BLEND_DIFFUSE_COLOUR)
            {
			    // choose alpha source 0 (lerp factor)
			    hr = __SetTextureStageState(stage, D3DTSS_ALPHAARG0, D3DTA_DIFFUSE);
			    if (FAILED(hr))
				    OGRE_EXCEPT(hr, "Failed to set lerp alpha source 0",
					    "D3D9RenderSystem::_setTextureBlendMode");
            }
        }
    }
    //---------------------------------------------------------------------
	void D3D9RenderSystem::_setTextureBlendMode( size_t stage, const LayerBlendModeEx& bm )
	{
		HRESULT hr = S_OK;
		D3DTEXTURESTAGESTATETYPE tss;
		D3DCOLOR manualD3D;

		// choose type of blend.
		if( bm.blendType == LBT_COLOUR )
			tss = D3DTSS_COLOROP;
		else if( bm.blendType == LBT_ALPHA )
			tss = D3DTSS_ALPHAOP;
		else
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
			"Invalid blend type", "D3D9RenderSystem::_setTextureBlendMode");

		// set manual factor if required by operation
		if (bm.operation == LBX_BLEND_MANUAL)
		{
			hr = __SetRenderState( D3DRS_TEXTUREFACTOR, D3DXCOLOR(0.0, 0.0, 0.0,  bm.factor) );
			if (FAILED(hr))
				OGRE_EXCEPT( hr, "Failed to set manual factor", "D3D9RenderSystem::_setTextureBlendMode" );
		}
		// set operation
		hr = __SetTextureStageState( stage, tss, D3D9Mappings::get(bm.operation, mCaps) );
		if (FAILED(hr))
			OGRE_EXCEPT( hr, "Failed to set operation", "D3D9RenderSystem::_setTextureBlendMode" );

		// choose source 1
		if( bm.blendType == LBT_COLOUR )
		{
			tss = D3DTSS_COLORARG1;
			manualD3D = D3DXCOLOR( bm.colourArg1.r, bm.colourArg1.g, bm.colourArg1.b, bm.colourArg1.a );
			mManualBlendColours[stage][0] = bm.colourArg1;
		}
		else if( bm.blendType == LBT_ALPHA )
		{
			tss = D3DTSS_ALPHAARG1;
			manualD3D = D3DXCOLOR( mManualBlendColours[stage][0].r, 
				mManualBlendColours[stage][0].g, 
				mManualBlendColours[stage][0].b, bm.alphaArg1 );
		}
		else
		{
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, 
				"Invalid blend type", "D3D9RenderSystem::_setTextureBlendMode");
		}
		// Set manual factor if required
		if (bm.source1 == LBS_MANUAL)
		{
			if (mPerStageConstantSupport)
			{
				// Per-stage state
				hr = __SetTextureStageState(stage, D3DTSS_CONSTANT, manualD3D);
			}
			else
			{
				// Global state
				hr = __SetRenderState( D3DRS_TEXTUREFACTOR, manualD3D );
			}
			if (FAILED(hr))
				OGRE_EXCEPT( hr, "Failed to set manual factor", "D3D9RenderSystem::_setTextureBlendMode" );
		}
		// set source 1
		hr = __SetTextureStageState( stage, tss, D3D9Mappings::get(bm.source1, mPerStageConstantSupport) );
		if (FAILED(hr))
			OGRE_EXCEPT( hr, "Failed to set source1", "D3D9RenderSystem::_setTextureBlendMode" );
		
		// choose source 2
		if( bm.blendType == LBT_COLOUR )
		{
			tss = D3DTSS_COLORARG2;
			manualD3D = D3DXCOLOR( bm.colourArg2.r, bm.colourArg2.g, bm.colourArg2.b, bm.colourArg2.a );
			mManualBlendColours[stage][1] = bm.colourArg1;
		}
		else if( bm.blendType == LBT_ALPHA )
		{
			tss = D3DTSS_ALPHAARG2;
			manualD3D = D3DXCOLOR( mManualBlendColours[stage][1].r, 
				mManualBlendColours[stage][1].g, 
				mManualBlendColours[stage][1].b, 
				bm.alphaArg2 );
		}
		// Set manual factor if required
		if (bm.source2 == LBS_MANUAL)
		{
			if (mPerStageConstantSupport)
			{
				// Per-stage state
				hr = __SetTextureStageState(stage, D3DTSS_CONSTANT, manualD3D);
			}
			else
			{
				hr = __SetRenderState( D3DRS_TEXTUREFACTOR, manualD3D );
			}
			if (FAILED(hr))
				OGRE_EXCEPT( hr, "Failed to set manual factor", "D3D9RenderSystem::_setTextureBlendMode" );
		}
		// Now set source 2
		hr = __SetTextureStageState( stage, tss, D3D9Mappings::get(bm.source2, mPerStageConstantSupport) );
		if (FAILED(hr))
			OGRE_EXCEPT( hr, "Failed to set source 2", "D3D9RenderSystem::_setTextureBlendMode" );

		// Set interpolation factor if lerping
		if (bm.operation == LBX_BLEND_DIFFUSE_COLOUR && 
			mCaps.TextureOpCaps & D3DTEXOPCAPS_LERP)
		{
			// choose source 0 (lerp factor)
			if( bm.blendType == LBT_COLOUR )
			{
				tss = D3DTSS_COLORARG0;
			}
			else if( bm.blendType == LBT_ALPHA )
			{
				tss = D3DTSS_ALPHAARG0;
			}
			hr = __SetTextureStageState(stage, tss, D3DTA_DIFFUSE);

			if (FAILED(hr))
				OGRE_EXCEPT( hr, "Failed to set lerp source 0", 
					"D3D9RenderSystem::_setTextureBlendMode" );

		}
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::_setSceneBlending( SceneBlendFactor sourceFactor, SceneBlendFactor destFactor )
	{
		HRESULT hr;
		if( sourceFactor == SBF_ONE && destFactor == SBF_ZERO)
		{
			if (FAILED(hr = __SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE)))
				OGRE_EXCEPT( hr, "Failed to set alpha blending option", "D3D9RenderSystem::_setSceneBlending" );
		}
		else
		{
			if (FAILED(hr = __SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE)))
				OGRE_EXCEPT( hr, "Failed to set alpha blending option", "D3D9RenderSystem::_setSceneBlending" );
			if( FAILED( hr = __SetRenderState( D3DRS_SRCBLEND, D3D9Mappings::get(sourceFactor) ) ) )
				OGRE_EXCEPT( hr, "Failed to set source blend", "D3D9RenderSystem::_setSceneBlending" );
			if( FAILED( hr = __SetRenderState( D3DRS_DESTBLEND, D3D9Mappings::get(destFactor) ) ) )
				OGRE_EXCEPT( hr, "Failed to set destination blend", "D3D9RenderSystem::_setSceneBlending" );
		}
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::_setAlphaRejectSettings( CompareFunction func, unsigned char value )
	{
		HRESULT hr;
        if (func != CMPF_ALWAYS_PASS)
        {
            if( FAILED( hr = __SetRenderState( D3DRS_ALPHATESTENABLE,  TRUE ) ) )
    			OGRE_EXCEPT( hr, "Failed to enable alpha testing", 
                "D3D9RenderSystem::_setAlphaRejectSettings" );
        }
        else
        {
            if( FAILED( hr = __SetRenderState( D3DRS_ALPHATESTENABLE,  FALSE ) ) )
    			OGRE_EXCEPT( hr, "Failed to disable alpha testing", 
                "D3D9RenderSystem::_setAlphaRejectSettings" );
        }
        // Set always just be sure
		if( FAILED( hr = __SetRenderState( D3DRS_ALPHAFUNC, D3D9Mappings::get(func) ) ) )
			OGRE_EXCEPT( hr, "Failed to set alpha reject function", "D3D9RenderSystem::_setAlphaRejectSettings" );
		if( FAILED( hr = __SetRenderState( D3DRS_ALPHAREF, value ) ) )
			OGRE_EXCEPT( hr, "Failed to set render state D3DRS_ALPHAREF", "D3D9RenderSystem::_setAlphaRejectSettings" );
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::_setCullingMode( CullingMode mode )
	{
		HRESULT hr;
        bool flip = ((mActiveRenderTarget->requiresTextureFlipping() && !mInvertVertexWinding) ||
                (!mActiveRenderTarget->requiresTextureFlipping() && mInvertVertexWinding));

		if( FAILED (hr = __SetRenderState(D3DRS_CULLMODE, 
            D3D9Mappings::get(mode, flip))) )
			OGRE_EXCEPT( hr, "Failed to set culling mode", "D3D9RenderSystem::_setCullingMode" );
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::_setDepthBufferParams( bool depthTest, bool depthWrite, CompareFunction depthFunction )
	{
		_setDepthBufferCheckEnabled( depthTest );
		_setDepthBufferWriteEnabled( depthWrite );
		_setDepthBufferFunction( depthFunction );
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::_setDepthBufferCheckEnabled( bool enabled )
	{
		HRESULT hr;

		if( enabled )
		{
			// Use w-buffer if available and enabled
			if( mWBuffer && mCaps.RasterCaps & D3DPRASTERCAPS_WBUFFER )
				hr = __SetRenderState( D3DRS_ZENABLE, D3DZB_USEW );
			else
				hr = __SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
		}
		else
			hr = __SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );

		if( FAILED( hr ) )
			OGRE_EXCEPT( hr, "Error setting depth buffer test state", "D3D9RenderSystem::_setDepthBufferCheckEnabled" );
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::_setDepthBufferWriteEnabled( bool enabled )
	{
		HRESULT hr;

		if( FAILED( hr = __SetRenderState( D3DRS_ZWRITEENABLE, enabled ) ) )
			OGRE_EXCEPT( hr, "Error setting depth buffer write state", "D3D9RenderSystem::_setDepthBufferWriteEnabled" );
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::_setDepthBufferFunction( CompareFunction func )
	{
		HRESULT hr;
		if( FAILED( hr = __SetRenderState( D3DRS_ZFUNC, D3D9Mappings::get(func) ) ) )
			OGRE_EXCEPT( hr, "Error setting depth buffer test function", "D3D9RenderSystem::_setDepthBufferFunction" );
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::_setDepthBias(short bias)
	{
		float bias_float = static_cast<float>(-bias);
		// scale down - certainly needed for nVidia
        // use 65536 according to 16 bits depth buffer
		bias_float /= 65536.0f;
		HRESULT hr = __SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&bias_float);
		if (FAILED(hr))
			OGRE_EXCEPT(hr, "Error setting depth bias", "D3D9RenderSystem::_setDepthBias");
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::_setColourBufferWriteEnabled(bool red, bool green, 
		bool blue, bool alpha)
	{
		DWORD val = 0;
		if (red) 
			val |= D3DCOLORWRITEENABLE_RED;
		if (green)
			val |= D3DCOLORWRITEENABLE_GREEN;
		if (blue)
			val |= D3DCOLORWRITEENABLE_BLUE;
		if (alpha)
			val |= D3DCOLORWRITEENABLE_ALPHA;
		HRESULT hr = __SetRenderState(D3DRS_COLORWRITEENABLE, val); 
		if (FAILED(hr))
			OGRE_EXCEPT(hr, "Error setting colour write enable flags", 
			"D3D9RenderSystem::_setColourBufferWriteEnabled");
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::_setFog( FogMode mode, const ColourValue& colour, Real densitiy, Real start, Real end )
	{
		HRESULT hr;

		D3DRENDERSTATETYPE fogType, fogTypeNot;

		if (mCaps.RasterCaps & D3DPRASTERCAPS_FOGTABLE)
		{
			fogType = D3DRS_FOGTABLEMODE;
			fogTypeNot = D3DRS_FOGVERTEXMODE;
		}
		else
		{
			fogType = D3DRS_FOGVERTEXMODE;
			fogTypeNot = D3DRS_FOGTABLEMODE;
		}

		if( mode == FOG_NONE)
		{
			// just disable
			hr = __SetRenderState(fogType, D3DFOG_NONE );
			hr = __SetRenderState(D3DRS_FOGENABLE, FALSE);
		}
		else
		{
			// Allow fog
			hr = __SetRenderState( D3DRS_FOGENABLE, TRUE );
			hr = __SetRenderState( fogTypeNot, D3DFOG_NONE );
			hr = __SetRenderState( fogType, D3D9Mappings::get(mode) );

			hr = __SetRenderState( D3DRS_FOGCOLOR, colour.getAsARGB() );
			hr = __SetFloatRenderState( D3DRS_FOGSTART, start );
			hr = __SetFloatRenderState( D3DRS_FOGEND, end );
			hr = __SetFloatRenderState( D3DRS_FOGDENSITY, densitiy );
		}

		if( FAILED( hr ) )
			OGRE_EXCEPT( hr, "Error setting render state", "D3D9RenderSystem::_setFog" );
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::_setPolygonMode(PolygonMode level)
	{
		HRESULT hr = __SetRenderState(D3DRS_FILLMODE, D3D9Mappings::get(level));
		if (FAILED(hr))
			OGRE_EXCEPT(hr, "Error setting polygon mode.", "D3D9RenderSystem::setPolygonMode");
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::setStencilCheckEnabled(bool enabled)
	{
		// Allow stencilling
		HRESULT hr = __SetRenderState(D3DRS_STENCILENABLE, enabled);
		if (FAILED(hr))
			OGRE_EXCEPT(hr, "Error enabling / disabling stencilling.",
			"D3D9RenderSystem::setStencilCheckEnabled");
	}
    //---------------------------------------------------------------------
    void D3D9RenderSystem::setStencilBufferParams(CompareFunction func, 
        uint32 refValue, uint32 mask, StencilOperation stencilFailOp, 
        StencilOperation depthFailOp, StencilOperation passOp, 
        bool twoSidedOperation)
    {
        HRESULT hr;
        bool flip;

        // 2-sided operation
        if (twoSidedOperation)
        {
            if (!mCapabilities->hasCapability(RSC_TWO_SIDED_STENCIL))
                OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "2-sided stencils are not supported",
                    "D3D9RenderSystem::setStencilBufferParams");
            hr = __SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, TRUE);
		    if (FAILED(hr))
			    OGRE_EXCEPT(hr, "Error setting 2-sided stencil mode.",
			    "D3D9RenderSystem::setStencilBufferParams");
            // NB: We should always treat CCW as front face for consistent with default
            // culling mode. Therefore, we must take care with two-sided stencil settings.
            flip = (mInvertVertexWinding && mActiveRenderTarget->requiresTextureFlipping()) ||
                   (!mInvertVertexWinding && !mActiveRenderTarget->requiresTextureFlipping());

            // Set alternative versions of ops
            // fail op
            hr = __SetRenderState(D3DRS_CCW_STENCILFAIL, D3D9Mappings::get(stencilFailOp, !flip));
            if (FAILED(hr))
                OGRE_EXCEPT(hr, "Error setting stencil fail operation (2-sided).",
                "D3D9RenderSystem::setStencilBufferParams");

            // depth fail op
            hr = __SetRenderState(D3DRS_CCW_STENCILZFAIL, D3D9Mappings::get(depthFailOp, !flip));
            if (FAILED(hr))
                OGRE_EXCEPT(hr, "Error setting stencil depth fail operation (2-sided).",
                "D3D9RenderSystem::setStencilBufferParams");

            // pass op
            hr = __SetRenderState(D3DRS_CCW_STENCILPASS, D3D9Mappings::get(passOp, !flip));
            if (FAILED(hr))
                OGRE_EXCEPT(hr, "Error setting stencil pass operation (2-sided).",
                "D3D9RenderSystem::setStencilBufferParams");
        }
        else
        {
            hr = __SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, FALSE);
		    if (FAILED(hr))
			    OGRE_EXCEPT(hr, "Error setting 1-sided stencil mode.",
			    "D3D9RenderSystem::setStencilBufferParams");
            flip = false;
        }

        // func
        hr = __SetRenderState(D3DRS_STENCILFUNC, D3D9Mappings::get(func));
		if (FAILED(hr))
			OGRE_EXCEPT(hr, "Error setting stencil buffer test function.",
			"D3D9RenderSystem::setStencilBufferParams");

        // reference value
        hr = __SetRenderState(D3DRS_STENCILREF, refValue);
		if (FAILED(hr))
			OGRE_EXCEPT(hr, "Error setting stencil buffer reference value.",
			"D3D9RenderSystem::setStencilBufferParams");

        // mask
        hr = __SetRenderState(D3DRS_STENCILMASK, mask);
		if (FAILED(hr))
			OGRE_EXCEPT(hr, "Error setting stencil buffer mask.",
			"D3D9RenderSystem::setStencilBufferParams");

		// fail op
        hr = __SetRenderState(D3DRS_STENCILFAIL, D3D9Mappings::get(stencilFailOp, flip));
		if (FAILED(hr))
			OGRE_EXCEPT(hr, "Error setting stencil fail operation.",
			"D3D9RenderSystem::setStencilBufferParams");

        // depth fail op
        hr = __SetRenderState(D3DRS_STENCILZFAIL, D3D9Mappings::get(depthFailOp, flip));
		if (FAILED(hr))
			OGRE_EXCEPT(hr, "Error setting stencil depth fail operation.",
			"D3D9RenderSystem::setStencilBufferParams");

        // pass op
        hr = __SetRenderState(D3DRS_STENCILPASS, D3D9Mappings::get(passOp, flip));
		if (FAILED(hr))
			OGRE_EXCEPT(hr, "Error setting stencil pass operation.",
			"D3D9RenderSystem::setStencilBufferParams");
	}
	//---------------------------------------------------------------------
    void D3D9RenderSystem::_setTextureUnitFiltering(size_t unit, FilterType ftype, 
        FilterOptions filter)
	{
		HRESULT hr;
		D3D9Mappings::eD3DTexType texType = D3D9Mappings::get(mTexStageDesc[unit].texType);
        hr = __SetSamplerState( unit, D3D9Mappings::get(ftype), 
            D3D9Mappings::get(ftype, filter, mCaps, texType));
		if (FAILED(hr))
			OGRE_EXCEPT(hr, "Failed to set texture filter ", "D3D9RenderSystem::_setTextureUnitFiltering");
	}
    //---------------------------------------------------------------------
	void D3D9RenderSystem::_setTextureLayerAnisotropy(size_t unit, unsigned int maxAnisotropy)
	{
		if ((DWORD)maxAnisotropy > mCaps.MaxAnisotropy)
			maxAnisotropy = mCaps.MaxAnisotropy;

		__SetSamplerState( unit, D3DSAMP_MAXANISOTROPY, maxAnisotropy );
	}
	//---------------------------------------------------------------------
	HRESULT D3D9RenderSystem::__SetRenderState(D3DRENDERSTATETYPE state, DWORD value)
	{
		HRESULT hr;
		DWORD oldVal;

		if ( FAILED( hr = mpD3DDevice->GetRenderState(state, &oldVal) ) )
			return hr;
		if ( oldVal == value )
			return D3D_OK;
		else
			return mpD3DDevice->SetRenderState(state, value);
	}
	//---------------------------------------------------------------------
	HRESULT D3D9RenderSystem::__SetSamplerState(DWORD sampler, D3DSAMPLERSTATETYPE type, DWORD value)
	{
		HRESULT hr;
		DWORD oldVal;

		if ( FAILED( hr = mpD3DDevice->GetSamplerState(sampler, type, &oldVal) ) )
			return hr;
		if ( oldVal == value )
			return D3D_OK;
		else
			return mpD3DDevice->SetSamplerState(sampler, type, value);
	}
	//---------------------------------------------------------------------
	HRESULT D3D9RenderSystem::__SetTextureStageState(DWORD stage, D3DTEXTURESTAGESTATETYPE type, DWORD value)
	{
		HRESULT hr;
		DWORD oldVal;
		
		if ( FAILED( hr = mpD3DDevice->GetTextureStageState(stage, type, &oldVal) ) )
			return hr;
		if ( oldVal == value )
			return D3D_OK;
		else
			return mpD3DDevice->SetTextureStageState(stage, type, value);
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::_setViewport( Viewport *vp )
	{
		if( vp != mActiveViewport || vp->_isUpdated() )
		{
			mActiveViewport = vp;
			mActiveRenderTarget = vp->getTarget();

			// ok, it's different, time to set render target and viewport params
			D3DVIEWPORT9 d3dvp;
			HRESULT hr;

			// Set render target
			RenderTarget* target;
			target = vp->getTarget();

			// Retrieve render surfaces (up to OGRE_MAX_MULTIPLE_RENDER_TARGETS)
			LPDIRECT3DSURFACE9 pBack[OGRE_MAX_MULTIPLE_RENDER_TARGETS];
			memset(pBack, 0, sizeof(pBack));
			target->getCustomAttribute( "DDBACKBUFFER", &pBack );
			if (!pBack[0])
				return;

			LPDIRECT3DSURFACE9 pDepth = NULL;
			target->getCustomAttribute( "D3DZBUFFER", &pDepth );
			if (!pDepth)
			{
				/// No depth buffer provided, use our own
				/// Request a depth stencil that is compatible with the format, multisample type and
				/// dimensions of the render target.
				D3DSURFACE_DESC srfDesc;
				if(FAILED(pBack[0]->GetDesc(&srfDesc)))
					return; // ?
				pDepth = _getDepthStencilFor(srfDesc.Format, srfDesc.MultiSampleType, srfDesc.Width, srfDesc.Height);
			}
			// Bind render targets
			uint count = mCapabilities->numMultiRenderTargets();
			for(uint x=0; x<count; ++x)
			{
				hr = mpD3DDevice->SetRenderTarget(x, pBack[x]);
				if (FAILED(hr))
				{
					String msg = DXGetErrorDescription9(hr);
					OGRE_EXCEPT( hr, "Failed to setRenderTarget : " + msg, "D3D9RenderSystem::_setViewport" );
				}
			}
			hr = mpD3DDevice->SetDepthStencilSurface(pDepth);
			if (FAILED(hr))
			{
				String msg = DXGetErrorDescription9(hr);
				OGRE_EXCEPT( hr, "Failed to setDepthStencil : " + msg, "D3D9RenderSystem::_setViewport" );
			}

			_setCullingMode( mCullingMode );

			// set viewport dimensions
			d3dvp.X = vp->getActualLeft();
			d3dvp.Y = vp->getActualTop();
			d3dvp.Width = vp->getActualWidth();
			d3dvp.Height = vp->getActualHeight();
            if (target->requiresTextureFlipping())
            {
                // Convert "top-left" to "bottom-left"
                d3dvp.Y = target->getHeight() - d3dvp.Height - d3dvp.Y;
            }

			// Z-values from 0.0 to 1.0 (TODO: standardise with OpenGL)
			d3dvp.MinZ = 0.0f;
			d3dvp.MaxZ = 1.0f;

			if( FAILED( hr = mpD3DDevice->SetViewport( &d3dvp ) ) )
				OGRE_EXCEPT( hr, "Failed to set viewport.", "D3D9RenderSystem::_setViewport" );

			vp->_clearUpdatedFlag();
		}
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::_beginFrame()
	{
		OgreGuard( "D3D9RenderSystem::_beginFrame" );

		HRESULT hr;

		if( !mActiveViewport )
			OGRE_EXCEPT( Exception::ERR_INTERNAL_ERROR, "Cannot begin frame - no viewport selected.", "D3D9RenderSystem::_beginFrame" );

		// Clear the viewport if required
		if( mActiveViewport->getClearEveryFrame() )
		{
            clearFrameBuffer(mActiveViewport->getClearBuffers(), 
                mActiveViewport->getBackgroundColour());
		}

		if( FAILED( hr = mpD3DDevice->BeginScene() ) )
		{
			String msg = DXGetErrorDescription9(hr);
			OGRE_EXCEPT( hr, "Error beginning frame :" + msg, "D3D9RenderSystem::_beginFrame" );
		}

		if(!mBasicStatesInitialised)
		{
			// First-time 
			// setup some defaults
			// Allow specular
			hr = __SetRenderState(D3DRS_SPECULARENABLE, TRUE);
			if (FAILED(hr))
			{
				String msg = DXGetErrorDescription9(hr);
				OGRE_EXCEPT(hr, "Error enabling alpha blending option : " + msg, "D3D9RenderSystem::_beginFrame");
			}
			mBasicStatesInitialised = true;
		}

        std::fill_n(mLights, MAX_LIGHTS, (Light*)0xdeadbeef);

		OgreUnguard();
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::_endFrame()
	{
		OgreGuard( "D3D9RenderSystem::_endFrame" );

		HRESULT hr;
		if( FAILED( hr = mpD3DDevice->EndScene() ) )
			OGRE_EXCEPT( hr, "Error ending frame", "D3D9RenderSystem::_endFrame" );

		OgreUnguard();
	}
    //---------------------------------------------------------------------
	void D3D9RenderSystem::setVertexDeclaration(VertexDeclaration* decl)
	{
		// Guard
		OgreGuard ("D3D9RenderSystem::setVertexDeclaration");
        HRESULT hr;

        D3D9VertexDeclaration* d3ddecl = 
            static_cast<D3D9VertexDeclaration*>(decl);

        if (FAILED(hr = mpD3DDevice->SetVertexDeclaration(d3ddecl->getD3DVertexDeclaration())))
        {
            OGRE_EXCEPT(hr, "Unable to set D3D9 vertex declaration", 
                "D3D9RenderSystem::setVertexDeclaration");
        }

        // UnGuard
		OgreUnguard();
	}
    //---------------------------------------------------------------------
	void D3D9RenderSystem::setVertexBufferBinding(VertexBufferBinding* binding)
	{
		// Guard
		OgreGuard ("D3D9RenderSystem::setVertexBufferBinding");

        HRESULT hr;

        // TODO: attempt to detect duplicates
        const VertexBufferBinding::VertexBufferBindingMap& binds = binding->getBindings();
        VertexBufferBinding::VertexBufferBindingMap::const_iterator i, iend;
        size_t source = 0;
        iend = binds.end();
        for (i = binds.begin(); i != iend; ++i, ++source)
        {
            // Unbind gap sources
            for ( ; source < i->first; ++source)
            {
                hr = mpD3DDevice->SetStreamSource(static_cast<UINT>(source), NULL, 0, 0);
                if (FAILED(hr))
                {
                    OGRE_EXCEPT(hr, "Unable to reset unused D3D9 stream source", 
                        "D3D9RenderSystem::setVertexBufferBinding");
                }
            }

            const D3D9HardwareVertexBuffer* d3d9buf = 
                static_cast<const D3D9HardwareVertexBuffer*>(i->second.get());
            hr = mpD3DDevice->SetStreamSource(
                static_cast<UINT>(source),
                d3d9buf->getD3D9VertexBuffer(),
                0, // no stream offset, this is handled in _render instead
                static_cast<UINT>(d3d9buf->getVertexSize()) // stride
                );
            if (FAILED(hr))
            {
                OGRE_EXCEPT(hr, "Unable to set D3D9 stream source for buffer binding", 
                    "D3D9RenderSystem::setVertexBufferBinding");
            }


        }

		// Unbind any unused sources
		for (size_t unused = source; unused < mLastVertexSourceCount; ++unused)
		{
			
            hr = mpD3DDevice->SetStreamSource(static_cast<UINT>(unused), NULL, 0, 0);
            if (FAILED(hr))
            {
                OGRE_EXCEPT(hr, "Unable to reset unused D3D9 stream source", 
                    "D3D9RenderSystem::setVertexBufferBinding");
            }
			
		}
		mLastVertexSourceCount = source;
		

		
        // UnGuard
		OgreUnguard();
	}
    //---------------------------------------------------------------------
    void D3D9RenderSystem::_render(const RenderOperation& op)
	{
		// Guard
		OgreGuard ("D3D9RenderSystem::_render");

        // Exit immediately if there is nothing to render
        // This caused a problem on FireGL 8800
        if (op.vertexData->vertexCount == 0)
            return;

        // Call super class
		RenderSystem::_render(op);

        // To think about: possibly remove setVertexDeclaration and 
        // setVertexBufferBinding from RenderSystem since the sequence is
        // a bit too D3D9-specific?
		setVertexDeclaration(op.vertexData->vertexDeclaration);
        setVertexBufferBinding(op.vertexData->vertexBufferBinding);

		// Determine rendering operation
		D3DPRIMITIVETYPE primType = D3DPT_TRIANGLELIST;
		DWORD primCount = 0;
        switch( op.operationType )
		{
        case RenderOperation::OT_POINT_LIST:
			primType = D3DPT_POINTLIST;
			primCount = (DWORD)(op.useIndexes ? op.indexData->indexCount : op.vertexData->vertexCount);
			break;

		case RenderOperation::OT_LINE_LIST:
			primType = D3DPT_LINELIST;
			primCount = (DWORD)(op.useIndexes ? op.indexData->indexCount : op.vertexData->vertexCount) / 2;
			break;

		case RenderOperation::OT_LINE_STRIP:
			primType = D3DPT_LINESTRIP;
			primCount = (DWORD)(op.useIndexes ? op.indexData->indexCount : op.vertexData->vertexCount) - 1;
			break;

		case RenderOperation::OT_TRIANGLE_LIST:
			primType = D3DPT_TRIANGLELIST;
			primCount = (DWORD)(op.useIndexes ? op.indexData->indexCount : op.vertexData->vertexCount) / 3;
			break;

		case RenderOperation::OT_TRIANGLE_STRIP:
			primType = D3DPT_TRIANGLESTRIP;
			primCount = (DWORD)(op.useIndexes ? op.indexData->indexCount : op.vertexData->vertexCount) - 2;
			break;

		case RenderOperation::OT_TRIANGLE_FAN:
			primType = D3DPT_TRIANGLEFAN;
			primCount = (DWORD)(op.useIndexes ? op.indexData->indexCount : op.vertexData->vertexCount) - 2;
			break;
		}

        if (!primCount)
			return;

		// Issue the op
        HRESULT hr;
		if( op.useIndexes )
		{
            D3D9HardwareIndexBuffer* d3dIdxBuf = 
                static_cast<D3D9HardwareIndexBuffer*>(op.indexData->indexBuffer.get());
			hr = mpD3DDevice->SetIndices( d3dIdxBuf->getD3DIndexBuffer() );
			if (FAILED(hr))
            {
				OGRE_EXCEPT( hr, "Failed to set index buffer", "D3D9RenderSystem::_render" );
            }

            do
            {
                // do indexed draw operation
			    hr = mpD3DDevice->DrawIndexedPrimitive(
                    primType, 
                    static_cast<INT>(op.vertexData->vertexStart), 
                    0, // Min vertex index - assume we can go right down to 0 
                    static_cast<UINT>(op.vertexData->vertexCount), 
                    static_cast<UINT>(op.indexData->indexStart), 
                    static_cast<UINT>(primCount)
                    );

            } while (updatePassIterationRenderState());
		}
		else
        {
            // nfz: gpu_iterate
            do
            {
                // Unindexed, a little simpler!
			    hr = mpD3DDevice->DrawPrimitive(
                    primType, 
                    static_cast<UINT>(op.vertexData->vertexStart), 
                    static_cast<UINT>(primCount)
                    ); 

            } while (updatePassIterationRenderState());
        } 

		if( FAILED( hr ) )
		{
			String msg = DXGetErrorDescription9(hr);
			OGRE_EXCEPT( hr, "Failed to DrawPrimitive : " + msg, "D3D9RenderSystem::_render" );
		}
        
        // UnGuard
		OgreUnguard();

	}
    //---------------------------------------------------------------------
    void D3D9RenderSystem::setNormaliseNormals(bool normalise)
    {
        __SetRenderState(D3DRS_NORMALIZENORMALS, 
            normalise ? TRUE : FALSE);
    }
	//---------------------------------------------------------------------
    void D3D9RenderSystem::bindGpuProgram(GpuProgram* prg)
    {
        HRESULT hr;
        switch (prg->getType())
        {
        case GPT_VERTEX_PROGRAM:
            hr = mpD3DDevice->SetVertexShader(
                static_cast<D3D9GpuVertexProgram*>(prg)->getVertexShader());
            if (FAILED(hr))
            {
                OGRE_EXCEPT(hr, "Error calling SetVertexShader", "D3D9RenderSystem::bindGpuProgram");
            }
            break;
        case GPT_FRAGMENT_PROGRAM:
            hr = mpD3DDevice->SetPixelShader(
                static_cast<D3D9GpuFragmentProgram*>(prg)->getPixelShader());
            if (FAILED(hr))
            {
                OGRE_EXCEPT(hr, "Error calling SetPixelShader", "D3D9RenderSystem::bindGpuProgram");
            }
            break;
        };
        
        RenderSystem::bindGpuProgram(prg);

    }
	//---------------------------------------------------------------------
    void D3D9RenderSystem::unbindGpuProgram(GpuProgramType gptype)
    {
        HRESULT hr;
        switch(gptype)
        {
        case GPT_VERTEX_PROGRAM:
            mActiveVertexGpuProgramParameters.setNull();
            hr = mpD3DDevice->SetVertexShader(NULL);
            if (FAILED(hr))
            {
                OGRE_EXCEPT(hr, "Error resetting SetVertexShader to NULL", 
                    "D3D9RenderSystem::unbindGpuProgram");
            }
            break;
        case GPT_FRAGMENT_PROGRAM:
            mActiveFragmentGpuProgramParameters.setNull();
            hr = mpD3DDevice->SetPixelShader(NULL);
            if (FAILED(hr))
            {
                OGRE_EXCEPT(hr, "Error resetting SetPixelShader to NULL", 
                    "D3D9RenderSystem::unbindGpuProgram");
            }
            break;
        };
        RenderSystem::unbindGpuProgram(gptype);
    }
	//---------------------------------------------------------------------
    void D3D9RenderSystem::bindGpuProgramParameters(GpuProgramType gptype, 
        GpuProgramParametersSharedPtr params)
    {
        HRESULT hr;
        unsigned int index;
        unsigned int count;
        unsigned int start;
        static std::vector<float> realConstants;
        static std::vector<int> intConstants;
        GpuProgramParameters::IntConstantIterator intIt = params->getIntConstantIterator();
        GpuProgramParameters::RealConstantIterator realIt = params->getRealConstantIterator();

        switch(gptype)
        {
        case GPT_VERTEX_PROGRAM:
            mActiveVertexGpuProgramParameters = params;
            // Bind floats
            if (params->hasRealConstantParams())
            {
                // Pre-allocate memory for collect real constants
                realConstants.resize(params->getRealConstantCount() * 4);

                // Iterate over params and set the relevant ones
                index = 0;
                count = 0;
                start = 0;
                while (realIt.hasMoreElements())
                {
                    const GpuProgramParameters::RealConstantEntry* e = realIt.peekNextPtr();
                    if (e->isSet)
                    {
                        if (!count)
                        {
                            start = index;
                        }
                        realConstants[(count * 4 ) + 0] = e->val[0];
                        realConstants[(count * 4 ) + 1] = e->val[1];
                        realConstants[(count * 4 ) + 2] = e->val[2];
                        realConstants[(count * 4 ) + 3] = e->val[3];
                        ++count;
                    }
                    else if (count)
                    {
                        if (FAILED(hr = mpD3DDevice->SetVertexShaderConstantF(
                            start, &realConstants[0], count)))
                        {
                            OGRE_EXCEPT(hr, "Unable to upload vertex shader float parameters", 
                                "D3D9RenderSystem::bindGpuProgramParameters");
                        }
                        count = 0;
                    }
                    index++;
                    realIt.moveNext();
                }
                if (count)
                {
                    if (FAILED(hr = mpD3DDevice->SetVertexShaderConstantF(
                        start, &realConstants[0], count)))
                    {
                        OGRE_EXCEPT(hr, "Unable to upload vertex shader float parameters", 
                            "D3D9RenderSystem::bindGpuProgramParameters");
                    }
                }
            }
            // Bind ints
            if (params->hasIntConstantParams())
            {
                // Iterate over params and set the relevant ones
                index = 0;
                while (intIt.hasMoreElements())
                {
                    const GpuProgramParameters::IntConstantEntry* e = intIt.peekNextPtr();
                    if (e->isSet)
                    {
                        if (FAILED(hr = mpD3DDevice->SetVertexShaderConstantI(
                            index, e->val, 1)))
                        {
                            OGRE_EXCEPT(hr, "Unable to upload vertex shader float parameters", 
                                "D3D9RenderSystem::bindGpuProgramParameters");
                        }
                    }
                    index++;
                    intIt.moveNext();
                }
            }
            break;
        case GPT_FRAGMENT_PROGRAM:
            mActiveFragmentGpuProgramParameters = params;
            // Bind floats
            if (params->hasRealConstantParams())
            {
                // Iterate over params and set the relevant ones
                index = 0;
                while (realIt.hasMoreElements())
                {
                    const GpuProgramParameters::RealConstantEntry* e = realIt.peekNextPtr();
                    if (e->isSet)
                    {
                        /*
                        // TEST
                        LogManager::getSingleton().logMessage(
                            "  Set Constant " + StringConverter::toString(index) + " to float4(" +
                            StringConverter::toString(e->val[0]) + ", " + 
                            StringConverter::toString(e->val[1]) + ", " + 
                            StringConverter::toString(e->val[2]) + ", " + 
                            StringConverter::toString(e->val[3]) + ")"); 
                        */

                        if (FAILED(hr = mpD3DDevice->SetPixelShaderConstantF(
                            index, e->val, 1)))
                        {
                            OGRE_EXCEPT(hr, "Unable to upload pixel shader float parameters", 
                                "D3D9RenderSystem::bindGpuProgramParameters");
                        }
                    }
                    index++;
                    realIt.moveNext();
                }
            }
            // Bind ints
            if (params->hasIntConstantParams())
            {
                // Iterate over params and set the relevant ones
                index = 0;
                while (intIt.hasMoreElements())
                {
                    const GpuProgramParameters::IntConstantEntry* e = intIt.peekNextPtr();
                    if (e->isSet)
                    {
                        if (FAILED(hr = mpD3DDevice->SetPixelShaderConstantI(
                            index, e->val, 1)))
                        {
                            OGRE_EXCEPT(hr, "Unable to upload pixel shader float parameters", 
                                "D3D9RenderSystem::bindGpuProgramParameters");
                        }
                    }
                    index++;
                    intIt.moveNext();
                }
            }
            break;
        };
    }
	//---------------------------------------------------------------------
    void D3D9RenderSystem::bindGpuProgramPassIterationParameters(GpuProgramType gptype)
    {

        HRESULT hr;
        GpuProgramParameters::RealConstantEntry* realEntry;

        switch(gptype)
        {
        case GPT_VERTEX_PROGRAM:
            realEntry = mActiveVertexGpuProgramParameters->getPassIterationEntry();
            if (realEntry)
            {
                if (FAILED(hr = mpD3DDevice->SetVertexShaderConstantF(
                        mActiveVertexGpuProgramParameters->getPassIterationEntryIndex(), realEntry->val, 1)))
                {
                    OGRE_EXCEPT(hr, "Unable to upload vertex shader multi pass parameters", 
                    "D3D9RenderSystem::bindGpuProgramMultiPassParameters");
                }
            }
            break;

        case GPT_FRAGMENT_PROGRAM:
            realEntry = mActiveFragmentGpuProgramParameters->getPassIterationEntry();
            if (realEntry)
            {
                if (FAILED(hr = mpD3DDevice->SetPixelShaderConstantF(
                        mActiveFragmentGpuProgramParameters->getPassIterationEntryIndex(), realEntry->val, 1)))
                {
                    OGRE_EXCEPT(hr, "Unable to upload pixel shader multi pass parameters", 
                    "D3D9RenderSystem::bindGpuProgramMultiPassParameters");
                }
            }
            break;

        }
    }
    //---------------------------------------------------------------------
    void D3D9RenderSystem::setClipPlanes(const PlaneList& clipPlanes)
    {
        size_t i;
        size_t numClipPlanes;
        float dx9ClipPlane[4];
        DWORD mask = 0;
        HRESULT hr;

        numClipPlanes = clipPlanes.size();
        for (i = 0; i < numClipPlanes; ++i)
        {
            const Plane& plane = clipPlanes[i];

            dx9ClipPlane[0] = plane.normal.x;
            dx9ClipPlane[1] = plane.normal.y;
            dx9ClipPlane[2] = plane.normal.z;
            dx9ClipPlane[3] = plane.d;

            hr = mpD3DDevice->SetClipPlane(i, dx9ClipPlane);
            if (FAILED(hr))
            {
                OGRE_EXCEPT(hr, "Unable to set clip plane", 
                    "D3D9RenderSystem::setClipPlanes");
            }

            mask |= (1 << i);
        }

        hr = __SetRenderState(D3DRS_CLIPPLANEENABLE, mask);
        if (FAILED(hr))
        {
            OGRE_EXCEPT(hr, "Unable to set render state for clip planes", 
                "D3D9RenderSystem::setClipPlanes");
        }
    }
	//---------------------------------------------------------------------
    void D3D9RenderSystem::setScissorTest(bool enabled, size_t left, size_t top, size_t right,
        size_t bottom)
    {
        HRESULT hr;
        if (enabled)
        {
            if (FAILED(hr = __SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE)))
            {
                OGRE_EXCEPT(hr, "Unable to enable scissor rendering state; " + getErrorDescription(hr), 
                    "D3D9RenderSystem::setScissorTest");
            }
            RECT rect;
            rect.left = left;
            rect.top = top;
            rect.bottom = bottom;
            rect.right = right;
            if (FAILED(hr = mpD3DDevice->SetScissorRect(&rect)))
            {
                OGRE_EXCEPT(hr, "Unable to set scissor rectangle; " + getErrorDescription(hr), 
                    "D3D9RenderSystem::setScissorTest");
            }
        }
        else
        {
            if (FAILED(hr = __SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE)))
            {
                OGRE_EXCEPT(hr, "Unable to disable scissor rendering state; " + getErrorDescription(hr), 
                    "D3D9RenderSystem::setScissorTest");
            }
        }
    }
    //---------------------------------------------------------------------
    void D3D9RenderSystem::clearFrameBuffer(unsigned int buffers, 
        const ColourValue& colour, Real depth, unsigned short stencil)
    {
        DWORD flags = 0;
        if (buffers & FBT_COLOUR)
        {
            flags |= D3DCLEAR_TARGET;
        }
        if (buffers & FBT_DEPTH)
        {
            flags |= D3DCLEAR_ZBUFFER;
        }
        // Only try to clear the stencil buffer if supported
        if (buffers & FBT_STENCIL && mCapabilities->hasCapability(RSC_HWSTENCIL))
        {
            flags |= D3DCLEAR_STENCIL;
        }
        HRESULT hr;
        if( FAILED( hr = mpD3DDevice->Clear( 
            0, 
            NULL, 
            flags,
            colour.getAsARGB(), 
            depth, 
            stencil ) ) )
        {
            String msg = DXGetErrorDescription9(hr);
            OGRE_EXCEPT( hr, "Error clearing frame buffer : " 
                + msg, "D3D9RenderSystem::clearFrameBuffer" );
        }
    }
    //---------------------------------------------------------------------
    void D3D9RenderSystem::_makeProjectionMatrix(Real left, Real right, 
        Real bottom, Real top, Real nearPlane, Real farPlane, Matrix4& dest,
        bool forGpuProgram)
    {
        // Correct position for off-axis projection matrix
        if (!forGpuProgram)
        {
            Real offsetX = left + right;
            Real offsetY = top + bottom;

            left -= offsetX;
            right -= offsetX;
            top -= offsetY;
            bottom -= offsetY;
        }

        Real width = right - left;
        Real height = top - bottom;
        Real q, qn;
        if (farPlane == 0)
        {
            q = 1 - Frustum::INFINITE_FAR_PLANE_ADJUST;
            qn = nearPlane * (Frustum::INFINITE_FAR_PLANE_ADJUST - 1);
        }
        else
        {
            q = farPlane / ( farPlane - nearPlane );
            qn = -q * nearPlane;
        }
        dest = Matrix4::ZERO;
        dest[0][0] = 2 * nearPlane / width;
        dest[0][2] = (right+left) / width;
        dest[1][1] = 2 * nearPlane / height;
        dest[1][2] = (top+bottom) / height;
        if (forGpuProgram)
        {
            dest[2][2] = -q;
            dest[3][2] = -1.0f;
        }
        else
        {
            dest[2][2] = q;
            dest[3][2] = 1.0f;
        }
        dest[2][3] = qn;
    }

    // ------------------------------------------------------------------
    void D3D9RenderSystem::setClipPlane (ushort index, Real A, Real B, Real C, Real D)
    {
        float plane[4] = { A, B, C, D };
        mpD3DDevice->SetClipPlane (index, plane);
    }

    // ------------------------------------------------------------------
    void D3D9RenderSystem::enableClipPlane (ushort index, bool enable)
    {
        DWORD prev;
        mpD3DDevice->GetRenderState(D3DRS_CLIPPLANEENABLE, &prev);
        __SetRenderState(D3DRS_CLIPPLANEENABLE, prev | (1 << index));
    }
    //---------------------------------------------------------------------
    HardwareOcclusionQuery* D3D9RenderSystem::createHardwareOcclusionQuery(void)
    {
		D3D9HardwareOcclusionQuery* ret = new D3D9HardwareOcclusionQuery (mpD3DDevice); 
		mHwOcclusionQueries.push_back(ret);
		return ret;
    }
    //---------------------------------------------------------------------
    Real D3D9RenderSystem::getHorizontalTexelOffset(void)
    {
        // D3D considers the origin to be in the center of a pixel
        return -0.5f;
    }
    //---------------------------------------------------------------------
    Real D3D9RenderSystem::getVerticalTexelOffset(void)
    {
        // D3D considers the origin to be in the center of a pixel
        return -0.5f;
    }
    //---------------------------------------------------------------------
    void D3D9RenderSystem::_applyObliqueDepthProjection(Matrix4& matrix, const Plane& plane, 
        bool forGpuProgram)
    {
        // Thanks to Eric Lenyel for posting this calculation at www.terathon.com

        // Calculate the clip-space corner point opposite the clipping plane
        // as (sgn(clipPlane.x), sgn(clipPlane.y), 1, 1) and
        // transform it into camera space by multiplying it
        // by the inverse of the projection matrix

        /* generalised version
        Vector4 q = matrix.inverse() * 
            Vector4(Math::Sign(plane.normal.x), Math::Sign(plane.normal.y), 1.0f, 1.0f);
        */
        Vector4 q;
        q.x = Math::Sign(plane.normal.x) / matrix[0][0];
        q.y = Math::Sign(plane.normal.y) / matrix[1][1];
        q.z = 1.0F; 
        // flip the next bit from Lengyel since we're right-handed
        if (forGpuProgram)
        {
            q.w = (1.0F - matrix[2][2]) / matrix[2][3];
        }
        else
        {
            q.w = (1.0F + matrix[2][2]) / matrix[2][3];
        }

        // Calculate the scaled plane vector
        Vector4 clipPlane4d(plane.normal.x, plane.normal.y, plane.normal.z, plane.d);
        Vector4 c = clipPlane4d * (1.0F / (clipPlane4d.dotProduct(q)));

        // Replace the third row of the projection matrix
        matrix[2][0] = c.x;
        matrix[2][1] = c.y;
        // flip the next bit from Lengyel since we're right-handed
        if (forGpuProgram)
        {
            matrix[2][2] = c.z; 
        }
        else
        {
            matrix[2][2] = -c.z; 
        }
        matrix[2][3] = c.w;        
    }
    //---------------------------------------------------------------------
    Real D3D9RenderSystem::getMinimumDepthInputValue(void)
    {
        // Range [0.0f, 1.0f]
        return 0.0f;
    }
    //---------------------------------------------------------------------
    Real D3D9RenderSystem::getMaximumDepthInputValue(void)
    {
        // Range [0.0f, 1.0f]
        // D3D inverts even identity view matrices, so maximum INPUT is -1.0
        return -1.0f;
    }
	//---------------------------------------------------------------------
	void D3D9RenderSystem::restoreLostDevice(void)
	{
		// Release all non-managed resources

		// Cleanup depth stencils
		_cleanupDepthStencils();

		// Set all texture units to nothing
		_disableTextureUnitsFrom(0);

		// Unbind any vertex streams
		for (size_t i = 0; i < mLastVertexSourceCount; ++i)
		{
			mpD3DDevice->SetStreamSource(i, NULL, 0, 0);
		}
        mLastVertexSourceCount = 0;

        // Release all automatic temporary buffers and free unused
        // temporary buffers, so we doesn't need to recreate them,
        // and they will reallocate on demand. This save a lot of
        // release/recreate of non-managed vertex buffers which
        // wasn't need at all.
        mHardwareBufferManager->_releaseBufferCopies(true);

		// We have to deal with non-managed textures and vertex buffers
		// GPU programs don't have to be restored
		static_cast<D3D9TextureManager*>(mTextureManager)->releaseDefaultPoolResources();
		static_cast<D3D9HardwareBufferManager*>(mHardwareBufferManager)
			->releaseDefaultPoolResources();

		// release additional swap chains (secondary windows)
		SecondaryWindowList::iterator sw;
		for (sw = mSecondaryWindows.begin(); sw != mSecondaryWindows.end(); ++sw)
		{
			(*sw)->destroyD3DResources();
		}

		// Reset the device, using the primary window presentation params
		HRESULT hr = mpD3DDevice->Reset(
			mPrimaryWindow->getPresentationParameters());

		if (hr == D3DERR_DEVICELOST)
		{
			// Don't continue
			return;
		}
		else if (FAILED(hr))
		{
			OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
				"Cannot reset device! " + getErrorDescription(hr), 
				"D3D9RenderWindow::restoreLostDevice" );
		}

		// will have lost basic states
		mBasicStatesInitialised = false;
        mVertexProgramBound = false;
        mFragmentProgramBound = false;


		// recreate additional swap chains
		for (sw = mSecondaryWindows.begin(); sw != mSecondaryWindows.end(); ++sw)
		{
			(*sw)->createD3DResources();
		}

		// Recreate all non-managed resources
		static_cast<D3D9TextureManager*>(mTextureManager)
			->recreateDefaultPoolResources();
		static_cast<D3D9HardwareBufferManager*>(mHardwareBufferManager)
			->recreateDefaultPoolResources();
			
		LogManager::getSingleton().logMessage("!!! Direct3D Device successfully restored.");

		mDeviceLost = false;

		fireEvent("DeviceRestored");

	}
	//---------------------------------------------------------------------
	bool D3D9RenderSystem::isDeviceLost(void)
	{
		return mDeviceLost;
	}
	//---------------------------------------------------------------------
	void D3D9RenderSystem::_notifyDeviceLost(void)
	{
		LogManager::getSingleton().logMessage("!!! Direct3D Device Lost!");
		mDeviceLost = true;
		// will have lost basic states
		mBasicStatesInitialised = false;

		fireEvent("DeviceLost");
	}

	//---------------------------------------------------------------------
	// Formats to try, in decreasing order of preference
	D3DFORMAT ddDepthStencilFormats[]={
		D3DFMT_D24FS8,
		D3DFMT_D24S8,
		D3DFMT_D24X4S4,
		D3DFMT_D24X8,
		D3DFMT_D15S1,
		D3DFMT_D16,
		D3DFMT_D32
	};
#define NDSFORMATS (sizeof(ddDepthStencilFormats)/sizeof(D3DFORMAT))
	
	D3DFORMAT D3D9RenderSystem::_getDepthStencilFormatFor(D3DFORMAT fmt)
	{
		// Check with current adapter display format
        D3DDISPLAYMODE d3ddm;
        if (FAILED(mpD3D->GetAdapterDisplayMode(mActiveD3DDriver->getAdapterNumber(), &d3ddm)))
		    return D3DFMT_UNKNOWN;
        else
            return _getDepthStencilFormatFor(fmt, d3ddm.Format);
    }

	D3DFORMAT D3D9RenderSystem::_getDepthStencilFormatFor(D3DFORMAT renderTargetFormat, D3DFORMAT adapterFormat)
	{
        FormatPair formatPair(renderTargetFormat, adapterFormat);

		/// Check if result is cached
		DepthStencilHash::iterator i = mDepthStencilHash.find(formatPair);
		if(i != mDepthStencilHash.end())
			return i->second;

		/// If not, probe with CheckDepthStencilMatch
		D3DFORMAT dsfmt = D3DFMT_UNKNOWN;

        /// Probe all depth stencil formats
        /// Break on first one that matches
        for(size_t x=0; x<NDSFORMATS; ++x)
        {
            // Verify that the depth format exists
            if (mpD3D->CheckDeviceFormat(
                mActiveD3DDriver->getAdapterNumber(),
                D3DDEVTYPE_HAL,
                adapterFormat,
                D3DUSAGE_DEPTHSTENCIL,
                D3DRTYPE_SURFACE,
                ddDepthStencilFormats[x]) != D3D_OK)
            {
                continue;
            }
            // Verify that the depth format is compatible
            if(mpD3D->CheckDepthStencilMatch(
                mActiveD3DDriver->getAdapterNumber(),
                D3DDEVTYPE_HAL, adapterFormat,
                renderTargetFormat, ddDepthStencilFormats[x]) == D3D_OK)
            {
                dsfmt = ddDepthStencilFormats[x];
                break;
            }
        }

		/// Cache result
		mDepthStencilHash[formatPair] = dsfmt;
		return dsfmt;
	}
	IDirect3DSurface9* D3D9RenderSystem::_getDepthStencilFor(D3DFORMAT fmt, D3DMULTISAMPLE_TYPE multisample, size_t width, size_t height)
	{
		D3DFORMAT dsfmt = _getDepthStencilFormatFor(fmt);
		if(dsfmt == D3DFMT_UNKNOWN)
			return 0;
		IDirect3DSurface9 *surface = 0;

		/// Check if result is cached
		ZBufferFormat zbfmt(dsfmt, multisample);
		ZBufferHash::iterator i = mZBufferHash.find(zbfmt);
		if(i != mZBufferHash.end())
		{
			/// Check if size is larger or equal
			if(i->second.width >= width && i->second.height >= height)
			{
				surface = i->second.surface;
			} 
			else
			{
				/// If not, destroy current buffer
				i->second.surface->Release();
				mZBufferHash.erase(i);
			}
		}
		if(!surface)
		{
			/// If not, create the depthstencil surface
			HRESULT hr = mpD3DDevice->CreateDepthStencilSurface( 
				width, 
				height, 
				dsfmt, 
				multisample, 
				NULL, 
				TRUE,  // discard true or false?
				&surface, 
				NULL);
			if(FAILED(hr))
			{
				String msg = DXGetErrorDescription9(hr);
				OGRE_EXCEPT( hr, "Error CreateDepthStencilSurface : " + msg, "D3D9RenderSystem::_getDepthStencilFor" );
			}
			/// And cache it
			ZBufferRef zb;
			zb.surface = surface;
			zb.width = width;
			zb.height = height;
			mZBufferHash[zbfmt] = zb;
		}
		return surface;
	}
	void D3D9RenderSystem::_cleanupDepthStencils()
	{
		for(ZBufferHash::iterator i = mZBufferHash.begin(); i != mZBufferHash.end(); ++i)
		{
			/// Release buffer
			i->second.surface->Release();
		}
		mZBufferHash.clear();
	}
}
