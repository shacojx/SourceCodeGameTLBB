/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 The OGRE Team
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


#ifndef __SDLInputReader_H__
#define __SDLInputReader_H__

#include "OgreInput.h"
#include "OgreInputEvent.h"
#include "OgreRenderWindow.h"

#include <map>

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#	include <SDL/sdl.h>
#else
#	include <SDL.h>
#endif

namespace Ogre
{

enum GrabMode
{
	GRAB_NONE,
	GRAB_MOUSE_OVER,
	GRAB_MOUSE_CLICK   // this is the default
};

class SDLInput : public InputReader
{
    public:
        SDLInput();
        virtual ~SDLInput();

        void initialise( RenderWindow* pWindow, bool useKeyboard = true, bool useMouse = true,
			 bool useGameController = false );
        void capture();

	/**
		Sets how to grab the mouse. Possible values for mode are GRAB_MOUSE_OVER
		or GRAB_MOUSE_BUTTON. The first will grab the mouse if the pointer is over the
		application window and the latter will wait for a mouse button click to aquire
		the mouse. Default is GRAB_MOUSE_CLICK.
	*/
	void setGrabMode( GrabMode mode ) { mGrabMode = mode; }

        /*
         * Mouse getters
         */
        virtual long getMouseRelX() const;
        virtual long getMouseRelY() const;
        virtual long getMouseRelZ() const;

        virtual long getMouseAbsX() const;
        virtual long getMouseAbsY() const;
        virtual long getMouseAbsZ() const;

        virtual void getMouseState( MouseState& state ) const;

        virtual bool getMouseButton( uchar button ) const;

    private:
	RenderWindow *mRenderWindow;
        // State at last 'capture' call
        Uint8* mKeyboardBuffer;
        int mMaxKey;
        int mMouseX, mMouseY;
        int mMouseRelativeX, mMouseRelativeY, mMouseRelativeZ;
        Uint8 mMouseKeys;
        bool _visible;
		
	bool mMouseGrabbed;  // true if Ogre has control over the mouse input
	bool mUseMouse;   // true if initialise() is called with useMouse == true
	bool mGrabMouse;  // grab the mouse input if the situation specified by mGrabMode arises
	bool mMouseLeft;  // true if the mouse pointer has left the window after calling releaseMouse(). Needed for
	// mGrabMode == GRAB_MOUSE_BUTTON.
	int mGrabMode;    // when/how to grab the mouse
	

        typedef std::map<SDLKey, KeyCode> InputKeyMap;
        InputKeyMap _key_map;
        bool warpMouse;

	// the value that is added to mMouseRelativeZ when the wheel
	// is moved one step (this value is actually added
	// twice per movement since a wheel movement triggers a
	// MOUSEBUTTONUP and a MOUSEBUTTONDOWN event).
	// The value is chosen according to the windoze value.
        static const unsigned int mWheelStep = 60;

        void processBufferedKeyboard();
        void processBufferedMouse();

	void _grabMouse();
	void _releaseMouse();
        bool isKeyDownImmediate( KeyCode kc ) const;
};
}
#endif

