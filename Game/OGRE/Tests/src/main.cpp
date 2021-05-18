#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include "OgreException.h"
#include "OgreLogManager.h"
#include "OgreMaterialManager.h"
#include "OgreCompositorManager.h"
#include "OgreHighLevelGpuProgramManager.h"
#include "OgreGpuProgramManager.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

int main( int argc, char **argv)
{
    std::auto_ptr<Ogre::LogManager> logMgr(new Ogre::LogManager());
    logMgr->createLog("OgreTest.log", true, true);

    bool wasSuccessful = false;
    try {
        CppUnit::TextUi::TestRunner runner;
        CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
        runner.addTest( registry.makeTest() );
        wasSuccessful = runner.run( "", false );
    }
    catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        std::cerr << "An exception has occured: " << e.getFullDescription();
#endif
    }
    catch( ... ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        MessageBox( NULL, "Unknown exception", "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        std::cerr << "An exception has occured: " << e.getFullDescription();
#endif
    }

    // shutdown and release managers that might have been created
    delete Ogre::HighLevelGpuProgramManager::getSingletonPtr();
    delete Ogre::GpuProgramManager::getSingletonPtr();
    delete Ogre::CompositorManager::getSingletonPtr();
    delete Ogre::MaterialManager::getSingletonPtr();
    delete Ogre::ResourceGroupManager::getSingletonPtr();

    return wasSuccessful ? 0 : 1;
}
