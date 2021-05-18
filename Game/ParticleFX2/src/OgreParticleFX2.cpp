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

#include "OgreRoot.h"
#include "OgreParticleSystemManager.h"
#include "OgreParticleAffectorFactory.h"
#include "OgreParticleEmitterFactory.h"

#include "OgreScaleInterpolatorAffectorFactory.h"
#include "OgreRevolutionAffectorFactory.h"
#include "OgreColourFadingAffectorFactory.h"
#include "OgreMovementAffectorFactory.h"

#include "OgrePolarEmitterFactory.h"

namespace Ogre {

    std::vector<ParticleEmitterFactory*> emitterFactories2;
    std::vector<ParticleAffectorFactory*> affectorFactories2;

    //-----------------------------------------------------------------------
    void registerParticleFactories2(void)
    {
        // -- Create all new particle emitter factories --
        ParticleEmitterFactory* pEmitFact;

        // PointEmitter
        pEmitFact = new PolarEmitterFactory();
        ParticleSystemManager::getSingleton().addEmitterFactory(pEmitFact);
        emitterFactories2.push_back(pEmitFact);


        // -- Create all new particle emitter factories --
        ParticleAffectorFactory* pAffFact;

        // ScaleInterpolatorAffector
        pAffFact = new ScaleInterpolatorAffectorFactory();
        ParticleSystemManager::getSingleton().addAffectorFactory(pAffFact);
        affectorFactories2.push_back(pAffFact);

        // RevolutionAffectorFactory
        pAffFact = new RevolutionAffectorFactory();
        ParticleSystemManager::getSingleton().addAffectorFactory(pAffFact);
        affectorFactories2.push_back(pAffFact);

		// ColourFadingAffectorFactory
		pAffFact = new ColourFadingAffectorFactory();
		ParticleSystemManager::getSingleton().addAffectorFactory(pAffFact);
		affectorFactories2.push_back(pAffFact);

		// MovementAffectorFactory
		pAffFact = new MovementAffectorFactory();
		ParticleSystemManager::getSingleton().addAffectorFactory(pAffFact);
		affectorFactories2.push_back(pAffFact);
    }
    //-----------------------------------------------------------------------
    void destroyParticleFactories2(void)
    {
        std::vector<ParticleEmitterFactory*>::iterator ei;
        std::vector<ParticleAffectorFactory*>::iterator ai;

        for (ei = emitterFactories2.begin(); ei != emitterFactories2.end(); ++ei)
        {
            delete (*ei);
        }

        for (ai = affectorFactories2.begin(); ai != affectorFactories2.end(); ++ai)
        {
            delete (*ai);
        }


    }
    //-----------------------------------------------------------------------
    //extern "C" void _OgreParticleFX2Export dllStartPlugin(void) throw()
    //{
    //    // Particle SFX
    //    registerParticleFactories2();
    //}

    //-----------------------------------------------------------------------
    //extern "C" void _OgreParticleFX2Export dllStopPlugin(void)
    //{
    //    // Particle SFX
    //    destroyParticleFactories2();

    //}


}

