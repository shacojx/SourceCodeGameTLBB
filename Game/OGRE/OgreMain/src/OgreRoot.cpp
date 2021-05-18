/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://ogre.sourceforge.net/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
*/
// Ogre includes
#include "OgreStableHeaders.h"

#include "OgreRoot.h"

#include "OgreRenderSystem.h"
#include "OgreRenderWindow.h"
#include "OgreException.h"
#include "OgreControllerManager.h"
#include "OgreLogManager.h"
#include "OgreMath.h"
#include "OgreDynLibManager.h"
#include "OgreDynLib.h"
#include "OgreConfigFile.h"
#include "OgreMaterialManager.h"
#include "OgreMeshManager.h"
#include "OgreTextureManager.h"
#include "OgreParticleSystemManager.h"
#include "OgreSkeletonManager.h"
#include "OgreOverlayElementFactory.h"
#include "OgreOverlayManager.h"
#include "OgreProfiler.h"
#include "OgreErrorDialog.h"
#include "OgreConfigDialog.h"
#include "OgreStringConverter.h"
#include "OgrePlatformManager.h"
#include "OgreArchiveManager.h"
#include "OgreZip.h"
#include "OgreFileSystem.h"
#include "OgreShadowVolumeExtrudeProgram.h"
#include "OgreResourceBackgroundQueue.h"
#include "OgreEntity.h"
#include "OgreBillboardSet.h"
#include "OgreBillboardChain.h"
#include "OgreRibbonTrail.h"
#include "OgreLight.h"
#include "OgreManualObject.h"
#include "OgreRenderQueueInvocation.h"
#include "OgrePlatformInformation.h"
	
#if OGRE_NO_DEVIL == 0
#include "OgreILCodecs.h"
#endif

#include "OgreFontManager.h"
#include "OgreHardwareBufferManager.h"

#include "OgreOverlay.h"
#include "OgreHighLevelGpuProgramManager.h"

#include "OgreExternalTextureSourceManager.h"
#include "OgreCompositorManager.h"

#include "OgreParticleFX.h"
#include "OgreParticleFX2.h"
#include "OgreOctreeSceneManagerDll.h"
#include "OgreCgProgramManagerDll.h"
#include "Scorpio.h"
#include "OgreD3D9EngineDll.h"

namespace Ogre {
    //-----------------------------------------------------------------------
    template<> Root* Singleton<Root>::ms_Singleton = 0;
    Root* Root::getSingletonPtr(void)
    {
        return ms_Singleton;
    }
    Root& Root::getSingleton(void)
    {
        assert( ms_Singleton );  return ( *ms_Singleton );
    }

    typedef void (*DLL_START_PLUGIN)(void);
	typedef void (*DLL_INIT_PLUGIN)(void);
    typedef void (*DLL_STOP_PLUGIN)(void);


    //-----------------------------------------------------------------------
    // Termination handler
    extern "C" _OgreExport void handleTerminate(void)
    {
        LogManager::getSingleton().logMessage("Termination handler: uncaught exception!", LML_CRITICAL);

        Root::getSingleton().shutdown();

        ErrorDialog* dlg = PlatformManager::getSingleton().createErrorDialog();

        Exception* e = Exception::getLastException();

        if (e)
            dlg->display(e->getFullDescription());
        else
            dlg->display("Unknown");

        // Abort
        exit(-1);

    }

    void Root::termHandler()
    {
        handleTerminate();
    }


    //-----------------------------------------------------------------------
    Root::Root(const String& pluginFileName, const String& configFileName, const String& logFileName)
      : mLogManager(0), mCurrentFrame(0), mFrameSmoothingTime(0.0f),
	  mNextMovableObjectTypeFlag(1), mIsInitialised(false)
    {
        // First create new exception handler
        SET_TERM_HANDLER;

        // superclass will do singleton checking
        String msg;

        // Init
        mActiveRenderer = 0;
        mVersion = StringConverter::toString(OGRE_VERSION_MAJOR) + "." +
            StringConverter::toString(OGRE_VERSION_MINOR) + "." +
            StringConverter::toString(OGRE_VERSION_PATCH) + " " +
            "(" + OGRE_VERSION_NAME + ")";
		mConfigFileName = configFileName;

		// Create log manager and default log file if there is no log manager yet
		if(LogManager::getSingletonPtr() == 0)
		{
			mLogManager = new LogManager();
			mLogManager->createLog(logFileName, true, true);
		}

        // Dynamic library manager
        mDynLibManager = new DynLibManager();

        mArchiveManager = new ArchiveManager();

		// ResourceGroupManager
		mResourceGroupManager = new ResourceGroupManager();

		// ResourceBackgroundQueue
		mResourceBackgroundQueue = new ResourceBackgroundQueue();

		// Create SceneManager enumerator (note - will be managed by singleton)
        mSceneManagerEnum = new SceneManagerEnumerator();
        mCurrentSceneManager = NULL;

        // ..material manager
        mMaterialManager = new MaterialManager();

        // Mesh manager
        mMeshManager = new MeshManager();

        // Skeleton manager
        mSkeletonManager = new SkeletonManager();

        // ..particle system manager
        mParticleManager = new ParticleSystemManager();

        // Platform manager
        mPlatformManager = new PlatformManager();

        // Timer
        mTimer = mPlatformManager->createTimer();

        // Overlay manager
        mOverlayManager = new OverlayManager();

        mPanelFactory = new PanelOverlayElementFactory();
        mOverlayManager->addOverlayElementFactory(mPanelFactory);

        mBorderPanelFactory = new BorderPanelOverlayElementFactory();
        mOverlayManager->addOverlayElementFactory(mBorderPanelFactory);

        mTextAreaFactory = new TextAreaOverlayElementFactory();
        mOverlayManager->addOverlayElementFactory(mTextAreaFactory);
        // Font manager
        mFontManager = new FontManager();

#if OGRE_PROFILING
        // Profiler
        mProfiler = new Profiler();
		Profiler::getSingleton().setTimer(mTimer);
#endif
        mFileSystemArchiveFactory = new FileSystemArchiveFactory();
        ArchiveManager::getSingleton().addArchiveFactory( mFileSystemArchiveFactory );
        mZipArchiveFactory = new ZipArchiveFactory();
        ArchiveManager::getSingleton().addArchiveFactory( mZipArchiveFactory );
#if OGRE_NO_DEVIL == 0
	    // Register image codecs
	    ILCodecs::registerCodecs();
#endif

        mHighLevelGpuProgramManager = new HighLevelGpuProgramManager();

		mExternalTextureSourceManager = new ExternalTextureSourceManager();
        mCompositorManager = new CompositorManager();

        // Auto window
        mAutoWindow = 0;

		// instantiate and register base movable factories
		mEntityFactory = new EntityFactory();
		addMovableObjectFactory(mEntityFactory);
		mLightFactory = new LightFactory();
		addMovableObjectFactory(mLightFactory);
		mBillboardSetFactory = new BillboardSetFactory();
		addMovableObjectFactory(mBillboardSetFactory);
		mManualObjectFactory = new ManualObjectFactory();
		addMovableObjectFactory(mManualObjectFactory);
		mBillboardChainFactory = new BillboardChainFactory();
		addMovableObjectFactory(mBillboardChainFactory);
		mRibbonTrailFactory = new RibbonTrailFactory();
		addMovableObjectFactory(mRibbonTrailFactory);

		// Load plugins
        if (!pluginFileName.empty())
            loadPlugins(pluginFileName);

		//LogManager::getSingleton().logMessage("*-*-* OGRE Initialising");
  //      msg = "*-*-* Version " + mVersion;
  //      LogManager::getSingleton().logMessage(msg);

        // Can't create managers until initialised
        mControllerManager = 0;

        mFirstTimePostWindowInit = false;

		mInnerPlugined = false;
		mRenderPlugined_D3D9 = false;
		mRenderPlugined_GL = false;
	}

    //-----------------------------------------------------------------------
    Root::~Root()
    {
        shutdown();
        delete mSceneManagerEnum;

		destroyAllRenderQueueInvocationSequences();
        delete mCompositorManager;
		delete mExternalTextureSourceManager;
#if OGRE_NO_DEVIL == 0
        ILCodecs::deleteCodecs();
#endif
#if OGRE_PROFILING
        delete mProfiler;
#endif
        delete mOverlayManager;
        delete mFontManager;
        delete mArchiveManager;
        delete mZipArchiveFactory;
        delete mFileSystemArchiveFactory;
        delete mSkeletonManager;
        delete mMeshManager;
        delete mParticleManager;

        if( mControllerManager )
            delete mControllerManager;
        if (mHighLevelGpuProgramManager)
            delete mHighLevelGpuProgramManager;

        delete mTextAreaFactory;
        delete mBorderPanelFactory;
        delete mPanelFactory;

        unloadPlugins();
		unloadRenderPlugins();
		unloadInnerPlugins();
        delete mMaterialManager;
        Pass::processPendingPassUpdates(); // make sure passes are cleaned
		delete mResourceBackgroundQueue;
        delete mResourceGroupManager;

		delete mEntityFactory;
		delete mLightFactory;
		delete mBillboardSetFactory;
		delete mManualObjectFactory;
		delete mBillboardChainFactory;
		delete mRibbonTrailFactory;


        mPlatformManager->destroyTimer(mTimer);
        delete mPlatformManager;
        delete mDynLibManager;
        delete mLogManager;

		mAutoWindow = 0;
		mFirstTimePostWindowInit = false;


        StringInterface::cleanupDictionary ();
    }

    //-----------------------------------------------------------------------
    void Root::saveConfig(void)
    {
		std::ofstream of(mConfigFileName.c_str());

        if (!of)
            OGRE_EXCEPT(Exception::ERR_CANNOT_WRITE_TO_FILE, "Cannot create settings file.",
            "Root::saveConfig");

        if (mActiveRenderer)
        {
            of << "Render System=" << mActiveRenderer->getName() << std::endl;
        }
        else
        {
            of << "Render System=" << std::endl;
        }

        for (RenderSystemList::const_iterator pRend = getAvailableRenderers()->begin(); pRend != getAvailableRenderers()->end(); ++pRend)
        {
            RenderSystem* rs = *pRend;
            of << std::endl;
            of << "[" << rs->getName() << "]" << std::endl;
            const ConfigOptionMap& opts = rs->getConfigOptions();
            for (ConfigOptionMap::const_iterator pOpt = opts.begin(); pOpt != opts.end(); ++pOpt)
            {
				of << pOpt->first << "=" << pOpt->second.currentValue << std::endl;
            }
        }

        of.close();

    }
    //-----------------------------------------------------------------------
    bool Root::restoreConfig(void)
    {
        // Restores configuration from saved state
        // Returns true if a valid saved configuration is
        //   available, and false if no saved config is
        //   stored, or if there has been a problem
        ConfigFile cfg;
        //RenderSystemList::iterator pRend;

        try {
            // Don't trim whitespace
            cfg.load(mConfigFileName, "\t:=", false);
        }
        catch (Exception& e)
        {
            if (e.getNumber() == Exception::ERR_FILE_NOT_FOUND)
            {
                return false;
            }
            else
            {
                throw;
            }
        }

        ConfigFile::SectionIterator iSection = cfg.getSectionIterator();
        while (iSection.hasMoreElements())
        {
            const String& renderSystem = iSection.peekNextKey();
            const ConfigFile::SettingsMultiMap& settings = *iSection.getNext();

            RenderSystem* rs = getRenderSystemByName(renderSystem);
            if (!rs)
            {
                // Unrecognised render system
                continue;
            }

            ConfigFile::SettingsMultiMap::const_iterator i;
            for (i = settings.begin(); i != settings.end(); ++i)
            {
                rs->setConfigOption(i->first, i->second);
            }
        }

        RenderSystem* rs = getRenderSystemByName(cfg.getSetting("Render System"));
        if (!rs)
        {
            // Unrecognised render system
            return false;
        }

        setRenderSystem(rs);

        // Successful load
        return true;

    }

    //-----------------------------------------------------------------------
    bool Root::showConfigDialog(void)
    {
        // Displays the standard config dialog
        // Will use stored defaults if available
        ConfigDialog* dlg;
        bool isOk;

        dlg = mPlatformManager->createConfigDialog();

        isOk = dlg->display();

        mPlatformManager->destroyConfigDialog(dlg);

        return isOk;

    }

    //-----------------------------------------------------------------------
    RenderSystemList* Root::getAvailableRenderers(void)
    {
        // Returns a vector of renders

        return &mRenderers;

    }

    //-----------------------------------------------------------------------
    RenderSystem* Root::getRenderSystemByName(const String& name)
    {
        if (name.empty())
        {
            // No render system
            return NULL;
        }

        RenderSystemList::const_iterator pRend;
        for (pRend = getAvailableRenderers()->begin(); pRend != getAvailableRenderers()->end(); ++pRend)
        {
            RenderSystem* rs = *pRend;
            if (rs->getName() == name)
                return rs;
        }

        // Unrecognised render system
        return NULL;
    }

    //-----------------------------------------------------------------------
    void Root::setRenderSystem(RenderSystem* system)
    {
        // Sets the active rendering system
        // Can be called direct or will be called by
        //   standard config dialog

        // Is there already an active renderer?
        // If so, disable it and init the new one
        if( mActiveRenderer && mActiveRenderer != system )
        {
            mActiveRenderer->shutdown();
        }

        mActiveRenderer = system;
        // Tell scene managers
        SceneManagerEnumerator::getSingleton().setRenderSystem(system);

    }
    //-----------------------------------------------------------------------
    void Root::addRenderSystem(RenderSystem *newRend)
    {
        mRenderers.push_back(newRend);
    }
    //-----------------------------------------------------------------------
	void Root::_setCurrentSceneManager(SceneManager* sm)
	{
		mCurrentSceneManager = sm;
	}
    //-----------------------------------------------------------------------
    RenderSystem* Root::getRenderSystem(void)
    {
        // Gets the currently active renderer
        return mActiveRenderer;

    }

    //-----------------------------------------------------------------------
    RenderWindow* Root::initialise(bool autoCreateWindow, const String& windowTitle)
    {
        if (!mActiveRenderer)
            OGRE_EXCEPT(Exception::ERR_NO_RENDERSYSTEM_SELECTED,
            "Cannot initialise - no render "
            "system has been selected.", "Root::initialise");

        if (!mControllerManager)
			mControllerManager = new ControllerManager();

		PlatformInformation::log(LogManager::getSingleton().getDefaultLog());
		mAutoWindow =  mActiveRenderer->initialise(autoCreateWindow, windowTitle);

		mResourceBackgroundQueue->initialise();

        if (autoCreateWindow && !mFirstTimePostWindowInit)
        {
            oneTimePostWindowInit();
            mAutoWindow->_setPrimary();
        }

        // Initialise timer
        mTimer->reset();

		// Init plugins
		initialisePlugins();

		mIsInitialised = true;

        return mAutoWindow;

    }
    //-----------------------------------------------------------------------
    String Root::getErrorDescription(long errorNumber)
    {

        // Pass to render system
        if (mActiveRenderer)
            return mActiveRenderer->getErrorDescription(errorNumber);
        else
            return StringUtil::BLANK;

    }
	//-----------------------------------------------------------------------
	void Root::addSceneManagerFactory(SceneManagerFactory* fact)
	{
		mSceneManagerEnum->addFactory(fact);
	}
	//-----------------------------------------------------------------------
	void Root::removeSceneManagerFactory(SceneManagerFactory* fact)
	{
		mSceneManagerEnum->removeFactory(fact);
	}
	//-----------------------------------------------------------------------
	const SceneManagerMetaData* Root::getSceneManagerMetaData(const String& typeName) const
	{
		return mSceneManagerEnum->getMetaData(typeName);
	}
	//-----------------------------------------------------------------------
	SceneManagerEnumerator::MetaDataIterator 
	Root::getSceneManagerMetaDataIterator(void) const
	{
		return mSceneManagerEnum->getMetaDataIterator();

	}
	//-----------------------------------------------------------------------
	SceneManager* Root::createSceneManager(const String& typeName, 
		const String& instanceName)
	{
		return mSceneManagerEnum->createSceneManager(typeName, instanceName);
	}
	//-----------------------------------------------------------------------
	SceneManager* Root::createSceneManager(SceneTypeMask typeMask, 
		const String& instanceName)
	{
		return mSceneManagerEnum->createSceneManager(typeMask, instanceName);
	}
	//-----------------------------------------------------------------------
	void Root::destroySceneManager(SceneManager* sm)
	{
		mSceneManagerEnum->destroySceneManager(sm);
	}
	//-----------------------------------------------------------------------
	SceneManager* Root::getSceneManager(const String& instanceName) const
	{
		return mSceneManagerEnum->getSceneManager(instanceName);
	}
	//-----------------------------------------------------------------------
	SceneManagerEnumerator::SceneManagerIterator Root::getSceneManagerIterator(void)
	{
		return mSceneManagerEnum->getSceneManagerIterator();
	}
    //-----------------------------------------------------------------------
    TextureManager* Root::getTextureManager(void)
    {
        return &TextureManager::getSingleton();
    }
    //-----------------------------------------------------------------------
    MeshManager* Root::getMeshManager(void)
    {
        return &MeshManager::getSingleton();
    }
    //-----------------------------------------------------------------------
    void Root::addFrameListener(FrameListener* newListener)
    {
		// Check if the specified listener is scheduled for removal
		std::set<FrameListener *>::iterator i = mRemovedFrameListeners.find(newListener);

		// If yes, cancel the removal. Otherwise add it to other listeners.
		if (i != mRemovedFrameListeners.end())
			mRemovedFrameListeners.erase(*i);
		else
			mFrameListeners.insert(newListener); // Insert, unique only (set)
    }

    //-----------------------------------------------------------------------
    void Root::removeFrameListener(FrameListener* oldListener)
    {
        // Remove, 1 only (set)
        mRemovedFrameListeners.insert(oldListener);
    }
    //-----------------------------------------------------------------------
    bool Root::_fireFrameStarted(FrameEvent& evt)
    {
        // Increment frame number
        ++mCurrentFrame;

        // Remove all marked listeners
        std::set<FrameListener*>::iterator i;
        for (i = mRemovedFrameListeners.begin();
            i != mRemovedFrameListeners.end(); i++)
        {
            mFrameListeners.erase(*i);
        }
        mRemovedFrameListeners.clear();

        // Tell all listeners
        for (i= mFrameListeners.begin(); i != mFrameListeners.end(); ++i)
        {
            if (!(*i)->frameStarted(evt))
                return false;
        }

        return true;

    }
    //-----------------------------------------------------------------------
    bool Root::_fireFrameEnded(FrameEvent& evt)
    {
        // Remove all marked listeners
        std::set<FrameListener*>::iterator i;
        for (i = mRemovedFrameListeners.begin();
            i != mRemovedFrameListeners.end(); i++)
        {
            mFrameListeners.erase(*i);
        }
        mRemovedFrameListeners.clear();

        // Tell all listeners
		bool ret = true;
        for (i= mFrameListeners.begin(); i != mFrameListeners.end(); ++i)
        {
            if (!(*i)->frameEnded(evt))
			{
                ret = false;
				break;
			}
        }

        // Tell buffer manager to free temp buffers used this frame
        if (HardwareBufferManager::getSingletonPtr())
            HardwareBufferManager::getSingleton()._releaseBufferCopies();

        return ret;
    }
    //-----------------------------------------------------------------------
    bool Root::_fireFrameStarted()
    {
        unsigned long now = mTimer->getMilliseconds();
        FrameEvent evt;
        evt.timeSinceLastEvent = calculateEventTime(now, FETT_ANY);
        evt.timeSinceLastFrame = calculateEventTime(now, FETT_STARTED);

        return _fireFrameStarted(evt);
    }
    //-----------------------------------------------------------------------
    bool Root::_fireFrameEnded()
    {
        unsigned long now = mTimer->getMilliseconds();
        FrameEvent evt;
        evt.timeSinceLastEvent = calculateEventTime(now, FETT_ANY);
        evt.timeSinceLastFrame = calculateEventTime(now, FETT_ENDED);

        return _fireFrameEnded(evt);
    }
    //-----------------------------------------------------------------------
    Real Root::calculateEventTime(unsigned long now, FrameEventTimeType type)
    {
        // Calculate the average time passed between events of the given type
        // during the last mFrameSmoothingTime seconds.

        std::deque<unsigned long>& times = mEventTimes[type];
        times.push_back(now);

        if(times.size() == 1)
            return 0;

        // Times up to mFrameSmoothingTime seconds old should be kept
        unsigned long discardThreshold =
			static_cast<unsigned long>(mFrameSmoothingTime * 1000.0f);

        // Find the oldest time to keep
        std::deque<unsigned long>::iterator it = times.begin(),
            end = times.end()-2; // We need at least two times
        while(it != end)
        {
            if (now - *it > discardThreshold)
                ++it;
            else
                break;
        }

        // Remove old times
        times.erase(times.begin(), it);

        return Real(times.back() - times.front()) / ((times.size()-1) * 1000);
    }
    //-----------------------------------------------------------------------
    void Root::queueEndRendering(void)
    {
	    mQueuedEnd = true;
    }
    //-----------------------------------------------------------------------
    void Root::startRendering(void)
    {
        assert(mActiveRenderer != 0);

        mActiveRenderer->_initRenderTargets();

        // Clear event times
        for(int i=0; i!=3; ++i)
            mEventTimes[i].clear();

        // Infinite loop, until broken out of by frame listeners
        // or break out by calling queueEndRendering()
        mQueuedEnd = false;

        while( !mQueuedEnd )
        {
            //Allow platform to pump/create/etc messages/events once per frame
            mPlatformManager->messagePump(mAutoWindow);

            if (!renderOneFrame())
                break;
        }
    }
    //-----------------------------------------------------------------------
    bool Root::renderOneFrame(void)
    {
        if(!_fireFrameStarted())
            return false;

        _updateAllRenderTargets();

        return _fireFrameEnded();
    }
    //-----------------------------------------------------------------------
    void Root::shutdown(void)
    {
		SceneManagerEnumerator::getSingleton().shutdownAll();
		shutdownPlugins();
		shutdownInnerPlugins();

        ShadowVolumeExtrudeProgram::shutdown();
		mResourceBackgroundQueue->shutdown();
        ResourceGroupManager::getSingleton().shutdownAll();

		mIsInitialised = false;

		LogManager::getSingleton().logMessage("*-*-* OGRE Shutdown");
    }
    //-----------------------------------------------------------------------
    void Root::loadPlugins( const String& pluginsfile )
    {
        StringVector pluginList;
        String pluginDir;
        ConfigFile cfg;

		try {
        	cfg.load( pluginsfile );
		}
		catch (Exception)
		{
			LogManager::getSingleton().logMessage(pluginsfile + " not found, automatic plugin loading disabled.");
			return;
		}

        pluginDir = cfg.getSetting("PluginFolder"); // Ignored on Mac OS X, uses Resources/ directory
        pluginList = cfg.getMultiSetting("Plugin");

        char last_char = pluginDir[pluginDir.length()-1];
        if (last_char != '/' && last_char != '\\')
        {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            pluginDir += "\\";
#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX
            pluginDir += "/";
#endif
        }

        for( StringVector::iterator it = pluginList.begin(); it != pluginList.end(); ++it )
        {
			loadPlugin(pluginDir + (*it));
        }

    }
    //-----------------------------------------------------------------------
	void Root::shutdownPlugins(void)
	{
		std::vector<DynLib*>::reverse_iterator i;

		// NB Shutdown plugins in reverse order to enforce dependencies
		for (i = mPluginLibs.rbegin(); i != mPluginLibs.rend(); ++i)
		{
			// Call plugin shutdown (optional)
			DLL_STOP_PLUGIN pFunc = (DLL_STOP_PLUGIN)(*i)->getSymbol("dllShutdownPlugin");
			if (pFunc)
			{
				pFunc();
			}

		}
	}
	//-----------------------------------------------------------------------
	void Root::initialisePlugins(void)
	{
		std::vector<DynLib*>::iterator i;

		for (i = mPluginLibs.begin(); i != mPluginLibs.end(); ++i)
		{
			// Call plugin initialise (optional)
			DLL_INIT_PLUGIN pFunc = (DLL_INIT_PLUGIN)(*i)->getSymbol("dllInitialisePlugin");
			if (pFunc)
			{
				pFunc();
			}

		}
	}
	//-----------------------------------------------------------------------
	void Root::unloadPlugins(void)
    {
        std::vector<DynLib*>::reverse_iterator i;

        // NB Unload plugins in reverse order to enforce dependencies
        for (i = mPluginLibs.rbegin(); i != mPluginLibs.rend(); ++i)
        {
            // Call plugin shutdown
            DLL_STOP_PLUGIN pFunc = (DLL_STOP_PLUGIN)(*i)->getSymbol("dllStopPlugin");
            pFunc();
            // Unload library & destroy
            DynLibManager::getSingleton().unload(*i);

        }

        mPluginLibs.clear();

    }
    //-----------------------------------------------------------------------
    void Root::addResourceLocation(const String& name, const String& locType,
		const String& groupName, bool recursive)
    {
		ResourceGroupManager::getSingleton().addResourceLocation(
			name, locType, groupName, recursive);
    }
	//-----------------------------------------------------------------------
	void Root::removeResourceLocation(const String& name, const String& groupName)
	{
		ResourceGroupManager::getSingleton().removeResourceLocation(
			name, groupName);
	}
    //-----------------------------------------------------------------------
    void Root::convertColourValue(const ColourValue& colour, uint32* pDest)
    {
        assert(mActiveRenderer != 0);
        mActiveRenderer->convertColourValue(colour, pDest);
    }
    //-----------------------------------------------------------------------
    RenderWindow* Root::getAutoCreatedWindow(void)
    {
        return mAutoWindow;
    }
    //-----------------------------------------------------------------------
	RenderWindow* Root::createRenderWindow(const String &name, unsigned int width, unsigned int height,
			bool fullScreen, const NameValuePairList *miscParams)
	{
        if (!mActiveRenderer)
        {
            OGRE_EXCEPT(Exception::ERR_NO_RENDERSYSTEM_SELECTED,
            "Cannot create window - no render "
            "system has been selected.", "Root::createRenderWindow");
        }
        RenderWindow* ret;
        ret = mActiveRenderer->createRenderWindow(name, width, height, fullScreen, miscParams);

        // Initialisation for classes dependent on first window created
        if(!mFirstTimePostWindowInit)
        {
            oneTimePostWindowInit();
            ret->_setPrimary();
        }

        return ret;

    }
    //-----------------------------------------------------------------------
    void Root::detachRenderTarget(RenderTarget* target)
    {
        if (!mActiveRenderer)
        {
            OGRE_EXCEPT(Exception::ERR_NO_RENDERSYSTEM_SELECTED,
            "Cannot create window - no render "
            "system has been selected.", "Root::destroyRenderWindow");
        }

        mActiveRenderer->detachRenderTarget( target->getName() );
    }
    //-----------------------------------------------------------------------
    void Root::detachRenderTarget(const String &name)
    {
        if (!mActiveRenderer)
        {
            OGRE_EXCEPT(Exception::ERR_NO_RENDERSYSTEM_SELECTED,
            "Cannot create window - no render "
            "system has been selected.", "Root::destroyRenderWindow");
        }

        mActiveRenderer->detachRenderTarget( name );
    }
    //-----------------------------------------------------------------------
    RenderTarget* Root::getRenderTarget(const String &name)
    {
        if (!mActiveRenderer)
        {
            OGRE_EXCEPT(Exception::ERR_NO_RENDERSYSTEM_SELECTED,
            "Cannot create window - no render "
            "system has been selected.", "Root::getRenderWindow");
        }

        return mActiveRenderer->getRenderTarget(name);
    }
    //-----------------------------------------------------------------------
	void Root::loadPlugin(const String& pluginName)
	{
		// Load plugin library
        DynLib* lib = DynLibManager::getSingleton().load( pluginName );
		// Store for later unload
		mPluginLibs.push_back(lib);

		// Call startup function
		DLL_START_PLUGIN pFunc = (DLL_START_PLUGIN)lib->getSymbol("dllStartPlugin");

		if (!pFunc)
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND, "Cannot find symbol dllStartPlugin in library " + pluginName,
				"Root::loadPlugins");
		pFunc();

		if (mIsInitialised)
		{
			// initialise too
			DLL_INIT_PLUGIN pFunc = (DLL_INIT_PLUGIN)lib->getSymbol("dllInitialisePlugin");
			if (pFunc)
			{
				pFunc();
			}
		}
	}
    //-----------------------------------------------------------------------
	void Root::unloadPlugin(const String& pluginName)
	{
        std::vector<DynLib*>::iterator i;

        for (i = mPluginLibs.begin(); i != mPluginLibs.end(); ++i)
        {
			if ((*i)->getName() == pluginName)
			{
				// Call plugin shutdown
				DLL_STOP_PLUGIN pFunc = (DLL_STOP_PLUGIN)(*i)->getSymbol("dllStopPlugin");
				pFunc();
				// Unload library (destroyed by DynLibManager)
				DynLibManager::getSingleton().unload(*i);
				mPluginLibs.erase(i);
				return;
			}

        }
	}
	//-----------------------------------------------------------------------
	void Root::loadInnerPlugins()
	{
		if( mInnerPlugined == false )
		{
			registerParticleFactories();
			registerParticleFactories2();
			StartOctreeSceneManager();
			StartCgProgramManager();
			StartScorpio();
			mInnerPlugined = true;
		}
	}
	//-----------------------------------------------------------------------
	void Root::loadRenderPlugins_D3D9()
	{
		if( mRenderPlugined_D3D9 == false )
		{
			StartDirect3D9();
			setRenderSystem(getRenderSystemByName("Direct3D9 Rendering Subsystem"));
			mRenderPlugined_D3D9 = true;
		}
	}
	//-----------------------------------------------------------------------
	void Root::loadRenderPlugins_GL()
	{
		if( mRenderPlugined_GL == false )
		{
			loadPlugin("RenderSystem_GL.dll");
			setRenderSystem(getRenderSystemByName("OpenGL Rendering Subsystem"));
			mRenderPlugined_GL = true;
		}
	}
	//-----------------------------------------------------------------------
	void Root::shutdownInnerPlugins()
	{
		if( mInnerPlugined  == true )
		{
			ShutdownOctreeSceneManager();
		}
	}
	//-----------------------------------------------------------------------
	void Root::unloadInnerPlugins()
	{
		if( mInnerPlugined  == true )
		{
			destroyParticleFactories();
			destroyParticleFactories2();
			StopOctreeSceneManager();
			StopCgProgramManager();
			StopScorpio();
			mInnerPlugined = false;
		}
	}
	//-----------------------------------------------------------------------
	void Root::unloadRenderPlugins()
	{
		if( mRenderPlugined_D3D9 == true )
		{
			StopDirect3D9();
			mRenderPlugined_D3D9 = false;
		}
		if( mRenderPlugined_GL == true )
		{
			unloadPlugin("RenderSystem_GL.dll");
			mRenderPlugined_GL = false;
		}
	}
	//-----------------------------------------------------------------------
	void Root::loadRenderPlugins()
	{
		StartDirect3D9();
		loadPlugin("RenderSystem_GL.dll");

		mRenderPlugined_D3D9 = true;
		mRenderPlugined_GL = true;
	}
	//-----------------------------------------------------------------------
	void Root::startupRenderPlugins(const String& name)
	{
		setRenderSystem(getRenderSystemByName(name));
	}
    //-----------------------------------------------------------------------
    Timer* Root::getTimer(void)
    {
        return mTimer;
    }
    //-----------------------------------------------------------------------
    void Root::oneTimePostWindowInit(void)
    {
        if (!mFirstTimePostWindowInit)
        {
			// Initialise material manager
			mMaterialManager->initialise();
            // Init particle systems manager
            mParticleManager->_initialise();
			// Init mesh manager
			MeshManager::getSingleton()._initialise();
            mFirstTimePostWindowInit = true;
        }

    }
    //-----------------------------------------------------------------------
    void Root::_updateAllRenderTargets(void)
    {
        // delegate
        mActiveRenderer->_updateAllRenderTargets();
    }
	//-----------------------------------------------------------------------
	void Root::clearEventTimes(void)
	{
		// Clear event times
		for(int i=0; i<3; ++i)
			mEventTimes[i].clear();
	}
	//---------------------------------------------------------------------
	void Root::addMovableObjectFactory(MovableObjectFactory* fact,
		bool overrideExisting)
	{
		MovableObjectFactoryMap::iterator facti = mMovableObjectFactoryMap.find(
			fact->getType());
		if (!overrideExisting && facti != mMovableObjectFactoryMap.end())
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
				"A factory of type '" + fact->getType() + "' already exists.",
				"Root::addMovableObjectFactory");
		}

		if (fact->requestTypeFlags())
		{
			if (facti != mMovableObjectFactoryMap.end() && facti->second->requestTypeFlags())
			{
				// Copy type flags from the factory we're replacing
				fact->_notifyTypeFlags(facti->second->getTypeFlags());
			}
			else
			{
				// Allocate new
				fact->_notifyTypeFlags(_allocateNextMovableObjectTypeFlag());
			}
		}

		// Save
		mMovableObjectFactoryMap[fact->getType()] = fact;

		LogManager::getSingleton().logMessage("MovableObjectFactory for type '" +
			fact->getType() + "' registered.");

	}
	//---------------------------------------------------------------------
	bool Root::hasMovableObjectFactory(const String& typeName) const
	{
		return !(mMovableObjectFactoryMap.find(typeName) == mMovableObjectFactoryMap.end());
	}
	//---------------------------------------------------------------------
	MovableObjectFactory* Root::getMovableObjectFactory(const String& typeName)
	{
		MovableObjectFactoryMap::iterator i =
			mMovableObjectFactoryMap.find(typeName);
		if (i == mMovableObjectFactoryMap.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
				"MovableObjectFactory of type " + typeName + " does not exist",
				"Root::getMovableObjectFactory");
		}
		return i->second;
	}
	//---------------------------------------------------------------------
	uint32 Root::_allocateNextMovableObjectTypeFlag(void)
	{
		if (mNextMovableObjectTypeFlag == SceneManager::USER_TYPE_MASK_LIMIT)
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
				"Cannot allocate a type flag since "
				"all the available flags have been used.",
				"Root::_allocateNextMovableObjectTypeFlag");

		}
		uint32 ret = mNextMovableObjectTypeFlag;
		mNextMovableObjectTypeFlag <<= 1;
		return ret;

	}
	//---------------------------------------------------------------------
	void Root::removeMovableObjectFactory(MovableObjectFactory* fact)
	{
		MovableObjectFactoryMap::iterator i = mMovableObjectFactoryMap.find(
			fact->getType());
		if (i != mMovableObjectFactoryMap.end())
		{
			mMovableObjectFactoryMap.erase(i);
		}

	}
	//---------------------------------------------------------------------
	Root::MovableObjectFactoryIterator
	Root::getMovableObjectFactoryIterator(void) const
	{
		return MovableObjectFactoryIterator(mMovableObjectFactoryMap.begin(),
			mMovableObjectFactoryMap.end());

	}
	//---------------------------------------------------------------------
	RenderQueueInvocationSequence* Root::createRenderQueueInvocationSequence(
		const String& name)
	{
		RenderQueueInvocationSequenceMap::iterator i =
			mRQSequenceMap.find(name);
		if (i != mRQSequenceMap.end())
		{
			OGRE_EXCEPT(Exception::ERR_DUPLICATE_ITEM,
				"RenderQueueInvocationSequence with the name " + name +
					" already exists.",
				"Root::createRenderQueueInvocationSequence");
		}
		RenderQueueInvocationSequence* ret = new RenderQueueInvocationSequence(name);
		mRQSequenceMap[name] = ret;
		return ret;
	}
	//---------------------------------------------------------------------
	RenderQueueInvocationSequence* Root::getRenderQueueInvocationSequence(
		const String& name)
	{
		RenderQueueInvocationSequenceMap::iterator i =
			mRQSequenceMap.find(name);
		if (i == mRQSequenceMap.end())
		{
			OGRE_EXCEPT(Exception::ERR_ITEM_NOT_FOUND,
				"RenderQueueInvocationSequence with the name " + name +
				" not found.",
				"Root::getRenderQueueInvocationSequence");
		}
		return i->second;
	}
	//---------------------------------------------------------------------
	void Root::destroyRenderQueueInvocationSequence(
		const String& name)
	{
		RenderQueueInvocationSequenceMap::iterator i =
			mRQSequenceMap.find(name);
		if (i != mRQSequenceMap.end())
		{
			delete i->second;
			mRQSequenceMap.erase(i);
		}
	}
	//---------------------------------------------------------------------
	void Root::destroyAllRenderQueueInvocationSequences(void)
	{
		for (RenderQueueInvocationSequenceMap::iterator i = mRQSequenceMap.begin();
			i != mRQSequenceMap.end(); ++i)
		{
			delete i->second;
		}
		mRQSequenceMap.clear();
	}
	//---------------------------------------------------------------------
    String Root::generateUniqueName(const String& prefix)
    {
        size_t num = mGeneratedNameSeqNumberMap[prefix]++;

        StringUtil::StrStreamType str;
        str << prefix << num;
        return str.str();
    }
    //---------------------------------------------------------------------



}
