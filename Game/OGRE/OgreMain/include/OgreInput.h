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
#ifndef __Input_H__
#define __Input_H__

#include "OgrePrerequisites.h"

namespace Ogre {

    /** Keyboard scan codes - copied from DirectInput for now for speed.
    */
    enum KeyCode
    {
        KC_ESCAPE          =0x01,
        KC_1               =0x02,
        KC_2               =0x03,
        KC_3               =0x04,
        KC_4               =0x05,
        KC_5               =0x06,
        KC_6               =0x07,
        KC_7               =0x08,
        KC_8               =0x09,
        KC_9               =0x0A,
        KC_0               =0x0B,
        KC_MINUS           =0x0C,    /* - on main keyboard */
        KC_EQUALS          =0x0D,
        KC_BACK            =0x0E,    /* backspace */
        KC_TAB             =0x0F,
        KC_Q               =0x10,
        KC_W               =0x11,
        KC_E               =0x12,
        KC_R               =0x13,
        KC_T               =0x14,
        KC_Y               =0x15,
        KC_U               =0x16,
        KC_I               =0x17,
        KC_O               =0x18,
        KC_P               =0x19,
        KC_LBRACKET        =0x1A,
        KC_RBRACKET        =0x1B,
        KC_RETURN          =0x1C,    /* Enter on main keyboard */
        KC_LCONTROL        =0x1D,
        KC_A               =0x1E,
        KC_S               =0x1F,
        KC_D               =0x20,
        KC_F               =0x21,
        KC_G               =0x22,
        KC_H               =0x23,
        KC_J               =0x24,
        KC_K               =0x25,
        KC_L               =0x26,
        KC_SEMICOLON       =0x27,
        KC_APOSTROPHE      =0x28,
        KC_GRAVE           =0x29,    /* accent grave */
        KC_LSHIFT          =0x2A,
        KC_BACKSLASH       =0x2B,
        KC_Z               =0x2C,
        KC_X               =0x2D,
        KC_C               =0x2E,
        KC_V               =0x2F,
        KC_B               =0x30,
        KC_N               =0x31,
        KC_M               =0x32,
        KC_COMMA           =0x33,
        KC_PERIOD          =0x34,    /* . on main keyboard */
        KC_SLASH           =0x35,    /* '/' on main keyboard */
        KC_RSHIFT          =0x36,
        KC_MULTIPLY        =0x37,    /* * on numeric keypad */
        KC_LMENU           =0x38,    /* left Alt */
        KC_SPACE           =0x39,
        KC_CAPITAL         =0x3A,
        KC_F1              =0x3B,
        KC_F2              =0x3C,
        KC_F3              =0x3D,
        KC_F4              =0x3E,
        KC_F5              =0x3F,
        KC_F6              =0x40,
        KC_F7              =0x41,
        KC_F8              =0x42,
        KC_F9              =0x43,
        KC_F10             =0x44,
        KC_NUMLOCK         =0x45,
        KC_SCROLL          =0x46,    /* Scroll Lock */
        KC_NUMPAD7         =0x47,
        KC_NUMPAD8         =0x48,
        KC_NUMPAD9         =0x49,
        KC_SUBTRACT        =0x4A,    /* - on numeric keypad */
        KC_NUMPAD4         =0x4B,
        KC_NUMPAD5         =0x4C,
        KC_NUMPAD6         =0x4D,
        KC_ADD             =0x4E,    /* + on numeric keypad */
        KC_NUMPAD1         =0x4F,
        KC_NUMPAD2         =0x50,
        KC_NUMPAD3         =0x51,
        KC_NUMPAD0         =0x52,
        KC_DECIMAL         =0x53,    /* . on numeric keypad */
        KC_OEM_102         =0x56,    /* < > | on UK/Germany keyboards */
        KC_F11             =0x57,
        KC_F12             =0x58,
        KC_F13             =0x64,    /*                     (NEC PC98) */
        KC_F14             =0x65,    /*                     (NEC PC98) */
        KC_F15             =0x66,    /*                     (NEC PC98) */
        KC_KANA            =0x70,    /* (Japanese keyboard)            */
        KC_ABNT_C1         =0x73,    /* / ? on Portugese (Brazilian) keyboards */
        KC_CONVERT         =0x79,    /* (Japanese keyboard)            */
        KC_NOCONVERT       =0x7B,    /* (Japanese keyboard)            */
        KC_YEN             =0x7D,    /* (Japanese keyboard)            */
        KC_ABNT_C2         =0x7E,    /* Numpad . on Portugese (Brazilian) keyboards */
        KC_NUMPADEQUALS    =0x8D,    /* = on numeric keypad (NEC PC98) */
        KC_PREVTRACK       =0x90,    /* Previous Track (KC_CIRCUMFLEX on Japanese keyboard) */
        KC_AT              =0x91,    /*                     (NEC PC98) */
        KC_COLON           =0x92,    /*                     (NEC PC98) */
        KC_UNDERLINE       =0x93,    /*                     (NEC PC98) */
        KC_KANJI           =0x94,    /* (Japanese keyboard)            */
        KC_STOP            =0x95,    /*                     (NEC PC98) */
        KC_AX              =0x96,    /*                     (Japan AX) */
        KC_UNLABELED       =0x97,    /*                        (J3100) */
        KC_NEXTTRACK       =0x99,    /* Next Track */
        KC_NUMPADENTER     =0x9C,    /* Enter on numeric keypad */
        KC_RCONTROL        =0x9D,
        KC_MUTE            =0xA0,    /* Mute */
        KC_CALCULATOR      =0xA1,    /* Calculator */
        KC_PLAYPAUSE       =0xA2,    /* Play / Pause */
        KC_MEDIASTOP       =0xA4,    /* Media Stop */
        KC_VOLUMEDOWN      =0xAE,    /* Volume - */
        KC_VOLUMEUP        =0xB0,    /* Volume + */
        KC_WEBHOME         =0xB2,    /* Web home */
        KC_NUMPADCOMMA     =0xB3,    /* , on numeric keypad (NEC PC98) */
        KC_DIVIDE          =0xB5,    /* / on numeric keypad */
        KC_SYSRQ           =0xB7,
        KC_RMENU           =0xB8,    /* right Alt */
        KC_PAUSE           =0xC5,    /* Pause */
        KC_HOME            =0xC7,    /* Home on arrow keypad */
        KC_UP              =0xC8,    /* UpArrow on arrow keypad */
        KC_PGUP            =0xC9,    /* PgUp on arrow keypad */
        KC_LEFT            =0xCB,    /* LeftArrow on arrow keypad */
        KC_RIGHT           =0xCD,    /* RightArrow on arrow keypad */
        KC_END             =0xCF,    /* End on arrow keypad */
        KC_DOWN            =0xD0,    /* DownArrow on arrow keypad */
        KC_PGDOWN          =0xD1,    /* PgDn on arrow keypad */
        KC_INSERT          =0xD2,    /* Insert on arrow keypad */
        KC_DELETE          =0xD3,    /* Delete on arrow keypad */
        KC_LWIN            =0xDB,    /* Left Windows key */
        KC_RWIN            =0xDC,    /* Right Windows key */
        KC_APPS            =0xDD,    /* AppMenu key */
        KC_POWER           =0xDE,    /* System Power */
        KC_SLEEP           =0xDF,    /* System Sleep */
        KC_WAKE            =0xE3,    /* System Wake */
        KC_WEBSEARCH       =0xE5,    /* Web Search */
        KC_WEBFAVORITES    =0xE6,    /* Web Favorites */
        KC_WEBREFRESH      =0xE7,    /* Web Refresh */
        KC_WEBSTOP         =0xE8,    /* Web Stop */
        KC_WEBFORWARD      =0xE9,    /* Web Forward */
        KC_WEBBACK         =0xEA,    /* Web Back */
        KC_MYCOMPUTER      =0xEB,    /* My Computer */
        KC_MAIL            =0xEC,    /* Mail */
        KC_MEDIASELECT     =0xED     /* Media Select */
    };

    /** Structure representing a snapshot of the state of the mouse
        input controller. */
    struct _OgreExport MouseState
    {
        /** Absolute position of the mouse pointer. */
        long Xabs, Yabs, Zabs;
        /** Relative position of the mouse pointer. */
        long Xrel, Yrel, Zrel;
        /** The buttons that have been pressed. Each bit maps to a mouse
        button. */
        long Buttons;

        /** Retrieves the pressed state of a mouse button. */
        inline long isButtonDown( uchar button ) const
        {
            return Buttons & ( 1 << button );
        }
    };

    /** Abstract class which allows input to be read from various
        controllers.
        @remarks
            You can access an appropriate concrete subclass of this interface by
            calling PlatformManager::createInputReader.
        @warning Temporary implementation only. This class is likely to be
            refactored into a better design when I get time to look at it
            properly. For now it's a quick-and-dirty way to get what I need.
        @see
            PlatformManager::createInputReader
    */
    class _OgreExport InputReader
    {
    public:
        InputReader();
	    virtual ~InputReader();

	    /** Tells the reader to use buffered input and update the passed in queue.
        @remarks
            The default behaviour of the input reader is simply to capture the
            current state of the mouse / keyboard on demand. An alternative is to use 
            buffered input where all events are registered on a queue.
        */
	    void useBufferedInput(EventQueue* pEventQueue, bool keys = true, bool mouse = true) ;

	    virtual void setBufferedInput(bool keys, bool mouse) ;
			

		/** Initialise the input system.
            @note
                Only keyboard and mouse currently implemented.
            @param
                pWindow The window to capture input for
            @param
                useKeyboard If true, keyboard input will be supported.
            @param
                useMouse If true, mouse input will be supported.
            @param
                useGameController If true, joysticks/gamepads will be supported.
        */
        virtual void initialise( 
            RenderWindow* pWindow, 
            bool useKeyboard = true, 
            bool useMouse = true, 
            bool useGameController = false ) = 0;

        /** Captures the state of all the input devices.
            @remarks
                This method captures the state of all input devices and
                stores it internally for use when the enquiry methods are
                next called. This is done to ensure that all input is
                captured at once and therefore combinations of input are not
                subject to time differences when methods are called.

        */
        virtual void capture() = 0;

        /** Determines if the specified key is currently depressed.
            @note In immediate mode, this enquiry method uses the state of the 
                keyboard at the last 'capture' call.
        */
        virtual bool isKeyDown( KeyCode kc ) const;

        /** Retrieves the relative position of the mouse when capture was
            called relative to the last time. */
        virtual long getMouseRelativeX() const { return getMouseRelX(); }

        /** Retrieves the relative position of the mouse when capture was
            called relative to the last time. */
        virtual long getMouseRelativeY() const { return getMouseRelY(); }

        /** Retrieves the relative position of the mouse when capture was
            called relative to the last time. */
        virtual long getMouseRelativeZ() const { return getMouseRelZ(); }

        /** Retrieves the relative (compared to the last input poll) mouse movement
            on the X (horizontal) axis. */
        virtual long getMouseRelX() const = 0;

        /** Retrieves the relative (compared to the last input poll) mouse movement
            on the Y (vertical) axis. */
        virtual long getMouseRelY() const = 0;

        /** Retrieves the relative (compared to the last input poll) mouse movement
            on the Z (mouse wheel) axis. */
        virtual long getMouseRelZ() const = 0;

        /** Retrieves the absolute mouse position on the X (horizontal) axis. */
        virtual long getMouseAbsX() const = 0;
        /** Retrieves the absolute mouse position on the Y (vertical) axis. */
        virtual long getMouseAbsY() const = 0;
        /** Retrieves the absolute mouse position on the Z (mouse wheel) axis. */
        virtual long getMouseAbsZ() const = 0;

        /** Retrieves the current state of the mouse. */
        virtual void getMouseState( MouseState& state ) const = 0;

        /** Retrieves the state of a mouse button. */
        virtual bool getMouseButton( uchar button ) const = 0;

		/** Adds a mouse motion listener to the cursor object.
		    This keeps the Cursor object hidden. */
		void addCursorMoveListener( MouseMotionListener* c );
	
		/** Remove a mouse motion listener to the cursor object.
		    This keeps the Cursor object hidden. */
		void removeCursorMoveListener( MouseMotionListener* c );
		static char getKeyChar(int keyCode, long modifiers = 0);

		/** Set mouse scale factor.
		@param scale The new mouse scale (The default on is around 0.001/0.002).
		*/
		virtual void setMouseScale( Real scale ) { mMouseScale = scale; }
		
		/** Get mouse scale factor.
		*/
		virtual Real getMouseScale( void ) const { return mMouseScale; }
		


	protected:

		/** The modifiers are a binary flags that represent what buttons are pressed, 
            and what key modifiers are down (e.g. shift/alt). */
		long mModifiers;

		/// Speed of mouse
		Real mMouseScale;

		/** Internal Cursor object. 
            @remarks
                This is a mathematical representation of where the cursor is, it does 
                not draw a cursor.
            @see CursorGuiElement. */

		Cursor* mCursor;

		/** EventQueue is used for buffered input support. */
		EventQueue* mEventQueue;

		/** Wether to use buffering input support - buffering support relies on using 
            an EventQueue. 
            @see class EventQueue */
		bool mUseBufferedKeys, mUseBufferedMouse;

        /** The mouse state in immediate mode. */
        MouseState mMouseState;

        /// Set of all the keys currently depressed based on buffered input events
        typedef std::set<KeyCode> BufferedKeysDownSet;
        BufferedKeysDownSet mBufferedKeysDown;

		/** Creates mouse moved or dragged events depending if any button is pressed. */
		void mouseMoved();

		/** Creates a MouseEvent that first gets processed by the cursor, then gets 
            pushed on the queue. */
		void createMouseEvent(int id, int button);

		/** Creates mouse pressed, released, and clicked events. */
		void triggerMouseButton(int nMouseCode, bool mousePressed);

		void createKeyEvent(int id, int key);
		void keyChanged(int key, bool down);
		
		/** Return whether a key is down in immediate mode. */
        virtual bool isKeyDownImmediate( KeyCode kc ) const = 0;
    };


    /** Defines the interface a platform-specific library must implement.
        @remarks
            Any library (.dll, .so) wishing to implement a
            platform-specific version of this input reader must export the
            symbol 'createInputReader' with the signature void
            createPlatformInputReader(InputReader** ppReader).
    */
    typedef void (*DLL_CREATEINPUTREADER)(InputReader** ppReader);

}

#endif
