#include "IntersectPoint.h"
#include "SceneManipulator.h"

#include "Core/WXUtils.h"

#include <OgreSceneManager.h>
#include <OgreMesh.h>
#include <OgreEntity.h>

namespace WX {

    IntersectPoint::IntersectPoint(SceneManipulator* sceneManipulator)
        : HitIndicator(sceneManipulator)
        , mIntersectNode(NULL)
        , mIntersectEntity(NULL)
    {
        // Create intersection node and entity

        mIntersectNode = getIndicatorRootSceneNode()->createChildSceneNode();

        Real radius = 1;
        int rings = 16;
        int segments = 16;
        Ogre::MeshPtr sphereMesh = createCommonSphere(radius, rings, segments);
        Ogre::MaterialPtr material = createColourMaterial(
            Ogre::ColourValue(1, 0, 0, 0.75),
            Ogre::ColourValue(0, 1, 0),
            1);

        mIntersectEntity = getSceneManager()->createEntity(mIntersectNode->getName(), sphereMesh->getName());
        mIntersectEntity->setQueryFlags(0);
        mIntersectEntity->setNormaliseNormals(true);
        mIntersectEntity->setMaterialName(material->getName());
        mIntersectEntity->setRenderQueueGroup(Ogre::RENDER_QUEUE_7);
        mIntersectNode->attachObject(mIntersectEntity);
        mIntersectNode->setVisible(false);

        reshape();
    }

    IntersectPoint::~IntersectPoint()
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

    const String& IntersectPoint::getName(void) const
    {
        static const String name = "IntersectPoint";
        return name;
    }

    void IntersectPoint::hide(void)
    {
        mIntersectNode->setVisible(false);
    }

    void IntersectPoint::_setHitPoint(Real x, Real y)
    {
        Ogre::Vector3 position;
        bool hit = getSceneManipulator()->getTerrainIntersects(x, y, position);
        mIntersectNode->setVisible(hit);
        if (hit)
        {
            mIntersectNode->setPosition(position);
        }
    }

    void IntersectPoint::reshape(void)
    {
        // Adjust intersection scale
        Real scale = getSceneManipulator()->getBaseScale() / (5 * mIntersectEntity->getMesh()->getBoundingSphereRadius());
        mIntersectNode->setScale(scale, scale, scale);
    }


	// 设置碰撞小球的材质.
	void IntersectPoint::SetMaterial(String& strMaterialName)
	{
		mIntersectEntity->setMaterialName(strMaterialName);
	}

	// 设置碰撞小球原来的材质.
	void IntersectPoint::SetMaterial()
	{
		 Ogre::MaterialPtr material = createColourMaterial(
            Ogre::ColourValue(1, 0, 0, 0.75),
            Ogre::ColourValue(0, 1, 0),
            1);

		 mIntersectEntity->setMaterialName(material->getName());
	}

	
}
