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
OgreEventListeners.h  -  
	This file contains definatitions of all the different EventListeners.
	They are all included in 1 file, due to their small size.

	They all inherit from an abstract base listener called EventListener.

	So far we have the following listeners:
		MouseListener - handles mouse buttons clicked, pressed released,
						and mouse entered,exited, 
		MouseMotionListener - handles mouse move and mouse drag
		ActionListener - Used for buttons, when pressed and released.

-------------------
begin                : Nov 19 2002
copyright            : (C) 2002 by Kenny Sabir
email                : kenny@sparksuit.com
***************************************************************************/

#ifndef __MouseListener_H__
#define __MouseListener_H__

#include "OgrePrerequisites.h"
#include "OgreMouseEvent.h"

namespace Ogre {

    class _OgreExport EventListener 
    {
	public:
		virtual ~EventListener() {}
	};
    
	/** Specialised EventListener for discrete mouse events.
	@remarks
		This excludes mouse motion (this is not a discrete event), see
		MouseMotionListener for that.
	*/
	class _OgreExport MouseListener : public EventListener
    {
	public :
		/**
		 * Invoked when the mouse has been clicked on a component.
		 */
		virtual void mouseClicked(MouseEvent* e) = 0;
		/**
		 * Invoked when the mouse enters a component.
		 */
		virtual void mouseEntered(MouseEvent* e) = 0;
		/**
		 * Invoked when the mouse exits a component.
		 */
		virtual void mouseExited(MouseEvent* e) = 0;

		/**
		 * Invoked when a mouse button has been pressed on a component.
		 */
		virtual void mousePressed(MouseEvent* e) = 0;
		/**
		 * Invoked when a mouse button has been released on a component.
		 */
		virtual void mouseReleased(MouseEvent* e) = 0;

        virtual void mouseDragEntered(MouseEvent* e) {};
        virtual void mouseDragExited(MouseEvent* e) {};
        virtual void mouseDragDropped(MouseEvent* e) {};

    };


	class _OgreExport KeyListener : public EventListener
    {
    protected:

	public :
		/**
		 * Invoked when the key has been clicked on a component.
		 */
		virtual void keyClicked(KeyEvent* e) = 0;
		/**
		 * Invoked when a key button has been pressed on a component.
		 */
		virtual void keyPressed(KeyEvent* e) = 0;
		/**
		 * Invoked when a key button has been released on a component.
		 */
		virtual void keyReleased(KeyEvent* e) = 0;
		/**
		 * Invoked when the target receives the keyboard focus
		 */
        virtual void keyFocusIn(KeyEvent* e) {}
		/**
		 * Invoked when the target loses the keyboard focus
		 */
        virtual void keyFocusOut(KeyEvent* e) {}

    };

	/** Specialised EventListener for mouse motion. */
	class _OgreExport MouseMotionListener : public EventListener
    {
    protected:

	public :
		/**
		 * Invoked when the mouse has been moved
		 */
		virtual void mouseMoved(MouseEvent* e) = 0;
		/**
		 * Invoked when the mouse dragged
		 */
		virtual void mouseDragged(MouseEvent* e) = 0;
        /**
         * sent to target
         */
        virtual void mouseDragMoved(MouseEvent* e) {};
    };



}


#endif  // __MouseListener_H__
