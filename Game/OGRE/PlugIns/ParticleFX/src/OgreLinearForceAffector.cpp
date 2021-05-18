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
#include "OgreLinearForceAffector.h"
#include "OgreParticleSystem.h"
#include "OgreParticle.h"
#include "OgreStringConverter.h"


namespace Ogre {

    // Instantiate statics
    LinearForceAffector::CmdForceVector LinearForceAffector::msForceVectorCmd;
    LinearForceAffector::CmdForceApp LinearForceAffector::msForceAppCmd;


    //-----------------------------------------------------------------------
    LinearForceAffector::LinearForceAffector(ParticleSystem* psys)
        :ParticleAffector(psys)
    {
        mType = "LinearForce";

        // Default to gravity-like
        mForceApplication = FA_ADD;
        mForceVector.x = mForceVector.z = 0;
        mForceVector.y = -100;

        // Set up parameters
        if (createParamDictionary("LinearForceAffector"))
        {
            addBaseParameters();
            // Add extra paramaters
            ParamDictionary* dict = getParamDictionary();
            dict->addParameter(ParameterDef("force_vector", 
                "The vector representing the force to apply.",
                PT_VECTOR3),&msForceVectorCmd);
            dict->addParameter(ParameterDef("force_application", 
                "How to apply the force vector to partices.",
                PT_STRING),&msForceAppCmd);

        }

    }
    //-----------------------------------------------------------------------
    void LinearForceAffector::_affectParticles(ParticleSystem* pSystem, Real timeElapsed)
    {
        ParticleIterator pi = pSystem->_getIterator();
        Particle *p;

        Vector3 scaledVector;

        // Precalc scaled force for optimisation
        if (mForceApplication == FA_ADD)
        {
            // Scale force by time
            scaledVector = mForceVector * timeElapsed;
        }

        while (!pi.end())
        {
            p = pi.getNext();
            if (mForceApplication == FA_ADD)
            {
                p->direction += scaledVector;
            }
            else // FA_AVERAGE
            {
                p->direction = (p->direction + mForceVector) / 2;
            }
        }
        
    }
    //-----------------------------------------------------------------------
    void LinearForceAffector::setForceVector(const Vector3& force)
    {
        mForceVector = force;
    }
    //-----------------------------------------------------------------------
    void LinearForceAffector::setForceApplication(ForceApplication fa)
    {
        mForceApplication = fa;
    }
    //-----------------------------------------------------------------------
    Vector3 LinearForceAffector::getForceVector(void) const
    {
        return mForceVector;
    }
    //-----------------------------------------------------------------------
    LinearForceAffector::ForceApplication LinearForceAffector::getForceApplication(void) const
    {
        return mForceApplication;
    }

    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    // Command objects
    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    String LinearForceAffector::CmdForceVector::doGet(const void* target) const
    {
        return StringConverter::toString(
            static_cast<const LinearForceAffector*>(target)->getForceVector() );
    }
    void LinearForceAffector::CmdForceVector::doSet(void* target, const String& val)
    {
        static_cast<LinearForceAffector*>(target)->setForceVector(
            StringConverter::parseVector3(val));
    }
#if OGRE_STRING_INTERFACE_COPY
    void LinearForceAffector::CmdForceVector::doCopy(void* target, const void* source)
    {
        static_cast<LinearForceAffector*>(target)->setForceVector(
            static_cast<const LinearForceAffector*>(source)->getForceVector());
    }
#endif
    //-----------------------------------------------------------------------
    String LinearForceAffector::CmdForceApp::doGet(const void* target) const
    {
        ForceApplication app = static_cast<const LinearForceAffector*>(target)->getForceApplication();
        switch(app)
        {
        case LinearForceAffector::FA_AVERAGE:
            return "average";
            break;
        case LinearForceAffector::FA_ADD:
            return "add";
            break;
        }
        // Compiler nicety
        return StringUtil::BLANK;
    }
    void LinearForceAffector::CmdForceApp::doSet(void* target, const String& val)
    {
        if (val == "average")
        {
            static_cast<LinearForceAffector*>(target)->setForceApplication(FA_AVERAGE);
        }
        else if (val == "add")
        {
            static_cast<LinearForceAffector*>(target)->setForceApplication(FA_ADD);
        }
    }
#if OGRE_STRING_INTERFACE_COPY
    void LinearForceAffector::CmdForceApp::doCopy(void* target, const void* source)
    {
        static_cast<LinearForceAffector*>(target)->setForceApplication(
            static_cast<const LinearForceAffector*>(source)->getForceApplication());
    }
#endif


}

