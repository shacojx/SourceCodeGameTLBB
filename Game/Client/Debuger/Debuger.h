// Debuger.h : main header file for the Debuger DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CDebugerApp
// See Debuger.cpp for the implementation of this class
//

class CDebugerApp : public CWinApp
{
public:
	CDebugerApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
