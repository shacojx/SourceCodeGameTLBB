#include "BasicRenderable.h"

#include <OgreNode.h>
#include <OgreSceneNode.h>
#include <OgreMaterial.h>
#include <OgreMaterialManager.h>
#include <OgreMovableObject.h>

//////////////////////////////////////////////////////////////////////////
namespace WX {

BasicRenderable::BasicRenderable(Ogre::MovableObject *parent)
    : Renderable()
    , mParent(parent)
    , mRenderOp()
    , mMaterial()
{
}
//-----------------------------------------------------------------------
BasicRenderable::~BasicRenderable()
{
}
//-----------------------------------------------------------------------
#if 0
void
BasicRenderable::setMaterial(const Ogre::String& matName)
{
    m_strMatName = matName;
    mMaterial = static_cast<Ogre::Material*>(
        Ogre::MaterialManager::getSingleton().getByName(m_strMatName));
    if (!mMaterial)
        OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND, "Could not find material " + m_strMatName,
            "BasicRenderable::setMaterial");

    // Won't load twice anyway
    mMaterial->load();
}
//-----------------------------------------------------------------------
void
BasicRenderable::setRenderOperation(const Ogre::RenderOperation& op)
{
    mRenderOp = op;
}
#endif
//-----------------------------------------------------------------------
const Ogre::MaterialPtr&
BasicRenderable::getMaterial(void) const
{
    return mMaterial;
}
//-----------------------------------------------------------------------
void
BasicRenderable::getRenderOperation(Ogre::RenderOperation& op)
{
    op = mRenderOp;
}
//-----------------------------------------------------------------------
void
BasicRenderable::getWorldTransforms(Ogre::Matrix4* xform) const
{
    *xform = mParent->_getParentNodeFullTransform();
}
//-----------------------------------------------------------------------
const Ogre::Quaternion&
BasicRenderable::getWorldOrientation(void) const
{
    Ogre::Node* n = mParent->getParentNode();
    assert(n);
    return n->_getDerivedOrientation();
}
//-----------------------------------------------------------------------
const Ogre::Vector3&
BasicRenderable::getWorldPosition(void) const
{
    Ogre::Node* n = mParent->getParentNode();
    assert(n);
    return n->_getDerivedPosition();
}
//-----------------------------------------------------------------------
Ogre::Real
BasicRenderable::getSquaredViewDepth(const Ogre::Camera* camera) const
{
    Ogre::Node* n = mParent->getParentNode();
    assert(n);
    return n->getSquaredViewDepth(camera);
}
//-----------------------------------------------------------------------
const Ogre::LightList&
BasicRenderable::getLights(void) const
{
    return mParent->queryLights();
}

}
