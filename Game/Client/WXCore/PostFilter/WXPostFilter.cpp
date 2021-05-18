#include "WXPostFilter.h"

#include <OgreException.h>

#include <OgreCompositorManager.h>
#include <OgreCompositorInstance.h>

using namespace WX;

PostFilter::PostFilter(const String& name, const String& compositorName)
    : mName(name)
    , mCompositorName(compositorName)
    , mViewport()
    , mCompositorInstance()
{
}

PostFilter::~PostFilter()
{
    if (mCompositorInstance && mViewport)
    {
        Ogre::CompositorManager::getSingleton().removeCompositor(mViewport, mCompositorName);
    }
}

void PostFilter::setEnabled(bool enable)
{
    if (mCompositorInstance)
    {
        mCompositorInstance->setEnabled(enable);
    }
}

bool PostFilter::getEnabled(void) const
{
    return mCompositorInstance && mCompositorInstance->getEnabled();
}

void PostFilter::setParameter(const String& name, const String& value)
{
    OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
        "Parameter '" + name + "' doesn't exists.",
        "PostFilter::setParameter");
}

String PostFilter::getParameter(const String& name) const
{
    OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
        "Parameter '" + name + "' doesn't exists.",
        "PostFilter::getParameter");
}

void PostFilter::_init(Ogre::Viewport* vp)
{
    mViewport = vp;
    mCompositorInstance = Ogre::CompositorManager::getSingleton().addCompositor(vp, mCompositorName);
}

void PostFilter::_notifyViewportSizeChanged(void)
{
    if (mCompositorInstance)
    {
        if (mCompositorInstance->getEnabled())
        {
            mCompositorInstance->setEnabled(false);
            mCompositorInstance->setEnabled(true);
        }
    }
}
