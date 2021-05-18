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
OgreCursor.h  -  
	The Cursor is an non-visual object that contains a the XYZ values that 
	are modified by a InputReader.
	An InputReader creates and contains a Cursor object that it uses when it 
	is set to buffered input (using the EventQueue).

	To get a graphical representation of the cursor, a CursorGuiElement is
	used, which is a MouseMotionListener to the Cursor.

-------------------
begin                : Nov 19 2002
copyright            : (C) 2002 by Kenny Sabir
email                : kenny@sparksuit.com
***************************************************************************/

#ifndef __Cursor_H__
#define __Cursor_H__

#include "OgrePrerequisites.h"
#include "OgreMouseMotionTarget.h"
#include "OgreMouseTarget.h"

namespace Ogre {
	/** The Cursor is an non-visual object that contains a the XYZ values that 
		are modified by a InputReader.
	@remarks
		An InputReader creates and contains a Cursor object that it uses when it 
		is set to buffered input (using the EventQueue).

		To get a graphical representation of the cursor, a CursorGuiElement is
		used, which is a MouseMotionListener to the Cursor.
	*/
	class _OgreExport Cursor : public MouseMotionTarget, public MouseTarget
    {
    protected:
		/** Cursor position */
        Real mMouseX, mMouseY, mMouseZ;

        /** relative cursor position */
        Real mRelX, mRelY, mRelZ;

		/** Cursor limits 0-1 */
		Real mXLowLimit, mXHighLimit, mYLowLimit, mYHighLimit, mZLowLimit, mZHighLimit;

		/** Scale the cursor movements. Initially set at 1 (no scaling).
			The scaling effects all axis, XYZ */
		Real mScale;

		/** inline function to clip a value to its low and high limits */
		inline Real limit(Real val, Real low, Real high)
		{	return (val < low) ? low: ((val > high) ? high : val); }
	public :
		Cursor();
		virtual ~Cursor();

		/** add relative amount to X */
		void addToX(Real val);

		/** add relative amount to Y */
		void addToY(Real val);

		/** add relative amount to Z */
		void addToZ(Real val);

		/** process the mouse events that are for any listeners to the cursor */
		void processEvent(InputEvent* e);


		/** get the current X position of the cursor 0 left, 1 right */
		Real getX() const;

		/** get the current Y position of the cursor 0 top, 1 bottom */
		Real getY() const;

		/** get the current Z position of the cursor 0 none, 1 full */
		Real getZ() const;

        /** get relative X cursor movement */
        Real getRelX() const {return mRelX;}

        /** get relative Y cursor movement */
        Real getRelY() const {return mRelY;}

        /** get relative Z cursor movement */
        Real getRelZ() const {return mRelZ;}



	// PositionTarget methods
        /** Gets the left of this element in relation to the screen (where 0 = far left, 1.0 = far right)  */
        Real getLeft(void) const ;

        /** Gets the top of this element in relation to the screen (where 0 = top, 1.0 = bottom)  */
        Real getTop(void) const ;

		/** The parent of the cursor is NULL as it's position is absolute in the window */
		PositionTarget* getPositionTargetParent() const;

		inline virtual bool isKeyEnabled() const
		{ return false; }

		/** Gets the current cursor movement scaling factor. */
		Real getScale(void) const { return mScale; }
		/** Sets the current cursor movement scaling factor. */
		void setScale(Real scale) { mScale = scale; }
		

    };

}


#endif  // __Cursor_H__
