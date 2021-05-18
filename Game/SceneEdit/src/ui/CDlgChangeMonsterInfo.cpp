/////////////////////////////////////////////////////////////////////////////
// Name:        CDlgChangeMonsterInfo.
// Author:      XX
// Created:     XX/XX/XX
// Copyright:   XX
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "CDlgChangeMonsterInfo.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "CDlgChangeMonsterInfo.h"


IMPLEMENT_DYNAMIC_CLASS(CDlgChangeMosterInfo, wxDialog)


BEGIN_EVENT_TABLE(CDlgChangeMosterInfo, wxDialog)
    
EVT_INIT_DIALOG(CDlgChangeMosterInfo::OnInitDialog)
	
	EVT_BUTTON(ID_BUTTON_CHANGE_TIME,	CDlgChangeMosterInfo::OnEndModifyDlg)
	EVT_BUTTON(ID_BUTTON_CHANGE_RADIUS, CDlgChangeMosterInfo::OnEndModifyDlg)
	EVT_BUTTON(ID_BUTTON_CHANGE_BOTH,	CDlgChangeMosterInfo::OnEndModifyDlg)
	EVT_BUTTON(ID_BUTTON_CANCEL,		CDlgChangeMosterInfo::OnEndModifyDlg)

END_EVENT_TABLE()

void CDlgChangeMosterInfo::OnInitDialog(wxInitDialogEvent &e)
{
    wxDialog::OnInitDialog(e);

	ChangeMonsterInfo(this, true, true);

	// 时间按钮.
	m_pTimeBn = wxDynamicCast(FindWindow(ID_BUTTON_CHANGE_TIME), wxButton);

	// 半径按钮.
	m_pRadiusBn = wxDynamicCast(FindWindow(ID_BUTTON_CHANGE_RADIUS), wxButton);

	// 两个都修改按钮.
	m_pBothBn = wxDynamicCast(FindWindow(ID_BUTTON_CHANGE_BOTH), wxButton);

	// 不修改按钮.
	m_pNoneBn = wxDynamicCast(FindWindow(ID_BUTTON_CANCEL), wxButton);

	// 活动半径文本。
	m_pAreaText = wxDynamicCast(FindWindow(ID_TEXTCTRL_AREA_RADIUS1), wxTextCtrl);

	// 时间半径文本。
	m_pTimeText = wxDynamicCast(FindWindow(ID_TEXTCTRL_REFRESH_TIME1), wxTextCtrl);

	if(m_pAreaText)
	{
		m_pAreaText->SetLabel("2.000");
	}

	if(m_pTimeText)
	{
		m_pTimeText->SetLabel("1000");
	}
   
}

// 结束对话框
void CDlgChangeMosterInfo::OnEndModifyDlg( wxCommandEvent &event )
{
	switch(event.GetId())
	{
		case ID_BUTTON_CHANGE_TIME:
			{
				m_iChangeType = TIME;
			}
			break;
		case ID_BUTTON_CHANGE_RADIUS:
			{
				m_iChangeType = RADIUS;
			}
			break;
		case ID_BUTTON_CHANGE_BOTH:
			{
				m_iChangeType = BOTH;
			}
			break;
		case ID_BUTTON_CANCEL:
			{
				m_iChangeType = NONE;
			}
			break;
	}

	char buf[1024];
	wxString strValue = "";
	if(m_pAreaText)
	{
		memset(buf, 0, sizeof(buf));
		strValue = m_pAreaText->GetValue();
		sprintf(buf, "%s", strValue.c_str());
		m_fRadius = atof(buf);
	}

	if(m_pTimeText)
	{
		memset(buf, 0, sizeof(buf));
		strValue = m_pTimeText->GetValue();
		sprintf(buf, "%s", strValue.c_str());
		m_iTime = atoi(buf);
	}
	EndModal(wxID_OK);


	
}
// WDR: class implementations



