/****************************************\
*										*
* 			   HelperItem				*
*										*
*						 by jinchao		*
\****************************************/
#pragma once


class CHelperItem
{
public:
	typedef std::list< CHelperItem > ChildBuf;

	/*************************************************************************
		Construction and Destruction
	*************************************************************************/
	CHelperItem(const String& strName, const String& strAnchor="", const String& strUrl="");
	virtual ~CHelperItem();

	/*************************************************************************
		Copy constructor
	*************************************************************************/
    CHelperItem(const CHelperItem& obj);
    CHelperItem& operator=(const CHelperItem& obj);

	/*************************************************************************
		Public interface
	*************************************************************************/
	/*!
	\brief
		Get Name.
	*/
	const String& GetName(void) const { return m_strName; }

	/*!
	\brief
		Get URL.
	*/
	const String& GetURL(void) const { return m_strUrl; }

	/*!
	\brief
		Add a new child item for this HelperItem.
	*/
	CHelperItem* AddChildItem(const CHelperItem& childItem);

	/*!
	\brief
		Get ChildItem Vector.
	*/
	ChildBuf& GetChildBuf(void) { return m_childBuf; }

	/*!
	\brief
		Set User data
	*/
	void SetUserData(int data) { m_nUserData = data; }

	/*!
	\brief
		Get User data
	*/
	int GetUserData(void) const { return m_nUserData; }

protected:
	String			m_strName;		// The name of this item to display. Required!

	String			m_strAnchor;	// The anchor name. Game application use this field to anchor.
	String			m_strUrl;		// The help content.
	
	ChildBuf		m_childBuf;		// Child items.

	int				m_nUserData;	// User data
};