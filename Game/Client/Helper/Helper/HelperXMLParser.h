/****************************************\
*										*
*										*
*										*
* 				XMLParser				*
*										*
*						 by jinchao		*
*						ref from CEGUI	*
\****************************************/

#pragma once

class CHelperXMLHandler;
class CHelperXMLParser
{
public:
	/*!
	\brief
	XMLParser base class constructor.
	*/
	CHelperXMLParser(void);

	/*!
	\brief
	XMLParser base class destructor.
	*/
	virtual ~CHelperXMLParser(void);

	/*!
	\brief
	Initialises the XMLParser module ready for use.

	Note that this calls the protected abstract method 'initialiseImpl', which should
	be provided in your implementation to perform any required initialisation.

	\return
	- true if the module initialised successfully.
	- false if the module initialisation failed.
	*/
	bool initialise(void);

	/*!
	\brief
	Cleans up the XMLParser module after use.

	Note that this calls the protected abstract method 'cleanupImpl', which should
	be provided in your implementation to perform any required cleanup.

	\return
	Nothing.
	*/
	void cleanup(void);

	/*!
	\brief
	abstract method which initiates parsing of an XML file.

	\param handler
	XMLHandler based object which will process the XML elements.

	\param filename
	String object holding the filename of the XML file to be parsed.

	\param schemaName
	String object holding the name of the XML schema file to use for validating the XML.
	Note that whether this is used or not is dependant upon the XMLParser in use.

	\param resourceGroup
	String object holding the resource group identifier which will be passed to the
	ResourceProvider when loading the XML and schema files.

	\return
	Nothing.
	*/
	virtual void parseXMLFile(CHelperXMLHandler& handler, const String& filename, const String& schemaName) = 0;

	/*!
	\brief
	Return identification string for the XML parser module.  If the internal id string has not been
	set by the XML parser module creator, a generic string of "Unknown XML parser" will be returned.

	\return
	String object holding a string that identifies the XML parser in use.
	*/
	const String& getIdentifierString() const;

protected:
	/*!
	\brief
	abstract method which initialises the XMLParser ready for use.

	\return
	- true if the module initialised successfully.
	- false if the module initialisation failed.
	*/
	virtual bool initialiseImpl(void) = 0;

	/*!
	\brief
	abstract method which cleans up the XMLParser after use.

	\return
	Nothing.
	*/

	virtual void cleanupImpl(void) = 0;

	// data fields
	std::string d_identifierString;                 //!< String that holds some id information about the module.

private:
	bool d_initialised;     //!< true if the parser module has been initialised,
};

