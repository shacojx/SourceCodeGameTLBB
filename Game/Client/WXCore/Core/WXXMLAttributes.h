#ifndef __FAIRYXMLATTRIBUTES_H__
#define __FAIRYXMLATTRIBUTES_H__

#include "WXPrerequisites.h"
#include <boost/lexical_cast.hpp>
#include <boost/call_traits.hpp>
#include <boost/throw_exception.hpp>

namespace WX
{

    /*!
    \brief
        Class representing a block of attributes associated with an XML element.
     */
    class XMLAttributes
    {
    public:
        /*!
        \brief
            XMLAttributes constructor.
         */
        XMLAttributes(void);

        /*!
        \brief
            XMLAttributes Destructor
         */
        ~XMLAttributes();

        /*!
        \brief
            Sets an attribute to the attribute block.  If the attribute value already exists, it is replaced with
            the new value.

        \param name
            String object holding the name of the attribute to be added.

        \param value
            String object holding a string representation of the attribute value.

        \return
            Nothing.
         */
        void setValue(const String& name, const String& value);

        /*!
        \brief
            Removes an attribute from the attribute block.

        \param name
            String object holding the name of the attribute to be removed.

        \return
            Nothing.
         */
        void remove(const String& name);

        /*!
        \brief
            Removes all attributes from the attribute block.

        \return
            Nothing.
         */
        void removeAll(void);

        /*!
        \brief
            Return whether the named attribute exists within the attribute block.

        \param name
            String object holding the name of the attribute to be checked.

        \return
            - true if an attribute with the name \a name is present in the attribute block.
            - false if no attribute named \a name is present in the attribute block.
        */
        bool exists(const String& name) const;

        /*!
        \brief
            Return the number of attributes in the attribute block.

        \return
            value specifying the number of attributes in this attribute block.
         */
        size_t getCount(void) const;

        /*!
        \brief
            Return the name of an attribute based upon its index within the attribute block.

        \note
            Nothing is specified about the order of elements within the attribute block.  Elements
            may not, for example, appear in the order they were specified in the XML file.

        \param index
            zero based index of the attribute whos name is to be returned.

        \return
            String object holding the name of the attribute at the requested index.

        \exception Ogre::Exception::ERR_ITEM_NOT_FOUND  thrown if \a index is out of range for this attribute block.
        */
        const String& getName(size_t index) const;

        /*!
        \brief
            Return the value string of an attribute based upon its index within the attribute block.

        \note
            Nothing is specified about the order of elements within the attribute block.  Elements
            may not, for example, appear in the order they were specified in the XML file.

        \param index
            zero based index of the attribute whos value string is to be returned.

        \return
            String object holding the string value of the attribute at the requested index.

        \exception Ogre::Exception::ERR_ITEM_NOT_FOUND  thrown if \a index is out of range for this attribute block.
        */
        const String& getValue(size_t index) const;

        /*!
        \brief
            Return the value string for attribute \a name.

        \param name
            String object holding the name of the attribute whos value string is to be returned

        \return
            String object hilding the value string for attribute \a name.

        \exception Ogre::Exception::ERR_ITEM_NOT_FOUND   thrown if no attribute named \a name is present in the attribute block.
         */
        const String& getValue(const String& name) const;

        //////////////////////////////////////////////////////////////////////////

        template <typename T>
        struct helper
        {
            typedef typename boost::call_traits<T>::value_type result_type;

            static result_type convert(const String& val)
            {
                return boost::lexical_cast<result_type>(val);
            }
        };

        template <>
        struct helper<String>
        {
            typedef const String& result_type;

            static result_type convert(const String& val)
            {
                return val;
            }
        };

        template <>
        struct helper<bool>
        {
            typedef bool result_type;

            static result_type convert(const String& val)
            {
                if (val == "false" || val == "no" || val == "0")
                {
                    return false;
                }
                else if (val == "true" || val == "yes" || val == "1")
                {
                    return true;
                }
                else
                {
                    boost::throw_exception(boost::bad_lexical_cast(typeid(result_type), typeid(const String&)));
                    return false;
                }
            }
        };

        //////////////////////////////////////////////////////////////////////////

        template <class T>
        typename helper<T>::result_type getValueAs(const String& name) const
        {
            return helper<T>::convert(getValue(name));
        }

        template <class T>
        typename helper<T>::result_type getValueAs(const String& name, typename boost::call_traits<T>::param_type def) const
        {
            if (!exists(name))
                return def;

            return getValueAs<T>(name);
        }

        //////////////////////////////////////////////////////////////////////////

        template <class T>
        void setValueAs(const String& name, T val)
        {
            setValue(name, boost::lexical_cast<String>(val));
        }

        template <class T>
        void setValueAs(const String& name, T val, typename boost::call_traits<T>::param_type def)
        {
            if (val != def)
            {
                setValueAs(name, val);
            }
        }

        void setValueAs(const String& name, bool val)
        {
            static const String true_value = "true";
            static const String false_value = "false";
            setValue(name, val ? true_value : false_value);
        }

        void setValueAs(const String& name, const String& val)
        {
            setValue(name, val);
        }

        void setValueAs(const String& name, const String& val, const String& def)
        {
            if (val != def)
            {
                setValueAs(name, val);
            }
        }

        //////////////////////////////////////////////////////////////////////////

    protected:
        typedef std::map<String, String> AttributeMap;
        AttributeMap mAttributes;
    };

}

#endif  // 
