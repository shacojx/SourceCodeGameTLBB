#ifndef __FAIRYXMLNODE_H__
#define __FAIRYXMLNODE_H__

#include "WXXMLAttributes.h"
#include <OgreIteratorWrappers.h>

namespace WX
{

    class XMLNode
    {
        typedef std::vector<XMLNode*> XMLNodeVector;
    public:
        typedef Ogre::ConstVectorIterator<XMLNodeVector> ChildrenIterator;

        XMLNode(const String& name);
        ~XMLNode();

        XMLNode* getParent(void) const
        {
            return mParent;
        }

        const String& getName(void) const
        {
            return mName;
        }

        //////////////////////////////////////////////////////////////////////////

        const XMLAttributes& attributes(void) const
        {
            return mAttributes;
        }

        XMLAttributes& attributes(void)
        {
            return mAttributes;
        }

        template <class T>
        typename XMLAttributes::helper<T>::result_type getAttribute(const String& name) const
        {
            return mAttributes.getValueAs<T>(name);
        }

        template <class T>
        typename XMLAttributes::helper<T>::result_type getAttribute(const String& name, typename boost::call_traits<T>::param_type def) const
        {
            return mAttributes.getValueAs<T>(name, def);
        }

        template <class T>
        void setAttribute(const String& name, T val)
        {
            mAttributes.setValueAs(name, val);
        }

        template <class T>
        void setAttribute(const String& name, T val, typename boost::call_traits<T>::param_type def)
        {
            mAttributes.setValueAs(name, val, def);
        }

        void setAttribute(const String& name, const String& val)
        {
            mAttributes.setValueAs(name, val);
        }

        void setAttribute(const String& name, const String& val, const String& def)
        {
            mAttributes.setValueAs(name, val, def);
        }

        //////////////////////////////////////////////////////////////////////////

        void setTextContent(const String& textContent);
        const String& getTextContent(void) const;

        XMLNode* createChild(const String& name);
        void addChild(XMLNode* child);
        void removeChild(size_t index);
        void removeAllChildren(void);
        size_t getChildCount(void) const;
        XMLNode* getChild(size_t index) const;
        ChildrenIterator getChildrenIterator(void) const;

    protected:
        XMLNode* mParent;
        String mName;
        String mTextContent;
        XMLAttributes mAttributes;
        XMLNodeVector mChildren;
    };

}

#endif  // 
