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
#ifndef __MeshRotationAffector_H__
#define __MeshRotationAffector_H__

#include "OgreParticleAffector.h"
#include "OgreVector3.h"


namespace Ogre {

	enum RotationType
	{
		YAW,
		PITCH,
		ROLL,
	};
    /** This class defines a ParticleAffector which applies a rotation to the mesh particle   
    */
    class MeshRotationAffector : public ParticleAffector
    {
    public:
       
        /** Command object for force application (see ParamCommand).*/
        class CmdYawRotationSpeedRangeStart : public ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
        };

        /// Command object for particle emitter  - see ParamCommand 
        class CmdYawRotationSpeedRangeEnd : public ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
        };

		/// Command object for particle emitter  - see ParamCommand 
        class CmdYawRotationRangeStart : public ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
        };

        /// Command object for particle emitter  - see ParamCommand 
        class CmdYawRotationRangeEnd : public ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
        };

		/** Command object for force application (see ParamCommand).*/
		class CmdPitchRotationSpeedRangeStart : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};

		/// Command object for particle emitter  - see ParamCommand 
		class CmdPitchRotationSpeedRangeEnd : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};

		/// Command object for particle emitter  - see ParamCommand 
		class CmdPitchRotationRangeStart : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};

		/// Command object for particle emitter  - see ParamCommand 
		class CmdPitchRotationRangeEnd : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};

		/** Command object for force application (see ParamCommand).*/
		class CmdRollRotationSpeedRangeStart : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};

		/// Command object for particle emitter  - see ParamCommand 
		class CmdRollRotationSpeedRangeEnd : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};

		/// Command object for particle emitter  - see ParamCommand 
		class CmdRollRotationRangeStart : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};

		/// Command object for particle emitter  - see ParamCommand 
		class CmdRollRotationRangeEnd : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
		};
       
	    /// Default constructor
        MeshRotationAffector(ParticleSystem* psys);

	    /** See ParticleAffector. */
		void _initParticle(Particle* pParticle);

        /** See ParticleAffector. */
        void _affectParticles(ParticleSystem* pSystem, Real timeElapsed);
 
		//////////////////////////////////////////////////////////////////////////
		/** Sets the minimum rotation speed of particles to be emitted. */
		void setRotationSpeedRangeStart(const Radian& angle, RotationType type);
#ifndef OGRE_FORCE_ANGLE_TYPES
		inline void setRotationSpeedRangeStart(Real angle, RotationType type) {
			setRotationSpeedRangeStart(Angle(angle), type);
		}
#endif//OGRE_FORCE_ANGLE_TYPES
		/** Sets the maximum rotation speed of particles to be emitted. */
		void setRotationSpeedRangeEnd(const Radian& angle, RotationType type);
#ifndef OGRE_FORCE_ANGLE_TYPES
		inline void setRotationSpeedRangeEnd(Real angle, RotationType type) {
			setRotationSpeedRangeEnd(Angle(angle), type);
		}
#endif//OGRE_FORCE_ANGLE_TYPES
		/** Gets the minimum rotation speed of particles to be emitted. */
		const Radian& getRotationSpeedRangeStart(RotationType type) const;
		/** Gets the maximum rotation speed of particles to be emitted. */
		const Radian& getRotationSpeedRangeEnd(RotationType type) const;


		/** Sets the minimum rotation angle of particles to be emitted. */
		void setRotationRangeStart(const Radian& angle, RotationType type);
#ifndef OGRE_FORCE_ANGLE_TYPES
		inline void setRotationRangeStart(Real angle, RotationType type) {
			setRotationRangeStart(Angle(angle), type);
		}
#endif//OGRE_FORCE_ANGLE_TYPES
		/** Sets the maximum rotation angle of particles to be emitted. */
		void setRotationRangeEnd(const Radian& angle, RotationType type);
#ifndef OGRE_FORCE_ANGLE_TYPES
		inline void setRotationRangeEnd(Real angle, RotationType type) {
			setRotationRangeEnd(Angle(angle), type);
		}
#endif//OGRE_FORCE_ANGLE_TYPES
		/** Gets the minimum rotation of particles to be emitted. */
		const Radian& getRotationRangeStart(RotationType type) const;
		/** Gets the maximum rotation of particles to be emitted. */
		const Radian& getRotationRangeEnd(RotationType type) const;
	
		//////////////////////////////////////////////////////////////////////////
		
		static CmdYawRotationSpeedRangeStart	msYawRotationSpeedRangeStartCmd;
		static CmdYawRotationSpeedRangeEnd		msYawRotationSpeedRangeEndCmd;
		static CmdYawRotationRangeStart		msYawRotationRangeStartCmd;
		static CmdYawRotationRangeEnd			msYawRotationRangeEndCmd;

		static CmdPitchRotationSpeedRangeStart	msPitchRotationSpeedRangeStartCmd;
		static CmdPitchRotationSpeedRangeEnd		msPitchRotationSpeedRangeEndCmd;
		static CmdPitchRotationRangeStart		msPitchRotationRangeStartCmd;
		static CmdPitchRotationRangeEnd			msPitchRotationRangeEndCmd;

		static CmdRollRotationSpeedRangeStart	msRollRotationSpeedRangeStartCmd;
		static CmdRollRotationSpeedRangeEnd		msRollRotationSpeedRangeEndCmd;
		static CmdRollRotationRangeStart		msRollRotationRangeStartCmd;
		static CmdRollRotationRangeEnd			msRollRotationRangeEndCmd;

    protected:

		/// Initial yaw rotation speed of particles (range start)
		Radian mYawRotationSpeedRangeStart;
		/// Initial yaw rotation speed of particles (range end)
		Radian mYawRotationSpeedRangeEnd;
		/// Initial yaw rotation angle of particles (range start)
		Radian mYawRotationRangeStart;
		/// Initial yaw rotation angle of particles (range end)
		Radian mYawRotationRangeEnd;

		/// Initial pitch rotation speed of particles (range start)
		Radian mPitchRotationSpeedRangeStart;
		/// Initial pitch rotation speed of particles (range end)
		Radian mPitchRotationSpeedRangeEnd;
		/// Initial pitch rotation angle of particles (range start)
		Radian mPitchRotationRangeStart;
		/// Initial pitch rotation angle of particles (range end)
		Radian mPitchRotationRangeEnd;

		/// Initial roll rotation speed of particles (range start)
		Radian mRollRotationSpeedRangeStart;
		/// Initial roll rotation speed of particles (range end)
		Radian mRollRotationSpeedRangeEnd;
		/// Initial roll rotation angle of particles (range start)
		Radian mRollRotationRangeStart;
		/// Initial roll rotation angle of particles (range end)
		Radian mRollRotationRangeEnd;
    };
}


#endif

