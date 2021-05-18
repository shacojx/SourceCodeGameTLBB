#include "SoundEditAction.h"

#include "SceneManipulator.h"

#include "Core/TerrainData.h"
#include "Core/WXUtils.h"

#include <OgreEntity.h>
#include <OgreStringConverter.h>

namespace WX
{
    SoundEditAction::SoundEditAction(SceneManipulator* manipulator)
        : Action(manipulator)
        , mSoundNode(NULL)
        , mSoundMovingEntity(NULL)
        , mCreatePos(Ogre::Vector3::ZERO)
        , mFirstInit(false)
        , mNextNameIndex(0)
        , mShowSoundEntity(false)
        , mSoundRadiusEntity(NULL)
    {

    }

    SoundEditAction::~SoundEditAction()
    {
        _destroyIndicatorInstance();
    }

    const String& SoundEditAction::getName(void) const
    {
        static const String name = "SoundEditAction";
        return name;
    }

    void SoundEditAction::setParameter(const String& name, const String& value)
    {
        if (name == "ShowSoundItem")
        {
            mShowSoundEntity = value == "true";
            if (mSoundNode)
                mSoundNode->setVisible(mShowSoundEntity);
        } 
        else if (name == "CreateSoundEntity")
        {
            Ogre::StringVector values = Ogre::StringUtil::split(value);

            _createSoundEntity(Ogre::StringConverter::parseInt(values[0]), Ogre::StringConverter::parseInt(values[1]));
        }
        else if (name == "ClearSoundEntity")
        {
            _clearAllSoundEntities();
        }
        else if (name == "DeleteSoundItem")
        {
            _deleteSoundEntity( Ogre::StringConverter::parseInt(value) );
        }
        else if (name == "ShowRadiusEntity")
        {
            Ogre::StringVector valueVector = Ogre::StringUtil::split(value);

            if (mSoundRadiusEntity)
            {
                Ogre::SceneNode* node = mSoundRadiusEntity->getParentSceneNode();
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

    void SoundEditAction::_onActive(bool active)
    {
        if (!mFirstInit)
            _createIndicatorInstance();

        mSoundMovingEntity->getParentSceneNode()->setVisible(active);
    }

    void SoundEditAction::_onMove(const Point& pt)
    {
        Ogre::Vector3 position, normal;
        if (mSceneManipulator->getTerrainIntersects(pt, position, &normal, true) && mSoundMovingEntity && mSoundNode)
        {
            mSoundMovingEntity->getParentSceneNode()->setPosition(position);
        }
    }

    void SoundEditAction::_onEnd(const Point& pt, bool canceled)
    {
        if (!canceled)
        {
            Ogre::Vector3 normal;
            if (mSceneManipulator->getTerrainIntersects(pt, mCreatePos, &normal, true) && mSoundMovingEntity && mSoundNode)
            {
                // Permanent it to final scene
                mSceneManipulator->_fireUIChanged(static_cast<void*>(&mCreatePos), UIC_SOUNDEDIT);

                Ogre::SceneManager* sceneManager = mSceneManipulator->getSceneManager();

                Ogre::Entity* entity = sceneManager->createEntity("SoundEntity" + Ogre::StringConverter::toString(mNextNameIndex++), "axes.mesh");

                mSoundNode->createChildSceneNode()->attachObject(entity);

                entity->getParentSceneNode()->setPosition(mCreatePos);
                entity->getParentSceneNode()->setScale(10,10,10);
                mSoundEntities.push_back(entity);
            }
        }
    }

    void SoundEditAction::_createIndicatorInstance(void)
    {
        Ogre::SceneManager* sceneManager = mSceneManipulator->getSceneManager();

        mSoundMovingEntity = sceneManager->createEntity("SoundMovingEntity", "axes.mesh");
        mSoundNode = mSceneManipulator->getIndicatorRootSceneNode()->createChildSceneNode();

        mSoundNode->setVisible(false);

        mSoundNode->createChildSceneNode()->attachObject(mSoundMovingEntity);
        mSoundMovingEntity->getParentSceneNode()->setScale(10,10,10);

        Real radius = 1;
        int rings = 16;
        int segments = 64;

        Ogre::MeshPtr sphereMesh = WX::createCommonSphere("SoundRadiusEntity", radius, rings, segments);
        Ogre::MaterialPtr material = WX::createPureColourMaterial(
            Ogre::ColourValue(1, 0, 0, 0.75)   );

        material->getTechnique(0)->getPass(0)->setPolygonMode(Ogre::PM_WIREFRAME);

        mSoundRadiusEntity = sceneManager->createEntity("SoundRadiusEntity", sphereMesh->getName());
        mSoundNode->createChildSceneNode()->attachObject(mSoundRadiusEntity);
        mSoundRadiusEntity->setMaterialName(material->getName());
        mSoundRadiusEntity->getParentSceneNode()->setVisible(false);

        mFirstInit = true;
    }

    void SoundEditAction::_destroyIndicatorInstance(void)
    {
        if (mFirstInit)
        {
            mSoundMovingEntity->getParentSceneNode()->detachObject(mSoundMovingEntity);
            mSoundMovingEntity->destroy();

            mSoundRadiusEntity->destroy();
        }
    }

    void SoundEditAction::_createSoundEntity(int gridX, int gridZ)
    {
        if (!mFirstInit)
            _createIndicatorInstance();

        WX::TerrainData* terrainData = mSceneManipulator->getTerrainData();

        std::pair<Real, Real> worldPos = terrainData->gridToWorld(gridX, gridZ);
        Real worldHeight = terrainData->getHeightAt(worldPos.first, worldPos.second);

        Ogre::SceneManager* sceneManager = mSceneManipulator->getSceneManager();

        Ogre::Entity* entity = sceneManager->createEntity("SoundEntity" + Ogre::StringConverter::toString(mNextNameIndex++), "axes.mesh");

        mSoundNode->createChildSceneNode()->attachObject(entity);

        entity->getParentSceneNode()->setPosition(worldPos.first, worldHeight, worldPos.second);

        entity->getParentSceneNode()->setVisible(mShowSoundEntity);
        entity->getParentSceneNode()->setScale(10,10,10);

        mSoundEntities.push_back(entity);
    }

    void SoundEditAction::_clearAllSoundEntities(void)
    {
        for (size_t i=0; i<mSoundEntities.size(); ++i)
        {
            Ogre::Entity* entity = mSoundEntities[i];
            entity->destroy();
        }
        mSoundEntities.clear();

        mNextNameIndex = 0;
    }

    void SoundEditAction::_deleteSoundEntity(size_t index)
    {
        Ogre::Entity* entity = mSoundEntities[index];
        entity->destroy();

        mSoundEntities.erase(mSoundEntities.begin() + index);
    }
}