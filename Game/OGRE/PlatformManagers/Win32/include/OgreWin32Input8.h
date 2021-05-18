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
#ifndef __Win32Input8_H__
#define __Win32Input8_H__

#include "OgreWin32Prerequisites.h"

#ifndef OGRE_NO_DX_INPUT

#ifndef DX7INPUTONLY 

#include "OgreInput.h"
#include "OgreInputEvent.h"

#include <dinput.h>

namespace Ogre {

    /** Utility class for dealing with user input on a Win32 system.
        Note that this is a basic implementation only at the moment.
    */
    class Win32Input8 : public InputReader
    {
    public:

        Win32Input8();
        ~Win32Input8();

        /** @copydoc InputReader::initialise */
	    virtual void initialise(
            RenderWindow* pWindow, 
            bool useKeyboard = true, 
            bool useMouse = true, 
            bool useGameController = false );

        /** @copydoc InputReader::capture */
        virtual void capture();


        /*
         *	Mouse getters.
         */
        virtual long getMouseRelX() const;
        virtual long getMouseRelY() const;
        virtual long getMouseRelZ() const;

        virtual long getMouseAbsX() const;
        virtual long getMouseAbsY() const;
        virtual long getMouseAbsZ() const;

        virtual void getMouseState( MouseState& state ) const;

	    virtual bool getMouseButton( uchar button ) const;

		void setBufferedInput(bool keys, bool mouse) ;
		void flushAllBuffers() ;
    protected:
        /** @copydoc InputReader::isKeyDown */
        virtual bool isKeyDownImmediate(KeyCode kc) const;

    private:
        // Input device details
        IDirectInput8* mlpDI;
        IDirectInputDevice8* mlpDIKeyboard;
        IDirectInputDevice8* mlpDIMouse;

        HWND mHWnd;

		/** specialised initialisation routines */		
	    void initialiseBufferedKeyboard();
	    void initialiseImmediateKeyboard();
	    void initialiseBufferedMouse();
		void initialiseImmediateMouse();

		/* immediate mode */
	    void captureKeyboard(void);
	    void captureMouse(void);

		/* buffered mode */
		bool readBufferedKeyboardData();
		bool readBufferedMouseData();

        /* State of modifiers at last 'capture' call 
		   NOTE this doesn't support keyboard buffering yet */
		long getKeyModifiers() const;

        /* For mouse immediate mode. Note that the space origin in DX is (0,0,0), here we
           only hold the 'last' center for relative input. */
        long mMouseCenterX, mMouseCenterY, mMouseCenterZ;
		bool mUseKeyboard, mUseMouse;

		/* For mouse buffered mode. */
		Real getScaled(DWORD dwVal) const;

		/* For keyboard immediate mode. */
        char mKeyboardBuffer[256];
    };



}

#endif
#endif
#endif
