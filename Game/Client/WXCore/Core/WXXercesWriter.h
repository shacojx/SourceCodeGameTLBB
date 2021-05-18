#ifndef __FAIRYXERCESWRITER_H__
#define __FAIRYXERCESWRITER_H__

#include "WXXMLWriter.h"
#include "WXXercesHelper.h"


namespace WX
{
    /*!
    \brief
        Implementation of XMLWriter using Xerces-C++
     */
    class XercesWriter : public XMLWriter, public XercesHelper
    {
    public:
        XercesWriter(void);
        ~XercesWriter();

        // Implementation of public abstract interface
        virtual void writeXMLFile(const XMLNode* root, const String& filename);
    };

}

#endif  
