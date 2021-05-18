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

#include "OgreGLXInput.h"
#include "OgreLogManager.h"
#include "OgreMouseEvent.h"
#include "OgreCursor.h"

#include "OgreGLXWindow.h"

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

// Thanks to SDL for this idea
#define MOUSE_FUDGE_FACTOR 8

namespace Ogre
{

GLXInput::GLXInput() :
	InputReader(),
	captureMouse(false),
	warpMouse(false),
	mouseLastX(0),
	mouseLastY(0),
	mDisplay(0),
	mWindow(0),
	mRenderWindow(0),
	mMouseWarped(false)
{
	mEventQueue = 0;
	mMouseScale = 0.002f;

	_key_map.insert(InputKeyMap::value_type(XK_Escape,KC_ESCAPE));
	_key_map.insert(InputKeyMap::value_type(XK_1, KC_1));
	_key_map.insert(InputKeyMap::value_type(XK_2, KC_2));
	_key_map.insert(InputKeyMap::value_type(XK_3, KC_3));
	_key_map.insert(InputKeyMap::value_type(XK_4, KC_4));
	_key_map.insert(InputKeyMap::value_type(XK_5, KC_5));
	_key_map.insert(InputKeyMap::value_type(XK_6, KC_6));
	_key_map.insert(InputKeyMap::value_type(XK_7, KC_7));
	_key_map.insert(InputKeyMap::value_type(XK_8, KC_8));
	_key_map.insert(InputKeyMap::value_type(XK_9, KC_9));
	_key_map.insert(InputKeyMap::value_type(XK_0, KC_0));
	_key_map.insert(InputKeyMap::value_type(XK_minus, KC_MINUS));
	_key_map.insert(InputKeyMap::value_type(XK_equal, KC_EQUALS));
	_key_map.insert(InputKeyMap::value_type(XK_BackSpace, KC_BACK));
	_key_map.insert(InputKeyMap::value_type(XK_Tab, KC_TAB));
	_key_map.insert(InputKeyMap::value_type(XK_q, KC_Q));
	_key_map.insert(InputKeyMap::value_type(XK_w, KC_W));
	_key_map.insert(InputKeyMap::value_type(XK_e, KC_E));
	_key_map.insert(InputKeyMap::value_type(XK_r, KC_R));
	_key_map.insert(InputKeyMap::value_type(XK_t, KC_T));
	_key_map.insert(InputKeyMap::value_type(XK_y, KC_Y));
	_key_map.insert(InputKeyMap::value_type(XK_u, KC_U));
	_key_map.insert(InputKeyMap::value_type(XK_i, KC_I));
	_key_map.insert(InputKeyMap::value_type(XK_o, KC_O));
	_key_map.insert(InputKeyMap::value_type(XK_p, KC_P));
	_key_map.insert(InputKeyMap::value_type(XK_Return, KC_RETURN));
	_key_map.insert(InputKeyMap::value_type(XK_Control_L, KC_LCONTROL));
	_key_map.insert(InputKeyMap::value_type(XK_a, KC_A));
	_key_map.insert(InputKeyMap::value_type(XK_s, KC_S));
	_key_map.insert(InputKeyMap::value_type(XK_d, KC_D));
	_key_map.insert(InputKeyMap::value_type(XK_f, KC_F));
	_key_map.insert(InputKeyMap::value_type(XK_g, KC_G));
	_key_map.insert(InputKeyMap::value_type(XK_h, KC_H));
	_key_map.insert(InputKeyMap::value_type(XK_j, KC_J));
	_key_map.insert(InputKeyMap::value_type(XK_k, KC_K));
	_key_map.insert(InputKeyMap::value_type(XK_l, KC_L));
	_key_map.insert(InputKeyMap::value_type(XK_semicolon, KC_SEMICOLON));
	_key_map.insert(InputKeyMap::value_type(XK_colon, KC_COLON));
	_key_map.insert(InputKeyMap::value_type(XK_apostrophe, KC_APOSTROPHE));
	_key_map.insert(InputKeyMap::value_type(XK_grave, KC_GRAVE));
	_key_map.insert(InputKeyMap::value_type(XK_Shift_L, KC_LSHIFT));
	_key_map.insert(InputKeyMap::value_type(XK_backslash, KC_BACKSLASH));
	_key_map.insert(InputKeyMap::value_type(XK_z, KC_Z));
	_key_map.insert(InputKeyMap::value_type(XK_x, KC_X));
	_key_map.insert(InputKeyMap::value_type(XK_c, KC_C));
	_key_map.insert(InputKeyMap::value_type(XK_v, KC_V));
	_key_map.insert(InputKeyMap::value_type(XK_b, KC_B));
	_key_map.insert(InputKeyMap::value_type(XK_n, KC_N));
	_key_map.insert(InputKeyMap::value_type(XK_m, KC_M));
	_key_map.insert(InputKeyMap::value_type(XK_comma, KC_COMMA));
	_key_map.insert(InputKeyMap::value_type(XK_period, KC_PERIOD));
	_key_map.insert(InputKeyMap::value_type(XK_Shift_R, KC_RSHIFT));
	_key_map.insert(InputKeyMap::value_type(XK_KP_Multiply, KC_MULTIPLY));
	_key_map.insert(InputKeyMap::value_type(XK_Alt_L, KC_LMENU));  // ?
	_key_map.insert(InputKeyMap::value_type(XK_space, KC_SPACE));
	_key_map.insert(InputKeyMap::value_type(XK_Escape, KC_CAPITAL));
	_key_map.insert(InputKeyMap::value_type(XK_F1, KC_F1));
	_key_map.insert(InputKeyMap::value_type(XK_F2, KC_F2));
	_key_map.insert(InputKeyMap::value_type(XK_F3, KC_F3));
	_key_map.insert(InputKeyMap::value_type(XK_F4, KC_F4));
	_key_map.insert(InputKeyMap::value_type(XK_F5, KC_F5));
	_key_map.insert(InputKeyMap::value_type(XK_F6, KC_F6));
	_key_map.insert(InputKeyMap::value_type(XK_F7, KC_F7));
	_key_map.insert(InputKeyMap::value_type(XK_F8, KC_F8));
	_key_map.insert(InputKeyMap::value_type(XK_F9, KC_F9));
	_key_map.insert(InputKeyMap::value_type(XK_F10, KC_F10));
	_key_map.insert(InputKeyMap::value_type(XK_Num_Lock, KC_NUMLOCK));
	_key_map.insert(InputKeyMap::value_type(XK_Scroll_Lock, KC_SCROLL));
	_key_map.insert(InputKeyMap::value_type(XK_KP_7, KC_NUMPAD7));
	_key_map.insert(InputKeyMap::value_type(XK_KP_8, KC_NUMPAD8));
	_key_map.insert(InputKeyMap::value_type(XK_KP_9, KC_NUMPAD9));
	_key_map.insert(InputKeyMap::value_type(XK_KP_Subtract, KC_SUBTRACT));
	_key_map.insert(InputKeyMap::value_type(XK_KP_4, KC_NUMPAD4));
	_key_map.insert(InputKeyMap::value_type(XK_KP_5, KC_NUMPAD5));
	_key_map.insert(InputKeyMap::value_type(XK_KP_6, KC_NUMPAD6));
	_key_map.insert(InputKeyMap::value_type(XK_KP_Add, KC_ADD));
	_key_map.insert(InputKeyMap::value_type(XK_KP_1, KC_NUMPAD1));
	_key_map.insert(InputKeyMap::value_type(XK_KP_2, KC_NUMPAD2));
	_key_map.insert(InputKeyMap::value_type(XK_KP_3, KC_NUMPAD3));
	_key_map.insert(InputKeyMap::value_type(XK_KP_0, KC_NUMPAD0));
	_key_map.insert(InputKeyMap::value_type(XK_KP_Decimal, KC_DECIMAL));
	_key_map.insert(InputKeyMap::value_type(XK_F11, KC_F11));
	_key_map.insert(InputKeyMap::value_type(XK_F12, KC_F12));
	_key_map.insert(InputKeyMap::value_type(XK_F13, KC_F13));
	_key_map.insert(InputKeyMap::value_type(XK_F14, KC_F14));
	_key_map.insert(InputKeyMap::value_type(XK_F15, KC_F15));
	_key_map.insert(InputKeyMap::value_type(XK_KP_Equal, KC_NUMPADEQUALS));
	_key_map.insert(InputKeyMap::value_type(XK_KP_Divide, KC_DIVIDE));
	_key_map.insert(InputKeyMap::value_type(XK_Print, KC_SYSRQ));
	_key_map.insert(InputKeyMap::value_type(XK_Alt_R, KC_RMENU)); // ?
	_key_map.insert(InputKeyMap::value_type(XK_Home, KC_HOME));
	_key_map.insert(InputKeyMap::value_type(XK_Up, KC_UP));
	_key_map.insert(InputKeyMap::value_type(XK_Page_Up, KC_PGUP));
	_key_map.insert(InputKeyMap::value_type(XK_Left, KC_LEFT));
	_key_map.insert(InputKeyMap::value_type(XK_Right, KC_RIGHT));
	_key_map.insert(InputKeyMap::value_type(XK_End, KC_END));
	_key_map.insert(InputKeyMap::value_type(XK_Down, KC_DOWN));
	_key_map.insert(InputKeyMap::value_type(XK_Page_Down, KC_PGDOWN));
	_key_map.insert(InputKeyMap::value_type(XK_Insert, KC_INSERT));
	_key_map.insert(InputKeyMap::value_type(XK_Delete, KC_DELETE));
	_key_map.insert(InputKeyMap::value_type(XK_Super_L, KC_LWIN)); // ?
	_key_map.insert(InputKeyMap::value_type(XK_Super_R, KC_RWIN)); // ?
}

GLXInput::~GLXInput() {
	GrabCursor(false);
	XFreeCursor(mDisplay, mHiddenCursor);
}

void GLXInput::initialise(RenderWindow* pWindow, bool useKeyboard, bool useMouse, bool useGameController) {
	mRenderWindow = pWindow;
	captureMouse = useMouse;

	// Extract Window and Display from pWindow in magic way
	// This also raises an exception if it is not an GLXRenderWindow
	pWindow->getCustomAttribute("GLXWINDOW", &mWindow);
	pWindow->getCustomAttribute("GLXDISPLAY", &mDisplay);

	//Change input mask to reflect the fact that we want input events (the rendersystem only requested window
	//events)
	XSelectInput(mDisplay, mWindow, StructureNotifyMask | VisibilityChangeMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask );

	// Create hidden cursor
	// X offers no standard support for this so we need to create a blank pixmap.
	Pixmap blank_pixmap = XCreatePixmap(mDisplay, mWindow, 1, 1, 1);
    	GC gc = XCreateGC(mDisplay, blank_pixmap, (unsigned long)0, (XGCValues*)0);
    	XDrawPoint(mDisplay, blank_pixmap, gc, 0, 0);
    	XFreeGC(mDisplay, gc);
	XColor color;
	color.flags = DoRed | DoGreen | DoBlue;
	color.red = color.blue = color.green = 0;
    	mHiddenCursor = XCreatePixmapCursor(mDisplay, blank_pixmap, blank_pixmap, &color, &color, 0, 0);
	XFreePixmap(mDisplay, blank_pixmap);

	// Mousey stuff
	warpMouse = false;
	GrabCursor(true);

	// Get the center and put the mouse there
	unsigned int width, height, depth;
	int left, top;
	pWindow->getMetrics(width, height, depth, left, top);

	mMouseState.Buttons = 0;
	mouseLastX = mMouseState.Xabs = width / 2;
	mouseLastY = mMouseState.Yabs = height / 2;
	mMouseState.Zabs = 0;
	mMouseState.Xrel = 0;
	mMouseState.Yrel = 0;
	mMouseState.Zrel = 0;
}

void GLXInput::capture() {
	GLXWindow *w = static_cast<GLXWindow*>(mRenderWindow);
	KeySym key;
	KeyCode ogrekey;
	XEvent event;

	mMouseState.Xrel = 0;
	mMouseState.Yrel = 0;
	mMouseState.Zrel = 0;

	bool hasMouseMoved = false;

	unsigned int width, height, depth;
	int left, top;
	w->getMetrics(width, height, depth, left, top);

	// Process X events until event pump exhausted
	while(XPending(mDisplay) > 0) {
		XNextEvent(mDisplay,&event);

		// Give window a shot as processing the event
		// TODO: call for every window
		w->injectXEvent(event);

		int button_mask = -1;
		int button_bits = 0;
		bool button_down = false;
		switch(event.type) {
		case KeyPress:
			// Ignore shift and capslock state
			event.xkey.state &= ~ShiftMask;
			event.xkey.state &= ~LockMask;

			XLookupString(&event.xkey,NULL,0,&key,NULL);
			// Ctrl-Escape should free mouse ala Q****
			// Alt-TAB should free mouse ala SDL
			if((event.xkey.state & ControlMask && key == XK_Escape)
			||(event.xkey.state & Mod1Mask && key == XK_Tab)) {
				GrabCursor(false);
				break;
			}
			
			ogrekey = _key_map[key]; // key

			// Unbuffered input
			_key_pressed_set.insert(ogrekey);

			// Buffered input
			if(mUseBufferedKeys)
				keyChanged(ogrekey, true);
			break;
		case KeyRelease:
			//Hide autogenerated keyrepeats
			if (XPending(mDisplay) > 0)
			{
				// Get the next event without
				// consuming it.
				XEvent nextEvent;
				XPeekEvent(mDisplay, &nextEvent);

				// Check if event pair matches pattern
				if ( nextEvent.type         == KeyPress &&
				     nextEvent.xkey.window  == event.xkey.window &&
				     nextEvent.xkey.keycode == event.xkey.keycode &&
				     (nextEvent.xkey.time - event.xkey.time) < 5 )
				{
					// Consume it and ignore actions
					XNextEvent(mDisplay, &nextEvent);
					break;
				}
			}

			XLookupString(&event.xkey,NULL,0,&key,NULL);
			ogrekey = _key_map[key];

			// Unbuffered input
			_key_pressed_set.erase(ogrekey);

			// Buffered input
			if(mUseBufferedKeys)
				keyChanged(ogrekey, false);
			break;
		case MotionNotify:
			if( mMouseWarped &&
			    (event.xmotion.x < MOUSE_FUDGE_FACTOR || event.xmotion.x > (width-MOUSE_FUDGE_FACTOR) ||
			     event.xmotion.y < MOUSE_FUDGE_FACTOR || event.xmotion.y > (height-MOUSE_FUDGE_FACTOR)) )

			{ //Mouse has already warped.. And, value is still off
				break;
			}
			else
			{ //Mouse values are fine.. regardless of warpage
				mMouseState.Xabs = event.xmotion.x;
				mMouseState.Yabs = event.xmotion.y;
				hasMouseMoved = true;
				mMouseWarped = false;
			}

			break;
		case ButtonPress:
			button_down = true;
		case ButtonRelease:
			if( event.xbutton.button == 1 ) // LEFT
			{
				button_mask = InputEvent::BUTTON0_MASK;
				button_bits = 1;
			}
			else if( event.xbutton.button == 2 ) // MIDDLE
			{
				button_mask = InputEvent::BUTTON1_MASK;
				button_bits = 2;
			}
			else if( event.xbutton.button == 3 ) // RIGHT
			{
				button_mask = InputEvent::BUTTON2_MASK;
				button_bits = 4;
			}
			else if( event.xbutton.button == 4 ) // WHEEL UP
			{
				//Wheel axis only triggered once (on release), and break out of case
				if( button_down == false )
					mMouseState.Zrel += mWheelStep;
				hasMouseMoved = true;
				break;
			}
			else if( event.xbutton.button == 5 ) // WHEEN DOWN
			{
				//Wheel axis only triggered once (on release), and break out of case
				if( button_down == false )
					mMouseState.Zrel -= mWheelStep;
				hasMouseMoved = true;
				break;
			}

			// Unbuffered mouse
			if(button_down)
				mMouseState.Buttons |= button_bits;
			else
				mMouseState.Buttons &= ~button_bits;


			// Buffered mouse
			if(mUseBufferedMouse) triggerMouseButton(button_mask, button_down);

			// Mouse re-grab
			if(captureMouse && !warpMouse && button_down &&  event.xbutton.button==1)
				GrabCursor(true);

			break;
		} //end switch
	} //End whie events pending

	if(warpMouse)
	{
		// Calculate the relative motion from the last mouse position and the new one.
		mMouseState.Xrel = mMouseState.Xabs - mouseLastX;
		mMouseState.Yrel = mMouseState.Yabs - mouseLastY;
		mouseLastX = mMouseState.Xabs;
		mouseLastY = mMouseState.Yabs;

		// If the mouse cursor has reached the border of the window  (it is closer than MOUSE_FUDGE_FACTOR),
		//warp it back to the middle.
		if(mMouseState.Xabs < MOUSE_FUDGE_FACTOR || mMouseState.Xabs > (width-MOUSE_FUDGE_FACTOR) ||
		   mMouseState.Yabs < MOUSE_FUDGE_FACTOR || mMouseState.Yabs > (height-MOUSE_FUDGE_FACTOR) )
		{
			mouseLastX = mMouseState.Xabs = width / 2;
			mouseLastY = mMouseState.Yabs = height / 2;
			XWarpPointer(mDisplay, None, mWindow, 0, 0, 0, 0, mMouseState.Xabs, mMouseState.Yabs);
			mMouseWarped = true;
		}
	}
	if(hasMouseMoved && mUseBufferedMouse) {
		// Send mouse moved event to application and move cursor.
		mouseMoved();
		mCursor->addToX(mMouseState.Xrel * mMouseScale);
		mCursor->addToY(mMouseState.Yrel * mMouseScale);
		mCursor->addToZ(mMouseState.Zrel * mMouseScale);
	}

}

bool GLXInput::isKeyDownImmediate(KeyCode kc) const {
	return _key_pressed_set.count(kc);
}

long GLXInput::getMouseRelX() const {
	return mMouseState.Xrel;
}

long GLXInput::getMouseRelY() const {
	return mMouseState.Yrel;
}

long GLXInput::getMouseRelZ() const {
	return mMouseState.Zrel;
}

long GLXInput::getMouseAbsX() const {
	return mMouseState.Xabs;
}

long GLXInput::getMouseAbsY() const {
	return mMouseState.Yabs;
}

long GLXInput::getMouseAbsZ() const {
	return mMouseState.Zabs;
}

bool GLXInput::getMouseButton( uchar button ) const {
	return mMouseState.isButtonDown( button );
}

void GLXInput::getMouseState( MouseState& state ) const {
	memcpy( &state, &mMouseState, sizeof( MouseState ) );
}

void GLXInput::GrabCursor(bool grab) {
	if(!captureMouse)
		// Never do mouse capture
		return;
	// Get window metrics
	unsigned int width, height, depth;
	int left, top;
	mRenderWindow->getMetrics(width, height, depth, left, top);

	warpMouse = grab;
	if(warpMouse) {
		// Hide mouse
		XDefineCursor(mDisplay, mWindow, mHiddenCursor);
		// Grab keyboard and mouse
		XGrabPointer(mDisplay,mWindow,True,0,GrabModeAsync,GrabModeAsync,mWindow,None,CurrentTime);
		XGrabKeyboard(mDisplay,mWindow,True,GrabModeAsync,GrabModeAsync,CurrentTime);
		// Mouse warpin' fun
		mouseLastX = mMouseState.Xabs = width / 2;
		mouseLastY = mMouseState.Yabs = height / 2;
		XWarpPointer(mDisplay, None, mWindow, 0, 0, 0, 0,
                                       mMouseState.Xabs, mMouseState.Yabs);
	} else {
		// Show mouse
		XUndefineCursor(mDisplay, mWindow);
		// Ungrab keyboard and mouse
		XUngrabPointer(mDisplay,CurrentTime);
		XUngrabKeyboard(mDisplay,CurrentTime);

	}
}


}
