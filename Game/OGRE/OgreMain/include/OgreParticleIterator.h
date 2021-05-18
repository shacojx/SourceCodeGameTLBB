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
#ifndef __ParticleIterator_H__
#define __ParticleIterator_H__

#include "OgrePrerequisites.h"

namespace Ogre {


    /** Convenience class to make it easy to step through all particles in a ParticleSystem.
    */
    class _OgreExport ParticleIterator
    {
        friend class ParticleSystem;
    protected:
        std::list<Particle*>::iterator mPos;
        std::list<Particle*>::iterator mStart;
        std::list<Particle*>::iterator mEnd;

        /// Protected constructor, only available from ParticleSystem::getIterator
        ParticleIterator(std::list<Particle*>::iterator start, std::list<Particle*>::iterator end);

    public:
        // Returns true when at the end of the particle list
        bool end(void);

        /** Returns a pointer to the next particle, and moves the iterator on by 1 element. */
        Particle* getNext(void);
    };
}


#endif

