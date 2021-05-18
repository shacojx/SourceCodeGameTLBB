#include "WXXMLNode.h"
#include <OgreException.h>

namespace WX
{

    XMLNode::XMLNode(const String& name)
        : mParent(NULL)
        , mName(name)
        , mTextContent()
        , mAttributes()
        , mChildren()
    {
    }

    XMLNode::~XMLNode()
    {
        for (XMLNodeVector::const_iterator it = mChildren.begin(); it != mChildren.end(); ++it)
        {
            XMLNode* child = *it;
            child->mParent = NULL;
            delete child;
        }
    }

    void XMLNode::setTextContent(const String& textContent)
    {
        mTextContent = textContent;
    }

    const String& XMLNode::getTextContent(void) const
    {
        return mTextContent;
    }

    XMLNode* XMLNode::createChild(const String& name)
    {
        std::auto_ptr<XMLNode> child(new XMLNode(name));
        addChild(child.get());
        return child.release();
    }

    void XMLNode::addChild(XMLNode* child)
    {
        assert(child);
        assert(child->getParent() == NULL);
        assert(std::find(mChildren.begin(), mChildren.end(), child) == mChildren.end());
        mChildren.push_back(child);
        child->mParent = this;
    }

    void XMLNode::removeChild(size_t index)
    {
        if (index >= mChildren.size())
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                "The specified index is out of range for this XMLNode",
                "XMLNode::removeChild");
        }

        XMLNodeVector::iterator it = mChildren.begin();
        std::advance(it, index);

        XMLNode* child = *it;
        mChildren.erase(it);
        child->mParent = NULL;
        delete child;
    }

    void XMLNode::removeAllChildren(void)
    {
        for (XMLNodeVector::const_iterator it = mChildren.begin(); it != mChildren.end(); ++it)
        {
            XMLNode* child = *it;
            child->mParent = NULL;
            delete child;
        }
        mChildren.clear();
    }

    size_t XMLNode::getChildCount(void) const
    {
        return mChildren.size();
    }

    XMLNode* XMLNode::getChild(size_t index) const
    {
        if (index >= mChildren.size())
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_ITEM_NOT_FOUND,
                "The specified index is out of range for this XMLNode",
                "XMLNode::getChild");
        }

        XMLNodeVector::const_iterator it = mChildren.begin();
        std::advance(it, index);

        XMLNode* child = *it;
        return child;
    }

    XMLNode::ChildrenIterator XMLNode::getChildrenIterator(void) const
    {
        return ChildrenIterator(mChildren.begin(), mChildren.end());
    }

}
