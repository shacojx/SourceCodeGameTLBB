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
#include "OgreD3D9RenderWindow.h"
#include "OgreLogManager.h"
#include "OgreViewport.h"
#include "OgreException.h"
#include "OgreD3D9RenderSystem.h"
#include "OgreRenderSystem.h"
#include "OgreBitwise.h"
#include "OgreImageCodec.h"
#include "OgreStringConverter.h"

#include "OgreNoMemoryMacros.h"
#include <d3d9.h>
#include "OgreMemoryMacros.h"
#include "OgreRoot.h"

namespace Ogre
{
	// Window procedure callback
	// This is a static member, so applies to all windows but we store the
	// D3D9RenderWindow instance in the window data GetWindowLog/SetWindowLog
	LRESULT D3D9RenderWindow::WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
	{
		if (uMsg == WM_CREATE)
		{
			// copy D3D9RenderWindow* from createwindow param to userdata slot
			SetWindowLong(hWnd, GWL_USERDATA,
				(LONG)(((LPCREATESTRUCT)lParam)->lpCreateParams));
			return 0;
		}

		D3D9RenderWindow* win =
			(D3D9RenderWindow*)GetWindowLong(hWnd, GWL_USERDATA);

		if (!win)
			return DefWindowProc(hWnd, uMsg, wParam, lParam);

		switch( uMsg )
		{
		case WM_ACTIVATE:
			win->mActive = (LOWORD(wParam) != WA_INACTIVE);
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
			win->destroy(); // cleanup and call DestroyWindow
			win->mClosed = true;
			return 0;
		}

		return DefWindowProc( hWnd, uMsg, wParam, lParam );
	}

	D3D9RenderWindow::D3D9RenderWindow(HINSTANCE instance, D3D9Driver *driver, LPDIRECT3DDEVICE9 deviceIfSwapChain):
	mInstance(instance),
		mDriver(driver)
	{
		mIsFullScreen = false;
		mIsSwapChain = (deviceIfSwapChain != NULL);
		mIsExternal = false;
		mHWnd = 0;
		mActive = false;
		mSizing = false;
		mClosed = false;
		mDisplayFrequency = 0;
	}

	D3D9RenderWindow::~D3D9RenderWindow()
	{
		destroy();
	}

	bool D3D9RenderWindow::_checkMultiSampleQuality(D3DMULTISAMPLE_TYPE type, DWORD *outQuality, D3DFORMAT format, UINT adapterNum, D3DDEVTYPE deviceType, BOOL fullScreen)
	{
		LPDIRECT3D9 pD3D = mDriver->getD3D();

		if (SUCCEEDED(pD3D->CheckDeviceMultiSampleType(
			adapterNum, 
			deviceType, format, 
			fullScreen, type, outQuality)))
			return true;
		else
			return false;
	}

	void D3D9RenderWindow::create(const String& name, unsigned int width, unsigned int height,
		bool fullScreen, const NameValuePairList *miscParams)
	{
		HINSTANCE hInst = mInstance;
		D3D9Driver* driver = mDriver;

		HWND parentHWnd = 0;
		HWND externalHandle = 0;
		mFSAAType = D3DMULTISAMPLE_NONE;
		mFSAAQuality = 0;
		mVSync = false;
		String title = name;
		unsigned int colourDepth = 32;
		int left = -1; // Defaults to screen center
		int top = -1; // Defaults to screen center
		bool depthBuffer = true;
		String border;
		bool outerSize = false;
		mUseNVPerfHUD = false;

		if(miscParams)
		{
			// Get variable-length params
			NameValuePairList::const_iterator opt;
			// left (x)
			opt = miscParams->find("left");
			if(opt != miscParams->end())
				left = StringConverter::parseInt(opt->second);
			// top (y)
			opt = miscParams->find("top");
			if(opt != miscParams->end())
				top = StringConverter::parseInt(opt->second);
			// Window title
			opt = miscParams->find("title");
			if(opt != miscParams->end())
				title = opt->second;
			// parentWindowHandle		-> parentHWnd
			opt = miscParams->find("parentWindowHandle");
			if(opt != miscParams->end())
				parentHWnd = (HWND)StringConverter::parseUnsignedInt(opt->second);
			// externalWindowHandle		-> externalHandle
			opt = miscParams->find("externalWindowHandle");
			if(opt != miscParams->end())
				externalHandle = (HWND)StringConverter::parseUnsignedInt(opt->second);
			// vsync	[parseBool]
			opt = miscParams->find("vsync");
			if(opt != miscParams->end())
				mVSync = StringConverter::parseBool(opt->second);
			// displayFrequency
			opt = miscParams->find("displayFrequency");
			if(opt != miscParams->end())
				mDisplayFrequency = StringConverter::parseUnsignedInt(opt->second);
			// colourDepth
			opt = miscParams->find("colourDepth");
			if(opt != miscParams->end())
				colourDepth = StringConverter::parseUnsignedInt(opt->second);
			// depthBuffer [parseBool]
			opt = miscParams->find("depthBuffer");
			if(opt != miscParams->end())
				depthBuffer = StringConverter::parseBool(opt->second);
			// FSAA type
			opt = miscParams->find("FSAA");
			if(opt != miscParams->end())
				mFSAAType = (D3DMULTISAMPLE_TYPE)StringConverter::parseUnsignedInt(opt->second);
			// FSAA quality
			opt = miscParams->find("FSAAQuality");
			if(opt != miscParams->end())
				mFSAAQuality = StringConverter::parseUnsignedInt(opt->second);
			// window border style
			opt = miscParams->find("border");
			if(opt != miscParams->end())
				border = opt->second;
			// set outer dimensions?
			opt = miscParams->find("outerDimensions");
			if(opt != miscParams->end())
				outerSize = StringConverter::parseBool(opt->second);
			// NV perf HUD?
			opt = miscParams->find("useNVPerfHUD");
			if(opt != miscParams->end())
				mUseNVPerfHUD = StringConverter::parseBool(opt->second);
			 
		}

		// Destroy current window if any
		if( mHWnd )
			destroy();


		if (!externalHandle)
		{
			DWORD dwStyle = WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
			RECT rc;

			mWidth = width;
			mHeight = height;
			mTop = top;
			mLeft = left;

			if (!fullScreen)
			{
				if (parentHWnd)
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

				if (!outerSize)
				{
					// Calculate window dimensions required
					// to get the requested client area
					SetRect(&rc, 0, 0, mWidth, mHeight);
					AdjustWindowRect(&rc, dwStyle, false);
					mWidth = rc.right - rc.left;
					mHeight = rc.bottom - rc.top;

					// Clamp width and height to the desktop dimensions
					int screenw = GetSystemMetrics(SM_CXSCREEN);
					int screenh = GetSystemMetrics(SM_CYSCREEN);
					if ((int)mWidth > screenw)
						mWidth = screenw;
					if ((int)mHeight > screenh)
						mHeight = screenh;
					if (mLeft < 0)
						mLeft = (screenw - mWidth) / 2;
					if (mTop < 0)
						mTop = (screenh - mHeight) / 2;
				}
			}
			else
			{
				dwStyle |= WS_POPUP;
				mTop = mLeft = 0;
			}

			// Register the window class
			// NB allow 4 bytes of window data for D3D9RenderWindow pointer
			WNDCLASS wc = { 0, WndProc, 0, 0, hInst,
				LoadIcon(0, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW),
				(HBRUSH)GetStockObject(BLACK_BRUSH), 0, "OgreD3D9Wnd" };
			RegisterClass(&wc);

			// Create our main window
			// Pass pointer to self
			mIsExternal = false;
			mHWnd = CreateWindow("OgreD3D9Wnd", title.c_str(), dwStyle,
				mLeft, mTop, mWidth, mHeight, parentHWnd, 0, hInst, this);
		}
		else
		{
			mHWnd = externalHandle;
			mIsExternal = true;
		}

		RECT rc;
		// top and left represent outer window coordinates
		GetWindowRect(mHWnd, &rc);
		mTop = rc.top;
		mLeft = rc.left;
		// width and height represent interior drawable area
		GetClientRect(mHWnd, &rc);
		mWidth = rc.right;
		mHeight = rc.bottom;

		mName = name;
		mIsDepthBuffered = depthBuffer;
		mIsFullScreen = fullScreen;
		mColourDepth = colourDepth;

		StringUtil::StrStreamType str;
		str << "D3D9 : Created D3D9 Rendering Window '"
			<< mName << "' : " << mWidth << "x" << mHeight 
			<< ", " << mColourDepth << "bpp";
		LogManager::getSingleton().logMessage(
			LML_NORMAL, str.str());

		createD3DResources();

		mActive = true;
	}

	void D3D9RenderWindow::createD3DResources(void)
	{
		// access device via driver
		LPDIRECT3DDEVICE9 mpD3DDevice = mDriver->getD3DDevice();

		if (mIsSwapChain && !mpD3DDevice)
		{
			OGRE_EXCEPT(Exception::ERR_INTERNAL_ERROR, 
				"Secondary window has not been given the device from the primary!",
				"D3D9RenderWindow::createD3DResources");
		}

		// Set up the presentation parameters
		HRESULT hr;
		LPDIRECT3D9 pD3D = mDriver->getD3D();
		D3DDEVTYPE devType = D3DDEVTYPE_HAL;

		// triple buffer if VSync is on
		const UINT desiredBackBufferCount = mVSync ? 2 : 1;

		ZeroMemory( &md3dpp, sizeof(D3DPRESENT_PARAMETERS) );
		md3dpp.Windowed					= !mIsFullScreen;
		md3dpp.SwapEffect				= D3DSWAPEFFECT_DISCARD;
		md3dpp.BackBufferCount			= desiredBackBufferCount;
		md3dpp.EnableAutoDepthStencil	= mIsDepthBuffered;
		md3dpp.hDeviceWindow			= mHWnd;
		md3dpp.BackBufferWidth			= mWidth;
		md3dpp.BackBufferHeight			= mHeight;
		md3dpp.FullScreen_RefreshRateInHz = mIsFullScreen ? mDisplayFrequency : 0;
		

		if (mVSync)
		{
			md3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
		}
		else
		{
			// NB not using vsync in windowed mode in D3D9 can cause jerking at low 
			// frame rates no matter what buffering modes are used (odd - perhaps a
			// timer issue in D3D9 since GL doesn't suffer from this) 
			// low is < 200fps in this context
			if (!mIsFullScreen)
			{
				LogManager::getSingleton().logMessage("D3D9 : WARNING - "
					"disabling VSync in windowed mode can cause timing issues at lower "
					"frame rates, turn VSync on if you observe this problem.");
			}
			md3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		}

		md3dpp.BackBufferFormat		= D3DFMT_R5G6B5;
		if( mColourDepth > 16 )
			md3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;

        // Check for depth-stencil format
		D3D9RenderSystem* rs = static_cast<D3D9RenderSystem*>(Root::getSingleton().getRenderSystem());
        if (mIsFullScreen)
        {
            // If full screen, adapter display format will same as back buffer format
            md3dpp.AutoDepthStencilFormat =
                rs->_getDepthStencilFormatFor(md3dpp.BackBufferFormat, md3dpp.BackBufferFormat);
        }
        else
        {
            // Check with current adapter display format
            md3dpp.AutoDepthStencilFormat =
                rs->_getDepthStencilFormatFor(md3dpp.BackBufferFormat);
        }

		md3dpp.MultiSampleType = mFSAAType;
		md3dpp.MultiSampleQuality = (mFSAAType == D3DMULTISAMPLE_NONE) ? 0 : mFSAAQuality;

		if (mIsSwapChain)
		{
			// Sets desired back buffer count
			md3dpp.BackBufferCount = desiredBackBufferCount;
			// Create swap chain			
			hr = mpD3DDevice->CreateAdditionalSwapChain(
				&md3dpp, &mpSwapChain);
			if (FAILED(hr) && md3dpp.BackBufferCount != desiredBackBufferCount)
			{
				// Try a second time, may fail the first time due to back buffer count,
				// which will be corrected by the runtime
				hr = mpD3DDevice->CreateAdditionalSwapChain(
					&md3dpp, &mpSwapChain);
			}
			if (FAILED(hr))
			{
				OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
					"Unable to create an additional swap chain",
					"D3D9RenderWindow::createD3DResources");
			}
			// Store references to buffers for convenience
			mpSwapChain->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &mpRenderSurface );
			// Additional swap chains need their own depth buffer
			// to support resizing them
			if (mIsDepthBuffered) 
			{
				hr = mpD3DDevice->CreateDepthStencilSurface(
					mWidth, mHeight,
					md3dpp.AutoDepthStencilFormat,
					md3dpp.MultiSampleType,
					md3dpp.MultiSampleQuality, 
					(md3dpp.Flags & D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL),
					&mpRenderZBuffer, NULL
					);

				if (FAILED(hr)) 
				{
					OGRE_EXCEPT(Exception::ERR_RENDERINGAPI_ERROR, 
						"Unable to create a depth buffer for the swap chain",
						"D3D9RenderWindow::createD3DResources");

				}
			} 
			else 
			{
				mpRenderZBuffer = 0;
			}
			// release immediately so we don't hog them
			mpRenderSurface->Release();
			// We'll need the depth buffer for rendering the swap chain
			//mpRenderZBuffer->Release();
		}
		else
		{
			if (!mpD3DDevice)
			{
				// We haven't created the device yet, this must be the first time
				D3DCAPS9 caps;
				hr = D3DERR_NOTAVAILABLE;

				// Do we want to preserve the FPU mode? Might be useful for scientific apps
				DWORD extraFlags = 0;
				ConfigOptionMap& options = Root::getSingleton().getRenderSystem()->getConfigOptions();
				ConfigOptionMap::iterator opti = options.find("Floating-point mode");
				if (opti != options.end() && opti->second.currentValue == "Consistent")
					extraFlags |= D3DCREATE_FPU_PRESERVE;

				// Set default settings (use the one Ogre discovered as a default)
				UINT adapterToUse = mDriver->getAdapterNumber();

				if (0 && mUseNVPerfHUD)
				{
					// Look for 'NVIDIA NVPerfHUD' adapter
					// If it is present, override default settings
					for (UINT adapter=0; adapter < mDriver->getD3D()->GetAdapterCount(); ++adapter)
					{
						D3DADAPTER_IDENTIFIER9 identifier;
						HRESULT res;
						res = mDriver->getD3D()->GetAdapterIdentifier(adapter,0,&identifier);
						if (strstr(identifier.Description,"NVPerfHUD") != 0)
						{
							adapterToUse = adapter;
							devType = D3DDEVTYPE_REF;
							break;
						}
					}
				}

				bool requireVertexShader = false;
				bool tryHardwareVertexProcessing = true;

                opti = options.find("Vertex Processing");
				if (opti != options.end())
                {
                    if (opti->second.currentValue == "Require Vertex Shader")
                        requireVertexShader = true;
                    else if (opti->second.currentValue == "Software")
                        tryHardwareVertexProcessing = false;
                }

				if (requireVertexShader && tryHardwareVertexProcessing)
				{
					// Try to create hardware vertex processing only when the driver report
					// that it's support programmable vertex shader in hardware.
					pD3D->GetDeviceCaps(adapterToUse, devType, &caps);
					if ((caps.VertexShaderVersion & 0x0000FF00) == 0)
						tryHardwareVertexProcessing = false;
				}

				if (tryHardwareVertexProcessing)
				{
                    LogManager::getSingleton().logMessage(
                        "Try to create Direct3D9 device with hardware vertex processing");
					// Sets desired back buffer count
					md3dpp.BackBufferCount = desiredBackBufferCount;
					hr = pD3D->CreateDevice( adapterToUse, devType, mHWnd,
						D3DCREATE_HARDWARE_VERTEXPROCESSING | extraFlags, &md3dpp, &mpD3DDevice );
					if (FAILED(hr) && md3dpp.BackBufferCount != desiredBackBufferCount)
					{
						// Try a second time, may fail the first time due to back buffer count,
						// which will be corrected down to 1 by the runtime
						hr = pD3D->CreateDevice( adapterToUse, devType, mHWnd,
							D3DCREATE_HARDWARE_VERTEXPROCESSING | extraFlags, &md3dpp, &mpD3DDevice );
					}

					if (SUCCEEDED(hr) && requireVertexShader)
					{
						// Strict check the programmable vertex shader capability.
						// Note: Even though driver reported that it's support programmable vertex
						// shader in hardware, but the created device maybe doesn't support this
						// capability for some reason.
						mpD3DDevice->GetDeviceCaps(&caps);
						if ((caps.VertexShaderVersion & 0x0000FF00) == 0)
						{
							SAFE_RELEASE( mpD3DDevice );
							hr = D3DERR_NOTAVAILABLE;
						}
					}
				}

				// Just create with the software vertex processing, Direct3D9 will always
				// support programmable vertex shader in software vertex processing.
				// We never try to create mixed vertex processing because the it support
				// programmable vertex shader only when hardware vertex processing supported.
				if( FAILED( hr ) )
				{
                    LogManager::getSingleton().logMessage(
                        "Try to create Direct3D9 device with software vertex processing");
					// Sets desired back buffer count
					md3dpp.BackBufferCount = desiredBackBufferCount;
					hr = pD3D->CreateDevice( adapterToUse, devType, mHWnd,
						D3DCREATE_SOFTWARE_VERTEXPROCESSING | extraFlags, &md3dpp, &mpD3DDevice );
					if (FAILED(hr) && md3dpp.BackBufferCount != desiredBackBufferCount)
					{
						// Try a second time, may fail the first time due to back buffer count,
						// which will be corrected down to 1 by the runtime
						hr = pD3D->CreateDevice( adapterToUse, devType, mHWnd,
							D3DCREATE_SOFTWARE_VERTEXPROCESSING | extraFlags, &md3dpp, &mpD3DDevice );
					}
				}

				// TODO: make this a bit better e.g. go from pure vertex processing to software
				if( FAILED( hr ) )
				{
					destroy();
					OGRE_EXCEPT( hr, "Failed to create Direct3D9 Device: " + 
						Root::getSingleton().getErrorDescription(hr), 
						"D3D9RenderWindow::createD3DResources" );
				}
			}
			// update device in driver
			mDriver->setD3DDevice( mpD3DDevice );
			// Store references to buffers for convenience
			mpD3DDevice->GetRenderTarget( 0, &mpRenderSurface );
			mpD3DDevice->GetDepthStencilSurface( &mpRenderZBuffer );
			// release immediately so we don't hog them
			mpRenderSurface->Release();
			mpRenderZBuffer->Release();
		}

	}

	void D3D9RenderWindow::destroyD3DResources()
	{
		mpRenderSurface = 0;
		if (mIsSwapChain)
		{
			SAFE_RELEASE(mpRenderZBuffer);
			SAFE_RELEASE(mpSwapChain);
		}
		else
		{
			// ignore depth buffer, access device through driver
			mpRenderZBuffer = 0;
		}
	}

	void D3D9RenderWindow::destroy()
	{
		destroyD3DResources();

		if (mHWnd && !mIsExternal)
			DestroyWindow(mHWnd);
		mHWnd = 0;
		mActive = false;
	}

	bool D3D9RenderWindow::isVisible() const
	{
		return (mHWnd && !IsIconic(mHWnd));
	}

	void D3D9RenderWindow::reposition(int top, int left)
	{
		if (mHWnd && !mIsFullScreen)
		{
			SetWindowPos(mHWnd, 0, top, left, 0, 0,
				SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
		}
	}

	void D3D9RenderWindow::resize(unsigned int width, unsigned int height)
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

	void D3D9RenderWindow::windowMovedOrResized()
	{
		if (!mHWnd || IsIconic(mHWnd))
			return;

		RECT rc;
		// top and left represent outer window position
		GetWindowRect(mHWnd, &rc);
		mTop = rc.top;
		mLeft = rc.left;
		// width and height represent drawable area only
		GetClientRect(mHWnd, &rc);
		unsigned int width = rc.right;
		unsigned int height = rc.bottom;
		if (mWidth == width && mHeight == height)
			return;

		if (mIsSwapChain) 
		{

			D3DPRESENT_PARAMETERS pp = md3dpp;

			pp.BackBufferWidth = width;
			pp.BackBufferHeight = height;

			SAFE_RELEASE( mpRenderZBuffer );
			SAFE_RELEASE( mpSwapChain );

			HRESULT hr = mDriver->getD3DDevice()->CreateAdditionalSwapChain(
				&pp,
				&mpSwapChain);

			if (FAILED(hr)) 
			{
				StringUtil::StrStreamType str;
				str << "D3D9RenderWindow: failed to reset device to new dimensions << "
					<< width << " x " << height << ". Trying to recover.";
				LogManager::getSingleton().logMessage(LML_CRITICAL, str.str());

				// try to recover
				hr = mDriver->getD3DDevice()->CreateAdditionalSwapChain(
					&md3dpp,
					&mpSwapChain);

				if (FAILED(hr))
					OGRE_EXCEPT( hr, "Reset window to last size failed", "D3D9RenderWindow::resize" );

			}		
			else 
			{
				md3dpp = pp;

				mWidth = width;
				mHeight = height;

				hr = mpSwapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &mpRenderSurface);
				hr = mDriver->getD3DDevice()->CreateDepthStencilSurface(
					mWidth, mHeight,
					md3dpp.AutoDepthStencilFormat,
					md3dpp.MultiSampleType,
					md3dpp.MultiSampleQuality, 
					(md3dpp.Flags & D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL),
					&mpRenderZBuffer, NULL
					);

				if (FAILED(hr)) 
				{
					OGRE_EXCEPT( hr, "Failed to create depth stencil surface for Swap Chain", "D3D9RenderWindow::resize" );
				}

				mpRenderSurface->Release();
			}
		}
		// primary windows must reset the device
		else 
		{
			md3dpp.BackBufferWidth = mWidth = width;
			md3dpp.BackBufferHeight = mHeight = height;
			static_cast<D3D9RenderSystem*>(
				Root::getSingleton().getRenderSystem())->_notifyDeviceLost();
		}

		// Notify viewports of resize
		ViewportList::iterator it = mViewportList.begin();
		while( it != mViewportList.end() )
			(*it++).second->_updateDimensions();
	}

	void D3D9RenderWindow::swapBuffers( bool waitForVSync )
	{
		// access device through driver
		LPDIRECT3DDEVICE9 mpD3DDevice = mDriver->getD3DDevice();
		if( mpD3DDevice )
		{
			HRESULT hr;
			if (mIsSwapChain)
			{
				hr = mpSwapChain->Present(NULL, NULL, NULL, NULL, 0);
			}
			else
			{
				hr = mpD3DDevice->Present( NULL, NULL, 0, NULL );
			}
			if( D3DERR_DEVICELOST == hr )
			{
				static_cast<D3D9RenderSystem*>(
					Root::getSingleton().getRenderSystem())->_notifyDeviceLost();
			}
			else if( FAILED(hr) )
				OGRE_EXCEPT( hr, "Error Presenting surfaces", "D3D9RenderWindow::swapBuffers" );
		}
	}

	void D3D9RenderWindow::getCustomAttribute( const String& name, void* pData )
	{
		// Valid attributes and their equvalent native functions:
		// D3DDEVICE			: getD3DDevice
		// HWND					: getWindowHandle

		if( name == "D3DDEVICE" )
		{
			LPDIRECT3DDEVICE9 *pDev = (LPDIRECT3DDEVICE9*)pData;
			*pDev = getD3DDevice();
			return;
		}
		else if( name == "HWND" )
		{
			HWND *pHwnd = (HWND*)pData;
			*pHwnd = getWindowHandle();
			return;
		}
		else if( name == "isTexture" )
		{
			bool *b = reinterpret_cast< bool * >( pData );
			*b = false;

			return;
		}
		else if( name == "D3DZBUFFER" )
		{
			LPDIRECT3DSURFACE9 *pSurf = (LPDIRECT3DSURFACE9*)pData;
			*pSurf = mpRenderZBuffer;
			return;
		}
		else if( name == "DDBACKBUFFER" )
		{
			LPDIRECT3DSURFACE9 *pSurf = (LPDIRECT3DSURFACE9*)pData;
			*pSurf = mpRenderSurface;
			return;
		}
		else if( name == "DDFRONTBUFFER" )
		{
			LPDIRECT3DSURFACE9 *pSurf = (LPDIRECT3DSURFACE9*)pData;
			*pSurf = mpRenderSurface;
			return;
		}
	}

	void D3D9RenderWindow::writeContentsToFile(const String& filename)
	{
		HRESULT hr;
		LPDIRECT3DSURFACE9 pTempSurf = NULL;
		D3DSURFACE_DESC desc;
		D3DDISPLAYMODE dm;
        LPDIRECT3DSWAPCHAIN9 pSwapChain = NULL;

		// access device through driver
		LPDIRECT3DDEVICE9 mpD3DDevice = mDriver->getD3DDevice();

        if (mIsSwapChain)
        {
            pSwapChain = mpSwapChain;
        }
        else
        {
            if (FAILED(hr = mpD3DDevice->GetSwapChain(0, &pSwapChain)))
                OGRE_EXCEPT(hr, "Can't get main swap chain!", "D3D9RenderWindow::writeContentsToFile");
            pSwapChain->Release();
        }

		// get display dimensions
		// this will be the dimensions of the front buffer
		if (FAILED(hr = pSwapChain->GetDisplayMode(&dm)))
			OGRE_EXCEPT(hr, "Can't get display mode!", "D3D9RenderWindow::writeContentsToFile");

		desc.Width = dm.Width;
		desc.Height = dm.Height;
		desc.Format = D3DFMT_A8R8G8B8;
		if (FAILED(hr = mpD3DDevice->CreateOffscreenPlainSurface(
			desc.Width, 
			desc.Height, 
			desc.Format, 
			D3DPOOL_SYSTEMMEM, 
			&pTempSurf, 
			NULL)))
		{
			OGRE_EXCEPT(hr, "Cannot create offscreen buffer!", "D3D9RenderWindow::writeContentsToFile");
		}

		if (FAILED(hr = pSwapChain->GetFrontBufferData(pTempSurf)))
		{
			SAFE_RELEASE(pTempSurf);
			OGRE_EXCEPT(hr, "Can't get front buffer!", "D3D9RenderWindow::writeContentsToFile");
		}
		
		D3DLOCKED_RECT lockedRect;
		if(mIsFullScreen)
		{
			if (FAILED(hr = pTempSurf->LockRect(&lockedRect, NULL, D3DLOCK_READONLY | D3DLOCK_NOSYSLOCK)))
			{
                SAFE_RELEASE(pTempSurf);
				OGRE_EXCEPT(hr, "can't lock rect!", "D3D9RenderWindow::writeContentsToFile");
			} 
		}
		else
		{
			RECT srcRect;
			GetClientRect(mHWnd, &srcRect);
            ClientToScreen(mHWnd, LPPOINT(&srcRect));
            srcRect.right += srcRect.left;
            srcRect.bottom += srcRect.top;

			desc.Width = srcRect.right - srcRect.left;
			desc.Height = srcRect.bottom - srcRect.top;

			if (FAILED(hr = pTempSurf->LockRect(&lockedRect, &srcRect, D3DLOCK_READONLY | D3DLOCK_NOSYSLOCK)))
			{
                SAFE_RELEASE(pTempSurf);
				OGRE_EXCEPT(hr, "can't lock rect!", "D3D9RenderWindow::writeContentsToFile");
			} 
		}

        // Create empty image
        Image image;
        image.loadDynamicImage(0, desc.Width, desc.Height, PF_BYTE_RGB);

        // Gather image data from front buffer
		uint x, y;
		uchar *pData, *pDest;

		pData = (uchar*)lockedRect.pBits;
		pDest = image.getData();
		for (y = 0; y < desc.Height; ++y)
		{
			uchar *pRow = pData;

			for (x = 0; x < desc.Width; ++x)
			{
				switch(desc.Format)
				{
				case D3DFMT_R5G6B5:
					WORD val;

					val = *((WORD*)pRow);
					pRow += 2;

					*pDest++ = Bitwise::convertBitPattern((WORD)val, (WORD)0xF800, (BYTE)0xFF);
					*pDest++ = Bitwise::convertBitPattern((WORD)val, (WORD)0x07E0, (BYTE)0xFF);
					*pDest++ = Bitwise::convertBitPattern((WORD)val, (WORD)0x001F, (BYTE)0xFF);
					break;
				case D3DFMT_A8R8G8B8:
				case D3DFMT_X8R8G8B8:
					// Actual format is BRGA for some reason
					*pDest++ = pRow[2]; // R
					*pDest++ = pRow[1]; // G
					*pDest++ = pRow[0]; // B
					pRow += 4; // skip alpha / dummy
					break;
				case D3DFMT_R8G8B8:
					// Actual format is BRGA for some reason
					*pDest++ = pRow[2]; // R
					*pDest++ = pRow[1]; // G
					*pDest++ = pRow[0]; // B
					pRow += 3; 
					break;
				}


			}
			// increase by one line
			pData += lockedRect.Pitch;
		}

        // Unlock and release offscreen buffer
        pTempSurf->UnlockRect();
		SAFE_RELEASE(pTempSurf);

		// Write out image
        image.save(filename);
	}
	//-----------------------------------------------------------------------------
	void D3D9RenderWindow::update(bool swap)
	{
		D3D9RenderSystem* rs = static_cast<D3D9RenderSystem*>(
			Root::getSingleton().getRenderSystem());

		// access device through driver
		LPDIRECT3DDEVICE9 mpD3DDevice = mDriver->getD3DDevice();

		if (rs->isDeviceLost())
		{
			// Test the cooperative mode first
			HRESULT hr = mpD3DDevice->TestCooperativeLevel();
			if (hr == D3DERR_DEVICELOST)
			{
				// device lost, and we can't reset
				// can't do anything about it here, wait until we get 
				// D3DERR_DEVICENOTRESET; rendering calls will silently fail until 
				// then (except Present, but we ignore device lost there too)
				mpRenderSurface = 0;
				// need to release if swap chain
				if (!mIsSwapChain)
					mpRenderZBuffer = 0;
				else
					SAFE_RELEASE (mpRenderZBuffer);
				Sleep(50);
				return;
			}
			else
			{
				// device lost, and we can reset
				rs->restoreLostDevice();

				// Still lost?
				if (rs->isDeviceLost())
				{
					// Wait a while
					Sleep(50);
					return;
				}

				if (!mIsSwapChain) 
				{
					// re-qeuery buffers
					mpD3DDevice->GetRenderTarget( 0, &mpRenderSurface );
					mpD3DDevice->GetDepthStencilSurface( &mpRenderZBuffer );
					// release immediately so we don't hog them
					mpRenderSurface->Release();
					mpRenderZBuffer->Release();
				}
				else 
				{
				    // Update dimensions incase changed
		            ViewportList::iterator it = mViewportList.begin();
		            while( it != mViewportList.end() )
			            (*it++).second->_updateDimensions();
					// Actual restoration of surfaces will happen in 
					// D3D9RenderSystem::restoreLostDevice when it calls
					// createD3DResources for each secondary window
				}
			}

		}
		RenderWindow::update(swap);
	}
}
