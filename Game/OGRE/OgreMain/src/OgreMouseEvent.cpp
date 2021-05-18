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
#include "OgreMouseEvent.h"
#include "OgreStringConverter.h"
#include "OgrePositionTarget.h"

namespace Ogre {

	MouseEvent::MouseEvent(PositionTarget* source, int id, int whichButton, Real when, int modifiers,
		Real x, Real y, Real z, int clickCount) :
		InputEvent(source, id, when, modifiers),
		mX(x),mY(y),mZ(z),
        mRelX(0), mRelY(0), mRelZ(0),
		mButtonID(whichButton),
		mClickCount(clickCount)
	{
	} 

    MouseEvent::MouseEvent(PositionTarget* source, int id, int whichButton, Real when, int modifiers,
		Real x, Real y, Real z,
        Real relx, Real rely, Real relz,
        int clickCount) :
		InputEvent(source, id, when, modifiers),
		mX(x), mY(y), mZ(z),
        mRelX(relx), mRelY(rely), mRelZ(relz),
		mButtonID(whichButton),
		mClickCount(clickCount)
	{
	} 


	/**
	 * Return the number of mouse clicks associated with this event.
	 *
	 * @return integer value for the number of clicks
	 */
	 int MouseEvent::getClickCount() {
		return mClickCount;
	}

	/**
	 * Returns the horizontal x position of the event relative to the 
	 * source GuiElement.
	 *
	 * @return x  an integer indicating horizontal position relative to
	 *            the GuiElement
	 */
	 Real MouseEvent::getX() const {
		return mX;
	}
	/**
	 * Returns the vertical y position of the event relative to the
	 * source GuiElement.
	 *
	 * @return y  an integer indicating vertical position relative to
	 *            the GuiElement
	 */
	 Real MouseEvent::getY() const {
		return mY;
	}

	/**
	 * Returns the vertical y position of the event relative to the
	 * source GuiElement.
	 *
	 * @return y  an integer indicating vertical position relative to
	 *            the GuiElement
	 */
	 Real MouseEvent::getZ() const {
		return mZ;
	}
	 int MouseEvent::getButtonID() const
	 {
		 return mButtonID;

	 }

	/**
	 * Returns a parameter string identifying this event.
	 * This method is useful for event-logging and for debugging.
	 *
	 * @return a string identifying the event and its attributes
	 */
	 String MouseEvent::paramString() const {
		String typeStr;
		switch(mId) {
		  case ME_MOUSE_PRESSED:
			  typeStr = "MOUSE_PRESSED";
			  break;
		  case ME_MOUSE_RELEASED:
			  typeStr = "MOUSE_RELEASED";
			  break;
		  case ME_MOUSE_CLICKED:
			  typeStr = "MOUSE_CLICKED";
			  break;
		  case ME_MOUSE_ENTERED:
			  typeStr = "MOUSE_ENTERED";
			  break;
		  case ME_MOUSE_EXITED:
			  typeStr = "MOUSE_EXITED";
			  break;
		  case ME_MOUSE_MOVED:
			  typeStr = "MOUSE_MOVED";
			  break;
		  case ME_MOUSE_DRAGGED:
			  typeStr = "MOUSE_DRAGGED";
			  break;
		  case ME_MOUSE_DRAGENTERED:
			  typeStr = "MOUSE_DRAGENTERED";
			  break;
		  case ME_MOUSE_DRAGEXITED:
			  typeStr = "MOUSE_DRAGEXITED";
			  break;
		  case ME_MOUSE_DRAGDROPPED:
			  typeStr = "MOUSE_DRAGDROPPED";
			  break;
		  case ME_MOUSE_DRAGMOVED:
			  typeStr = "MOUSE_DRAGMOVED";
			  break;
		  default:
			  typeStr = "unknown type";
		}
		return typeStr + ",("+StringConverter::toString(mX)+","+StringConverter::toString(mY)+")"+ ",mods="+StringConverter::toString(getModifiers())+ 
			   ",clickCount="+StringConverter::toString(mClickCount);
	}

	/**
	 * Translates the event's coordinates to a new position
	 * by adding specified x (horizontal) and y (veritcal) offsets.
	 *
	 * @param x the horizontal x value to add to the current x coordinate position
	 * @param y the vertical y value to add to the current y coordinate position
	 */
	 void MouseEvent::translatePoint(Real x, Real y) {
		mX += x;
		mY += y;
	}

}

