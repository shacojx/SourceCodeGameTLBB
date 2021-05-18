#include "WXPropertyInterface.h"

namespace WX
{
    PropertyDictionaryMap PropertyInterface::msDictionary;

    void PropertyInterface::defineProperty(const String& name,
                                           const String& description,
                                           const String& type,
                                           PropertyCommand* propertyCmd,
                                           uint flags)
    {
        getPropertyDictionary()->addProperty(
            PropertyDef(name,
                        description,
                        type,
                        propertyCmd ? propertyCmd->doGetAsString(this) : String(),
                        flags),
            propertyCmd);
    }
    //-----------------------------------------------------------------------
    const PropertyList& PropertyInterface::getProperties(void) const
    {
        const PropertyDictionary* dict = getPropertyDictionary();
        if (dict)
        {
            return dict->getProperties();
        }
        else
        {
            static const PropertyList emptyList;
            return emptyList;
        }
    };
    //-----------------------------------------------------------------------
    bool PropertyInterface::hasProperty(const String& name) const
    {
        // Get dictionary
        const PropertyDictionary* dict = getPropertyDictionary();

        if (dict)
        {
            // Look up command object
            const PropertyCommand* cmd = dict->getPropertyCommand(name);
            if (cmd)
            {
                return true;
            }
        }

        // Fallback
        return !getProperty(name).empty();
    }
    //-----------------------------------------------------------------------
    bool PropertyInterface::setProperty(const String& name, const Variant& value)
    {
        // Get dictionary
        PropertyDictionary* dict = getPropertyDictionary();

        if (dict)
        {
            // Look up command object
            PropertyCommand* cmd = dict->getPropertyCommand(name);
            if (cmd)
            {
                cmd->doSet(this, value);
                return true;
            }
        }

        // Fallback
        return false;
    }
    //-----------------------------------------------------------------------
    bool PropertyInterface::setPropertyAsString(const String& name, const String& value)
    {
        // Get dictionary
        PropertyDictionary* dict = getPropertyDictionary();

        if (dict)
        {
            // Look up command object
            PropertyCommand* cmd = dict->getPropertyCommand(name);
            if (cmd)
            {
                cmd->doSetAsString(this, value);
                return true;
            }
        }

        // Fallback
        return false;
    }
    //-----------------------------------------------------------------------
    void PropertyInterface::setPropertyList(const PropertyNameStringPairList& propertyList)
    {
        PropertyNameStringPairList::const_iterator i;
        for (i = propertyList.begin(); i != propertyList.end(); ++i)
        {
            setPropertyAsString(i->first, i->second);
        }
    }
    //-----------------------------------------------------------------------
    void PropertyInterface::setPropertyList(const PropertyNameVariantPairList& propertyList)
    {
        PropertyNameVariantPairList::const_iterator i;
        for (i = propertyList.begin(); i != propertyList.end(); ++i)
        {
            setProperty(i->first, i->second);
        }
    }
    //-----------------------------------------------------------------------
    Variant PropertyInterface::getProperty(const String& name) const
    {
        // Get dictionary
        const PropertyDictionary* dict = getPropertyDictionary();

        if (dict)
        {
            // Look up command object
            const PropertyCommand* cmd = dict->getPropertyCommand(name);
            if (cmd)
            {
                return cmd->doGet(this);
            }
        }

        // Fallback
        return Variant();
    }
    //-----------------------------------------------------------------------
    String PropertyInterface::getPropertyAsString(const String& name) const
    {
        // Get dictionary
        const PropertyDictionary* dict = getPropertyDictionary();

        if (dict)
        {
            // Look up command object
            const PropertyCommand* cmd = dict->getPropertyCommand(name);
            if (cmd)
            {
                return cmd->doGetAsString(this);
            }
        }

        // Fallback
        return Ogre::StringUtil::BLANK;
    }
    //-----------------------------------------------------------------------
    uint PropertyInterface::getPropertyFlags(const String& name) const
    {
        // Get dictionary
        const PropertyDictionary* dict = getPropertyDictionary();

        if (dict)
        {
            const PropertyDef* def = dict->getPropertyDef(name);
            if (def)
            {
                return def->flags;
            }
        }

        // Fallback
        return 0;
    }
    //-----------------------------------------------------------------------
    void PropertyInterface::copyPropertiesTo(PropertyInterface* dest) const
    {
        // Get dictionary
        const PropertyDictionary* dict = getPropertyDictionary();

        if (dict)
        {
            // Iterate through own properties
            PropertyList::const_iterator i;
            for (i = dict->mPropertyDefs.begin(); i != dict->mPropertyDefs.end(); ++i)
            {
                dest->setProperty(i->name, getProperty(i->name));
            }
        }
    }
    //-----------------------------------------------------------------------
    void PropertyInterface::cleanupDictionary ()
    {
        msDictionary.clear();
    }
}
