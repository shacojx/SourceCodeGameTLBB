/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/
#include "OgreStableHeaders.h"

#include "OgreCursor.h"	
#include "OgreEventListeners.h"


namespace Ogre {

    //-----------------------------------------------------------------------

	Cursor::Cursor() :
		MouseMotionTarget(),
		MouseTarget()
	{
		mXLowLimit = 0;
		mXHighLimit = 1;
		mYLowLimit = 0;
		mYHighLimit = 1;
		mZLowLimit = 0;
		mZHighLimit = 1;

		mScale = 1;
		mMouseX = 0.5;
		mMouseY = 0.5;
		mMouseZ = 0.5;

        mRelX = 0.0;
        mRelY = 0.0;
        mRelZ = 0.0;
	}

    //-----------------------------------------------------------------------

	Cursor::~Cursor()
	{
	}

    //-----------------------------------------------------------------------
	void Cursor::addToX(Real val)
	{
		Real valReal = val * mScale;
        mRelX = valReal;
		mMouseX = limit(mMouseX + valReal,mXLowLimit, mXHighLimit);
	}

    //-----------------------------------------------------------------------
	void Cursor::addToY(Real val)
	{
		Real valReal = val * mScale;
        mRelY = valReal;
		mMouseY = limit(mMouseY + valReal,mYLowLimit, mYHighLimit);
	}

    //-----------------------------------------------------------------------
	void Cursor::addToZ(Real val)
	{
		Real valReal = val * mScale;
        mRelZ = valReal;
		mMouseZ = limit(mMouseZ + valReal,mZLowLimit, mZHighLimit);
	}

    //-----------------------------------------------------------------------
	void Cursor::processEvent(InputEvent* e)
	{
		switch(e->getID()) 
		{
		case MouseEvent::ME_MOUSE_PRESSED:
		case MouseEvent::ME_MOUSE_RELEASED:
		case MouseEvent::ME_MOUSE_CLICKED:
		case MouseEvent::ME_MOUSE_ENTERED:
		case MouseEvent::ME_MOUSE_EXITED:
			processMouseEvent(static_cast<MouseEvent*>(e));
			break;
		case MouseEvent::ME_MOUSE_MOVED:
		case MouseEvent::ME_MOUSE_DRAGGED:
			processMouseMotionEvent(static_cast<MouseEvent*>(e));
			break;
		}

        mRelX = 0.0;
        mRelY = 0.0;
        mRelZ = 0.0;
	}

    //-----------------------------------------------------------------------
	Real Cursor::getX() const
	{
		return mMouseX;
	}

    //-----------------------------------------------------------------------
	Real Cursor::getY() const
	{
		return mMouseY;
	}

    //-----------------------------------------------------------------------
	Real Cursor::getZ() const
	{
		return mMouseZ;
	}

    //-----------------------------------------------------------------------
    Real Cursor::getLeft(void) const 
	{
		return getX();
	}

    //-----------------------------------------------------------------------
    Real Cursor::getTop(void) const 
	{
		return getY();

	}

    //-----------------------------------------------------------------------
	PositionTarget* Cursor::getPositionTargetParent() const
	{
		return NULL;
	}

}

