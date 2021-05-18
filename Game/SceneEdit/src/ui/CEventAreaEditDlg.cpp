/////////////////////////////////////////////////////////////////////////////
// Name:        CEventAreaEditDlg.cpp
// Author:      XX
// Created:     XX/XX/XX
// Copyright:   XX
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "CEventAreaEditDlg.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "CEventAreaEditDlg.h"


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

#include "Core/Terrain.h"
#include "Core/TerrainData.h"

#include "SceneManipulator.h"
#include "Action.h"
#include "WXOperatorManager.h"
#include "wx/wxprec.h"
#include "RegionEditDlgSelector.h"
#include "res/NpcPatrolEdit_wdr.h"
#include "EventAreaEditAction.h"
#include "UnreachRegion.h"
#include "SceneManipulator.h"
#include "WXOperatorManager.h"

using namespace WX;

BEGIN_EVENT_TABLE(CEventAreaEditDlg, wxPanel)
 
	EVT_BUTTON(ID_BUTTON_NEW_AREA_ID,	CEventAreaEditDlg::OnAddNewId)
	EVT_BUTTON(ID_BUTTON_MODITY,		CEventAreaEditDlg::OnModifyArea)
	EVT_BUTTON(ID_BUTTON_DEL_RECT, CEventAreaEditDlg::OnDeleteRect)
	EVT_RADIOBUTTON(ID_RADIOBUTTON_SELECT, CEventAreaEditDlg::OnSelect)
	EVT_RADIOBUTTON(ID_RADIOBUTTON_DRAW, CEventAreaEditDlg::OnDraw)
	EVT_LIST_ITEM_SELECTED(ID_LISTCTRL, CEventAreaEditDlg::OnSelectEventAreaRect)
END_EVENT_TABLE()


CEventAreaEditDlg::CEventAreaEditDlg(const wxString& resourceGroup)
// 已经添加区域列表
: m_pRectListCtrl(NULL)
// 添加新id按钮.
,m_pAddNewEventAreaIdBn(NULL)

// 添加Select
,m_pSelectBn(NULL)

// 添加Draw
,m_pDrawBn(NULL)

// id控件
,m_pEventIdText(NULL)

// 脚本控件
,m_pScriptidText(NULL)

// 场景操作
,m_pSceneManipulator(NULL)

// 对应的action
,m_pEditAction(NULL)




{
    //Init(resourceGroup);
	
	//InitData();
}

CEventAreaEditDlg::CEventAreaEditDlg(
							wxWindow *parent, 
							const wxString& resourceGroup,
							wxWindowID winid,
							const wxPoint& pos,
							const wxSize& size,
							long style,
							const wxString& name
							)
							: m_pRectListCtrl(NULL)
// 添加新id按钮.
,m_pAddNewEventAreaIdBn(NULL)

// 添加Select
,m_pSelectBn(NULL)

// 添加Draw
,m_pDrawBn(NULL)

// id控件
,m_pEventIdText(NULL)

// 脚本控件
,m_pScriptidText(NULL)

// 场景操作
,m_pSceneManipulator(NULL)

// 对应的action
,m_pEditAction(NULL)

						
{
    //Init(resourceGroup);
    Create(parent, winid, pos, size, style, name);
	////////////////////////////////////////////////////////////////////////////////////
	//
	// 获取各控件的引用
	//

	// 获得区域控件的指针.
	m_pRectListCtrl = wxDynamicCast(FindWindow(ID_LISTCTRL), wxListCtrl);

	// 添加新id按钮
	m_pAddNewEventAreaIdBn = wxDynamicCast(FindWindow(ID_BUTTON_NEW_AREA_ID), wxButton);

	// // 添加Select
	//m_pSelectBn = wxDynamicCast(FindWindow(ID_RADIOBUTTON_SELECT), wxButton);

	//  // 添加Draw
	//m_pDrawBn = wxDynamicCast(FindWindow(ID_RADIOBUTTON_DRAW), wxButton);


	 // id控件
	m_pEventIdText  = wxDynamicCast(FindWindow(ID_TEXTCTRL_AREA_ID), wxTextCtrl);

	// 脚本控件
	m_pScriptidText = wxDynamicCast(FindWindow(ID_TEXTCTRL_SCRIPT_ID), wxTextCtrl);
	

	if(m_pEventIdText)
	{
		m_pEventIdText->SetLabel("0");
	}

	if(m_pScriptidText)
	{
		m_pScriptidText->SetLabel("-1");
	}//

	// 得到已经添加区域列表的指针.
	m_pRectListCtrl = wxDynamicCast(FindWindow(ID_LISTCTRL), wxListCtrl);

	m_pRectListCtrl->SetSingleStyle(wxLC_SINGLE_SEL, true);
	m_pRectListCtrl->InsertColumn(0, "事件区域");
    //m_pRectListCtrl->InsertColumn(1, "EventId");
	//m_pRectListCtrl->InsertColumn(2, "ScriptId");
    m_pRectListCtrl->InsertColumn(1, "事件ID");
	m_pRectListCtrl->InsertColumn(2, "脚本ID");

  }

void CEventAreaEditDlg::Init(const wxString& resourceGroup)
{
 

}


bool CEventAreaEditDlg::Create(
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

	EventArea(this, true, true);

    return true;
}

CEventAreaEditDlg::~CEventAreaEditDlg(void)
{

}



bool CEventAreaEditDlg::Show(bool show)
{
	SetActive(show);
	return wxWindow::Show(show);
}

void CEventAreaEditDlg::SetActive(bool bShow)
{
	if (bShow)
    {
		if(m_pSceneManipulator)
		{
			WX::Action* action = m_pSceneManipulator->_getAction("EventAreaEdit");
			m_pSceneManipulator->setActiveAction(action);//
			m_pEditAction = (CEventAreaEditAction *)action;
			
		}
       
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

void CEventAreaEditDlg::SetSceneManipulator(WX::SceneManipulator* sceneManipulator)
{
	m_pSceneManipulator = sceneManipulator;
	
	if(m_pSceneManipulator)
	{
		m_pSceneManipulator->addSceneListener(this);
		GetAction();
	}
}


/////////////////////////////////////////////////////////////////////////////////////////
//
// 场景编辑的回掉函数
//


///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  ui界面， 事件响应函数
//


/*******/
///////////////////////////////////////////////////////////////////////////////////////////
//
// 逻辑数据操作
//

// 初始化逻辑数据
void CEventAreaEditDlg::InitData()
{
	// 当前的区域id
	m_CurEventAreaId = 0;

	// 但当前的脚本id
	m_iCurScriptId   = 0;

	// 操作类型.
	m_OperatroType   = 1;
}

// 清空数据
void CEventAreaEditDlg::ClearData()
{
 
	// 清空界面数据
	this->ClearUI();

	// 清空action数据.
	if(m_pEditAction)
	{
		m_pEditAction->ReleaseRectInfoMap();
	}//

}



// 从RegionAction中刷新数据, 读盘的时候使用
void CEventAreaEditDlg::RefreshData()
{


}


// 清除ui界面的数据
void CEventAreaEditDlg::ClearUiData()
{

}


// 读取数据
void CEventAreaEditDlg::LoadData(std::string strFile)
{
	if(m_pEditAction)
	{
		m_pEditAction->ReadEventAreaFromFile(strFile);
		RefreshDataUI();
	}
}

// 存储数据
void CEventAreaEditDlg::SaveData(std::string strFile)
{
	if(m_pEditAction)
	{
		m_pEditAction->SaveEventAreaToFile(strFile);
	}
}

// 得到区域编辑的场景控制
void CEventAreaEditDlg::GetEventAreaAction()
{

}


// 按钮函数， 增加一个新id
void CEventAreaEditDlg::OnAddNewId( wxCommandEvent &event )
{
	if(m_pEditAction)
	{
		m_pEditAction->m_iMaxEventAreaId++;
		m_pEditAction->m_iCurEditEventAreaId = m_pEditAction->m_iMaxEventAreaId;

		char buf[64];
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%d", m_pEditAction->m_iCurEditEventAreaId);

		m_pEventIdText  = wxDynamicCast(FindWindow(ID_TEXTCTRL_AREA_ID), wxTextCtrl);
		if(m_pEventIdText)
		{
			m_pEventIdText->SetLabel(buf);
		}
	}
}


// 按钮函数， 增加一个新id
void CEventAreaEditDlg::OnModifyArea( wxCommandEvent &event )
{
	

	if(m_pEditAction)
	{
		//m_pEditAction->m_iCurEditEventAreaId);

		std::string strStlValue = "";
		int iEventid  = -1;
		int iScriptid = -1;

		strStlValue = m_pEventIdText->GetValue();
		iEventid    = atoi(strStlValue.c_str());
		strStlValue = m_pScriptidText->GetValue();
		iScriptid   = atoi(strStlValue.c_str());


		int iSelect = -1;
		int iItemCount = m_pRectListCtrl->GetItemCount();
		for(int i = 0; i < iItemCount; i++)
		{
			if(m_pEditAction->m_pCurOperatorRect->m_iId == m_pRectListCtrl->GetItemData(i))
			{
				iSelect = i;
				break;
			}
		}

		if(-1 != iSelect)
		{
			if(m_pEventIdText)
			{
				m_pRectListCtrl->SetItem(iSelect, 1, m_pEventIdText->GetValue().c_str());
			}

			// 脚本控件
			if(m_pScriptidText)
			{
				m_pRectListCtrl->SetItem(iSelect, 2, m_pScriptidText->GetValue().c_str());
			}
		}

		m_pEditAction->ModifyEventArea(m_pEditAction->m_pCurOperatorRect->m_iId, iEventid, iScriptid);

	}
}


// 按钮函数， 增加一个新id
void CEventAreaEditDlg::OnDeleteRect( wxCommandEvent &event )
{
	if((m_pEditAction)&&(m_pRectListCtrl))
	{
		int iSelItemIndex = m_pRectListCtrl->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		int iIndex = m_pRectListCtrl->GetItemData(iSelItemIndex);
		m_pEditAction->DeleteRect(iIndex);
		
	}
}









// 按钮函数， 选择
void CEventAreaEditDlg::OnSelect( wxCommandEvent &event )
{
	if(m_pEditAction)
	{
		m_pEditAction->m_iOperator = SELECT;
	}
}

// 按钮函数， 画
void CEventAreaEditDlg::OnDraw( wxCommandEvent &event )
{
	
	if(m_pEditAction)
	{
		m_pEditAction->m_iOperator = DRAW;
	}

}

//得到脚本id
std::string CEventAreaEditDlg::GetScriptIdValue()
{

	wxString strValue = "";
	std::string strStlValue = "";
	if(m_pScriptidText)
	{
		strValue = m_pScriptidText->GetValue();

		strStlValue = strValue.c_str();
		
	}//

	return strStlValue;
}

///////////////////////////////////////////////////////////////////////////////////////////
//
// 逻辑数据操作
//


// 设置编辑action的脚本id
void CEventAreaEditDlg::SetActionScriptValue()
{
	std::string strScriptValue = "-1";

	strScriptValue = GetScriptIdValue();
	char bufInfo[256];
	memset(bufInfo, 0, sizeof(bufInfo));
	sprintf(bufInfo, "%s", strScriptValue.c_str());
	int iScriptValue = atoi(bufInfo);
	int iEventId = 0;


	if(m_pEventIdText)
	{
		iEventId = atoi(m_pEventIdText->GetValue().c_str());
	}

	if(m_pEditAction)
	{
		m_pEditAction->m_iCurEditScriptid = iScriptValue;
		if(m_pEditAction->m_iMaxEventAreaId < iEventId)
		{
			m_pEditAction->m_iMaxEventAreaId = iEventId;

		}

		m_pEditAction->m_iCurEditEventAreaId = iEventId;

	}
	
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 界面响应回调函数
//

void CEventAreaEditDlg::onAddNewRect(unsigned long ulTypeId, unsigned long ulInstanceId)
{
	char bufInfo[200];
	memset(bufInfo, 0, sizeof(bufInfo));

	if(m_pRectListCtrl)
	{
		int iItemCount = m_pRectListCtrl->GetItemCount();
	
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "事件区域_%d", ulInstanceId);
		m_pRectListCtrl->InsertItem(iItemCount, bufInfo);
		//绑定怪物id
		m_pRectListCtrl->SetItemData(iItemCount, ulInstanceId);
		
		if(m_pEventIdText)
		{
			m_pRectListCtrl->SetItem(iItemCount, 1, m_pEventIdText->GetValue().c_str());
		}

		// 脚本控件
		if(m_pScriptidText)
		{
			m_pRectListCtrl->SetItem(iItemCount, 2, m_pScriptidText->GetValue().c_str());
		}
		
		m_pRectListCtrl->SetItemState(iItemCount, wxLIST_STATE_SELECTED , wxLIST_STATE_SELECTED);
		m_pRectListCtrl->EnsureVisible(iItemCount);
	}
}

	// 删除一个怪物
void CEventAreaEditDlg::onDelRect( unsigned long ulTypeId, unsigned long ulInstanceId)
{
	if(NULL == m_pRectListCtrl)
	{
		return;
	}

	int iItemCount = m_pRectListCtrl->GetItemCount();
	for(int i = 0; i < iItemCount; i++)
	{
		if(ulInstanceId == m_pRectListCtrl->GetItemData(i))
		{
			m_pRectListCtrl->DeleteItem(i);
			break;
		}
	}
}
	
	// 选择一个怪物
void CEventAreaEditDlg::onSelRect( unsigned long ulTypeId, unsigned long ulInstanceId)
{
	int iItemCount = m_pRectListCtrl->GetItemCount();
	for(int i = 0; i < iItemCount; i++)
	{
		if(ulInstanceId == m_pRectListCtrl->GetItemData(i))
		{
			m_pRectListCtrl->SetItemState(i, wxLIST_STATE_SELECTED , wxLIST_STATE_SELECTED);
			m_pRectListCtrl->EnsureVisible(i);
			break;
		}
	}

	if((m_pEditAction)&&(m_pEditAction->m_pCurOperatorRect))
	{
		char bufInfo[128];
		
		// 设置区域id
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "%d", m_pEditAction->m_pCurOperatorRect->m_iEventAreaId);
		m_pEventIdText->SetValue(bufInfo);

		// 设置脚本值
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "%d", m_pEditAction->m_pCurOperatorRect->m_iScriptId);
		m_pScriptidText->SetValue(bufInfo);
	}//

}

// 得到action
void CEventAreaEditDlg::GetAction()
{
	if(m_pSceneManipulator)
	{
		WX::Action* action = m_pSceneManipulator->_getAction("EventAreaEdit");
		m_pSceneManipulator->setActiveAction(action);//
		m_pEditAction = (CEventAreaEditAction *)action;
		
	}
}

// 清空界面
void CEventAreaEditDlg::ClearUI()
{

	if(m_pRectListCtrl)
	{
		m_pRectListCtrl->DeleteAllItems();
	}

}

// 刷新界面
void CEventAreaEditDlg::RefreshDataUI()
{
	ClearUI();

	int iItemCount = 0;
	if((m_pEditAction)&&(m_pRectListCtrl))
	{

		char bufInfo[256];
		iItemCount = m_pRectListCtrl->GetItemCount();
		CEventAreaInfo* pEventAreaInfo = NULL;
		SHOW_RECT_MAP::iterator itEventAreaEnd;
		SHOW_RECT_MAP::iterator it;

		itEventAreaEnd = m_pEditAction->m_RectShowMap.end();
		for(it = m_pEditAction->m_RectShowMap.begin(); it != itEventAreaEnd; it++)
		{
			pEventAreaInfo = it->second;
			if(pEventAreaInfo)
			{
				memset(bufInfo, 0, sizeof(bufInfo));
				sprintf(bufInfo, "%d", pEventAreaInfo->m_iId);
				m_pRectListCtrl->InsertItem(iItemCount, bufInfo);
				//绑定怪物id
				m_pRectListCtrl->SetItemData(iItemCount, iItemCount);
		
				memset(bufInfo, 0, sizeof(bufInfo));
				sprintf(bufInfo, "%d", pEventAreaInfo->m_iEventAreaId);
				m_pRectListCtrl->SetItem(iItemCount, 1, bufInfo);


				memset(bufInfo, 0, sizeof(bufInfo));
				sprintf(bufInfo, "%d", pEventAreaInfo->m_iScriptId);
				m_pRectListCtrl->SetItem(iItemCount, 2, bufInfo);
				
				m_pRectListCtrl->SetItemState(iItemCount, wxLIST_STATE_SELECTED , wxLIST_STATE_SELECTED);
				m_pRectListCtrl->EnsureVisible(iItemCount);
				iItemCount++;
						
			}
		}

	}
}


// 选择一个生长点的类型
void CEventAreaEditDlg::OnSelectEventAreaRect(wxListEvent& event)
{
	
	if(m_pRectListCtrl)
	{
		int iSelIndex = event.m_itemIndex;
		int iId = 0;
		if(iSelIndex >= 0)
		{
			iId = m_pRectListCtrl->GetItemData(iSelIndex);
			if(m_pEditAction)
			{
				m_pEditAction->SelectEventAreaRect(iId);
			}
		}
		
	}//
}

