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
#include "OgreDirectionRandomiserAffector.h"
#include "OgreParticleSystem.h"
#include "OgreParticle.h"
#include "OgreStringConverter.h"


namespace Ogre {

    // Instantiate statics
    DirectionRandomiserAffector::CmdRandomness DirectionRandomiserAffector::msRandomnessCmd;
    DirectionRandomiserAffector::CmdScope DirectionRandomiserAffector::msScopeCmd;
    DirectionRandomiserAffector::CmdKeepVelocity DirectionRandomiserAffector::msKeepVelocityCmd;

    //-----------------------------------------------------------------------
    DirectionRandomiserAffector::DirectionRandomiserAffector(ParticleSystem* psys)
       : ParticleAffector(psys)
    {
        mType = "DirectionRandomiser";

        // defaults
        mRandomness = 1.0;
        mScope = 1.0;
        mKeepVelocity = false;

        // Set up parameters
        if (createParamDictionary("DirectionRandomiserAffector"))
        {
            addBaseParameters();
            // Add extra paramaters
            ParamDictionary* dict = getParamDictionary();
            dict->addParameter(ParameterDef("randomness",
                "The amount of randomness (chaos) to apply to the particle movement.",
                PT_REAL), &msRandomnessCmd);
            dict->addParameter(ParameterDef("scope",
                "The percentage of particles which is affected.",
                PT_REAL), &msScopeCmd);
            dict->addParameter(ParameterDef("keep_velocity",
                "Detemines whether the velocity of the particles is changed.",
                PT_BOOL), &msKeepVelocityCmd);
        }
    }
    //-----------------------------------------------------------------------
    void DirectionRandomiserAffector::_affectParticles(ParticleSystem* pSystem, Real timeElapsed)
    {
        ParticleIterator pi = pSystem->_getIterator();
        Particle *p;
        Real length;

        while (!pi.end())
        {
            p = pi.getNext();
            if (mScope > Math::UnitRandom())
            {
                if (!p->direction.isZeroLength())
                {
                    if (mKeepVelocity)
                    {
                        length = p->direction.length();
                    }

                    p->direction += Vector3(Math::RangeRandom(-mRandomness, mRandomness) * timeElapsed,
                        Math::RangeRandom(-mRandomness, mRandomness) * timeElapsed,
                        Math::RangeRandom(-mRandomness, mRandomness) * timeElapsed);

                    if (mKeepVelocity)
                    {
                        p->direction *= length / p->direction.length();
                    }
                }
            }
        }
    }
    //-----------------------------------------------------------------------
    void DirectionRandomiserAffector::setRandomness(Real force)
    {
        mRandomness = force;
    }
    //-----------------------------------------------------------------------
    void DirectionRandomiserAffector::setScope(Real scope)
    {
        mScope = scope;
    }
    //-----------------------------------------------------------------------
    void DirectionRandomiserAffector::setKeepVelocity(bool keepVelocity)
    {
        mKeepVelocity = keepVelocity;
    }
    //-----------------------------------------------------------------------
    Real DirectionRandomiserAffector::getRandomness(void) const
    {
        return mRandomness;
    }
    //-----------------------------------------------------------------------
    Real DirectionRandomiserAffector::getScope(void) const
    {
        return mScope;
    }
    //-----------------------------------------------------------------------
    bool DirectionRandomiserAffector::getKeepVelocity(void) const
    {
        return mKeepVelocity;
    }

    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    // Command objects
    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    String DirectionRandomiserAffector::CmdRandomness::doGet(const void* target) const
    {
        return StringConverter::toString(
            static_cast<const DirectionRandomiserAffector*>(target)->getRandomness() );
    }
    void DirectionRandomiserAffector::CmdRandomness::doSet(void* target, const String& val)
    {
        static_cast<DirectionRandomiserAffector*>(target)->setRandomness(StringConverter::parseReal(val));
    }
#if OGRE_STRING_INTERFACE_COPY
    void DirectionRandomiserAffector::CmdRandomness::doCopy(void* target, const void* source)
    {
        static_cast<DirectionRandomiserAffector*>(target)->setRandomness(
            static_cast<const DirectionRandomiserAffector*>(source)->getRandomness());
    }
#endif

    String DirectionRandomiserAffector::CmdScope::doGet(const void* target) const
    {
        return StringConverter::toString(
            static_cast<const DirectionRandomiserAffector*>(target)->getScope() );
    }
    void DirectionRandomiserAffector::CmdScope::doSet(void* target, const String& val)
    {
        static_cast<DirectionRandomiserAffector*>(target)->setScope(StringConverter::parseReal(val));
    }
#if OGRE_STRING_INTERFACE_COPY
    void DirectionRandomiserAffector::CmdScope::doCopy(void* target, const void* source)
    {
        static_cast<DirectionRandomiserAffector*>(target)->setScope(
            static_cast<const DirectionRandomiserAffector*>(source)->getScope());
    }
#endif
    String DirectionRandomiserAffector::CmdKeepVelocity::doGet(const void* target) const
    {
        return StringConverter::toString(
            static_cast<const DirectionRandomiserAffector*>(target)->getKeepVelocity() );
    }
    void DirectionRandomiserAffector::CmdKeepVelocity::doSet(void* target, const String& val)
    {
        static_cast<DirectionRandomiserAffector*>(target)->setKeepVelocity(StringConverter::parseBool(val));
    }
#if OGRE_STRING_INTERFACE_COPY
    void DirectionRandomiserAffector::CmdKeepVelocity::doCopy(void* target, const void* source)
    {
        static_cast<DirectionRandomiserAffector*>(target)->setKeepVelocity(
            static_cast<const DirectionRandomiserAffector*>(source)->getKeepVelocity());
    }
#endif

}
