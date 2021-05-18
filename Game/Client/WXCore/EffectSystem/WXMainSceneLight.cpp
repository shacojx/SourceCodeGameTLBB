/********************************************************************
filename:   WXMainSceneLight.cpp

purpose:    keep the info of main scene light of scene.
*********************************************************************/

#include "WXMainSceneLight.h"

#include "Core/WXSystem.h"
#include "Core/WXSceneInfo.h"
#include "Core/WXLightObject.h"

#include <OgreLight.h>
#include <OgreSceneManager.h>
#include <OgreStringConverter.h>
#include <OgreLogManager.h>

namespace WX
{
    //---------------------------------------------------------------------
    MainSceneLight::MainSceneLight(System* system) :
    mSystem(system),
    mMainSceneLightOne(NULL),
    mMainSceneLightTwo(NULL),
    mLastTime(0.0f),
    mFadeInTime(0.0f),
    mCurrentFadeInTime(0.0f),
    mFadeOutTime(0.0f),
    mCurrentFadeOutTime(0.0f),
    mCurrentTime(0.0f),
    mOriginOneLightColour(Ogre::ColourValue::Black),
    mOriginTwoLightColour(Ogre::ColourValue::Black),
    mOriginAmbientColour(Ogre::ColourValue::Black),
    mDestColour(Ogre::ColourValue::Black),
    mLightModifyPhase(LMP_IDLE)
    {
    }
    //---------------------------------------------------------------------
    void MainSceneLight::updateLight(float deltaTime)
    {
        if (mLightModifyPhase != LMP_IDLE)
        {
            if (mMainSceneLightTwo && mMainSceneLightOne)
            {
                mCurrentTime += deltaTime;

                // ÔÚfade in½×¶Î
                if (mCurrentTime < mFadeInTime)
                {     
                    mLightModifyPhase = LMP_FADEIN;

                    mCurrentFadeInTime += deltaTime;

                    if (mCurrentFadeInTime >= mFadeInTime)
                    {
                        mMainSceneLightOne->setDiffuseColour(mDestColour);
                        mMainSceneLightTwo->setDiffuseColour(mDestColour);
                        mSystem->getSceneManager()->setAmbientLight(mDestColour);
                    }
                    else
                    {
                        float per = mCurrentFadeInTime / mFadeInTime;

                        // light one
                        Ogre::ColourValue nowColour = (mDestColour - mOriginOneLightColour) * per +
                            mOriginOneLightColour;

                        mMainSceneLightOne->setDiffuseColour(nowColour);

                        // light two
                        nowColour = (mDestColour - mOriginTwoLightColour) * per +
                            mOriginTwoLightColour;

                        mMainSceneLightTwo->setDiffuseColour(nowColour);

                        // ambient
                        nowColour = (mDestColour - mOriginAmbientColour) * per +
                            mOriginAmbientColour;

                        mSystem->getSceneManager()->setAmbientLight(nowColour);
                    }
                }
                // ³ÖÐø½×¶Î
                else if (mCurrentTime >= mFadeInTime && mCurrentTime < mFadeInTime + mLastTime)
                {
                    mLightModifyPhase = LMP_LAST;
                }
                // fade out
                else if (mCurrentTime >= mFadeInTime + mLastTime 
                    && mCurrentTime < mFadeInTime + mLastTime + mFadeOutTime)
                {
                    mLightModifyPhase = LMP_FADEOUT;

                    mCurrentFadeOutTime += deltaTime;

                    if (mCurrentFadeOutTime >= mFadeOutTime)
                    {
                        mMainSceneLightOne->setDiffuseColour(mOriginOneLightColour);
                        mMainSceneLightTwo->setDiffuseColour(mOriginTwoLightColour);
                        mSystem->getSceneManager()->setAmbientLight(mOriginAmbientColour);
                    }
                    else
                    {
                        float per = mCurrentFadeOutTime / mFadeOutTime;

                        // light one
                        Ogre::ColourValue nowColour = (mOriginOneLightColour - mDestColour) * per +
                            mDestColour;

                        mMainSceneLightOne->setDiffuseColour(nowColour);

                        // light two
                        nowColour = (mOriginTwoLightColour - mDestColour) * per +
                            mDestColour;

                        mMainSceneLightTwo->setDiffuseColour(nowColour);

                        // ambient
                        nowColour = (mOriginAmbientColour - mDestColour) * per +
                            mDestColour;

                        mSystem->getSceneManager()->setAmbientLight(nowColour);
                    }                    
                }
                else if (mCurrentTime >= mFadeInTime + mLastTime + mFadeOutTime)
                {
                    mLightModifyPhase = LMP_IDLE;
                    mCurrentTime = 0.0f;
                    mCurrentFadeInTime = 0.0f;
                    mCurrentFadeOutTime = 0.0f;
                    mLastTime = 0.0f;
                }
            }
        }
    }
    //---------------------------------------------------------------------
    void MainSceneLight::updateLightInfo(void)
    {
        SceneInfo* sceneInfo = mSystem->getSceneInfo();

        const ObjectPtr lightOneObject = sceneInfo->findObjectByName("#FairyMainLightOne");

        if (lightOneObject)
        {
            LightObject* lightObject = static_cast<LightObject*> (lightOneObject.get());

            mMainSceneLightOne = lightObject->getLight();

            if (mMainSceneLightOne)
                mOriginOneLightColour = mMainSceneLightOne->getDiffuseColour();
            else
            {
                Ogre::LogManager::getSingleton().logMessage("the light one hasn't been created!  "
                    "Maybe it is a static light! "
                    "MainSceneLight::updateLightInfo");
            }
        }
        else
        {
            Ogre::LogManager::getSingleton().logMessage("can't find the light with the name '#FairyMainLightOne' "
                "MainSceneLight::updateLightInfo");

            mMainSceneLightOne = NULL;
        }

        const ObjectPtr lightTwoObject = sceneInfo->findObjectByName("#FairyMainLightTwo");

        if (lightTwoObject)
        {
            LightObject* lightObject = static_cast<LightObject*> (lightTwoObject.get());

            mMainSceneLightTwo = lightObject->getLight();

            if (mMainSceneLightTwo)
                mOriginTwoLightColour = mMainSceneLightTwo->getDiffuseColour();
            else
            {
                Ogre::LogManager::getSingleton().logMessage("the light two hasn't been created!  "
                    "Maybe it is a static light! "
                    "MainSceneLight::updateLightInfo");
            }
        }
        else
        {
            Ogre::LogManager::getSingleton().logMessage("can't find the light with the name '#FairyMainLightTwo' "
                "MainSceneLight::updateLightInfo");

            mMainSceneLightTwo = NULL;
        }

        mOriginAmbientColour = mSystem->getSceneManager()->getAmbientLight();
    }
}