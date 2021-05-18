//////////////////////////////////////////////////
// ref from http://vibration.cnblogs.com/archive/2005/09/20/240252.html
//////////////////////////////////////////////////

#pragma once

////////////////////////////////////////////////////
// !!!!!Orignal comment!!!!!

///////////////////////////////////////////////////////////////////////////////////////////////////////////
//	class CCaptionButton
//		Add push buttons to the caption bar
//
//	Author:		Yao Zhifeng
//	Contact:	yaozhifeng@hotmail.com
//
//	Usage:
//		1.use this class as a base class
//		2.use CHAIN_MSG_MAP to chain message to this class
//		3.call AddButton to add one or more buttons to the caption bar
//		4.(IMPORTANT) override GetButtonPos function to provide position for each button added
//		5.handle WM_COMMAND notification sent by caption button in the same way as handle normal button clicking
//		6.(optional) call CheckButton to change the checked status of the caption button
//		7.(optional) call EnableButton to change the enable status of the caption button
//
//	template argument:
//		T: derived class, must also derived from CWindowImpl directly or indirectly
//

class CCaptionButtonDlg : public CDialog
{
private:
	struct	_button
	{
		UINT	uID;	//command ID
		int		cx;		//width
		int		cy;		//height
		HIMAGELIST	himl;	//image list
		UINT	uStatus;	//status (image index)
		char	szHint[80];	//tooltip text
	};

	CToolTipCtrl	m_tooltips;
	std::vector<_button>	m_buttons;
	int	m_nPressed; //currently pressed button index

public:
	enum
	{
		//Button status /image index
		CAPTION_BTN_NORMAL		= 0,
		CAPTION_BTN_PUSHED		= 1,
		CAPTION_BTN_HOVER		= 2,
		CAPTION_BTN_CHECKED		= 3,
		CAPTION_BTN_DISABLED	= 4,

		//Interval between buttons
		CAPTION_BTN_INTERVAL	= 2
	};



	CCaptionButtonDlg(UINT id, CWnd* pParent = NULL);
	virtual ~CCaptionButtonDlg();

protected:
	DECLARE_MESSAGE_MAP()

	/////////////////////////////////////////////////////////////////
	// remove all the buttons
	void Clear();

	/////////////////////////////////////////////////////////////
	// remove the button at given position
	void RemoveButton(int index);

	////////////////////////////////////////////////////////////////
	// append a button
	// return the count of buttons
	int AddButton(UINT uID, int cx, int cy, HIMAGELIST himl, LPCTSTR lpszHint=NULL);
	
	///////////////////////////////////////////////////////////////////////////////////
	// check or uncheck the button at postion index
	void CheckButton(int index, bool bCheck);

	/////////////////////////////////////////////////////////////////////////////////
	// if the button at position index checked
	bool IsButtonChecked(int index);

	/////////////////////////////////////////////////////////////////////////////
	// enable/disable button at given position
	void EnableButton(int index, bool bEnable);

	bool IsButtonEnabled(int index);

	/////////////////////////////////////////////////////////////////////////////////
	// Get the zero based index of the button, given the button ID
	int GetButtonIndex(UINT uID);

	//////////////////////////////////////////////////////////////////////////////////
	// get the topleft corner of the given button, in window coordinate
	// override this function in the derived class to specify button position
	// this default implementation only works for tool window without any existing caption buttons
	POINT GetButtonPos(int index);
	
	///////////////////////////////////////////////////////////////////////////////////
	// get the rect of given button in window coordinate
	RECT GetButtonRect(int index);
	
	////////////////////////////////////////////////////////////////////////////////////
	// get button under point, in window coordinate
	int GetButtonAtPos(POINT pt);

private:
	////////////////////////////////////////////////////////////////////////////////////
	// WM_NCHITTEST	 message handler
	UINT OnNcHitTest(CPoint pt);

	///////////////////////////////////////////////////////////////////////////////////
	// WM_NCPAINT message handler
	// draw the buttons
	VOID OnNcPaint();

	///////////////////////////////////////////////////////////////////////////////////
	// WM_NCLBUTTONDOWN messange handler
	// if pressed on a button, SetCapture if not already captured
	// and push the button
	VOID OnNcLButtonDown(UINT, CPoint pt);

	///////////////////////////////////////////////////////////////////////////////////
	// WM_MOUSEMOVE message handler
	// valid only if mouse captured
	VOID OnMouseMove(UINT, CPoint pt);

	///////////////////////////////////////////////////////////////////////////////
	// WM_LBUTTONUP message handler
	// valid only if mouse captured
	// fire button clicked event if one button clicked
	VOID OnLButtonUp(UINT, CPoint pt);
	
	///////////////////////////////////////////////////////////////////////////////
	// WM_ACTIVATE messsage handler
	// force repaint of non-client area
	VOID OnActivate(UINT, CWnd*, BOOL);

	///////////////////////////////////////////////////////////////////////////
	// Notify TTN_GETDISPINFO handler
	// provide tooltip text dynamicly
	LRESULT OnGetDispInfo(int idCtrl, LPNMHDR lpnmHdr, BOOL &bHandled);

protected:
	/////////////////////////////////////////////////////////////////////////////////////
	// fire button cliced event
	// can be override in derived class
	// the default implementation send WM_COMMAND with button ID
	void OnCaptionButtonClicked(int index);

	///////////////////////////////////////////////////////////////////////////
	// update all button status and repaint
	void UpdateStatus();

	///////////////////////////////////////////////////////////////////////////////////
	// tooltip processing
	void RelayEvent();

	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};
