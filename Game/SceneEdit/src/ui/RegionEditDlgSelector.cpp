
// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "RegionEditDlgSelector.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h>.
#include <wx/wxprec.h>

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
#endif

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
// 
// ----------------------------------------------------------------------------
#include "Core/Terrain.h"
#include "Core/TerrainData.h"
#include "SceneManipulator.h"
#include "Action.h"
#include "WXOperatorManager.h"
#include "RegionEditDlgSelector.h"
#include "res/RegionEdit_wdr.h"
#include "RegionEditAction.h"
#include "UnreachRegion.h"
#include "SceneManipulator.h"
#include "WXOperatorManager.h"
// ----------------------------------------------------------------------------
// BrushSelector class
// ----------------------------------------------------------------------------
//
//#if wxUSE_STL
//#define AS_STRING(s)    (s)
//#else
//#define AS_STRING(s)    ((s).c_str())
//#endif
//
////static const int feID_PIXMAP_LIST = wxNewId();//
using namespace WX;


CRegionEditDlgSector* g_pRegionEdit = NULL;

BEGIN_EVENT_TABLE(CRegionEditDlgSector, wxPanel)
 //   EVT_UPDATE_UI(feID_PIXMAP_LIST, BrushSelector::OnUpdateControls)

	//EVT_CHECKBOX(feID_FLIP_HORIZONTAL, BrushSelector::OnCheck) 
	//EVT_CHECKBOX(feID_FLIP_VERTICAL, BrushSelector::OnCheck) 
	//EVT_CHECKBOX(feID_ROTATE_RIGHT_ANGLE, BrushSelector::OnCheck) 
	//EVT_CHECKBOX(feID_RANDOM, BrushSelector::OnCheck) 
 //   EVT_CHECKBOX(feID_ENABLE_PIXMAP_PREVIEW, BrushSelector::OnCheck) 

	//EVT_BUTTON(feID_RELOAD_PIXMAP_LIST, BrushSelector::OnReloadTextureList)
	//EVT_TREE_SEL_CHANGED(feID_PIXMAP_LIST, BrushSelector::OnPixmapListSelected)
 //   EVT_TREE_ITEM_RIGHT_CLICK(feID_PIXMAP_LIST, BrushSelector::OnRightButtonUp)

 //   EVT_MENU(feID_PIXMAP_FULLTERRAINPAINT, BrushSelector::OnFullTerrainPaint)
 //   EVT_MENU(feID_PIXMAP_PAINTINONEGRID, BrushSelector::OnPaintInOneGrid)//
	  EVT_BUTTON(ID_BUTTON_DEL_REGION, CRegionEditDlgSector::OnDelRegion)
	  EVT_BUTTON(ID_BN_ADD_NEW_REGION, CRegionEditDlgSector::OnAddNewRegion)
	  EVT_BUTTON(ID_BUTTON_DEL_POINT,  CRegionEditDlgSector::OnDelPointInRegion)
	  EVT_BUTTON(ID_BUTTON_REGION_OK,  CRegionEditDlgSector::OnFinishRegion)

	  EVT_LISTBOX(ID_LISTBOX_REGION, CRegionEditDlgSector::OnRegionListBox)
	  EVT_LISTBOX(ID_LISTBOX_POINT, CRegionEditDlgSector::OnPointListBox)

	  EVT_TEXT_ENTER(ID_CHK_FLYABLE, CRegionEditDlgSector::OnFlyable)
	 
END_EVENT_TABLE()


CRegionEditDlgSector::CRegionEditDlgSector(const wxString& resourceGroup)
:m_pListboxRegion(NULL)
,m_pListboxPointInRegion(NULL)
,m_pDelPointBn(NULL)
,m_pSceneManipulator(NULL)
,m_pRegionEditAction(NULL)
,m_pRegionOkBn(NULL)
,m_pDelRegionBn(NULL)
,m_pAddNewRegionBn(NULL)
,m_bIsEditingRegion(false)
,m_iCurSelRegionIndex(-1)

{
   
	//Init(resourceGroup);
	InitData();
}

CRegionEditDlgSector::CRegionEditDlgSector(
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
							,m_pRegionEditAction(NULL)
							,m_pRegionOkBn(NULL)
							,m_pDelRegionBn(NULL)
							,m_pAddNewRegionBn(NULL)
							,m_bIsEditingRegion(false)
							,m_iCurSelRegionIndex(-1)
{
   	//Init(resourceGroup);
    Create(parent, winid, pos, size, style, name);
	InitData();

}


// 完成一个区域的编辑
void CRegionEditDlgSector::FinishRegion()
{

	if(m_pRegionEditAction)
	{
		if(m_pListboxRegion)
		{
			int iIndex = m_pListboxRegion->GetSelection();
			if(-1 != iIndex)
			{
				unsigned long uiId = (unsigned long)m_pListboxRegion->GetClientData(iIndex);
				if(!m_pRegionEditAction->IsRegionValidate(uiId))
				{
					wxMessageBox(_T("当前区域含有不合法的点， 请改正。\n"), _T("正在编辑区域！"), wxOK, this);
					return;

				}
			}
		}
	}

	if(m_pRegionEditAction)
	{
		if(m_pListboxRegion)
		{
			int iIndex = m_pListboxRegion->GetSelection();
			if(-1 != iIndex)
			{
				unsigned long uiId = (unsigned long)m_pListboxRegion->GetClientData(iIndex);//fujia
				CUnreachRegion* pRegion = m_pRegionEditAction->GetRegionById(uiId);
				if( pRegion )
				{
					int PassLevel =  Ogre::StringConverter::parseInt(m_pFlyableCB->GetValue().c_str());
					if (PassLevel < 1) PassLevel = 1;
					if (PassLevel > 10) PassLevel = 10;
					pRegion->SetFlyable(PassLevel);
				}
				m_pRegionEditAction->ReshapeArea(uiId);
				m_pRegionEditAction->HideRegionArea(uiId, false);
			}
		}
		
	}
	
	if(m_pAddNewRegionBn)
	{
		m_bIsEditingRegion = false;
		m_pAddNewRegionBn->Enable( true );
	}
}


void CRegionEditDlgSector::Init(const wxString& resourceGroup)
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


bool CRegionEditDlgSector::Create(
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

	EditRegion(this, true, true);

	////////////////////////////////////////////////////////////////////////////////////
	//
	// 获取各控件的引用
	//

	// 获得区域控件的指针.
	m_pListboxRegion = wxDynamicCast(FindWindow(ID_LISTBOX_REGION), wxListBox);

	// 获得点控件的指针.
	m_pListboxPointInRegion = wxDynamicCast(FindWindow(ID_LISTBOX_POINT), wxListBox);

	// 得到删除点的控件的指针
	m_pDelPointBn =  wxDynamicCast(FindWindow(ID_BUTTON_DEL_POINT), wxButton);

	// 得到完成区域按钮指针
	m_pRegionOkBn = wxDynamicCast(FindWindow(ID_BUTTON_REGION_OK), wxButton);

	// 得到删除区域按钮指针。
	m_pDelRegionBn = wxDynamicCast(FindWindow(ID_BUTTON_DEL_REGION), wxButton);

	// 得到添加区域按钮的指针。
    m_pAddNewRegionBn = wxDynamicCast(FindWindow(ID_BN_ADD_NEW_REGION), wxButton);

	// 得到是否能轻功通过CheckBox指针
	m_pFlyableCB = wxDynamicCast(FindWindow(ID_CHK_FLYABLE), wxTextCtrl);

	wxListBox *pListBox = wxDynamicCast(FindWindow(ID_LISTBOX_Line), wxListBox);
	if(pListBox)
	{
		pListBox->Show(false);
	}
	wxChoice  *pChoice = wxDynamicCast(FindWindow(ID_CHOICE_LINE_PROPERTY), wxChoice); 
	if(pChoice)
	{
		pChoice->Show(false);
	}
  
  


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

    return true;
}

CRegionEditDlgSector::~CRegionEditDlgSector(void)
{
}

void CRegionEditDlgSector::DelRegion()
{
	if(wxMessageBox(_T("你确定要删除这个区域吗？\n"), _T("删除区域警告！"), wxICON_QUESTION | wxYES_NO, this) == wxYES)
	{
		// 参数暂时没有用.
		DelRegion(0);
	}
}
void CRegionEditDlgSector::OnDelRegion(wxCommandEvent &event)
{
	// test
	if(wxMessageBox(_T("你确定要删除这个区域吗？\n"), _T("删除区域警告！"), wxICON_QUESTION | wxYES_NO, this) == wxYES)
	{
		// 参数暂时没有用.
		DelRegion(0);
	}
}

bool CRegionEditDlgSector::Show(bool show)
{
	SetActive(show);
	return wxWindow::Show(show);
}

void CRegionEditDlgSector::SetActive(bool bShow)
{
	if (bShow)
    {
		if(m_pSceneManipulator)
		{
			WX::Action* action = m_pSceneManipulator->_getAction("RegionEdit");
			////action->setParameter("type", "Actor");
			////action->setParameter("%actor name", name);
			m_pSceneManipulator->setActiveAction(action);//
			m_pRegionEditAction = (CRegionEditAction *)action;
			
		}
       
    }
	else
	{
		if(m_pRegionEditAction)
		{
			m_pRegionEditAction->SetOldHitPointMaterial();
			//m_pRegionEditAction->setActive(false);
			m_pSceneManipulator->setActiveAction(NULL);
		}
	}

	// 显示隐藏场景
	if(m_pRegionEditAction)
	{
		m_pRegionEditAction->HideScene(bShow);

		if(bShow)
		{
			// 根据地形的高度显示区域.
			m_pRegionEditAction->SetRegionHeight();
		}
	}

}

void CRegionEditDlgSector::SetSceneManipulator(WX::SceneManipulator* sceneManipulator)
{
	m_pSceneManipulator = sceneManipulator;
	
	if(m_pSceneManipulator)
	{
		m_pSceneManipulator->addSceneListener(this);
		GetRegionAction();
	}
}


/////////////////////////////////////////////////////////////////////////////////////////
//
// 场景编辑的回掉函数
//

void CRegionEditDlgSector::onAddObject(const WX::ObjectPtr& object)
{
	if(m_pRegionEditAction)
	{
		//m_pRegionEditAction->get
		;
	}
}



// 选中一个区域的回调函数。
void CRegionEditDlgSector::onRegionSel(unsigned long ulRegionId)
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
void CRegionEditDlgSector::onRegionAddNewPoint(unsigned long ulRegionId,unsigned long ulPointId, Ogre::Vector3 position)
{
	// 只有当前区域是这个区域时才添加.
	if(m_ulCurRegionId == ulRegionId)
	{
		RegionAddNewPoint(ulPointId);
	}
}

// 删除一个点的回调函数
void CRegionEditDlgSector::onRegionDelPoint(unsigned long ulRegionId,unsigned long ulPointId)
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
void CRegionEditDlgSector::OnAddNewRegion( wxCommandEvent &event )
{
	AddNewRegion();
}


void CRegionEditDlgSector::OnFinishRegion( wxCommandEvent &event )
{
	FinishRegion();
}

void CRegionEditDlgSector::OnDelPointInRegion( wxCommandEvent &event )
{
	if(m_pSceneManipulator)
	{
		m_pSceneManipulator->getOperatorManager()->undo();
	}
	 
}

void CRegionEditDlgSector::OnFlyable( wxCommandEvent &event )
{
	if(m_pRegionEditAction)
	{
		int iIndex = m_pListboxRegion->GetSelection();
		if( iIndex != -1 )
		{
			unsigned long ulRegionId = (unsigned long)m_pListboxRegion->GetClientData(iIndex);
			CUnreachRegion* pRegion = m_pRegionEditAction->GetRegionById(ulRegionId);
			if( pRegion )
			{
				int PassLevel =  Ogre::StringConverter::parseInt(m_pFlyableCB->GetValue().c_str());
				if (PassLevel < 1) PassLevel = 1;
				if (PassLevel > 10) PassLevel = 10;
				pRegion->SetFlyable( PassLevel );
			}
		}
	}
}


/*******/
///////////////////////////////////////////////////////////////////////////////////////////
//
// 逻辑数据操作
//

// 删除一个区域
void CRegionEditDlgSector::DelRegion(unsigned long ulId)
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
		if(m_pRegionEditAction)
		{
			// 删除当前的region数据
			m_pRegionEditAction->DelRegion(ulRegionId);
			
			
			
			//设置当前正在编辑的区域
			m_ulCurRegionId = ulNextRegionId;
			m_pRegionEditAction->SelRegion(ulNextRegionId);

			// 清空以前的数据， 填充点的数据
			m_pListboxPointInRegion->Clear();

			
			// 得到下一个要选择的区域
			CUnreachRegion* pRegion = m_pRegionEditAction->GetRegionById(ulNextRegionId);
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
		
		}//if(m_pRegionEditAction)

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
void CRegionEditDlgSector::AddNewRegion()
{
	SetActive(true);
	
	// 添加一个新的区域
	char buf[256];

	if(m_pRegionEditAction)
	{
		m_pRegionEditAction->UnSelRegion(m_ulCurRegionId);
		m_ulCurRegionId = m_pRegionEditAction->AddNewRegion();

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
void CRegionEditDlgSector::RegionAddNewPoint(unsigned long ulId, bool bRefresh)
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
	if(m_pRegionEditAction)
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
void CRegionEditDlgSector::InitData()
{
	m_ulCurRegionId = 0;	
	m_ulCurPointId  = 0;	
	m_ulCurLineId   = 0;	

	m_bIsChangeCamera = true;



}


// 区域选择listbox响应的事件函数
void CRegionEditDlgSector::OnRegionListBox(wxCommandEvent &event)
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


    if(event.GetId() == ID_LISTBOX_REGION)
	{
		DoSelRegion();
	}

}


// 点选择listbox响应的事件函数
void CRegionEditDlgSector::OnPointListBox(wxCommandEvent &event)
{
	if (event.GetInt() == -1)
    {
       // "ListBox has no selections anymore
        return;
    }

	if(m_bIsEditingRegion)
	{
		return;
	}

    if(event.GetId() == ID_LISTBOX_POINT)
	{
		DoSelPoint();
	}

}

// 选中点的操作
void CRegionEditDlgSector::DoSelPoint()
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
		if(m_pRegionEditAction)
		{
			m_pRegionEditAction->SelPoint(ulPointId);
		}
	}
}


// 清空数据
void CRegionEditDlgSector::ClearData()
{
 
	ClearUiData();

	if(m_pRegionEditAction)
	{
		m_pRegionEditAction->ClearRegionData();
	}

}

// 做区域选中时的操作
void CRegionEditDlgSector::DoSelRegion()
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
		if(m_pRegionEditAction)
		{
			CUnreachRegion* pRegion = m_pRegionEditAction->GetRegionById(ulRegionId);
			if(NULL == pRegion)
			{
				return ;
			}

			m_pRegionEditAction->UnSelRegion(m_ulCurRegionId);
			// 当前正在编辑的区域
			m_ulCurRegionId = ulRegionId;
			m_pRegionEditAction->SelRegion(ulRegionId, m_bIsChangeCamera);

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

			//pRegion->SetFlyable( m_pFlyableCB->GetValue() );
			m_pFlyableCB->SetValue( Ogre::StringConverter::toString( pRegion->GetFlyable()) );
		
		}
	}
}

// 从RegionAction中刷新数据, 读盘的时候使用
void CRegionEditDlgSector::RefreshData()
{
	if((NULL == m_pRegionEditAction)||(NULL == m_pListboxRegion))
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
	itEndRegion  = m_pRegionEditAction->m_UnreachRegionMap.end();
	for(itRegion = m_pRegionEditAction->m_UnreachRegionMap.begin(); itRegion != itEndRegion; itRegion++)
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
void CRegionEditDlgSector::RefreshRegionPointData(unsigned long ulRegionId)
{
	
	if(m_pRegionEditAction)
	{

		CUnreachRegion* pRegion = m_pRegionEditAction->GetRegionById(ulRegionId);
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
void CRegionEditDlgSector::ClearUiData()
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
void CRegionEditDlgSector::LoadData(std::string strFile)
{
	if(m_pRegionEditAction)
	{
		// 清空以前的数据.
		ClearData();
		
		if(0 == m_pRegionEditAction->ReadRegionFromFile(strFile))
		{
			RefreshData();
		}
	}
}

// 存储数据
void CRegionEditDlgSector::SaveData(std::string strFile)
{
	if(m_pRegionEditAction)
	{
		m_pRegionEditAction->SaveRegionToFile(strFile);
			
		//yangfei_begin
		std::string strNavMapFile = strFile;
		const char* dotChar = ".";
		std::basic_string <char>::size_type chBegin, chEnd;
		chBegin = strNavMapFile.find_last_of(dotChar);
		chEnd = strNavMapFile.npos;
		
		strNavMapFile.replace(chBegin,chEnd,".nav");
	
		m_pRegionEditAction->SaveRegionToNavMapFile(strNavMapFile);
	}
}

void CRegionEditDlgSector::GetRegionAction()
{

	// 得到区域编辑的场景控制
	WX::Action* action = m_pSceneManipulator->_getAction("RegionEdit");
	m_pRegionEditAction = (CRegionEditAction *)action;

}


// 删除区域中的一个点
void CRegionEditDlgSector::DelRegionPoint(unsigned long ulPointId)
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
				if(m_pRegionEditAction)
				{
					m_pRegionEditAction->SelPoint(ulPointId);
				}
			}
		}
	}
}


// 设置正在编辑区域标记。
void CRegionEditDlgSector::SetEditRegionFlag(bool bIsEdit)
{
	m_bIsEditingRegion = bIsEdit;

	if(m_pAddNewRegionBn)
	{
		m_pAddNewRegionBn->Enable( !bIsEdit );
	}
}




//// 按钮函数， 删除一个点.
//void OnDelPointInRegion( wxCommandEvent &event )
//{
//
//}//