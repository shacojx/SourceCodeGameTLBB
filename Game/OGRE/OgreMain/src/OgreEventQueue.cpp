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
// Ogre includes
#include "OgreEventQueue.h"
#include "OgrePlatformManager.h"
#include "OgreInput.h"
#include "OgreInputEvent.h"

namespace Ogre {    
    EventQueue::EventQueue()
    {
		mActivateEventQueue = false;
        


    }

    //-----------------------------------------------------------------------
    EventQueue::~EventQueue()
    {
		while (mEventQueue.size() > 0)
		{
			InputEvent* temp = mEventQueue.front();
			mEventQueue.pop();
			delete temp;
		}


    }

    //-----------------------------------------------------------------------
	void EventQueue::push(InputEvent* e)
	{
		if (mActivateEventQueue)
		{
			mEventQueue.push(e);
		}
	}

    //-----------------------------------------------------------------------
	InputEvent* EventQueue::pop()
	{
		InputEvent* temp = NULL;
		if (mActivateEventQueue && mEventQueue.size() > 0)
		{
			temp = mEventQueue.front();
			mEventQueue.pop();
		}
		return temp;
	}

    //-----------------------------------------------------------------------
	void EventQueue::activateEventQueue(bool set)
	{
		mActivateEventQueue = set;
	}

    //-----------------------------------------------------------------------
	size_t EventQueue::getSize() const
	{
		return mEventQueue.size();
	}

}
