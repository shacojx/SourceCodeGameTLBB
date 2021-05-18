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

/***************************************************************************
OgreEventDispatcher.h  -  
	Handles the Dispatching of events to a EventTarget (mainly a PositionTarget for 
	mouse moving). The EventTargets are managed by a target manager.

	A EventDispatcher is needed for each TargetManager. ie 1 dispatcher for the OverlayManager,
	for managing the 2D GUI components, and another EventDispatcher for a SceneManager 
	managing the 3D objects.(NOTE currently the SceneManager isn't a TargetManager.. this is a TODO).

-------------------
begin                : Nov 19 2002
copyright            : (C) 2002 by Kenny Sabir
email                : kenny@sparksuit.com
***************************************************************************/



#ifndef __EventDispatcher_H__
#define __EventDispatcher_H__

#include "OgrePrerequisites.h"

namespace Ogre {

	/**
		Handles the Dispatching of events to a EventTarget (mainly a PositionTarget for 
		mouse moving). 
	@remarks
		A EventDispatcher is needed for each TargetManager. ie 1 dispatcher for the OverlayManager,
		for managing the 2D GUI components, and another EventDispatcher for a SceneManager 
		managing the 3D objects.(NOTE currently the SceneManager isn't a TargetManager.. this is a TODO).
	*/
    class _OgreExport EventDispatcher
    {
    protected:
	    TargetManager* mTargetManager;		

	    /**
	    * The current PositionTarget that has focus that is being
	    * hosted by this GuiContainer.  If this is a null reference then 
	    * there is currently no focus on a PositionTarget being 
	    * hosted by this GuiContainer 
	    */
	    PositionTarget* mFocus;	// isn't implemented yet

	    /**
	    * The current PositionTarget, over which, the current drag operation originated.
        * Only valid when mDragging is true.  May be null.
	    */
	    PositionTarget* mMouseDragSource;

	    /**
	    * PositionTarget the Keycursor is on 
	    */
	    PositionTarget* mKeyCursorOn;	

	    /**
	    * The last PositionTarget entered
	    */
            PositionTarget* mTargetLastEntered;

        /**
        *  Screen coordinates of the last (or current) mouse event.  Will eventually be
        *  useful for managing mouse cursors.
        */
        Real mMouseX;
        Real mMouseY;

	    /**
	    * Indicates if the mouse pointer is currently being dragged...
	    * this is needed because we may receive exit events while dragging
	    * and need to keep the current mouse target in this case.
	    */
	    bool mDragging;       // i.e. mouse grab

	    /**
	    * Indicates whether or not Drag/Drop event will be generated during the
        * next drag sequence.
        *
        * @see setDragDrop
	    */
        bool mDragDropOn;

        /**
	    * Indicates whether or not Drag/Drop events are currently being generated.
	    */
        bool mDragDropActive;

	    int mEventMask;

	    //-------------------
	    // protected methods	

	    bool processKeyEvent(KeyEvent* e) ;

	    /**
	    * This method attempts to distribute a mouse event to a lightweight
	    * PositionTarget.  It tries to avoid doing any unnecessary probes down
	    * into the PositionTarget tree to minimize the overhead of determining
	    * where to route the event, since mouse movement events tend to
	    * come in large and frequent amounts.
	    */

	    bool processMouseEvent(MouseEvent* e) ;

	    /**
	    * Sends a mouse event to the current mouse event recipient using
	    * the given event (sent to the windowed host) as a srcEvent.  If
	    * the mouse event target is still in the PositionTarget tree, the 
	    * coordinates of the event are translated to those of the target.
	    * If the target has been removed, we don't bother to send the
	    * message.
	    */

	    void retargetMouseEvent(PositionTarget* target, MouseEvent* e) ;

	    /**
	    * Sends a mouse event to the current mouse event recipient using
	    * the given event (sent to the windowed host) as a srcEvent.  If
	    * the mouse event target is still in the PositionTarget tree, the 
	    * coordinates of the event are translated to those of the target.
	    * If the target has been removed, we don't bother to send the
	    * message.
	    */

	    void retargetMouseEvent(PositionTarget* target, int id, MouseEvent* e, bool consume = false) ;

	    /**
	    * Sends a key event to the current mouse event recipient using
	    * the given event (sent to the windowed host) as a srcEvent.
	    */

	    void retargetKeyEvent(PositionTarget* target, int id, MouseEvent* e) ;

	    /*
	    * Generates enter/exit events as mouse moves over lw PositionTargets
	    * @param targetOver	Target mouse is over (including native container)
	    * @param e			Mouse event in native container
	    */

	    void trackMouseEnterExit(PositionTarget* targetOver, MouseEvent* e) ;

        /*
	    * Generates FocusIn/FocusOut events as the keyboard focus changes.
	    * @param targetOver Target mouse is over (including native container)
	    * @param e			 Mouse event in native container
	    */
        void trackKeyEnterExit(PositionTarget* targetOver, MouseEvent* e);

    public:
	    EventDispatcher(TargetManager* pTargetManager);
        virtual ~EventDispatcher();       

	    /**
	    * Dispatches an event to a PositionTarget if necessary, and
	    * returns whether or not the event was forwarded to a 
	    * sub-PositionTarget.
	    *
	    * @param e the event
	    */
	    bool dispatchEvent(InputEvent* e);

	    /**
	    * Enables or disables the mouse drag/drop events on the next mouse
        * drag sequence.   It has no effect on the current drag sequence, if any.
        * Drag/drop events are typically enabled when the mouse enters the area
        * of a particular PositionTarget.
	    *
	    * @param dragDropOn Indicates whether or not the drag/drop events should
        * be enabled.
	    */
        void setDragDrop(bool dragDropOn);

        /**
	    * Returns the X screen coordinate of the current mouse position.
	    */
        inline Real getMouseX() const { return mMouseX; }
        /**
	    * Returns the Y screen coordinate of the current mouse position.
	    */
        inline Real getMouseY() const { return mMouseY; }
    };

}


#endif 
