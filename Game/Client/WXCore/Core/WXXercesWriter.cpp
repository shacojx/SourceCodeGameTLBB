#include "WXXercesWriter.h"
#include "WXXMLNode.h"
#include "WXXMLAttributes.h"

#include <OgreException.h>

// Xerces-C includes
#include <OgreNoMemoryMacros.h>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMDocumentType.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMAttr.hpp>
#include <xercesc/dom/DOMWriter.hpp>
#include <OgreMemoryMacros.h>


namespace WX
{
    ////////////////////////////////////////////////////////////////////////////////
    //
    // Helper routines
    //
    ////////////////////////////////////////////////////////////////////////////////

    static void
    populateDOMAttributes(const XMLAttributes& src, XERCES_CPP_NAMESPACE::DOMElement* dest)
    {
        for (size_t i = 0; i < src.getCount(); ++i)
        {
            dest->setAttribute(XercesHelper::transcode(src.getName(i)).c_str(),
                               XercesHelper::transcode(src.getValue(i)).c_str());
        }
    }
    //-----------------------------------------------------------------------
    static void
    populateDOMElement(const XMLNode* src, XERCES_CPP_NAMESPACE::DOMElement* dest)
    {
        XERCES_CPP_NAMESPACE_USE;

        populateDOMAttributes(src->attributes(), dest);

        if (!src->getTextContent().empty())
        {
            dest->setTextContent(XercesHelper::transcode(src->getTextContent()).c_str());
        }

        for (size_t i = 0; i < src->getChildCount(); ++i)
        {
            const XMLNode* src_child = src->getChild(i);
            DOMElement* dest_child = dest->getOwnerDocument()->createElement(
                XercesHelper::transcode(src_child->getName()).c_str());
            dest->appendChild(dest_child);
            populateDOMElement(src_child, dest_child);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    //
    // XercesWriter methods
    //
    ////////////////////////////////////////////////////////////////////////////////

    //-----------------------------------------------------------------------
    XMLWriter* CreateXercesWriter(void)
    {
        return new XercesWriter;
    }
    //-----------------------------------------------------------------------
    XercesWriter::XercesWriter(void)
    {
    }
    //-----------------------------------------------------------------------
    XercesWriter::~XercesWriter()
    {
    }
    //-----------------------------------------------------------------------
    void XercesWriter::writeXMLFile(const XMLNode* root, const String& filename)
    {
        XERCES_CPP_NAMESPACE_USE;

        DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(L"Core");
        DOMDocumentType* docType = NULL;
        DOMDocument* doc = impl->createDocument(NULL, transcode(root->getName()).c_str(), docType);

        populateDOMElement(root, doc->getDocumentElement());

        LocalFileFormatTarget destination(filename.c_str());
        DOMWriter* writer = impl->createDOMWriter();
        writer->setFeature(XMLUni::fgDOMWRTFormatPrettyPrint, true);
        writer->writeNode(&destination, *doc);
        writer->release();
        doc->release();
    }

}
