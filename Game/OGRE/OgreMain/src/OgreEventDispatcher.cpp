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

#include "OgreMouseEvent.h"
#include "OgreEventDispatcher.h"
#include "OgreTargetManager.h"
#include "OgreEventProcessor.h"
#include "OgrePositionTarget.h"
#include "OgreKeyEvent.h"

namespace Ogre {

    EventDispatcher::EventDispatcher(TargetManager* pTargetManager)
        :mTargetManager(pTargetManager)	// abstract this out TODO
    {
		mFocus = 0;
        mMouseDragSource = 0;
		mKeyCursorOn = 0;
		mEventMask = 0;
		mTargetLastEntered = 0;
        mMouseX = 0;
        mMouseY = 0;
		mDragging = false;
        mDragDropOn = false;
        mDragDropActive = false;
    }

    //---------------------------------------------------------------------
    EventDispatcher::~EventDispatcher()
    {
    }

    //---------------------------------------------------------------------
	bool EventDispatcher::dispatchEvent(InputEvent* e) 
	{
		bool ret = false;
		if (e->isEventBetween(MouseEvent::ME_FIRST_EVENT, MouseEvent::ME_LAST_EVENT))	// i am open to suggestions for a better way to do this
																						// maybe a method e->isEvent(InputEvent::MouseEvent) ??
		{
			MouseEvent* me = static_cast<MouseEvent*> (e);
			ret = processMouseEvent(me);
		}
		else if (e->isEventBetween(KeyEvent::KE_FIRST_EVENT, KeyEvent::KE_LAST_EVENT))
		{
			KeyEvent* ke = static_cast<KeyEvent*> (e);
			ret = processKeyEvent(ke);

		}
		return ret;
	}

    //---------------------------------------------------------------------
    void EventDispatcher::setDragDrop(bool dragDropOn)
    {
        mDragDropOn = dragDropOn;
    }

    //---------------------------------------------------------------------
	bool EventDispatcher::processKeyEvent(KeyEvent* e) 
	{
		if (mKeyCursorOn != 0)
		{
			mKeyCursorOn->processEvent(e);
		}
		return e->isConsumed();
	}
	
    //---------------------------------------------------------------------
	bool EventDispatcher::processMouseEvent(MouseEvent* e) 
	{
		PositionTarget* targetOver;

        mMouseX = e->getX();
        mMouseY = e->getY();

		targetOver = mTargetManager->getPositionTargetAt(e->getX(), e->getY());
		trackMouseEnterExit(targetOver, e);

		switch (e->getID())
		{		
		case MouseEvent::ME_MOUSE_PRESSED:
			mDragging = true;
            if (mDragDropOn)
                mDragDropActive = true;
			mMouseDragSource = targetOver;
    		retargetMouseEvent(targetOver, e);
            trackKeyEnterExit(targetOver, e);
			break;

		case MouseEvent::ME_MOUSE_RELEASED:
            if (targetOver != 0)
            {
			    if (targetOver == mMouseDragSource)
			    {
                    retargetMouseEvent(mMouseDragSource, MouseEvent::ME_MOUSE_CLICKED, e);
                    retargetMouseEvent(mMouseDragSource, e);
			    }
			    else // i.e. targetOver != mMouseDragSource
			    {
                    if (mDragDropActive)
                        retargetMouseEvent(targetOver, MouseEvent::ME_MOUSE_DRAGDROPPED, e);
                    retargetMouseEvent(mMouseDragSource, e);
				    retargetMouseEvent(targetOver, MouseEvent::ME_MOUSE_ENTERED, e);
			    }
            }
            else
                retargetMouseEvent(mMouseDragSource, e);

			mDragging = false;
            mDragDropActive = false;
            mMouseDragSource = 0;
			break;

		case MouseEvent::ME_MOUSE_MOVED:
		case MouseEvent::ME_MOUSE_DRAGGED:
            if (!mDragging || targetOver == mMouseDragSource)
            {
        		retargetMouseEvent(targetOver, e);
            }
            else // i.e. mDragging && targetOver != mMouseDragSource
            {
        		retargetMouseEvent(mMouseDragSource, MouseEvent::ME_MOUSE_DRAGGED, e, true);
                if (mDragDropActive)
        		    retargetMouseEvent(targetOver, MouseEvent::ME_MOUSE_DRAGMOVED, e);
            }
			break;
		}

		return e->isConsumed();
	}

    //---------------------------------------------------------------------
	void EventDispatcher::retargetMouseEvent(PositionTarget* target, MouseEvent* e) 
	{
		if (target == NULL)
		{
			return;
		}

		MouseEvent* retargeted = new MouseEvent(target,
											   e->getID(), 
											   e->getButtonID(),
											   e->getWhen(), 
											   e->getModifiers(),
											   e->getX(), 
											   e->getY(), 
											   e->getZ(),
											   e->getClickCount());

		target->processEvent(retargeted);		
		delete retargeted;
		
		e->consume();
	}

    //---------------------------------------------------------------------
	void EventDispatcher::retargetMouseEvent(PositionTarget* target, int id, MouseEvent* e, bool consume) 
	{
		if (target == NULL)
		{
			return;
		}

		MouseEvent* retargeted = new MouseEvent(target,
											   id, 
											   e->getButtonID(),
											   e->getWhen(), 
											   e->getModifiers(),
											   e->getX(), 
											   e->getY(), 
											   e->getZ(),
											   e->getClickCount());

		target->processEvent(retargeted);		
		delete retargeted;

        if (consume)
		    e->consume();
	}

    //---------------------------------------------------------------------
	void EventDispatcher::retargetKeyEvent(PositionTarget* target, int id, MouseEvent* e) 
	{
		if (target == NULL)
		{
			return;
		}

		KeyEvent* retargeted = new KeyEvent(target,
											   id,
                                               0,
											   e->getWhen(), 
											   e->getModifiers());

		target->processEvent(retargeted);		
		delete retargeted;
	}

    //---------------------------------------------------------------------
	void EventDispatcher::trackMouseEnterExit(PositionTarget* targetOver, MouseEvent* e) 
	{
		if (mTargetLastEntered == targetOver)
		{
			return;
		}

		if (mTargetLastEntered != 0)
		{
            if (!mDragging || mTargetLastEntered == mMouseDragSource)
            {
			    retargetMouseEvent(mTargetLastEntered, MouseEvent::ME_MOUSE_EXITED, e);
            }
            else if (mDragDropActive) // i.e. mDragging && mTargetLastEntered != mMouseDragSource && mDragDropActive
            {
		        retargetMouseEvent(mTargetLastEntered, MouseEvent::ME_MOUSE_DRAGEXITED, e);
            }
		}

		if (targetOver != 0)
		{
            if (!mDragging || targetOver == mMouseDragSource)
            {
			    retargetMouseEvent(targetOver, MouseEvent::ME_MOUSE_ENTERED, e);
            }
            else if (mDragDropActive) // i.e. mDragging && targetOver != mMouseDragSource && mDragDropActive
            {
		        retargetMouseEvent(targetOver, MouseEvent::ME_MOUSE_DRAGENTERED, e);
            }
		}

        mTargetLastEntered = targetOver;
	}

    //---------------------------------------------------------------------
    void EventDispatcher::trackKeyEnterExit(PositionTarget* targetOver, MouseEvent* e)
    {
        if (targetOver != mKeyCursorOn)
        {
            if (mKeyCursorOn != 0)
            {
                retargetKeyEvent(mKeyCursorOn, KeyEvent::KE_KEY_FOCUSOUT, e);
            }

			if (targetOver != 0 && targetOver->isKeyEnabled())
			{
				mKeyCursorOn = targetOver;
                retargetKeyEvent(targetOver, KeyEvent::KE_KEY_FOCUSIN, e);
			}
			else
			{
				mKeyCursorOn = NULL;
			}
        }
    }

}



