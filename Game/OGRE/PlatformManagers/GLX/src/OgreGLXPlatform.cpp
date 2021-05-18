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

#include "OgreRoot.h"
#include "OgreGLXConfig.h"
#include "OgreGLXError.h"
#include "OgreGLXInput.h"
#include "OgreGLXTimer.h"

#include "OgreGLXWindow.h"

//Kinda a hack. but neccessary to know if any OgreInput has/is being used
int GLXInputCreated = 0;

namespace Ogre {
// Platform factory functions
extern "C" void createPlatformConfigDialog(ConfigDialog** ppDlg) {
	*ppDlg = new GLXConfig();
}

extern "C" void createPlatformErrorDialog(ErrorDialog** ppDlg) {
	*ppDlg = new GLXError();
}

extern "C" void createPlatformInputReader(InputReader** ppDlg) {
	*ppDlg = new GLXInput();
	++GLXInputCreated;
}

extern "C" void createTimer(Timer** ppTimer) {
	*ppTimer = new GLXTimer();
	(*ppTimer)->reset();
}

extern "C" void destroyTimer(Timer* ppTimer) {
	delete ppTimer;
}

extern "C" void destroyPlatformConfigDialog(ConfigDialog* dlg) {
	delete dlg;
}

extern "C" void destroyPlatformErrorDialog(ErrorDialog* dlg) {
	delete dlg;
}

extern "C" void destroyPlatformRenderWindow(RenderWindow* wnd) {
	delete wnd;
}

extern "C" void destroyPlatformInputReader(InputReader* reader) {
	if( reader )
		--GLXInputCreated;
	delete reader;
}

extern "C" void messagePump(RenderWindow* rw) {
	//Do not do this if the GLXInput is also pumping events - hopefully Input will be removed
	//from Ogre soon...
	if(GLXInputCreated > 0)
		return;
	
	//Pump X Events
	GLXWindow *w = static_cast<GLXWindow*>(rw);
	Display* dis = w->getXDisplay();
	XEvent event;

	// Process X events until event pump exhausted
	while(XPending(dis) > 0) 
	{
		XNextEvent(dis,&event);
		w->injectXEvent(event);
	}
}

}
