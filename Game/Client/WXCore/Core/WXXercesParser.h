#ifndef __FAIRYXERCESPARSER_H__
#define __FAIRYXERCESPARSER_H__

#include "WXXMLParser.h"
#include "WXXercesHelper.h"


namespace WX
{
    /*!
    \brief
        Implementation of XMLParser using Xerces-C++
     */
    class XercesParser : public XMLParser, public XercesHelper
    {
    public:
        XercesParser(void);
        ~XercesParser();

        // Implementation of public abstract interface
        void parseXMLFile(XMLHandler& handler, const String& xmlName, const String& xmlResourceGroup, const String& schemaName, const String& schemaResourceGroup);
    };

} 

#endif  
