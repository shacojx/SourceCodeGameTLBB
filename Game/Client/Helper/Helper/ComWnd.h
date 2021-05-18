#pragma once

typedef struct
{
	unsigned char	cCmdType;
	unsigned char	cReserve1;
	unsigned char	cReserve2;
	unsigned char	cReserve3;
	char			pContents[MAX_PATH];
} HELPER_COMMAND, FAR *LPHELPER_COMMAND;

// CComWnd dialog

class CComWnd : public CDialog
{
	DECLARE_DYNAMIC(CComWnd)

public:
	CComWnd(CWnd* pParent = NULL);   // standard constructor
	virtual ~CComWnd();

// Dialog Data
	enum { IDD = IDD_COMWND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);

	DECLARE_MESSAGE_MAP()
};
