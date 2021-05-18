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
#include "OgreMeshAnimationAffector.h"
#include "OgreParticleSystem.h"
#include "OgreParticle.h"
#include "OgreStringConverter.h"
#include "OgreExt/OgreMeshParticleVisualData.h"

namespace Ogre {

    // Instantiate statics
    MeshAnimationAffector::CmdAnimationName	MeshAnimationAffector::msAnimationNameCmd;
    MeshAnimationAffector::CmdAnimationLoop	MeshAnimationAffector::msAnimationLoopCmd;
    MeshAnimationAffector::CmdAnimationSpeedFactor	MeshAnimationAffector::msAnimationSpeedFactorCmd;

    //-----------------------------------------------------------------------
    MeshAnimationAffector::MeshAnimationAffector(ParticleSystem* psys)
		:ParticleAffector(psys),
		mAnimationName(""),
        mAnimationLoop(false),
        mAnimationSpeedFactor(1.0f)
    {
        mType = "MeshAnimationAffector";
		
		// Init parameters
		if (createParamDictionary("MeshAnimationAffector"))
		{
			ParamDictionary* dict = getParamDictionary();

			dict->addParameter(ParameterDef("animation_name", 
				"the name of the animation.", PT_STRING),
				&msAnimationNameCmd);

			dict->addParameter(ParameterDef("animation_loop", 
				"set the looping of the animation.", PT_BOOL),
				&msAnimationLoopCmd);	

            dict->addParameter(ParameterDef("animation_speed", 
                "set the speed factor of the animation.", PT_REAL),
                &msAnimationSpeedFactorCmd);			
		}
    }
	//-----------------------------------------------------------------------
	void MeshAnimationAffector::_affectParticles(ParticleSystem* pSystem, Real timeElapsed)
	{
		ParticleIterator pi = pSystem->_getIterator();
		Particle *p;

		while (!pi.end())
		{
			p = pi.getNext();

			MeshParticleVisualData *data = static_cast<MeshParticleVisualData *>(p->getVisualData());

            data->mAnimationName = mAnimationName;

            data->mDeltaTime = timeElapsed;

            data->mAnimationLoop = mAnimationLoop;

            data->mAnimationSpeedFactor = mAnimationSpeedFactor;

            data->mAnimationUpdated = true;
		}
	}	
	//-----------------------------------------------------------------------
	String MeshAnimationAffector::CmdAnimationName::doGet(const void* target) const
	{
		return static_cast<const MeshAnimationAffector*>(target)->getAnimationName();
	}
	void MeshAnimationAffector::CmdAnimationName::doSet(void* target, const String& val)
	{
		static_cast<MeshAnimationAffector*>(target)->setAnimationName(val);
	}
	//-----------------------------------------------------------------------
	String MeshAnimationAffector::CmdAnimationLoop::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const MeshAnimationAffector*>(target)->getAnimationLoop() );
	}
	void MeshAnimationAffector::CmdAnimationLoop::doSet(void* target, const String& val)
	{
		static_cast<MeshAnimationAffector*>(target)->setAnimationLoop(StringConverter::parseBool(val));
	}
    //-----------------------------------------------------------------------
    String MeshAnimationAffector::CmdAnimationSpeedFactor::doGet(const void* target) const
    {
        return StringConverter::toString(
            static_cast<const MeshAnimationAffector*>(target)->getAnimationSpeedFactor() );
    }
    void MeshAnimationAffector::CmdAnimationSpeedFactor::doSet(void* target, const String& val)
    {
        static_cast<MeshAnimationAffector*>(target)->setAnimationSpeedFactor(StringConverter::parseReal(val));
    }
}

