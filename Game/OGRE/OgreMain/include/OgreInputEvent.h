/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General License for more details.

You should have received a copy of the GNU Lesser General License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/

/***************************************************************************
OgreInputEvent.h  -  
	 * The root event class for all GuiElement-level input events.
	 *
	 * Input events are delivered to listeners before they are
	 * processed normally by the source where they originated.
	 * This allows listeners and GuiElement subclasses to "consume"
	 * the event so that the source will not process them in their
	 * default manner.  For example, consuming mousePressed events
	 * on a Button GuiElement will prevent the Button from being
	 * activated.
-------------------
begin                : Nov 19 2002
copyright            : (C) 2002 by Kenny Sabir
email                : kenny@sparksuit.com
***************************************************************************/

#ifndef __InputEvent_H__
#define __InputEvent_H__

#include "OgrePrerequisites.h"

namespace Ogre {

    /** The root event class for all GuiElement-level input events.
	 @remarks
	  Input events are delivered to listeners before they are
	  processed normally by the source where they originated.
	  This allows listeners and GuiElement subclasses to "consume"
	  the event so that the source will not process them in their
	  default manner.  For example, consuming mousePressed events
	  on a Button GuiElement will prevent the Button from being
	  activated.
	*/
	class _OgreExport InputEvent
    {
    protected:

		/**
		 * Not implemented yet
		 */
		Real mWhen;
		/**
		 * The state of the modifier keys at the time the input
		 * event was fired.
		 */
		int mModifiers;

		/**
		 * The target to process the event. This is ususally found by the dispatcher
		 */

		EventTarget* mSource;

		/**
		 * The ID of the event
		 */
		int mId;

		/**
		 * whether the event has been consumed
		 */
		bool mConsumed;

	public:
	
		
		enum		// using the enum hack cause VC6 doesn't support static const in classes
		{
			/**
			 * This flag indicates that the Shift key was down when the event
			 * occurred.
			 */
			SHIFT_MASK		= 1 << 0,
			
			/**
			 * This flag indicates that the Control key was down when the event
			 * occurred.
			 */
			CTRL_MASK		= 1 << 1,
			
			/**
			 * This flag indicates that the Meta key was down when the event
			 * occurred. For mouse events, this flag indicates that the right
			 * button was pressed or released.
			 */

			 META_MASK		= 1 << 2,
			/**
			 * This flag indicates that the Alt key was down when
			 * the event occurred. For mouse events, this flag indicates that the
			 * middle mouse button was pressed or released.
			 */
			ALT_MASK			= 1 << 3,
			BUTTON0_MASK		= 1 << 4,
			BUTTON1_MASK		= 1 << 5,
			BUTTON2_MASK		= 1 << 6,
			BUTTON3_MASK		= 1 << 7,
			BUTTON_ANY_MASK		= 0xF << 4
		};



		/**
		 * Constructs an InputEvent object with the specified source GuiElement,
		 * modifiers, and type.
		 * @param source the object where the event originated
		 * @id the event type
		 * @when the time the event occurred
		 * @modifiers the modifier keys down while event occurred
		 */
		InputEvent(EventTarget* source, int id, Real when, int modifiers);

		/**
		 * Consumes this event so that it will not be processed
		 * in the default manner by the source which originated it.
		 */
		void consume();

		/**
		 * Returns the modifiers flag for this event.
		 */
		int getModifiers() const;
		
		/**
		 * Returns the timestamp of when this event occurred. Not implemented yet
		 */
		Real getWhen() const;

		/**
		 * Returns whether or not the Alt modifier is down on this event.
		 */
		bool isAltDown() const;

		/**
		 * Returns whether or not this event has been consumed.
		 * @see #consume
		 */
		bool isConsumed() const;

		/**
		 * Returns whether or not the Control modifier is down on this event.
		 */
		bool isControlDown() const;

		/**
		 * Returns whether or not the Meta modifier is down on this event.
		 */
		bool isMetaDown() const;

		/**
		 * Returns whether or not the Shift modifier is down on this event.
		 */
		bool isShiftDown() const;

		bool isEventBetween(int start, int end) const;
		int getID() const;

		EventTarget* getSource() const;
    };
}


#endif  // __InputEvent_H__

