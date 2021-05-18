/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
LGPL like the rest of the engine.
-----------------------------------------------------------------------------
*/

/*
-----------------------------------------------------------------------------
Filename:    ExampleFrameListener.h
Description: Defines an example frame listener which responds to frame events.
             This frame listener just moves a specified camera around based on
             keyboard and mouse movements.
             Mouse:    Freelook
             W or Up:  Forward
             S or Down:Backward
             A:           Step left
             D:        Step right
             PgUp:     Move upwards
             PgDown:   Move downwards
             O/P:       Yaw the root scene node (and it's children)
             I/K:       Pitch the root scene node (and it's children)
             F:           Toggle frame rate stats on/off
-----------------------------------------------------------------------------
*/

#ifndef __ExampleRefAppFrameListener_H__
#define __ExampleRefAppFrameListener_H__

#include "OgreReferenceAppLayer.h"
#include "OgreKeyEvent.h"
#include "OgreEventListeners.h"
#include "OgreException.h"

using namespace Ogre;
using namespace OgreRefApp;

class ExampleRefAppFrameListener: public FrameListener, public KeyListener
{
private:
    void updateStats(void)
    {
        static String currFps = "Current FPS: ";
        static String avgFps = "Average FPS: ";
        static String bestFps = "Best FPS: ";
        static String worstFps = "Worst FPS: ";
        static String tris = "Triangle Count: ";

        // update stats when necessary
        OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
        OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
        OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
        OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");
        
        guiAvg->setCaption(avgFps + StringConverter::toString(mWindow->getAverageFPS()));
        guiCurr->setCaption(currFps + StringConverter::toString(mWindow->getLastFPS()));
        guiBest->setCaption(bestFps + StringConverter::toString(mWindow->getBestFPS())
            +" "+StringConverter::toString(mWindow->getBestFrameTime())+" ms");
        guiWorst->setCaption(worstFps + StringConverter::toString(mWindow->getWorstFPS())
            +" "+StringConverter::toString(mWindow->getWorstFrameTime())+" ms");
            
        OverlayElement* guiTris = OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
        guiTris->setCaption(tris + StringConverter::toString(mWindow->getTriangleCount()));

        OverlayElement* guiDbg = OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
        guiDbg->setCaption(mWindow->getDebugText());
    }
    
public:
    // Constructor takes a RenderWindow because it uses that to determine input context
    ExampleRefAppFrameListener(RenderWindow* win, CollideCamera* cam, bool useBufferedInputKeys = false, bool useBufferedInputMouse = false)
    {
        mUseBufferedInputKeys = useBufferedInputKeys;
		mUseBufferedInputMouse = useBufferedInputMouse;
		mInputTypeSwitchingOn = mUseBufferedInputKeys || mUseBufferedInputMouse;

		if (mInputTypeSwitchingOn)
		{
            mEventProcessor = new EventProcessor();
			mEventProcessor->initialise(win);
			mEventProcessor->startProcessingEvents();
			mEventProcessor->addKeyListener(this);
			mInputDevice = mEventProcessor->getInputReader();

		}
        else
        {
            mInputDevice = PlatformManager::getSingleton().createInputReader();
            mInputDevice->initialise(win,true, true);
        }
        mCamera = cam;
        mWindow = win;
        mStatsOn = true;
		mNumScreenShots = 0;
		mTimeUntilNextToggle = 0;

        showDebugOverlay(true);
    }
    virtual ~ExampleRefAppFrameListener()
    {
		if (mInputTypeSwitchingOn)
		{
            delete mEventProcessor;
		}
        else
        {
            PlatformManager::getSingleton().destroyInputReader( mInputDevice );
        }
    }

    bool processUnbufferedKeyInput(const FrameEvent& evt)
    {
        if (mInputDevice->isKeyDown(KC_A))
        {
            // Move camera left
            mTranslateVector.x = -mMoveScale;
        }

        if (mInputDevice->isKeyDown(KC_D))
        {
            // Move camera RIGHT
            mTranslateVector.x = mMoveScale;
        }

        /* Move camera forward by keypress. */
        if (mInputDevice->isKeyDown(KC_UP) || mInputDevice->isKeyDown(KC_W) )
        {
            mTranslateVector.z = -mMoveScale;
        }
        /* Move camera forward by mousewheel. */
        if( mInputDevice->getMouseRelativeZ() > 0 )
        {
            mTranslateVector.z = -mMoveScale * 8.0;
        }

        /* Move camera backward by keypress. */
        if (mInputDevice->isKeyDown(KC_DOWN) || mInputDevice->isKeyDown(KC_S) )
        {
            mTranslateVector.z = mMoveScale;
        }

        /* Move camera backward by mouse wheel. */
        if( mInputDevice->getMouseRelativeZ() < 0 )
        {
            mTranslateVector.z = mMoveScale * 8.0;
        }

        if (mInputDevice->isKeyDown(KC_PGUP))
        {
            // Move camera up
            mTranslateVector.y = mMoveScale;
        }

        if (mInputDevice->isKeyDown(KC_PGDOWN))
        {
            // Move camera down
            mTranslateVector.y = -mMoveScale;
        }

        if (mInputDevice->isKeyDown(KC_RIGHT))
        {
            mCamera->yaw(-mRotScale);
        }
		
        if (mInputDevice->isKeyDown(KC_LEFT))
        {
            mCamera->yaw(mRotScale);
        }

        if( mInputDevice->isKeyDown( KC_ESCAPE) )
        {            
            return false;
        }

		// see if switching is on, and you want to toggle 
        if (mInputTypeSwitchingOn && mInputDevice->isKeyDown(KC_M) && mTimeUntilNextToggle <= 0)
        {
			switchMouseMode();
            mTimeUntilNextToggle = 1;
        }

        if (mInputTypeSwitchingOn && mInputDevice->isKeyDown(KC_K) && mTimeUntilNextToggle <= 0)
        {
			// must be going from immediate keyboard to buffered keyboard
			switchKeyMode();
            mTimeUntilNextToggle = 1;
        }
        if (mInputDevice->isKeyDown(KC_F) && mTimeUntilNextToggle <= 0)
        {
            mStatsOn = !mStatsOn;
            showDebugOverlay(mStatsOn);

            mTimeUntilNextToggle = 1;
        }

        if (mInputDevice->isKeyDown(KC_SYSRQ) && mTimeUntilNextToggle <= 0)
        {
			StringUtil::StrStreamType tmp;
			tmp << "screenshot_" << ++mNumScreenShots << ".png";
            mWindow->writeContentsToFile(tmp.str());
            mTimeUntilNextToggle = 0.5;
			mWindow->setDebugText(String("Wrote ") + tmp.str());
        }



        // Return true to continue rendering
        return true;
    }

    bool processUnbufferedMouseInput(const FrameEvent& evt)
    {
        /* Rotation factors, may not be used if the second mouse button is pressed. */

        /* If the second mouse button is pressed, then the mouse movement results in 
           sliding the camera, otherwise we rotate. */
        if( mInputDevice->getMouseButton( 1 ) )
        {
            mTranslateVector.x += mInputDevice->getMouseRelativeX() * 0.13;
            mTranslateVector.y -= mInputDevice->getMouseRelativeY() * 0.13;
        }
        else
        {
            mRotX = Degree(-mInputDevice->getMouseRelativeX() * 0.13);
            mRotY = Degree(-mInputDevice->getMouseRelativeY() * 0.13);
        }


		return true;
	}

	void moveCamera()
	{

        // Make all the changes to the camera
        // Note that YAW direction is around a fixed axis (freelook style) rather than a natural YAW (e.g. airplane)
        mCamera->yaw(mRotX);
        mCamera->pitch(mRotY);
        mCamera->translate(mTranslateVector);


	}

    void showDebugOverlay(bool show)
    {   
        Overlay* o = OverlayManager::getSingleton().getByName("Core/DebugOverlay");
        if (!o)
            OGRE_EXCEPT( Exception::ERR_ITEM_NOT_FOUND, "Could not find overlay Core/DebugOverlay",
                "showDebugOverlay" );
        if (show)
        {
            o->show();
        }
        else
        {
            o->hide();
        }
    }

    // Override frameEnded event 
    bool frameEnded(const FrameEvent& evt)
    {

        if (!mInputTypeSwitchingOn)
    	{
            mInputDevice->capture();
        }


		if ( !mUseBufferedInputMouse || !mUseBufferedInputKeys)
		{
			// one of the input modes is immediate, so setup what is needed for immediate mouse/key movement
			if (mTimeUntilNextToggle >= 0) 
				mTimeUntilNextToggle -= evt.timeSinceLastFrame;

			// If this is the first frame, pick a speed
			if (evt.timeSinceLastFrame == 0)
			{
				mMoveScale = 0.5;
				mRotScale = 0.1;
			}
			// Otherwise scale movement units by time passed since last frame
			else
			{
				// Move about 50 units per second,
				mMoveScale = 50.0 * evt.timeSinceLastFrame;
				// Take about 10 seconds for full rotation
				mRotScale = Degree(36 * evt.timeSinceLastFrame);
			}
			mRotX = 0;
            mRotY = 0;
	        mTranslateVector = Vector3::ZERO;
		}

        if (mUseBufferedInputKeys)
        {
            // no need to do any processing here, it is handled by event processor and 
			// you get the results as KeyEvents
        }
        else
        {
            if (processUnbufferedKeyInput(evt) == false)
			{
				return false;
			}
        }
        if (mUseBufferedInputMouse)
        {
            // no need to do any processing here, it is handled by event processor and 
			// you get the results as MouseEvents
        }
        else
        {
            if (processUnbufferedMouseInput(evt) == false)
			{
				return false;
			}
        }

		if ( !mUseBufferedInputMouse || !mUseBufferedInputKeys)
		{
			// one of the input modes is immediate, so update the movement vector

			moveCamera();

		}

        // Perform simulation step
        World::getSingleton().simulationStep(evt.timeSinceLastFrame);

        updateStats();
		return true;
    }

	void switchMouseMode() 
	{
        mUseBufferedInputMouse = !mUseBufferedInputMouse;
		mInputDevice->setBufferedInput(mUseBufferedInputKeys, mUseBufferedInputMouse);
	}
	void switchKeyMode() 
	{
        mUseBufferedInputKeys = !mUseBufferedInputKeys;
		mInputDevice->setBufferedInput(mUseBufferedInputKeys, mUseBufferedInputMouse);
	}

	void keyClicked(KeyEvent* e) 
	{
		if (e->getKeyChar() == 'm')
		{
			switchMouseMode();
		}
		else if (e->getKeyChar() == 'k')
		{

			switchKeyMode();
		}

	}
	void keyPressed(KeyEvent* e) {}
	void keyReleased(KeyEvent* e) {}

protected:
    EventProcessor* mEventProcessor;
    InputReader* mInputDevice;
    CollideCamera* mCamera;
    Vector3 mTranslateVector;
    RenderWindow* mWindow;
    bool mStatsOn;
    bool mUseBufferedInputKeys, mUseBufferedInputMouse, mInputTypeSwitchingOn;
	unsigned int mNumScreenShots;
    float mMoveScale;
    Radian mRotScale;
    // just to stop toggles flipping too fast
    Real mTimeUntilNextToggle ;
    Radian mRotX, mRotY;

};

#endif
