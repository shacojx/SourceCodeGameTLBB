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
OgreKeyTarget.h  -  
	Handles the adding and removing of KeyListeners.

	Components that process the Key Event should subclass this class 
	and call processKeyEvent when that event is identified.

-------------------
begin                : Nov 19 2002
copyright            : (C) 2002 by Kenny Sabir
email                : kenny@sparksuit.com
***************************************************************************/

#ifndef __KeyTarget_H__
#define __KeyTarget_H__

#include "OgrePrerequisites.h"
#include "OgreEventTarget.h"

namespace Ogre {

    class _OgreExport KeyTarget : public EventTarget
    {
    private:
        std::set<KeyListener*> mRemovedListeners;

    protected:
        std::set<KeyListener*> mKeyListeners;

    public:
		KeyTarget() { }
        virtual ~KeyTarget() { }

	    void processKeyEvent(KeyEvent* e) ;
	    void addKeyListener(KeyListener* l) ;
	    void removeKeyListener(KeyListener* l) ;
    };



}


#endif //__KeyTarget_H__

