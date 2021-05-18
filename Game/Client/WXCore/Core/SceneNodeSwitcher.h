#ifndef __SceneNodeSwitcher_H__
#define __SceneNodeSwitcher_H__

#include <OgreRenderTargetListener.h>

namespace WX {

class SceneNodeSwitcher : public Ogre::RenderTargetListener
{
protected:
    Ogre::SceneNode* mParent;
    Ogre::SceneNode* mChild;
    Ogre::RenderTarget* mRenderTarget;

public:
    SceneNodeSwitcher(Ogre::SceneNode* parent, Ogre::SceneNode *child, Ogre::RenderTarget* renderTarget);
    ~SceneNodeSwitcher();

    Ogre::SceneNode* getParentNode() const
    {
        return mParent;
    }

    Ogre::SceneNode* getChildNode() const
    {
        return mChild;
    }

    void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
    void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt);
};

}

#endif // 