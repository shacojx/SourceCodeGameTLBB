
#pragma once

struct AX_URL_LNK
{
	TCHAR		szText[64];
	TCHAR		szLnk[MAX_PATH];

	WNDPROC		m_oldWndProc ;
	BOOL		m_bCallOldProc ;

};

LRESULT CALLBACK StaticLnkWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

void CreateLinkFont(HWND hDlg);
void DeleteLinkFont(void);
