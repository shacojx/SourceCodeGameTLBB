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
#ifndef __ScaleInterpolatorAffector_H__
#define __ScaleInterpolatorAffector_H__

#include "OgreParticleFX2Prerequisites.h"
#include "OgreParticleAffector.h"
#include "OgreStringInterface.h"
#include "OgreVector3.h"

namespace Ogre {

    typedef Ogre::Vector3 ScaleValue;

    class _OgreParticleFX2Export ScaleInterpolatorAffector : public ParticleAffector
    {

    public:

		/** Command object for particle_width_range_start (see ParamCommand).*/
		class CmdWidthRangeStart : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
		};
		/** Command object for particle_width_range_end (see ParamCommand).*/
		class CmdWidthRangeEnd : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };        
		/** Command object for particle_height_range_start (see ParamCommand).*/
		class CmdHeightRangeStart : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };
		/** Command object for particle_height_range_end (see ParamCommand).*/
		class CmdHeightRangeEnd : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		/** Command object for particle_height_range_end (see ParamCommand).*/
		class CmdConstantScale : public ParamCommand
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


        /** Command object for red adjust (see ParamCommand).*/
        class CmdScaleAdjust : public ParamCommand
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

		/** Command object for repeat times (see ParamCommand).*/
		class CmdUseConstantScale : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		/** Command object for repeat times (see ParamCommand).*/
		class CmdUseInterpolatedScale : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		/** Command object for repeat times (see ParamCommand).*/
		class CmdUseUniformSize : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

        /** Default constructor. */
        ScaleInterpolatorAffector(ParticleSystem* psys);

		/** See ParticleAffector. */
		void _initParticle(Particle* pParticle);

        /** See ParticleAffector. */
        void _affectParticles(ParticleSystem* pSystem, Real timeElapsed);

        void setScaleAdjust(size_t index, ScaleValue& scale);
        ScaleValue getScaleAdjust(size_t index) const;

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

		void setConstantScale(ScaleValue& scale)
		{
			mConstantScaleValue = scale;
		}

		ScaleValue getConstantScale(void) const
		{
			return mConstantScaleValue;
		}

		//-----------------------------------------------------------------------
		void setWidthRangeStart(Real widthRangeStart)
		{
			mWidthRangeStart = widthRangeStart;		
		}
		//-----------------------------------------------------------------------
		Real getWidthRangeStart(void) const
		{
			return mWidthRangeStart;
		}
		//-----------------------------------------------------------------------
		void setWidthRangeEnd(Real widthRangeEnd)
		{
			mWidthRangeEnd = widthRangeEnd;
		}
		//-----------------------------------------------------------------------
		Real getWidthRangeEnd(void) const
		{
			return mWidthRangeEnd;
		}
		//-----------------------------------------------------------------------
		void setHeightRangeStart(Real heightRangeStart)
		{
			mHeightRangeStart = heightRangeStart;
		}
		//-----------------------------------------------------------------------
		Real getHeightRangeStart(void) const
		{
			return mHeightRangeStart;
		}
		//-----------------------------------------------------------------------
		void setHeightRangeEnd(Real heightRangeEnd)
		{
			mHeightRangeEnd = heightRangeEnd;
		}
		//-----------------------------------------------------------------------
		Real getHeightRangeEnd(void) const
		{
			return mHeightRangeEnd;
		}

		void setUseConstantScale(bool use)
		{
			mUseConstantScale = use;
		}
		bool getUseConstantScale(void) const
		{
			return mUseConstantScale;
		}
		void setUseInterpolatedScale(bool use)
		{
			mUseInterpolatedScale = use;
		}
		bool getUseInterpolatedScale(void) const
		{
			return mUseInterpolatedScale;
		}

		void setUseUniformSize(bool use)
		{
			mUseUniformSize = use;
		}
		bool getUseUniformSize(void) const
		{
			return mUseUniformSize;
		}

		static CmdHeightRangeStart msHeightRangeStartCmd;
		static CmdHeightRangeEnd msHeightRangeEndCmd;
		static CmdWidthRangeStart msWidthRangeStartCmd;
		static CmdWidthRangeEnd msWidthRangeEndCmd;
		static CmdConstantScale msConstantScaleCmd;

        static CmdScaleAdjust	msScaleCmd[MAX_STAGES];
        static CmdTimeAdjust	msTimeCmd[MAX_STAGES];
		static CmdRepeatTimes   msRepeatTimesCmd;
		static CmdUseConstantScale msUseConstantScaleCmd;
		static CmdUseInterpolatedScale msUseInterpolatedScaleCmd;
		static CmdUseUniformSize msUseUniformSizeCmd;

    protected:

		/// the minimum width of each particle
		Real mWidthRangeStart;
		/// the maximum width of each particle
		Real mWidthRangeEnd;
		/// the minimum height of each particle
		Real mHeightRangeStart;
		/// the maximum height of each particle
		Real mHeightRangeEnd;

		ScaleValue              mConstantScaleValue;
        ScaleValue				mScaleAdj[MAX_STAGES];
        Real					mTimeAdj[MAX_STAGES];
		Real                    mRepeatTimes;

		bool                    mUseConstantScale;
		bool                    mUseInterpolatedScale;
		bool                    mUseUniformSize;
    };


}


#endif