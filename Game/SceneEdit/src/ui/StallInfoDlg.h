/////////////////////////////////////////////////////////////////////////////
// Name:       StallInfoDlg.h
// Author:      XX
// Created:     XX/XX/XX
// Copyright:   XX
/////////////////////////////////////////////////////////////////////////////
#ifndef __STALLINFODLG_H__
#define __STALLINFODLG_H__
#pragma once

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma interface "StallInfoDlg.cpp"
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "./res/StallInfoDlg_wdr.h"

// WDR: class declarations

#include <wx/panel.h>
#include <wx/image.h>
#include "WXSceneListener.h"

namespace WX 
{
    
	class SceneManipulator;
	class CStallInfoAction;
	
}
class CStallInfoDlg: public wxPanel, WX::SceneListener
{
public:


///////////////////////////////////////////////////////////////////////////////////////////
//
// 构造函数。
//
public:

	CStallInfoDlg(const wxString& resourceGroup = wxT("StallInfoEdit"));

	CStallInfoDlg(
						wxWindow *parent, 
						const wxString& resourceGroup = wxT("StallInfoEdit"),
						wxWindowID winid = wxID_ANY,
						const wxPoint& pos = wxDefaultPosition,
						const wxSize& size = wxDefaultSize,
						long style = wxTAB_TRAVERSAL | wxNO_BORDER,
						const wxString& name = "StallInfoEdit"
						);


	~CStallInfoDlg();


//
// 构造函数。
//
///////////////////////////////////////////////////////////////////////////////////////////



	//****************************************************************************************************/
	//
	// 初始化函数 清除函数。
	//

	
	 // 是否摆摊
	wxTextCtrl* m_pTextCanStall;		

	// 税费
	wxTextCtrl* m_pTextTax;		

	// 摊位费
	wxTextCtrl* m_pTextPosTax;		

	// 额外信息
	wxTextCtrl* m_pExtraInfo;		

	// 当前操作的类型
	int        m_iCurOperateType;

	unsigned char	m_iTradeTax;	//交易税
	unsigned long	m_dwPosTax;		//摊位费
	unsigned char	m_iExtraInfo;	//附加信息


	// 创建
    bool Create(
				wxWindow *parent,
                wxWindowID winid = wxID_ANY,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxTAB_TRAVERSAL | wxNO_BORDER,
                const wxString& name = "StallInfoEdit");



	// 得到控件信息
	void GetAllControl();

	
	//
	// 初始化函数 清除函数。
	//
	//****************************************************************************************************/


	
	// 读取数据
	void LoadData(std::string strFile);

	// 存储数据
	void SaveData(std::string strFile);

	// 创建摊位信息
	void CreateStallInfo();

	/******/
	/////////////////////////////////////////////////////////////////////////////////////////
	//
	// 场景操作
	//

	 // 场景操作
	 WX::SceneManipulator* m_pSceneManipulator;

	 // 对应的action
	 WX::CStallInfoAction* m_pEditAction;

	// 得到action
	void GetAction();

	// 激活区域编辑操作
	void SetActive(bool bShow);

	// 设置场景操作句柄.
	void SetSceneManipulator(WX::SceneManipulator* sceneManipulator);

	// 显示窗口.
	bool Show(bool show);


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 事件函数
	//

	// 按钮函数， 设置摊位信息
	void OnSetStallInfo( wxCommandEvent &event );

	// 按钮函数， 选择
	void OnDelStallInfo( wxCommandEvent &event );

	// 按钮函数， 删除一个区域
	void OnViewStallInfo( wxCommandEvent &event );

	// 税率改变
	void OnTradeTaxChange(wxCommandEvent& event);

	// 摊位费改变
	void OnPosTaxChange(wxCommandEvent& event);

	// 额外信息改变
	void OnExtraInfoChange(wxCommandEvent& event);


	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	// 界面通知回掉函数
	//

public:

		virtual void onViewStallInfo(
								unsigned char	bCanStall,	//是否可以摆摊
								unsigned char	iTradeTax,	//交易税
								unsigned long	dwPosTax,	//摊位费
								unsigned char	iExtraInfo	//附加信息 
								);
		

	
	//
	// 界面通知回掉函数
	//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////


	DECLARE_EVENT_TABLE()

};

#endif //__STALLINFODLG_H__