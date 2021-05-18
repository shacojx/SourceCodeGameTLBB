#ifndef __FAIRYSCENE_H__
#define __FAIRYSCENE_H__

#include "WXObject.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/key_extractors.hpp>

namespace WX
{
    struct by_sequenced;
    struct by_identity;
    struct by_category;
    struct by_type;
    struct by_name;

    // Type hiding technique to reduction of symbol name lengths

    struct ObjectSetIndices :
        boost::multi_index::indexed_by<
            boost::multi_index::sequenced<
                boost::multi_index::tag<by_sequenced>
            >,
            boost::multi_index::ordered_unique<
                boost::multi_index::tag<by_identity>,
                boost::multi_index::identity<ObjectPtr>
            >,
            boost::multi_index::ordered_non_unique<
                boost::multi_index::tag<by_category>,
                BOOST_MULTI_INDEX_CONST_MEM_FUN(Object, const String&, getCategory)
            >,
            boost::multi_index::ordered_non_unique<
                boost::multi_index::tag<by_type>,
                BOOST_MULTI_INDEX_CONST_MEM_FUN(Object, const String&, getType)
            >,
            boost::multi_index::ordered_unique<
                boost::multi_index::tag<by_name>,
                BOOST_MULTI_INDEX_MEMBER(Object, String, mName)
            >
        >
    {};

    class Scene
    {
    public:
        String mName;
        String mAuthor;
        String mCopyright;
        String mDescription;
        String mTerrainFilename;

        struct Objects :
            boost::multi_index::multi_index_container<
                ObjectPtr,
                ObjectSetIndices
            >
        {};

        typedef std::map<String, String> Attributes;

    protected:
        Objects mObjects;
        Attributes mAttributes;

        String mAutoNamePrefix;
        size_t mAutoNameIndex;

    public:
        static const String DEFAULT_AUTO_NAME_PREFIX;

        typedef Objects::index<by_type>::type::iterator ObjectsByTypeIterator;
        typedef std::pair<ObjectsByTypeIterator, ObjectsByTypeIterator> ObjectsByTypeRange;
        typedef Objects::index<by_category>::type::iterator ObjectsByCategoryIterator;
        typedef std::pair<ObjectsByCategoryIterator, ObjectsByCategoryIterator> ObjectsByCategoryRange;

        Scene();
        ~Scene();

        void setAutoNamePrefix(const String& newAutoNamePrefix);
        const String& getAutoNamePrefix(void) const         { return mAutoNamePrefix; }

        String generateAutoName(const ObjectPtr &object);
        bool isAutoName(const String &name) const;

        void reset();

        const Objects& getObjects(void) const
        {
            return mObjects;
        }

        void addObject(const ObjectPtr& object);
        void removeObject(const String& name);
        void removeObject(const ObjectPtr& object);
        void renameObject(const String& oldName, const String& newName);
        void renameObject(const ObjectPtr& object, const String& newName);

        const ObjectPtr& findObjectByName(const String& name) const;
        ObjectsByTypeRange findObjectsByType(const String& type) const;
        ObjectsByCategoryRange findObjectsByCategory(const String& category) const;

        void broadcastMessage(const String& msg, const Variant& params);

        const Attributes& getAttributes(void) const
        {
            return mAttributes;
        }

        bool existAttribute(const String& name);
        const String& getAttribute(const String& name) const;
        void setAttribute(const String& name, const String& value);
        void removeAttribute(const String& name);
    };
}

#endif 
