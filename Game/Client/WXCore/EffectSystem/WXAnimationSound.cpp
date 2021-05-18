/********************************************************************
filename:   WXAnimationRibbon.cpp

purpose:    the ribbon that attach to the bone or a locator.
*********************************************************************/

#include "WXAnimationSound.h"

#include <OgreStringConverter.h>

namespace WX	{

	AnimationSound::CmdAttachTime	AnimationSound::msAttachTimeCmd;
	AnimationSound::CmdSoundName    AnimationSound::msSoundNameCmd;
	//////////////////////////////////////////////////////////////////////////
	AnimationSound::AnimationSound() :	
		mAttachTime(0.0f), mSoundName(""), mSoundHandle(-1), mPlayed(false)
	{
		if (createParamDictionary("AnimationSound"))
		{
			Ogre::ParamDictionary* dict = getParamDictionary();

			dict->addParameter(Ogre::ParameterDef("SoundName", 
				"the file name of sound.",
				Ogre::PT_STRING),&msSoundNameCmd);	

			dict->addParameter(Ogre::ParameterDef("AttachTime", 
				"the time that play sound.",
				Ogre::PT_REAL),&msAttachTimeCmd);			
		}
	}
    //-----------------------------------------------------------------------
    void AnimationSound::copyParameters(AnimationSound& newInfo) const
    {
        newInfo.mAttachTime = mAttachTime;
        newInfo.mSoundName = mSoundName;
    }
	//////////////////////////////////////////////////////////////////////////
	String AnimationSound::CmdAttachTime::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationSound *>(target)->getAttachTime() );
	}
	void AnimationSound::CmdAttachTime::doSet(void* target, const String& val)
	{
		static_cast<AnimationSound *>(target)->setAttachTime(Ogre::StringConverter::parseReal(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void AnimationSound::CmdAttachTime::doCopy(void* target, const void* source)
    {
        static_cast<AnimationSound *>(target)->setAttachTime(
            static_cast<const AnimationSound *>(source)->getAttachTime() );
    }
#endif

	String AnimationSound::CmdSoundName::doGet(const void* target) const
	{
		return static_cast<const AnimationSound *>(target)->getSoundName();
	}
	void AnimationSound::CmdSoundName::doSet(void* target, const String& val)
	{
		static_cast<AnimationSound *>(target)->setSoundName(val);
	}
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void AnimationSound::CmdSoundName::doCopy(void* target, const void* source)
    {
        static_cast<AnimationSound *>(target)->setSoundName(
            static_cast<const AnimationSound *>(source)->getSoundName() );
    }
#endif
}