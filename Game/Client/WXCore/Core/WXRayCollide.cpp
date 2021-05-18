/********************************************************************
    filename:   WXRayCollide.cpp
    
    purpose:    Ray collide for pick model exactly
*********************************************************************/

#include "WXRayCollide.h"
#include "WXCollisionModel.h"
#include "WXCollisionModelManager.h"
#include "WXOpcode.h"

#include <OgreRay.h>
#include <OgreMovableObject.h>
#include <OgreEntity.h>
#include "OgreExt/OgreAutoAnimationEntity.h"

namespace WX {

std::pair<bool, Real>
doPicking(const Ogre::Ray& localRay, const CollisionModel& collisionModel, CullingMode cullingMode)
{
    // Convert our ray to Opcode ray
    IceMaths::Ray world_ray(
        IceMaths::Point(localRay.getOrigin().x, localRay.getOrigin().y, localRay.getOrigin().z),
        IceMaths::Point(localRay.getDirection().x, localRay.getDirection().y, localRay.getDirection().z));

    // Be aware we store triangle as ccw in collision mode, and Opcode treat it as cw,
    // so need to inverse cull mode here.
    Opcode::CullMode cullMode;
    switch (cullingMode)
    {
    default:
    case CULL_NONE:
        cullMode = Opcode::CULLMODE_NONE;
        break;
    case CULL_CLOCKWISE:
        cullMode = Opcode::CULLMODE_CCW;
        break;
    case CULL_ANTICLOCKWISE:
        cullMode = Opcode::CULLMODE_CW;
        break;
    }

    // Cull mode callback for Opcode
    struct Local
    {
        static Opcode::CullMode cullModeCallback(udword triangle_index, void* user_data)
        {
            return (Opcode::CullMode) (int) user_data;
        }
    };

    std::pair<bool, Real> ret;

    // Do picking
    Opcode::CollisionFace picked_face;
    ret.first = Opcode::Picking(picked_face,
        world_ray, collisionModel.getOpcodeModel(), 0,
        0, FLT_MAX,
        world_ray.mOrig,
        &Local::cullModeCallback, (void*) (int) cullMode);
    ret.second = ret.first ? picked_face.mDistance : 0;

    return ret;
}

std::pair<bool, Real>
rayCollide(const Ogre::Ray& ray,
           Ogre::MovableObject* movable,
           bool accurate,
           CullingMode cullingMode,
           bool allowAnimable)
{
    // Get local space axis aligned bounding box
    const Ogre::AxisAlignedBox& aabb = movable->getBoundingBox();

    // Matrix4 to transform local space to world space
    const Ogre::Matrix4& localToWorld = movable->_getParentNodeFullTransform();

    // Matrix4 to transform world space to local space
    Ogre::Matrix4 worldToLocal = localToWorld.inverse();

    // Matrix3 to transform world space normal to local space
    Ogre::Matrix3 worldToLocalN;
    worldToLocal.extract3x3Matrix(worldToLocalN);

    // Convert world space ray to local space ray
    // Note:
    //      By preserving the scale between world space and local space of the
    //      direction, we don't need to recalculate the distance later.
    Ogre::Ray localRay;
    localRay.setOrigin(worldToLocal * ray.getOrigin());
    localRay.setDirection(worldToLocalN * ray.getDirection());

    // Intersect with axis aligned bounding box, but because we transformed
    // ray to local space of the bounding box, so this test just like test
    // with oriented bounding box.
    std::pair<bool, Real> ret = localRay.intersects(aabb);

    // Do accurate test if hitted bounding box and user required.
    if (ret.first && accurate)
    {
        if (movable->getMovableType() == Ogre::EntityFactory::FACTORY_TYPE_NAME ||
            allowAnimable && movable->getMovableType() == Ogre::AutoAnimationEntityFactory::FACTORY_TYPE_NAME)
        {
            Ogre::Entity* entity = static_cast<Ogre::Entity*>(movable);
            if (!entity->_isAnimated())
            {
                // Static entity

                // Get the entity mesh
                const Ogre::MeshPtr& mesh = entity->getMesh();

                // Get the collision mode
                CollisionModelPtr collisionModel = CollisionModelManager::getSingleton().getCollisionModel(mesh);

                ret = doPicking(localRay, *collisionModel, cullingMode);
            }
            else if (allowAnimable)
            {
                // Animation entity

                bool addedSoftwareAnimation = false;
                if (entity->getSoftwareAnimationRequests() <= 0)
                {
                    entity->addSoftwareAnimationRequest(false);
                    entity->_updateAnimation();
                    addedSoftwareAnimation = true;
                }

                CollisionModel collisionModel;
                collisionModel.addEntity(entity);
                collisionModel.build(true);

                ret = doPicking(localRay, collisionModel, cullingMode);

                if (addedSoftwareAnimation)
                {
                    entity->removeSoftwareAnimationRequest(false);
                }
            }
        }
    }

    return ret;
}

}
