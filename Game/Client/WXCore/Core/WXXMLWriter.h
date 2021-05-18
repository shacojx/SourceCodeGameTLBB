#ifndef __FAIRYXMLWRITER_H__
#define __FAIRYXMLWRITER_H__

#include "WXPrerequisites.h"

namespace WX
{
    class XMLNode;

    class XMLWriter
    {
    public:
        XMLWriter(void);

        virtual ~XMLWriter();

        virtual void writeXMLFile(const XMLNode* root, const String& filename) = 0;
    };

}

#endif  // end of guard 
