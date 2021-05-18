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
#include "OgreScaleInterpolatorAffector.h"
#include "OgreParticleSystem.h"
#include "OgreStringConverter.h"
#include "OgreParticle.h"

namespace Ogre {

    // init statics
    ScaleInterpolatorAffector::CmdScaleAdjust		ScaleInterpolatorAffector::msScaleCmd[MAX_STAGES];
    ScaleInterpolatorAffector::CmdTimeAdjust		ScaleInterpolatorAffector::msTimeCmd[MAX_STAGES];
	ScaleInterpolatorAffector::CmdRepeatTimes		ScaleInterpolatorAffector::msRepeatTimesCmd;
	ScaleInterpolatorAffector::CmdHeightRangeStart ScaleInterpolatorAffector::msHeightRangeStartCmd;
	ScaleInterpolatorAffector::CmdHeightRangeEnd ScaleInterpolatorAffector::msHeightRangeEndCmd;
	ScaleInterpolatorAffector::CmdWidthRangeStart ScaleInterpolatorAffector::msWidthRangeStartCmd;
	ScaleInterpolatorAffector::CmdWidthRangeEnd ScaleInterpolatorAffector::msWidthRangeEndCmd;
	ScaleInterpolatorAffector::CmdConstantScale ScaleInterpolatorAffector::msConstantScaleCmd;
	ScaleInterpolatorAffector::CmdUseConstantScale ScaleInterpolatorAffector::msUseConstantScaleCmd;
	ScaleInterpolatorAffector::CmdUseInterpolatedScale ScaleInterpolatorAffector::msUseInterpolatedScaleCmd;
	ScaleInterpolatorAffector::CmdUseUniformSize ScaleInterpolatorAffector::msUseUniformSizeCmd;
    //-----------------------------------------------------------------------
    ScaleInterpolatorAffector::ScaleInterpolatorAffector(ParticleSystem* psys)
        : ParticleAffector(psys)
    {
        for (int i=0;i<MAX_STAGES;i++)
        {
            // set default colour to transparent grey, transparent since we might not want to display the particle here
            // grey because when a colour component is 0.5f the maximum difference to another colour component is 0.5f
            mScaleAdj[i]	= Ogre::Vector3(1.0f, 1.0f, 1.0f);
            mTimeAdj[i]		= 1.0f;
        }

		mRepeatTimes = 1.0f;

		mConstantScaleValue = Vector3(1.0f, 1.0f, 1.0f);
		mUseConstantScale = false;
		mUseInterpolatedScale = false;
		mUseUniformSize = true;

		mWidthRangeStart = psys->getDefaultWidth();
		mWidthRangeEnd = psys->getDefaultWidth();
		mHeightRangeStart = psys->getDefaultHeight();
		mHeightRangeEnd = psys->getDefaultHeight();

        mType = "ScaleInterpolator";

        // Init parameters
        if (createParamDictionary("ScaleInterpolatorAffector"))
        {
            ParamDictionary* dict = getParamDictionary();

			dict->addParameter(ParameterDef("uniform_size", 
				"The minimum width of particles in world units.",
				PT_BOOL),
				&msUseUniformSizeCmd);

			dict->addParameter(ParameterDef("width_range_start", 
				"The minimum width of particles in world units.",
				PT_REAL),
				&msWidthRangeStartCmd);

			dict->addParameter(ParameterDef("width_range_end", 
				"The maximum width of particles in world units.",
				PT_REAL),
				&msWidthRangeEndCmd);

			dict->addParameter(ParameterDef("height_range_start", 
				"The minimum height of particles in world units.",
				PT_REAL),
				&msHeightRangeStartCmd);

			dict->addParameter(ParameterDef("height_range_end", 
				"The maximum height of particles in world units.",
				PT_REAL),
				&msHeightRangeEndCmd);

			dict->addParameter(ParameterDef("use_constant_scale", 
				"The maximum height of particles in world units.",
				PT_BOOL),
				&msUseConstantScaleCmd);

			dict->addParameter(ParameterDef("constant_scale", 
				"The maximum height of particles in world units.",
				PT_VECTOR3),
				&msConstantScaleCmd);

			dict->addParameter(ParameterDef("use_interpolated_scale", 
				"The maximum height of particles in world units.",
				PT_BOOL),
				&msUseInterpolatedScaleCmd);

            for (int i=0;i<MAX_STAGES;i++)
            {
                msScaleCmd[i].mIndex	= i;
                msTimeCmd[i].mIndex		= i;

                StringUtil::StrStreamType stage;
                stage << i;
                String	scale_title	= String("scale") + stage.str();
                String	time_title		= String("time") + stage.str();
                String	scale_descr	= String("Stage ") + stage.str() + String(" scale.");
                String	time_descr		= String("Stage ") + stage.str() + String(" time.");

                dict->addParameter(ParameterDef(scale_title, scale_descr, PT_VECTOR3), &msScaleCmd[i]);
                dict->addParameter(ParameterDef(time_title,   time_descr,   PT_REAL),		 &msTimeCmd[i]);
            }

			dict->addParameter(ParameterDef("repeat_times", 
				"the repeat times of the colour interpolate.",
				PT_REAL), &msRepeatTimesCmd);
        }
    }
	/** See ParticleAffector. */
	void ScaleInterpolatorAffector::_initParticle(Particle* pParticle)
	{
		if ( (mWidthRangeStart != mParent->getDefaultWidth()) ||
			(mWidthRangeEnd != mParent->getDefaultWidth()) ||
			(mHeightRangeStart != mParent->getDefaultHeight()) ||
			(mHeightRangeEnd != mParent->getDefaultHeight())
			)
		{
			if (mUseUniformSize)
			{
				Real size = Math::RangeRandom( mWidthRangeStart, mWidthRangeEnd );
				pParticle->setDimensions( size,	size );
			}
			else
			{
				pParticle->setDimensions( Math::RangeRandom( mWidthRangeStart, mWidthRangeEnd ),
					Math::RangeRandom( mHeightRangeStart, mHeightRangeEnd ) );
			}
		}
	}
    //-----------------------------------------------------------------------
    void ScaleInterpolatorAffector::_affectParticles(ParticleSystem* pSystem, Real timeElapsed)
    {
        Particle*			p;
        ParticleIterator	pi				= pSystem->_getIterator();

		Real NewWide = 0.0f;
		Real NewHigh = 0.0f;

        bool needFmod = mRepeatTimes != 1.0f;

        while (!pi.end())
        {
            p = pi.getNext();

			if( p->hasOwnDimensions() == false )
			{
				p->setDimensions( pSystem->getDefaultWidth() , pSystem->getDefaultHeight() );
			}
			else
			{
				if (mUseConstantScale)
				{
					NewWide = p->getOwnWidth() + mConstantScaleValue.x * timeElapsed;
					NewHigh = p->getOwnHeight() + mConstantScaleValue.y * timeElapsed;

					if (NewWide < 0.0f)
						NewWide = 0.0f;

					if (NewHigh < 0.0f)
						NewHigh = 0.0f;

					p->setDimensions( NewWide, NewHigh );
				}
				else if (mUseInterpolatedScale)
				{
					const Real		life_time		= p->totalTimeToLive;
					Real			particle_time	= 
                        needFmod ? fmod( ( 1.0f - (p->timeToLive / life_time) ) * mRepeatTimes, 1.0f ) : ( 1.0f - (p->timeToLive / life_time) ) ; 

					if (particle_time <= mTimeAdj[0])
					{
						p->setDimensions(pSystem->getDefaultWidth()*mScaleAdj[0].x, pSystem->getDefaultHeight()*mScaleAdj[0].y);
					} 
					else if (particle_time >= mTimeAdj[MAX_STAGES - 1])
					{
						p->setDimensions(pSystem->getDefaultWidth()*mScaleAdj[MAX_STAGES-1].x, pSystem->getDefaultHeight()*mScaleAdj[MAX_STAGES-1].y);
					}
					else
					{
						for (int i=0;i<MAX_STAGES-1;i++)
						{
							if (particle_time >= mTimeAdj[i] && particle_time < mTimeAdj[i + 1])
							{
								particle_time -= mTimeAdj[i];
								particle_time /= (mTimeAdj[i+1]-mTimeAdj[i]);
								NewWide = pSystem->getDefaultWidth()*((mScaleAdj[i+1].x * particle_time) + (mScaleAdj[i].x * (1.0f - particle_time)));
								NewHigh = pSystem->getDefaultHeight()*((mScaleAdj[i+1].y * particle_time) + (mScaleAdj[i].y * (1.0f - particle_time)));
								p->setDimensions(NewWide, NewHigh);
								break;
							}
						}
					}
				}
			}                        
        }
    }

    //-----------------------------------------------------------------------
    void ScaleInterpolatorAffector::setScaleAdjust(size_t index, ScaleValue& scale)
    {
        mScaleAdj[index] = scale;
    }
    //-----------------------------------------------------------------------
    ScaleValue ScaleInterpolatorAffector::getScaleAdjust(size_t index) const
    {
        return mScaleAdj[index];
    }


    //-----------------------------------------------------------------------
    void ScaleInterpolatorAffector::setTimeAdjust(size_t index, Real time)
    {
        mTimeAdj[index] = time;
    }
    //-----------------------------------------------------------------------
    Real ScaleInterpolatorAffector::getTimeAdjust(size_t index) const
    {
        return mTimeAdj[index];
    }
    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    // Command objects
    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	String ScaleInterpolatorAffector::CmdHeightRangeStart::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const ScaleInterpolatorAffector*>(target)->getHeightRangeStart() );
	}
	//-----------------------------------------------------------------------
	void ScaleInterpolatorAffector::CmdHeightRangeStart::doSet(void* target, const String& val)
	{
		static_cast<ScaleInterpolatorAffector*>(target)->setHeightRangeStart(
			StringConverter::parseReal(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void ScaleInterpolatorAffector::CmdHeightRangeStart::doCopy(void* target, const void* source)
    {
        static_cast<ScaleInterpolatorAffector*>(target)->setHeightRangeStart(
            static_cast<const ScaleInterpolatorAffector*>(source)->getHeightRangeStart() );
    }
#endif
	//-----------------------------------------------------------------------
	String ScaleInterpolatorAffector::CmdHeightRangeEnd::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const ScaleInterpolatorAffector*>(target)->getHeightRangeEnd() );
	}
	//-----------------------------------------------------------------------
	void ScaleInterpolatorAffector::CmdHeightRangeEnd::doSet(void* target, const String& val)
	{
		static_cast<ScaleInterpolatorAffector*>(target)->setHeightRangeEnd(
			StringConverter::parseReal(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void ScaleInterpolatorAffector::CmdHeightRangeEnd::doCopy(void* target, const void* source)
    {
        static_cast<ScaleInterpolatorAffector*>(target)->setHeightRangeEnd(
            static_cast<const ScaleInterpolatorAffector*>(source)->getHeightRangeEnd() );
    }
#endif
	//-----------------------------------------------------------------------
	String ScaleInterpolatorAffector::CmdWidthRangeStart::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const ScaleInterpolatorAffector*>(target)->getWidthRangeStart() );
	}
	//-----------------------------------------------------------------------
	void ScaleInterpolatorAffector::CmdWidthRangeStart::doSet(void* target, const String& val)
	{
		static_cast<ScaleInterpolatorAffector*>(target)->setWidthRangeStart(
			StringConverter::parseReal(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void ScaleInterpolatorAffector::CmdWidthRangeStart::doCopy(void* target, const void* source)
    {
        static_cast<ScaleInterpolatorAffector*>(target)->setWidthRangeStart(
            static_cast<const ScaleInterpolatorAffector*>(source)->getWidthRangeStart() );
    }
#endif
	//-----------------------------------------------------------------------
	String ScaleInterpolatorAffector::CmdWidthRangeEnd::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const ScaleInterpolatorAffector*>(target)->getWidthRangeEnd() );
	}
	//-----------------------------------------------------------------------
	void ScaleInterpolatorAffector::CmdWidthRangeEnd::doSet(void* target, const String& val)
	{
		static_cast<ScaleInterpolatorAffector*>(target)->setWidthRangeEnd(
			StringConverter::parseReal(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void ScaleInterpolatorAffector::CmdWidthRangeEnd::doCopy(void* target, const void* source)
    {
        static_cast<ScaleInterpolatorAffector*>(target)->setWidthRangeEnd(
            static_cast<const ScaleInterpolatorAffector*>(source)->getWidthRangeEnd() );
    }
#endif
	//-----------------------------------------------------------------------
	String ScaleInterpolatorAffector::CmdConstantScale::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const ScaleInterpolatorAffector*>(target)->getConstantScale() );
	}
	//-----------------------------------------------------------------------
	void ScaleInterpolatorAffector::CmdConstantScale::doSet(void* target, const String& val)
	{
		static_cast<ScaleInterpolatorAffector*>(target)->setConstantScale(
			StringConverter::parseVector3(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void ScaleInterpolatorAffector::CmdConstantScale::doCopy(void* target, const void* source)
    {
        static_cast<ScaleInterpolatorAffector*>(target)->setConstantScale(
            static_cast<const ScaleInterpolatorAffector*>(source)->getConstantScale() );
    }
#endif
    String ScaleInterpolatorAffector::CmdScaleAdjust::doGet(const void* target) const
    {
        return StringConverter::toString(
            static_cast<const ScaleInterpolatorAffector*>(target)->getScaleAdjust(mIndex) );
    }
    void ScaleInterpolatorAffector::CmdScaleAdjust::doSet(void* target, const String& val)
    {
        static_cast<ScaleInterpolatorAffector*>(target)->setScaleAdjust(mIndex,
            StringConverter::parseVector3(val));
    }
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void ScaleInterpolatorAffector::CmdScaleAdjust::doCopy(void* target, const void* source)
    {
        static_cast<ScaleInterpolatorAffector*>(target)->setScaleAdjust(mIndex,
            static_cast<const ScaleInterpolatorAffector*>(source)->getScaleAdjust(mIndex) );
    }
#endif
    //-----------------------------------------------------------------------
    String ScaleInterpolatorAffector::CmdTimeAdjust::doGet(const void* target) const
    {
        return StringConverter::toString(
            static_cast<const ScaleInterpolatorAffector*>(target)->getTimeAdjust(mIndex) );
    }
    void ScaleInterpolatorAffector::CmdTimeAdjust::doSet(void* target, const String& val)
    {
        static_cast<ScaleInterpolatorAffector*>(target)->setTimeAdjust(mIndex,
            StringConverter::parseReal(val));
    }
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void ScaleInterpolatorAffector::CmdTimeAdjust::doCopy(void* target, const void* source)
    {
        static_cast<ScaleInterpolatorAffector*>(target)->setTimeAdjust(mIndex,
            static_cast<const ScaleInterpolatorAffector*>(source)->getTimeAdjust(mIndex) );
    }
#endif
	//-----------------------------------------------------------------------
	String ScaleInterpolatorAffector::CmdRepeatTimes::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const ScaleInterpolatorAffector*>(target)->getRepeatTimes() );
	}
	void ScaleInterpolatorAffector::CmdRepeatTimes::doSet(void* target, const String& val)
	{
		static_cast<ScaleInterpolatorAffector*>(target)->setRepeatTimes(
			StringConverter::parseReal(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void ScaleInterpolatorAffector::CmdRepeatTimes::doCopy(void* target, const void* source)
    {
        static_cast<ScaleInterpolatorAffector*>(target)->setRepeatTimes(
            static_cast<const ScaleInterpolatorAffector*>(source)->getRepeatTimes() );
    }
#endif
	String ScaleInterpolatorAffector::CmdUseConstantScale::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const ScaleInterpolatorAffector*>(target)->getUseConstantScale() );
	}
	void ScaleInterpolatorAffector::CmdUseConstantScale::doSet(void* target, const String& val)
	{
		static_cast<ScaleInterpolatorAffector*>(target)->setUseConstantScale(
			StringConverter::parseBool(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void ScaleInterpolatorAffector::CmdUseConstantScale::doCopy(void* target, const void* source)
    {
        static_cast<ScaleInterpolatorAffector*>(target)->setUseConstantScale(
            static_cast<const ScaleInterpolatorAffector*>(source)->getUseConstantScale() );
    }
#endif
	String ScaleInterpolatorAffector::CmdUseInterpolatedScale::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const ScaleInterpolatorAffector*>(target)->getUseInterpolatedScale() );
	}
	void ScaleInterpolatorAffector::CmdUseInterpolatedScale::doSet(void* target, const String& val)
	{
		static_cast<ScaleInterpolatorAffector*>(target)->setUseInterpolatedScale(
			StringConverter::parseBool(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void ScaleInterpolatorAffector::CmdUseInterpolatedScale::doCopy(void* target, const void* source)
    {
        static_cast<ScaleInterpolatorAffector*>(target)->setUseInterpolatedScale(
            static_cast<const ScaleInterpolatorAffector*>(source)->getUseInterpolatedScale() );
    }
#endif
	String ScaleInterpolatorAffector::CmdUseUniformSize::doGet(const void* target) const
	{
		return StringConverter::toString(
			static_cast<const ScaleInterpolatorAffector*>(target)->getUseUniformSize() );
	}
	void ScaleInterpolatorAffector::CmdUseUniformSize::doSet(void* target, const String& val)
	{
		static_cast<ScaleInterpolatorAffector*>(target)->setUseUniformSize(
			StringConverter::parseBool(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void ScaleInterpolatorAffector::CmdUseUniformSize::doCopy(void* target, const void* source)
    {
        static_cast<ScaleInterpolatorAffector*>(target)->setUseUniformSize(
            static_cast<const ScaleInterpolatorAffector*>(source)->getUseUniformSize() );
    }
#endif
}