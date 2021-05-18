/********************************************************************
filename:   WXAnimationSceneLightInfo.h

purpose:    some skills are able to change the main light of the scene.
*********************************************************************/

#ifndef _FAIRYANIMATIONSCENELIGHTINFOINFO_H_
#define _FAIRYANIMATIONSCENELIGHTINFOINFO_H_

#include "Core/WXPrerequisites.h"

#include <OgreStringInterface.h>
#include <OgreColourValue.h>

namespace WX
{
    class AnimationSceneLightInfo : public Ogre::StringInterface
    {
    public:
        class CmdAttachTime : public Ogre::ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif
        };

        class CmdLastTime : public Ogre::ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };

        class CmdFadeInTime : public Ogre::ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };

        class CmdFadeOutTime : public Ogre::ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };

        class CmdDestColour : public Ogre::ParamCommand
        {
        public:
            String doGet(const void* target) const;
            void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif      
        };

        AnimationSceneLightInfo(void);

        void setAttachTime(float time)
        {
            mAttachTime = time;
        }
        float getAttachTime(void) const
        {
            return mAttachTime;
        }

        void setLastTime(float time)
        {
            mLastTime = time;
        }
        float getLastTime(void) const
        {
            return mLastTime;
        }

        void setFadeInTime(float time)
        {
            mFadeInTime = time;
        }
        float getFadeInTime(void) const
        {
            return mFadeInTime;
        }

        void setFadeOutTime(float time)
        {
            mFadeOutTime = time;
        }
        float getFadeOutTime(void) const
        {
            return mFadeOutTime;
        }

        void setDestColour(const Ogre::ColourValue& colour)
        {
            mDestColour = colour;
        }
        const Ogre::ColourValue& getDestColour(void) const
        {
            return mDestColour;
        }

        /** 拷贝所有特效参数变量
        @remarks 由于原来用Ogre::StringInterface::copyParametersTo这个函数
        来进行参数的拷贝速度太慢（因为里面有一些字符串的操作），所以
        用一个函数来复制所有的参数变量，每加一个参数，都要在这个函数
        中添加相对项
        */
        void copyParameters(AnimationSceneLightInfo& newInfo) const;

    protected:

        float mAttachTime;
        float mLastTime;
        float mFadeInTime;
        float mFadeOutTime;

        Ogre::ColourValue mDestColour;

        //////////////////////////////////////////////////////////////////////////
        static CmdAttachTime msAttachTimeCmd;
        static CmdLastTime msLastTimeCmd;
        static CmdFadeInTime msFadeInTimeCmd;
        static CmdFadeOutTime msFadeOutTimeCmd;
        static CmdDestColour msDestColourCmd;
    };
}

#endif