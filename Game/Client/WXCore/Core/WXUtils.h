#ifndef __FAIRYUTILS_H__
#define __FAIRYUTILS_H__

#include "WXPrerequisites.h"

#include <OgreAnimationState.h>

namespace WX {

Ogre::MeshPtr
createSphere(const String& name,
             const Real radius = 1, const int rings = 16, const int segments = 16,
             const bool normals = true,
             const int numTexCoordSets = 1,
             const bool inwards = false);

Ogre::MeshPtr
createCommonSphere(const String& namePrefix,
                   const Real radius = 1, const int rings = 16, const int segments = 16,
                   const bool normals = true,
                   const int numTexCoordSets = 1,
                   const bool inwards = false);

Ogre::MeshPtr
createCommonSphere(const Real radius = 1, const int rings = 16, const int segments = 16,
                   const bool normals = true,
                   const int numTexCoordSets = 1,
                   const bool inwards = false);

Ogre::MaterialPtr
createPureColourMaterial(const Ogre::ColourValue& colour);

Ogre::MaterialPtr
createColourMaterial(const Ogre::ColourValue& colour);

Ogre::MaterialPtr
createColourMaterial(const Ogre::ColourValue& colour, const Ogre::ColourValue& specular, Real shininess);

//////////////////////////////////////////////////////////////////////////

Ogre::MaterialPtr
createProjectiveMaterial(const String& materialTemplateName, const Ogre::Frustum* frustum);

void
fixupProjectiveMaterial(const Ogre::MaterialPtr& material, const Ogre::Frustum* frustum);

//////////////////////////////////////////////////////////////////////////

typedef std::list<Ogre::MovableObject*> ObjectList;

extern void
findAllAttachedObjects(Ogre::SceneNode* sn, ObjectList& objects);

extern void
destroyObject(Ogre::SceneManager* creator, Ogre::MovableObject* object);

extern void
destroyObjects(Ogre::SceneManager* creator, const ObjectList& objects);

extern void
cleanupSceneNode(Ogre::SceneNode* sn);

extern void
cleanupAndDestroySceneNode(Ogre::SceneNode* sn);

//////////////////////////////////////////////////////////////////////////

extern Ogre::AnimationState*
getAnimationStateSafely(Ogre::AnimationStateSet* animationStateSet, const String& name);

extern Ogre::AnimationState*
getAnimationStateSafely(Ogre::Entity* entity, const String& name);

extern Ogre::Animation*
getAnimationSafely(const Ogre::Skeleton* skeleton, const String& name);

extern Ogre::Animation*
getAnimationSafely(const Ogre::SkeletonPtr& skeleton, const String& name);

extern Ogre::Animation*
getAnimationSafely(const Ogre::Mesh* mesh, const String& name);

extern Ogre::Animation*
getAnimationSafely(const Ogre::MeshPtr& mesh, const String& name);

//////////////////////////////////////////////////////////////////////////

// Returns transformed bounding box which will skinning by the given skeleton.
extern Ogre::AxisAlignedBox
skinningTransformBoundingBox(const Ogre::AxisAlignedBox& boundingBox, const Ogre::Skeleton* skeleton);

// Returns transformed bounding box which will skinning by the given skeleton.
extern Ogre::AxisAlignedBox
skinningTransformBoundingBox(const Ogre::AxisAlignedBox& boundingBox, const Ogre::SkeletonPtr& skeleton);

// Returns a bounding box that includes all possible positions of bones in the given skeleton.
extern Ogre::AxisAlignedBox
getSkeletonBoundingBox(const Ogre::Skeleton* skeleton);

// Returns a bounding box that includes all possible positions of bones in the given skeleton.
extern Ogre::AxisAlignedBox
getSkeletonBoundingBox(const Ogre::SkeletonPtr& skeleton);

//////////////////////////////////////////////////////////////////////////

extern Ogre::MeshPtr
createNullMeshForSkeleton(const String& meshName, const Ogre::SkeletonPtr& skeleton);

extern Ogre::MeshPtr
createNullMeshForSkeleton(const Ogre::SkeletonPtr& skeleton);

//////////////////////////////////////////////////////////////////////////

extern bool
convertEntityToHardwareSkinning(Ogre::Entity* entity);

}

#endif 
