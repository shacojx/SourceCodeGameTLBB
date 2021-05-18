#ifndef __FAIRYEXPATPARSER_H__
#define __FAIRYEXPATPARSER_H__

#include "WXXMLParser.h"

#define __DAMN_MICROSOFT__
#ifdef __DAMN_MICROSOFT__
#include "OgreUTFString.h"
#endif

namespace WX
{
    struct ConvertBuffer
    {
#ifdef __DAMN_MICROSOFT__
        Ogre::UTFString utfString;
#else
        std::vector<wchar_t> wideCharBuffer;
#endif
        std::vector<char> multiByteBuffer;
    };

    /*!
    \brief
        Implementation of XMLParser using Expat
     */
    class ExpatParser : public XMLParser
    {
    public:
        /** Construct with optional locale to convert unicode to local 8
            bit encoding.
        @remarks
            Internal will clone a copy of the locale object, so you don't need
            keep another copy of it.
        @par
            If no locale given, assumed use 936 codepage character encoding.
        */
        ExpatParser(const std::locale* loc);
        ~ExpatParser();

        // Implementation of public abstract interface
        void parseXMLFile(XMLHandler& handler,
                          const String& xmlName, const String& xmlResourceGroup,
                          const String& schemaName, const String& schemaResourceGroup);

    protected:
        // The locale use to convert unicode to local 8 bit encoding, default is
        // program locale, which default to operate system locale. You may set
        // the locale manual if you want to exchange the XML file between the compute
        // that use difference default locale.
        std::auto_ptr<std::locale> _locale;

        // Buffer used to convert unicode to local 8 bit encoding.
        ConvertBuffer _buffer;
    };

}

#endif  // 
