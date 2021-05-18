/****************************************\
*										*
*										*
*										*
* 				TinyXMLParser			*
*										*
*						 by jinchao		*
*						ref from CEGUI	*
\****************************************/

#pragma once

#include "HelperXMLParser.h"

class CHelperTinyXMLParser : public CHelperXMLParser
{
public:
	CHelperTinyXMLParser(void);
	~CHelperTinyXMLParser(void);

	// Implementation of public abstract interface
	void parseXMLFile(CHelperXMLHandler& handler, const String& filename, const String& schemaName);

protected:
	// Implementation of abstract interface.
	bool initialiseImpl(void);
	void cleanupImpl(void);
};

