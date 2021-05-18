/********************************************************************
filename:   WXAnimationEffectInfo.cpp

purpose:    the effect info that used by skill.
*********************************************************************/

#include "WXAnimationEffectInfo.h"
#include "WXEffectManager.h"

#include <OgreStringConverter.h>

namespace WX	{

	AnimationEffectInfo::CmdAttachTime	AnimationEffectInfo::msAttachTimeCmd;
	AnimationEffectInfo::CmdAttachPoint		AnimationEffectInfo::msAttachPointCmd;
	AnimationEffectInfo::CmdEffectTemplateName	AnimationEffectInfo::msEffectTemplateNameCmd;
	AnimationEffectInfo::CmdOffsetPos	 AnimationEffectInfo::msOffsetPosCmd;
	AnimationEffectInfo::CmdOffsetRotation	AnimationEffectInfo::msOffsetRotationCmd;
	AnimationEffectInfo::CmdAttach	AnimationEffectInfo::msAttachCmd;
	//////////////////////////////////////////////////////////////////////////
	AnimationEffectInfo::AnimationEffectInfo() :
	mAttachTime(0.0f), mAttachPoint(""), mEffectTemplateName(""), mEffect(NULL),
	mOffsetPos(Ogre::Vector3::ZERO), mOffsetRotation(Ogre::Quaternion::IDENTITY), mAttach(false)
	{
		initParamDictionary();
	}
    //---------------------------------------------------------------------
	AnimationEffectInfo::~AnimationEffectInfo()
	{
		if (mEffect)
		{
			EffectManager::getSingleton().removeEffect(mEffect,false,false);
			mEffect = NULL;
		}	
	}
    //---------------------------------------------------------------------
	bool AnimationEffectInfo::initParamDictionary(void)
	{
		if (createParamDictionary("AnimationEffectInfo"))
		{
			Ogre::ParamDictionary* dict = getParamDictionary();			

			dict->addParameter(Ogre::ParameterDef("AttachTime", 
				"the time that showing the effect.",
				Ogre::PT_REAL),&msAttachTimeCmd);	  

			dict->addParameter(Ogre::ParameterDef("AttachPoint", 
				"the attached bone or locator.",
				Ogre::PT_STRING),&msAttachPointCmd);	  

			dict->addParameter(Ogre::ParameterDef("EffectTemplateName", 
				"name of effect template.",
				Ogre::PT_STRING),&msEffectTemplateNameCmd);	  

			dict->addParameter(Ogre::ParameterDef("OffsetPos", 
				"offset position to the attach point.",
				Ogre::PT_VECTOR3),&msOffsetPosCmd);	  

			dict->addParameter(Ogre::ParameterDef("OffsetRotation", 
				"offset orientation to the attach point.",
				Ogre::PT_QUATERNION),&msOffsetRotationCmd);	  

			dict->addParameter(Ogre::ParameterDef("Attach", 
				"whether the effect will move along with the attach point.",
				Ogre::PT_BOOL),&msAttachCmd);	  

			return true;
		}

		return false;
	}
    //---------------------------------------------------------------------
    void AnimationEffectInfo::copyParameters(AnimationEffectInfo& newInfo) const
    {
        newInfo.mAttachTime = mAttachTime;
        newInfo.mAttachPoint = mAttachPoint;
        newInfo.mEffectTemplateName = mEffectTemplateName;
        newInfo.mOffsetPos = mOffsetPos;
        newInfo.mOffsetRotation = mOffsetRotation;
        newInfo.mAttach = mAttach;
    }
	//////////////////////////////////////////////////////////////////////////
	String AnimationEffectInfo::CmdAttachTime::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationEffectInfo *>(target)->getAttachTime() );
	}
	void AnimationEffectInfo::CmdAttachTime::doSet(void* target, const String& val)
	{
		static_cast<AnimationEffectInfo *>(target)->setAttachTime(Ogre::StringConverter::parseReal(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    void AnimationEffectInfo::CmdAttachTime::doCopy(void* target, const void* source)
    {
        static_cast<AnimationEffectInfo *>(target)->setAttachTime(
            static_cast<const AnimationEffectInfo *>(source)->getAttachTime() );
    }
#endif

	String AnimationEffectInfo::CmdAttachPoint::doGet(const void* target) const
	{
		return static_cast<const AnimationEffectInfo *>(target)->getAttachPoint();
	}
	void AnimationEffectInfo::CmdAttachPoint::doSet(void* target, const String& val)
	{
		static_cast<AnimationEffectInfo *>(target)->setAttachPoint(val);
	}
#if OGRE_STRING_INTERFACE_COPY
    void AnimationEffectInfo::CmdAttachPoint::doCopy(void* target, const void* source)
    {
        static_cast<AnimationEffectInfo *>(target)->setAttachPoint(
            static_cast<const AnimationEffectInfo *>(source)->getAttachPoint() );
    }
#endif

	String AnimationEffectInfo::CmdEffectTemplateName::doGet(const void* target) const
	{
		return static_cast<const AnimationEffectInfo *>(target)->getEffectTemplateName();
	}
	void AnimationEffectInfo::CmdEffectTemplateName::doSet(void* target, const String& val)
	{
		static_cast<AnimationEffectInfo *>(target)->setEffectTemplateName(val);
	}
#if OGRE_STRING_INTERFACE_COPY
    void AnimationEffectInfo::CmdEffectTemplateName::doCopy(void* target, const void* source)
    {
        static_cast<AnimationEffectInfo *>(target)->setEffectTemplateName(
            static_cast<const AnimationEffectInfo *>(source)->getEffectTemplateName() );
    }
#endif

	String AnimationEffectInfo::CmdOffsetPos::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationEffectInfo *>(target)->getOffsetPos() );
	}
	void AnimationEffectInfo::CmdOffsetPos::doSet(void* target, const String& val)
	{
		static_cast<AnimationEffectInfo *>(target)->setOffsetPos(Ogre::StringConverter::parseVector3(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    void AnimationEffectInfo::CmdOffsetPos::doCopy(void* target, const void* source)
    {
        static_cast<AnimationEffectInfo *>(target)->setOffsetPos(
            static_cast<const AnimationEffectInfo *>(source)->getOffsetPos() );
    }
#endif

	String AnimationEffectInfo::CmdOffsetRotation::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationEffectInfo *>(target)->getOffsetRotation() );
	}
	void AnimationEffectInfo::CmdOffsetRotation::doSet(void* target, const String& val)
	{
		static_cast<AnimationEffectInfo *>(target)->setOffsetRotation(Ogre::StringConverter::parseQuaternion(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    void AnimationEffectInfo::CmdOffsetRotation::doCopy(void* target, const void* source)
    {
        static_cast<AnimationEffectInfo *>(target)->setOffsetRotation(
            static_cast<const AnimationEffectInfo *>(source)->getOffsetRotation() );
    }
#endif

	String AnimationEffectInfo::CmdAttach::doGet(const void* target) const
	{
		return Ogre::StringConverter::toString(
			static_cast<const AnimationEffectInfo *>(target)->getAttach() );
	}
	void AnimationEffectInfo::CmdAttach::doSet(void* target, const String& val)
	{
		static_cast<AnimationEffectInfo *>(target)->setAttach(Ogre::StringConverter::parseBool(val));
	}
#if OGRE_STRING_INTERFACE_COPY
    void AnimationEffectInfo::CmdAttach::doCopy(void* target, const void* source)
    {
        static_cast<AnimationEffectInfo *>(target)->setAttach(
            static_cast<const AnimationEffectInfo *>(source)->getAttach() );
    }
#endif
}