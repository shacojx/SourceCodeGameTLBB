#include "JunctionPoints.h"
#include "SceneManipulator.h"
#include "TerrainSelections.h"

#include "Core/WXUtils.h"

#include <OgreSceneManager.h>
#include <OgreMesh.h>
#include <OgreEntity.h>
#include <OgreStringConverter.h>

namespace WX {

    JunctionPoints::JunctionPoints(SceneManipulator* sceneManipulator)
        : HitIndicator(sceneManipulator)
        , mBaseMesh()
        , mScale(1)
    {
        Real radius = 1;
        int rings = 8;
        int segments = 8;
        mBaseMesh = createCommonSphere(radius, rings, segments);

        reshape();
    }

    JunctionPoints::~JunctionPoints()
    {
        for (DisplayNodes::const_iterator it = mDisplayNodes.begin(); it != mDisplayNodes.end(); ++it)
        {
            cleanupAndDestroySceneNode(it->second);
        }

        for (FreeNodes::const_iterator it = mFreeNodes.begin(); it != mFreeNodes.end(); ++it)
        {
            cleanupAndDestroySceneNode(*it);
        }
    }

    Ogre::SceneNode*
    JunctionPoints::allocNode(void)
    {
        Ogre::SceneNode* node;
        if (mFreeNodes.empty())
        {
            node = getIndicatorRootSceneNode()->createChildSceneNode();
            Ogre::Entity* entity = getSceneManager()->createEntity(node->getName(), mBaseMesh->getName());
            entity->setQueryFlags(0);
            entity->setNormaliseNormals(true);
            node->attachObject(entity);
        }
        else
        {
            node = mFreeNodes.back();
            mFreeNodes.pop_back();
            getIndicatorRootSceneNode()->addChild(node);
        }

        node->setScale(mScale, mScale, mScale);
        return node;
    }

    void
    JunctionPoints::freeNode(Ogre::SceneNode* node)
    {
        node->getParentSceneNode()->removeChild(node->getName());
        mFreeNodes.push_back(node);
    }

    const String&
    JunctionPoints::getName(void) const
    {
        static const String name = "JunctionPoints";
        return name;
    }

    void
    JunctionPoints::hide(void)
    {
        for (DisplayNodes::const_iterator it = mDisplayNodes.begin(); it != mDisplayNodes.end(); ++it)
        {
            freeNode(it->second);
        }
        mDisplayNodes.clear();
    }

    void
    JunctionPoints::_setHitPoint(Real x, Real y)
    {
        JunctionSelection* selection = static_cast<JunctionSelection*>(
            getSceneManipulator()->_getSelection("JunctionSelection"));

        Ogre::Vector3 position;
        bool intersected = getSceneManipulator()->getTerrainIntersects(x, y, position);
        if (!intersected)
        {
            selection->reset();
            hide();
            return;
        }
        getSceneManipulator()->_buildSelection(selection, position.x, position.z);

        refresh();
    }

    void
    JunctionPoints::refresh(void)
    {
        JunctionSelection* selection = static_cast<JunctionSelection*>(
            getSceneManipulator()->_getSelection("JunctionSelection"));
        const JunctionSelection::JunctionMap& junctions = selection->getJunctions();

        DisplayNodes newDisplayNodes;

        // Use existing point if possible
        for (JunctionSelection::JunctionMap::const_iterator it = junctions.begin(); it != junctions.end(); ++it)
        {
            const JunctionSelection::Junction& junction = it->second;

            Ogre::Vector3 pos = getTerrainData()->_getPosition(junction.x, junction.z);
            DisplayNodes::iterator found = mDisplayNodes.find(Ogre::Vector4(pos.x, pos.y, pos.z, junction.weight));
            Ogre::SceneNode* node = NULL;
            if (found != mDisplayNodes.end())
            {
                node = found->second;
                mDisplayNodes.erase(found);
            }

			newDisplayNodes.insert(DisplayNodes::value_type(Ogre::Vector4(pos.x, pos.y, pos.z, junction.weight), node));
        }

        // Build new point and adjust position
        for (DisplayNodes::iterator it = newDisplayNodes.begin(); it != newDisplayNodes.end(); ++it)
        {
            if (!it->second)
            {
                if (!mDisplayNodes.empty())
                {
                    DisplayNodes::iterator found = mDisplayNodes.begin();
                    it->second = found->second;
                    mDisplayNodes.erase(found);
                }
                else
                {
                    it->second = allocNode();
                }

                it->second->setPosition(it->first.x,it->first.y,it->first.z);

				// 根据节点的权重来设置节点所挂物体的材质颜色
				Ogre::MaterialPtr material = createPureColourMaterial(Ogre::ColourValue(it->first.w,(1.0f-it->first.w),0.0f));

				// 设置节点的材质
				static_cast<Ogre::Entity *>(it->second->getAttachedObject(0))->setMaterialName(material->getName());		
            }
        }

        // Hide extra points
        for (DisplayNodes::const_iterator it = mDisplayNodes.begin(); it != mDisplayNodes.end(); ++it)
        {
            freeNode(it->second);
        }

        std::swap(newDisplayNodes, mDisplayNodes);
    }

    void
    JunctionPoints::reshape(void)
    {
        // Adjust scale
        mScale = getSceneManipulator()->getBaseScale() / (7 * mBaseMesh->getBoundingSphereRadius());
        for (DisplayNodes::const_iterator it = mDisplayNodes.begin(); it != mDisplayNodes.end(); ++it)
        {
            Ogre::SceneNode* node = it->second;
            node->setScale(mScale, mScale, mScale);
        }
    }

}
