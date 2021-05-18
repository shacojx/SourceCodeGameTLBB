/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://ogre.sourceforge.net/

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

#include "GTKInput.h"
#include "OgreInputEvent.h"
#include "OgreRenderWindow.h"

#include <sigc++/slot.h>

#include <gdk/gdkkeysyms.h>
#include <gdk/gdkx.h>
#include <gdkmm/cursor.h>

using namespace Ogre;

GTKInput::GTKInput() : InputReader(),
	_win(0),
	_widget(0),
	_kit(0)

{
    mEventQueue = 0;
    mScale = 0.002;
    _visible = false;

    _key_map.insert(InputKeyMap::value_type(GDK_Escape,KC_ESCAPE));
    _key_map.insert(InputKeyMap::value_type(GDK_1, KC_1));
    _key_map.insert(InputKeyMap::value_type(GDK_2, KC_2));
    _key_map.insert(InputKeyMap::value_type(GDK_3, KC_3));
    _key_map.insert(InputKeyMap::value_type(GDK_4, KC_4));
    _key_map.insert(InputKeyMap::value_type(GDK_5, KC_5));
    _key_map.insert(InputKeyMap::value_type(GDK_6, KC_6));
    _key_map.insert(InputKeyMap::value_type(GDK_7, KC_7));
    _key_map.insert(InputKeyMap::value_type(GDK_8, KC_8));
    _key_map.insert(InputKeyMap::value_type(GDK_9, KC_9));
    _key_map.insert(InputKeyMap::value_type(GDK_0, KC_0));
    _key_map.insert(InputKeyMap::value_type(GDK_minus, KC_MINUS));
    _key_map.insert(InputKeyMap::value_type(GDK_equal, KC_EQUALS));
    _key_map.insert(InputKeyMap::value_type(GDK_BackSpace, KC_BACK));
    _key_map.insert(InputKeyMap::value_type(GDK_Tab, KC_TAB));
    _key_map.insert(InputKeyMap::value_type(GDK_q, KC_Q));
    _key_map.insert(InputKeyMap::value_type(GDK_w, KC_W));
    _key_map.insert(InputKeyMap::value_type(GDK_e, KC_E));
    _key_map.insert(InputKeyMap::value_type(GDK_r, KC_R));
    _key_map.insert(InputKeyMap::value_type(GDK_t, KC_T));
    _key_map.insert(InputKeyMap::value_type(GDK_y, KC_Y));
    _key_map.insert(InputKeyMap::value_type(GDK_u, KC_U));
    _key_map.insert(InputKeyMap::value_type(GDK_i, KC_I));
    _key_map.insert(InputKeyMap::value_type(GDK_o, KC_O));
    _key_map.insert(InputKeyMap::value_type(GDK_p, KC_P));
    _key_map.insert(InputKeyMap::value_type(GDK_Return, KC_RETURN));
    _key_map.insert(InputKeyMap::value_type(GDK_Control_L, KC_LCONTROL));
    _key_map.insert(InputKeyMap::value_type(GDK_a, KC_A));
    _key_map.insert(InputKeyMap::value_type(GDK_s, KC_S));
    _key_map.insert(InputKeyMap::value_type(GDK_d, KC_D));
    _key_map.insert(InputKeyMap::value_type(GDK_f, KC_F));
    _key_map.insert(InputKeyMap::value_type(GDK_g, KC_G));
    _key_map.insert(InputKeyMap::value_type(GDK_h, KC_H));
    _key_map.insert(InputKeyMap::value_type(GDK_j, KC_J));
    _key_map.insert(InputKeyMap::value_type(GDK_k, KC_K));
    _key_map.insert(InputKeyMap::value_type(GDK_l, KC_L));
    _key_map.insert(InputKeyMap::value_type(GDK_Shift_L, KC_LSHIFT));
    _key_map.insert(InputKeyMap::value_type(GDK_backslash, KC_BACKSLASH));
    _key_map.insert(InputKeyMap::value_type(GDK_z, KC_Z));
    _key_map.insert(InputKeyMap::value_type(GDK_x, KC_X));
    _key_map.insert(InputKeyMap::value_type(GDK_c, KC_C));
    _key_map.insert(InputKeyMap::value_type(GDK_v, KC_V));
    _key_map.insert(InputKeyMap::value_type(GDK_b, KC_B));
    _key_map.insert(InputKeyMap::value_type(GDK_n, KC_N));
    _key_map.insert(InputKeyMap::value_type(GDK_m, KC_M));
    _key_map.insert(InputKeyMap::value_type(GDK_comma, KC_COMMA));
    _key_map.insert(InputKeyMap::value_type(GDK_period, KC_PERIOD));
    _key_map.insert(InputKeyMap::value_type(GDK_Shift_R, KC_RSHIFT));
    _key_map.insert(InputKeyMap::value_type(GDK_KP_Multiply, KC_MULTIPLY));
    _key_map.insert(InputKeyMap::value_type(GDK_Alt_L, KC_LMENU));
    _key_map.insert(InputKeyMap::value_type(GDK_space, KC_SPACE));
    _key_map.insert(InputKeyMap::value_type(GDK_Caps_Lock, KC_CAPITAL));
    _key_map.insert(InputKeyMap::value_type(GDK_F1, KC_F1));
    _key_map.insert(InputKeyMap::value_type(GDK_F2, KC_F2));
    _key_map.insert(InputKeyMap::value_type(GDK_F3, KC_F3));
    _key_map.insert(InputKeyMap::value_type(GDK_F4, KC_F4));
    _key_map.insert(InputKeyMap::value_type(GDK_F5, KC_F5));
    _key_map.insert(InputKeyMap::value_type(GDK_F6, KC_F6));
    _key_map.insert(InputKeyMap::value_type(GDK_F7, KC_F7));
    _key_map.insert(InputKeyMap::value_type(GDK_F8, KC_F8));
    _key_map.insert(InputKeyMap::value_type(GDK_F9, KC_F9));
    _key_map.insert(InputKeyMap::value_type(GDK_F10, KC_F10));
    _key_map.insert(InputKeyMap::value_type(GDK_Num_Lock, KC_NUMLOCK));
    _key_map.insert(InputKeyMap::value_type(GDK_Scroll_Lock, KC_SCROLL));
    _key_map.insert(InputKeyMap::value_type(GDK_KP_7, KC_NUMPAD7));
    _key_map.insert(InputKeyMap::value_type(GDK_KP_8, KC_NUMPAD8));
    _key_map.insert(InputKeyMap::value_type(GDK_KP_9, KC_NUMPAD9));
    _key_map.insert(InputKeyMap::value_type(GDK_KP_Subtract, KC_SUBTRACT));
    _key_map.insert(InputKeyMap::value_type(GDK_KP_4, KC_NUMPAD4));
    _key_map.insert(InputKeyMap::value_type(GDK_KP_5, KC_NUMPAD5));
    _key_map.insert(InputKeyMap::value_type(GDK_KP_6, KC_NUMPAD6));
    _key_map.insert(InputKeyMap::value_type(GDK_KP_Add, KC_ADD));
    _key_map.insert(InputKeyMap::value_type(GDK_KP_1, KC_NUMPAD1));
    _key_map.insert(InputKeyMap::value_type(GDK_KP_2, KC_NUMPAD2));
    _key_map.insert(InputKeyMap::value_type(GDK_KP_3, KC_NUMPAD3));
    _key_map.insert(InputKeyMap::value_type(GDK_KP_0, KC_NUMPAD0));
    _key_map.insert(InputKeyMap::value_type(GDK_KP_Decimal, KC_DECIMAL));
    _key_map.insert(InputKeyMap::value_type(GDK_F11, KC_F11));
    _key_map.insert(InputKeyMap::value_type(GDK_F12, KC_F12));
    _key_map.insert(InputKeyMap::value_type(GDK_F13, KC_F13));
    _key_map.insert(InputKeyMap::value_type(GDK_F14, KC_F14));
    _key_map.insert(InputKeyMap::value_type(GDK_F15, KC_F15));
    _key_map.insert(InputKeyMap::value_type(GDK_KP_Equal, KC_NUMPADEQUALS));
    _key_map.insert(InputKeyMap::value_type(GDK_KP_Divide, KC_DIVIDE));
    _key_map.insert(InputKeyMap::value_type(GDK_Sys_Req, KC_SYSRQ));
    _key_map.insert(InputKeyMap::value_type(GDK_Alt_R, KC_RMENU));
    _key_map.insert(InputKeyMap::value_type(GDK_Home, KC_HOME));
    _key_map.insert(InputKeyMap::value_type(GDK_Up, KC_UP));
    _key_map.insert(InputKeyMap::value_type(GDK_Page_Up, KC_PGUP));
    _key_map.insert(InputKeyMap::value_type(GDK_Left, KC_LEFT));
    _key_map.insert(InputKeyMap::value_type(GDK_Right, KC_RIGHT));
    _key_map.insert(InputKeyMap::value_type(GDK_End, KC_END));
    _key_map.insert(InputKeyMap::value_type(GDK_Down, KC_DOWN));
    _key_map.insert(InputKeyMap::value_type(GDK_Page_Down, KC_PGDOWN));
    _key_map.insert(InputKeyMap::value_type(GDK_Insert, KC_INSERT));
    _key_map.insert(InputKeyMap::value_type(GDK_Delete, KC_DELETE));
    _key_map.insert(InputKeyMap::value_type(GDK_Super_L, KC_LWIN));
    _key_map.insert(InputKeyMap::value_type(GDK_Super_R, KC_RWIN));

    for(InputKeyMap::iterator it = _key_map.begin(); 
        it != _key_map.end(); ++it)
    {
        _rkey_map.insert(RInputKeyMap::value_type(it->second, it->first));
    }
}

GTKInput::~GTKInput()
{
    	// XXX Do more?
	// It's not needed to detach the various signals here, that
	// happens automatically.
}

void GTKInput::initialise(RenderWindow* pWindow, bool useKeyboard,
                          bool useMouse, bool useGameController)
{
    	_kit = Gtk::Main::instance();
   	// Extract Window and DrawingArea from pWindow in magic way
    	pWindow->getCustomAttribute("GTKMMWINDOW", &_win);
    	pWindow->getCustomAttribute("GTKGLMMWIDGET", &_widget);

	// Do mouse capture only when this is stated
	mUseMouse = useMouse;

	if(mUseMouse) {
		unsigned int w, h, d;
		int l, t;
    		pWindow->getMetrics(w, h, d, l, t);

    		mMouseCenterX = w / 2;
    		mMouseCenterY = h / 2;

    		Glib::RefPtr<Gdk::Window> win = _win->get_window();
    		Gdk::Color col;
    		char invisible_cursor_bits[] = { 0x0 };
    		Glib::RefPtr<Gdk::Pixmap> nada = Gdk::Pixmap::create_from_data(win,
        		invisible_cursor_bits, 1, 1, 1, col, col);

    		win->set_cursor(Gdk::Cursor(nada, nada, col, col, 0, 0));
	}

	/**
    	 * Connect key handlers before any other
	 */
    	_win->signal_key_press_event().connect(
        	SigC::slot(*this, &GTKInput::on_key_press), false);
    	_win->signal_key_release_event().connect(
        	SigC::slot(*this, &GTKInput::on_key_release), false);

	if(mUseMouse) {
	    	_widget->signal_motion_notify_event().connect(
        		SigC::slot(*this, &GTKInput::on_mouse_motion));
	    	_widget->signal_button_press_event().connect(
        		SigC::slot(*this, &GTKInput::on_button_press));
	    	_widget->signal_button_release_event().connect(
        		SigC::slot(*this, &GTKInput::on_button_release));
	}
}

void GTKInput::capture()
{
    _visible = true;
    // We pump it a few times to make sure we get smooth operation
    for (int x = 0; x < 10; ++x)
    {
    	if (_kit->events_pending())
    	{
        	_kit->iteration(false);
	} else break;
    }
    _captured_keys = _cur_keys;

    	if (mUseMouse) {
		if (!mUseBufferedMouse) {
        		mMouseState.Xabs = mMouseX;
        		mMouseState.Yabs = mMouseY;
        		mMouseState.Zabs = 0;

        		mMouseState.Xrel = mMouseX - mMouseCenterX;
        		mMouseState.Yrel = mMouseY - mMouseCenterY;
        		mMouseState.Zrel = 0;
    		}

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
    		XWarpPointer(GDK_WINDOW_XDISPLAY(_win->get_window()->gobj()), None,
                	 GDK_WINDOW_XWINDOW(_win->get_window()->gobj()), 0, 0, 0, 0, 
                 	mMouseCenterX, mMouseCenterY);
#endif

    		mMouseX = mMouseCenterX;
    		mMouseY = mMouseCenterY;
	}
}

bool GTKInput::isKeyDownImmediate( KeyCode kc ) const
{
    RInputKeyMap::const_iterator it = _rkey_map.find(kc);
    // Make sure it's a mappable key
    if (it == _rkey_map.end())
        return false;

    if (std::count(_captured_keys.begin(), _captured_keys.end(), it->second))
        return true;
    return false;
}

long GTKInput::getMouseRelX() const
{
    return mMouseState.Xrel;
}

long GTKInput::getMouseRelY() const
{
    return mMouseState.Yrel;
}

long GTKInput::getMouseRelZ() const
{
    return 0;
}

long GTKInput::getMouseAbsX() const
{
    return mMouseState.Xabs;
}

long GTKInput::getMouseAbsY() const
{
    return mMouseState.Yabs;
}

long GTKInput::getMouseAbsZ() const
{
    return 0;
}

bool GTKInput::getMouseButton( uchar button ) const
{
    return mMouseState.isButtonDown( button );
}

void GTKInput::getMouseState( MouseState& state) const
{
    state = mMouseState;
}

bool GTKInput::on_mouse_motion(GdkEventMotion* event)
{
    // Skip warps, might hit a regular movement, but that's acceptable
    if (event->x == mMouseCenterX && event->y == mMouseCenterY)
    {
        return false;
    }

    mMouseX = static_cast<int>(event->x);
    mMouseY = static_cast<int>(event->y);

    //printf("MOUSE AT: %d,%d\n", mMouseX, mMouseY);

    if (mUseBufferedMouse)
    {
        mouseMoved();
    }

    return false;
}

bool GTKInput::on_key_press(GdkEventKey* event)
{
    _cur_keys.insert(event->keyval);
    if (mUseBufferedKeys)
        keyChanged(_key_map[event->keyval], true);

    return false;
}

bool GTKInput::on_key_release(GdkEventKey* event)
{ 
    _cur_keys.erase(event->keyval);
    if (mUseBufferedKeys)
        keyChanged(_key_map[event->keyval], false);

    return false;
}

bool GTKInput::on_button_press(GdkEventButton* event)
{
    int button;

    switch(event->button)
    {
    case 1:
        button = InputEvent::BUTTON0_MASK;
        break;
    case 2:
        button = InputEvent::BUTTON2_MASK;
        break;
    case 3:
        button = InputEvent::BUTTON1_MASK;
        break;
    };

    if (mUseBufferedMouse)
        triggerMouseButton(button, true);

    return false;
}

bool GTKInput::on_button_release(GdkEventButton* event)
{
    int button;

    switch(event->button)
    {
    case 1:
        button = InputEvent::BUTTON0_MASK;
        break;
    case 2:
        button = InputEvent::BUTTON2_MASK;
        break;
    case 3:
        button = InputEvent::BUTTON1_MASK;
        break;
    };

    if (mUseBufferedMouse)
        triggerMouseButton(button, false);

    return false;
}
