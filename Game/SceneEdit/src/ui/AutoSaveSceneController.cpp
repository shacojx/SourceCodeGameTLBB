/********************************************************************
filename:   AutoSaveSceneController.cpp

purpose:    一个每帧运行的Controller，用于在规定时间间隔内自动保存场景
*********************************************************************/

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "AutoSaveSceneController.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "AutoSaveSceneController.h"
#include "WXEditorFrame.h"

#include <OgreControllerManager.h>
#include <OgrePredefinedControllers.h>

class AutoSaveSceneControllerValue : public Ogre::ControllerValue<float>
{
public:

    AutoSaveSceneControllerValue(AutoSaveSceneController* controller) :
    mController(controller), mNowTime(0.0f), mNeedReset(false)
    {
    }

    float getValue(void) const
    {
        return 0;
    }

    void setValue(float value)
    {
        if (mController->getAutoSaveEnable())
        {
            if (mNeedReset)
            {
                mNowTime = 0.0f;
                mNeedReset = false;
            }
        
            mNowTime += value;
            if ( mNowTime > mController->getIntervalTime() )
            {       
                // 时间到了，保存场景，并且mNowTime置0
                mController->getFrame()->SaveImpl(true);
                mNowTime = 0;
            }
        }        
    }

    void reset(void)
    {
        mNeedReset = true;
    }

private:

    AutoSaveSceneController* mController;
    float mNowTime;
    bool mNeedReset;
};

AutoSaveSceneController::AutoSaveSceneController(FairyEditorFrame* frame, float interval) :
mFrame(frame),
mController(NULL),
mIntervalTime(interval),
mEnableAutoSave(true)
{
    _createController(frame, mIntervalTime);
}

AutoSaveSceneController::~AutoSaveSceneController()
{
    if (mController)
    {
        Ogre::ControllerManager::getSingleton().destroyController(mController);
    }
}

void AutoSaveSceneController::_createController(FairyEditorFrame* frame, float interval)
{
    if (mController)
    {
        Ogre::ControllerManager::getSingleton().destroyController(mController);
    }

    Ogre::ControllerManager& controllerManager = Ogre::ControllerManager::getSingleton();
    mController = controllerManager.createFrameTimePassthroughController(
        Ogre::ControllerValueRealPtr(new AutoSaveSceneControllerValue(this)));
}

void AutoSaveSceneController::setIntervalTime(float time)
{
    mIntervalTime = time;
    reset();
}

void AutoSaveSceneController::setAutoSaveEnable(bool enable)
{
    mEnableAutoSave = enable;
    reset();
}

void AutoSaveSceneController::reset(void)
{
    static_cast<AutoSaveSceneControllerValue* >(mController->getDestination().get())->reset();
}



