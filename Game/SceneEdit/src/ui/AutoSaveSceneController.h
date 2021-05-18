/********************************************************************
filename:   AutoSaveSceneController.h

purpose:    一个每帧运行的Controller，用于在规定时间间隔内自动保存场景
*********************************************************************/

#ifndef _AUTOSAVESCENECONTROLLER_H_
#define _AUTOSAVESCENECONTROLLER_H_

#include "Core/WXPrerequisites.h"

#include <OgreController.h>

class FairyEditorFrame;

class AutoSaveSceneController
{

public:

    AutoSaveSceneController(FairyEditorFrame* frame, float interval);
    ~AutoSaveSceneController();

    void setIntervalTime(float time);
    float getIntervalTime(void)
    {
        return mIntervalTime;
    }

    void setAutoSaveEnable(bool enable);
    bool getAutoSaveEnable(void)
    {
        return mEnableAutoSave;
    }

    FairyEditorFrame* getFrame(void)
    {
        return mFrame;
    }

    void reset(void);

private:

    void _createController(FairyEditorFrame* frame, float interval);

private:

    Ogre::Controller<float>* mController;

    FairyEditorFrame* mFrame;

    float mIntervalTime;

    bool mEnableAutoSave;
};

#endif