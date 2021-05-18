/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General  License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General  License for more details.

You should have received a copy of the GNU Lesser General  License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/
#include "OgreStableHeaders.h"

#include "OgreString.h"
#include "OgreKeyEvent.h"
#include "OgreStringConverter.h"
#include "OgrePositionTarget.h"
#include "OgreInput.h"

namespace Ogre {

	 KeyEvent::KeyEvent(PositionTarget* source, int id, int key, Real when, int modifiers) :
		InputEvent(source, id, when, modifiers),
		mKey(key)
	{
	} 

	 int KeyEvent::getKey()
	 {
		 return mKey;

	 }

	 
	 char KeyEvent::getKeyChar()
	 {
		 return InputReader::getKeyChar(mKey, mModifiers);
	 }

	/**
	 * Returns a parameter string identifying this event.
	 * This method is useful for event-logging and for debugging.
	 *
	 * @return a string identifying the event and its attributes
	 */
	 String KeyEvent::paramString() const {
		String typeStr;
		switch(mId) {
		  case KE_KEY_PRESSED:
			  typeStr = "KEY_PRESSED";
			  break;
		  case KE_KEY_RELEASED:
			  typeStr = "KEY_RELEASED";
			  break;
		  case KE_KEY_CLICKED:
			  typeStr = "KEY_CLICKED";
			  break;
		  case KE_KEY_FOCUSIN:
			  typeStr = "KEY_FOCUSIN";
			  break;
		  case KE_KEY_FOCUSOUT:
			  typeStr = "KEY_FOCUSOUT";
			  break;
		  default:
			  typeStr = "unknown type";
		}
		return typeStr + ", key="+StringConverter::toString(mKey);
	}
}

