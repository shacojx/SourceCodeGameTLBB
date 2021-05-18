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

#include "OgreWin32Window.h"
#include "OgreRoot.h"
#include "OgreLogManager.h"
#include "OgreRenderSystem.h"
#include "OgreImageCodec.h"
#include "OgreStringConverter.h"
#include "OgreException.h"
#include "OgreWin32GLSupport.h"
#include "OgreWin32Context.h"

namespace Ogre {

	Win32Window::Win32Window(Win32GLSupport &glsupport):
		mGLSupport(glsupport),
		mContext(0)
	{
		mIsFullScreen = false;
		mHWnd = 0;
		mIsExternal = false;
		mSizing = false;
		mClosed = false;
		mDisplayFrequency = 0;
		mActive = false;
	}

	Win32Window::~Win32Window()
	{
		destroy();
	}

	void Win32Window::create(const String& name, unsigned int width, unsigned int height,
							bool fullScreen, const NameValuePairList *miscParams)
	{
		// destroy current window, if any
		if (mHWnd)
			destroy();

		HINSTANCE hInst = GetModuleHandle("RenderSystem_GL.dll");

		mHWnd = 0;
		mName = name;
		mIsFullScreen = fullScreen;
		mClosed = false;

		// load window defaults
		mLeft = mTop = -1; // centered
		mWidth = width;
		mHeight = height;
		mDisplayFrequency = 0;
		mIsDepthBuffered = true;
		mColourDepth = mIsFullScreen? 32 : GetDeviceCaps(GetDC(0), BITSPIXEL);

		HWND parent = 0;
		String title = name;
		bool vsync = false;
		int fsaa = 0;
		String border;
		bool outerSize = false;

		if(miscParams)
		{
			// Get variable-length params
			NameValuePairList::const_iterator opt;
			NameValuePairList::const_iterator end = miscParams->end();

			if ((opt = miscParams->find("title")) != end)
				title = opt->second;

			if ((opt = miscParams->find("left")) != end)
				mLeft = StringConverter::parseInt(opt->second);

			if ((opt = miscParams->find("top")) != end)
				mTop = StringConverter::parseInt(opt->second);

			if ((opt = miscParams->find("depthBuffer")) != end)
				mIsDepthBuffered = StringConverter::parseBool(opt->second);

			if ((opt = miscParams->find("vsync")) != end)
				vsync = StringConverter::parseBool(opt->second);

			if ((opt = miscParams->find("FSAA")) != end)
				fsaa = StringConverter::parseUnsignedInt(opt->second);

			if ((opt = miscParams->find("externalWindowHandle")) != end)
			{
				mHWnd = (HWND)StringConverter::parseUnsignedInt(opt->second);
				if (mHWnd)
				{
					mIsExternal = true;
					mIsFullScreen = false;
				}
			}
			// window border style
			opt = miscParams->find("border");
			if(opt != miscParams->end())
				border = opt->second;
			// set outer dimensions?
			opt = miscParams->find("outerDimensions");
			if(opt != miscParams->end())
				outerSize = StringConverter::parseBool(opt->second);

			if (mIsFullScreen)
			{
				// only available with fullscreen
				if ((opt = miscParams->find("displayFrequency")) != end)
					mDisplayFrequency = StringConverter::parseUnsignedInt(opt->second);
				if ((opt = miscParams->find("colourDepth")) != end)
					mColourDepth = StringConverter::parseUnsignedInt(opt->second);
			}
			else
			{
				// incompatible with fullscreen
				if ((opt = miscParams->find("parentWindowHandle")) != end)
					parent = (HWND)StringConverter::parseUnsignedInt(opt->second);
			}
		}

		if (!mIsExternal)
		{
			DWORD dwStyle = WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
			DWORD dwStyleEx = 0;
			int outerw, outerh;

			if (mIsFullScreen)
			{
				dwStyle |= WS_POPUP;
				dwStyleEx |= WS_EX_TOPMOST;
				outerw = mWidth;
				outerh = mHeight;
				mLeft = mTop = 0;
			}
			else
			{
				if (parent)
				{
					dwStyle |= WS_CHILD;
				}
				else
				{
					if (border == "none")
						dwStyle |= WS_POPUP;
					else if (border == "fixed")
						dwStyle |= WS_OVERLAPPED | WS_BORDER | WS_CAPTION |
						WS_SYSMENU | WS_MINIMIZEBOX;
					else
						dwStyle |= WS_OVERLAPPEDWINDOW;
				}

				int screenw = GetSystemMetrics(SM_CXSCREEN);
				int screenh = GetSystemMetrics(SM_CYSCREEN);

				if (!outerSize)
				{
					// calculate overall dimensions for requested client area
					RECT rc = { 0, 0, mWidth, mHeight };
					AdjustWindowRect(&rc, dwStyle, false);

					// clamp window dimensions to screen size
					outerw = (rc.right-rc.left < screenw)? rc.right-rc.left : screenw;
					outerh = (rc.bottom-rc.top < screenh)? rc.bottom-rc.top : screenh;
				}

				// center window if given negative coordinates
				if (mLeft < 0)
					mLeft = (screenw - outerw) / 2;
				if (mTop < 0)
					mTop = (screenh - outerh) / 2;

				// keep window contained in visible screen area
				if (mLeft > screenw - outerw)
					mLeft = screenw - outerw;
				if (mTop > screenh - outerh)
					mTop = screenh - outerh;
			}

			// register class and create window
			WNDCLASS wc = { CS_OWNDC, WndProc, 0, 0, hInst,
				LoadIcon(NULL, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW),
				(HBRUSH)GetStockObject(BLACK_BRUSH), NULL, "OgreGLWindow" };
			RegisterClass(&wc);

			// Pass pointer to self as WM_CREATE parameter
			mHWnd = CreateWindowEx(dwStyleEx, "OgreGLWindow", title.c_str(),
				dwStyle, mLeft, mTop, outerw, outerh, parent, 0, hInst, this);

			StringUtil::StrStreamType str;
			str << "Created Win32Window '"
				<< mName << "' : " << mWidth << "x" << mHeight
				<< ", " << mColourDepth << "bpp";
			LogManager::getSingleton().logMessage(LML_NORMAL, str.str());

			if (mIsFullScreen)
			{
				DEVMODE dm;
				dm.dmSize = sizeof(DEVMODE);
				dm.dmBitsPerPel = mColourDepth;
				dm.dmPelsWidth = mWidth;
				dm.dmPelsHeight = mHeight;
				dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
				if (mDisplayFrequency)
				{
					dm.dmDisplayFrequency = mDisplayFrequency;
					dm.dmFields |= DM_DISPLAYFREQUENCY;
				}
				if (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
					LogManager::getSingleton().logMessage(LML_CRITICAL, "ChangeDisplaySettings failed");
			}
		}

		HDC old_hdc = wglGetCurrentDC();
		HGLRC old_context = wglGetCurrentContext();

		RECT rc;
		// top and left represent outer window position
		GetWindowRect(mHWnd, &rc);
		mTop = rc.top;
		mLeft = rc.left;
		// width and height represent drawable area only
		GetClientRect(mHWnd, &rc);
		mWidth = rc.right;
		mHeight = rc.bottom;

		mHDC = GetDC(mHWnd);

		if (!mGLSupport.selectPixelFormat(mHDC, mColourDepth, fsaa))
		{
			if (fsaa == 0)
				OGRE_EXCEPT(0, "selectPixelFormat failed", "Win32Window::create");

			LogManager::getSingleton().logMessage(LML_NORMAL, "FSAA level not supported, falling back");
			if (!mGLSupport.selectPixelFormat(mHDC, mColourDepth, 0))
				OGRE_EXCEPT(0, "selectPixelFormat failed", "Win32Window::create");
		}

		mGlrc = wglCreateContext(mHDC);
		if (!mGlrc)
			OGRE_EXCEPT(0, "wglCreateContext", "Win32Window::create");
		if (!wglMakeCurrent(mHDC, mGlrc))
			OGRE_EXCEPT(0, "wglMakeCurrent", "Win32Window::create");

		// Don't use wglew as if this is the first window, we won't have initialised yet
		PFNWGLSWAPINTERVALEXTPROC _wglSwapIntervalEXT = 
			(PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
		if (_wglSwapIntervalEXT)
			_wglSwapIntervalEXT(vsync? 1 : 0);

        if (old_context)
        {
            // Restore old context
		    if (!wglMakeCurrent(old_hdc, old_context))
			    OGRE_EXCEPT(0, "wglMakeCurrent() failed", "Win32Window::create");

            // Share lists with old context
		    if (!wglShareLists(old_context, mGlrc))
			    OGRE_EXCEPT(0, "wglShareLists() failed", " Win32Window::create");
        }

		// Create RenderSystem context
		mContext = new Win32Context(mHDC, mGlrc);

		mActive = true;
	}

	void Win32Window::destroy(void)
	{
		if (!mHWnd)
			return;

		// Unregister and destroy OGRE GLContext
		delete mContext;

		if (mGlrc)
		{
			wglDeleteContext(mGlrc);
			mGlrc = 0;
		}
		if (!mIsExternal)
		{
			if (mIsFullScreen)
				ChangeDisplaySettings(NULL, 0);
			DestroyWindow(mHWnd);
		}
		mActive = false;
		mHDC = 0; // no release thanks to CS_OWNDC wndclass style
		mHWnd = 0;
	}

	bool Win32Window::isVisible() const
	{
		return (mHWnd && !IsIconic(mHWnd));
	}

	bool Win32Window::isClosed() const
	{
		return mClosed;
	}

	void Win32Window::reposition(int left, int top)
	{
		if (mHWnd && !mIsFullScreen)
		{
			SetWindowPos(mHWnd, 0, left, top, 0, 0,
				SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
		}
	}

	void Win32Window::resize(unsigned int width, unsigned int height)
	{
		if (mHWnd && !mIsFullScreen)
		{
			RECT rc = { 0, 0, width, height };
			AdjustWindowRect(&rc, GetWindowLong(mHWnd, GWL_STYLE), false);
			width = rc.right - rc.left;
			height = rc.bottom - rc.top;
			SetWindowPos(mHWnd, 0, 0, 0, width, height,
				SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
		}
	}

	void Win32Window::windowMovedOrResized()
	{
		if (!isVisible())
			return;

		RECT rc;
		// top and left represent outer window position
		GetWindowRect(mHWnd, &rc);
		mTop = rc.top;
		mLeft = rc.left;
		// width and height represent drawable area only
		GetClientRect(mHWnd, &rc);

		if (mWidth == rc.right && mHeight == rc.bottom)
			return;

		mWidth = rc.right;
		mHeight = rc.bottom;

		// Notify viewports of resize
		ViewportList::iterator it, itend;
		itend = mViewportList.end();
		for( it = mViewportList.begin(); it != itend; ++it )
			(*it).second->_updateDimensions();
	}

	void Win32Window::swapBuffers(bool waitForVSync)
	{
		SwapBuffers(mHDC);
	}

	void Win32Window::writeContentsToFile(const String& filename)
	{
		ImageCodec::ImageData *imgData = new ImageCodec::ImageData();
		imgData->width = mWidth;
		imgData->height = mHeight;
		imgData->depth = 1;
		imgData->format = PF_BYTE_RGB;

		// Allocate buffer 
		uchar* pBuffer = new uchar[mWidth * mHeight * 3];

		// Read pixels
		// I love GL: it does all the locking & colour conversion for us
		glReadPixels(0,0, mWidth-1, mHeight-1, GL_RGB, GL_UNSIGNED_BYTE, pBuffer);

		// Wrap buffer in a memory stream
		DataStreamPtr stream(new MemoryDataStream(pBuffer, mWidth * mHeight * 3, false));

		// Need to flip the read data over in Y though
		Image img;
		img.loadRawData(stream, mWidth, mHeight, imgData->format );
		img.flipAroundX();

		MemoryDataStreamPtr streamFlipped(new MemoryDataStream(img.getData(), stream->size(), false));

		// Get codec 
		size_t pos = filename.find_last_of(".");
		String extension;
		if( pos == String::npos )
			OGRE_EXCEPT(
			Exception::ERR_INVALIDPARAMS, 
			"Unable to determine image type for '" + filename + "' - invalid extension.",
			"Win32Window::writeContentsToFile" );

		while( pos != filename.length() - 1 )
			extension += filename[++pos];

		// Get the codec
		Codec * pCodec = Codec::getCodec(extension);

		// Write out
		Codec::CodecDataPtr ptr(imgData);
		pCodec->codeToFile(streamFlipped, filename, ptr);

		delete [] pBuffer;
	}

	// Window procedure callback
	// This is a static member, so applies to all windows but we store the
	// Win32Window instance in the window data GetWindowLog/SetWindowLog
	LRESULT Win32Window::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{

		if (uMsg == WM_CREATE)
		{
			// Store pointer to Win32Window in user data area
			SetWindowLongPtr(hWnd, GWLP_USERDATA,
				(LONG)(((LPCREATESTRUCT)lParam)->lpCreateParams));
			return 0;
		}

		// look up window instance
		// note: it is possible to get a WM_SIZE before WM_CREATE
		Win32Window* win = (Win32Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (!win)
			return DefWindowProc(hWnd, uMsg, wParam, lParam);

		switch( uMsg )
		{
		case WM_ACTIVATE:
			if (win->mIsFullScreen)
			{
				if (LOWORD(wParam) == WA_INACTIVE)
				{
					win->mActive = false;
					ChangeDisplaySettings(NULL, 0);
					ShowWindow(hWnd, SW_SHOWMINNOACTIVE);
				}
				else
				{
					win->mActive = true;
					ShowWindow(hWnd, SW_SHOWNORMAL);

					DEVMODE dm;
					dm.dmSize = sizeof(DEVMODE);
					dm.dmBitsPerPel = win->mColourDepth;
					dm.dmPelsWidth = win->mWidth;
					dm.dmPelsHeight = win->mHeight;
					dm.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
					if (win->mDisplayFrequency)
					{
						dm.dmDisplayFrequency = win->mDisplayFrequency;
						dm.dmFields |= DM_DISPLAYFREQUENCY;
					}
					ChangeDisplaySettings(&dm, CDS_FULLSCREEN);
				}
			}
			break;

		case WM_ENTERSIZEMOVE:
			win->mSizing = true;
			break;

		case WM_EXITSIZEMOVE:
			win->windowMovedOrResized();
			win->mSizing = false;
			break;

		case WM_MOVE:
		case WM_SIZE:
			if (!win->mSizing)
				win->windowMovedOrResized();
			break;

		case WM_GETMINMAXINFO:
			// Prevent the window from going smaller than some minimu size
			((MINMAXINFO*)lParam)->ptMinTrackSize.x = 100;
			((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100;
			break;

		case WM_CLOSE:
			win->destroy(); // will call DestroyWindow
			win->mClosed = true;
			return 0;
		}

		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}

	void Win32Window::getCustomAttribute( const String& name, void* pData )
	{
		if( name == "GLCONTEXT" ) {
			*static_cast<GLContext**>(pData) = mContext;
			return;
		} else if( name == "HWND" )
		{
			HWND *pHwnd = (HWND*)pData;
			*pHwnd = getWindowHandle();
			return;
		} 
	}

}
