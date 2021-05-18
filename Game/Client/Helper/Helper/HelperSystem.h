/****************************************\
*										*
* 			   HelperSystem				*
*										*
*						 by jinchao		*
\****************************************/

#pragma once

class CHelperXMLParser;
class CHelperItemSet;

class CHelperSystem
{

public:
	/*************************************************************************
		Public interface
	*************************************************************************/
	/*!
	\brief
		Get the XML Parser.
	*/
	const String& GetWorkDictory(void) const { return m_strWorkDictory; }

	/*!
	\brief
		Get the XML Parser.
	*/
	CHelperXMLParser* GetXMLParser(void) { return m_pXMLParser; }

	/*!
	\brief
		Get the XML Parser.
	*/
	CHelperItemSet*	GetHelpItemSet(void) { return m_pHelpItemSet; }

	/*!
	\brief
		Got to the spec item
	*/
	void GotoAnchor(const String& strAnchor);

protected:
	/*************************************************************************
		Implementation Data
	*************************************************************************/
	String				m_strWorkDictory;	// Work dictory(all local file base here)
	CHelperXMLParser*	m_pXMLParser;		// XML Parser user by system
	CHelperItemSet*		m_pHelpItemSet;		// The Help Itemset.

public:
	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	CHelperSystem();
	virtual ~CHelperSystem();

	/*!
	\brief
		Return pointer to singleton HelperSystem object

	\return
		Pointer to singleton HelperSystem object
	*/
	static CHelperSystem* GetMe(void) { return s_pMe; }

private:
    // unimplemented constructors / assignment
    CHelperSystem(const CHelperSystem& obj);
    CHelperSystem& operator=(const CHelperSystem& obj);

	static CHelperSystem* s_pMe;
};