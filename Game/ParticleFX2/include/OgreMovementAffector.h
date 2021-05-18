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
#ifndef __MovementAffector_H__
#define __MovementAffector_H__

#include "OgreParticleFX2Prerequisites.h"
#include "OgreParticleAffector.h"
#include "OgreStringInterface.h"
#include "OgreVector3.h"

namespace Ogre {


	typedef Ogre::Vector3 Velocity;
	typedef Ogre::Vector3 Acceleration;
	typedef Ogre::Vector3 VelocityLoss;
	typedef Ogre::Vector3 Randomness;

	/** This particle affector is used to set the movement of the particle.
	@remarks
		Now it is able to:
		1.set the random direction of particle when the particle is born
		2.set the acceleration 
		3.set the randomness direction force

	@todo
		1.set the velocity loss
		2.improve the speed factor
	*/
	class _OgreParticleFX2Export MovementAffector : public ParticleAffector
	{
	public:		

		/** Command object for start velocity min (see ParamCommand).*/
		class CmdUseStartVelocity : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
		};

		/** Command object for start velocity min (see ParamCommand).*/
		class CmdStartVelocityMin : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		/** Command object for start velocity max (see ParamCommand).*/
		class CmdStartVelocityMax : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		/** Command object for acceleration (see ParamCommand).*/
		class CmdAcceleration : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		/** Command object for repeat times (see ParamCommand).*/
		class CmdVelocityLossMin : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		/** Command object for repeat times (see ParamCommand).*/
		class CmdVelocityLossMax : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		/** Command object for repeat times (see ParamCommand).*/
		class CmdRandomnessMin : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		/** Command object for repeat times (see ParamCommand).*/
		class CmdRandomnessMax : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
        };

		/** Default constructor. */
		MovementAffector(ParticleSystem* psys);

		/** See ParticleAffector. */
		void _initParticle(Particle* pParticle);

		/** See ParticleAffector. */
		void _affectParticles(ParticleSystem* pSystem, Real timeElapsed);

		void setStartVelocityMin(Velocity &v)
		{
			mStartVelocityMin = v;
		}
		Velocity getStartVelocityMin(void)	const
		{
			return mStartVelocityMin;
		}

		void setStartVelocityMax(Velocity &v)
		{
			mStartVelocityMax = v;
		}
		Velocity getStartVelocityMax(void)	const
		{
			return mStartVelocityMax;
		}

		void setAcceleration(Acceleration &a)
		{
			mAcceleration = a;
		}
		Acceleration getAcceleration(void)	const
		{
			return mAcceleration;
		}

		void setVelocityLossMin( VelocityLoss& loss )
		{
			mVelocityLossMin = loss;
		}
		VelocityLoss getVelocityLossMin(void) const
		{
			return mVelocityLossMin;
		}

		void setVelocityLossMax( VelocityLoss& loss )
		{
			mVelocityLossMax = loss;
		}
		VelocityLoss getVelocityLossMax(void) const
		{
			return mVelocityLossMax;
		}

		void setRandomnessMin( Randomness& loss )
		{
			mRandomnessMin = loss;
		}
		Randomness getRandomnessMin(void) const
		{
			return mRandomnessMin;
		}

		void setRandomnessMax( Randomness& loss )
		{
			mRandomnessMax = loss;
		}
		Randomness getRandomnessMax(void) const
		{
			return mRandomnessMax;
		}

		void setUseStartVelocity(bool use)
		{
			mUseStartVelocity = use;
		}
		bool getUseStartVelocity(void) const
		{
			return mUseStartVelocity;
		}

		/*void setUseSpeedFactor(bool use)
		{
			mUseSpeedFactor = use;
		}
		bool getUseSpeedFactor(void) const
		{
			return mUseSpeedFactor;
		}
		void setSpeedFactor(size_t index, Real rate );
		Real getSpeedFactor(size_t index) const;

		void setTimeAdjust(size_t index, Real time);
		Real getTimeAdjust(size_t index) const;

		void setRepeatTimes(Real times)
		{
			mRepeatTimes = times;
		}
		Real getRepeatTimes(void) const
		{
			return mRepeatTimes;
		}		*/

		static CmdUseStartVelocity msUseStartVelocityCmd;
		static CmdStartVelocityMin msStartVelocityMinCmd;
		static CmdStartVelocityMax msStartVelocityMaxCmd;
		static CmdAcceleration msAccelerationCmd;

		static CmdVelocityLossMin msVelocityLossMinCmd;
		static CmdVelocityLossMax msVelocityLossMaxCmd;

		static CmdRandomnessMin msRandomnessMinCmd;
		static CmdRandomnessMax msRandomnessMaxCmd;

		/*static CmdUseSpeedFactor msUseSpeedFactorCmd;
		static CmdSpeedFactorAdjust msSpeedFactorCmd[MAX_STAGES];
		static CmdTimeAdjust	msTimeCmd[MAX_STAGES];
		static CmdRepeatTimes   msRepeatTimesCmd;*/

	protected:

		bool mUseStartVelocity;
		Velocity mStartVelocityMin;
		Velocity mStartVelocityMax;		

        Acceleration mAcceleration;
		VelocityLoss mVelocityLossMin;
		VelocityLoss mVelocityLossMax;

		Randomness mRandomnessMin;
		Randomness mRandomnessMax;

		/*bool                    mUseSpeedFactor;
		Real                    mSpeedFactorAdj[MAX_STAGES];
		Real					mTimeAdj[MAX_STAGES];
		Real                    mRepeatTimes;*/
	};
}


#endif

