#include "SceneNodeSwitcher.h"
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>
#include <OgreRenderTarget.h>

namespace WX {

SceneNodeSwitcher::SceneNodeSwitcher(Ogre::SceneNode* parent, Ogre::SceneNode *child, Ogre::RenderTarget* renderTarget)
    : mParent(parent)
    , mChild(child)
    , mRenderTarget(renderTarget)
{
    assert(mParent);
    assert(mChild);
    assert(!mChild->getParent());
    assert(mRenderTarget);
    mRenderTarget->addListener(this);
}

SceneNodeSwitcher::~SceneNodeSwitcher()
{
    mRenderTarget->removeListener(this);
}

void
SceneNodeSwitcher::preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
{
    mParent->getCreator()->setSpecialCaseRenderQueueMode(Ogre::SceneManager::SCRQM_INCLUDE);
    mParent->addChild(mChild);
}

void
SceneNodeSwitcher::postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
{
    mParent->removeChild(mChild);
    mParent->getCreator()->setSpecialCaseRenderQueueMode(Ogre::SceneManager::SCRQM_EXCLUDE);
}

}
