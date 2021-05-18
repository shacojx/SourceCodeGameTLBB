#ifndef __FAIRYPROPERTYINTERFACE_H__
#define __FAIRYPROPERTYINTERFACE_H__

#include "WXPrerequisites.h"
#include "WXVariant.h"

#include <OgreString.h>

namespace WX
{
    typedef std::map<String, String> PropertyNameStringPairList;
    typedef std::map<String, Variant> PropertyNameVariantPairList;

    enum PropertyFlags
    {
        PF_PRIMARY      = 1 << 0,   /// The property is primary, the value must be set after object created and before any other method being call.
        PF_DERIVED      = 1 << 1,   /// The property is derived, so setting the value of it just ignored, and is will never use to serialization.
        PF_READONLY     = 1 << 2,   /// The property is read only, it a hint for property editor.
    };

    /// Definition of a property supported by a PropertyInterface class, for introspection
    class PropertyDef
    {
    public:
        String name;
        String description;
        String type;
        String defaultValue;
        uint flags;

        PropertyDef(const String& name,
                    const String& description,
                    const String& type,
                    const String& defaultValue = Ogre::StringUtil::BLANK,
                    uint flags = 0)
            : name(name)
            , description(description)
            , type(type)
            , defaultValue(defaultValue)
            , flags(flags)
        {
        }
    };
    typedef std::vector<PropertyDef> PropertyList;

    /** Abstract class which is command object which gets/sets properties.*/
    class PropertyCommand
    {
    public:
        virtual Variant doGet(const void* source) const = 0;
        virtual void doSet(void* target, const Variant& value) = 0;
        virtual String doGetAsString(const void* source) const = 0;
        virtual void doSetAsString(void* target, const String& value) = 0;
    };
    typedef std::map<String, PropertyCommand*> PropertyCommandMap;

    /** Class to hold a dictionary of properties for a single class. */
    class PropertyDictionary
    {
        friend class PropertyInterface;
    protected:
        /// Definitions of properties
        PropertyList mPropertyDefs;

        /// Command objects to get/set
        PropertyCommandMap mPropertyCommands;

        /** Retrieves the property command object for a named property. */
        PropertyCommand* getPropertyCommand(const String& name)
        {
            PropertyCommandMap::iterator i = mPropertyCommands.find(name);
            if (i != mPropertyCommands.end())
                return i->second;
            else
                return 0;
        }

		const PropertyCommand* getPropertyCommand(const String& name) const
        {
            PropertyCommandMap::const_iterator i = mPropertyCommands.find(name);
            if (i != mPropertyCommands.end())
                return i->second;
            else
                return 0;
        }

        const PropertyDef* getPropertyDef(const String& name) const
        {
            for (PropertyList::const_iterator it = mPropertyDefs.begin(); it != mPropertyDefs.end(); ++it)
            {
                const PropertyDef& propertyDef = *it;
                if (propertyDef.name == name)
                    return &propertyDef;
            }
            return 0;
        }

    public:
        /** Method for adding a property definition for this class. 
        @param paramDef A PropertyDef object defining the property
        @param paramCmd Pointer to a PropertyCommand subclass to handle the getting / setting of this property.
            NB this class will not destroy this on shutdown, please ensure you do
        */
        void addProperty(const PropertyDef& propertyDef, PropertyCommand* propertyCmd)
        {
            mPropertyDefs.push_back(propertyDef);
            mPropertyCommands[propertyDef.name] = propertyCmd;
        }

        /** Retrieves a list of properties valid for this object. 
        @returns
            A reference to a static list of PropertyDef objects.
        */
        const PropertyList& getProperties(void) const
        {
            return mPropertyDefs;
        }
    };
    typedef std::map<String, PropertyDictionary> PropertyDictionaryMap;

    /** Class defining the common interface which classes can use to 
        present a reflection-style, self-defining property set to callers.
    @remarks
        This class also holds a static map of class name to property dictionaries
        for each subclass to use. See PropertyDictionary for details. 
    @remarks
        In order to use this class, each subclass must call createPropertyDictionary in their constructors
        which will create a property dictionary for the class if it does not exist yet.
    */
    class PropertyInterface 
    {
    protected:

        /// Dictionary of properties
        static PropertyDictionaryMap msDictionary;

        /// Class name for this instance to be used as a lookup (must be initialised by subclasses)
        String mPropertyDictName;

        /** Internal method for creating a property dictionary for the class, if it does not already exist.
        @remarks
            This method will check to see if a property dictionary exist for this class yet,
            and if not will create one. NB you must supply the name of the class (RTTI is not 
            used or performance).
        @param
            className the name of the class using the dictionary
        @returns
            true if a new dictionary was created, false if it was already there
        */
        bool createPropertyDictionary(const String& className)
        {
            mPropertyDictName = className;
            if (msDictionary.find(className) == msDictionary.end())
            {
                msDictionary[className] = PropertyDictionary();
                return true;
            }
            return false;
        }

        void defineProperty(const String& name,
                            const String& description,
                            const String& type,
                            PropertyCommand* propertyCmd,
                            uint flags = 0);

    public:
        /** Virtual desctructor, see Effective C++ */
        virtual ~PropertyInterface() {}

        /** Retrieves the property dictionary for this class. 
        @remarks
            Only valid to call this after createPropertyDictionary.
        @returns
            Pointer to PropertyDictionary shared by all instances of this class
            which you can add properties to, retrieve properties etc.
        */
        PropertyDictionary* getPropertyDictionary(void)
        {
            PropertyDictionaryMap::iterator i = msDictionary.find(mPropertyDictName);
            if (i != msDictionary.end())
                return &(i->second);
            else
                return 0;
        }

		const PropertyDictionary* getPropertyDictionary(void) const
        {
            PropertyDictionaryMap::const_iterator i = msDictionary.find(mPropertyDictName);
            if (i != msDictionary.end())
                return &(i->second);
            else
                return 0;
        }

        /** Retrieves a list of properties valid for this object. 
        @returns
            A reference to a static list of PropertyDef objects.
        */
        const PropertyList& getProperties(void) const;

        /** Retrieves a property exist or not.
        @returns
            true if property exists, false otherwise.
        */
        virtual bool hasProperty(const String& name) const;

        /** Generic property setting method.
        @remarks
            Call this method with the name of a property and a string version of the value
            to set. The implementor will convert the variant to a native type internally.
            If in doubt, check the property definition in the list returned from 
            PropertyInterface::getProperties.
        @param
            name The name of the property to set
        @param
            value variant value. Must be in the right format for the type specified in the property definition.
        @returns
            true if set was successful, false otherwise (NB no exceptions thrown - tolerant method)
        */
        virtual bool setProperty(const String& name, const Variant& value);

        /** Generic property setting method.
        @remarks
            Call this method with the name of a property and a string version of the value
            to set. The implementor will convert the string to a native type internally.
            If in doubt, check the property definition in the list returned from 
            PropertyInterface::getProperties.
        @param
            name The name of the property to set
        @param
            value String value. Must be in the right format for the type specified in the property definition.
            See the StringConverter class for more information.
        @returns
            true if set was successful, false otherwise (NB no exceptions thrown - tolerant method)
        */
        virtual bool setPropertyAsString(const String& name, const String& value);

        /** Generic multiple property setting method.
        @remarks
            Call this method with a list of name / value pairs
            to set. The implementor will convert the string to a native type internally.
            If in doubt, check the property definition in the list returned from 
            PropertyInterface::getProperties.
        @param
            propertyList Name/value pair list
        */
        virtual void setPropertyList(const PropertyNameVariantPairList& propertyList);

        /** Generic multiple property setting method.
        @remarks
            Call this method with a list of name / value pairs
            to set. The implementor will convert the variant to a native type internally.
            If in doubt, check the property definition in the list returned from 
            PropertyInterface::getProperties.
        @param
            propertyList Name/value pair list
        */
        virtual void setPropertyList(const PropertyNameStringPairList& propertyList);

        /** Generic property retrieval method.
        @remarks
            Call this method with the name of a property to retrieve a variant value of
            the property in question. If in doubt, check the property definition in the
            list returned from getProperties for the type of this property. If you
            like you can use VariantCast to convert this variant back into a native type.
        @param
            name The name of the property to get
        @returns
            variant value of property, empty if not found
        */
        virtual Variant getProperty(const String& name) const;

        /** Generic property retrieval method.
        @remarks
            Call this method with the name of a property to retrieve a string-format value of
            the property in question. If in doubt, check the property definition in the
            list returned from getProperties for the type of this property. If you
            like you can use StringConverter to convert this string back into a native type.
        @param
            name The name of the property to get
        @returns
            String value of property, blank if not found
        */
        virtual String getPropertyAsString(const String& name) const;

        /** Generic property applicable check.
        @remarks
            Call this method with the name of a property to retrieve a bit masks value of
            the property flags. If in doubt, check the property definition in the list
            returned from getProperties for the type of this property. The property flags
            declare how to use, display, edit and serialization the value of the property.
        @param
            name The name of the property to get
        @returns
            The bit masks of PropertyFlags, zero if not found
        */
        virtual uint getPropertyFlags(const String& name) const;

        /** Method for copying this object's properties to another object.
        @remarks
            This method takes the values of all the object's properties and tries to set the
            same values on the destination object. This provides a completely type independent
            way to copy properties to other objects. Note that because of the String manipulation 
            involved, this should not be regarded as an efficient process and should be saved for
            times outside of the rendering loop.
        @par
            Any unrecognised properties will be ignored as with setProperty method.
        @param dest Pointer to object to have it's properties set the same as this object.

        */
        virtual void copyPropertiesTo(PropertyInterface* dest) const;

        /** Cleans up the static 'msDictionary' required to reset ,
        otherwise the containers are left with invalid pointers.
        */
        static void cleanupDictionary() ;
    };
}

#endif __FairyPropertyInterface_H__
