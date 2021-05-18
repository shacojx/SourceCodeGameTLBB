#ifndef __FAIRYPOSTFILTERMANAGER_H__
#define __FAIRYPOSTFILTERMANAGER_H__

#include "WXPostFilterPrerequisites.h"

namespace WX {

// This should be an Singleton, but you
// need to take into account its destruction, whereas
// the singleton pattern can let's you forget about this
// So, consider singleton-ize it in your framework
class PostFilterManager
{
public:
    PostFilterManager(void);
    ~PostFilterManager();

public:
    void _notifyViewportSizeChanged(Ogre::Viewport* vp);

    PostFilter* createPostFilter(const String& name, const String& type, Ogre::Viewport* vp);
    PostFilter* createPostFilter(const String& type, Ogre::Viewport* vp);
    PostFilter* getPostFilter(const String& name) const;
    bool hasPostFilter(const String& name) const;
    void destroyPostFilter(PostFilter* filter);
    void destroyPostFilter(const String& name);
    void destroyAllPostFilter(void);

    void setPostFilterParameter(const String& filterName, const String& parameterName, const String& value);
    String getPostFilterParameter(const String& filterName, const String& parameterName);

    void setPostFilterEnabled(const String& filterName, bool enabled);
    bool getPostFilterEnabled(const String& filterName) const;

    typedef std::map<String, PostFilter*> PostFilterList;

    const PostFilterList& getPostFilters(void) const
    {
        return mPostFilters;
    }

    void registerFactory(PostFilterFactory* factory);
    PostFilterFactory* getFactory(const String& type) const;

    typedef std::map<String, PostFilterFactory*> PostFilterFactoryList;

    const PostFilterFactoryList& getFactoryList(void) const
    {
        return mPostFilterFactories;
    }

private:
    PostFilterFactoryList mPostFilterFactories;
    PostFilterList mPostFilters;
};

}

#endif 
