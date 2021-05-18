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
#ifndef __MeshAnimationAffector_H__
#define __MeshAnimationAffector_H__

#include "OgreParticleAffector.h"
#include "OgreVector3.h"


namespace Ogre {

    /** This class defines a ParticleAffector which applies a animation update
        to the mesh particle   
    */
    class MeshAnimationAffector : public ParticleAffector
    {
    public:
       
        class CmdAnimationName : public ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
        };

        class CmdAnimationLoop : public ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
        };

        class CmdAnimationSpeedFactor : public ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
        };
		
	    /// Default constructor
        MeshAnimationAffector(ParticleSystem* psys);

        /** See ParticleAffector. */
        void _affectParticles(ParticleSystem* pSystem, Real timeElapsed);
 
		void setAnimationName(const String& animName)
        {
            mAnimationName = animName;
        }
        const String& getAnimationName(void) const
        {
            return mAnimationName;
        }

        void setAnimationLoop(bool loop)
        {
            mAnimationLoop = loop;
        }
        bool getAnimationLoop(void) const
        {
            return mAnimationLoop;
        }

        void setAnimationSpeedFactor(Real factor)
        {
            mAnimationSpeedFactor = factor;
        }
        Real getAnimationSpeedFactor(void) const
        {
            return mAnimationSpeedFactor;
        }
	
		//////////////////////////////////////////////////////////////////////////		
		static CmdAnimationName	msAnimationNameCmd;
		static CmdAnimationLoop	msAnimationLoopCmd;
		static CmdAnimationSpeedFactor msAnimationSpeedFactorCmd;

    protected:

		String mAnimationName;
        bool mAnimationLoop;
        Real mAnimationSpeedFactor;
    };
}

#endif

