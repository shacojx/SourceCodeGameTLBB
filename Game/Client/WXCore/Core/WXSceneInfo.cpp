#include "WXSceneInfo.h"
#include "WXSceneSerializer.h"
#include "Terrain.h"
#include "WXSystem.h"

#include <OgreSceneManager.h>
#include <OgreFrameListener.h>
#include <OgreRoot.h>
#include <OgreCamera.h>

namespace WX
{
#define STARTUP_BUILD_OBJECTS   64
#define RUNNING_BUILD_OBJECTS   1
const float MAX_DISTANCE = 2000.0f;
const float MAX_DISTANCE_SQUARED = MAX_DISTANCE * MAX_DISTANCE;

    //-----------------------------------------------------------------------
    class IncrementalSceneBuilder : public Ogre::FrameListener
    {
    public:
        IncrementalSceneBuilder(WX::System* system)
            : mSystem(system)
            , mObjectsPerFrame(STARTUP_BUILD_OBJECTS)
            , mDisableCreateObjects(false)
            , mImmediatelyCreateObjects(false)
            , mEnableStaticGeometry(false)
            , mStaticGeometryBaked(false)
        {
            const char* s = getenv("Fairy_Hack");
            if (s)
            {
                mDisableCreateObjects = strstr(s, "DisableCreateObjects") != 0;
                mImmediatelyCreateObjects = strstr(s, "ImmediatelyCreateObjects") != 0;
                mEnableStaticGeometry = strstr(s, "EnableStaticGeometry") != 0;
            }

            mSystem->getRoot()->addFrameListener(this);
        }

        ~IncrementalSceneBuilder()
        {
            mSystem->getRoot()->removeFrameListener(this);
        }

        bool frameStarted(const Ogre::FrameEvent& evt)
        {
            if (!objects.empty())
            {
                for (size_t count = mObjectsPerFrame; count; --count)
                {
                    if (!buildOneObject())
                    {
                        break;
                    }
                }

                mObjectsPerFrame = RUNNING_BUILD_OBJECTS;
            }
            else if (mEnableStaticGeometry && !mStaticGeometryBaked)
            {
                mStaticGeometryBaked = true;
                mSystem->bakeStaticGeometries(0);
            }

            return true;
        }

        bool buildOneObject(void)
        {
            Ogre::Camera* currentCam = mSystem->getCamera();

            // get the position of current camera
            Ogre::Vector3 cameraPos = currentCam->getDerivedPosition();;

            // get the position of terrain intersect point
            Ogre::Vector3 position;

            Ogre::Ray cameraRay( currentCam->getPosition(), currentCam->getDirection() );
            bool hit = mSystem->getTerrainIntersects(cameraRay, position);

            // if we find the intersect point
            if (hit)
            {
                // if the intersect point is to far, adjust it.
                if ( (position - cameraPos).squaredLength() > MAX_DISTANCE_SQUARED )
                    cameraPos += currentCam->getDirection() * MAX_DISTANCE;
                else
                    cameraPos = position;
            }  

            ObjectPtr bestObject;
            Real bestDistance = 0;
            for (ObjectInfos::iterator it = objects.begin(); it != objects.end(); )
            {
                ObjectPtr object = it->first.lock();
                if (object)
                {
                    Real distance = (it->second - cameraPos).squaredLength();

                    if (!bestObject || distance < bestDistance)
                    {
                        bestObject = object;
                        bestDistance = distance;
                        if (!distance)
                            break;
                    }

                    ++it;
                }
                else
                {
					ObjectInfos::iterator itErase = it++;
                    objects.erase(itErase);
                }
            }

            if (!bestObject)
                return false;

            bestObject->createRenderInstance(mSystem);
            objects.erase(bestObject);
            return true;
        }

        bool addObject(const ObjectPtr& object)
        {
            if (object->getCategory() == "Enviroment" ||
                object->getCategory() == "Light")
                return false;
            if (object->getType() == "TerrainLiquid")
                return false;
            Variant v = object->getProperty("position");
            if (v.empty())
                return false;

            if (mImmediatelyCreateObjects)
                return mDisableCreateObjects;

            if (!mDisableCreateObjects)
            {
                objects[object] = VariantCast<Ogre::Vector3>(v);
            }

            return true;
        }

        void reset(void)
        {
            objects.clear();
            mObjectsPerFrame = STARTUP_BUILD_OBJECTS;
            mStaticGeometryBaked = false;
        }

    protected:
        WX::System* mSystem;
        size_t mObjectsPerFrame;
        bool mDisableCreateObjects;
        bool mImmediatelyCreateObjects;
        bool mEnableStaticGeometry;
        bool mStaticGeometryBaked;

        typedef std::pair<ObjectWeakPtr, Ogre::Vector3> ObjectInfo;
        typedef std::map<ObjectWeakPtr, Ogre::Vector3> ObjectInfos;
        ObjectInfos objects;
    };

    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    //-----------------------------------------------------------------------
    SceneInfo::SceneInfo()
        : mTerrain(NULL)
        , mTerrainData(NULL)
        , mIncrementalSceneBuilder(NULL)
    {
        mTerrain = new Terrain;
        mTerrainData = mTerrain->getData();
    }
    //-----------------------------------------------------------------------
    SceneInfo::~SceneInfo()
    {
        delete mIncrementalSceneBuilder;
        delete mTerrain;
    }
    //-----------------------------------------------------------------------
    void SceneInfo::load(XMLParser* parser, const String& filename, const String& groupName, bool completely)
    {
        SceneSerializer serializer;
        serializer.load(this, parser, filename, groupName);

        if (!mTerrainFilename.empty())
        {
            getTerrainData()->load(parser, mTerrainFilename, groupName, completely);
        }
    }
    //-----------------------------------------------------------------------
    void SceneInfo::save(XMLWriter* writer, const String& filename)
    {
        String baseName, path;
        Ogre::StringUtil::splitFilename(filename, baseName, path);
        if (Ogre::StringUtil::endsWith(baseName, ".scene"))
            baseName.erase(baseName.length() - (sizeof(".scene") - 1));

        mName = baseName;
        mTerrainFilename = baseName + ".Terrain";
        getTerrainData()->save(writer, path + mTerrainFilename);

        SceneSerializer serializer;
        serializer.save(this, writer, filename);
    }
    //-----------------------------------------------------------------------
    void SceneInfo::destroy(void)
    {
        if (mIncrementalSceneBuilder)
        {
            mIncrementalSceneBuilder->reset();
        }

        for (Objects::const_iterator it = mObjects.begin(); it != mObjects.end(); ++it)
        {
            const ObjectPtr& object = *it;
            object->destroyRenderInstance();
        }

        getTerrain()->clearGeometry();
    }
    //-----------------------------------------------------------------------
    void SceneInfo::reset(void)
    {
        destroy();

        Scene::reset();
        getTerrain()->reset();
    }
    //-----------------------------------------------------------------------
    void SceneInfo::initialise(WX::System* system)
    {
        assert(system);
        system->getSceneManager()->setAmbientLight(Ogre::ColourValue::Black);

        getTerrain()->buildGeometry(system->getBaseSceneNode(), system->isEditable());

        if (system->getDisableIncrementalBuildScene() || system->isEditable())
        {
            if (mIncrementalSceneBuilder)
            {
                mIncrementalSceneBuilder->reset();
            }

            for (Objects::const_iterator it = mObjects.begin(); it != mObjects.end(); ++it)
            {
			    try
			    {
				    const ObjectPtr& object = *it;

                    if (object->hasProperty("create level"))
                    {
                        if ( false == system->_determineCreateLevel( 
                            VariantCast<Real>( object->getProperty("create level") ) ) )
                            continue;
                    }

				    object->createRenderInstance(system);
			    }
			    catch ( Ogre::Exception &e )
			    {
				    system->addExceptionInfo( e.getDescription(), System::ExceptionInfo( (*it)->getName() ) );
			    }
            }
        }
        else
        {
            if (!mIncrementalSceneBuilder)
            {
                mIncrementalSceneBuilder = new IncrementalSceneBuilder(system);
            }

            mIncrementalSceneBuilder->reset();

            for (Objects::const_iterator it = mObjects.begin(); it != mObjects.end(); ++it)
            {
                const ObjectPtr& object = *it;
                if (!mIncrementalSceneBuilder->addObject(object))
                {
                    object->createRenderInstance(system);
                }
            }
        }
    }

}
