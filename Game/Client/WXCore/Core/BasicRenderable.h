#ifndef __BasicRenderable_H__
#define __BasicRenderable_H__

#include <OgreRenderable.h>

namespace WX {

//////////////////////////////////////////////////////////////////////////

class BasicRenderable : public Ogre::Renderable
{
protected:
    Ogre::MovableObject* mParent;

public:
    Ogre::MaterialPtr mMaterial;
    Ogre::RenderOperation mRenderOp;

public:
    BasicRenderable(Ogre::MovableObject *parent);
    ~BasicRenderable();

    /** Overridden - see Renderable. */
    const Ogre::MaterialPtr& getMaterial(void) const;

    /** Overridden - see Renderable. */
    void getRenderOperation(Ogre::RenderOperation& op);

    /** Overridden - see Renderable. */
    void getWorldTransforms(Ogre::Matrix4* xform) const;

    /** Overridden - see Renderable. */
    const Ogre::Quaternion& getWorldOrientation(void) const;

    /** Overridden - see Renderable. */
    const Ogre::Vector3& getWorldPosition(void) const;

    /** Overridden - see Renderable. */
    Ogre::Real getSquaredViewDepth(const Ogre::Camera* camra) const;

    /** Overridden - see Renderable. */
    const Ogre::LightList& getLights(void) const;
};

}

#endif // __BasicRenderable_H__
