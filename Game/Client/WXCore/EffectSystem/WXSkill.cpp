/********************************************************************
filename:   FairySkill.cpp

purpose:    a skill contains effects, sounds ,ribbons or any other things, skill is defined in 
.skill files.
*********************************************************************/

#include "WXSkill.h"
#include "Core/WXSystem.h"
#include "WXAnimationEffectInfo.h"
#include "WXAnimationRibbon.h"
#include "WXAnimationSceneLightInfo.h"

// ogre header
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreStringConverter.h>

namespace WX	{

	Skill::CmdAnimationName Skill::msAnimationNameCmd;
	Skill::CmdBreakTime Skill::msBreakTimeCmd;
	Skill::CmdHitTime Skill::msHitTimeCmd;
	Skill::CmdShakeTime Skill::msShakeTimeCmd;
    Skill::CmdRepeatEffect Skill::msRepeatEffectCmd;

	//////////////////////////////////////////////////////////////////////////
	Skill::Skill(const String &skillName, System *system) : 
	mSkillName(skillName), mSystem(system), mRepeatEffect(false)
	{
		mHitTimeArray.push_back(-1.0f);
		mBreakTimeArray.push_back(-1.0f);
		mShakeTimeArray.push_back(-1.0f);

		initParamDictionary();
	}
	//---------------------------------------------------------------------
	Skill::~Skill()
	{
		removeAllAnimationEffectInfos();
		removeAllAnimationRibbons();
		removeAllAnimationSounds();
        removeAllAnimationSceneLightInfos();
	}
	//---------------------------------------------------------------------
	bool Skill::initParamDictionary(void)
	{
		if (createParamDictionary("Skill"))
		{
			Ogre::ParamDictionary* dict = getParamDictionary();			

			dict->addParameter(Ogre::ParameterDef("Animation", 
				"name of animation.",
				Ogre::PT_STRING),&msAnimationNameCmd);	  

			dict->addParameter(Ogre::ParameterDef("BreakTime", 
				"the break time of skill.",
				Ogre::PT_STRING),&msBreakTimeCmd);	  

			dict->addParameter(Ogre::ParameterDef("HitTime", 
				"the hit time of skill.",
				Ogre::PT_STRING),&msHitTimeCmd);	  

			dict->addParameter(Ogre::ParameterDef("ShakeTime", 
				"the shake time of skill.",
				Ogre::PT_STRING),&msShakeTimeCmd);	 

            dict->addParameter(Ogre::ParameterDef("RepeatEffect", 
                "whether recreate effect when the animation is loop.",
                Ogre::PT_BOOL),&msRepeatEffectCmd);	  

			return true;
		}

		return false;
	}
	//---------------------------------------------------------------------
	Skill & Skill::operator = (const Skill &rhs)
	{
		removeAllAnimationEffectInfos();

		// Copy effect infos
		unsigned int i;
		for(i = 0; i < rhs.getNumAnimationEffectInfos(); ++i)
		{
			AnimationEffectInfo *rhsAEI = rhs.getAnimationEffectInfo(i);
			AnimationEffectInfo *newAEI = addAnimationEffectInfo();

			assert ( rhsAEI && newAEI );

		//	rhsAEI->copyParametersTo(newAEI);
            rhsAEI->copyParameters(*newAEI);
		}

		removeAllAnimationRibbons();

		// copy ribbons
		for(i = 0; i < rhs.getNumAnimationRibbons(); ++i)
		{
			AnimationRibbon *rhsAEI = rhs.getAnimationRibbon(i);
			AnimationRibbon *newAEI = addAnimationRibbon();

			assert ( rhsAEI && newAEI );

		//	rhsAEI->copyParametersTo(newAEI);
            rhsAEI->copyParameters(*newAEI);
		}

		removeAllAnimationSounds();

		// copy sounds
		for(i = 0; i < rhs.getNumAnimationSounds(); ++i)
		{
			AnimationSound *rhsAEI = rhs.getAnimationSound(i);
			AnimationSound *newAEI = addAnimationSound();

			assert ( rhsAEI && newAEI );

			//rhsAEI->copyParametersTo(newAEI);
            rhsAEI->copyParameters(*newAEI);
		}

        removeAllAnimationSceneLightInfos();

        // Copy effect infos
        for(i = 0; i < rhs.getNumAnimationSceneLightInfos(); ++i)
        {
            AnimationSceneLightInfo *rhsAEI = rhs.getAnimationSceneLightInfo(i);
            AnimationSceneLightInfo *newAEI = addAnimationSceneLightInfo();

            assert ( rhsAEI && newAEI );

          //  rhsAEI->copyParametersTo(newAEI);
            rhsAEI->copyParameters(*newAEI);
        }

		// copy skill parameter
	//	rhs.copyParametersTo(this);
        rhs.copyParameters(this);

		return *this;
	}
	//---------------------------------------------------------------------
	AnimationEffectInfo* Skill::addAnimationEffectInfo(void)
	{
		AnimationEffectInfo *effectInfo = new AnimationEffectInfo();
		assert (effectInfo);

		mAnimationEffectInfos.push_back(effectInfo);

		return effectInfo;
	}
	//---------------------------------------------------------------------
	AnimationEffectInfo* Skill::getAnimationEffectInfo(unsigned short index) const
	{
		assert(index < mAnimationEffectInfos.size() && "Animation Effect Info index out of bounds!");
		return mAnimationEffectInfos[index];
	}
	//---------------------------------------------------------------------
	unsigned short Skill::getNumAnimationEffectInfos(void) const
	{
		return static_cast<unsigned short>( mAnimationEffectInfos.size() );
	}
	//---------------------------------------------------------------------
	void Skill::removeAnimationEffectInfo(unsigned short index)
	{
		assert(index < mAnimationEffectInfos.size() && "Animation Effect Info index out of bounds!");
		AnimationEffectInfos::iterator ai = mAnimationEffectInfos.begin() + index;

		if (*ai)
		{
			delete *ai;
			*ai = NULL;
		}

		mAnimationEffectInfos.erase(ai);
	}
	//---------------------------------------------------------------------
	void Skill::removeAllAnimationEffectInfos(void)
	{
		/*for (unsigned short i=0; i<getNumAnimationEffectInfos(); ++i )
		{
			removeAnimationEffectInfo(i);
		}
		mAnimationEffectInfos.clear();*/
		for ( AnimationEffectInfos::iterator i = mAnimationEffectInfos.begin(); i != mAnimationEffectInfos.end(); ++i )
		{
			if (*i)
			{
				delete *i;
				*i = NULL;
			}
		}
		mAnimationEffectInfos.clear();
	}
	//---------------------------------------------------------------------
	AnimationRibbon* Skill::addAnimationRibbon(void)
	{
		AnimationRibbon *ribbon = new AnimationRibbon(mSystem);
		assert (ribbon);

		mAnimationRibbons.push_back(ribbon);

		return ribbon;
	}
	//---------------------------------------------------------------------
	AnimationRibbon* Skill::getAnimationRibbon(unsigned short index) const
	{
		assert(index < mAnimationRibbons.size() && "Animation ribbon index out of bounds!");
		return mAnimationRibbons[index];
	}
	//---------------------------------------------------------------------
	unsigned short Skill::getNumAnimationRibbons(void) const
	{
		return static_cast<unsigned short>( mAnimationRibbons.size() );
	}
	//---------------------------------------------------------------------
	void Skill::removeAnimationRibbon(unsigned short index)
	{
		assert(index < mAnimationRibbons.size() && "Animation ribbon index out of bounds!");
		AnimationRibbons::iterator ai = mAnimationRibbons.begin() + index;

		if (*ai)
		{
			delete *ai;
			*ai = NULL;
		}

		mAnimationRibbons.erase(ai);
	}
	//---------------------------------------------------------------------
	void Skill::removeAllAnimationRibbons(void)
	{
		/*for (unsigned short i=0; i<getNumAnimationRibbons(); ++i )
		{
			removeAnimationRibbon(i);
		}*/
		for ( AnimationRibbons::iterator i = mAnimationRibbons.begin(); i != mAnimationRibbons.end(); ++i )
		{
			if (*i)
			{
				delete *i;
				*i = NULL;
			}
		}
		mAnimationRibbons.clear();
	}
	//---------------------------------------------------------------------
	AnimationSound* Skill::addAnimationSound(void)
	{
		AnimationSound *sound = new AnimationSound();
		assert (sound);

		mAnimationSounds.push_back(sound);

		return sound;
	}
	//---------------------------------------------------------------------
	AnimationSound* Skill::getAnimationSound(unsigned short index) const
	{
		assert(index < mAnimationSounds.size() && "Animation sound index out of bounds!");
		return mAnimationSounds[index];
	}
	//---------------------------------------------------------------------
	unsigned short Skill::getNumAnimationSounds(void) const
	{
		return static_cast<unsigned short>( mAnimationSounds.size() );
	}
	//---------------------------------------------------------------------
	void Skill::removeAnimationSound(unsigned short index)
	{
		assert(index < mAnimationSounds.size() && "Animation sound index out of bounds!");
		AnimationSounds::iterator ai = mAnimationSounds.begin() + index;

		if (*ai)
		{
			delete *ai;
			*ai = NULL;
		}

		mAnimationSounds.erase(ai);
	}
	//---------------------------------------------------------------------
	void Skill::removeAllAnimationSounds(void)
	{		
		for ( AnimationSounds::iterator i = mAnimationSounds.begin(); i != mAnimationSounds.end(); ++i )
		{
			if (*i)
			{
				delete *i;
				*i = NULL;
			}
		}
		mAnimationSounds.clear();
	}
    //---------------------------------------------------------------------
    AnimationSceneLightInfo* Skill::addAnimationSceneLightInfo(void)
    {
        AnimationSceneLightInfo *effectInfo = new AnimationSceneLightInfo();
        assert (effectInfo);

        mAnimationSceneLightInfos.push_back(effectInfo);

        return effectInfo;
    }
    //---------------------------------------------------------------------
    AnimationSceneLightInfo* Skill::getAnimationSceneLightInfo(unsigned short index) const
    {
        assert(index < mAnimationSceneLightInfos.size() && "Animation scene light Info index out of bounds!");
        return mAnimationSceneLightInfos[index];
    }
    //---------------------------------------------------------------------
    unsigned short Skill::getNumAnimationSceneLightInfos(void) const
    {
        return static_cast<unsigned short>( mAnimationSceneLightInfos.size() );
    }
    //---------------------------------------------------------------------
    void Skill::removeAnimationSceneLightInfo(unsigned short index)
    {
        assert(index < mAnimationSceneLightInfos.size() && "Animation scene light Info index out of bounds!");
        AnimationSceneLightInfos::iterator ai = mAnimationSceneLightInfos.begin() + index;

        if (*ai)
        {
            delete *ai;
            *ai = NULL;
        }

        mAnimationSceneLightInfos.erase(ai);
    }
    //---------------------------------------------------------------------
    void Skill::removeAllAnimationSceneLightInfos(void)
    {
        for ( AnimationSceneLightInfos::iterator i = mAnimationSceneLightInfos.begin(); i != mAnimationSceneLightInfos.end(); ++i )
        {
            if (*i)
            {
                delete *i;
                *i = NULL;
            }
        }
        mAnimationSceneLightInfos.clear();
    }
	//---------------------------------------------------------------------
	void Skill::setBreakTimeFromString(const String &breakTimeStr)
	{
		mBreakTimeArray.clear();

		Ogre::StringVector breakTimes = Ogre::StringUtil::split(breakTimeStr);

		for (size_t i=0; i<breakTimes.size(); ++i)
		{
			mBreakTimeArray.push_back( Ogre::StringConverter::parseReal(breakTimes[i]) );
		}
	}
	//---------------------------------------------------------------------
	String Skill::getBreakTimeString(void) const
	{
		String resultStr("");

		for (size_t i=0; i<mBreakTimeArray.size(); ++i)
		{
			resultStr += Ogre::StringConverter::toString(mBreakTimeArray[i]);
			resultStr += " ";
		}

		return resultStr;
	}
	//---------------------------------------------------------------------
	unsigned short Skill::getNumBreakTimes(void)
	{
		return static_cast<unsigned short>(mBreakTimeArray.size());
	}
	//---------------------------------------------------------------------
	void Skill::setHitTimeFromString(const String &hitTimeStr)
	{
		mHitTimeArray.clear();

		Ogre::StringVector hitTimes = Ogre::StringUtil::split(hitTimeStr);

		for (size_t i=0; i<hitTimes.size(); ++i)
		{
			mHitTimeArray.push_back( Ogre::StringConverter::parseReal(hitTimes[i]) );
		}
	}
	//---------------------------------------------------------------------
	String Skill::getHitTimeString(void) const
	{
		String resultStr("");

		for (size_t i=0; i<mHitTimeArray.size(); ++i)
		{
			resultStr += Ogre::StringConverter::toString(mHitTimeArray[i]);
			resultStr += " ";
		}

		return resultStr;
	}
	//---------------------------------------------------------------------
	unsigned short Skill::getNumHitTimes(void)
	{
		return static_cast<unsigned short>(mHitTimeArray.size());
	}
	//---------------------------------------------------------------------
	void Skill::setShakeTimeFromString(const String &shakeTimeStr)
	{
		mShakeTimeArray.clear();

		Ogre::StringVector shakeTimes = Ogre::StringUtil::split(shakeTimeStr);

		for (size_t i=0; i<shakeTimes.size(); ++i)
		{
			mShakeTimeArray.push_back( Ogre::StringConverter::parseReal(shakeTimes[i]) );
		}
	}
	//---------------------------------------------------------------------
	String Skill::getShakeTimeString(void) const
	{
		String resultStr("");

		for (size_t i=0; i<mShakeTimeArray.size(); ++i)
		{
			resultStr += Ogre::StringConverter::toString(mShakeTimeArray[i]);
			resultStr += " ";
		}

		return resultStr;
	}
	//---------------------------------------------------------------------
	unsigned short Skill::getNumShakeTimes(void)
	{
		return static_cast<unsigned short>(mShakeTimeArray.size());
	}
	//---------------------------------------------------------------------
	void Skill::shutdown(void)
	{
		for ( AnimationEffectInfos::iterator i = mAnimationEffectInfos.begin(); i != mAnimationEffectInfos.end(); ++i )
		{
			AnimationEffectInfo* info = *i;
			Effect* effect = info->getEffect();

			if (effect)
			{
				effect->shutdown();
			}
		}
	}
    //---------------------------------------------------------------------
    void Skill::copyParameters(Skill* newInfo) const
    {
        newInfo->mAnimName = mAnimName;
        newInfo->mBreakTimeArray = mBreakTimeArray;
        newInfo->mHitTimeArray = mHitTimeArray;
        newInfo->mShakeTimeArray = mShakeTimeArray;
        newInfo->mRepeatEffect = mRepeatEffect;
    }
	//////////////////////////////////////////////////////////////////////////
	String Skill::CmdAnimationName::doGet(const void* target) const
	{
		return static_cast<const Skill *>(target)->getAnimationName();
	}
	void Skill::CmdAnimationName::doSet(void* target, const String& val)
	{
		static_cast<Skill *>(target)->setAnimationName(val);
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void Skill::CmdAnimationName::doCopy(void* target, const void* source)
    {
        static_cast<Skill *>(target)->setAnimationName(
            static_cast<const Skill *>(source)->getAnimationName() );
    }
#endif
	//-----------------------------------------------------------------------
	String Skill::CmdBreakTime::doGet(const void* target) const
	{
		return static_cast<const Skill *>(target)->getBreakTimeString();
	}
	void Skill::CmdBreakTime::doSet(void* target, const String& val)
	{
		static_cast<Skill *>(target)->setBreakTimeFromString(val);
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void Skill::CmdBreakTime::doCopy(void* target, const void* source)
    {
        static_cast<Skill *>(target)->setBreakTimeFromString(
            static_cast<const Skill *>(source)->getBreakTimeString() );
    }
#endif
	//-----------------------------------------------------------------------
	String Skill::CmdHitTime::doGet(const void* target) const
	{
		return static_cast<const Skill *>(target)->getHitTimeString();
	}
	void Skill::CmdHitTime::doSet(void* target, const String& val)
	{
		static_cast<Skill *>(target)->setHitTimeFromString(val);
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void Skill::CmdHitTime::doCopy(void* target, const void* source)
    {
        static_cast<Skill *>(target)->setHitTimeFromString(
            static_cast<const Skill *>(source)->getHitTimeString() );
    }
#endif
	//-----------------------------------------------------------------------
	String Skill::CmdShakeTime::doGet(const void* target) const
	{
		return static_cast<const Skill *>(target)->getShakeTimeString();
	}
	void Skill::CmdShakeTime::doSet(void* target, const String& val)
	{
		static_cast<Skill *>(target)->setShakeTimeFromString(val);
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void Skill::CmdShakeTime::doCopy(void* target, const void* source)
    {
        static_cast<Skill *>(target)->setShakeTimeFromString(
            static_cast<const Skill *>(source)->getShakeTimeString() );
    }
#endif
    //-----------------------------------------------------------------------
    String Skill::CmdRepeatEffect::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString( 
            static_cast<const Skill *>(target)->getRepeatEffect() );
    }
    void Skill::CmdRepeatEffect::doSet(void* target, const String& val)
    {
        static_cast<Skill *>(target)->setRepeatEffect( Ogre::StringConverter::parseBool(val) );
    }
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void Skill::CmdRepeatEffect::doCopy(void* target, const void* source)
    {
        static_cast<Skill *>(target)->setRepeatEffect(
            static_cast<const Skill *>(source)->getRepeatEffect() );
    }
#endif
}