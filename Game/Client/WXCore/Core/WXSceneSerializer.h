#ifndef __FAIRYSCENESERIALIZER_H__
#define __FAIRYSCENESERIALIZER_H__

#include "WXPrerequisites.h"

namespace WX
{
    class Scene;
    class XMLParser;
    class XMLWriter;

    class SceneSerializer
    {
    public:
        SceneSerializer(void);
        virtual ~SceneSerializer();

        virtual void load(Scene* scene, XMLParser* parser, const String& filename, const String& groupName);
        virtual void save(Scene* scene, XMLWriter* writer, const String& filename);
    };
}

#endif // 
