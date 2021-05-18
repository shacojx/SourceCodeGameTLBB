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

#ifndef __GLXInputReader_H__
#define __GLXInputReader_H__

#include "OgreInput.h"
#include "OgreInputEvent.h"
#include "OgreRenderWindow.h"

#include <map>
#include <set>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/Xproto.h>
namespace Ogre {
class GLXInput : public InputReader {
public:
	GLXInput();
	virtual ~GLXInput();

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

private:
	CARD32 mHiddenCursor;

	// Map of X keys -> Ogre keys
	typedef std::map<KeySym, KeyCode> InputKeyMap;
	InputKeyMap _key_map;

	// Map of currently pressed keys
	typedef std::set<KeyCode> KeyPressedSet;
	KeyPressedSet _key_pressed_set;

	// Does the ogre input system want to capture the mouse (set with useMouse flag
	// on initialise)
	bool captureMouse;
	// Is mouse currently warped (captured within window?)
	bool warpMouse;
	int mouseLastX, mouseLastY;

	// Display and window, for event pumping
	Display *mDisplay;
	Window mWindow;
	RenderWindow *mRenderWindow;
	bool mMouseWarped;

	static const unsigned int mWheelStep = 100;

	void GrabCursor(bool grab);

	bool isKeyDownImmediate( KeyCode kc ) const;

};

}; // Namespace

#endif

