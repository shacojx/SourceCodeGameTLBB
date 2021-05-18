#ifndef __FAIRYRESOURCES_H__
#define __FAIRYRESOURCES_H__

#include "WXPrerequisites.h"

#include <OgreResource.h>

namespace WX {

extern const String GRAMMAR_RESOURCE_GROUP_NAME;
extern const String EDITOR_RESOURCE_GROUP_NAME;

extern const String SCENE_SCHEMA_NAME;
extern const String TERRAIN_SCHEMA_NAME;

extern const String DEFAULT_RESOURCE_GROUP_NAME;
extern const String BRUSH_RESOURCE_GROUP_NAME;

extern const String AUTO_ANIMATION_NAME;

class FakeManualResourceLoader : public Ogre::ManualResourceLoader
{
public:
    void loadResource(Ogre::Resource* resource)
    {
        // do nothing
    }
};

extern FakeManualResourceLoader gsFakeManualResourceLoader;

extern bool
splitResourceName(const String& name, String& resourceName, String& groupName);

extern bool
findCorrelativeResource(String& resourceName, String& groupName,
                        const String& baseResourceName, const String& baseGroupName);

extern Ogre::ResourcePtr
loadCorrelativeResource(const String& resourceName, const String& groupName,
                        const String& baseResourceName, const String& baseGroupName,
                        Ogre::ResourceManager& resourceManager);

extern Ogre::MeshPtr
loadMesh(const String& meshName, const String& groupName,
                    const String& baseResourceName, const String& baseGroupName);

}

#endif // 
