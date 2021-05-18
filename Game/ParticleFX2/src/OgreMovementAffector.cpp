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
#include "OgreMovementAffector.h"
#include "OgreParticleSystem.h"
#include "OgreStringConverter.h"
#include "OgreParticleEmitter.h"
#include "OgreNode.h"

namespace Ogre {

	// init statics
	MovementAffector::CmdUseStartVelocity MovementAffector::msUseStartVelocityCmd;
	MovementAffector::CmdStartVelocityMin MovementAffector::msStartVelocityMinCmd;
	MovementAffector::CmdStartVelocityMax MovementAffector::msStartVelocityMaxCmd;
	MovementAffector::CmdAcceleration MovementAffector::msAccelerationCmd;
	MovementAffector::CmdVelocityLossMin MovementAffector::msVelocityLossMinCmd;
	MovementAffector::CmdVelocityLossMax MovementAffector::msVelocityLossMaxCmd;

	MovementAffector::CmdRandomnessMin MovementAffector::msRandomnessMinCmd;
	MovementAffector::CmdRandomnessMax MovementAffector::msRandomnessMaxCmd;

	/*MovementAffector::CmdUseSpeedFactor		MovementAffector::msUseSpeedFactorCmd;
	MovementAffector::CmdSpeedFactorAdjust MovementAffector::msSpeedFactorCmd[MAX_STAGES];
	MovementAffector::CmdTimeAdjust		MovementAffector::msTimeCmd[MAX_STAGES];
	MovementAffector::CmdRepeatTimes		MovementAffector::msRepeatTimesCmd;*/

	//-----------------------------------------------------------------------
	MovementAffector::MovementAffector(ParticleSystem* psys)
		:ParticleAffector(psys)
	{
		/*mUseSpeedFactor = false;

		for (int i=0;i<MAX_STAGES;i++)
		{
			mSpeedFactorAdj[i]	= 1.0f;
			mTimeAdj[i]		= 1.0f;
		}

		mRepeatTimes = 1.0f;*/

		mStartVelocityMin = Ogre::Vector3::ZERO;
		mStartVelocityMax = Ogre::Vector3::ZERO;
		mAcceleration = Ogre::Vector3::ZERO;

		mVelocityLossMin = Ogre::Vector3::ZERO;
		mVelocityLossMax = Ogre::Vector3::ZERO;

		mRandomnessMin = Ogre::Vector3::ZERO;
		mRandomnessMax = Ogre::Vector3::ZERO;

		mUseStartVelocity = false;

		mType = "Movement";

		// Init parameters
		if (createParamDictionary("MovementAffector"))
		{
			ParamDictionary* dict = getParamDictionary();

			dict->addParameter(ParameterDef("use_start_velocity", 
				"the repeat times of the colour interpolate.",
				PT_BOOL), &msUseStartVelocityCmd);

			dict->addParameter(ParameterDef("start_velocity_min", 
				"the repeat times of the colour interpolate.",
				PT_VECTOR3), &msStartVelocityMinCmd);

			dict->addParameter(ParameterDef("start_velocity_max", 
				"the repeat times of the colour interpolate.",
				PT_VECTOR3), &msStartVelocityMaxCmd);

			dict->addParameter(ParameterDef("acceleration", 
				"the repeat times of the colour interpolate.",
				PT_VECTOR3), &msAccelerationCmd);

			dict->addParameter(ParameterDef("velocity_loss_min", 
				"the repeat times of the colour interpolate.",
				PT_VECTOR3), &msVelocityLossMinCmd);

			dict->addParameter(ParameterDef("velocity_loss_max", 
				"the repeat times of the colour interpolate.",
				PT_VECTOR3), &msVelocityLossMaxCmd);

			dict->addParameter(ParameterDef("randomness_min", 
				"the repeat times of the colour interpolate.",
				PT_VECTOR3), &msRandomnessMinCmd);

			dict->addParameter(ParameterDef("randomness_max", 
				"the repeat times of the colour interpolate.",
				PT_VECTOR3), &msRandomnessMaxCmd);

			/*dict->addParameter(ParameterDef("use_speed_factor", 
				"the repeat times of the colour interpolate.",
				PT_BOOL), &msUseSpeedFactorCmd);

			for (int i=0;i<MAX_STAGES;i++)
			{
				msSpeedFactorCmd[i].mIndex	= i;
				msTimeCmd[i].mIndex		= i;

				StringUtil::StrStreamType stage;
				stage << i;
				String	speed_title	= String("speed_factor") + stage.str();
				String	time_title		= String("time") + stage.str();
				String	speed_descr	= String("Stage ") + stage.str() + String(" speed.");
				String	time_descr		= String("Stage ") + stage.str() + String(" time.");

				dict->addParameter(ParameterDef(speed_title, speed_descr, PT_REAL), &msSpeedFactorCmd[i]);
				dict->addParameter(ParameterDef(time_title,   time_descr,   PT_REAL),		 &msTimeCmd[i]);
			}

			dict->addParameter(ParameterDef("repeat_times", 
				"the repeat times of the colour interpolate.",
				PT_REAL), &msRepeatTimesCmd);*/
		}
	}
	//-----------------------------------------------------------------------
	void MovementAffector::_initParticle(Particle* pParticle)
	{
		if (mUseStartVelocity)
		{
			// set the velocity when the particle was born
			pParticle->direction.x = Math::RangeRandom( mStartVelocityMin.x, mStartVelocityMax.x );
			pParticle->direction.y = Math::RangeRandom( mStartVelocityMin.y, mStartVelocityMax.y );
			pParticle->direction.z = Math::RangeRandom( mStartVelocityMin.z, mStartVelocityMax.z );
		}
	}
	//-----------------------------------------------------------------------
	void MovementAffector::_affectParticles(ParticleSystem* pSystem, Real timeElapsed)
	{
		ParticleIterator pi = pSystem->_getIterator();
		Particle *p;

		while (!pi.end())
		{
			p = pi.getNext();

			// apply the acceleration
			p->direction += mAcceleration * timeElapsed;;

			// apply the randomness direction
			p->direction += Vector3(Math::RangeRandom(mRandomnessMin.x, mRandomnessMax.x) * timeElapsed,
				Math::RangeRandom(mRandomnessMin.y, mRandomnessMax.y) * timeElapsed,
				Math::RangeRandom(mRandomnessMin.z, mRandomnessMax.z) * timeElapsed);
		
			//// apply the speed factor
			//if (mUseSpeedFactor)
			//{
			//	const Real		life_time		= p->totalTimeToLive;
			//	Real			particle_time	= 1.0f - (p->timeToLive / life_time); 

			//	// wrap the particle time
			//	Real            repeatedParticleTime = fmod( particle_time * mRepeatTimes, 1.0f );

			//	if (repeatedParticleTime <= mTimeAdj[0])
			//	{
			//		p->direction *= mSpeedFactorAdj[0];
			//	} 
			//	else if (repeatedParticleTime >= mTimeAdj[MAX_STAGES - 1])
			//	{
			//		p->direction *= mSpeedFactorAdj[MAX_STAGES - 1];

			//	} else
			//	{
			//		for (int i=0;i<MAX_STAGES-1;i++)
			//		{
			//			if (repeatedParticleTime >= mTimeAdj[i] && repeatedParticleTime < mTimeAdj[i + 1])
			//			{
			//				repeatedParticleTime -= mTimeAdj[i];
			//				repeatedParticleTime /= (mTimeAdj[i+1]-mTimeAdj[i]);
			//				p->direction *= (mSpeedFactorAdj[i+1] * repeatedParticleTime) +
			//					(mSpeedFactorAdj[i] * (1.0f - repeatedParticleTime));
			//				break;
			//			}
			//		}
			//	}
			//}			
		}	
	}
	//-----------------------------------------------------------------------
	String MovementAffector::CmdStartVelocityMin::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const MovementAffector*>(target)->getStartVelocityMin() );
	}
	void MovementAffector::CmdStartVelocityMin::doSet(void* target, const String& val)
	{
		static_cast<MovementAffector*>(target)->setStartVelocityMin(
			StringConverter::parseVector3(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void MovementAffector::CmdStartVelocityMin::doCopy(void* target, const void* source)
    {
        static_cast<MovementAffector*>(target)->setStartVelocityMin(
            static_cast<const MovementAffector*>(source)->getStartVelocityMin() );
    }
#endif
	//-----------------------------------------------------------------------
	String MovementAffector::CmdStartVelocityMax::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const MovementAffector*>(target)->getStartVelocityMax() );
	}
	void MovementAffector::CmdStartVelocityMax::doSet(void* target, const String& val)
	{
		static_cast<MovementAffector*>(target)->setStartVelocityMax(
			StringConverter::parseVector3(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void MovementAffector::CmdStartVelocityMax::doCopy(void* target, const void* source)
    {
        static_cast<MovementAffector*>(target)->setStartVelocityMax(
            static_cast<const MovementAffector*>(source)->getStartVelocityMax() );
    }
#endif
	//-----------------------------------------------------------------------
	String MovementAffector::CmdAcceleration::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const MovementAffector*>(target)->getAcceleration() );
	}
	void MovementAffector::CmdAcceleration::doSet(void* target, const String& val)
	{
		static_cast<MovementAffector*>(target)->setAcceleration(
			StringConverter::parseVector3(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void MovementAffector::CmdAcceleration::doCopy(void* target, const void* source)
    {
        static_cast<MovementAffector*>(target)->setAcceleration(
            static_cast<const MovementAffector*>(source)->getAcceleration() );
    }
#endif
	//-----------------------------------------------------------------------
	String MovementAffector::CmdVelocityLossMin::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const MovementAffector*>(target)->getVelocityLossMin() );
	}
	void MovementAffector::CmdVelocityLossMin::doSet(void* target, const String& val)
	{
		static_cast<MovementAffector*>(target)->setVelocityLossMin(
			StringConverter::parseVector3(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void MovementAffector::CmdVelocityLossMin::doCopy(void* target, const void* source)
    {
        static_cast<MovementAffector*>(target)->setVelocityLossMin(
            static_cast<const MovementAffector*>(source)->getVelocityLossMin() );
    }
#endif
	//-----------------------------------------------------------------------
	String MovementAffector::CmdVelocityLossMax::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const MovementAffector*>(target)->getVelocityLossMax() );
	}
	void MovementAffector::CmdVelocityLossMax::doSet(void* target, const String& val)
	{
		static_cast<MovementAffector*>(target)->setVelocityLossMax(
			StringConverter::parseVector3(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void MovementAffector::CmdVelocityLossMax::doCopy(void* target, const void* source)
    {
        static_cast<MovementAffector*>(target)->setVelocityLossMax(
            static_cast<const MovementAffector*>(source)->getVelocityLossMax() );
    }
#endif
	//-----------------------------------------------------------------------
	String MovementAffector::CmdRandomnessMin::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const MovementAffector*>(target)->getRandomnessMin() );
	}
	void MovementAffector::CmdRandomnessMin::doSet(void* target, const String& val)
	{
		static_cast<MovementAffector*>(target)->setRandomnessMin(
			StringConverter::parseVector3(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void MovementAffector::CmdRandomnessMin::doCopy(void* target, const void* source)
    {
        static_cast<MovementAffector*>(target)->setRandomnessMin(
            static_cast<const MovementAffector*>(source)->getRandomnessMin() );
    }
#endif
	//-----------------------------------------------------------------------
	String MovementAffector::CmdRandomnessMax::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const MovementAffector*>(target)->getRandomnessMax() );
	}
	void MovementAffector::CmdRandomnessMax::doSet(void* target, const String& val)
	{
		static_cast<MovementAffector*>(target)->setRandomnessMax(
			StringConverter::parseVector3(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void MovementAffector::CmdRandomnessMax::doCopy(void* target, const void* source)
    {
        static_cast<MovementAffector*>(target)->setRandomnessMax(
            static_cast<const MovementAffector*>(source)->getRandomnessMax() );
    }
#endif
	//-----------------------------------------------------------------------
	String MovementAffector::CmdUseStartVelocity::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const MovementAffector*>(target)->getUseStartVelocity() );
	}
	void MovementAffector::CmdUseStartVelocity::doSet(void* target, const String& val)
	{
		static_cast<MovementAffector*>(target)->setUseStartVelocity(
			StringConverter::parseBool(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void MovementAffector::CmdUseStartVelocity::doCopy(void* target, const void* source)
    {
        static_cast<MovementAffector*>(target)->setUseStartVelocity(
            static_cast<const MovementAffector*>(source)->getUseStartVelocity() );
    }
#endif
}