#include "WXXMLAttributes.h"
#include <OgreException.h>


namespace WX
{

    XMLAttributes::XMLAttributes(void)
    {
    }
    //-----------------------------------------------------------------------
    XMLAttributes::~XMLAttributes()
    {
    }
    //-----------------------------------------------------------------------
    void XMLAttributes::setValue(const String& name, const String& value)
    {
        std::pair<AttributeMap::iterator, bool> inserted =
            mAttributes.insert(AttributeMap::value_type(name, value));

        if (!inserted.second)
        {
            inserted.first->second = value;
        }
    }
    //-----------------------------------------------------------------------
    void XMLAttributes::remove(const String& name)
    {
        AttributeMap::iterator it = mAttributes.find(name);

        if (it == mAttributes.end())
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                "The specified attribute named '" + name + "' doesn't exists for this XMLAttributes block",
                "XMLAttributes::remove");
        }

        mAttributes.erase(it);
    }
    //-----------------------------------------------------------------------
    void XMLAttributes::removeAll(void)
    {
        mAttributes.clear();
    }
    //-----------------------------------------------------------------------
    bool XMLAttributes::exists(const String& name) const
    {
        return mAttributes.find(name) != mAttributes.end();
    }
    //-----------------------------------------------------------------------
    size_t XMLAttributes::getCount(void) const
    {
        return mAttributes.size();
    }
    //-----------------------------------------------------------------------
    const String& XMLAttributes::getName(size_t index) const
    {
        if (index >= mAttributes.size())
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                "The specified index is out of range for this XMLAttributes block",
                "XMLAttributes::getName");
        }

        AttributeMap::const_iterator it = mAttributes.begin();
        std::advance(it, index);

        return it->first;
    }
    //-----------------------------------------------------------------------
    const String& XMLAttributes::getValue(size_t index) const
    {
        if (index >= mAttributes.size())
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                "The specified index is out of range for this XMLAttributes block",
                "XMLAttributes::getValue");
        }

        AttributeMap::const_iterator it = mAttributes.begin();
        std::advance(it, index);

        return it->second;
    }
    //-----------------------------------------------------------------------
    const String& XMLAttributes::getValue(const String& name) const
    {
        AttributeMap::const_iterator it = mAttributes.find(name);

        if (it == mAttributes.end())
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                "No value exists for an attribute named '" + name + "'",
                "XMLAttributes::getValue");
        }

        return it->second;
    }

}
