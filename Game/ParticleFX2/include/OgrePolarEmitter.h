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
#ifndef __PolarEmitter_H__
#define __PolarEmitter_H__

#include "OgreParticleFX2Prerequisites.h"
#include "OgreParticleEmitter.h"

namespace Ogre {

    /** Particle emitter which emits particles randomly from points inside
    an area (box, sphere, ellipsoid whatever subclasses choose to be).
    @remarks
    This is an empty superclass and needs to be subclassed. Basic particle
    emitter emits particles from/in an (unspecified) area. The
    initial direction of these particles can either be a single direction
    (i.e. a line), a random scattering inside a cone, or a random
    scattering in all directions, depending the 'angle' parameter, which
    is the angle across which to scatter the particles either side of the
    base direction of the emitter. 
    */
    class _OgreParticleFX2Export PolarEmitter : public ParticleEmitter
    {
    public:
        /** Command object for area emitter size (see ParamCommand).*/
        class CmdRadiusMin : public ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
        };
		/** Command object for area emitter size (see ParamCommand).*/
		class CmdRadiusStep : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };
		class CmdRadiusMax : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };
        /** Command object for area emitter size (see ParamCommand).*/
        class CmdThetaMin : public ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };
        

        /** Command object for area emitter size (see ParamCommand).*/
        class CmdThetaStep : public ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };

		class CmdThetaMax : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		/** Command object for area emitter size (see ParamCommand).*/
		class CmdPhiMin : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };
        /** Command object for area emitter size (see ParamCommand).*/
        class CmdPhiStep : public ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };

		/** Command object for area emitter size (see ParamCommand).*/
		class CmdPhiMax : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		class CmdUsePolarStep : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		class CmdFlipYZAxis : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

        class CmdResetCount : public ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };

        class CmdReset : public ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };

        PolarEmitter(ParticleSystem* psys);

        virtual unsigned short _getEmissionCount(Real timeElapsed);

        virtual void _initParticle(Particle* pParticle);    

		virtual void setEnabled(bool enabled);

		void resetParameter(void)
		{
			mRadiusTotal = mRadiusMin;
			mThetaTotal = mThetaMin;
			mPhiTotal = mPhiMin;
		}

    protected:

		static CmdRadiusMin msRadiusMinCmd;
		static CmdRadiusStep msRadiusStepCmd;
		static CmdRadiusMax msRadiusMaxCmd;

        static CmdThetaMin msThetaMinCmd;
		static CmdThetaStep msThetaStepCmd;
		static CmdThetaMax msThetaMaxCmd;

        static CmdPhiMin msPhiMinCmd;
		static CmdPhiStep msPhiStepCmd;
		static CmdPhiMax msPhiMaxCmd;

		static CmdUsePolarStep msUsePolarStepCmd;
		static CmdFlipYZAxis msFlipYZAxisCmd;
        static CmdResetCount msResetRadiusCountCmd;
        static CmdReset msResetRadiusCmd;
        //////////////////////////////////////////////////////////////////////////
      //  Real mSphereRadius;
		Real mRadiusMin;
		Real mRadiusStep;
		Real mRadiusMax;

        Real mThetaMin;
        Real mThetaStep;
		Real mThetaMax;

		Real mPhiMin;
        Real mPhiStep;
		Real mPhiMax;

		Real mThetaTotal;
		Real mPhiTotal;
		Real mRadiusTotal;

		bool mUsePolarStep;
		bool mFlipYZAxis;

        /// 如果使用半径递增，ResetCount可以控制在发射多少个粒子之后半径回到最小值
        int mResetRadiusCount;
        int mCurrentResetRadiusCount;
        bool mResetRadius;
    };

}

#endif

