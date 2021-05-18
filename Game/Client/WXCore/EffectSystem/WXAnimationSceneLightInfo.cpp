/********************************************************************
filename:   WXAnimationSceneLightInfo.cpp

purpose:    some skills are able to change the main light of the scene.
*********************************************************************/

#include "WXAnimationSceneLightInfo.h"

#include <OgreStringConverter.h>

namespace WX
{
    AnimationSceneLightInfo::CmdAttachTime  AnimationSceneLightInfo::msAttachTimeCmd;
    AnimationSceneLightInfo::CmdLastTime    AnimationSceneLightInfo::msLastTimeCmd;
    AnimationSceneLightInfo::CmdFadeInTime  AnimationSceneLightInfo::msFadeInTimeCmd;
    AnimationSceneLightInfo::CmdFadeOutTime AnimationSceneLightInfo::msFadeOutTimeCmd;
    AnimationSceneLightInfo::CmdDestColour  AnimationSceneLightInfo::msDestColourCmd;

    //////////////////////////////////////////////////////////////////////////
    AnimationSceneLightInfo::AnimationSceneLightInfo(void) :
    mAttachTime(0.0f),
    mLastTime(0.0f),
    mFadeInTime(0.0f),
    mFadeOutTime(0.0f),
    mDestColour(Ogre::ColourValue::Black)
    {
        if (createParamDictionary("AnimationSceneLightInfo"))
        {
            Ogre::ParamDictionary* dict = getParamDictionary();

            dict->addParameter(Ogre::ParameterDef("AttachTime", 
                "the time that starting changing the light.",
                Ogre::PT_REAL),&msAttachTimeCmd);	

            dict->addParameter(Ogre::ParameterDef("LastTime", 
                "the last time that changing the light.",
                Ogre::PT_REAL),&msLastTimeCmd);			

            dict->addParameter(Ogre::ParameterDef("FadeInTime", 
                "the fade in time.",
                Ogre::PT_REAL),&msFadeInTimeCmd);

            dict->addParameter(Ogre::ParameterDef("FadeOutTime", 
                "the fade out time.",
                Ogre::PT_REAL),&msFadeOutTimeCmd);

            dict->addParameter(Ogre::ParameterDef("DestColour", 
                "the dest colour of light.",
                Ogre::PT_COLOURVALUE),&msDestColourCmd);
        }
    }
    //-----------------------------------------------------------------------
    void AnimationSceneLightInfo::copyParameters(AnimationSceneLightInfo& newInfo) const
    {
        newInfo.mAttachTime = mAttachTime;
        newInfo.mLastTime = mLastTime;
        newInfo.mFadeInTime = mFadeInTime;
        newInfo.mFadeOutTime = mFadeOutTime;
        newInfo.mDestColour = mDestColour;
    }
    //////////////////////////////////////////////////////////////////////////
    String AnimationSceneLightInfo::CmdAttachTime::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString(
            static_cast<const AnimationSceneLightInfo *>(target)->getAttachTime() );
    }
    void AnimationSceneLightInfo::CmdAttachTime::doSet(void* target, const String& val)
    {
        static_cast<AnimationSceneLightInfo *>(target)->setAttachTime(Ogre::StringConverter::parseReal(val));
    }
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void AnimationSceneLightInfo::CmdAttachTime::doCopy(void* target, const void* source)
    {
        static_cast<AnimationSceneLightInfo *>(target)->setAttachTime(
            static_cast<const AnimationSceneLightInfo *>(source)->getAttachTime() );
    }
#endif

    String AnimationSceneLightInfo::CmdLastTime::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString(
            static_cast<const AnimationSceneLightInfo *>(target)->getLastTime() );
    }
    void AnimationSceneLightInfo::CmdLastTime::doSet(void* target, const String& val)
    {
        static_cast<AnimationSceneLightInfo *>(target)->setLastTime(Ogre::StringConverter::parseReal(val));
    }
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void AnimationSceneLightInfo::CmdLastTime::doCopy(void* target, const void* source)
    {
        static_cast<AnimationSceneLightInfo *>(target)->setLastTime(
            static_cast<const AnimationSceneLightInfo *>(source)->getLastTime() );
    }
#endif

    String AnimationSceneLightInfo::CmdFadeInTime::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString(
            static_cast<const AnimationSceneLightInfo *>(target)->getFadeInTime() );
    }
    void AnimationSceneLightInfo::CmdFadeInTime::doSet(void* target, const String& val)
    {
        static_cast<AnimationSceneLightInfo *>(target)->setFadeInTime(Ogre::StringConverter::parseReal(val));
    }
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void AnimationSceneLightInfo::CmdFadeInTime::doCopy(void* target, const void* source)
    {
        static_cast<AnimationSceneLightInfo *>(target)->setFadeInTime(
            static_cast<const AnimationSceneLightInfo *>(source)->getFadeInTime() );
    }
#endif

    String AnimationSceneLightInfo::CmdFadeOutTime::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString(
            static_cast<const AnimationSceneLightInfo *>(target)->getFadeOutTime() );
    }
    void AnimationSceneLightInfo::CmdFadeOutTime::doSet(void* target, const String& val)
    {
        static_cast<AnimationSceneLightInfo *>(target)->setFadeOutTime(Ogre::StringConverter::parseReal(val));
    }
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void AnimationSceneLightInfo::CmdFadeOutTime::doCopy(void* target, const void* source)
    {
        static_cast<AnimationSceneLightInfo *>(target)->setFadeOutTime(
            static_cast<const AnimationSceneLightInfo *>(source)->getFadeOutTime() );
    }
#endif

    String AnimationSceneLightInfo::CmdDestColour::doGet(const void* target) const
    {
        return Ogre::StringConverter::toString(
            static_cast<const AnimationSceneLightInfo *>(target)->getDestColour() );
    }
    void AnimationSceneLightInfo::CmdDestColour::doSet(void* target, const String& val)
    {
        static_cast<AnimationSceneLightInfo *>(target)->setDestColour(Ogre::StringConverter::parseColourValue(val));
    }
#if OGRE_STRING_INTERFACE_COPY
    //-----------------------------------------------------------------------
    void AnimationSceneLightInfo::CmdDestColour::doCopy(void* target, const void* source)
    {
        static_cast<AnimationSceneLightInfo *>(target)->setDestColour(
            static_cast<const AnimationSceneLightInfo *>(source)->getDestColour() );
    }
#endif
}