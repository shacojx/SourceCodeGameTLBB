#include "ScorpioCloth.h"
#include "ScorpioFabric.h"

#include <OgreRoot.h>

namespace Ogre
{
    namespace
    {
        static std::vector<MovableObjectFactory*> movableObjectFactories;
        //-----------------------------------------------------------------------
        static void registerMovableObjectFactories(void)
        {
            // -- Create all new movable object factories --
            MovableObjectFactory* pFactory;

            // Cloth
            pFactory = new ClothFactory;
            movableObjectFactories.push_back(pFactory);
            Root::getSingleton().addMovableObjectFactory(pFactory);

            // Fabric
            pFactory = new FabricFactory;
            movableObjectFactories.push_back(pFactory);
            Root::getSingleton().addMovableObjectFactory(pFactory);
        }
        //-----------------------------------------------------------------------
        static void destroyMovableObjectFactories(void)
        {
            std::vector<MovableObjectFactory*>::const_iterator mi;
            for (mi = movableObjectFactories.begin(); mi != movableObjectFactories.end(); ++mi)
            {
                delete *mi;
            }
        }
    }
    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    void _OgreExport StartScorpio(void) throw()
    {
        // Physics system
        registerMovableObjectFactories();
    }
    //-----------------------------------------------------------------------
    void _OgreExport StopScorpio(void) throw()
    {
        // Physics system
        destroyMovableObjectFactories();
    }
}

#include <OgreNoMemoryMacros.h>

namespace Ogre
{
    // Since VC CRT allow replacable operator new/new[], but not that
    // for operator delete/delete[]. Thus, if some source file which
    // used new[]/delete[] but never includes OgreMemoryMacros.h, and
    // all source files that included OgreMemoryMacros.h and then use
    // new/delete only, the problem occuring then: In this case, only
    // new/delete are instantiated, and then in link time, replacable
    // new[] supplied by CRT will linked, which call to ourself version
    // of new, and the CRT DLL version delete[] will linked too, which
    // call to CRT DLL version of delete!
    //
    // Force instantiate all four functions here to resolve this problem.
    //
    void __force_instantiate_global_overridden(void)
    {
        ::operator new(0);
        ::operator delete(0);
        ::operator new[](0);
        ::operator delete[](0);
    }
}
