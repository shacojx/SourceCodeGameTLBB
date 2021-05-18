#ifndef __FrameStatsListener_H__
#define __FrameStatsListener_H__

#include <OgreFrameListener.h>

namespace WX {

class FrameStatsListener : public Ogre::FrameListener
{
protected:
    Ogre::Overlay* mDebugOverlay;
    Ogre::OverlayElement* mStatPanel;
    Ogre::OverlayElement* mLogoPanel;
    Ogre::RenderTarget* mRenderTarget;

protected:

    void updateStats(void);

public:
    // Constructor takes a RenderTarget because it uses that to determine frame stats
    FrameStatsListener(Ogre::RenderTarget* renderTarget);
    ~FrameStatsListener();

    void showDebugOverlay(bool show);
    bool isDebugOverlayShown(void) const;
    void showStatPanel(bool show);
    bool isStatPanelShown(void) const;
    void showLogoPanel(bool show);
    bool isLogoPanelShown(void) const;

    bool frameEnded(const Ogre::FrameEvent& evt);
};

}

#endif // 