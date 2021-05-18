// DlgWorld.cpp : implementation file
//

#include "stdafx.h"
#include "Debuger.h"
#include "DlgWorld.h"
#include ".\dlgworld.h"
#include "Global.h"
#include "TDDBC_Struct.h"

#include "TDKernel.h"
#include "TDWorldSystem.h"

#include "TDGameInterface.h"

IMPLEMENT_DYNAMIC(CDlgWorld, CResizableDialog)
CDlgWorld::CDlgWorld(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CDlgWorld::IDD, pParent)
{
	m_pWorldManager = NULL;
	m_nActiveSceneID = INVALID_UID;
}

CDlgWorld::~CDlgWorld()
{
}

void CDlgWorld::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MINIMAP, m_staMiniMap);
	DDX_Control(pDX, IDC_SLIDER_BLUR, m_sliderBlur);
	DDX_Control(pDX, IDC_SLIDER_SHINE, m_sliderShine);
	DDX_Control(pDX, IDC_STATIC_BLUR, m_staBlur);
	DDX_Control(pDX, IDC_STATIC_SHINE, m_staShine);
}


BEGIN_MESSAGE_MAP(CDlgWorld, CResizableDialog)
	ON_BN_CLICKED(IDC_RADIO_PF_NULL, OnBnClickedRadioPfNull)
	ON_BN_CLICKED(IDC_RADIO_PF_HEATVISION, OnBnClickedRadioPfHeatvision)
	ON_BN_CLICKED(IDC_RADIO_PF_FLOODLIGHTING, OnBnClickedRadioPfFloodlighting)
	ON_BN_CLICKED(IDC_RADIO_PF_BLOOM, OnBnClickedRadioPfBloom)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_RADIO_LM_NULL, OnBnClickedRadioLmNull)
	ON_BN_CLICKED(IDC_RADIO_LM_MIDDLE, OnBnClickedRadioLmMiddle)
	ON_BN_CLICKED(IDC_RADIO_LM_HIGH, OnBnClickedRadioLmHigh)
END_MESSAGE_MAP()


// CDlgWorld message handlers

BOOL CDlgWorld::OnInitDialog()
{
	CResizableDialog::OnInitDialog();

	AddAnchor(IDC_STATIC_GROUP_SCENE_DESC, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_EDIT_SCENE_DESC, TOP_LEFT, TOP_RIGHT);

	AddAnchor(IDC_STATIC_GROUP_MINIMAP, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_STATIC_MINIMAP, TOP_LEFT, TOP_RIGHT);

	AddAnchor(IDC_STATIC_GROUP_POSTFILTER, TOP_LEFT, TOP_RIGHT);
	AddAnchor(IDC_STATIC_GROUP_LIGHTMAP, TOP_LEFT, TOP_RIGHT);

	//AddAnchor(IDC_EDIT_ENTER_POS_X, TOP_LEFT, TOP_RIGHT);
	//AddAnchor(IDC_EDIT_ENTER_POS_Z, TOP_LEFT, TOP_RIGHT);
	//AddAnchor(IDC_BUTTON_ENTER_SCENE, TOP_CENTER);

	m_pWorldManager = (tWorldSystem*)g_pKernel->GetNode(_T("bin\\worldman"));
	//const SCENE_DEFINE_VECTOR& sceneDef = m_pWorldManager->GetSceneDefineVector();

	//for(int i=0; i<(int)sceneDef.size(); i++)
	//{
	//	m_comScene.AddString((LPCTSTR)sceneDef[i].strSceneName.c_str());
	//}
	//m_comScene.SetCurSel(0);

	CheckRadioButton(IDC_RADIO_PF_NULL, IDC_RADIO_PF_HEATVISION, IDC_RADIO_PF_FLOODLIGHTING);

	CheckRadioButton(IDC_RADIO_LM_NULL, IDC_RADIO_LM_HIGH, IDC_RADIO_LM_MIDDLE);

	m_sliderBlur.SetRange( 0, 100);
	m_sliderBlur.SetPos(0);

	m_sliderShine.SetRange( 0, 100);
	m_sliderShine.SetPos(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgWorld::Tick(void)
{
/**
	使用"if(m_pActiveScene != pSceneNow)..."判断会出现问题,
	有时候两个Scene不同，指针却相同!!!!
*/

	m_staMiniMap.Update();

	if(!m_pWorldManager) return;

	tScene* pSceneNow = m_pWorldManager->GetActiveScene();
	if(pSceneNow)
	{
		INT nIDNow = pSceneNow->GetSceneDefine()->nID;

		if(nIDNow != m_nActiveSceneID)
		{
			RefreshSceneDesc();
			m_nActiveSceneID = nIDNow;
		}
	}
}

void CDlgWorld::RefreshSceneDesc(void)
{
	tScene* pActiveScene = m_pWorldManager->GetActiveScene();

	const _DBC_SCENE_DEFINE* pSceneDef = pActiveScene->GetSceneDefine();

	TCHAR szTemp[1024];
	_sntprintf(szTemp, 1024, 
		_T("   ID: %d\r\n")
		_T(" NAME: %s\r\n")
		_T("XSize: %d\r\n")
		_T("ZSize: %d\r\n")
		,(int)pSceneDef->nID, 
		pSceneDef->szName, 
		pSceneDef->nXSize, pSceneDef->nZSize);

	GetDlgItem(IDC_EDIT_SCENE_DESC)->SetWindowText(szTemp);
	
}

void CDlgWorld::MiniMap_SetCenter(FLOAT fX, FLOAT fY, float fDir)
{
	m_staMiniMap.SetViewCenter(fX, fY, fDir);
}

void CDlgWorld::MiniMap_SetFileName(LPCTSTR szFileName, INT nWidth, INT nHeight, INT nWorldWidth, INT nWorldHeight)
{
	m_staMiniMap.SetMapFile(szFileName, nWidth, nHeight, nWorldWidth, nWorldHeight);
}

void CDlgWorld::OnBnClickedRadioPfNull()
{
	// TODO: Add your control notification handler code here
	OnPostFilterChanged();
}

void CDlgWorld::OnBnClickedRadioPfHeatvision()
{
	// TODO: Add your control notification handler code here
	OnPostFilterChanged();
}

void CDlgWorld::OnBnClickedRadioPfFloodlighting()
{
	// TODO: Add your control notification handler code here
	OnPostFilterChanged();
}

void CDlgWorld::OnBnClickedRadioPfBloom()
{
	// TODO: Add your control notification handler code here
	OnPostFilterChanged();
}

void CDlgWorld::OnPostFilterChanged(void)
{
	switch( GetCheckedRadioButton(IDC_RADIO_PF_NULL, IDC_RADIO_PF_HEATVISION))
	{
	case IDC_RADIO_PF_NULL:
		g_pInterface->Scene_SetPostFilter("");
		break;

	case IDC_RADIO_PF_BLOOM:
		g_pInterface->Scene_SetPostFilter("Bloom");
		break;

	case IDC_RADIO_PF_FLOODLIGHTING:
		g_pInterface->Scene_SetPostFilter("Floodlighting");
		break;

	case IDC_RADIO_PF_HEATVISION:
		g_pInterface->Scene_SetPostFilter("HeatVision");
		break;

	default:
		break;
	}

	if( GetCheckedRadioButton(IDC_RADIO_PF_NULL, IDC_RADIO_PF_HEATVISION) == IDC_RADIO_PF_FLOODLIGHTING)
	{
		m_sliderBlur.EnableWindow(TRUE);
		m_sliderShine.EnableWindow(TRUE);
		m_staBlur.EnableWindow(TRUE);
		m_staShine.EnableWindow(TRUE);
	}
	else
	{
		m_sliderBlur.EnableWindow(FALSE);
		m_sliderShine.EnableWindow(FALSE);
		m_staBlur.EnableWindow(FALSE);
		m_staShine.EnableWindow(FALSE);
	}
}

void CDlgWorld::PostFilter_Enable(BOOL bEnable)
{
	GetDlgItem(IDC_RADIO_PF_NULL)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_PF_BLOOM)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_PF_FLOODLIGHTING)->EnableWindow(bEnable);
	GetDlgItem(IDC_RADIO_PF_HEATVISION)->EnableWindow(bEnable);

	m_sliderBlur.EnableWindow(bEnable);
	m_sliderShine.EnableWindow(bEnable);
	m_staBlur.EnableWindow(bEnable);
	m_staShine.EnableWindow(bEnable);
}

void CDlgWorld::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	RefreshPostFilter();
	
	g_pGfxSystem->Debug_Scene_SetFloodlighting(
		m_sliderBlur.GetPos()/100.0f,
		m_sliderShine.GetPos()/100.0f); 

	CResizableDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDlgWorld::RefreshPostFilter(void)
{
	CString strTemp;

	float fBlur = m_sliderBlur.GetPos()/100.0f;
	strTemp.Format("[%.2f] Blur:", fBlur);
	m_staBlur.SetWindowText(strTemp);

	float fShine = m_sliderShine.GetPos()/100.0f;
	strTemp.Format("[%.2f]Shine:", fShine);
	m_staShine.SetWindowText(strTemp);
}

void CDlgWorld::PostFilter_SetFloodlighting_Param(FLOAT fBlur, FLOAT fShine)
{
	CheckRadioButton(IDC_RADIO_PF_NULL, IDC_RADIO_PF_HEATVISION, IDC_RADIO_PF_FLOODLIGHTING);
	
	m_sliderBlur.SetPos((INT)(fBlur*100.0f));

	m_sliderShine.SetPos((INT)(fShine*100.0f));

	RefreshPostFilter();
}


void CDlgWorld::OnBnClickedRadioLmNull()
{
	// TODO: Add your control notification handler code here
	OnLightMapChanged();
}

void CDlgWorld::OnBnClickedRadioLmMiddle()
{
	// TODO: Add your control notification handler code here
	OnLightMapChanged();
}

void CDlgWorld::OnBnClickedRadioLmHigh()
{
	// TODO: Add your control notification handler code here
	OnLightMapChanged();
}

void CDlgWorld::OnLightMapChanged()
{
	switch( GetCheckedRadioButton(IDC_RADIO_LM_NULL, IDC_RADIO_LM_HIGH))
	{
	case IDC_RADIO_LM_NULL:
		g_pInterface->Scene_SetLightmapQuality(0);
		break;

	case IDC_RADIO_LM_MIDDLE:
		g_pInterface->Scene_SetLightmapQuality(1);
		break;

	case IDC_RADIO_LM_HIGH:
		g_pInterface->Scene_SetLightmapQuality(2);
		break;

	default:
		break;
	}
}
