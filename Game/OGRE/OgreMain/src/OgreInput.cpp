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
#include "OgreStableHeaders.h"

#include "OgreInput.h"
#include "OgreCursor.h"
#include "OgreEventQueue.h"
#include "OgreKeyEvent.h"
#include "OgreEventListeners.h"
#include "OgreLogManager.h"


namespace Ogre {
	//-----------------------------------------------------------------------
    InputReader::InputReader()
    {
		mCursor = 0;
        mModifiers = 0;
		mEventQueue = 0;
		mUseBufferedKeys = false;
		mUseBufferedMouse = false;
	}

    //-----------------------------------------------------------------------
    void InputReader::useBufferedInput(EventQueue* pEventQueue, bool keys, bool mouse) 
    {
		mEventQueue = pEventQueue;

		if (mCursor)
			delete mCursor;

		mCursor = new Cursor();

		// initial states of buffered don't call setBufferedInput 
		// because that can be overriden (in the future) to save releasing and acquiring unchanged inputs
		// if we ever decide to release and acquire devices
		mUseBufferedKeys = keys;
		mUseBufferedMouse = mouse;
    }

    //-----------------------------------------------------------------------
    void InputReader::setBufferedInput(bool keys, bool mouse) 
    {
		mUseBufferedKeys = keys;
		mUseBufferedMouse = mouse;
    }


    //-----------------------------------------------------------------------
    InputReader::~InputReader()
    {
		if (mCursor)
		{
			delete mCursor;
		}
    }

    //-----------------------------------------------------------------------
	void InputReader::triggerMouseButton(int nMouseCode, bool mousePressed)
	{
		if (mousePressed)
		{
			mModifiers |= nMouseCode;
			createMouseEvent(MouseEvent::ME_MOUSE_PRESSED, nMouseCode);
            // Update immediate-mode mouse button state
            switch(nMouseCode)
            {
            case InputEvent::BUTTON0_MASK:
                mMouseState.Buttons |= 0x1;
                break;
            case InputEvent::BUTTON1_MASK:
                mMouseState.Buttons |= 0x2;
                break;
            case InputEvent::BUTTON2_MASK:
                mMouseState.Buttons |= 0x4;
                break;
            }

		}
		else
		{	// button up... trigger MouseReleased, and MouseClicked
			mModifiers &= ~nMouseCode;
			createMouseEvent(MouseEvent::ME_MOUSE_RELEASED, nMouseCode);
			//createMouseEvent(MouseEvent::ME_MOUSE_CLICKED, nMouseCode);	JCA - moved to EventDispatcher
            // Update immediate-mode mouse button state
            switch(nMouseCode)
            {
            case InputEvent::BUTTON0_MASK:
                mMouseState.Buttons &= 0xFE;
                break;
            case InputEvent::BUTTON1_MASK:
                mMouseState.Buttons &= 0xFD;
                break;
            case InputEvent::BUTTON2_MASK:
                mMouseState.Buttons &= 0xFB;
                break;
            }
		}

	}

    //-----------------------------------------------------------------------
	void InputReader::createMouseEvent(int id, int button)
	{
		MouseEvent* me =
            new MouseEvent(
                NULL, id, button, 0, // hack fix time
			    mModifiers,
                mCursor->getX(), mCursor->getY(), mCursor->getZ(),
                mCursor->getRelX(), mCursor->getRelY(), mCursor->getRelZ(),
                0
            );	// hack fix click count

        
        mCursor->processEvent(me);
		mEventQueue->push(me);

	}



    //-----------------------------------------------------------------------
	void InputReader::createKeyEvent(int id, int key)
	{
		KeyEvent* ke = new KeyEvent(NULL, id, key, 0, // hack fix time
			mModifiers);	// hack fix click count
		mEventQueue->push(ke);

	}
 
    //-----------------------------------------------------------------------
	void InputReader::mouseMoved()
	{

		if (mModifiers & InputEvent::BUTTON_ANY_MASK)	// don't need to know which button. you can get that from the modifiers
		{
			createMouseEvent(MouseEvent::ME_MOUSE_DRAGGED, 0);
		}
		else
		{
			createMouseEvent(MouseEvent::ME_MOUSE_MOVED, 0);
		}
	}
    //-----------------------------------------------------------------------
	void InputReader::addCursorMoveListener(MouseMotionListener* c)
	{
		mCursor->addMouseMotionListener(c);
	}
    //-----------------------------------------------------------------------
	void InputReader::removeCursorMoveListener(MouseMotionListener* c)
	{
		mCursor->removeMouseMotionListener(c);
	}
    //-----------------------------------------------------------------------

	void InputReader::keyChanged(int key, bool down)
	{
		if (down)
		{
            switch (key) {
            case KC_LMENU :
            case KC_RMENU :
  			    mModifiers |= InputEvent::ALT_MASK;
                break;

            case KC_LSHIFT :
            case KC_RSHIFT :
  			    mModifiers |= InputEvent::SHIFT_MASK;
                break;

            case KC_LCONTROL :
            case KC_RCONTROL :
  			    mModifiers |= InputEvent::CTRL_MASK;
                break;
            }

			createKeyEvent(KeyEvent::KE_KEY_PRESSED, key);

            // Update keydown map
            mBufferedKeysDown.insert(static_cast<KeyCode>(key));
		}
		else
		{
            switch (key) {
            case KC_LMENU :
            case KC_RMENU :
  			    mModifiers &= ~InputEvent::ALT_MASK;
                break;

            case KC_LSHIFT :
            case KC_RSHIFT :
  			    mModifiers &= ~InputEvent::SHIFT_MASK;
                break;

            case KC_LCONTROL :
            case KC_RCONTROL :
  			    mModifiers &= ~InputEvent::CTRL_MASK;
                break;
            }

			createKeyEvent(KeyEvent::KE_KEY_RELEASED, key);
			createKeyEvent(KeyEvent::KE_KEY_CLICKED, key);
            // Update keydown map
            mBufferedKeysDown.erase(static_cast<KeyCode>(key));
		}
	}
	
	//-----------------------------------------------------------------------
	char InputReader::getKeyChar(int keyCode, long modifiers)
	{
		if (modifiers == 0)
		{
			switch (keyCode) {
			case KC_1: return '1';
			case KC_2: return '2';
			case KC_3: return '3';
			case KC_4: return '4';
			case KC_5: return '5';
			case KC_6: return '6';
			case KC_7: return '7';
			case KC_8: return '8';
			case KC_9: return '9';
			case KC_0: return '0';
			case KC_MINUS: return '-';			/* - on main keyboard */
			case KC_EQUALS: return '=';
			case KC_Q: return 'q';
			case KC_W: return 'w';
			case KC_E: return 'e';
			case KC_R: return 'r';
			case KC_T: return 't';
			case KC_Y: return 'y';
			case KC_U: return 'u';
			case KC_I: return 'i';
			case KC_O: return 'o';
			case KC_P: return 'p';
			case KC_LBRACKET: return '[';
			case KC_RBRACKET: return ']';
			case KC_A: return 'a';
			case KC_S: return 's';
			case KC_D: return 'd';
			case KC_F: return 'f';
			case KC_G: return 'g';
			case KC_H: return 'h';
			case KC_J: return 'j';
			case KC_K: return 'k';
			case KC_L: return 'l';
			case KC_SEMICOLON: return ';';
			case KC_APOSTROPHE: return '\'';
			case KC_GRAVE: return '`';			/* accent grave */
			case KC_BACKSLASH: return '\\';
			case KC_Z: return 'z';
			case KC_X: return 'x';
			case KC_C: return 'c';
			case KC_V: return 'v';
			case KC_B: return 'b';
			case KC_N: return 'n';
			case KC_M: return 'm';
			case KC_COMMA: return ',';
			case KC_PERIOD: return '.';			/* . on main keyboard */
			case KC_SLASH: return '/';			/* '/' on main keyboard */
			case KC_MULTIPLY: return '*';		/* * on numeric keypad */
			case KC_SPACE: return ' ';
			case KC_NUMPAD7: return '7';
			case KC_NUMPAD8: return '8';
			case KC_NUMPAD9: return '9';
			case KC_SUBTRACT: return '-';		/* - on numeric keypad */
			case KC_NUMPAD4: return '4';
			case KC_NUMPAD5: return '5';
			case KC_NUMPAD6: return '6';
			case KC_ADD: return '+';			/* + on numeric keypad */
			case KC_NUMPAD1: return '1';
			case KC_NUMPAD2: return '2';
			case KC_NUMPAD3: return '3';
			case KC_NUMPAD0: return '0';
			case KC_DECIMAL: return '.';		/* . on numeric keypad */
			case KC_NUMPADEQUALS: return '=';	/* = on numeric keypad (NEC PC98) */
			case KC_AT: return '@';				/*                     (NEC PC98) */
			case KC_COLON: return ':';			/*                     (NEC PC98) */
			case KC_UNDERLINE: return '_';		/*                     (NEC PC98) */
			case KC_NUMPADCOMMA: return ',';	/* , on numeric keypad (NEC PC98) */
			case KC_DIVIDE: return '/';			/* / on numeric keypad */
			}
		}
		else if (modifiers == InputEvent::SHIFT_MASK)
		{
			switch (keyCode) {
			case KC_1: return '!';
			case KC_2: return '@';
			case KC_3: return '#';
			case KC_4: return '$';
			case KC_5: return '%';
			case KC_6: return '^';
			case KC_7: return '&';
			case KC_8: return '*';
			case KC_9: return '(';
			case KC_0: return ')';
			case KC_MINUS: return '_';
			case KC_EQUALS: return '+';
			case KC_Q: return 'Q';
			case KC_W: return 'W';
			case KC_E: return 'E';
			case KC_R: return 'R';
			case KC_T: return 'T';
			case KC_Y: return 'Y';
			case KC_U: return 'U';
			case KC_I: return 'I';
			case KC_O: return 'O';
			case KC_P: return 'P';
			case KC_LBRACKET: return '{';
			case KC_RBRACKET: return '}';
			case KC_A: return 'A';
			case KC_S: return 'S';
			case KC_D: return 'D';
			case KC_F: return 'F';
			case KC_G: return 'G';
			case KC_H: return 'H';
			case KC_J: return 'J';
			case KC_K: return 'K';
			case KC_L: return 'L';
			case KC_SEMICOLON: return ':';
			case KC_APOSTROPHE: return '"';
			case KC_GRAVE: return '~';			/* accent grave */
			case KC_BACKSLASH: return '|';
			case KC_Z: return 'Z';
			case KC_X: return 'X';
			case KC_C: return 'C';
			case KC_V: return 'V';
			case KC_B: return 'B';
			case KC_N: return 'N';
			case KC_M: return 'M';
			case KC_COMMA: return '<';
			case KC_PERIOD: return '>';			/* . on main keyboard */
			case KC_SLASH: return '?';			/* '/' on main keyboard */
			case KC_MULTIPLY: return '*';		/* * on numeric keypad */
			case KC_SPACE: return ' ';
			}
		}
		return 0;  
    }
    //-----------------------------------------------------------------------
    bool InputReader::isKeyDown( KeyCode kc ) const
    {
        if (mUseBufferedKeys)
        {
            return mBufferedKeysDown.find(kc) != mBufferedKeysDown.end();
        }
        else
        {
            return isKeyDownImmediate(kc);
        }
    }
}
