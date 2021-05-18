
#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "StallInfoDlg.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/wxprec.h"
#include ".\stallinfodlg.h"

#include "SceneManipulator.h"
#include "WXEditorFrame.h"
#include "WXSceneCanvas.h"
#include ".\stallinfoaction.h"
#include "TLBB_DBC.h"				// 读取excel


using namespace WX;
BEGIN_EVENT_TABLE(CStallInfoDlg, wxPanel)

	EVT_RADIOBUTTON(ID_RADIOBUTTON_SET_STALL,  CStallInfoDlg::OnSetStallInfo)
 	EVT_RADIOBUTTON(ID_RADIOBUTTON_DEL_STALL,  CStallInfoDlg::OnDelStallInfo)
	EVT_RADIOBUTTON(ID_RADIOBUTTON_VIEW_STALL, CStallInfoDlg::OnViewStallInfo)

	EVT_TEXT(ID_TEXTCTRL__TRADE_TAX,	CStallInfoDlg::OnTradeTaxChange)
	EVT_TEXT(ID_TEXTCTRL__POS_TAX,		CStallInfoDlg::OnPosTaxChange)
	EVT_TEXT(ID_TEXTCTRL__EXTRAINFO,	CStallInfoDlg::OnExtraInfoChange)

END_EVENT_TABLE()



CStallInfoDlg::~CStallInfoDlg(void)
{
}


CStallInfoDlg::CStallInfoDlg(const wxString& resourceGroup)
// 是否摆摊
:m_pTextCanStall(NULL)
// 税费
,m_pTextTax(NULL)	
// 摊位费
,m_pTextPosTax(NULL)
// 额外信息
,m_pExtraInfo(NULL)	
,m_pSceneManipulator(NULL)
,m_pEditAction(NULL)
{
    //Init(resourceGroup);
	//InitData();

	// 设置当前查看信息.
	m_iCurOperateType = CStallInfoAction::VIEW_STALL_INFO;
}

CStallInfoDlg::CStallInfoDlg(
						wxWindow *parent, 
						const wxString& resourceGroup,
						wxWindowID winid,
						const wxPoint& pos,
						const wxSize& size,
						long style,
						const wxString& name
						)						
										
{
	// 是否摆摊
	m_pTextCanStall = NULL;		

	// 税费
	m_pTextTax = NULL;		

	// 摊位费
	m_pTextPosTax = NULL;		

	// 额外信息
	m_pExtraInfo = NULL;		

	// 场景操作
	m_pSceneManipulator = NULL;

	// 对应的action
	m_pEditAction = NULL;

    Create(parent, winid, pos, size, style, name);
	//InitData();

}


bool CStallInfoDlg::Create(
								  wxWindow *parent, 
								  wxWindowID winid,
								  const wxPoint& pos, 
								  const wxSize& size,
								  long style, 
								  const wxString& name
								  )
{
    if (!wxPanel::Create(parent, winid, pos, size, style, name))
        return false;

	// 不调用这个函数， 对话框的控件不能显示
	StallInfoDlg(this, true, true);
	
	//// 获取各控件的引用
	GetAllControl();
	GetAction();
	return true;
}

// 显示窗口
bool CStallInfoDlg::Show(bool show)
{
	SetActive(show);
	return wxWindow::Show(show);
}

// 得到控件信息
void CStallInfoDlg::GetAllControl()
{
	
    wxDynamicCast(FindWindow(ID_TEXT__CAN_STALL), wxStaticText)->SetLabel("是否可以摆摊");

	m_pTextTax    = (wxTextCtrl*)wxDynamicCast(FindWindow(ID_TEXTCTRL__TRADE_TAX), wxTextCtrl);
	m_pTextPosTax = (wxTextCtrl*)wxDynamicCast(FindWindow(ID_TEXTCTRL__POS_TAX),   wxTextCtrl);
	m_pExtraInfo  = (wxTextCtrl*)wxDynamicCast(FindWindow(ID_TEXTCTRL__EXTRAINFO), wxTextCtrl);

	wxDynamicCast(FindWindow(ID_TEXT__TRADE_TAX), wxStaticText)->SetLabel("交易税(0~255)         ");  
	wxDynamicCast(FindWindow(ID_TEXT__POS_TAX),   wxStaticText)->SetLabel("摊位费(0~0xffffffff)   ");
	wxDynamicCast(FindWindow(ID_TEXT__EXTRAINFO), wxStaticText)->SetLabel("附加信息(0~255)      ");

	
	wxDynamicCast(FindWindow(ID_RADIOBUTTON_SET_STALL),	   wxRadioButton)->SetLabel("设置摊位");
	wxDynamicCast(FindWindow(ID_RADIOBUTTON_DEL_STALL),	   wxRadioButton)->SetLabel("删除摊位");
	wxDynamicCast(FindWindow(ID_RADIOBUTTON_VIEW_STALL),   wxRadioButton)->SetLabel("查看摊位");

	wxRadioButton* pViewStall = (wxRadioButton*)wxDynamicCast(FindWindow(ID_RADIOBUTTON_VIEW_STALL),   wxRadioButton);
	pViewStall->SetValue(1);

	m_pTextTax->SetLabel("0");
	m_pTextPosTax->SetLabel("0");
	m_pExtraInfo->SetLabel("0"); 
    
}




// 激活区域编辑操作
void CStallInfoDlg::SetActive(bool bShow)
{
	if (bShow)
    {
		//if(m_pSceneManipulator)
		//{
		//	WX::Action* action = m_pSceneManipulator->_getAction("StallInfoEdit");
		//	m_pSceneManipulator->setActiveAction(action);//
		//	m_pEditAction = (CEventAreaEditAction *)action;
		//	
		//}

		GetAction();
       
    }
	else
	{
		if(m_pEditAction)
		{
			//m_pEditAction->SetOldHitPointMaterial();
			m_pSceneManipulator->setActiveAction(NULL);
		}
	}

	// 显示隐藏场景
	if(m_pEditAction)
	{
		m_pEditAction->HideScene(bShow);
		if(bShow)
		{
			// 根据地形的高度显示区域.
			//m_pEditAction->SetRegionHeight();
		}
	}//
}

// 设置场景操作句柄.
void CStallInfoDlg::SetSceneManipulator(WX::SceneManipulator* sceneManipulator)
{
	m_pSceneManipulator = sceneManipulator;
	
	if(m_pSceneManipulator)
	{
		m_pSceneManipulator->addSceneListener(this);
		GetAction();
	}//
}

// 得到action
void CStallInfoDlg::GetAction()
{
	if(m_pSceneManipulator)
	{
		WX::Action* action = m_pSceneManipulator->_getAction("StallInfoEdit");
		m_pSceneManipulator->setActiveAction(action);//
		m_pEditAction = (CStallInfoAction *)action;
		
	}//
}


	// 按钮函数， 设置摊位信息
void CStallInfoDlg::OnSetStallInfo( wxCommandEvent &event )
{
	if(m_pEditAction)
	{
		m_pEditAction->m_iOperateType = CStallInfoAction::SET_STALL_INFO;
		m_iCurOperateType = CStallInfoAction::SET_STALL_INFO;
	}
}

	// 按钮函数， 选择
void CStallInfoDlg::OnDelStallInfo( wxCommandEvent &event )
{
	if(m_pEditAction)
	{
		m_pEditAction->m_iOperateType = CStallInfoAction::DEL_STALL_INFO;
		m_iCurOperateType = CStallInfoAction::DEL_STALL_INFO;
	}
}

	// 按钮函数， 删除一个区域
void CStallInfoDlg::OnViewStallInfo( wxCommandEvent &event )
{
	if(m_pEditAction)
	{
		m_pEditAction->m_iOperateType = CStallInfoAction::VIEW_STALL_INFO;
		m_iCurOperateType = CStallInfoAction::VIEW_STALL_INFO;
	}
}


// 税率改变
void CStallInfoDlg::OnTradeTaxChange(wxCommandEvent& event)
{
	if(m_pEditAction)
	{
		
		wxString strValue = m_pTextTax->GetValue();
		m_pEditAction->m_curStallInfoBrush.m_iTradeTax = atoi(strValue.c_str());

		//if(m_pEditAction->m_curStallInfoBrush.m_iTradeTax <= 0)
		//{
		//	m_pTextTax->SetLabel("0");//
		//}

		//if(m_pEditAction->m_curStallInfoBrush.m_iTradeTax >= 255)
		//{
		//	m_pTextTax->SetLabel("255");//
		//}//

	}
}

// 摊位费改变
void CStallInfoDlg::OnPosTaxChange(wxCommandEvent& event)
{
	if(m_pEditAction)
	{
		wxString strValue = m_pTextPosTax->GetValue();
		m_pEditAction->m_curStallInfoBrush.m_dwPosTax = atoi(strValue.c_str());
		//if(m_pEditAction->m_curStallInfoBrush.m_iExtraInfo <= 0)
		//{
		//	m_pTextPosTax->SetLabel("0");//
		//}

		//if(m_pEditAction->m_curStallInfoBrush.m_iExtraInfo >= 0xffffffff)
		//{
		//	char bufInfo[128];
		//	memset(bufInfo, 0, sizeof(bufInfo));
		//	sprintf(bufInfo, "%d", 0xffffffff);

		//	m_pTextPosTax->SetLabel(bufInfo);//
		//}//
	}
}

// 额外信息改变
void CStallInfoDlg::OnExtraInfoChange(wxCommandEvent& event)
{
	if(m_pEditAction)
	{
		wxString strValue = m_pExtraInfo->GetValue();
		m_pEditAction->m_curStallInfoBrush.m_iExtraInfo = atoi(strValue.c_str());
		
		//if(m_pEditAction->m_curStallInfoBrush.m_iExtraInfo <= 0)
		//{
		//	m_pExtraInfo->SetLabel("0");//
		//}

		//if(m_pEditAction->m_curStallInfoBrush.m_iExtraInfo >= 255)
		//{
		//	m_pExtraInfo->SetLabel("255");//
		//}//
	}
}

// 回调函数查看摊位信息
void CStallInfoDlg::onViewStallInfo(
								unsigned char	bCanStall,	//是否可以摆摊
								unsigned char	iTradeTax,	//交易税
								unsigned long	dwPosTax,	//摊位费
								unsigned char	iExtraInfo	//附加信息 
								)
{
	char szbufInfo[256];
	

	wxString strCanStall  = "";
	wxString strTradeTax  = "";
	wxString strPosTax    = "";
	wxString strExtraInfo = "";
	wxString strShowInfo = "";

	if(bCanStall)
	{
		strCanStall = "可以摆摊\r\n";
	}
	else
	{
		strCanStall = "不可以摆摊\r\n";
	}

	memset(szbufInfo, 0, sizeof(szbufInfo));
	sprintf(szbufInfo, "交易税:%d\r\n", iTradeTax);
	strTradeTax = szbufInfo;

	memset(szbufInfo, 0, sizeof(szbufInfo));
	sprintf(szbufInfo, "摊位费:%d\r\n", dwPosTax);
	strPosTax = szbufInfo;

	memset(szbufInfo, 0, sizeof(szbufInfo));
	sprintf(szbufInfo, "附加信息:%d\r\n", iExtraInfo);
	strExtraInfo = szbufInfo;

	memset(szbufInfo, 0, sizeof(szbufInfo));
	sprintf(szbufInfo, "%s,%s,%s,%s", strCanStall.c_str(), strTradeTax.c_str(), strPosTax.c_str(), strExtraInfo.c_str());
	strShowInfo = szbufInfo;

	wxDynamicCast(FindWindow(ID_TEXTCTRL_STALL_INFO), wxTextCtrl)->SetLabel(strShowInfo);


}


// 读取数据
void CStallInfoDlg::LoadData(std::string strFile)
{
	if(m_pEditAction)
	{
		m_pEditAction->ReadStallInfoFromFile(strFile);
	}
	
}

	// 存储数据
void CStallInfoDlg::SaveData(std::string strFile)
{
	if(m_pEditAction)
	{
		m_pEditAction->SaveStallInfoToFile(strFile);
	}
}

// 创建摊位信息
void CStallInfoDlg::CreateStallInfo()
{
	if(m_pEditAction)
	{
		m_pEditAction->CreateStallInfo();
	}
}
