#include "StdAfx.h"
#include "TDTheDebuger.h"
#include "DlgDebuger.h"
#include <conio.h>
#include "TDGameInterface.h"
#include "Global.h"
#include "TDException.h"
#include "TDKernel.h"
#include "Dlg_Control_Infotrack.h"

TD_IMPLEMENT_DYNAMIC(TDDebuger, GETCLASS(tDebuger));
TDDebuger::TDDebuger(void)
{
	m_pDlgDebuger = NULL;
}

TDDebuger::~TDDebuger(void)
{
}

void TDDebuger::Initial(void*)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(m_pDlgDebuger) return;

	g_pInterface = (tGameInterfaceBase*)g_pKernel->GetNode("bin\\interface");
	TDAssert(g_pInterface);

	g_pGfxSystem = (tGfxSystem*)g_pKernel->GetNode("bin\\gfx");
	TDAssert(g_pGfxSystem);

	g_pTimeSystem = (tTimeSystem*)g_pKernel->GetNode("bin\\time");
	TDAssert(g_pTimeSystem);

	g_pEventSystem = (tEventSystem*)g_pKernel->GetNode("bin\\event");
	TDAssert(g_pEventSystem);

	g_pVariableSystem = (tVariableSystem*)g_pKernel->GetNode("bin\\var");
	TDAssert(g_pVariableSystem);

	g_pDBCSystem = (tDataBaseSystem*)g_pKernel->GetNode("bin\\dbc");
	TDAssert(g_pDBCSystem);

	//´´½¨Debuger´°¿Ú
	m_pDlgDebuger = new CDlgDebuger;
	m_pDlgDebuger->Create( IDD_DIALOG_DEBUGER, NULL);
}

void TDDebuger::Tick(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	if(!m_pDlgDebuger) return;
}

void TDDebuger::Release(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(m_pDlgDebuger)
	{
		m_pDlgDebuger->DestroyWindow();
		delete m_pDlgDebuger;
		m_pDlgDebuger = NULL;
	}
}

void TDDebuger::InfoTrack_Reset(INT nWidth, INT nHeight)
{
	m_pDlgDebuger->GetDlgUI()->m_pInfotrackDlg->Reset(nWidth, nHeight);
}

void TDDebuger::InfoTrack_AddPoint(INT nX, INT nY)
{
	m_pDlgDebuger->GetDlgUI()->m_pInfotrackDlg->AddPoint(nX, nY);
}

