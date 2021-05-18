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

#include "OgrePointEmitterFactory.h"
#include "OgreBoxEmitterFactory.h"
#include "OgreEllipsoidEmitterFactory.h"
#include "OgreHollowEllipsoidEmitterFactory.h"
#include "OgreRingEmitterFactory.h"
#include "OgreCylinderEmitterFactory.h"
#include "OgreLinearForceAffectorFactory.h"
#include "OgreColourFaderAffectorFactory.h"
#include "OgreColourFaderAffectorFactory2.h"
#include "OgreColourImageAffectorFactory.h"
#include "OgreColourInterpolatorAffectorFactory.h"
#include "OgreScaleAffectorFactory.h"
#include "OgreRotationAffectorFactory.h"
#include "OgreDirectionRandomiserAffectorFactory.h"
#include "OgreDeflectorPlaneAffectorFactory.h"

namespace Ogre {

    std::vector<ParticleEmitterFactory*> emitterFactories;
    std::vector<ParticleAffectorFactory*> affectorFactories;

    //-----------------------------------------------------------------------
    void registerParticleFactories(void)
    {
        // -- Create all new particle emitter factories --
        ParticleEmitterFactory* pEmitFact;

        // PointEmitter
        pEmitFact = new PointEmitterFactory();
        ParticleSystemManager::getSingleton().addEmitterFactory(pEmitFact);
        emitterFactories.push_back(pEmitFact);

        // BoxEmitter
        pEmitFact = new BoxEmitterFactory();
        ParticleSystemManager::getSingleton().addEmitterFactory(pEmitFact);
        emitterFactories.push_back(pEmitFact);

        // EllipsoidEmitter
        pEmitFact = new EllipsoidEmitterFactory();
        ParticleSystemManager::getSingleton().addEmitterFactory(pEmitFact);
        emitterFactories.push_back(pEmitFact);
        
	    // CylinderEmitter
        pEmitFact = new CylinderEmitterFactory();
        ParticleSystemManager::getSingleton().addEmitterFactory(pEmitFact);
        emitterFactories.push_back(pEmitFact);
	
        // RingEmitter
        pEmitFact = new RingEmitterFactory();
        ParticleSystemManager::getSingleton().addEmitterFactory(pEmitFact);
        emitterFactories.push_back(pEmitFact);

        // HollowEllipsoidEmitter
        pEmitFact = new HollowEllipsoidEmitterFactory();
        ParticleSystemManager::getSingleton().addEmitterFactory(pEmitFact);
        emitterFactories.push_back(pEmitFact);

        // -- Create all new particle affector factories --
        ParticleAffectorFactory* pAffFact;

        // LinearForceAffector
        pAffFact = new LinearForceAffectorFactory();
        ParticleSystemManager::getSingleton().addAffectorFactory(pAffFact);
        affectorFactories.push_back(pAffFact);

        // ColourFaderAffector
        pAffFact = new ColourFaderAffectorFactory();
        ParticleSystemManager::getSingleton().addAffectorFactory(pAffFact);
        affectorFactories.push_back(pAffFact);

        // ColourFaderAffector2
        pAffFact = new ColourFaderAffectorFactory2();
        ParticleSystemManager::getSingleton().addAffectorFactory(pAffFact);
        affectorFactories.push_back(pAffFact);

        // ColourImageAffector
        pAffFact = new ColourImageAffectorFactory();
        ParticleSystemManager::getSingleton().addAffectorFactory(pAffFact);
        affectorFactories.push_back(pAffFact);

        // ColourInterpolatorAffector
        pAffFact = new ColourInterpolatorAffectorFactory();
        ParticleSystemManager::getSingleton().addAffectorFactory(pAffFact);
        affectorFactories.push_back(pAffFact);

        // ScaleAffector
        pAffFact = new ScaleAffectorFactory();
        ParticleSystemManager::getSingleton().addAffectorFactory(pAffFact);
        affectorFactories.push_back(pAffFact);

        // RotationAffector
        pAffFact = new RotationAffectorFactory();
        ParticleSystemManager::getSingleton().addAffectorFactory(pAffFact);
        affectorFactories.push_back(pAffFact);


		// DirectionRandomiserAffector
		pAffFact = new DirectionRandomiserAffectorFactory();
		ParticleSystemManager::getSingleton().addAffectorFactory(pAffFact);
		affectorFactories.push_back(pAffFact);

		// DeflectorPlaneAffector
		pAffFact = new DeflectorPlaneAffectorFactory();
		ParticleSystemManager::getSingleton().addAffectorFactory(pAffFact);
		affectorFactories.push_back(pAffFact);

	}
    //-----------------------------------------------------------------------
    void destroyParticleFactories(void)
    {
        std::vector<ParticleEmitterFactory*>::iterator ei;
        std::vector<ParticleAffectorFactory*>::iterator ai;

        for (ei = emitterFactories.begin(); ei != emitterFactories.end(); ++ei)
        {
            delete (*ei);
        }

        for (ai = affectorFactories.begin(); ai != affectorFactories.end(); ++ai)
        {
            delete (*ai);
        }


    }
    //-----------------------------------------------------------------------
    //extern "C" void _OgreParticleFXExport dllStartPlugin(void) throw()
    //{
    //    // Particle SFX
    //    registerParticleFactories();
    //}

    //-----------------------------------------------------------------------
    //extern "C" void _OgreParticleFXExport dllStopPlugin(void)
    //{
    //    // Particle SFX
    //    destroyParticleFactories();

    //}


}

