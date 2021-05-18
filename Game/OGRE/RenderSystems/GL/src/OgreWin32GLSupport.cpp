#include "OgreRoot.h"
#include "OgreException.h"
#include "OgreLogManager.h"
#include "OgreStringConverter.h"

#include <algorithm>

#include "OgreWin32GLSupport.h"
#include "OgreGLTexture.h"
#include "OgreWin32Window.h"
#include <GL/wglext.h>
#include "OgreWin32RenderTexture.h"

using namespace Ogre;

GLenum wglewContextInit (Ogre::GLSupport *glSupport);

namespace Ogre {
	Win32GLSupport::Win32GLSupport()
        : mInitialWindow(0)
        , mHasPixelFormatARB(false)
        , mHasMultisample(false)
    {
		// immediately test WGL_ARB_pixel_format and FSAA support
		// so we can set configuration options appropriately
		initialiseWGL();
    } 

	template<class C> void remove_duplicates(C& c)
	{
		std::sort(c.begin(), c.end());
		typename C::iterator p = std::unique(c.begin(), c.end());
		c.erase(p, c.end());
	}

	void Win32GLSupport::addConfig()
	{
		//TODO: EnumDisplayDevices http://msdn.microsoft.com/library/en-us/gdi/devcons_2303.asp
		/*vector<string> DisplayDevices;
		DISPLAY_DEVICE DisplayDevice;
		DisplayDevice.cb = sizeof(DISPLAY_DEVICE);
		DWORD i=0;
		while (EnumDisplayDevices(NULL, i++, &DisplayDevice, 0) {
			DisplayDevices.push_back(DisplayDevice.DeviceName);
		}*/
		  
		ConfigOption optFullScreen;
		ConfigOption optVideoMode;
		ConfigOption optColourDepth;
		ConfigOption optDisplayFrequency;
		ConfigOption optVSync;
		ConfigOption optFSAA;
		ConfigOption optRTTMode;

		// FS setting possiblities
		optFullScreen.name = "Full Screen";
		optFullScreen.possibleValues.push_back("Yes");
		optFullScreen.possibleValues.push_back("No");
		optFullScreen.currentValue = "Yes";
		optFullScreen.immutable = false;

		// Video mode possiblities
		DEVMODE DevMode;
		DevMode.dmSize = sizeof(DEVMODE);
		optVideoMode.name = "Video Mode";
		optVideoMode.immutable = false;
		for (DWORD i = 0; EnumDisplaySettings(NULL, i, &DevMode); ++i)
		{
			if (DevMode.dmBitsPerPel < 16 || DevMode.dmPelsHeight < 480)
				continue;
			mDevModes.push_back(DevMode);
			char szBuf[16];
			snprintf(szBuf, 16, "%d x %d", DevMode.dmPelsWidth, DevMode.dmPelsHeight);
			optVideoMode.possibleValues.push_back(szBuf);
		}
		remove_duplicates(optVideoMode.possibleValues);
		optVideoMode.currentValue = optVideoMode.possibleValues.front();

		optColourDepth.name = "Colour Depth";
		optColourDepth.immutable = false;
		optColourDepth.currentValue.clear();

		optDisplayFrequency.name = "Display Frequency";
		optDisplayFrequency.immutable = false;
        optDisplayFrequency.currentValue.clear();

		optVSync.name = "VSync";
		optVSync.immutable = false;
		optVSync.possibleValues.push_back("No");
		optVSync.possibleValues.push_back("Yes");
		optVSync.currentValue = "No";

		optFSAA.name = "FSAA";
		optFSAA.immutable = false;
		optFSAA.possibleValues.push_back("0");
		for (std::vector<int>::iterator it = mFSAALevels.begin(); it != mFSAALevels.end(); ++it)
			optFSAA.possibleValues.push_back(StringConverter::toString(*it));
		optFSAA.currentValue = "0";

		optRTTMode.name = "RTT Preferred Mode";
		optRTTMode.possibleValues.push_back("FBO");
		optRTTMode.possibleValues.push_back("PBuffer");
		optRTTMode.possibleValues.push_back("Copy");
		optRTTMode.currentValue = "FBO";
		optRTTMode.immutable = false;


		mOptions[optFullScreen.name] = optFullScreen;
		mOptions[optVideoMode.name] = optVideoMode;
		mOptions[optColourDepth.name] = optColourDepth;
		mOptions[optDisplayFrequency.name] = optDisplayFrequency;
		mOptions[optVSync.name] = optVSync;
		mOptions[optFSAA.name] = optFSAA;
		mOptions[optRTTMode.name] = optRTTMode;

		refreshConfig();
	}

	void Win32GLSupport::refreshConfig()
	{
		ConfigOptionMap::iterator optVideoMode = mOptions.find("Video Mode");
		ConfigOptionMap::iterator moptColourDepth = mOptions.find("Colour Depth");
		ConfigOptionMap::iterator moptDisplayFrequency = mOptions.find("Display Frequency");
		if(optVideoMode == mOptions.end() || moptColourDepth == mOptions.end() || moptDisplayFrequency == mOptions.end())
			OGRE_EXCEPT(999, "Can't find mOptions!", "Win32GLSupport::refreshConfig");
		ConfigOption* optColourDepth = &moptColourDepth->second;
		ConfigOption* optDisplayFrequency = &moptDisplayFrequency->second;

		const String& val = optVideoMode->second.currentValue;
		String::size_type pos = val.find('x');
		if (pos == String::npos)
			OGRE_EXCEPT(999, "Invalid Video Mode provided", "Win32GLSupport::refreshConfig");
		DWORD width = StringConverter::parseUnsignedInt(val.substr(0, pos));
		DWORD height = StringConverter::parseUnsignedInt(val.substr(pos+1, String::npos));

		for(std::vector<DEVMODE>::const_iterator i = mDevModes.begin(); i != mDevModes.end(); ++i)
		{
			if (i->dmPelsWidth != width || i->dmPelsHeight != height)
				continue;
			optColourDepth->possibleValues.push_back(StringConverter::toString(i->dmBitsPerPel));
			optDisplayFrequency->possibleValues.push_back(StringConverter::toString(i->dmDisplayFrequency));
		}
		remove_duplicates(optColourDepth->possibleValues);
		remove_duplicates(optDisplayFrequency->possibleValues);
		optColourDepth->currentValue = optColourDepth->possibleValues.back();
		if (optDisplayFrequency->currentValue != "N/A")
			optDisplayFrequency->currentValue = optDisplayFrequency->possibleValues.front();
	}

	void Win32GLSupport::setConfigOption(const String &name, const String &value)
	{
		ConfigOptionMap::iterator it = mOptions.find(name);

		// Update
		if(it != mOptions.end())
			it->second.currentValue = value;
		else
		{
            StringUtil::StrStreamType str;
            str << "Option named '" << name << "' does not exist.";
			OGRE_EXCEPT( Exception::ERR_INVALIDPARAMS, str.str(), "Win32GLSupport::setConfigOption" );
		}

		if( name == "Video Mode" )
			refreshConfig();

		if( name == "Full Screen" )
		{
			it = mOptions.find( "Display Frequency" );
			if( value == "No" )
			{
				it->second.currentValue = "N/A";
				it->second.immutable = true;
			}
			else
			{
				it->second.currentValue = it->second.possibleValues.front();
				it->second.immutable = false;
			}
		}
	}

	String Win32GLSupport::validateConfig()
	{
		// TODO, DX9
		return StringUtil::BLANK;
	}

	RenderWindow* Win32GLSupport::createWindow(bool autoCreateWindow, GLRenderSystem* renderSystem, const String& windowTitle)
	{
		if (autoCreateWindow)
        {
            ConfigOptionMap::iterator opt = mOptions.find("Full Screen");
            if (opt == mOptions.end())
                OGRE_EXCEPT(999, "Can't find full screen options!", "Win32GLSupport::createWindow");
            bool fullscreen = (opt->second.currentValue == "Yes");

            opt = mOptions.find("Video Mode");
            if (opt == mOptions.end())
                OGRE_EXCEPT(999, "Can't find video mode options!", "Win32GLSupport::createWindow");
            String val = opt->second.currentValue;
            String::size_type pos = val.find('x');
            if (pos == String::npos)
                OGRE_EXCEPT(999, "Invalid Video Mode provided", "Win32GLSupport::createWindow");

			unsigned int w = StringConverter::parseUnsignedInt(val.substr(0, pos));
            unsigned int h = StringConverter::parseUnsignedInt(val.substr(pos + 1));

			// Parse optional parameters
			NameValuePairList winOptions;
			opt = mOptions.find("Colour Depth");
			if (opt == mOptions.end())
				OGRE_EXCEPT(999, "Can't find Colour Depth options!", "Win32GLSupport::createWindow");
			unsigned int colourDepth =
				StringConverter::parseUnsignedInt(opt->second.currentValue);
			winOptions["colourDepth"] = StringConverter::toString(colourDepth);

			opt = mOptions.find("VSync");
			if (opt == mOptions.end())
				OGRE_EXCEPT(999, "Can't find VSync options!", "Win32GLSupport::createWindow");
			bool vsync = (opt->second.currentValue == "Yes");
			winOptions["vsync"] = StringConverter::toString(vsync);
			renderSystem->setWaitForVerticalBlank(vsync);

			opt = mOptions.find("FSAA");
			if (opt == mOptions.end())
				OGRE_EXCEPT(999, "Can't find FSAA options!", "Win32GLSupport::createWindow");
			unsigned int multisample =
				StringConverter::parseUnsignedInt(opt->second.currentValue);
			winOptions["FSAA"] = StringConverter::toString(multisample);

            return renderSystem->createRenderWindow(windowTitle, w, h, fullscreen, &winOptions);
        }
        else
        {
            // XXX What is the else?
			return NULL;
        }
	}

	RenderWindow* Win32GLSupport::newWindow(const String &name, unsigned int width, 
		unsigned int height, bool fullScreen, const NameValuePairList *miscParams)
	{
		ConfigOptionMap::iterator opt = mOptions.find("Display Frequency");
		if (opt == mOptions.end())
			OGRE_EXCEPT(999, "Can't find Display Frequency options!", "Win32GLSupport::newWindow");
		unsigned int displayFrequency = StringConverter::parseUnsignedInt(opt->second.currentValue);

		Win32Window* window = new Win32Window(*this);
		window->create(name, width, height, fullScreen, miscParams);

		if(!mInitialWindow)
			mInitialWindow = window;
		return window;
	}

	void Win32GLSupport::start()
	{
		LogManager::getSingleton().logMessage("*** Starting Win32GL Subsystem ***");
	}

	void Win32GLSupport::stop()
	{
		LogManager::getSingleton().logMessage("*** Stopping Win32GL Subsystem ***");
	}

	void Win32GLSupport::initialiseExtensions() {
		assert(mInitialWindow);
		// First, initialise the normal extensions
		GLSupport::initialiseExtensions();
		// wglew init
		wglewContextInit(this);

		// Check for W32 specific extensions probe function
		PFNWGLGETEXTENSIONSSTRINGARBPROC _wglGetExtensionsStringARB = 
			(PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
		if(!_wglGetExtensionsStringARB)
			return;
		const char *wgl_extensions = _wglGetExtensionsStringARB(mInitialWindow->getHDC());
        StringUtil::StrStreamType str;
        str << "Supported WGL extensions: " << wgl_extensions;
		LogManager::getSingleton().logMessage(
			LML_NORMAL, str.str());
		// Parse them, and add them to the main list
		std::stringstream ext;
        String instr;
		ext << wgl_extensions;
        while(ext >> instr)
        {
            extensionList.insert(instr);
        }
	}


	void* Win32GLSupport::getProcAddress(const String& procname)
	{
        	return (void*)wglGetProcAddress( procname.c_str() );
	}
/*
	RenderTexture * Win32GLSupport::createRenderTexture( const String & name, 
		unsigned int width, unsigned int height,
		TextureType texType, PixelFormat internalFormat, 
		const NameValuePairList *miscParams ) 
	{
#ifdef HW_RTT
		bool useBind = checkExtension("WGL_ARB_render_texture");

		if(Root::getSingleton().getRenderSystem()->getCapabilities()->hasCapability(RSC_HWRENDER_TO_TEXTURE))
			return new Win32RenderTexture(*this, name, width, height, texType, 
				internalFormat, miscParams, useBind);
		else
#endif
			return new GLRenderTexture(name, width, height, texType, internalFormat, miscParams);
	}

*/
	void Win32GLSupport::initialiseWGL()
	{
		// wglGetProcAddress does not work without an active OpenGL context,
		// but we need wglChoosePixelFormatARB's address before we can
		// create our main window.  Thank you very much, Microsoft!
		//
		// The solution is to create a dummy OpenGL window first, and then
		// test for WGL_ARB_pixel_format support.  If it is not supported,
		// we make sure to never call the ARB pixel format functions.
		//
		// If is is supported, we call the pixel format functions at least once
		// to initialise them (pointers are stored by glprocs.h).  We can also
		// take this opportunity to enumerate the valid FSAA modes.
		
		LPCSTR dummyText = "OgreWglDummy";
		HINSTANCE hinst = GetModuleHandle("RenderSystem_GL.dll");
		
		WNDCLASS dummyClass;
		memset(&dummyClass, 0, sizeof(WNDCLASS));
		dummyClass.style = CS_OWNDC;
		dummyClass.hInstance = hinst;
		dummyClass.lpfnWndProc = dummyWndProc;
		dummyClass.lpszClassName = dummyText;
		RegisterClass(&dummyClass);

		HWND hwnd = CreateWindow(dummyText, dummyText,
			WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
			0, 0, 32, 32, 0, 0, hinst, 0);

		// if a simple CreateWindow fails, then boy are we in trouble...
		if (hwnd == NULL)
			OGRE_EXCEPT(0, "CreateWindow() failed", "Win32GLSupport::initializeWGL");


		// no chance of failure and no need to release thanks to CS_OWNDC
		HDC hdc = GetDC(hwnd); 

		// assign a simple OpenGL pixel format that everyone supports
		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.cColorBits = 16;
		pfd.cDepthBits = 15;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		
		// if these fail, wglCreateContext will also quietly fail
		int format;
		if ((format = ChoosePixelFormat(hdc, &pfd)) != 0)
			SetPixelFormat(hdc, format, &pfd);

		HGLRC hrc = wglCreateContext(hdc);
		if (hrc)
		{
			// if wglMakeCurrent fails, wglGetProcAddress will return null
			wglMakeCurrent(hdc, hrc);
			
			PFNWGLGETEXTENSIONSSTRINGARBPROC _wglGetExtensionsStringARB =
				(PFNWGLGETEXTENSIONSSTRINGARBPROC)
				wglGetProcAddress("wglGetExtensionsStringARB");
			
			// check for pixel format and multisampling support
			if (_wglGetExtensionsStringARB)
			{
				std::istringstream wglexts(_wglGetExtensionsStringARB(hdc));
				std::string ext;
				while (wglexts >> ext)
				{
					if (ext == "WGL_ARB_pixel_format")
						mHasPixelFormatARB = true;
					else if (ext == "WGL_ARB_multisample")
						mHasMultisample = true;
				}
			}

			if (mHasPixelFormatARB && mHasMultisample)
			{
				// enumerate all formats w/ multisampling
				static const int iattr[] = {
					WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
					WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
					WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
					WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
					WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
                    /* We are no matter about the colour, depth and stencil buffers here
					WGL_COLOR_BITS_ARB, 24,
					WGL_ALPHA_BITS_ARB, 8,
					WGL_DEPTH_BITS_ARB, 24,
					WGL_STENCIL_BITS_ARB, 8,
                    */
					WGL_SAMPLES_ARB, 2,
					0
				};
				int formats[256];
				unsigned int count;
                // cheating here.  wglChoosePixelFormatARB procc address needed later on
                // when a valid GL context does not exist and glew is not initialized yet.
                __wglewChoosePixelFormatARB =
                    (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
                if (__wglewChoosePixelFormatARB(hdc, iattr, 0, 256, formats, &count))
                {
                    // determine what multisampling levels are offered
                    int query = WGL_SAMPLES_ARB, samples;
                    for (unsigned int i = 0; i < count; ++i)
                    {
                        PFNWGLGETPIXELFORMATATTRIBIVARBPROC _wglGetPixelFormatAttribivARB =
                            (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)
                            wglGetProcAddress("wglGetPixelFormatAttribivARB");
                        if (_wglGetPixelFormatAttribivARB(hdc, formats[i], 0, 1, &query, &samples))
                        {
                            mFSAALevels.push_back(samples);
                        }
                    }
                    remove_duplicates(mFSAALevels);
                }
			}
			
			wglMakeCurrent(0, 0);
			wglDeleteContext(hrc);
		}

		// clean up our dummy window and class
		DestroyWindow(hwnd);
		UnregisterClass(dummyText, hinst);
	}

	LRESULT Win32GLSupport::dummyWndProc(HWND hwnd, UINT umsg, WPARAM wp, LPARAM lp)
	{
		return DefWindowProc(hwnd, umsg, wp, lp);
	}

	bool Win32GLSupport::selectPixelFormat(HDC hdc, int colourDepth, int multisample)
	{
		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(pfd));
		pfd.nSize = sizeof(pfd);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = (colourDepth > 16)? 24 : colourDepth;
		pfd.cAlphaBits = (colourDepth > 16)? 8 : 0;
		pfd.cDepthBits = 24;
		pfd.cStencilBits = 8;

		int format = 0;

		if (multisample)
		{
			// only available with driver support
			if (!mHasMultisample || !mHasPixelFormatARB)
				return false;
			
			int iattr[] = {
				WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
				WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
				WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
				WGL_SAMPLE_BUFFERS_ARB, GL_TRUE,
				WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
				WGL_COLOR_BITS_ARB, pfd.cColorBits,
				WGL_ALPHA_BITS_ARB, pfd.cAlphaBits,
				WGL_DEPTH_BITS_ARB, 24,
				WGL_STENCIL_BITS_ARB, 8,
				WGL_SAMPLES_ARB, multisample,
				0
			};

			UINT nformats;
            assert(__wglewChoosePixelFormatARB && "failed to get proc address for ChoosePixelFormatARB");
            // ChoosePixelFormatARB proc address was obtained when setting up a dummy GL context in initialiseWGL()
            // since glew hasn't been initialized yet, we have to cheat and use the previously obtained address
			if (!__wglewChoosePixelFormatARB(hdc, iattr, NULL, 1, &format, &nformats) || nformats <= 0)
                return false;
		}
		else
		{
			format = ChoosePixelFormat(hdc, &pfd);
		}

		return (format != 0 && SetPixelFormat(hdc, format, &pfd));
	}

	bool Win32GLSupport::supportsPBuffers()
	{
		return __WGLEW_ARB_pbuffer != GL_FALSE;
	}
    GLPBuffer *Win32GLSupport::createPBuffer(PixelComponentType format, size_t width, size_t height)
	{
		return new Win32PBuffer(format, width, height);
	}
}
