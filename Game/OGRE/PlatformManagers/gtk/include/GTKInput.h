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

#ifndef INCL_GTKINPUT_H
#define INCL_GTKINPUT_H

#include "OgreInput.h"

#include <gdk/gdkevents.h>
#include <gtkmm/window.h>
#include <gtkmm/main.h>
#include <gtkmm/gl/drawingarea.h>

#include <sigc++/object.h>

namespace Ogre {

class GTKInput : public InputReader, public SigC::Object
{
public:
    GTKInput();
    virtual ~GTKInput();

    void initialise( RenderWindow* pWindow, bool useKeyboard = true, bool useMouse = true, bool useGameController = false );
    void capture();


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

protected:
    bool on_mouse_motion(GdkEventMotion* event);
    bool on_key_press(GdkEventKey* event);
    bool on_key_release(GdkEventKey* event);
    bool on_button_press(GdkEventButton* event);
    bool on_button_release(GdkEventButton* event);
    bool isKeyDownImmediate( KeyCode kc ) const;

private:
    // Capture mouse?
    bool mUseMouse;
    int mMouseX, mMouseY;
    int mMouseCenterX, mMouseCenterY;
    
    Real mScale;
    unsigned int mMouseKeys;
    bool _visible;
    typedef std::map<guint, KeyCode> InputKeyMap;
    InputKeyMap _key_map;
    typedef std::map<KeyCode, guint> RInputKeyMap;
    RInputKeyMap _rkey_map;
    typedef std::set<guint> CurKeySet;
    CurKeySet _cur_keys;
    CurKeySet _captured_keys;
    //GTKWindow* _win;
    Gtk::Window *_win;
    Gtk::GL::DrawingArea * _widget;
    Gtk::Main *_kit;
}; // class GTKInput

}; // namespace Ogre

#endif // INCL_GTKINPUT_H
