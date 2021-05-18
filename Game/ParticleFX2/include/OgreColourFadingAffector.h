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
#ifndef __ColourFadingAffector_H__
#define __ColourFadingAffector_H__

#include "OgreParticleFX2Prerequisites.h"
#include "OgreParticleAffector.h"
#include "OgreColourValue.h"

namespace Ogre {

	/** The particle affector is based on the ColourInterpolatorAffector in Plugin_ParticleFX
	@remarks
		This affector (see ParticleAffector) will change the colour of the particle during the lifetime
	of the particle, this is the same as ColourInterpolatorAffector.But in ColourInterpolatorAffector,
	the colour change of each stage is process just one time, now we can use the use parameter "RepeatTimes"
	to set the process times of colour change.
	We also can set the opacity of the particle with the "Opacity" parameter, and set the "fade in" and "fade
	out" time of the particle.
		If you want to use the origin colour of the texture, and just want to set the "fade in" "fade out" or 
	"Opacity", you can set the colour of stage one to "1.0 1.0 1.0 1.0",and set the time of stage one th 1
	*/
	class _OgreParticleFX2Export ColourFadingAffector : public ParticleAffector
	{
	public:

		// this is something of a hack.. 
		// needs to be replaced with something more.. 
		// ..elegant
		enum { MAX_STAGES = 6 };

		/** Command object for colour of each stage (see ParamCommand).*/
		class CmdColourAdjust : public ParamCommand
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

		/** Command object for time of each stage (see ParamCommand).*/
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

		/** Command object for opacity (see ParamCommand).*/
		class CmdOpacity : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		/** Command object for the time of fading in (see ParamCommand).*/
		class CmdFadeInTime : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		/** Command object for the time of fading out (see ParamCommand).*/
		class CmdFadeOutTime : public ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		/// Default constructor
		ColourFadingAffector(ParticleSystem* psys);

		/** See ParticleAffector. */
		void _initParticle(Particle* pParticle);

		/** See ParticleAffector. */
		void _affectParticles(ParticleSystem* pSystem, Real timeElapsed);

		void setColourAdjust(size_t index, ColourValue colour);
		ColourValue getColourAdjust(size_t index) const;

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

		void setOpacity(Real opacity)
		{
			mOpacity = opacity;
		}

		Real getOpacity(void) const
		{
			return mOpacity;
		}

		void setFadeInTime(Real time)
		{
			mFadeInTime = time;
		}

		Real getFadeInTime(void) const
		{
			return mFadeInTime;
		}

		void setFadeOutTime(Real time)
		{
			mFadeOutTime = time;
		}

		Real getFadeOutTime(void) const
		{
			return mFadeOutTime;
		}

		static CmdColourAdjust	msColourCmd[MAX_STAGES];
		static CmdTimeAdjust	msTimeCmd[MAX_STAGES];
		static CmdRepeatTimes   msRepeatTimesCmd;
		static CmdOpacity msOpacityCmd;
		static CmdFadeInTime msFadeInTimeCmd;
		static CmdFadeOutTime msFadeOutTimeCmd;	

	protected:
		
		/// the opacity of the particle
		Real mOpacity;
		/** the fade in time of the particle
		@remark 
			that determines how long the particle will take to fade in, beginning from the moment it is spawned.
		*/
		Real mFadeInTime;

		/** the fade out time of the particle
		@remark 
		that determines how long the particle will take to fade out.The particle 
		will be completely Faded out at the moment it die.
		*/
		Real mFadeOutTime;

		ColourValue				mColourAdj[MAX_STAGES];
		Real					mTimeAdj[MAX_STAGES];
		Real                    mRepeatTimes;
	};
}

#endif

