/////////////////////////////////////////////////////////////////////////////
// Name:        NpcPatrolDlg.cpp
// Author:      XX
// Created:     XX/XX/XX
// Copyright:   XX
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "NpcPatrolDlg.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "NpcPatrolDlg.h"

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
#include "res/NpcPatrolEdit_wdr.h"
#include "NpcPatrolAction.h"
#include "UnreachRegion.h"

using namespace WX;

BEGIN_EVENT_TABLE(CNpcPatrolEditDlg, wxPanel)
	  EVT_BUTTON(ID_BUTTON_DEL_LINE, CNpcPatrolEditDlg::OnDelRegion)
	  EVT_BUTTON(ID_BN_ADD_NEW_LINE, CNpcPatrolEditDlg::OnAddNewRegion)
	  EVT_BUTTON(ID_BUTTON_DEL_POINT,  CNpcPatrolEditDlg::OnDelPointInRegion)
	  EVT_BUTTON(ID_BUTTON_LINE_OK,  CNpcPatrolEditDlg::OnFinishRegion)

	  EVT_LISTBOX(ID_LISTBOX_LINE, CNpcPatrolEditDlg::OnRegionListBox)
	  EVT_LISTBOX(ID_LISTBOX_POINT, CNpcPatrolEditDlg::OnPointListBox)
	  EVT_CHECKBOX(ID_IFY, CNpcPatrolEditDlg::OnPointUseY)
	  EVT_TEXT_ENTER(ID_POINT_X, CNpcPatrolEditDlg::OnEditXYZ)
	  EVT_TEXT_ENTER(ID_POINT_Y, CNpcPatrolEditDlg::OnEditXYZ)
	  EVT_TEXT_ENTER(ID_POINT_Z, CNpcPatrolEditDlg::OnEditXYZ)
	 
END_EVENT_TABLE()


CNpcPatrolEditDlg::CNpcPatrolEditDlg(const wxString& resourceGroup)
:m_pListboxRegion(NULL)
,m_pListboxPointInRegion(NULL)
,m_pDelPointBn(NULL)
,m_pSceneManipulator(NULL)
,m_pAction(NULL)
,m_pRegionOkBn(NULL)
,m_pDelRegionBn(NULL)
,m_pAddNewRegionBn(NULL)
,m_bIsEditingRegion(false)
,m_iCurSelRegionIndex(-1)
,m_pPointX(NULL)
,m_pPointY(NULL)
,m_pPointZ(NULL)
,m_pUseY(NULL)
,m_pDistance(NULL)

{
    //Init(resourceGroup);
	InitData();

	// 不可行走区域列表
	m_pListboxRegion = NULL;

	// 不可行走区域列表
	m_pListboxPointInRegion = NULL;

	// 删除点的按钮.
	m_pDelPointBn = NULL;

	// 确定完成编辑区域按钮.
	m_pRegionOkBn = NULL;

	// 删除点的按钮.
	m_pDelRegionBn = NULL;

	// 确定完成编辑区域按钮.
	m_pAddNewRegionBn = NULL;

	// 场景操作
	m_pSceneManipulator = NULL;

	m_pAction = NULL;



}

CNpcPatrolEditDlg::CNpcPatrolEditDlg(
							wxWindow *parent, 
							const wxString& resourceGroup,
							wxWindowID winid,
							const wxPoint& pos,
							const wxSize& size,
							long style,
							const wxString& name
							)
							:m_pListboxRegion(NULL)
							,m_pListboxPointInRegion(NULL)
							,m_pDelPointBn(NULL)
							,m_pSceneManipulator(NULL)
							,m_pAction(NULL)
							,m_pRegionOkBn(NULL)
							,m_pDelRegionBn(NULL)
							,m_pAddNewRegionBn(NULL)
							,m_bIsEditingRegion(false)
							,m_iCurSelRegionIndex(-1)
							,m_pPointX(NULL)
							,m_pPointY(NULL)
							,m_pPointZ(NULL)
							,m_pUseY(NULL)
							,m_pDistance(NULL)
{
    //Init(resourceGroup);


	// 不可行走区域列表
	m_pListboxRegion = NULL;

	// 不可行走区域列表
	m_pListboxPointInRegion = NULL;

	// 删除点的按钮.
	m_pDelPointBn = NULL;

	// 确定完成编辑区域按钮.
	m_pRegionOkBn = NULL;

	// 删除点的按钮.
	m_pDelRegionBn = NULL;

	// 确定完成编辑区域按钮.
	m_pAddNewRegionBn = NULL;

	// 场景操作
	m_pSceneManipulator = NULL;

	m_pAction = NULL;

    Create(parent, winid, pos, size, style, name);
	InitData();

}


// 完成一个区域的编辑
void CNpcPatrolEditDlg::FinishRegion()
{

	//if(m_pAction)
	//{
	//	if(m_pListboxRegion)
	//	{
	//		int iIndex = m_pListboxRegion->GetSelection();
	//		if(-1 != iIndex)
	//		{
	//			unsigned long uiId = (unsigned long)m_pListboxRegion->GetClientData(iIndex);
	//			if(!m_pAction->IsRegionValidate(uiId))
	//			{
	//				wxMessageBox(_T("当前区域含有不合法的点， 请改正。\n"), _T("正在编辑区域！"), wxOK, this);
	//				return;

	//			}
	//		}
	//	}
	//}//

	if(m_pAction)
	{
		if(m_pListboxRegion)
		{
			int iIndex = m_pListboxRegion->GetSelection();
			if(-1 != iIndex)
			{
				unsigned long uiId = (unsigned long)m_pListboxRegion->GetClientData(iIndex);
				//m_pAction->ReshapeArea(uiId);
				//m_pAction->HideRegionArea(uiId, false);//
			}
		}
		
	}
	
	if(m_pAddNewRegionBn)
	{
		m_bIsEditingRegion = false;
		m_pAddNewRegionBn->Enable( true );
	}

}


void CNpcPatrolEditDlg::Init(const wxString& resourceGroup)
{
 //   mSceneManipulator = NULL;
 //   mSplitter = NULL;
 //   mBrushesTree = NULL;
	//mBrushesPreview = NULL;

	//mFlipHorizontal = NULL;
	//mFlipVertical = NULL;
	//mRotateRightAngle = NULL;
	//mRandom = NULL;

	//mEnablePreview = NULL;

	//mCurrentOrientationType = 0;
	//mCurrentFileName = "";

 //   // 定义预览图的大小
 //   mPreviewImageWidth = 64;
 //   mPreviewImageHeight = 64;
	//// 初始化image，数据为黑色
	//mCurrentPreviewImage.Create( mPreviewImageWidth, mPreviewImageHeight, true );

 //   // 获得黑白图
 //   mBlackWhitePreviewImage = CellGridBitmaps(1).ConvertToImage();

 //   mResourceGroup = resourceGroup;

 //   mPaintInOneGrid = false;//
}


bool CNpcPatrolEditDlg::Create(
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

	EditRegionDlg(this, true, true);

	////////////////////////////////////////////////////////////////////////////////////
	//
	// 获取各控件的引用
	//

	// 获得区域控件的指针.
	m_pListboxRegion = wxDynamicCast(FindWindow(ID_LISTBOX_LINE), wxListBox);

	// 获得点控件的指针.
	m_pListboxPointInRegion = wxDynamicCast(FindWindow(ID_LISTBOX_POINT), wxListBox);

	// 得到删除点的控件的指针
	m_pDelPointBn =  wxDynamicCast(FindWindow(ID_BUTTON_DEL_POINT), wxButton);

	// 得到完成区域按钮指针
	m_pRegionOkBn = wxDynamicCast(FindWindow(ID_BUTTON_LINE_OK), wxButton);

	// 得到删除区域按钮指针。
	m_pDelRegionBn = wxDynamicCast(FindWindow(ID_BUTTON_DEL_LINE), wxButton);

	// 得到添加区域按钮的指针。
    m_pAddNewRegionBn = wxDynamicCast(FindWindow(ID_BN_ADD_NEW_LINE), wxButton);


	m_pPointX = wxDynamicCast(FindWindow(ID_POINT_X), wxTextCtrl);
	
	m_pPointY = wxDynamicCast(FindWindow(ID_POINT_Y), wxTextCtrl);
	
	m_pPointZ = wxDynamicCast(FindWindow(ID_POINT_Z), wxTextCtrl);

	m_pUseY = wxDynamicCast(FindWindow(ID_IFY), wxCheckBox);

	m_pDistance = wxDynamicCast(FindWindow(ID_DISTANCE), wxTextCtrl);

	//
	//if(m_pDelPointBn)
	//{
	//	wxWindow* pWindow = NULL;
	//	
	//	pWindow = (wxWindow*)m_pDelPointBn;
	//	pWindow->Show(false);
	//		
	//}//

	if(m_pListboxRegion)
	{
		//m_pListboxRegion->Insert("1111111111", 0);
		
		m_pListboxRegion->Clear();
	}

	if(m_pListboxPointInRegion)
	{
		//m_pListboxRegion->Insert("1111111111", 0);
		
		m_pListboxPointInRegion->Clear();
	}
	//mBrushesPreview = wxDynamicCast(this->FindWindow(feID_PIXMAP_PREVIEW), wxStaticBitmap);
	//mFlipHorizontal = wxDynamicCast(this->FindWindow(feID_FLIP_HORIZONTAL), wxCheckBox);
	//mFlipVertical = wxDynamicCast(this->FindWindow(feID_FLIP_VERTICAL), wxCheckBox);
	//mRotateRightAngle = wxDynamicCast(this->FindWindow(feID_ROTATE_RIGHT_ANGLE), wxCheckBox);
	//mRandom = wxDynamicCast(this->FindWindow(feID_RANDOM), wxCheckBox);

	//mEnablePreview = wxDynamicCast(this->FindWindow(feID_ENABLE_PIXMAP_PREVIEW), wxCheckBox);

 //   ReloadTextureList();//

	GetRegionAction();
    return true;
}

CNpcPatrolEditDlg::~CNpcPatrolEditDlg(void)
{
}


void CNpcPatrolEditDlg::OnDelRegion(wxCommandEvent &event)
{
	// test
	if(wxMessageBox(_T("你确定要删除这个区域吗？\n"), _T("删除区域警告！"), wxICON_QUESTION | wxYES_NO, this) == wxYES)
	{
		// 参数暂时没有用.
		DelRegion(0);
	}
}

bool CNpcPatrolEditDlg::Show(bool show)
{
	SetActive(show);
	return wxWindow::Show(show);
}


void CNpcPatrolEditDlg::ActiveAction(bool bActive)
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

void CNpcPatrolEditDlg::SetActive(bool bShow)
{

	if (bShow)
    {
		if(m_pSceneManipulator)
		{
			WX::Action* action = m_pSceneManipulator->_getAction("NpcPatrol");
			m_pSceneManipulator->setActiveAction(action);
			m_pAction = (CNpcPatrolAction *)action;
			
		}
       
    }
	else
	{
		if(m_pAction)
		{
			m_pAction->SetOldHitPointMaterial();
			//m_pAction->setActive(false);
			m_pSceneManipulator->setActiveAction(NULL);
		}
	}

	// 显示隐藏场景
	if(m_pAction)
	{
		m_pAction->HideScene(bShow);

		if(bShow)
		{
			// 根据地形的高度显示区域.
			m_pAction->SetRegionHeight();
		}
	}

}

void CNpcPatrolEditDlg::SetSceneManipulator(WX::SceneManipulator* sceneManipulator)
{
	m_pSceneManipulator = sceneManipulator;
	
	if(m_pSceneManipulator)
	{
		m_pSceneManipulator->addSceneListener(this);
		WX::Action* action = m_pSceneManipulator->_getAction("NpcPatrol");
		m_pAction = (CNpcPatrolAction *)action;
		SetActive(false);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////
//
// 场景编辑的回掉函数
//

void CNpcPatrolEditDlg::onAddObject(const WX::ObjectPtr& object)
{
	if(m_pAction)
	{
		//m_pAction->get
		;
	}
}



// 选中一个区域的回调函数。
void CNpcPatrolEditDlg::onRegionSel(unsigned long ulRegionId)
{

	if(NULL == m_pListboxRegion)
	{
		return;
	}
	
	unsigned long ulFindRegionId = 0;

	int iRegionCount = m_pListboxRegion->GetCount();

	for(int i = 0; i < iRegionCount; i++)
	{
		// 得到区域id
		ulFindRegionId = (unsigned long)m_pListboxRegion->GetClientData(i);
		if(ulFindRegionId == ulRegionId)
		{
			m_bIsChangeCamera = false;
			m_pListboxRegion->SetSelection(i);
			DoSelRegion();

			m_bIsChangeCamera = true;
			break;
		}
		
	}

}



// 区域编辑的回掉函数， 通知界面添加一个新的点.
void CNpcPatrolEditDlg::onRegionAddNewPoint(unsigned long ulRegionId,unsigned long ulPointId, Ogre::Vector3 position)
{
	// 只有当前区域是这个区域时才添加.
	if(m_ulCurRegionId == ulRegionId)
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
		if( position.y == -100000.0f )
		{
			m_pPointY->Clear();
			m_pPointY->Enable( false );
			m_pUseY->SetValue( false );
		}
		else
		{
			sprintf( buf, "%.4f", (position.y) / fScale );
			m_pPointY->Clear();
			m_pPointY->Enable( true );
			m_pPointY->WriteText( buf );
			m_pUseY->SetValue( true );
		}
		sprintf( buf, "%.4f", (position.z - fTerrainDataPosZ) / fScale );
		m_pPointZ->Clear();
		m_pPointZ->WriteText( buf );
	}
}

// 删除一个点的回调函数
void CNpcPatrolEditDlg::onRegionDelPoint(unsigned long ulRegionId,unsigned long ulPointId)
{
	// 只有当前区域是这个区域时才添加.
	if(m_ulCurRegionId == ulRegionId)
	{
		DelRegionPoint(ulPointId);
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ui界面， 事件响应函数
//

// 按钮函数， 添加一个区域.
void CNpcPatrolEditDlg::OnAddNewRegion( wxCommandEvent &event )
{
	AddNewRegion();
}


void CNpcPatrolEditDlg::OnFinishRegion( wxCommandEvent &event )
{
	FinishRegion();
}

void CNpcPatrolEditDlg::OnDelPointInRegion( wxCommandEvent &event )
{
	if(m_pSceneManipulator)
	{
		m_pSceneManipulator->getOperatorManager()->undo();
	}
	 
}

void CNpcPatrolEditDlg::OnPointUseY( wxCommandEvent &event )
{
	//if( m_bIsEditingRegion )
	//{
		CPointInfo* pInfo = 0;
		int iIndex = m_pListboxPointInRegion->GetSelection();
		if( iIndex != -1 )
			pInfo = m_pAction->GetRegionById( m_pAction->GetCurEditRegionId() )->GetPointInfoByIndex( iIndex );
		if( m_pUseY->GetValue() )
		{
			m_pUseY->SetValue( true );
			m_pPointY->Enable( true );
			if( pInfo ) pInfo->SetValuedY( true );
		}
		else
		{
			m_pUseY->SetValue( false );
			m_pPointY->Enable( false );
			if( pInfo ) pInfo->SetValuedY( false );
		}
	//}
	//else
	//{
	//	m_pUseY->SetValue( !m_pUseY->GetValue() );
	//}
}

void CNpcPatrolEditDlg::OnEditXYZ( wxCommandEvent &event )
{
	//m_bIsEditingRegion = true;

	Ogre::Vector3 pt;
	pt.x = (float)atof( m_pPointX->GetValue() );
	pt.y = (float)atof( m_pPointY->GetValue() );
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
	if( m_pUseY->GetValue() )
	{
		pt.y = pt.y * fScale;
	}
	else
	{
		pt.y = 0.0f;
	}

	m_pAction->ChangeCurEditPointPos( m_ulCurRegionId, m_ulCurPointId, pt );
}


/*******/
///////////////////////////////////////////////////////////////////////////////////////////
//
// 逻辑数据操作
//

// 删除一个区域
void CNpcPatrolEditDlg::DelRegion(unsigned long ulId)
{
	if(NULL == m_pListboxRegion)
	{
		return;
	}

	// 删除当前区域后需要选中的区域的index
	int iNextIndex   = -1;
	int iIndex       = m_pListboxRegion->GetSelection();
	int iRegionCount = m_pListboxRegion->GetCount();

	unsigned long ulRegionId = 0;
	// 删除当前区域后需要选中的区域的ID
	unsigned long ulNextRegionId = 0;

	if(-1 != iIndex)
	{
		if(iIndex > 0)
		{
			// 如果不是选择删除的头一个区域
			// 下一个选择的区域就是当前选择的前一个区域
			iNextIndex = iIndex-1;
		}
		else
		{
			if(iRegionCount > 1)
			{
				
				iNextIndex = iIndex+1;
				
			}
			else
			{
				iNextIndex = -1;
			}
		}

		if(-1 != iNextIndex)
		{
			ulNextRegionId = (unsigned long)m_pListboxRegion->GetClientData(iNextIndex);
		}

		// 得到区域id
		ulRegionId = (unsigned long)m_pListboxRegion->GetClientData(iIndex);
		if(m_pAction)
		{
			// 删除当前的region数据
			m_pAction->DelRegion(ulRegionId);
			
			
			
			//设置当前正在编辑的区域
			m_ulCurRegionId = ulNextRegionId;
			m_pAction->SelRegion(ulNextRegionId);

			// 清空以前的数据， 填充点的数据
			m_pListboxPointInRegion->Clear();

			
			// 得到下一个要选择的区域
			CUnreachRegion* pRegion = m_pAction->GetRegionById(ulNextRegionId);
			if(pRegion)
			{
				int iPointCount = pRegion->GetPointCount();
				CPointInfo* pPointInfo = NULL;
				for(int i = 0; i < iPointCount; i++)
				{
					pPointInfo = pRegion->GetPointInfoByIndex(i);
					if(pPointInfo)
					{
						RegionAddNewPoint(pPointInfo->m_ulPointId, true);
					}
				}//
			}
		
		}//if(m_pAction)

		if(-1 != iNextIndex)
		{
			m_pListboxRegion->SetSelection(iNextIndex);
			m_iCurSelRegionIndex = iNextIndex;
		}

		m_pListboxRegion->Delete(iIndex);

		// 设置当前没有编辑区域标记
		SetEditRegionFlag(false);
		

	}// if(-1 != iIndex)


}

// 添加一个区域.
void CNpcPatrolEditDlg::AddNewRegion()
{
	SetActive(true);
	
	// 添加一个新的区域
	char buf[256];

	if(m_pAction)
	{
		m_pAction->UnSelRegion(m_ulCurRegionId);
		m_ulCurRegionId = m_pAction->AddNewRegion();

		memset(buf, 0, sizeof(buf));
		sprintf(buf, "区域%d", m_ulCurRegionId);

		int iCount = m_pListboxRegion->GetCount();
		m_pListboxRegion->Insert(buf, iCount, (void*)m_ulCurRegionId);

		//m_pListboxRegion->Select(iCount);
		m_pListboxRegion->SetSelection(iCount);

		// 设置当前正在编辑的region索引.
		m_iCurSelRegionIndex = iCount;
		m_pListboxPointInRegion->Clear();

		if(m_pAddNewRegionBn)
		{
			m_bIsEditingRegion = true;
			m_pAddNewRegionBn->Enable( false );
		}
			
	}
}

// 在当前的区域添加一个新的点
void CNpcPatrolEditDlg::RegionAddNewPoint(unsigned long ulId, bool bRefresh)
{
	if(0 == ulId)
	{
		// 当前点的id不合法
		return;
	}

	if(0 == m_ulCurRegionId)
	{
		// 当前区域的id不合法
		return;
	}

	// 添加一个新的区域
	char buf[256];
	if(m_pAction)
	{
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "区域%d->点%d", m_ulCurRegionId, ulId);

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
void CNpcPatrolEditDlg::InitData()
{
	m_ulCurRegionId = 0;	
	m_ulCurPointId  = 0;	
	m_ulCurLineId   = 0;	

	m_bIsChangeCamera = true;



}


// 区域选择listbox响应的事件函数
void CNpcPatrolEditDlg::OnRegionListBox(wxCommandEvent &event)
{
	if (event.GetInt() == -1)
    {
       // "ListBox has no selections anymore
        return;
    }

	if(m_bIsEditingRegion)
	{
		wxMessageBox(_T("您正在编辑一个区域， 请点击完成区域编辑按钮\n"), _T("正在编辑区域！"), wxOK, this);

		if(-1 != m_iCurSelRegionIndex)
		{
			m_pListboxRegion->SetSelection(m_iCurSelRegionIndex);
		}

		return;
	}


    if(event.GetId() == ID_LISTBOX_LINE)
	{
		DoSelRegion();
	}

}


// 点选择listbox响应的事件函数
void CNpcPatrolEditDlg::OnPointListBox(wxCommandEvent &event)
{
	if (event.GetInt() == -1)
    {
       // "ListBox has no selections anymore
        return;
    }

	//if(m_bIsEditingRegion)
	//{
	//	return;
	//}

    if(event.GetId() == ID_LISTBOX_POINT)
	{
		int iIndex = m_pListboxPointInRegion->GetSelection();
		if( iIndex != -1 )
		{
			CPointInfo* pInfo = m_pAction->GetRegionById( m_pAction->GetCurEditRegionId() )->GetPointInfoByIndex( iIndex );
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
				if( pInfo->GetValuedY() )
				{
					sprintf( buf, "%.4f", (position.y ) / fScale );
					m_pPointY->Clear();
					m_pPointY->Enable( true );
					m_pPointY->WriteText( buf );
					m_pUseY->SetValue( true );
				}
				else
				{
					m_pPointY->Clear();
					m_pPointY->Enable( false );
				}
				sprintf( buf, "%.4f", (position.z - fTerrainDataPosZ) / fScale );
				m_pPointZ->Clear();
				m_pPointZ->WriteText( buf );
			}

		}

		DoSelPoint();
	}

}

// 选中点的操作
void CNpcPatrolEditDlg::DoSelPoint()
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
		// 得到区域id
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
					CPointInfo* pPoint = pRegion->GetPointInfoById(dw);
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
				char buf[128];
				sprintf( buf, "%.4f", fDistance / m_pAction->getSceneManipulator()->getBaseScale() );
				m_pDistance->Clear();
				m_pDistance->WriteText( buf );
			}
		}
	}
}


// 清空数据
void CNpcPatrolEditDlg::ClearData()
{
 
	ClearUiData();

	if(m_pAction)
	{
		m_pAction->ClearRegionData();
	}

}

// 做区域选中时的操作
void CNpcPatrolEditDlg::DoSelRegion()
{
	if(NULL == m_pListboxRegion)
	{
		return;
	}

	int iIndex = m_pListboxRegion->GetSelection();

	// 记录当前在listbox中选择的区域index
	m_iCurSelRegionIndex = iIndex;
	unsigned long ulRegionId = 0;
	if(-1 != iIndex)
	{

		// 得到区域id
		ulRegionId = (unsigned long)m_pListboxRegion->GetClientData(iIndex);
		if(m_pAction)
		{
			CUnreachRegion* pRegion = m_pAction->GetRegionById(ulRegionId);
			if(NULL == pRegion)
			{
				return ;
			}

			m_pAction->UnSelRegion(m_ulCurRegionId);
			// 当前正在编辑的区域
			m_ulCurRegionId = ulRegionId;
			m_pAction->SelRegion(ulRegionId, m_bIsChangeCamera);

			// 清空以前的数据.
			m_pListboxPointInRegion->Clear();

			int iPointCount = pRegion->GetPointCount();
			CPointInfo* pPointInfo = NULL;
			for(int i = 0; i < iPointCount; i++)
			{
				pPointInfo = pRegion->GetPointInfoByIndex(i);
				if(pPointInfo)
				{
					RegionAddNewPoint(pPointInfo->m_ulPointId, true);
				}

			}//
		
		}
	}
}

// 从RegionAction中刷新数据, 读盘的时候使用
void CNpcPatrolEditDlg::RefreshData()
{
	if((NULL == m_pAction)||(NULL == m_pListboxRegion))
	{
		return;
	}
	// 清空ui界面数据.
	ClearUiData();

	// 
	UNREACHREGIONMAP::iterator itRegion;
	UNREACHREGIONMAP::iterator itEndRegion;
	unsigned long   ulId    = 0;
	int				iCount  = 0;

	CUnreachRegion* pRegion = NULL;

		
	char buf[256];
	// 存储每个区域的信息
	itEndRegion  = m_pAction->m_UnreachRegionMap.end();
	for(itRegion = m_pAction->m_UnreachRegionMap.begin(); itRegion != itEndRegion; itRegion++)
	{	
		pRegion = itRegion->second;
		if(pRegion)
		{
			memset(buf, 0, sizeof(buf));
	
			// 存储区域的id
			ulId = pRegion->m_ulId;
			sprintf(buf, "区域%d", ulId);
			iCount = m_pListboxRegion->GetCount();
			m_pListboxRegion->Insert(buf, iCount, (void*)ulId);
			
			
			
		}
	}

	if(m_pListboxRegion->GetCount())
	{
		m_pListboxRegion->SetSelection(0);
	}
	

}


// 刷新区域中的点的数据
void CNpcPatrolEditDlg::RefreshRegionPointData(unsigned long ulRegionId)
{
	
	if(m_pAction)
	{

		CUnreachRegion* pRegion = m_pAction->GetRegionById(ulRegionId);
		if(NULL == pRegion)
		{
			return ;
		}
		
		// 清空以前的数据.
		m_pListboxPointInRegion->Clear();

		int iPointCount = pRegion->GetPointCount();
		CPointInfo* pPointInfo = NULL;
		for(int i = 0; i < iPointCount; i++)
		{
			pPointInfo = pRegion->GetPointInfoByIndex(i);
			if(pPointInfo)
			{
				RegionAddNewPoint(pPointInfo->m_ulPointId, true);
			}
		}//

	}




}

// 清除ui界面的数据
void CNpcPatrolEditDlg::ClearUiData()
{
	if(m_pListboxRegion)
	{
		m_pListboxRegion->Clear();
	}

	if(m_pListboxPointInRegion)
	{
		m_pListboxPointInRegion->Clear();
	}
}


// 读取数据
void CNpcPatrolEditDlg::LoadData(std::string& strFile)
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
void CNpcPatrolEditDlg::SaveData(std::string strFile)
{
	if(m_pAction)
	{
		m_pAction->SaveRegionToFile(strFile);
			
		//yangfei_begin
		std::string strNavMapFile = strFile;
		const char* dotChar = ".";
		std::basic_string <char>::size_type chBegin, chEnd;
		chBegin = strNavMapFile.find_last_of(dotChar);
		chEnd = strNavMapFile.npos;
		
		strNavMapFile.replace(chBegin,chEnd,".nav");
	
		m_pAction->SaveRegionToNavMapFile(strNavMapFile);
	}
}

void CNpcPatrolEditDlg::GetRegionAction()
{

	// 得到区域编辑的场景控制
	if(m_pSceneManipulator)
	{
		WX::Action* action = m_pSceneManipulator->_getAction("NpcPatrolEdit");
		m_pAction = (CNpcPatrolAction *)action;
	}

}


// 删除区域中的一个点
void CNpcPatrolEditDlg::DelRegionPoint(unsigned long ulPointId)
{
	if(m_pListboxPointInRegion)
	{
		int iCount = m_pListboxPointInRegion->GetCount();
		m_pListboxPointInRegion->Delete(iCount - 1);

		if(iCount > 1)
		{
			// 得到选中的索引
			int iIndex = iCount - 2;

			m_pListboxPointInRegion->Select(iIndex);
			unsigned long ulPointId = 0;
			if(-1 != iIndex)
			{
				// 得到区域id
				ulPointId = (unsigned long)m_pListboxPointInRegion->GetClientData(iIndex);
				if(m_pAction)
				{
					m_pAction->SelPoint(ulPointId);
				}
			}
		}
	}
}


// 设置正在编辑区域标记。
void CNpcPatrolEditDlg::SetEditRegionFlag(bool bIsEdit)
{
	m_bIsEditingRegion = bIsEdit;

	if(m_pAddNewRegionBn)
	{
		m_pAddNewRegionBn->Enable( !bIsEdit );
	}
}



