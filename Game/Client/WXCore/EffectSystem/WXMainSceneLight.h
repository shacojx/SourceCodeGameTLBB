/********************************************************************
filename:   FairyMainSceneLight.h

purpose:    keep the info of main scene light of scene.
*********************************************************************/

#ifndef _FairyMainSceneLight_H_
#define _FairyMainSceneLight_H_

#include <OgreColourValue.h>

namespace Ogre
{
    class Light;
}

namespace WX
{
    class System;

    /// 场景灯光变化的几个阶段
    enum LightModifyPhase
    {
        LMP_IDLE,       /// 空闲，这时候可以使用那些能控制场景灯光的特效
        LMP_UPDATE,     /// 更新状态，只有在这个状态下，灯光信息才会进行改变
        LMP_FADEIN,     /// 场景灯光正出于fade in状态
        LMP_LAST,       /// 场景灯光正处于稳定状态
        LMP_FADEOUT,    /// 场景灯光正出于fade out状态
    };

    class MainSceneLight
    {
    public:
        MainSceneLight(System* system);

        /// 更新场景灯光信息，在创建新场景后调用，以得到当前场景的光源信息
        void updateLightInfo(void);

        /// 更新灯光变化
        void updateLight(float deltaTime);

        /// 增加灯光持续时间
        void addLastTime(float time)
        {
            mLastTime += time;
        }

        /// 设置灯光的持续时间
        void setLastTime(float time)
        {
            mLastTime = time;
        }

        /// 设置灯光变化的目标颜色值
        void setColourValue(const Ogre::ColourValue& colour)
        {
            mDestColour = colour;
        }

        /// 设置fade in，fade out时间
        void setFadeTime(float fadeInTime, float fadeOutTime)
        {
            mFadeInTime = fadeInTime;
            mFadeOutTime = fadeOutTime;
            mCurrentFadeInTime = 0.0f;
            mCurrentFadeOutTime = 0.0f;
            mLightModifyPhase = LMP_UPDATE;
        }

        /// 获取当前灯光变化的状态
        LightModifyPhase getLightModifyPhase(void)
        {
            return mLightModifyPhase;
        }

    protected:

        /// 保存场景中两个主光源的指针
        Ogre::Light* mMainSceneLightOne;
        Ogre::Light* mMainSceneLightTwo;

        float mLastTime;
        float mFadeInTime;
        float mCurrentFadeInTime;
        float mFadeOutTime;
        float mCurrentFadeOutTime;

        /// 当前灯光更新的总时间
        float mCurrentTime;

        Ogre::ColourValue mOriginOneLightColour;
        Ogre::ColourValue mOriginTwoLightColour;
        Ogre::ColourValue mOriginAmbientColour;
        Ogre::ColourValue mDestColour;

        LightModifyPhase mLightModifyPhase;

        System* mSystem;
    };
}

#endif