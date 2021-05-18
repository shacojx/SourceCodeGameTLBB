#ifndef __FAIRYPOSTFILTER_H__
#define __FAIRYPOSTFILTER_H__

#include "WXPostFilterPrerequisites.h"

namespace WX {

class PostFilter
{
public:
    PostFilter(const String& name, const String& compositorName);
    virtual ~PostFilter();

public:

    virtual const String& getType(void) const = 0;

    const String& getName(void) const
    {
        return mName;
    }

    const String& getCompositorName(void) const
    {
        return mCompositorName;
    }

    Ogre::Viewport* getViewport(void) const
    {
        return mViewport;
    }

    Ogre::CompositorInstance* getCompositorInstance(void) const
    {
        return mCompositorInstance;
    }

    void setEnabled(bool enable);
    bool getEnabled(void) const;

    virtual void setParameter(const String& name, const String& value);
    virtual String getParameter(const String& name) const;

    virtual void _init(Ogre::Viewport* vp);

    virtual void _notifyViewportSizeChanged(void);

protected:
    String mName;
    String mCompositorName;
    Ogre::Viewport* mViewport;
    Ogre::CompositorInstance* mCompositorInstance;
};

}

#endif 
