#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "PathFindDlg.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#include "PathFindDlg.h"

#include <wx/splitter.h>
#include <wx/treectrl.h>
#include <wx/listctrl.h>

// ----------------------------------------------------------------------------
// Ogre headers
// ----------------------------------------------------------------------------

#include <OgreCodec.h>
#include <OgreResourceGroupManager.h>
#include <OgreStringConverter.h>

// ----------------------------------------------------------------------------
#include "SceneManipulator.h"
#include "Action.h"
#include "WXOperatorManager.h"

#include "Core/Terrain.h"
#include "Core/TerrainData.h"

#include "RegionEditDlgSelector.h"
#include "res/PathFindEdit_wdr.h"
#include "PathFindAction.h"
#include "UnreachRegion.h"
#include "RegionEditAction.h"

using namespace WX;

BEGIN_EVENT_TABLE(CPathFindEditDlg, wxPanel)
//EVT_BUTTON(ID_BUTTON_DEL_LINE, CPathFindEditDlg::OnDelRegion)
//EVT_BUTTON(ID_BN_ADD_NEW_LINE, CPathFindEditDlg::OnAddNewRegion)
EVT_BUTTON(ID_BUTTON_DEL_POINT,  CPathFindEditDlg::OnDelPointInRegion)
//EVT_BUTTON(ID_BUTTON_LINE_OK,  CPathFindEditDlg::OnFinishRegion)

//EVT_LISTBOX(ID_LISTBOX_LINE, CPathFindEditDlg::OnRegionListBox)
EVT_LISTBOX(ID_LISTBOX_POINT, CPathFindEditDlg::OnPointListBox)
//EVT_CHECKBOX(ID_IFY, CPathFindEditDlg::OnPointUseY)
EVT_TEXT_ENTER(ID_POINT_X, CPathFindEditDlg::OnEditXYZ)
//EVT_TEXT_ENTER(ID_POINT_Y, CPathFindEditDlg::OnEditXYZ)
EVT_TEXT_ENTER(ID_POINT_Z, CPathFindEditDlg::OnEditXYZ)

END_EVENT_TABLE()


CPathFindEditDlg::CPathFindEditDlg(const wxString& resourceGroup)
:m_pListboxPointInRegion(NULL)
,m_pDelPointBn(NULL)
,m_pSceneManipulator(NULL)
,m_pAction(NULL)
,m_bIsEditingRegion(false)
,m_iCurSelRegionIndex(-1)
,m_pPointX(NULL)
,m_pPointZ(NULL)
,m_ulCurRegionId(-1)

{
	InitData();


	// 不可行走区域列表
	m_pListboxPointInRegion = NULL;

	// 删除点的按钮.
	m_pDelPointBn = NULL;

	// 场景操作
	m_pSceneManipulator = NULL;

	m_pAction = NULL;
}

CPathFindEditDlg::CPathFindEditDlg(
									 wxWindow *parent, 
									 const wxString& resourceGroup,
									 wxWindowID winid,
									 const wxPoint& pos,
									 const wxSize& size,
									 long style,
									 const wxString& name
									 )
									 :m_pListboxPointInRegion(NULL)
									 ,m_pDelPointBn(NULL)
									 ,m_pSceneManipulator(NULL)
									 ,m_pAction(NULL)
									 ,m_bIsEditingRegion(false)
									 ,m_iCurSelRegionIndex(-1)
									 ,m_pPointX(NULL)
									 ,m_pPointZ(NULL)
									 ,m_ulCurRegionId(-1)
{
	// 不可行走区域列表
	m_pListboxPointInRegion = NULL;

	// 删除点的按钮.
	m_pDelPointBn = NULL;

	// 场景操作
	m_pSceneManipulator = NULL;

	m_pAction = NULL;

	Create(parent, winid, pos, size, style, name);
	InitData();

}

bool CPathFindEditDlg::Create(
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

	PathFindDlg(this, true, true);

	////////////////////////////////////////////////////////////////////////////////////
	//
	// 获取各控件的引用
	//

	// 获得点控件的指针.
	m_pListboxPointInRegion = wxDynamicCast(FindWindow(ID_LISTBOX_POINT), wxListBox);

	// 得到删除点的控件的指针
	m_pDelPointBn =  wxDynamicCast(FindWindow(ID_BUTTON_DEL_POINT), wxButton);

	m_pPointX = wxDynamicCast(FindWindow(ID_POINT_X), wxTextCtrl);
	m_pPointX->Enable(false);
	m_pPointZ = wxDynamicCast(FindWindow(ID_POINT_Z), wxTextCtrl);
	m_pPointZ->Enable(false);
	if(m_pListboxPointInRegion)
	{
		m_pListboxPointInRegion->Clear();
	}

	GetRegionAction();
	return true;
}

CPathFindEditDlg::~CPathFindEditDlg(void)
{
}

bool CPathFindEditDlg::Show(bool show)
{
	SetActive(show);
	return wxWindow::Show(show);
}

void CPathFindEditDlg::ActiveAction(bool bActive)
{
	if(m_pAction)
	{
		if(!bActive)
		{
			m_pSceneManipulator->setActiveAction(NULL);
		}
		m_pAction->HideScene(bActive);
	}
}

void CPathFindEditDlg::SetActive(bool bShow)
{

	if (bShow)
	{
		if(m_pSceneManipulator)
		{
			WX::Action* action = m_pSceneManipulator->_getAction("PathFind");
			m_pSceneManipulator->setActiveAction(action);
			m_pAction = (CPathFindAction *)action;
			AddNewRegion();
		}
	}
	else
	{
		if(m_pAction)
		{
			m_pAction->SetOldHitPointMaterial();
			m_pSceneManipulator->setActiveAction(NULL);
		}
	}

	// 显示隐藏场景
	if(m_pAction)
	{
		m_pAction->HideScene(bShow);

		WX::Action* action = m_pSceneManipulator->_getAction("RegionEdit");
		WX::CRegionEditAction* pRegionEditAction = ( WX::CRegionEditAction*)action;
		pRegionEditAction->HideScene(bShow);

		if(bShow)
		{
			// 根据地形的高度显示区域.
			m_pAction->SetRegionHeight();
		}
	}

}

void CPathFindEditDlg::SetSceneManipulator(WX::SceneManipulator* sceneManipulator)
{
	m_pSceneManipulator = sceneManipulator;

	if(m_pSceneManipulator)
	{
		m_pSceneManipulator->addSceneListener(this);
		WX::Action* action = m_pSceneManipulator->_getAction("PathFind");
		m_pAction = (CPathFindAction *)action;
		SetActive(false);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////
//
// 场景编辑的回掉函数
//

// 选中一个区域的回调函数。
void CPathFindEditDlg::onSelDijkPoint(unsigned long ulPointId)
{
	//unsigned long ulFindRegionId = 0;

	//int iRegionCount = m_pListboxRegion->GetCount();

	//for(int i = 0; i < iRegionCount; i++)
	//{
	//	// 得到区域id
	//	ulFindRegionId = (unsigned long)m_pListboxRegion->GetClientData(i);
	//	if(ulFindRegionId == ulRegionId)
	//	{
	//		m_bIsChangeCamera = false;
	//		m_pListboxRegion->SetSelection(i);
	//		DoSelRegion();

	//		m_bIsChangeCamera = true;
	//		break;
	//	}
	//}
}
//fujia
// 区域编辑的回掉函数， 通知界面添加一个新的点.
void CPathFindEditDlg::onAddDijkPoint(unsigned long ulPointId, Ogre::Vector3 position)
{
	// 只有当前区域是这个区域时才添加.
	if(m_ulCurRegionId == unsigned long(1) )
	{
		RegionAddNewPoint(ulPointId);

		float fTerrainDataPosX = m_pAction->getTerrainData()->mPosition.x;
		float fTerrainDataPosZ = m_pAction->getTerrainData()->mPosition.z;
		float fScale = 1.0;
		if( m_pAction->getSceneManipulator() )
		{
			fScale = m_pAction->getSceneManipulator()->getBaseScale();
			if(abs((float)fScale) < 0.00001) fScale = 1.0;
		}

		char buf[128];
		sprintf( buf, "%.4f", (position.x - fTerrainDataPosX) / fScale );
		m_pPointX->Clear();
		m_pPointX->WriteText( buf );
		sprintf( buf, "%.4f", (position.z - fTerrainDataPosZ) / fScale );
		m_pPointZ->Clear();
		m_pPointZ->WriteText( buf );
	}
}

// 删除一个点的回调函数
void CPathFindEditDlg::onDelDijkPoint(unsigned long ulPointId)
{
	// 只有当前区域是这个区域时才添加.
	if(m_ulCurRegionId == unsigned long(1))
	{
		DelRegionPoint(ulPointId);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ui界面， 事件响应函数
//
void CPathFindEditDlg::OnDelPointInRegion( wxCommandEvent &event )
{
	int iIndex = m_pListboxPointInRegion->GetSelection();
	CPointInfo* pInfo = m_pAction->GetRegionById( m_pAction->GetCurEditRegionId() )->GetPointInfoByIndex1( iIndex );
	m_pAction->DelPoint(pInfo->m_ulPointId);
}

void CPathFindEditDlg::OnEditXYZ( wxCommandEvent &event )
{
	Ogre::Vector3 pt;
	pt.x = (float)atof( m_pPointX->GetValue() );
	pt.z = (float)atof( m_pPointZ->GetValue() );

	float fTerrainDataPosX = m_pAction->getTerrainData()->mPosition.x;
	float fTerrainDataPosZ = m_pAction->getTerrainData()->mPosition.z;
	float fScale = 1.0;
	if( m_pAction->getSceneManipulator() )
	{
		fScale = m_pAction->getSceneManipulator()->getBaseScale();
		if(abs((float)fScale) < 0.00001) fScale = 1.0;
	}

	pt.x = pt.x * fScale + fTerrainDataPosX;
	pt.z = pt.z * fScale + fTerrainDataPosZ;

	m_pAction->ChangeCurEditPointPos(m_ulCurPointId, pt );
}


///////////////////////////////////////////////////////////////////////////////////////////
//
// 逻辑数据操作
//

// 添加一个区域.
void CPathFindEditDlg::AddNewRegion()
{
	//SetActive(true);

	if(m_pAction)
	{
		unsigned long ul = m_pAction->AddNewRegion();
		if( m_ulCurRegionId != ul )
		{
			if( m_ulCurRegionId != unsigned long(-1) )
				m_pAction->UnSelRegion(m_ulCurRegionId);
			m_ulCurRegionId = ul;

			m_iCurSelRegionIndex = 0;
			m_pListboxPointInRegion->Clear();
		}
	}
}

// 在当前的区域添加一个新的点
void CPathFindEditDlg::RegionAddNewPoint(unsigned long ulId, bool bRefresh)
{
	if(0 == ulId)
	{
		// 当前点的id不合法
		return;
	}

	// 添加一个新的区域
	char buf[256];
	if(m_pAction)
	{
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "第%d点", ulId);

		int iCount = m_pListboxPointInRegion->GetCount();
		m_pListboxPointInRegion->Insert(buf, iCount, (void*)ulId);

		// 当前编辑的点
		m_ulCurPointId = ulId;

		m_pListboxPointInRegion->Select(iCount);

		if(!bRefresh)
		{
			// 设置当前正在编辑区域
			SetEditRegionFlag(true);
		}

	}
}

// 初始化逻辑数据
void CPathFindEditDlg::InitData()
{
	m_ulCurPointId  = 0;	
	m_ulCurLineId   = 0;	
	m_bIsChangeCamera = true;
	
}


// 点选择listbox响应的事件函数
void CPathFindEditDlg::OnPointListBox(wxCommandEvent &event)
{
	if (event.GetInt() == -1)
	{
		return;
	}

	if(event.GetId() == ID_LISTBOX_POINT)
	{
		int iIndex = m_pListboxPointInRegion->GetSelection();
		if( iIndex != -1 )
		{
			CPointInfo* pInfo = m_pAction->GetRegionById( m_pAction->GetCurEditRegionId() )->GetPointInfoByIndex1( iIndex );
			if( pInfo )
			{
				float fTerrainDataPosX = m_pAction->getTerrainData()->mPosition.x;
				float fTerrainDataPosZ = m_pAction->getTerrainData()->mPosition.z;
				float fScale = 1.0;
				if( m_pAction->getSceneManipulator() )
				{
					fScale = m_pAction->getSceneManipulator()->getBaseScale();
					if(abs((float)fScale) < 0.00001) fScale = 1.0;
				}

				Ogre::Vector3 position = pInfo->GetPosition();
				char buf[128];
				sprintf( buf, "%.4f", (position.x - fTerrainDataPosX) / fScale );
				m_pPointX->Clear();
				m_pPointX->WriteText( buf );
				sprintf( buf, "%.4f", (position.z - fTerrainDataPosZ) / fScale );
				m_pPointZ->Clear();
				m_pPointZ->WriteText( buf );
			}

		}
		DoSelPoint();
	}
}

// 选中点的操作
void CPathFindEditDlg::DoSelPoint()
{
	if(NULL == m_pListboxPointInRegion)
	{
		return;
	}

	// 得到选中的索引
	int iIndex = m_pListboxPointInRegion->GetSelection();

	unsigned long ulPointId = 0;
	if(-1 != iIndex)
	{
		// 得到点id
		ulPointId = (unsigned long)m_pListboxPointInRegion->GetClientData(iIndex);
		if(m_pAction)
		{
			DWORD dwOldID = m_ulCurPointId;
			DWORD dwMax, dwMin;
			FLOAT fDistance = 0.0f;
			Ogre::Vector3 p1, p2;

			//设置选中点
			m_ulCurPointId = ulPointId;
			m_pAction->SelPoint(ulPointId);

			//求原先的选中点和新的选中点的折线距离
			CUnreachRegion* pRegion = m_pAction->GetRegionById( m_ulCurRegionId );
			if( dwOldID != m_ulCurPointId && pRegion )
			{
				//试图得到原先点和选中点间的每一个点
				dwMax = dwOldID > m_ulCurPointId ? dwOldID : m_ulCurPointId;
				dwMin = dwOldID > m_ulCurPointId ? m_ulCurPointId : dwOldID;
				for( DWORD dw = dwMin; dw <= dwMax; dw ++ )
				{
					CPointInfo* pPoint = pRegion->GetPointInfoById1(dw);
					if( !pPoint ) continue;
					if( dw == dwMin )
					{
						p1 = pPoint->GetPosition();
					}
					else
					{
						p2 = pPoint->GetPosition();
						fDistance += sqrtf( ( p1.x - p2.x ) * ( p1.x - p2.x ) + ( p1.y - p2.y ) * ( p1.y - p2.y ) + ( p1.z - p2.z ) * ( p1.z - p2.z ) );
						p1 = p2;
					}
				}
			}
		}
	}
}


// 清空数据
void CPathFindEditDlg::ClearData()
{
	ClearUiData();

	if(m_pAction)
	{
		m_pAction->ClearRegionData();
		m_pAction->m_ulCurEditRegionId = m_pAction->AddNewRegion();
	}
}

// 从RegionAction中刷新数据, 读盘的时候使用
void CPathFindEditDlg::RefreshData()
{
	//if((NULL == m_pAction)||(NULL == m_pListboxRegion))
	//{
	//	return;
	//}
	//// 清空ui界面数据.
	//ClearUiData();

	//UNREACHREGIONMAP::iterator itRegion;
	//UNREACHREGIONMAP::iterator itEndRegion;
	//unsigned long   ulId    = 0;
	//int				iCount  = 0;

	//CUnreachRegion* pRegion = NULL;


	//char buf[256];
	//// 存储每个区域的信息
	//itEndRegion  = m_pAction->m_UnreachRegionMap.end();
	//for(itRegion = m_pAction->m_UnreachRegionMap.begin(); itRegion != itEndRegion; itRegion++)
	//{	
	//	pRegion = itRegion->second;
	//	if(pRegion)
	//	{
	//		memset(buf, 0, sizeof(buf));

	//		// 存储区域的id
	//		ulId = pRegion->m_ulId;
	//		sprintf(buf, "区域%d", ulId);
	//		iCount = m_pListboxRegion->GetCount();
	//		m_pListboxRegion->Insert(buf, iCount, (void*)ulId);
	//	}
	//}

	//if(m_pListboxRegion->GetCount())
	//{
	//	m_pListboxRegion->SetSelection(0);
	//}
}


// 刷新区域中的点的数据
void CPathFindEditDlg::RefreshRegionPointData(unsigned long ulRegionId)
{
	//if(m_pAction)
	//{
	//	CUnreachRegion* pRegion = m_pAction->GetRegionById(ulRegionId);
	//	if(NULL == pRegion)
	//	{
	//		return ;
	//	}

	//	// 清空以前的数据.
	//	m_pListboxPointInRegion->Clear();

	//	int iPointCount = pRegion->GetPointCount1();
	//	CPointInfo* pPointInfo = NULL;
	//	for(int i = 0; i < iPointCount; i++)
	//	{
	//		pPointInfo = pRegion->GetPointInfoByIndex1(i);
	//		if(pPointInfo)
	//		{
	//			RegionAddNewPoint(pPointInfo->m_ulPointId, true);
	//		}
	//	}
	//}
}

// 清除ui界面的数据
void CPathFindEditDlg::ClearUiData()
{
	if(m_pListboxPointInRegion)
	{
		m_pListboxPointInRegion->Clear();
	}
}


// 读取数据
void CPathFindEditDlg::LoadData(std::string& strFile)
{
	if(m_pAction)
	{
		// 清空以前的数据.
		ClearData();

		if(0 == m_pAction->ReadRegionFromFile(strFile))
		{
			RefreshData();
		}
	}
}

// 存储数据
void CPathFindEditDlg::SaveData(std::string strFile)
{
	if(m_pAction)
	{
		m_pAction->SaveRegionToFile(strFile);
	}
}

void CPathFindEditDlg::GetRegionAction()
{
	// 得到区域编辑的场景控制
	if(m_pSceneManipulator)
	{
		WX::Action* action = m_pSceneManipulator->_getAction("PathFindEdit");
		m_pAction = (CPathFindAction *)action;
	}
}


// 删除区域中的一个点
void CPathFindEditDlg::DelRegionPoint(unsigned long ulPointId)
{
	if(m_pListboxPointInRegion)
	{
		int iCount = m_pListboxPointInRegion->GetCount();
		int iIndex = m_pListboxPointInRegion->GetSelection();;
		m_pListboxPointInRegion->Delete(iIndex);
		if (iCount > 1)
		{
			if(iIndex > 0)
			{
				iIndex --;
			}
			m_pListboxPointInRegion->Select(iIndex);
			unsigned long ulPointId = 0;
			ulPointId = (unsigned long)m_pListboxPointInRegion->GetClientData(iIndex);
			if(m_pAction)
			{
				m_pAction->SelPoint(ulPointId);
			}
		}
	}
}


// 设置正在编辑区域标记。
void CPathFindEditDlg::SetEditRegionFlag(bool bIsEdit)
{
	m_bIsEditingRegion = bIsEdit;
}



