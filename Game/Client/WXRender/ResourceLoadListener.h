#pragma once
#include <OgreResourceGroupManager.h>

// ----------------------------------------------------------------------------
// the listener used to found detail error when parsing scripts
// ----------------------------------------------------------------------------

class ScriptParsingListener : public Ogre::ResourceGroupListener
{
public:
    Ogre::String _groupName;
    size_t _scriptCount;
    size_t _scriptIndex;
    Ogre::String _scriptName;

    ScriptParsingListener(VOID)
        : _groupName()
        , _scriptCount()
        , _scriptIndex()
        , _scriptName()
    {
        Ogre::ResourceGroupManager::getSingleton().addResourceGroupListener(this);
    }

    ~ScriptParsingListener()
    {
        Ogre::ResourceGroupManager::getSingleton().removeResourceGroupListener(this);
    }

    VOID resourceGroupScriptingStarted(const Ogre::String& groupName, size_t scriptCount);

    VOID scriptParseStarted(const Ogre::String& scriptName);

    VOID scriptParseEnded(VOID);

    VOID resourceGroupScriptingEnded(const Ogre::String& groupName);

    ////////////////////////////////////////////////////////////////////////////

    VOID resourceGroupLoadStarted(const Ogre::String& groupName, size_t resourceCount)
    {
    }

    VOID resourceLoadStarted(const Ogre::ResourcePtr& resource)
    {
    }

    VOID resourceLoadEnded(VOID)
    {
    }

    VOID worldGeometryStageStarted(const Ogre::String& description)
    {
    }

    VOID worldGeometryStageEnded(VOID)
    {
    }

    VOID resourceGroupLoadEnded(const Ogre::String& groupName)
    {
    }
};


