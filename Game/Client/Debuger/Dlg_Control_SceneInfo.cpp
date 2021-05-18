// Dlg_Control_SceneInfo.cpp : implementation file
//

#include "stdafx.h"
#include "Debuger.h"
#include "Dlg_Control_SceneInfo.h"
#include ".\dlg_control_sceneinfo.h"
#include "Global.h"
#include "TDVariable.h"
#include "TDEventSystem.h"
#include "TDEventDefine.h"
#include "TDDataBase.h"
#include "TDDBC_Struct.h"
#include "TDException.h"

// CDlg_Control_SceneInfo dialog
CDlg_Control_SceneInfo* g_pSceneInfoDlg = NULL;
IMPLEMENT_DYNAMIC(CDlg_Control_SceneInfo, CResizableDialog)
CDlg_Control_SceneInfo::CDlg_Control_SceneInfo(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CDlg_Control_SceneInfo::IDD, pParent)
{
	g_pSceneInfoDlg = this;
}

CDlg_Control_SceneInfo::~CDlg_Control_SceneInfo()
{
}

void CDlg_Control_SceneInfo::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_INFO, m_staInfo);
}


BEGIN_MESSAGE_MAP(CDlg_Control_SceneInfo, CResizableDialog)
END_MESSAGE_MAP()


// CDlg_Control_SceneInfo message handlers

BOOL CDlg_Control_SceneInfo::OnInitDialog()
{
	CResizableDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	AddAnchor(IDC_STATIC_INFO, TOP_LEFT, BOTTOM_RIGHT);

	//关心场景转移
	g_pEventSystem->RegisterEventHandle("SCENE_TRANSED", _OnSceneTransed);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

VOID WINAPI CDlg_Control_SceneInfo::_OnSceneTransed(const EVENT* pEvent, UINT dwOwnerData)
{
	g_pSceneInfoDlg->OnSceneTransed(pEvent);	
}

VOID CDlg_Control_SceneInfo::OnSceneTransed(const EVENT* pEvent)
{
	//得到当前场景号
	if(pEvent->pEventDef->idEvent == GE_SCENE_TRANSED)
	{
		BOOL bHave;
		INT nSceneID = g_pVariableSystem->GetAs_Int("Scene_ID", &bHave);
		if(!bHave)
		{
			m_staInfo.SetWindowText("");
			return;
		}

		//查找场景定义
		static const tDataBase* s_pSceneDBC = NULL;
		if(!s_pSceneDBC)
		{
			s_pSceneDBC = g_pDBCSystem->GetDataBase(DBC_SCENE_DEFINE);
			TDAssert(s_pSceneDBC);
		}
		const _DBC_SCENE_DEFINE* pSceneDef = (const _DBC_SCENE_DEFINE*)s_pSceneDBC->Search_Index_EQU(nSceneID);
		if(!pSceneDef)
		{
			m_staInfo.SetWindowText("");
			return;
		}

		char szTemp[MAX_PATH];
		CString strSceneInfo;
		
		strSceneInfo = "Name: ";
		strSceneInfo += pSceneDef->szName;

		strSceneInfo += "\nSize: ";
		_snprintf(szTemp, MAX_PATH, "%d, %d", pSceneDef->nXSize, pSceneDef->nZSize);
		strSceneInfo += szTemp;

		STRING strServerAddress = g_pVariableSystem->GetAs_String("GameServer_Address");
		INT nServerPort = g_pVariableSystem->GetAs_Int("GameServer_Port");

		_snprintf(szTemp, MAX_PATH, "%s[%d]", strServerAddress.c_str(), nServerPort);
		strSceneInfo += "\nServer: ";
		strSceneInfo += szTemp;

		m_staInfo.SetWindowText(strSceneInfo);
	}
}
