/****************************************\
*										*
*										*
*										*
* 			HelperXMLHandler			*
*										*
*						 by jinchao		*
*						ref from CEGUI	*
\****************************************/
#pragma once

class CHelperXMLAttributes;
class CHelperXMLHandler
{
public:
	/*!
	\brief
	XMLHandler base class constructor.
	*/
	CHelperXMLHandler(void);

	/*!
	\brief
	XMLHandler base class destructor.
	*/
	virtual ~CHelperXMLHandler(void);

	/*!
	\brief
	Method called to notify the handler at the start of each XML element encountered.

	\param element
	String object holding the name of the element that is starting.

	\param attributes
	An XMLAttributes object holding the collection of attributes specified for the element.

	\return
	Nothing.
	*/
	virtual void elementStart(const String& element, const CHelperXMLAttributes& attributes);

	/*!
	\brief
	Method called to notify the handler at the end of each XML element encountered.

	\param element
	String object holding the name of the element that is ending.

	\return
	Nothing.
	*/
	virtual void elementEnd(const String& element);
};

