#include "StdAfx.h"
#include "HelperXMLParser.h"
#include "HelperXMLHandler.h"

CHelperXMLParser::CHelperXMLParser(void) :
d_identifierString("Unknown XML parser (vendor did not set the ID string!)"),
d_initialised(false)
{}

CHelperXMLParser::~CHelperXMLParser(void)
{}

bool CHelperXMLParser::initialise(void)
{
	// do this to ensure only one initialise call is made
	if (!d_initialised)
	{
		d_initialised = initialiseImpl();
	}

	return d_initialised;
}

void CHelperXMLParser::cleanup(void)
{
	if (d_initialised)
	{
		cleanupImpl();
		d_initialised = false;
	}
}

const String& CHelperXMLParser::getIdentifierString() const
{
	return d_identifierString;
}

