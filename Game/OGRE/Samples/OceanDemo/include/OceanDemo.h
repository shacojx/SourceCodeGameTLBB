/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 The OGRE Team
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
LGPL like the rest of the engine.
-----------------------------------------------------------------------------
*/

#ifndef _OceanDemo_H_
#define _OceanDemo_H_

#include <CEGUI.h>
#include "OgreCEGUIRenderer.h"

#include "OgreConfigFile.h"
#include "OgreEventQueue.h"
#include "OgreKeyEvent.h"
#include "OgreEventListeners.h"
#include "OgreStringConverter.h"
#include "OgreException.h"
#include "OgreFrameListener.h"

#include "MaterialControls.h"

//---------------------------------------------------------------------------
enum MovementType
{
	mv_CAMERA, mv_MODEL, mv_LIGHT
};

//---------------------------------------------------------------------------
class OceanDemo;

class OceanDemo_FrameListener : public Ogre::FrameListener, public Ogre::KeyListener, Ogre::MouseMotionListener, Ogre::MouseListener
{
#define MINSPEED .150f
#define MOVESPEED 30
#define MAXSPEED 1.800f


protected:
    Ogre::EventProcessor* mEventProcessor;
    Ogre::InputReader* mInputDevice;
	OceanDemo* mMain;

    Ogre::Vector3 mTranslateVector;
    bool mStatsOn;
	unsigned int mNumScreenShots;
	bool mWriteToFile;
    float mMoveScale;
    float mRotScale;
	float mSpeed;
	float mAvgFrameTime;
	int mSceneDetailIndex;
    Ogre::Real mMoveSpeed;
    Ogre::Real mRotateSpeed;
	float mSkipCount;
	float mUpdateFreq;
	CEGUI::Point mLastMousePosition;
	bool mLastMousePositionSet;
	bool mSpinModel;
	bool mSpinLight;

    // just to stop toggles flipping too fast
    Ogre::Real mTimeUntilNextToggle ;
    float mRotX, mRotY;
    Ogre::TextureFilterOptions mFiltering;
    int mAniso;
	bool mQuit;
	bool mLMBDown;
	bool mRMBDown;
	bool mProcessMovement;
	bool mUpdateMovement;
	bool mMoveFwd;
	bool mMoveBck;
	bool mMoveLeft;
	bool mMoveRight;

	CEGUI::Renderer* mGuiRenderer;
	CEGUI::Window* mGuiAvg;
	CEGUI::Window* mGuiCurr;
	CEGUI::Window* mGuiBest;
	CEGUI::Window* mGuiWorst;
	CEGUI::Window* mGuiTris;
	CEGUI::Window* mGuiDbg;
	CEGUI::Window* mRoot;

	CEGUI::MouseButton convertOgreButtonToCegui(int ogre_button_id);
	void CheckMovementKeys( CEGUI::Key::Scan keycode, bool state );
	void updateStats(void);


public:
	OceanDemo_FrameListener(OceanDemo* main);
	virtual ~OceanDemo_FrameListener();


	virtual void mouseMoved (Ogre::MouseEvent *e);
	virtual void mouseDragged (Ogre::MouseEvent *e);
	virtual void keyPressed (Ogre::KeyEvent *e);
	virtual void keyReleased (Ogre::KeyEvent *e);
	virtual void mousePressed (Ogre::MouseEvent *e);
	virtual void mouseReleased (Ogre::MouseEvent *e);

	// do-nothing events
	virtual void keyClicked (Ogre::KeyEvent *e) {}
	virtual void mouseClicked (Ogre::MouseEvent *e) {}
	virtual void mouseEntered (Ogre::MouseEvent *e) {}
	virtual void mouseExited (Ogre::MouseEvent *e) {}


	bool frameStarted(const Ogre::FrameEvent& evt);
	bool handleMouseMove(const CEGUI::EventArgs& e);
	bool handleMouseButtonUp(const CEGUI::EventArgs& e);
	bool handleMouseButtonDown(const CEGUI::EventArgs& e);
	bool handleMouseWheelEvent(const CEGUI::EventArgs& e);
	bool handleKeyDownEvent(const CEGUI::EventArgs& e);
	bool handleKeyUpEvent(const CEGUI::EventArgs& e);
	bool handelModelSpinChange(const CEGUI::EventArgs& e);
	bool handelLightSpinChange(const CEGUI::EventArgs& e);
};




//---------------------------------------------------------------------------
class OceanDemo
{
protected:
    Ogre::Root*			  mRoot;
    Ogre::Camera*		  mCamera;
    Ogre::SceneManager*	  mSceneMgr;
	// the scene node of the entity
	Ogre::SceneNode*	  mMainNode;

    OceanDemo_FrameListener* mFrameListener;
    Ogre::RenderWindow*	  mWindow;
    CEGUI::OgreCEGUIRenderer*    mGUIRenderer;
    CEGUI::System*        mGUISystem;
	Ogre::Entity*		  mCurrentEntity;
    Ogre::Entity*         mOceanSurfaceEnt;

	size_t				  mCurrentMaterial;
	Ogre::MaterialPtr	  mActiveMaterial;
	Ogre::Pass*			  mActivePass;
	Ogre::GpuProgramPtr	  mActiveFragmentProgram;
	Ogre::GpuProgramPtr	  mActiveVertexProgram;
	Ogre::GpuProgramParametersSharedPtr mActiveFragmentParameters;
	Ogre::GpuProgramParametersSharedPtr mActiveVertexParameters;

	typedef std::vector< ShaderControlGUIWidget > ShaderControlContainer;
	typedef ShaderControlContainer::iterator ShaderControlIterator;

	ShaderControlContainer    mShaderControlContainer;
    MaterialControlsContainer mMaterialControlsContainer;
	CEGUI::Scrollbar*	  mVertScroll;
	MovementType		  mMouseMovement;


    // These internal methods package up the stages in the startup process
    /** Sets up the application - returns false if the user chooses to abandon configuration. */
    bool setup(void);

	/** Configures the application - returns false if the user chooses to abandon configuration. */
    bool configure(void);
    void chooseSceneManager(void);
    void createCamera(void);
    void createViewports(void);

    /// Method which will define the source of resources (other than current folder)
    void setupResources(void);
	void loadResources(void);
	void createScene(void);
	void createFrameListener(void);

	void initComboBoxes(void);
	void initDemoEventWiring(void);
	void configureShaderControls(void);

	void doErrorBox(const char* text);

	bool handleQuit(const CEGUI::EventArgs& e);
	bool handleShaderControl(const CEGUI::EventArgs& e);
	bool handleModelComboChanged(const CEGUI::EventArgs& e);
	bool handleShaderComboChanged(const CEGUI::EventArgs& e);
	bool handleScrollControlsWindow(const CEGUI::EventArgs& e);
	bool handleMovementTypeChange(const CEGUI::EventArgs& e);

	bool handleErrorBox(const CEGUI::EventArgs& e);
	void setShaderControlVal(const float val, const size_t index);

public:
	OceanDemo() : mRoot(0), mFrameListener(0), mGUIRenderer(NULL), mGUISystem(0),
        mCurrentEntity(0), mCurrentMaterial(0), mActivePass(0), mMouseMovement(mv_CAMERA)
    {
    }

    ~OceanDemo();

    void go(void);
	Ogre::Camera* getCamera(void) const { return mCamera; }
	Ogre::SceneManager* getSceneManager(void) const { return mSceneMgr; }
	Ogre::RenderWindow* getRenderWindow(void) const { return mWindow; }
	MovementType getMouseMovement(void) const { return mMouseMovement; }
	Ogre::SceneNode* getMainNode(void) const { return mMainNode; }

};


#endif	// end _OceanDemo_H_
