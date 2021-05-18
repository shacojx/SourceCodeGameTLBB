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
#include "OgreWin32Input8.h"

#ifndef OGRE_NO_DX_INPUT
#ifndef DX7INPUTONLY

#include "OgreRenderWindow.h"
#include "OgreLogManager.h"
#include "OgreException.h"
#include "OgreRoot.h"
#include "OgreRenderSystem.h"
#include "OgreMouseEvent.h"
#include "OgreInputEvent.h"
#include "OgreEventQueue.h"
#include "OgreCursor.h"
#include <dxerr8.h>

#if OGRE_COMPILER == OGRE_COMPILER_MSVC

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

#if defined(_M_IA64)
#pragma section(".base", long, read)
extern "C"
__declspec(allocate(".base"))
const IMAGE_DOS_HEADER __ImageBase;
#else
extern "C"
const IMAGE_DOS_HEADER __ImageBase;
#endif

#endif  // OGRE_COMPILER == OGRE_COMPILER_MSVC

#define DINPUT_BUFFERSIZE  64
//#define DIPROP_BUFFERSIZE 256

namespace Ogre {
    //-----------------------------------------------------------------------
    Win32Input8::Win32Input8() :
		InputReader()
    {
        mlpDI = 0;
        mlpDIKeyboard = 0;
        mlpDIMouse = 0;
		mEventQueue = 0;
		mMouseScale = 0.001;

        memset(mKeyboardBuffer,0,256);
    }
    //-----------------------------------------------------------------------
    Win32Input8::~Win32Input8()
    {
        // Shutdown
        if (mlpDIKeyboard)
        {
            mlpDIKeyboard->Unacquire();
            mlpDIKeyboard->Release();
            mlpDIKeyboard = 0;
        }
        if (mlpDIMouse)
        {
            mlpDIMouse->Unacquire();
            mlpDIMouse->Release();
            mlpDIMouse = 0;
        }
        if (mlpDI)
        {
            mlpDI->Release();
            mlpDI = 0;
        }

    }

    //-----------------------------------------------------------------------
    void Win32Input8::initialiseBufferedKeyboard()
	{

        HRESULT hr;
        LogManager::getSingleton().logMessage("Win32Input8: Establishing keyboard input.");

        // Create keyboard device
        hr = mlpDI->CreateDevice(GUID_SysKeyboard, &mlpDIKeyboard, NULL);


        if (FAILED(hr))
            OGRE_EXCEPT(hr, "Unable to create DirectInput keyboard device.",
                "Win32Input8 - initialise");

        // Set data format
        hr = mlpDIKeyboard->SetDataFormat(&c_dfDIKeyboard);
        if (FAILED(hr))
            OGRE_EXCEPT(hr, "Unable to set DirectInput keyboard device data format.",
                "Win32Input8 - initialise");

        // Make the window grab keyboard behaviour when foreground
        hr = mlpDIKeyboard->SetCooperativeLevel(mHWnd,
                   DISCL_FOREGROUND | DISCL_EXCLUSIVE);
        if (FAILED(hr))
            OGRE_EXCEPT(hr, "Unable to set DirectInput keyboard device co-operative level.",
                "Win32Input8 - initialise");


		// IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
		//
		// DirectInput uses unbuffered I/O (buffer size = 0) by default.
		// If you want to read buffered data, you need to set a nonzero
		// buffer size.
		//
		// Set the buffer size to SAMPLE_BUFFER_SIZE (defined above) elements.
		//
		// The buffer size is a DWORD property associated with the device.
		DIPROPDWORD dipdw;
		dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj        = 0;
		dipdw.diph.dwHow        = DIPH_DEVICE;
		dipdw.dwData            = DINPUT_BUFFERSIZE; // Arbitary buffer size

		hr = mlpDIKeyboard->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph );

        if (FAILED(hr))
            OGRE_EXCEPT(hr, "Unable to create DirectInput keyboard buffer.",
                "Win32Input8 - initialise");

        // Acquire input... we could have lost focus if the
		// user tabbed away during init or perhaps we're in
		// the debugger.  In either case when the input is
		// checked we will try to acquire again.
		hr = mlpDIKeyboard->Acquire();
		if (FAILED(hr) && hr != DIERR_OTHERAPPHASPRIO)
            OGRE_EXCEPT(hr, "Unable to set aquire DirectInput keyboard device.",
                "Win32Input8 - initialise");

        LogManager::getSingleton().logMessage("Win32Input8: Keyboard input established.");
	}

    //-----------------------------------------------------------------------
    void Win32Input8::initialiseImmediateKeyboard()
	{
        HRESULT hr;
        LogManager::getSingleton().logMessage("Win32Input8: Establishing keyboard input.");

        // Create keyboard device
        hr = mlpDI->CreateDevice(GUID_SysKeyboard, &mlpDIKeyboard, NULL);


        if (FAILED(hr))
            OGRE_EXCEPT(hr, "Unable to create DirectInput keyboard device.",
                "Win32Input8 - initialise");

        // Set data format
        hr = mlpDIKeyboard->SetDataFormat(&c_dfDIKeyboard);
        if (FAILED(hr))
            OGRE_EXCEPT(hr, "Unable to set DirectInput keyboard device data format.",
                "Win32Input8 - initialise");

        // Make the window grab keyboard behaviour when foreground
        // NB Keyboard is never exclusive
        hr = mlpDIKeyboard->SetCooperativeLevel(mHWnd,
                   DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
        if (FAILED(hr))
            OGRE_EXCEPT(hr, "Unable to set DirectInput keyboard device co-operative level.",
                "Win32Input8 - initialise");

		// IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
		//
		// DirectInput uses unbuffered I/O (buffer size = 0) by default.
		// If you want to read buffered data, you need to set a nonzero
		// buffer size.
		//
		// Set the buffer size to DINPUT_BUFFERSIZE (defined above) elements.
		//
		// The buffer size is a DWORD property associated with the device.
		DIPROPDWORD dipdw;

		dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj        = 0;
		dipdw.diph.dwHow        = DIPH_DEVICE;
		dipdw.dwData            = DINPUT_BUFFERSIZE ; // Arbitary buffer size

		hr = mlpDIKeyboard->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph ) ;

        if (FAILED(hr))
            OGRE_EXCEPT(hr, "Unable to create DirectInput keyboard buffer.",
                "Win32Input8 - initialise");


        // Acquire input... we could have lost focus if the
		// user tabbed away during init or perhaps we're in
		// the debugger.  In either case when the input is
		// checked we will try to acquire again.        hr = mlpDIKeyboard->Acquire();
		if (FAILED(hr) && hr != DIERR_OTHERAPPHASPRIO)
            OGRE_EXCEPT(hr, "Unable to set aquire DirectInput keyboard device.",
                "Win32Input8 - initialise");

        LogManager::getSingleton().logMessage("Win32Input8: Keyboard input established.");
	}
    //-----------------------------------------------------------------------
    void Win32Input8::initialiseImmediateMouse()
	{
        OgreGuard( "Win32Input8::initialiseImmediateMouse" );

        HRESULT hr;
        DIPROPDWORD dipdw;
        LogManager::getSingleton().logMessage( "Win32Input8: Initializing mouse input in immediate mode." );

        dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
        dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        dipdw.diph.dwObj        = 0;
        dipdw.diph.dwHow        = DIPH_DEVICE;
        dipdw.dwData            = DIPROPAXISMODE_ABS;

        if( /* Create the DI Device. */
            FAILED( hr = mlpDI->CreateDevice( GUID_SysMouse, &mlpDIMouse, NULL ) ) ||
            /* Set the data format so that it knows it's a mouse. */
            FAILED( hr = mlpDIMouse->SetDataFormat( &c_dfDIMouse2 ) ) ||
            /* Absolute mouse input. We can derive the relative input from this. */
            FAILED( hr = mlpDIMouse->SetProperty( DIPROP_AXISMODE, &dipdw.diph ) ) ||
            /* Exclusive when in foreground, steps back when in background. */
            FAILED( hr = mlpDIMouse->SetCooperativeLevel( mHWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE ) ) )
        {
            OGRE_EXCEPT( hr, "Unable to initialise mouse", "Win32Input8::initialiseImmediateMouse" );
        }
        /* Note that we did not acquire the mouse in the code above, since the call may fail (ie you're in the
           debugger) and an exception would be thrown. Acquisition happens in the captureMouse() function. */

        /* Get initial mouse data. We might as well fail this initial attempt, so no biggie. */
        captureMouse();

        /* Clear any relative mouse data. */
        mMouseState.Xrel = mMouseState.Yrel = mMouseState.Zrel = 0;

        LogManager::getSingleton().logMessage( "Win32Input8: Mouse input in immediate mode initialized." );

        OgreUnguard();
	}

    //-----------------------------------------------------------------------
    void Win32Input8::initialiseBufferedMouse()
	{
        HRESULT hr;
        LogManager::getSingleton().logMessage("Win32Input8: Establishing mouse input.");

        // Create mouse device
        hr = mlpDI->CreateDevice(GUID_SysMouse, &mlpDIMouse, NULL);


        if (FAILED(hr))
            OGRE_EXCEPT(hr, "Unable to create DirectInput mouse device.",
                "Win32Input8 - initialise");

        // Set data format
        hr = mlpDIMouse->SetDataFormat(&c_dfDIMouse2);
        if (FAILED(hr))
            OGRE_EXCEPT(hr, "Unable to set DirectInput mouse device data format.",
                "Win32Input8 - initialise");

        // Make the window grab mouse behaviour when foreground
        hr = mlpDIMouse->SetCooperativeLevel(mHWnd,
                   DISCL_FOREGROUND | DISCL_EXCLUSIVE);
        if (FAILED(hr))
            OGRE_EXCEPT(hr, "Unable to set DirectInput mouse device co-operative level.",
                "Win32Input8 - initialise");


		// IMPORTANT STEP TO USE BUFFERED DEVICE DATA!
		//
		// DirectInput uses unbuffered I/O (buffer size = 0) by default.
		// If you want to read buffered data, you need to set a nonzero
		// buffer size.
		//
		// Set the buffer size to SAMPLE_BUFFER_SIZE (defined above) elements.
		//
		// The buffer size is a DWORD property associated with the device.
		DIPROPDWORD dipdw;
		dipdw.diph.dwSize       = sizeof(DIPROPDWORD);
		dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		dipdw.diph.dwObj        = 0;
		dipdw.diph.dwHow        = DIPH_DEVICE;
		dipdw.dwData            = DINPUT_BUFFERSIZE; // Arbitary buffer size

		hr = mlpDIMouse->SetProperty( DIPROP_BUFFERSIZE, &dipdw.diph );

        if (FAILED(hr))
            OGRE_EXCEPT(hr, "Unable to create DirectInput mouse buffer.",
                "Win32Input8 - initialise");

        // Acquire input... we could have lost focus if the
		// user tabbed away during init or perhaps we're in
		// the debugger.  In either case when the input is
		// checked we will try to acquire again.
		hr = mlpDIMouse->Acquire();
        if (FAILED(hr) && hr != DIERR_OTHERAPPHASPRIO)
            OGRE_EXCEPT(hr, "Unable to set aquire DirectInput mouse device.",
                "Win32Input8 - initialise");

        LogManager::getSingleton().logMessage("Win32Input8: Mouse input established.");

	}

    //-----------------------------------------------------------------------
    void Win32Input8::initialise(RenderWindow* pWindow, bool useKeyboard, bool useMouse, bool useGameController)
    {
        HRESULT hr;

		mUseKeyboard = useKeyboard;
		mUseMouse = useMouse;
        LogManager::getSingleton().logMessage("Win32Input8: DirectInput Activation Starts");

        // Get HINST
#if OGRE_COMPILER == OGRE_COMPILER_MSVC
        HINSTANCE hInst = (HINSTANCE) &__ImageBase;
#else
        HINSTANCE hInst = GetModuleHandle(OGRE_PLATFORM_LIB);
#endif

        // Get HWND
        HWND hWnd;
        pWindow->getCustomAttribute("HWND", &hWnd);

        mHWnd = hWnd;

        ShowCursor(FALSE);


    // Register with the DirectInput subsystem and get a pointer
    // to a IDirectInput interface we can use.
    // Create a DInput object
		hr = DirectInput8Create( hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&mlpDI, NULL );
        if (FAILED(hr))
            OGRE_EXCEPT(hr, "Unable to initialise DirectInput.",
                "Win32Input8 - initialise");

		if (useKeyboard)
		{
			if (mUseBufferedKeys)
			{
				initialiseBufferedKeyboard();
			}
			else
			{
				initialiseImmediateKeyboard();
			}
		}

		if (useMouse)
		{
			if (mUseBufferedMouse)
			{
				initialiseBufferedMouse();
			}
			else
			{
				initialiseImmediateMouse();
			}
		}


        LogManager::getSingleton().logMessage("Win32Input8: DirectInput OK.");

    }

/*	  void Win32Input8::setBufferedInput(bool keys, bool mouse)
    {
		  flushAllBuffers();
		  InputReader::setBufferedInput(keys, mouse);
	}
*/
	void Win32Input8::flushAllBuffers()
	{

		DWORD dwItems = INFINITE;
		HRESULT hr = mlpDIKeyboard->GetDeviceData( sizeof(DIDEVICEOBJECTDATA),
										 NULL, &dwItems, 0 );
		hr = mlpDIMouse->GetDeviceData( sizeof(DIDEVICEOBJECTDATA),
										 NULL, &dwItems, 0 );
	}

    //-----------------------------------------------------------------------

	// this function is not needed at the moment because we are making everything buffered
	  void Win32Input8::setBufferedInput(bool keys, bool mouse)
    {
		if (mUseKeyboard && mUseBufferedKeys != keys)
		{
			if (mlpDIKeyboard)
			{
			    mlpDIKeyboard->Unacquire();
			    mlpDIKeyboard->Release();
				mlpDIKeyboard = 0;
			}
			if (keys)
			{
				initialiseBufferedKeyboard();
			}
			else
			{
				initialiseImmediateKeyboard();
			}

		}
		if (mUseMouse && mUseBufferedMouse != mouse)
		{
			if (mlpDIMouse)
			{
			    mlpDIMouse->Unacquire();
			    mlpDIMouse->Release();
				mlpDIMouse= 0;
			}
			if (mouse)
			{
				initialiseBufferedMouse();
			}
			else
			{
				initialiseImmediateMouse();
			}

		}
		InputReader::setBufferedInput(keys,mouse);
    }

    //-----------------------------------------------------------------------
    void Win32Input8::capture(void)
    {
		if (mUseKeyboard)
		{
			if (mUseBufferedKeys )
			{
				readBufferedKeyboardData();
			}
			else
			{
				mModifiers = getKeyModifiers();
				captureKeyboard();
			}
		}
		if (mUseMouse)
		{
			if (mUseBufferedMouse )
			{
				readBufferedMouseData();
			}
			else
			{
				captureMouse();
			}
		}

	}
    //-----------------------------------------------------------------------
    void Win32Input8::captureKeyboard(void)
    {
        HRESULT  hr;

        // Get keyboard state
        hr = mlpDIKeyboard->GetDeviceState(sizeof(mKeyboardBuffer),(LPVOID)&mKeyboardBuffer);
        if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
        {
            hr = mlpDIKeyboard->Acquire();
            if (hr == DIERR_OTHERAPPHASPRIO)
            {
                hr = 0;
            }
            else
            {
                hr = mlpDIKeyboard->GetDeviceState(sizeof(mKeyboardBuffer),(LPVOID)&mKeyboardBuffer);
            }
        }
        else if (hr == DIERR_OTHERAPPHASPRIO)
        {
            // We've gone into the background - ignore
            hr = 0;
        }
        else if (hr == DIERR_NOTINITIALIZED)
        {
            hr = 0;
        }
        else if (hr == E_PENDING)
        {
            hr = 0;
        }
        else if (FAILED(hr))
        {
            // Ignore for now
            // TODO - sort this out
            hr = 0;
        }

	}

    //-----------------------------------------------------------------------
    void Win32Input8::captureMouse(void)
    {
        DIMOUSESTATE2 mouseState;
        HRESULT hr;

        // Get mouse state
        hr = mlpDIMouse->GetDeviceState( sizeof( DIMOUSESTATE2 ), (LPVOID)&mouseState );

        if( SUCCEEDED( hr ) ||
            ( ( hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED ) &&
              SUCCEEDED( mlpDIMouse->Acquire() ) &&
              SUCCEEDED( mlpDIMouse->GetDeviceState( sizeof( DIMOUSESTATE2 ), (LPVOID)&mouseState ) ) ) )
        {
            /* Register the new 'origin'. */
            mMouseCenterX = mMouseState.Xabs;
            mMouseCenterY = mMouseState.Yabs;
            mMouseCenterZ = mMouseState.Zabs;

            /* Get the new absolute position. */
            mMouseState.Xabs = mouseState.lX;
            mMouseState.Yabs = mouseState.lY;
            mMouseState.Zabs = mouseState.lZ;

            /* Compute the new relative position. */
            mMouseState.Xrel = mMouseState.Xabs - mMouseCenterX;
            mMouseState.Yrel = mMouseState.Yabs - mMouseCenterY;
            mMouseState.Zrel = mMouseState.Zabs - mMouseCenterZ;

            /* Get the mouse buttons. This for loop can be unwrapped for speed. */
            mMouseState.Buttons = 0;
            for( size_t i = 0; i < 8; i++ )
                if( mouseState.rgbButtons[ i ] & 0x80 )
                    mMouseState.Buttons |= ( 1 << i );
        }
        else if (hr == DIERR_OTHERAPPHASPRIO)
        {
            // We've gone into the background - ignore
            hr = 0;
        }
        else if (hr == DIERR_NOTINITIALIZED)
        {
            hr = 0;
        }
        else if (hr == E_PENDING)
        {
            hr = 0;
        }
        else if (FAILED(hr))
        {
            // Ignore for now
            // TODO - sort this out
            hr = 0;
        }

   }



	//-----------------------------------------------------------------------------
	// Name: readBufferedData()
	// Desc: Read the input device's state when in buffered mode and display it.
	//-----------------------------------------------------------------------------
	bool Win32Input8::readBufferedKeyboardData()
	{
		DIDEVICEOBJECTDATA didod[ DINPUT_BUFFERSIZE ];  // Receives buffered data
		DWORD              dwElements;
		HRESULT            hr;

		if( NULL == mlpDIKeyboard )
			return true;

		dwElements = DINPUT_BUFFERSIZE;

		hr = mlpDIKeyboard->GetDeviceData( sizeof(DIDEVICEOBJECTDATA),
										 didod, &dwElements, 0 );
		if( FAILED(hr) )
		{
			// Error
			hr = mlpDIKeyboard->Acquire();
			while( hr == DIERR_INPUTLOST )
				hr = mlpDIKeyboard->Acquire();

			// hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
			// may occur when the app is minimized or in the process of
			// switching, so just try again later
			return !FAILED(hr);
		}

		// DI_BUFFEROVERFLOW we don't do anything with since we still want to
		// process what's in the buffer

		for(unsigned int i = 0; i < dwElements; i++ )
		{
			keyChanged( didod[ i ].dwOfs, (didod[ i ].dwData & 0x80) != 0);
		}
		return true;
	}

	//-----------------------------------------------------------------------------
	// Name: readBufferedData()
	// Desc: Read the input device's state when in buffered mode and display it.
	//-----------------------------------------------------------------------------
	bool Win32Input8::readBufferedMouseData()
	{
		DIDEVICEOBJECTDATA didod[ DINPUT_BUFFERSIZE ];  // Receives buffered data
		DWORD              dwElements;
		HRESULT            hr;

		if( NULL == mlpDIMouse )
			return true;

		dwElements = DINPUT_BUFFERSIZE;

        // Try to read the data. Continue normally on success (DI_OK
        // or DI_BUFFEROVERFLOW) as we have to process the read data
        // we got.
		hr = mlpDIMouse->GetDeviceData( sizeof(DIDEVICEOBJECTDATA),
										 didod, &dwElements, 0 );
		if( FAILED(hr) )
		{
            // No need to handle DI_BUFFEROVERFLOW in ERROR handling.
            // So moved it to later.
            //
			// We got an error.
			//
			// It means that continuous contact with the
			// device has been lost, either due to an external
			// interruption.
			//

			hr = mlpDIMouse->Acquire();
			while( hr == DIERR_INPUTLOST )
				hr = mlpDIMouse->Acquire();

			// hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
			// may occur when the app is minimized or in the process of
			// switching, so just try again later
			return !FAILED(hr);
		}

		// DI_BUFFEROVERFLOW we don't do anything with since we still want to
		// process what's in the buffer

		bool xSet = false;
		bool ySet = false;
		bool zSet = false;
/// Redefine FIELD_OFFSET in case of GCC
#ifdef __GNUC__
    #undef FIELD_OFFSET
    #define FIELD_OFFSET offsetof
#endif // __GNUC__

		for(unsigned int i = 0; i < dwElements; i++ )
		{
			int nMouseCode = -1;		// not set

			// this will display then scan code of the key
			// plus a 'D' - meaning the key was pressed
			//   or a 'U' - meaning the key was released
			switch( didod [ i ].dwOfs )
			{
				case DIMOFS_BUTTON0:
					nMouseCode = InputEvent::BUTTON0_MASK;
					break;

				case DIMOFS_BUTTON1:
					nMouseCode = InputEvent::BUTTON1_MASK;
					break;

				case DIMOFS_BUTTON2:
					nMouseCode = InputEvent::BUTTON2_MASK;
					break;

				case DIMOFS_BUTTON3:
					nMouseCode = InputEvent::BUTTON3_MASK;
					break;

				case DIMOFS_X:
					if (xSet)
					{	// process the last X move since we have a new one
						mouseMoved();
						xSet = false;
					}
					mCursor->addToX(getScaled(didod[i].dwData));
					xSet = true;
					break;

				case DIMOFS_Y:
					if (ySet)
					{
						mouseMoved();
						ySet = false;
					}
					mCursor->addToY(getScaled(didod[i].dwData));
					ySet = true;
					break;

				case DIMOFS_Z:
					if (zSet)
					{
						mouseMoved();
						zSet = false;
					}
					mCursor->addToZ(getScaled(didod[i].dwData));
					zSet = true;
					break;

				default:
					break;
			}
			if (nMouseCode != -1)
			{
				triggerMouseButton(nMouseCode, (didod [ i ].dwData & 0x80) != 0);
			}
			if (xSet && ySet)	// don't create 2 mousemove events for an single X and Y move, just create 1.
			{
				mouseMoved();
				ySet = false;
				xSet = false;
			}


		}
		if (zSet || xSet || ySet) // check for last moved at end
		{
			mouseMoved();
		}

		return true;
	}
    //-----------------------------------------------------------------------

	Real Win32Input8::getScaled(DWORD dwVal) const
	{
		return (Real)((int)dwVal) * mMouseScale;
	}

    //-----------------------------------------------------------------------
    bool Win32Input8::isKeyDownImmediate(KeyCode kc) const
    {
        return ( mKeyboardBuffer[ kc ] & 0x80 ) != 0;
    }

    //---------------------------------------------------------------------------------------------
    long Win32Input8::getMouseRelX() const
    {
        return mMouseState.Xrel;
    }

    //---------------------------------------------------------------------------------------------
    long Win32Input8::getMouseRelY() const
    {
        return mMouseState.Yrel;
    }

    //---------------------------------------------------------------------------------------------
    long Win32Input8::getMouseRelZ() const
    {
        return mMouseState.Zrel;
    }

    long Win32Input8::getMouseAbsX() const
    {
        return mMouseState.Xabs;
    }

    long Win32Input8::getMouseAbsY() const
    {
        return mMouseState.Yabs;
    }

    long Win32Input8::getMouseAbsZ() const
    {
        return mMouseState.Zabs;
    }

    //---------------------------------------------------------------------------------------------
    bool Win32Input8::getMouseButton( uchar button ) const
    {
        return mMouseState.isButtonDown( button ) != 0;
    }

    //---------------------------------------------------------------------------------------------
    void Win32Input8::getMouseState( MouseState& state ) const
    {
        memcpy( &state, &mMouseState, sizeof( MouseState ) );
    }

    //---------------------------------------------------------------------------------------------
	long Win32Input8::getKeyModifiers() const
	{
		long ret = mModifiers;

		if (isKeyDown(KC_LMENU) || isKeyDown(KC_RMENU))
		{
			ret |= InputEvent::ALT_MASK;
		}
		else
		{
			ret &= ~InputEvent::ALT_MASK;
		}

		if (isKeyDown(KC_LSHIFT) || isKeyDown(KC_RSHIFT))
		{
			ret |= InputEvent::SHIFT_MASK;
		}
		else
		{
			ret &= ~InputEvent::SHIFT_MASK;
		}

		if (isKeyDown(KC_LCONTROL) || isKeyDown(KC_LCONTROL))
		{
			ret |= InputEvent::CTRL_MASK;
		}
		else
		{
			ret &= ~InputEvent::CTRL_MASK;
		}

		return ret;
	}

} // namespace
#endif
#endif
