#include "WXExpatParser.h"
#include "WXXMLHandler.h"
#include "WXXMLAttributes.h"

#include "utf.h"

#include <OgreException.h>
#include <OgreResourceGroupManager.h>
#include <OgreStringConverter.h>

// Expat includes
#include <OgreNoMemoryMacros.h>
#include <expat.h>
#include <windows.h>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <OgreMemoryMacros.h>


namespace WX
{
    ////////////////////////////////////////////////////////////////////////////////
    //
    // Helper routines
    //
    ////////////////////////////////////////////////////////////////////////////////

    typedef boost::function2<const std::string&, std::string&, const std::string&> UTFConverter;

    namespace {

        static const std::basic_string<char>&
        expat_to_local(std::basic_string<char>& r, const std::basic_string<XML_Char>& s, const std::locale& loc)
        {
#if defined(XML_UNICODE)
    #if defined(XML_UNICODE_WCHAR_T) || !defined(_NATIVE_WCHAR_T_DEFINED)
            // XML_Char == wchar_t
            return r = utf::ucs_to_mbcs(s, loc);
    #else
            // XML_Char != wchar_t
            typedef std::basic_string<char> cvt(const std::basic_string<XML_Char>& s);
            return r = (reinterpret_cast<cvt*>(&utf::ucs_to_mbcs))(s, loc);
    #endif
#else
            return r = utf::utf8_to_mbcs(s, loc);
#endif
        }

        static const std::basic_string<char>&
        expat_to_local_win32(std::basic_string<char>& r, const std::basic_string<XML_Char>& s, ConvertBuffer& buffers)
        {
#ifdef __DAMN_MICROSOFT__
            buffers.utfString.assign(s);

            const std::wstring& wstr = buffers.utfString.asWStr();
            int cchWideChar = wstr.length() + 1;
            const wchar_t* pWideStr = wstr.c_str();
#else
            int cchWideChar = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                s.c_str(), s.length()+1,
                NULL, 0);
            if (cchWideChar <= 0)
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                    "Can't convert UTF8 '" + s + "' to MBCS",
                    "expat_to_local_win32");
            }

            buffers.wideCharBuffer.resize(cchWideChar);
            int cchConvertedWideChar = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
                s.c_str(), s.length()+1,
                &buffers.wideCharBuffer.front(), cchWideChar);
            if (cchConvertedWideChar != cchWideChar)
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                    "Can't convert UTF8 '" + s + "' to MBCS",
                    "expat_to_local_win32");
            }

            const wchar_t* pWideStr = &buffers.wideCharBuffer.front();
#endif

            int cbMultiByte = WideCharToMultiByte(936, 0,
                pWideStr, cchWideChar,
                NULL, 0,
                NULL, NULL);
            if (cbMultiByte <= 0)
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                    "Can't convert UTF8 '" + s + "' to MBCS",
                    "expat_to_local_win32");
            }

            buffers.multiByteBuffer.resize(cbMultiByte);
            int cchConvertedMultiByte = WideCharToMultiByte(936, 0,
                pWideStr, cchWideChar,
                &buffers.multiByteBuffer.front(), cbMultiByte,
                NULL, NULL);
            if (cchConvertedMultiByte != cbMultiByte)
            {
                OGRE_EXCEPT(Ogre::Exception::ERR_INVALIDPARAMS,
                    "Can't convert UTF8 '" + s + "' to MBCS",
                    "expat_to_local_win32");
            }

            return r.assign(&buffers.multiByteBuffer.front(), cchConvertedMultiByte-1);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////
    //
    //
    ////////////////////////////////////////////////////////////////////////////////
    struct MyUserData
    {
        XMLHandler* handler;
        UTFConverter converter;
        std::string s0, s1;
    };

    static void XMLCALL expatStartElementHandler(void *userData, const XML_Char *name, const XML_Char **atts)
    {
        MyUserData* myUserData = static_cast<MyUserData*>(userData);
        assert(myUserData);
        XMLHandler* handler = myUserData->handler;
        const UTFConverter& converter = myUserData->converter;
        assert(handler);
        assert(converter);
        std::string& s0 = myUserData->s0;
        std::string& s1 = myUserData->s1;

        XMLAttributes attributes;
        for (const XML_Char**p = atts; *p; p += 2)
        {
            assert(p[0] && p[1]);
            s0 = p[0]; s1 = p[1];
            attributes.setValue(converter(s0, s0), converter(s1, s1));
        }

        s0 = name;
        handler->startElement(converter(s0, s0), attributes);
    }
    //-----------------------------------------------------------------------
    static void XMLCALL expatEndElementHandler(void *userData, const XML_Char *name)
    {
        MyUserData* myUserData = static_cast<MyUserData*>(userData);
        assert(myUserData);
        XMLHandler* handler = myUserData->handler;
        const UTFConverter& converter = myUserData->converter;
        assert(handler);
        assert(converter);
        std::string& s0 = myUserData->s0;

        s0 = name;
        handler->endElement(converter(s0, s0));
    }
    //-----------------------------------------------------------------------
    static void XMLCALL expatCharacterDataHandler(void *userData, const XML_Char *s, int len)
    {
        MyUserData* myUserData = static_cast<MyUserData*>(userData);
        assert(myUserData);
        XMLHandler* handler = myUserData->handler;
        const UTFConverter& converter = myUserData->converter;
        assert(handler);
        assert(converter);
        std::string& s0 = myUserData->s0;

        s0 = std::basic_string<XML_Char>(s, len);
        handler->characters(converter(s0, s0));
    }

    ////////////////////////////////////////////////////////////////////////////////
    //
    // ExpatParser methods
    //
    ////////////////////////////////////////////////////////////////////////////////

    ExpatParser::ExpatParser(const std::locale* loc)
        : _locale()
    {
        if (loc)
        {
            _locale.reset(new std::locale(*loc));
        }
    }
    //-----------------------------------------------------------------------
    ExpatParser::~ExpatParser()
    {
    }
    //-----------------------------------------------------------------------
    void ExpatParser::parseXMLFile(XMLHandler& handler,
                                   const String& xmlName, const String& xmlResourceGroup,
                                   const String& schemaName, const String& schemaResourceGroup)
    {
        XML_Parser parser = XML_ParserCreate(NULL);

        try
        {
            MyUserData myUserData;
            myUserData.handler = &handler;
            if (_locale.get())
                myUserData.converter = boost::bind(&expat_to_local, _1, _2, boost::ref(*_locale));
            else
                myUserData.converter = boost::bind(&expat_to_local_win32, _1, _2, boost::ref(_buffer));

            XML_SetUserData(parser, &myUserData);
            XML_SetElementHandler(parser, expatStartElementHandler, expatEndElementHandler);
            XML_SetCharacterDataHandler(parser, expatCharacterDataHandler);

            const size_t MaxBufferSize = 64*1024;
            Ogre::DataStreamPtr dataStream = Ogre::ResourceGroupManager::getSingleton().openResource(xmlName, xmlResourceGroup);
            bool done;
            do 
            {
                void* buffer = XML_GetBuffer(parser, MaxBufferSize);
                if (!buffer)
                {
                    OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "Can't allocate buffer while parse XML file '" + xmlName + "'.",
                        "ExpatParser::parseXMLFile");
                }

                size_t count = dataStream->read(buffer, MaxBufferSize);
                done = dataStream->eof();

                if (XML_ParseBuffer(parser, count, done) != XML_STATUS_OK)
                {
                    OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                        "An error occurred while parsing XML file '" + xmlName +
                        "' at line " + Ogre::StringConverter::toString(XML_GetCurrentLineNumber(parser)) +
                        ". Additional information: " + XML_ErrorString(XML_GetErrorCode(parser)),
                        "ExpatParser::parseXMLFile");
                }
            } while (!done);
        }
        catch(...)
        {
            XML_ParserFree(parser);
            throw;
        }

        XML_ParserFree(parser);
    }

}
