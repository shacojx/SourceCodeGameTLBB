#include "DatuPointEditAction.h"

#include "SceneManipulator.h"

#include "Core/TerrainData.h"
#include "Core/WXUtils.h"

#include <OgreEntity.h>
#include <OgreStringConverter.h>

namespace WX
{
    DatuPointEditAction::DatuPointEditAction(SceneManipulator* manipulator)
        : Action(manipulator)
        , mSoundNode(NULL)
        , mSoundMovingEntity(NULL)
        , mCreatePos(Ogre::Vector3::ZERO)
        , mFirstInit(false)
        , mNextNameIndex(0)
        , mShowSoundEntity(false)
        , mDatuPointRadiusEntity(NULL)
    {

    }

    DatuPointEditAction::~DatuPointEditAction()
    {
        _destroyIndicatorInstance();
    }

    const String& DatuPointEditAction::getName(void) const
    {
        static const String name = "DatuPointEditAction";
        return name;
    }

    void DatuPointEditAction::setParameter(const String& name, const String& value)
    {
        if (name == "ShowDatuPointItem")
        {
            mShowSoundEntity = value == "true";
            if (mSoundNode)
            {
                mSoundNode->setVisible(mShowSoundEntity);
            }
        } 
        else if (name == "CreatePointEntity")
        {
            Ogre::StringVector values = Ogre::StringUtil::split(value);

            _createPointEntity(Ogre::StringConverter::parseInt(values[0]), Ogre::StringConverter::parseInt(values[1]));
        }
        else if (name == "ClearPointEntity")
        {
            _clearAllPointEntities();
        }
        else if (name == "DeleteDatuPointItem")
        {
            _deletePointEntity( Ogre::StringConverter::parseInt(value) );
        }
        else if (name == "ShowRadiusEntity")
        {
            Ogre::StringVector valueVector = Ogre::StringUtil::split(value);

            if (mDatuPointRadiusEntity)
            {
                Ogre::SceneNode* node = mDatuPointRadiusEntity->getParentSceneNode();
                node->setVisible(true);

                node->setPosition( Ogre::StringConverter::parseReal(valueVector[0]),
                    Ogre::StringConverter::parseReal(valueVector[1]),
                    Ogre::StringConverter::parseReal(valueVector[2]) );

                float scale = Ogre::StringConverter::parseReal(valueVector[3]) * 100.0f;
                node->setScale( scale, 1, scale );
            }
        }
        else
        {
            Action::setParameter(name, value);
        }
    }

    void DatuPointEditAction::_onActive(bool active)
    {
        if (!mFirstInit)
            _createIndicatorInstance();

        mSoundMovingEntity->getParentSceneNode()->setVisible(active);
    }

    void DatuPointEditAction::_onMove(const Point& pt)
    {
        Ogre::Vector3 position, normal;
        if (mSceneManipulator->getTerrainIntersects(pt, position, &normal, true) && mSoundMovingEntity && mSoundNode)
        {
            mSoundMovingEntity->getParentSceneNode()->setPosition(position);
        }
    }

    void DatuPointEditAction::_onEnd(const Point& pt, bool canceled)
    {
        if (!canceled)
        {
            Ogre::Vector3 normal;
            if (mSceneManipulator->getTerrainIntersects(pt, mCreatePos, &normal, true) && mSoundMovingEntity && mSoundNode)
            {
                // Permanent it to final scene
                mSceneManipulator->_fireUIChanged(static_cast<void*>(&mCreatePos), UIC_DATUPOINTEDIT);

                Ogre::SceneManager* sceneManager = mSceneManipulator->getSceneManager();

                Ogre::Entity* entity = sceneManager->createEntity("DatuPointEntity" + Ogre::StringConverter::toString(mNextNameIndex++), "axes.mesh");

                mSoundNode->createChildSceneNode()->attachObject(entity);

                entity->getParentSceneNode()->setPosition(mCreatePos);
                entity->getParentSceneNode()->setScale(10,10,10);
                mSoundEntities.push_back(entity);
            }
        }
    }

    void DatuPointEditAction::_createIndicatorInstance(void)
    {
        Ogre::SceneManager* sceneManager = mSceneManipulator->getSceneManager();

        mSoundMovingEntity = sceneManager->createEntity("DatuPointMovingEntity", "axes.mesh");
        mSoundNode = mSceneManipulator->getIndicatorRootSceneNode()->createChildSceneNode();

        mSoundNode->setVisible(false);

        mSoundNode->createChildSceneNode()->attachObject(mSoundMovingEntity);

        mSoundMovingEntity->getParentSceneNode()->setScale(10,10,10);

        Real radius = 1;
        int rings = 16;
        int segments = 64;

        Ogre::MeshPtr sphereMesh = WX::createCommonSphere("DatuPointRadiusEntity", radius, rings, segments);
        Ogre::MaterialPtr material = WX::createPureColourMaterial(
            Ogre::ColourValue(1, 0, 0, 0.75)   );

        material->getTechnique(0)->getPass(0)->setPolygonMode(Ogre::PM_WIREFRAME);

        mDatuPointRadiusEntity = sceneManager->createEntity("DatuPointRadiusEntity", sphereMesh->getName());
        mSoundNode->createChildSceneNode()->attachObject(mDatuPointRadiusEntity);
        mDatuPointRadiusEntity->setMaterialName(material->getName());
        mDatuPointRadiusEntity->getParentSceneNode()->setVisible(false);

        mFirstInit = true;
    }

    void DatuPointEditAction::_destroyIndicatorInstance(void)
    {
        if (mFirstInit)
        {
            mSoundMovingEntity->getParentSceneNode()->detachObject(mSoundMovingEntity);
            mSoundMovingEntity->destroy();

            mDatuPointRadiusEntity->destroy();
        }
    }

    void DatuPointEditAction::_createPointEntity(int gridX, int gridZ)
    {
        if (!mFirstInit)
            _createIndicatorInstance();

        WX::TerrainData* terrainData = mSceneManipulator->getTerrainData();

        std::pair<Real, Real> worldPos = terrainData->gridToWorld(gridX, gridZ);
        Real worldHeight = terrainData->getHeightAt(worldPos.first, worldPos.second);

        Ogre::SceneManager* sceneManager = mSceneManipulator->getSceneManager();

        Ogre::Entity* entity = sceneManager->createEntity("DatuPointEntity" + Ogre::StringConverter::toString(mNextNameIndex++), "axes.mesh");

        mSoundNode->createChildSceneNode()->attachObject(entity);

        entity->getParentSceneNode()->setPosition(worldPos.first, worldHeight, worldPos.second);

        entity->getParentSceneNode()->setVisible(mShowSoundEntity);
        entity->getParentSceneNode()->setScale(10,10,10);

        mSoundEntities.push_back(entity);
    }

    void DatuPointEditAction::_clearAllPointEntities(void)
    {
        for (size_t i=0; i<mSoundEntities.size(); ++i)
        {
            Ogre::Entity* entity = mSoundEntities[i];
            entity->destroy();
        }
        mSoundEntities.clear();

        mNextNameIndex = 0;
    }

    void DatuPointEditAction::_deletePointEntity(size_t index)
    {
        Ogre::Entity* entity = mSoundEntities[index];
        entity->destroy();

        mSoundEntities.erase(mSoundEntities.begin() + index);
    }
}