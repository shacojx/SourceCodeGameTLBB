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

#include "OgreInputEvent.h"

namespace Ogre {


    //-----------------------------------------------------------------------
	InputEvent::InputEvent(EventTarget* source, int id, Real when, int modifiers) :
		mWhen(when),
		mModifiers(modifiers),
		mSource(source),
		mId(id)
	{
			mConsumed = false;
	}

    //-----------------------------------------------------------------------
	void InputEvent::consume() {
		mConsumed = true;
	}

    //-----------------------------------------------------------------------
	int InputEvent::getModifiers() const {
		return mModifiers;
	}

    //-----------------------------------------------------------------------
	Real InputEvent::getWhen() const {
		return mWhen;
	}

    //-----------------------------------------------------------------------
	bool InputEvent::isAltDown() const {
		return (mModifiers & InputEvent::ALT_MASK) != 0;
	}

    //-----------------------------------------------------------------------
	bool InputEvent::isConsumed() const {
		return mConsumed;
	}

    //-----------------------------------------------------------------------
	bool InputEvent::isControlDown() const {
		return (mModifiers & InputEvent::CTRL_MASK) != 0;
	}

    //-----------------------------------------------------------------------
	bool InputEvent::isMetaDown() const {
		return (mModifiers & InputEvent::META_MASK) != 0;
	}

    //-----------------------------------------------------------------------
	bool InputEvent::isShiftDown() const {
		return (mModifiers & InputEvent::SHIFT_MASK) != 0;
	}

    //-----------------------------------------------------------------------
	bool InputEvent::isEventBetween(int start, int end) const
	{
		return (mId >= start) && (mId <= end);
	}

    //-----------------------------------------------------------------------
	int InputEvent::getID() const
	{
		return mId;

	}

	EventTarget* InputEvent::getSource() const
	{
		return mSource;

	}

}



