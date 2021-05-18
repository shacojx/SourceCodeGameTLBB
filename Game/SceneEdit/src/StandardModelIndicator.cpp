#include "StandardModelIndicator.h"
#include "SceneManipulator.h"

#include "Core/WXUtils.h"

#include <OgreSceneManager.h>
#include <OgreMesh.h>
#include <OgreEntity.h>
#include <OgreCamera.h>
#include <OgreRay.h>

namespace WX {

    StandardModelIndicator::StandardModelIndicator(SceneManipulator* sceneManipulator)
        : HitIndicator(sceneManipulator)
        , mIntersectNode(NULL)
        , mIntersectEntity(NULL)
    {
        // Create intersection node and entity

        mIntersectNode = getIndicatorRootSceneNode()->createChildSceneNode();

        mIntersectEntity = getSceneManager()->createEntity(mIntersectNode->getName(), "standardmodel.mesh");
        mIntersectEntity->setQueryFlags(0);
        mIntersectEntity->setNormaliseNormals(true);
        mIntersectEntity->setMaterialName("BaseWhite");
        mIntersectEntity->setRenderQueueGroup(Ogre::RENDER_QUEUE_7);
        mIntersectNode->attachObject(mIntersectEntity);
        mIntersectNode->setVisible(false);

        reshape();
    }

    StandardModelIndicator::~StandardModelIndicator()
    {
        if (mIntersectNode)
        {
            mIntersectNode->destroy();
        }
        if (mIntersectEntity)
        {
            mIntersectEntity->destroy();
        }
    }

    const String& StandardModelIndicator::getName(void) const
    {
        static const String name = "StandardModelIndicator";
        return name;
    }

    void StandardModelIndicator::hide(void)
    {
        mIntersectNode->setVisible(false);
    }

    void StandardModelIndicator::_setHitPoint(Real x, Real y)
    {
        Ogre::Vector3 position;

        Ogre::Ray cameraRay( getCamera()->getPosition(), getCamera()->getDirection() );
        bool hit = getSceneManipulator()->getTerrainIntersects(cameraRay, position);
        mIntersectNode->setVisible(hit);
        if (hit)
        {
            mIntersectNode->setPosition(position);
        }
    }  
}
