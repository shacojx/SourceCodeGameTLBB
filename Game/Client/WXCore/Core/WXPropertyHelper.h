#ifndef __FAIRYPROPERTYHELPER_H__
#define __FAIRYPROPERTYHELPER_H__

#include "WXPrerequisites.h"
#include "WXPropertyInterface.h"

#include <OgreStringConverter.h>

#include <boost/lexical_cast.hpp>

namespace WX
{
    typedef String::value_type Char;

    struct Constant
    {
        const Char* label;
        int value;
    };

    bool mapEnumLabel(const Constant* constants, int value, String& label);
    bool mapEnumValue(const Constant* constants, const String& label, int& value, bool caseSensitive = true);

    bool populateFlagsLabel(const Constant* constants, int value, String& label);
    bool populateFlagsValue(const Constant* constants, const String& label, int& value, bool caseSensitive = true);

    String getEnumTypeString(const String& type, const Constant* constants);
    String getFlagsTypeString(const String& type, const Constant* constants);

    //////////////////////////////////////////////////////////////////////////

    Ogre::ColourValue String2ColourRGB(const String& val);
    String ColourRGB2String(const Ogre::ColourValue& val);

    Ogre::ColourValue String2ColourARGB(const String& val);
    String ColourARGB2String(const Ogre::ColourValue& val);

    //////////////////////////////////////////////////////////////////////////

    template <class T>
    inline String
    __toString(const T& v)
    {
        return Ogre::StringConverter::toString(v);
    }

    template <>
    inline String
    __toString<String>(const String& v)
    {
        return v;
    }

    //////////////////////////////////////////////////////////////////////////

    template <class T>
    inline T
    __parseValue(const String& s)
    {
        return boost::lexical_cast<T>(s);
    }

    template <>
    inline String
    __parseValue<String>(const String& s)
    {
        return s;
    }

    template <>
    inline bool
    __parseValue<bool>(const String& s)
    {
        return Ogre::StringConverter::parseBool(s);
    }

    template <>
    extern Ogre::Vector3
    __parseValue<Ogre::Vector3>(const String& s);

    template <>
    extern Ogre::Quaternion
    __parseValue<Ogre::Quaternion>(const String& s);

    //////////////////////////////////////////////////////////////////////////

    template <class Class, typename Type, Type Class::*PtrToMember>
    struct ObjectMemberPropertyCommand : PropertyCommand
    {
        Variant doGet(const void* source) const
        {
            const Class* object = static_cast<const Class*>(source);
            return object->*PtrToMember;
        }

        String doGetAsString(const void* source) const
        {
            const Class* object = static_cast<const Class*>(source);
            return __toString(object->*PtrToMember);
        }

        void doSet(void* target, const Variant& value)
        {
            Class* object = static_cast<Class*>(target);
            object->*PtrToMember = VariantCast<Type>(value);
            object->updateRenderInstance();
        }

        void doSetAsString(void* target, const String& value)
        {
            Class* object = static_cast<Class*>(target);
            object->*PtrToMember = __parseValue<Type>(value);
            object->updateRenderInstance();
        }
    };

}

#endif 
