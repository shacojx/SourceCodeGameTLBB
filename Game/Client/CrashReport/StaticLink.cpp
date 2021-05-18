#include "StdAfx.h"
#include "StaticLink.h"
#include "Global.h"

HFONT g_hLinkFont = NULL;

UINT WINAPI ShellExecute_Proc( void* pUrl )
{
	char* pszUrl = (char*)pUrl;

	if(pszUrl[0] == '1')
	{
		//用记事本打开详细数据文件
		CHAR szNotePad[MAX_PATH];

		GetWindowsDirectory(szNotePad, MAX_PATH);
		PathAppend(szNotePad, "notepad.exe");

		char szCmdLine[MAX_PATH] = {0};
		_snprintf(szCmdLine, MAX_PATH, "%s %s", szNotePad, g_szBigInfoFile);

		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si,sizeof(STARTUPINFO));
		ZeroMemory(&pi,sizeof(PROCESS_INFORMATION));
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_SHOWNORMAL;

		if (CreateProcess( NULL, szCmdLine, NULL, NULL, FALSE,CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi) == FALSE)
		{
			MessageBox(NULL, "无法启动记事本!", NULL, MB_OK|MB_ICONSTOP);
		}
	}
	else if(pszUrl[0] == '2')
	{
		//用WinDbg调试Dmp文件
		char szCmdLine[MAX_PATH] = {0};
		_snprintf(szCmdLine, MAX_PATH, "%s -z %s -c\".ecxr\"", g_szWinDbg, g_szDumpFile);

		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		ZeroMemory(&si,sizeof(STARTUPINFO));
		ZeroMemory(&pi,sizeof(PROCESS_INFORMATION));
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_SHOWNORMAL;

		if (CreateProcess( NULL, szCmdLine, NULL, NULL, FALSE,CREATE_DEFAULT_ERROR_MODE, NULL, NULL, &si, &pi) == FALSE)
		{
			MessageBox(NULL, "无法启动WinDbg!", NULL, MB_OK|MB_ICONSTOP);
		}

	}
	else
	{
		ShellExecute( GetDesktopWindow(), TEXT( "open" ), ( LPCSTR )pUrl, NULL, NULL, SW_SHOW ) ;
	}

	return 0 ;
}

LRESULT CALLBACK StaticLnkWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	AX_URL_LNK *pData = (AX_URL_LNK *)(LONG_PTR)GetWindowLong(hWnd, GWL_USERDATA);

	switch ( uMsg )
	{
	case WM_SETCURSOR :
		{
			SetCursor( LoadCursor( NULL, IDC_HAND ) ) ;
			pData->m_bCallOldProc = FALSE;
		}
		break ;

	case WM_LBUTTONUP :
		{
			UINT threadID;
			HANDLE hThread = (HANDLE)_beginthreadex(NULL, 0, ShellExecute_Proc, ( LPVOID )pData->szLnk, 0, &threadID ) ;
			CloseHandle( hThread ) ;

			pData->m_bCallOldProc = TRUE;
		}
		break ;

	case WM_PAINT :
		{
			PAINTSTRUCT ps ;
			HDC hDC = BeginPaint( hWnd, &ps ) ;

			SelectObject( hDC, g_hLinkFont ) ;
			
			RECT rect ;
			GetWindowRect( hWnd, &rect ) ;

			DWORD dwPos = GetMessagePos() ;
			POINT pt ;
			pt.x = GET_X_LPARAM( dwPos ) ;
			pt.y = GET_Y_LPARAM( dwPos ) ;
			
			SetBkMode( hDC, TRANSPARENT );
//			if ( PtInRect( &rect, pt ) )
//			{
//				SetTextColor( hDC, RGB( 128, 0, 128 ) ) ;
//			}
//			else 
			{
				SetTextColor( hDC, RGB( 0, 0, 255 ) ) ;
			}
			
			SetBkColor( hDC, GetSysColor( COLOR_3DFACE ) ) ;
			
			AX_URL_LNK *pData = (AX_URL_LNK *)(LONG_PTR)GetWindowLong(hWnd, GWL_USERDATA);
			TextOut( hDC, 0, 0, pData->szText, (INT)strlen(pData->szText) ) ;

			EndPaint( hWnd, &ps ) ;

//			HWND hCWnd = GetCapture();
//			if ( hWnd == hCWnd )
//			{
//				ReleaseCapture() ;
//			}
			pData->m_bCallOldProc = TRUE;
		}
		break;

	case WM_DESTROY:
		break ;
	}

	if(pData->m_bCallOldProc && pData->m_oldWndProc )
	{
		pData->m_oldWndProc(hWnd, uMsg, wParam, lParam);
	}
	pData->m_bCallOldProc = TRUE;

	return TRUE;
}

void CreateLinkFont(HWND hDlg)
{
    HFONT hFont = NULL ;
    LOGFONT logFont ;
	memset( &logFont, 0, sizeof( LOGFONT ) ) ;

	if( NULL == g_hLinkFont )
	{
		hFont = ( HFONT )SendMessage( hDlg, WM_GETFONT, 0, 0 ) ;
		if ( NULL != hFont )
		{
			if ( 0 != GetObject( hFont, sizeof( LOGFONT ), &logFont ) )
			{
				logFont.lfUnderline = TRUE ;
				g_hLinkFont = CreateFontIndirect( &logFont ) ;
			}
		}
	}
}

void DeleteLinkFont(void)
{
	if(g_hLinkFont)
	{
		DeleteObject(g_hLinkFont);
		g_hLinkFont=NULL;
	}
}