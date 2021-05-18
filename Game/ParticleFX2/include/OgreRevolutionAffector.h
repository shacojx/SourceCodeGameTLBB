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
#ifndef __RevolutionAffector_H__
#define __RevolutionAffector_H__

#include "OgreParticleFX2Prerequisites.h"
#include "OgreParticleAffector.h"
#include "OgreVector3.h"


namespace Ogre {

    /** This class defines a ParticleAffector which applies a linear force to particles in a system.
    @remarks
    This affector (see ParticleAffector) applies a linear force, such as gravity, to a particle system.
    This force can be applied in 2 ways: by taking the average of the particle's current momentum and the 
    force vector, or by adding the force vector to the current particle's momentum. 
    @par
    The former approach is self-stabilising i.e. once a particle's momentum
    is equal to the force vector, no further change is made to it's momentum. It also results in
    a non-linear acceleration of particles.
    The latter approach is simpler and applies a constant acceleration to particles. However,
    it is not self-stabilising and can lead to perpetually increasing particle velocities. 
    You choose the approach by calling the setForceApplication method.
    */
    class _OgreParticleFX2Export RevolutionAffector : public ParticleAffector
    {
    public:
       
        /** Command object for force application (see ParamCommand).*/
        class CmdRotationSpeed : public ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
        };

        class CmdRotationAxis : public ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };

		class CmdRadiusIncrement : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		// this is something of a hack.. 
		// needs to be replaced with something more.. 
		// ..elegant
		enum { MAX_STAGES = 6 };

		class CmdCenterOffsetMin : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		class CmdCenterOffsetMax : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		class CmdUseRadiusIncrementScale : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		/** Command object for red adjust (see ParamCommand).*/
		class CmdRadiusIncrementAdjust : public ParamCommand
		{
		public:
			size_t		mIndex;

		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		/** Command object for red adjust (see ParamCommand).*/
		class CmdTimeAdjust : public ParamCommand
		{
		public:
			size_t		mIndex;

		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		/** Command object for repeat times (see ParamCommand).*/
		class CmdRepeatTimes : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };
       
	    /// Default constructor
        RevolutionAffector(ParticleSystem* psys);

        /** See ParticleAffector. */
        void _affectParticles(ParticleSystem* pSystem, Real timeElapsed);
      
		void setCenterOffsetMin(const Vector3& point)
		{
			mCenterOffsetMin = point;
		}
		Vector3 getCenterOffsetMin(void) const
		{
			return mCenterOffsetMin;
		}

		void setCenterOffsetMax(const Vector3& point)
		{
			mCenterOffsetMax = point;
		}
		Vector3 getCenterOffsetMax(void) const
		{
			return mCenterOffsetMax;
		}

		void setRadiusIncrementAdjust(size_t index, Real increment);
		Real getRadiusIncrementAdjust(size_t index) const;

		void setTimeAdjust(size_t index, Real time);
		Real getTimeAdjust(size_t index) const;

		void setRepeatTimes(Real times)
		{
			mRepeatTimes = times;
		}
		Real getRepeatTimes(void) const
		{
			return mRepeatTimes;
		}

		void setUseRadiusIncrementScale(bool use)
		{
			mUseRadiusIncrementScale = use;
		}
		bool getUseRadiusIncrementScale(void) const
		{
			return mUseRadiusIncrementScale;
		}

		/// Command objects
		static CmdRotationSpeed msRotationSpeedCmd;
		static CmdRotationAxis msRotationAxisCmd;
		static CmdRadiusIncrement msRadiusIncrementCmd;

		static CmdUseRadiusIncrementScale msUseRadiusIncrementScaleCmd;
		static CmdCenterOffsetMin     msCenterOffsetMinCmd;
		static CmdCenterOffsetMax     msCenterOffsetMaxCmd;
		static CmdRadiusIncrementAdjust	msRadiusIncrementAdjustCmd[MAX_STAGES];
		static CmdTimeAdjust	msTimeCmd[MAX_STAGES];
		static CmdRepeatTimes   msRepeatTimesCmd;

    protected:

        Radian mRotationSpeed;

        Vector3 mRotateAxis;

		Real mRadiusIncrement;

		Real					mRadiusIncrementAdj[MAX_STAGES];
		Real					mTimeAdj[MAX_STAGES];
		Real                    mRepeatTimes;

		Vector3                 mCenterOffsetMin;
		Vector3                 mCenterOffsetMax;
		bool                    mUseRadiusIncrementScale;
    };


}


#endif

