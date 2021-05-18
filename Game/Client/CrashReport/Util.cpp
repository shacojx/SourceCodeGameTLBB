#include "StdAfx.h"
#include "Util.h"


void CenterDlg(HWND hDlg)
{
	int x,y;
	int w,h;
	
	RECT rect;

	GetWindowRect(hDlg,&rect);

	w = rect.right - rect.left;
	h = rect.bottom - rect.top;

	x = ( GetSystemMetrics(SM_CXMAXIMIZED) - w ) /2;
	y = ( GetSystemMetrics(SM_CYMAXIMIZED) - h ) /2;

//	MoveWindow(hDlg,x,y,w,h,TRUE);

	::SetWindowPos(hDlg, HWND_TOPMOST, x, y, w, h, SWP_SHOWWINDOW);
}
