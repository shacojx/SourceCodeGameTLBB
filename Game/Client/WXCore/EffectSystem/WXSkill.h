/********************************************************************
filename:   FairySkill.h

purpose:    a skill contains effects, sounds ,ribbons or any other things, skill is defined in 
			.skill files.
*********************************************************************/

#ifndef __FairySkill_H__
#define __FairySkill_H__

#include <OgreStringVector.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreStringInterface.h>

#include "Core/WXPrerequisites.h"
#include "WXEffectManager.h"
#include "WXAnimationRibbon.h"
#include "WXAnimationSound.h"

namespace WX	{
	
	class AnimationEffectInfo;
	class AnimationRibbon;
	class AnimationSound;
    class AnimationSceneLightInfo;

	class Skill : public Ogre::StringInterface
	{
		typedef std::vector<Real> TimeArray;

	public:

		class CmdAnimationName : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
		};

		class CmdHitTime : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		class CmdBreakTime : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		class CmdShakeTime : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

        class CmdRepeatEffect : public Ogre::ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };

		//////////////////////////////////////////////////////////////////////////
		Skill(const String &skillName, System *system);

		~Skill();

		const String& getAnimationName(void) const
		{
			return mAnimName;
		}

		void setAnimationName(const String &name)
		{
			mAnimName = name;
		}

		const String& getSkillName(void) const
		{
			return mSkillName;
		}

		System * getSystem(void)
		{
			return mSystem;
		}

		void setBreakTime(Real time, unsigned short index)
		{
			assert (index < mBreakTimeArray.size());
			mBreakTimeArray[index] = time;
		}

		Real getBreakTime(unsigned short index) const
		{
			assert (index < mBreakTimeArray.size());
			return mBreakTimeArray[index];
		}

		unsigned short getNumBreakTimes(void);

		void setBreakTimeFromString(const String &breakTimeStr);

		String getBreakTimeString(void) const;
		
		void setHitTime(Real time, unsigned short index)
		{
			assert (index < mHitTimeArray.size());
			mHitTimeArray[index] = time;
		}

		Real getHitTime(unsigned short index) const
		{
			assert (index < mHitTimeArray.size());
			return mHitTimeArray[index];
		}

		unsigned short getNumHitTimes(void);

		void setHitTimeFromString(const String &hitTimeStr);

		String getHitTimeString(void) const;

		void setShakeTime(Real time, unsigned short index)
		{
			assert (index < mShakeTimeArray.size());
			mShakeTimeArray[index] = time;
		}

		Real getShakeTime(unsigned short index) const
		{
			assert (index < mShakeTimeArray.size());
			return mShakeTimeArray[index];
		}

		unsigned short getNumShakeTimes(void);

		void setShakeTimeFromString(const String &shakeTimeStr);

		String getShakeTimeString(void) const;

		Skill & operator = (const Skill &rhs);

		// 进行一些skill中各种元素的清除操作
		void shutdown(void);

        void setRepeatEffect(bool repeat)
        {
            mRepeatEffect = repeat;
        }
        bool getRepeatEffect(void) const
        {
            return mRepeatEffect;
        }

        /** 拷贝所有特效参数变量
        @remarks 由于原来用Ogre::StringInterface::copyParametersTo这个函数
        来进行参数的拷贝速度太慢（因为里面有一些字符串的操作），所以
        用一个函数来复制所有的参数变量，每加一个参数，都要在这个函数
        中添加相对项
        */
        void copyParameters(Skill* newInfo) const;

		//////////////////////////////////////////////////////////////////////////		
		AnimationEffectInfo* addAnimationEffectInfo(void);
		
		AnimationEffectInfo* getAnimationEffectInfo(unsigned short index) const;

		unsigned short getNumAnimationEffectInfos(void) const;

		void removeAnimationEffectInfo(unsigned short index);

		void removeAllAnimationEffectInfos(void);

		//////////////////////////////////////////////////////////////////////////
		AnimationRibbon* addAnimationRibbon(void);

		AnimationRibbon* getAnimationRibbon(unsigned short index) const;

		unsigned short getNumAnimationRibbons(void) const;

		void removeAnimationRibbon(unsigned short index);

		void removeAllAnimationRibbons(void);

		//////////////////////////////////////////////////////////////////////////
		AnimationSound* addAnimationSound(void);

		AnimationSound* getAnimationSound(unsigned short index) const;

		unsigned short getNumAnimationSounds(void) const;

		void removeAnimationSound(unsigned short index);

		void removeAllAnimationSounds(void);

        //////////////////////////////////////////////////////////////////////////		
        AnimationSceneLightInfo* addAnimationSceneLightInfo(void);

        AnimationSceneLightInfo* getAnimationSceneLightInfo(unsigned short index) const;

        unsigned short getNumAnimationSceneLightInfos(void) const;

        void removeAnimationSceneLightInfo(unsigned short index);

        void removeAllAnimationSceneLightInfos(void);

	public:

		typedef std::vector<AnimationEffectInfo *> AnimationEffectInfos;
		typedef std::vector<AnimationRibbon *> AnimationRibbons;
		typedef std::vector<AnimationSound *> AnimationSounds;
        typedef std::vector<AnimationSceneLightInfo *> AnimationSceneLightInfos;

	protected:

		bool initParamDictionary(void);

    protected:

		String mAnimName;
		String mSkillName;

		System *mSystem;

		AnimationEffectInfos mAnimationEffectInfos;
		AnimationRibbons mAnimationRibbons;
		AnimationSounds mAnimationSounds;
        AnimationSceneLightInfos mAnimationSceneLightInfos;

		//Real mHitTime;
		TimeArray mHitTimeArray;

		//Real mBreakTime;
		TimeArray mBreakTimeArray;
	
		TimeArray mShakeTimeArray;

		bool mRepeatEffect;
		//////////////////////////////////////////////////////////////////////////
		static CmdAnimationName msAnimationNameCmd;
		static CmdBreakTime msBreakTimeCmd;
		static CmdHitTime msHitTimeCmd;
		static CmdShakeTime msShakeTimeCmd;
        static CmdRepeatEffect msRepeatEffectCmd;
	};
}

#endif