#include "WXPropertyHelper.h"
#include <OgreString.h>
#include <OgreStringConverter.h>
#include <OgreColourValue.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>

namespace WX
{
    bool mapEnumLabel(const Constant* constants, int value, String& label)
    {
        assert(constants);

        for (const Constant* i = constants; i->label; ++i)
        {
            if (i->value == value)
            {
                label = i->label;
                return true;
            }
        }

        return false;
    }

    bool mapEnumValue(const Constant* constants, const String& label, int& value, bool caseSensitive)
    {
        assert(constants);
        typedef int (*CmpFunc)(const Char*, const Char*);
        CmpFunc cmp = caseSensitive ? strcmp : stricmp;

        for (const Constant* i = constants; i->label; ++i)
        {
            if (cmp(i->label, label.c_str()) == 0)
            {
                value = i->value;
                return true;
            }
        }

        return false;
    }

    bool populateFlagsLabel(const Constant* constants, int value, String& label)
    {
        assert(constants);

        label.clear();
        for (const Constant* i = constants; i->label; ++i)
        {
            if (i->value & value)
            {
                if (!label.empty())
                    label.append(", ");
                label.append(i->label);
            }
        }

        return true;
    }

    bool populateFlagsValue(const Constant* constants, const String& label, int& value, bool caseSensitive)
    {
        assert(constants);
        std::vector<String> items = Ogre::StringUtil::split(label, ",|");

        int flags = 0;
        for (std::vector<String>::iterator i = items.begin(); i != items.end(); ++i)
        {
            String& item = *i;
            Ogre::StringUtil::trim(item);

            int flag;
            if (!mapEnumValue(constants, item, flag, caseSensitive))
                return false;
            flags |= flag;
        }

        value = flags;
        return true;
    }

    static String getLabelList(const Constant* constants)
    {
        String result;
        for (const Constant* i = constants; i->label; ++i)
        {
            if (!result.empty())
                result.append(", ");
            result.append(i->label);
        }
        return result;
    }

    String getEnumTypeString(const String& type, const Constant* constants)
    {
        return "Enum " + type + " ( " + getLabelList(constants) + " )";
    }

    String getFlagsTypeString(const String& type, const Constant* constants)
    {
        return "Flags " + type + " ( " + getLabelList(constants) + " )";
    }

    Ogre::ColourValue String2ColourRGB(const String& val)
    {
        // 如果是由color控件得到的颜色字符串，它的第一个字符为(
        if (val[0] == '(')
        {

            String tempColorStr = val.substr(1,val.length()-2);

            std::vector<String> vec = Ogre::StringUtil::split(tempColorStr, ",");

            assert(vec.size() == 3);

            int r = Ogre::StringConverter::parseInt(vec[0]);
            int g = Ogre::StringConverter::parseInt(vec[1]);
            int b = Ogre::StringConverter::parseInt(vec[2]);

            return Ogre::ColourValue( (float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f );
        }
        else
            return String2ColourARGB(val);
    }

    String ColourRGB2String(const Ogre::ColourValue& val)
    {
        Ogre::StringUtil::StrStreamType stream;
        stream << val.r << " " << val.g << " " << val.b;
        return stream.str();
    }

    Ogre::ColourValue String2ColourARGB(const String& val)
    {
        return Ogre::StringConverter::parseColourValue(val);
    }

    String ColourARGB2String(const Ogre::ColourValue& val)
    {
        return Ogre::StringConverter::toString(val);
    }

    template <>
    extern Ogre::Vector3
    __parseValue<Ogre::Vector3>(const String& s)
    {
        return Ogre::StringConverter::parseVector3(s);
    }

    template <>
    extern Ogre::Quaternion
    __parseValue<Ogre::Quaternion>(const String& s)
    {
        return Ogre::StringConverter::parseQuaternion(s);
    }

}
