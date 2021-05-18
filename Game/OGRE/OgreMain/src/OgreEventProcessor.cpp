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

#include "OgreEventProcessor.h"
#include "OgreEventDispatcher.h"
#include "OgreEventQueue.h"
#include "OgreRoot.h"
#include "OgreMouseEvent.h"
#include "OgreKeyEvent.h"
#include "OgreInput.h"
#include "OgreCursor.h"
#include "OgrePlatformManager.h"


namespace Ogre {
    //-----------------------------------------------------------------------
    template<> EventProcessor* Singleton<EventProcessor>::ms_Singleton = 0;
    EventProcessor* EventProcessor::getSingletonPtr(void)
    {
        return ms_Singleton;
    }
    EventProcessor& EventProcessor::getSingleton(void)
    {  
        assert( ms_Singleton );  return ( *ms_Singleton );  
    }
    //-----------------------------------------------------------------------
//-----------------------------------------------------------------------------
    EventProcessor::EventProcessor() :
		MouseTarget(),
		MouseMotionTarget()
    {
		mEventQueue = 0;
		mInputDevice = 0;
        mRegisteredAsFrameListener = false;
    }

//-----------------------------------------------------------------------------
    EventProcessor::~EventProcessor()
    {
        // just in case more frames are still being rendered
        // and was registered as a FrameListener.
        stopProcessingEvents();
		cleanup();
    }

//-----------------------------------------------------------------------------
	void EventProcessor::cleanup()
	{
		if (mEventQueue)
			delete mEventQueue;

        for(DispatcherList::iterator i = mDispatcherList.begin(); i != mDispatcherList.end(); ++i )                 
        {
			delete *i;
        }
		mDispatcherList.clear();

		PlatformManager::getSingleton().destroyInputReader(mInputDevice);

	}

//-----------------------------------------------------------------------------
	void EventProcessor::stopProcessingEvents()
	{

		mEventQueue->activateEventQueue(false);

        if(mRegisteredAsFrameListener)
        {
		    Root::getSingleton().removeFrameListener(this);
            mRegisteredAsFrameListener = false;
        }

	}

//-----------------------------------------------------------------------------
	void EventProcessor::initialise(RenderWindow* ren)
	{
		cleanup();


		mEventQueue = new EventQueue();

		mInputDevice = PlatformManager::getSingleton().createInputReader();
		mInputDevice->useBufferedInput(mEventQueue);
		mInputDevice->initialise(ren,true, true, false);	

	}
//-----------------------------------------------------------------------------

	void EventProcessor::addTargetManager(TargetManager* targetManager)
	{
		EventDispatcher* pDispatcher = new EventDispatcher(targetManager);	
		mDispatcherList.push_back(pDispatcher);
	}

    //-----------------------------------------------------------------------------
	void EventProcessor::addEventTarget(EventTarget* eventTarget)
	{
		mEventTargetList.push_back(eventTarget);
	}


//-----------------------------------------------------------------------------
	void EventProcessor::startProcessingEvents(bool registerListener)
	{
        if(registerListener)
        {
		    Root::getSingleton().addFrameListener(this);
            mRegisteredAsFrameListener = true;
        }

		mEventQueue->activateEventQueue(true);
	}


//-----------------------------------------------------------------------------
	bool EventProcessor::frameStarted(const FrameEvent& evt)
	{
		mInputDevice->capture();

		while (mEventQueue->getSize() > 0)
		{
			InputEvent* e = mEventQueue->pop();
            processEvent(e);
			delete e;
		}

		return true;
	}

//-----------------------------------------------------------------------------
	void EventProcessor::processEvent(InputEvent* e)
	{
            // try the event dispatcher list
    	for (DispatcherList::iterator i = mDispatcherList.begin(); i != mDispatcherList.end(); ++i )                 
	    {
			(*i)->dispatchEvent(e);
		}

            // try the event target list
        if (!e->isConsumed())
        {
            EventTargetList::iterator i, iEnd;

            iEnd = mEventTargetList.end();
    	    for (i = mEventTargetList.begin(); i != iEnd; ++i )                 
	        {
			    (*i)->processEvent(e);
		    }
        }

        if (!e->isConsumed())
        {
		    switch(e->getID()) 
		    {
		    case MouseEvent::ME_MOUSE_PRESSED:
		    case MouseEvent::ME_MOUSE_RELEASED:
		    case MouseEvent::ME_MOUSE_CLICKED:
		    case MouseEvent::ME_MOUSE_ENTERED:
		    case MouseEvent::ME_MOUSE_EXITED:
		    case MouseEvent::ME_MOUSE_DRAGENTERED:
		    case MouseEvent::ME_MOUSE_DRAGEXITED:
		    case MouseEvent::ME_MOUSE_DRAGDROPPED:
			    processMouseEvent(static_cast<MouseEvent*>(e));
			    break;
		    case MouseEvent::ME_MOUSE_MOVED:
		    case MouseEvent::ME_MOUSE_DRAGGED:
		    case MouseEvent::ME_MOUSE_DRAGMOVED:
			    processMouseMotionEvent(static_cast<MouseEvent*>(e));
			    break;
		    case KeyEvent::KE_KEY_PRESSED:
		    case KeyEvent::KE_KEY_RELEASED:
		    case KeyEvent::KE_KEY_CLICKED:
			    processKeyEvent(static_cast<KeyEvent*>(e));
			    break;
		    }
        }
	}

//-----------------------------------------------------------------------------
	void EventProcessor::addCursorMoveListener(MouseMotionListener* c)
	{
		mInputDevice->addCursorMoveListener(c);
	}
//-----------------------------------------------------------------------------
	void EventProcessor::removeCursorMoveListener(MouseMotionListener* c)
	{
		mInputDevice->removeCursorMoveListener(c);
	}

//-----------------------------------------------------------------------------
	Real EventProcessor::getLeft() const
	{
		return 0;
	}

//-----------------------------------------------------------------------------
	Real EventProcessor::getTop() const
	{
		return 0;
	}
	
//-----------------------------------------------------------------------------
	PositionTarget* EventProcessor::getPositionTargetParent() const 
	{
		return NULL;
	}
//-----------------------------------------------------------------------------

}

