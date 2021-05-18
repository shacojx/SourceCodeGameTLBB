#include "FrameStatsListener.h"

#include <OgreOverlayManager.h>
#include <OgreRenderTarget.h>
#include <OgreOverlayElement.h>
#include <OgreStringConverter.h>
#include <OgreRoot.h>

namespace WX {

FrameStatsListener::FrameStatsListener(Ogre::RenderTarget* renderTarget)
    : mDebugOverlay(NULL)
    , mStatPanel(NULL)
    , mLogoPanel(NULL)
    , mRenderTarget(renderTarget)
{
    mDebugOverlay = Ogre::OverlayManager::getSingleton().getByName("Core/DebugOverlay");

    try
    {
        mStatPanel = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/StatPanel");
    }
    catch (...)
    {
        // ignore
    }
    try
    {
        mLogoPanel = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/LogoPanel");
    }
    catch (...)
    {
        // ignore
    }

    showDebugOverlay(true);
    Ogre::Root::getSingleton().addFrameListener(this);
}

FrameStatsListener::~FrameStatsListener()
{
    Ogre::Root::getSingleton().removeFrameListener(this);
}

void
FrameStatsListener::updateStats(void)
{
    static Ogre::String currFps = "Current FPS: ";
    static Ogre::String avgFps = "Average FPS: ";
    static Ogre::String bestFps = "Best FPS: ";
    static Ogre::String worstFps = "Worst FPS: ";
    static Ogre::String tris = "Triangle Count: ";

    // update stats when necessary
    try {
        Ogre::OverlayElement* guiAvg = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
        Ogre::OverlayElement* guiCurr = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
        Ogre::OverlayElement* guiBest = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
        Ogre::OverlayElement* guiWorst = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");

        const Ogre::RenderTarget::FrameStats& stats = mRenderTarget->getStatistics();

        guiAvg->setCaption(avgFps + Ogre::StringConverter::toString(stats.avgFPS));
        guiCurr->setCaption(currFps + Ogre::StringConverter::toString(stats.lastFPS));
        guiBest->setCaption(bestFps + Ogre::StringConverter::toString(stats.bestFPS)
            + " " + Ogre::StringConverter::toString(stats.bestFrameTime) + " ms");
        guiWorst->setCaption(worstFps + Ogre::StringConverter::toString(stats.worstFPS)
            + " " + Ogre::StringConverter::toString(stats.worstFrameTime) + " ms");

        Ogre::OverlayElement* guiTris = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
        guiTris->setCaption(tris + Ogre::StringConverter::toString(stats.triangleCount));

        Ogre::OverlayElement* guiDbg = Ogre::OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
        //guiDbg->setCaption(mRenderTarget->getDebugText());
    }
    catch (...)
    {
        // ignore
    }
}

void
FrameStatsListener::showDebugOverlay(bool show)
{
    if (mDebugOverlay)
    {
        if (show)
        {
            mDebugOverlay->show();
        }
        else
        {
            mDebugOverlay->hide();
        }
    }
}

bool
FrameStatsListener::isDebugOverlayShown(void) const
{
    return mDebugOverlay && mDebugOverlay->isVisible();
}

void
FrameStatsListener::showStatPanel(bool show)
{
    if (mStatPanel)
    {
        if (show)
        {
            mStatPanel->show();
        }
        else
        {
            mStatPanel->hide();
        }
    }
}

bool
FrameStatsListener::isStatPanelShown(void) const
{
    return mStatPanel && mStatPanel->isVisible();
}

void
FrameStatsListener::showLogoPanel(bool show)
{
    if (mLogoPanel)
    {
        if (show)
        {
            mLogoPanel->show();
        }
        else
        {
            mLogoPanel->hide();
        }
    }
}

bool
FrameStatsListener::isLogoPanelShown(void) const
{
    return mLogoPanel && mLogoPanel->isVisible();
}

bool
FrameStatsListener::frameEnded(const Ogre::FrameEvent& evt)
{
    if (isStatPanelShown() || isLogoPanelShown())
    {
        updateStats();
    }

    return true;
}

}
