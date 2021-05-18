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
#include "OgreStableHeaders.h"
#include "OgreParticleEmitterCommands.h"
#include "OgreParticleEmitter.h"
#include "OgreStringConverter.h"


namespace Ogre {

    namespace EmitterCommands {

        //-----------------------------------------------------------------------
        String CmdAngle::doGet(const void* target) const
        {
            return StringConverter::toString(
                static_cast<const ParticleEmitter*>(target)->getAngle() );
        }
        void CmdAngle::doSet(void* target, const String& val)
        {
            static_cast<ParticleEmitter*>(target)->setAngle(StringConverter::parseAngle(val));
        }
#if OGRE_STRING_INTERFACE_COPY
        void CmdAngle::doCopy(void* target, const void* source)
        {
            static_cast<ParticleEmitter*>(target)->setAngle(
                static_cast<const ParticleEmitter*>(source)->getAngle());
        }
#endif
        //-----------------------------------------------------------------------
        String CmdColour::doGet(const void* target) const
        {
            return StringConverter::toString(
                static_cast<const ParticleEmitter*>(target)->getColour() );
        }
        void CmdColour::doSet(void* target, const String& val)
        {
            static_cast<ParticleEmitter*>(target)->setColour(StringConverter::parseColourValue(val));
        }
#if OGRE_STRING_INTERFACE_COPY
        void CmdColour::doCopy(void* target, const void* source)
        {
            static_cast<ParticleEmitter*>(target)->setColour(
                static_cast<const ParticleEmitter*>(source)->getColour());
        }
#endif
        //-----------------------------------------------------------------------
        String CmdColourRangeStart::doGet(const void* target) const
        {
            return StringConverter::toString(
                static_cast<const ParticleEmitter*>(target)->getColourRangeStart() );
        }
        void CmdColourRangeStart::doSet(void* target, const String& val)
        {
            static_cast<ParticleEmitter*>(target)->setColourRangeStart(StringConverter::parseColourValue(val));
        }
#if OGRE_STRING_INTERFACE_COPY
        void CmdColourRangeStart::doCopy(void* target, const void* source)
        {
            static_cast<ParticleEmitter*>(target)->setColourRangeStart(
                static_cast<const ParticleEmitter*>(source)->getColourRangeStart());
        }
#endif
        //-----------------------------------------------------------------------
        String CmdColourRangeEnd::doGet(const void* target) const
        {
            return StringConverter::toString(
                static_cast<const ParticleEmitter*>(target)->getColourRangeEnd() );
        }
        void CmdColourRangeEnd::doSet(void* target, const String& val)
        {
            static_cast<ParticleEmitter*>(target)->setColourRangeEnd(StringConverter::parseColourValue(val));
        }
#if OGRE_STRING_INTERFACE_COPY
        void CmdColourRangeEnd::doCopy(void* target, const void* source)
        {
            static_cast<ParticleEmitter*>(target)->setColourRangeEnd(
                static_cast<const ParticleEmitter*>(source)->getColourRangeEnd());
        }
#endif
        //-----------------------------------------------------------------------
        String CmdDirection::doGet(const void* target) const
        {
            return StringConverter::toString(
                static_cast<const ParticleEmitter*>(target)->getDirection() );
        }
        void CmdDirection::doSet(void* target, const String& val)
        {
            static_cast<ParticleEmitter*>(target)->setDirection(StringConverter::parseVector3(val));
        }
#if OGRE_STRING_INTERFACE_COPY
        void CmdDirection::doCopy(void* target, const void* source)
        {
            static_cast<ParticleEmitter*>(target)->setDirection(
                static_cast<const ParticleEmitter*>(source)->getDirection());
        }
#endif
        //-----------------------------------------------------------------------
        String CmdEmissionRate::doGet(const void* target) const
        {
            return StringConverter::toString(
                static_cast<const ParticleEmitter*>(target)->getEmissionRate() );
        }
        void CmdEmissionRate::doSet(void* target, const String& val)
        {
            static_cast<ParticleEmitter*>(target)->setEmissionRate(StringConverter::parseReal(val));
        }
#if OGRE_STRING_INTERFACE_COPY
        void CmdEmissionRate::doCopy(void* target, const void* source)
        {
            static_cast<ParticleEmitter*>(target)->setEmissionRate(
                static_cast<const ParticleEmitter*>(source)->getEmissionRate());
        }
#endif
        //-----------------------------------------------------------------------
        String CmdMaxTTL::doGet(const void* target) const
        {
            return StringConverter::toString(
                static_cast<const ParticleEmitter*>(target)->getMaxTimeToLive() );
        }
        void CmdMaxTTL::doSet(void* target, const String& val)
        {
            static_cast<ParticleEmitter*>(target)->setMaxTimeToLive(StringConverter::parseReal(val));
        }
#if OGRE_STRING_INTERFACE_COPY
        void CmdMaxTTL::doCopy(void* target, const void* source)
        {
            static_cast<ParticleEmitter*>(target)->setMaxTimeToLive(
                static_cast<const ParticleEmitter*>(source)->getMaxTimeToLive());
        }
#endif
        //-----------------------------------------------------------------------
        String CmdMinTTL::doGet(const void* target) const
        {
            return StringConverter::toString(
                static_cast<const ParticleEmitter*>(target)->getMinTimeToLive() );
        }
        void CmdMinTTL::doSet(void* target, const String& val)
        {
            static_cast<ParticleEmitter*>(target)->setMinTimeToLive(StringConverter::parseReal(val));
        }
#if OGRE_STRING_INTERFACE_COPY
        void CmdMinTTL::doCopy(void* target, const void* source)
        {
            static_cast<ParticleEmitter*>(target)->setMinTimeToLive(
                static_cast<const ParticleEmitter*>(source)->getMinTimeToLive());
        }
#endif
        //-----------------------------------------------------------------------
        String CmdMaxVelocity::doGet(const void* target) const
        {
            return StringConverter::toString(
                static_cast<const ParticleEmitter*>(target)->getMaxParticleVelocity() );
        }
        void CmdMaxVelocity::doSet(void* target, const String& val)
        {
            static_cast<ParticleEmitter*>(target)->setMaxParticleVelocity(StringConverter::parseReal(val));
        }
#if OGRE_STRING_INTERFACE_COPY
        void CmdMaxVelocity::doCopy(void* target, const void* source)
        {
            static_cast<ParticleEmitter*>(target)->setMaxParticleVelocity(
                static_cast<const ParticleEmitter*>(source)->getMaxParticleVelocity());
        }
#endif
        //-----------------------------------------------------------------------
        String CmdMinVelocity::doGet(const void* target) const
        {
            return StringConverter::toString(
                static_cast<const ParticleEmitter*>(target)->getMinParticleVelocity() );
        }
        void CmdMinVelocity::doSet(void* target, const String& val)
        {
            static_cast<ParticleEmitter*>(target)->setMinParticleVelocity(StringConverter::parseReal(val));
        }
#if OGRE_STRING_INTERFACE_COPY
        void CmdMinVelocity::doCopy(void* target, const void* source)
        {
            static_cast<ParticleEmitter*>(target)->setMinParticleVelocity(
                static_cast<const ParticleEmitter*>(source)->getMinParticleVelocity());
        }
#endif
        //-----------------------------------------------------------------------
        String CmdPosition::doGet(const void* target) const
        {
            return StringConverter::toString(
                static_cast<const ParticleEmitter*>(target)->getPosition() );
        }
        void CmdPosition::doSet(void* target, const String& val)
        {
            static_cast<ParticleEmitter*>(target)->setPosition(StringConverter::parseVector3(val));
        }
#if OGRE_STRING_INTERFACE_COPY
        void CmdPosition::doCopy(void* target, const void* source)
        {
            static_cast<ParticleEmitter*>(target)->setPosition(
                static_cast<const ParticleEmitter*>(source)->getPosition());
        }
#endif
        //-----------------------------------------------------------------------
        String CmdTTL::doGet(const void* target) const
        {
            return StringConverter::toString(
                static_cast<const ParticleEmitter*>(target)->getTimeToLive() );
        }
        void CmdTTL::doSet(void* target, const String& val)
        {
            static_cast<ParticleEmitter*>(target)->setTimeToLive(StringConverter::parseReal(val));
        }
#if OGRE_STRING_INTERFACE_COPY
        void CmdTTL::doCopy(void* target, const void* source)
        {
            static_cast<ParticleEmitter*>(target)->setTimeToLive(
                static_cast<const ParticleEmitter*>(source)->getTimeToLive());
        }
#endif
        //-----------------------------------------------------------------------
        String CmdVelocity::doGet(const void* target) const
        {
            return StringConverter::toString(
                static_cast<const ParticleEmitter*>(target)->getParticleVelocity() );
        }
        void CmdVelocity::doSet(void* target, const String& val)
        {
            static_cast<ParticleEmitter*>(target)->setParticleVelocity(StringConverter::parseReal(val));
        }
#if OGRE_STRING_INTERFACE_COPY
        void CmdVelocity::doCopy(void* target, const void* source)
        {
            static_cast<ParticleEmitter*>(target)->setParticleVelocity(
                static_cast<const ParticleEmitter*>(source)->getParticleVelocity());
        }
#endif
        //-----------------------------------------------------------------------
        String CmdDuration::doGet(const void* target) const
        {
            return StringConverter::toString(
                static_cast<const ParticleEmitter*>(target)->getDuration() );
        }
        void CmdDuration::doSet(void* target, const String& val)
        {
            static_cast<ParticleEmitter*>(target)->setDuration(StringConverter::parseReal(val));
        }
#if OGRE_STRING_INTERFACE_COPY
        void CmdDuration::doCopy(void* target, const void* source)
        {
            static_cast<ParticleEmitter*>(target)->setDuration(
                static_cast<const ParticleEmitter*>(source)->getDuration());
        }
#endif
        //-----------------------------------------------------------------------
        String CmdMinDuration::doGet(const void* target) const
        {
            return StringConverter::toString(
                static_cast<const ParticleEmitter*>(target)->getMinDuration() );
        }
        void CmdMinDuration::doSet(void* target, const String& val)
        {
            static_cast<ParticleEmitter*>(target)->setMinDuration(StringConverter::parseReal(val));
        }
#if OGRE_STRING_INTERFACE_COPY
        void CmdMinDuration::doCopy(void* target, const void* source)
        {
            static_cast<ParticleEmitter*>(target)->setMinDuration(
                static_cast<const ParticleEmitter*>(source)->getMinDuration());
        }
#endif
        //-----------------------------------------------------------------------
        String CmdMaxDuration::doGet(const void* target) const
        {
            return StringConverter::toString(
                static_cast<const ParticleEmitter*>(target)->getMaxDuration() );
        }
        void CmdMaxDuration::doSet(void* target, const String& val)
        {
            static_cast<ParticleEmitter*>(target)->setMaxDuration(StringConverter::parseReal(val));
        }
#if OGRE_STRING_INTERFACE_COPY
        void CmdMaxDuration::doCopy(void* target, const void* source)
        {
            static_cast<ParticleEmitter*>(target)->setMaxDuration(
                static_cast<const ParticleEmitter*>(source)->getMaxDuration());
        }
#endif
        //-----------------------------------------------------------------------
        String CmdRepeatDelay::doGet(const void* target) const
        {
            return StringConverter::toString(
                static_cast<const ParticleEmitter*>(target)->getRepeatDelay() );
        }
        void CmdRepeatDelay::doSet(void* target, const String& val)
        {
            static_cast<ParticleEmitter*>(target)->setRepeatDelay(StringConverter::parseReal(val));
        }
#if OGRE_STRING_INTERFACE_COPY
        void CmdRepeatDelay::doCopy(void* target, const void* source)
        {
            static_cast<ParticleEmitter*>(target)->setRepeatDelay(
                static_cast<const ParticleEmitter*>(source)->getRepeatDelay());
        }
#endif
        //-----------------------------------------------------------------------
        String CmdMinRepeatDelay::doGet(const void* target) const
        {
            return StringConverter::toString(
                static_cast<const ParticleEmitter*>(target)->getMinRepeatDelay() );
        }
        void CmdMinRepeatDelay::doSet(void* target, const String& val)
        {
            static_cast<ParticleEmitter*>(target)->setMinRepeatDelay(StringConverter::parseReal(val));
        }
#if OGRE_STRING_INTERFACE_COPY
        void CmdMinRepeatDelay::doCopy(void* target, const void* source)
        {
            static_cast<ParticleEmitter*>(target)->setMinRepeatDelay(
                static_cast<const ParticleEmitter*>(source)->getMinRepeatDelay());
        }
#endif
        //-----------------------------------------------------------------------
        String CmdMaxRepeatDelay::doGet(const void* target) const
        {
            return StringConverter::toString(
                static_cast<const ParticleEmitter*>(target)->getMaxRepeatDelay() );
        }
        void CmdMaxRepeatDelay::doSet(void* target, const String& val)
        {
            static_cast<ParticleEmitter*>(target)->setMaxRepeatDelay(StringConverter::parseReal(val));
        }
#if OGRE_STRING_INTERFACE_COPY
        void CmdMaxRepeatDelay::doCopy(void* target, const void* source)
        {
            static_cast<ParticleEmitter*>(target)->setMaxRepeatDelay(
                static_cast<const ParticleEmitter*>(source)->getMaxRepeatDelay());
        }
#endif
 

    
    }
}

