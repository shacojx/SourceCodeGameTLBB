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
#ifndef __Win32Input_H__
#define __Win32Input_H__


#include "OgreWin32Prerequisites.h"

#ifndef OGRE_NO_DX_INPUT

#ifdef DX7INPUTONLY
#include "OgreInput.h"



#include <dinput.h>

namespace Ogre {

    /** Utility class for dealing with user input on a Win32 system.
        Note that this is a basic implementation only at the moment.
    */
    class Win32Input : public InputReader
    {
    public:

        Win32Input();
        ~Win32Input();

        /** Initialise the input system.
            @param pWindow The window to capture input for
            @param useKeyboard If true, keyboard input will be supported.
            @param useMouse If true, mouse input will be supported.
            @param useGameController If true, joysticks/gamepads will be supported.
        */
        void initialise(RenderWindow* pWindow, bool useKeyboard = true, bool useMouse = true, bool useGameController = false);

        /** Captures the state of all the input devices.
            This method captures the state of all input devices and stores it internally for use when
            the enquiry methods are next called. This is done to ensure that all input is captured at once
            and therefore combinations of input are not subject to time differences when methods are called.

        */
        void capture(void);


        /*
         *	Mouse getters.
         */
      virtual long getMouseRelX() const;
      virtual long getMouseRelY() const;
      virtual long getMouseRelZ() const{return 0;};

      virtual long getMouseAbsX() const{return 0;};
      virtual long getMouseAbsY() const{return 0;};
	  virtual long getMouseAbsZ() const{return 0;};

      virtual void getMouseState( MouseState& state ) const{};

      virtual bool getMouseButton( uchar button ) const{return false;};

//		void setBufferedInput(bool keys, bool mouse) ;
//		void flushAllBuffers() ;


    protected:
        /** Determines if the specified key is currently depressed.
        Note that this enquiry method uses the state of the keyboard at the last 'capture' call.
        */
        bool isKeyDownImmediate(KeyCode kc) const ;


    private:
        // Input device details
        LPDIRECTINPUT7 mlpDI;
        LPDIRECTINPUTDEVICE7 mlpDIKeyboard;
        LPDIRECTINPUTDEVICE7 mlpDIMouse;

        HWND mHWnd;


        // State of keyboard at last 'capture' call
        char mKeyboardBuffer[256];
        int mMouseX, mMouseY;
        int mMouseCenterX, mMouseCenterY;
        bool mLMBDown, mRMBDown;
    };



}

#endif
#endif
#endif
