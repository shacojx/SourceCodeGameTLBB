#include "StdAfx.h"
#include "HelperXMLAttributes.h"
#include <sstream>
#include <iterator>
#include "utf/utf.h"

struct AUTO_LOCALE_CONVERT
{
	AUTO_LOCALE_CONVERT()
	{
		std::locale langLocale("");
		old = std::locale::global(langLocale);
	}
	~AUTO_LOCALE_CONVERT()
	{
		std::locale::global(old);
	}

	std::locale old;
};


CHelperXMLAttributes::CHelperXMLAttributes(void)
{}

CHelperXMLAttributes::~CHelperXMLAttributes(void)
{}

void CHelperXMLAttributes::add(const String& attrName, const String& attrValue)
{
	d_attrs[attrName] = attrValue;
}

void CHelperXMLAttributes::remove(const String& attrName)
{
	AttributeMap::iterator pos = d_attrs.find(attrName);

	if (pos != d_attrs.end())
		d_attrs.erase(pos);
}

bool CHelperXMLAttributes::exists(const String& attrName) const
{
	return d_attrs.find(attrName) != d_attrs.end();
}

size_t CHelperXMLAttributes::getCount(void) const
{
	return d_attrs.size();
}

const String& CHelperXMLAttributes::getName(size_t index) const
{
	if (index >= d_attrs.size())
	{
		throw std::exception("CHelperXMLAttributes::getName - The specified index is out of range for this CHelperXMLAttributes block.");
	}

	AttributeMap::const_iterator iter = d_attrs.begin();
	std::advance(iter, index);

	return (*iter).first;
}

const String& CHelperXMLAttributes::getValue(size_t index) const
{
	if (index >= d_attrs.size())
	{
		throw std::exception("CHelperXMLAttributes::getValue - The specified index is out of range for this CHelperXMLAttributes block.");
	}

	AttributeMap::const_iterator iter = d_attrs.begin();
	std::advance(iter, index);

	return (*iter).second;
}

const String& CHelperXMLAttributes::getValue(const String& attrName) const
{
	AttributeMap::const_iterator pos = d_attrs.find(attrName);

	if (pos != d_attrs.end())
	{
		return (*pos).second;
	}
	else
	{
		char szException[MAX_PATH] = {0};
		_snprintf(szException, MAX_PATH, "CHelperXMLAttributes::getValue - no value exists for an attribute named '%s'.", 
			attrName.c_str());

		throw std::exception(szException);
	}
}

const String CHelperXMLAttributes::getValueAsString(const String& attrName, const String& def) const
{
	AUTO_LOCALE_CONVERT theConverter;

	if(!exists(attrName)) return def;
	// utf8 -> mbcs
	return utf::utf8_to_mbcs(getValue(attrName));
}


bool CHelperXMLAttributes::getValueAsBool(const String& attrName, bool def) const
{
	if (!exists(attrName))
	{
		return def;
	}

	const String& val = getValue(attrName);

	if (val == "false" || val == "0")
	{
		return false;
	}
	else if (val == "true" || val == "1")
	{
		return true;
	}
	else
	{
		char szException[MAX_PATH] = {0};
		_snprintf(szException, MAX_PATH, 
			"CHelperXMLAttributes::getValueAsInteger - failed to convert attribute '%s' with value '%s' to bool.", 
			attrName.c_str(), 
			getValue(attrName).c_str());

		throw std::exception(szException);
	}
}

int CHelperXMLAttributes::getValueAsInteger(const String& attrName, int def) const
{
	if (!exists(attrName))
	{
		return def;
	}

	int val;
	std::istringstream strm(getValue(attrName).c_str());

	strm >> val;

	// success?
	if (strm.fail())
	{
		char szException[MAX_PATH] = {0};
		_snprintf(szException, MAX_PATH, 
			"CHelperXMLAttributes::getValueAsInteger - failed to convert attribute '%s' with value '%s' to integer.",
			attrName.c_str(), 
			getValue(attrName).c_str());

		throw std::exception(szException);
	}

	return val;
}

float CHelperXMLAttributes::getValueAsFloat(const String& attrName, float def) const
{
	if (!exists(attrName))
	{
		return def;
	}

	float val;
	std::istringstream strm(getValue(attrName).c_str());

	strm >> val;

	// success?
	if (strm.fail())
	{
		char szException[MAX_PATH] = {0};
		_snprintf(szException, MAX_PATH, 
			"CHelperXMLAttributes::getValueAsInteger - failed to convert attribute '%s' with value '%s' to float.",
			attrName.c_str(), 
			getValue(attrName).c_str());

		throw std::exception(szException);
	}

	return val;
}
