#include "WXXercesParser.h"
#include "WXXMLHandler.h"
#include "WXXMLAttributes.h"

#include <OgreException.h>
#include <OgreResourceGroupManager.h>
#include <OgreStringConverter.h>

// Xerces-C includes
#include <OgreNoMemoryMacros.h>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/sax2/DefaultHandler.hpp>
#include <xercesc/sax2/SAX2XMLReader.hpp>
#include <xercesc/sax2/XMLReaderFactory.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <OgreMemoryMacros.h>


namespace WX
{
    ////////////////////////////////////////////////////////////////////////////////
    //
    // Xerces SAX handler to pass through to Fairy based XML Handler
    //
    ////////////////////////////////////////////////////////////////////////////////

    class XercesHandler : public XERCES_CPP_NAMESPACE::DefaultHandler
    {
    public:
        XercesHandler(XMLHandler& handler);
        ~XercesHandler();

        // Implementation of methods in Xerces DefaultHandler.
        void characters(const XMLCh* const chars, const unsigned int length);
        void startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname, const XERCES_CPP_NAMESPACE::Attributes& attrs);
        void endElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname);
        void warning(const XERCES_CPP_NAMESPACE::SAXParseException &exc);
        void error(const XERCES_CPP_NAMESPACE::SAXParseException &exc);
        void fatalError(const XERCES_CPP_NAMESPACE::SAXParseException &exc);

    protected:
        XMLHandler& mHandler;      //!< This is the 'real' Fairy based handler which we interface via.
    };

    ////////////////////////////////////////////////////////////////////////////////
    //
    // Helper routines
    //
    ////////////////////////////////////////////////////////////////////////////////

    static XERCES_CPP_NAMESPACE::InputSource*
    loadResourceInputSource(const Ogre::String& name, const Ogre::String& resourceGroup)
    {
        XERCES_CPP_NAMESPACE_USE;

        // load in the raw data
        Ogre::DataStreamPtr dataStream = Ogre::ResourceGroupManager::getSingleton().openResource(name, resourceGroup);
        size_t dataSize = dataStream->size();
        XMLByte* data = static_cast<XMLByte*>(XMLPlatformUtils::fgArrayMemoryManager->allocate(dataSize));
        dataStream->read(data, dataSize);

        // wrap data in a xerces MemBufInputSource object
        return new MemBufInputSource(data, dataSize, name.c_str(), true);
    }

    //////////////////////////////////////////////////////////////////////////

    static XERCES_CPP_NAMESPACE::SAX2XMLReader*
    createReader(XERCES_CPP_NAMESPACE::DefaultHandler& handler)
    {
        XERCES_CPP_NAMESPACE_USE;

        std::auto_ptr<SAX2XMLReader> reader(XMLReaderFactory::createXMLReader());

        // set basic settings we want from reader
        reader->setFeature(XMLUni::fgSAX2CoreNameSpaces, true);

        // enable schema use and set validation options
        reader->setFeature(XMLUni::fgSAX2CoreValidation, true);
        reader->setFeature(XMLUni::fgXercesSchema, true);
        reader->setFeature(XMLUni::fgXercesValidationErrorAsFatal, true);

        // set handlers
        reader->setContentHandler(&handler);
        reader->setErrorHandler(&handler);

        return reader.release();
    }
    //-----------------------------------------------------------------------
    static void
    initialiseSchema(XERCES_CPP_NAMESPACE::SAX2XMLReader* reader,
                     const Ogre::String& schemaName, const Ogre::String& schemaResourceGroup)
    {
        XERCES_CPP_NAMESPACE_USE;

        // load in the schema data
        std::auto_ptr<InputSource> inputSource(loadResourceInputSource(schemaName, schemaResourceGroup));

        // load the schema
        reader->loadGrammar(*inputSource, Grammar::SchemaGrammarType, true);

        // enable grammar reuse
        reader->setFeature(XMLUni::fgXercesUseCachedGrammarInParse, true);

        // set schema for usage
        reader->setProperty(XMLUni::fgXercesSchemaExternalNoNameSpaceSchemaLocation,
            const_cast<XMLCh*>(XercesHelper::transcode(schemaName).c_str()));
    }
    //-----------------------------------------------------------------------
    static void
    doParseXMLFile(XERCES_CPP_NAMESPACE::SAX2XMLReader* reader,
                   const Ogre::String& xmlName, const Ogre::String& xmlResourceGroup)
    {
        XERCES_CPP_NAMESPACE_USE;

        // load in the file data
        std::auto_ptr<InputSource> inputSource(loadResourceInputSource(xmlName, xmlResourceGroup));

        // perform parse
	    reader->parse(*inputSource);
    }
    //-----------------------------------------------------------------------
    static void
    populateXMLAttributes(const XERCES_CPP_NAMESPACE::Attributes& src, XMLAttributes& dest)
    {
        String attributeName;
        String attributeValue;
        for (unsigned int i = 0; i < src.getLength(); ++i)
        {
            attributeName = XercesHelper::transcode(src.getLocalName(i));
            attributeValue = XercesHelper::transcode(src.getValue(i));
            dest.setValue(attributeName, attributeValue);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    //
    // XercesParser methods
    //
    ////////////////////////////////////////////////////////////////////////////////

    //-----------------------------------------------------------------------
    XMLParser* CreateXercesParser(void)
    {
        return new XercesParser;
    }
    //-----------------------------------------------------------------------
    XercesParser::XercesParser(void)
    {
    }
    //-----------------------------------------------------------------------
    XercesParser::~XercesParser()
    {
    }
    //-----------------------------------------------------------------------
    void
    XercesParser::parseXMLFile(XMLHandler& handler,
                               const String& xmlName, const String& xmlResourceGroup,
                               const String& schemaName, const String& schemaResourceGroup)
    {
        XERCES_CPP_NAMESPACE_USE;

        XercesHandler xercesHandler(handler);

        // create reader
        std::auto_ptr<SAX2XMLReader> reader(createReader(xercesHandler));

        // setup schema
        if (!schemaName.empty())
        {
            initialiseSchema(reader.get(), schemaName, schemaResourceGroup);
        }

        // do parse
        try
        {
            doParseXMLFile(reader.get(), xmlName, xmlResourceGroup);
        }
        catch(const XMLException& exc)
        {
		    if (exc.getCode() != XMLExcepts::NoError)
		    {
		        // prepare a message about the failure
			    Ogre::String message("An error occurred while parsing XML file '" + xmlName + "'.  Additional information: ");
			    message += transcode(exc.getMessage());
                OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, message, "XercesParser::parseXMLFile");
		    }
        }
        catch(const SAXParseException& exc)
        {
		    // prepare a message about the failure
		    Ogre::String message("A SAX exception occurred while parsing XML file '" + xmlName + 
				"  line num: " + Ogre::StringConverter::toString(exc.getLineNumber()) + "'.  Additional information: ");
		    message += transcode(exc.getMessage());
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, message, "XercesParser::parseXMLFile");
        }
        catch(const Ogre::Exception&)
        {
            // No transform on OGRE exception
            throw;
        }
        catch(...)
        {
		    // prepare a message about the failure
            Ogre::String message("An unexpected error occurred while parsing XML file '" + xmlName + "'.");
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, message, "XercesParser::parseXMLFile");
        }
    }
    
    ////////////////////////////////////////////////////////////////////////////////
    //
    // XercesHandler methods
    //
    ////////////////////////////////////////////////////////////////////////////////

    XercesHandler::XercesHandler(XMLHandler& handler)
        : mHandler(handler)
    {
    }
    //-----------------------------------------------------------------------
    XercesHandler::~XercesHandler(void)
    {
    }
    //-----------------------------------------------------------------------
    void XercesHandler::characters(const XMLCh* const chars, const unsigned int length)
    {
        String s(XercesHelper::transcode(chars));

        mHandler.characters(s);
    }
    //-----------------------------------------------------------------------
    void
    XercesHandler::startElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname,
                                const XERCES_CPP_NAMESPACE::Attributes& attrs)
    {
        XMLAttributes attributes;
        populateXMLAttributes(attrs, attributes);

        String element(XercesHelper::transcode(localname));

        mHandler.startElement(element, attributes);
    }
    //-----------------------------------------------------------------------
    void
    XercesHandler::endElement(const XMLCh* const uri, const XMLCh* const localname, const XMLCh* const qname)
    {
        String element(XercesHelper::transcode(localname));

        mHandler.endElement(element);
    }
    //-----------------------------------------------------------------------
    void
    XercesHandler::warning(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
    {
        throw exc;
    }
    //-----------------------------------------------------------------------
    void
    XercesHandler::error(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
    {
        throw exc;
    }
    //-----------------------------------------------------------------------
    void
    XercesHandler::fatalError(const XERCES_CPP_NAMESPACE::SAXParseException &exc)
    {
        throw exc;
    }

}
