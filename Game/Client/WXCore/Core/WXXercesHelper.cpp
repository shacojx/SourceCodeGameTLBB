#include "WXXercesHelper.h"

#include <OgreException.h>

// Xerces-C includes
#include <OgreNoMemoryMacros.h>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <OgreMemoryMacros.h>

namespace WX
{
    ////////////////////////////////////////////////////////////////////////////////
    //
    // Helper routines
    //
    ////////////////////////////////////////////////////////////////////////////////

    std::basic_string<char>
    XercesHelper::transcode(const XMLCh *toTranscode)
    {
        XERCES_CPP_NAMESPACE_USE;

	    char* transcoded = XMLString::transcode(toTranscode);
	    std::basic_string<char> result(transcoded);
	    XMLString::release(&transcoded);
        return result;
    }
    //-----------------------------------------------------------------------
    std::basic_string<XMLCh>
    XercesHelper::transcode(const char *toTranscode)
    {
        XERCES_CPP_NAMESPACE_USE;

	    XMLCh* transcoded = XMLString::transcode(toTranscode);
	    std::basic_string<XMLCh> result(transcoded);
	    XMLString::release(&transcoded);
        return result;
    }
    //-----------------------------------------------------------------------
    std::basic_string<char>
    XercesHelper::transcode(const std::basic_string<XMLCh>& toTranscode)
    {
        return XercesHelper::transcode(toTranscode.c_str());
    }
    //-----------------------------------------------------------------------
    std::basic_string<XMLCh>
    XercesHelper::transcode(const std::basic_string<char>& toTranscode)
    {
        return XercesHelper::transcode(toTranscode.c_str());
    }

    ////////////////////////////////////////////////////////////////////////////////
    //
    // XercesHelper class
    //
    ////////////////////////////////////////////////////////////////////////////////

    XercesHelper::XercesHelper(void)
        : mInitialised(false)
    {
	    XERCES_CPP_NAMESPACE_USE;

        // initialise Xerces-C XML system
	    try
	    {
		    XMLPlatformUtils::Initialize();
            mInitialised = true;
	    }
	    catch (XMLException& exc)
	    {
		    // prepare a message about the failure
            Ogre::String message("An exception occurred while initialising the Xerces-C XML system.  Additional information: ");
            message += XercesHelper::transcode(exc.getMessage());
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR, message, "XercesHelper::XercesHelper");
	    }
    }
    //-----------------------------------------------------------------------
    XercesHelper::~XercesHelper()
    {
	    XERCES_CPP_NAMESPACE_USE;

        if (mInitialised)
        {
	        // cleanup XML stuff
	        XMLPlatformUtils::Terminate();
        }
    }

}
