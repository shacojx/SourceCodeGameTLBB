#ifndef __FAIRYXMLPARSER_H__
#define __FAIRYXMLPARSER_H__

#include "WXPrerequisites.h"

namespace WX
{
    class XMLHandler;

    class XMLParser
    {
    public:
        XMLParser(void);

        virtual ~XMLParser();

        virtual void parseXMLFile(XMLHandler& handler, const String& filename, const String& resourceGroup, const String& schemaName, const String& schemaResourceGroup) = 0;
    };

} 

#endif  // end of guard 
