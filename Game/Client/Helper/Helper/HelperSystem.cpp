#include "StdAfx.h"
#include "HelperSystem.h"
#include "HelperTinyXMLParser.h"
#include "HelperItemSet.h"
#include "HelperItem.h"

#include "Resource.h"
#include "HelperDlg.h"

/*************************************************************************
	Static Data Definitions
*************************************************************************/
// singleton instance pointer
CHelperSystem* CHelperSystem::s_pMe = 0;


/*************************************************************************
	Constructor
*************************************************************************/
CHelperSystem::CHelperSystem() : 
	m_pXMLParser(0)
{
	s_pMe = this;

	//Set work dir
	char szDir[MAX_PATH] = {0};
	GetCurrentDirectory(MAX_PATH, szDir);
	PathAppend(szDir, "..\\Helper");
	PathAddBackslash(szDir);
	m_strWorkDictory = szDir;

	// create TinyXML Parser.
	m_pXMLParser = new CHelperTinyXMLParser;
	// load all helper items.
	m_pHelpItemSet = new CHelperItemSet();
	m_pHelpItemSet->LoadItemSet(CHelperSystem::GetMe()->GetWorkDictory() + "HelperItems.xml");
}

/*************************************************************************
	Destructor
*************************************************************************/
CHelperSystem::~CHelperSystem()
{
	delete m_pHelpItemSet; m_pHelpItemSet = 0;
	delete m_pXMLParser; m_pXMLParser = 0;

	s_pMe = 0;
}

/*************************************************************************
	Destructor
*************************************************************************/
void CHelperSystem::GotoAnchor(const String& strAnchor)
{
	// serch item...
	CHelperItem* pItem = m_pHelpItemSet->FindItem_Anchor(strAnchor);
	if(!pItem) return;

	//goto item
	HTREEITEM hItem = (HTREEITEM)(pItem->GetUserData());
	if(0 == hItem) return;

	((CHelperDlg*)AfxGetApp()->GetMainWnd())->GotoHelperItem(hItem);

	// bring my to top
	//DWORD dwTimeOut;
	//SystemParametersInfo(SPI_GETFOREGROUNDLOCKTIMEOUT,  0,  &dwTimeOut,  0);  
	//SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT,  0, (LPVOID)0,  SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE);  
	//int ret = SetForegroundWindow(((CHelperDlg*)AfxGetApp()->GetMainWnd())->GetSafeHwnd());  
	//SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT,  0,  &dwTimeOut,  SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE);  

}

