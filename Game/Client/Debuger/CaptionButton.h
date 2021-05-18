// CaptionButton.h: Schnittstelle für die Klasse CCaptionButton.
//
//////////////////////////////////////////////////////////////////////
/*! 
	\class		CCaptionButton 

 *  \date		04/2004
 *  \author	<b> Thomas Latuske </b>

////////////////////////////////////////////////////////////////////
*/
#include <afxtempl.h>
#include "windows.h"

#ifndef DECLARE_USER_MESSAGE
	#define DECLARE_USER_MESSAGE(name) \
				static const UINT name = ::RegisterWindowMessage(name##_MSG);
#endif

#define WMU_HITCAPBUTTON_MSG    _T("WMU_HITCAPBUTTON--{46CB2AA1-EB38-48ab-A4EC-D7C1D5D15648}")

#if !defined(AFX_CAPTIONBUTTON_H__8579D0A0_FD12_4476_8235_526DCF123FD9__INCLUDED_)
#define AFX_CAPTIONBUTTON_H__8579D0A0_FD12_4476_8235_526DCF123FD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define WMU_HITCAPBUTTON WM_APP + 15
	
static WNDPROC m_pWndProcOld;

class CCaptionButton : public CWnd 
{
public:
// *** C'tor + Deconstructor:
	CCaptionButton();
	virtual ~CCaptionButton();
// *** static functions:
	// Initialize the class with the Handle of the window/dialogue
	static BOOL InitCapBtn(HWND hWnd);
// ***member functions:
	// set the bitmap ID's and the No.(order) of the button(s)
	void SetBmpID(UINT iBtnNo,
				  UINT ID_Bmp_up, UINT ID_Bmp_down = 0,
				  BOOL bCheckBtn = FALSE);	
	// enable/disable button TRUE/FALSE
	BOOL EnableButton(BOOL bEnable);
	void ChangeButtonState();

private:
// *** static variables:
	static HWND m_hWndCap;	// Handle of the window that receives the button
	static BOOL m_bActive;	// state of the window/dialogue
	static CMap<UINT, UINT&, CCaptionButton*, CCaptionButton*&> m_mapBtn;  // map of: button no.<->CCaptionButton instance
// *** member variables:
	CRect m_rc;			// caption/button rectangle
	UINT m_idBmpUp;		// bitmap ID
	UINT m_idBmpDown;	// bitmap ID
	UINT m_uiBtnCount;	// No. of buttons
	BOOL m_bCheckBtn;	// check button TRUE/FALSE
	BOOL m_bBtnChecked;	// button is checked TRUE/FALSE
	BOOL m_bHighLight;	// mouse hovers over a button TRUE/FALSE
	BOOL m_bEnable;		// button is enabled TRUE/FALSE
	COLORADJUSTMENT *m_CAdj; // structure to adjust the color of the bitmap
	TRACKMOUSEEVENT m_TME;
// *** member functions:
	// calculate the caption-rectangle
	void CalcCapRect(CRect *pCapRect);	
	// paint the frame of the button (highlighted/diabled or not)
	void PaintBtnFrame(CDC *dc);		
	// change the COLORADJUSTMENT structure dependent on the window-state
	void PaintBmpActive(CDC *dc);		
	// the main paint procedure
	BOOL PaintBmpCheckBtn(UINT uiBtnCount = 1);	
	// find out if a button was hit or not:
	BOOL ButtonHitTest( CPoint point=(0,0),		
						BOOL bHover = FALSE);

	
// *** static functions:
	// is the window/dialogue active or inactive:
	static BOOL isWindowActive(BOOL &bActive);
	// change the caption text of the window/dialogue:
	static void OnMsgSetText(LPCSTR lpText);
// *** static functions to Subclass the dialogue:
	// Subclass a dialogue (change the name of it's WindowProc):
	static BOOL HookDlg();
	// the "new" WindowProc:
	static LRESULT CALLBACK HookedWindowProc(HWND, UINT, WPARAM, LPARAM);
};

#endif // !defined(AFX_CAPTIONBUTTON_H__8579D0A0_FD12_4476_8235_526DCF123FD9__INCLUDED_)

