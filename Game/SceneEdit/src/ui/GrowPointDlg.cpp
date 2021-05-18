/////////////////////////////////////////////////////////////////////////////
// Name:        GrowPointDlg.
// Author:      XX
// Created:     XX/XX/XX
// Copyright:   XX
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "GrowPointDlg.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "GrowPointDlg.h"

#include <wx/treectrl.h>
#include <wx/listctrl.h>

#include "GrowPointEditAction.h"
#include "SceneManipulator.h"
#include "WXEditorFrame.h"
#include "WXSceneCanvas.h"
#include "TLBB_DBC.h"				// 读取excel
#include "SceneInfoMapDialog.h"		// 导航图界面.
#include "RegionEditAction.h"

// WDR: class implementations


BEGIN_EVENT_TABLE(CGrowPointEditDlg, wxPanel)

 	 EVT_LIST_ITEM_SELECTED(ID_LISTCTRL_GROW_POINT,		  CGrowPointEditDlg::OnGrowPointTypeSelected)
	 EVT_TREE_SEL_CHANGED(ID_TREECTRL_ADDED_GROW_POINT,   CGrowPointEditDlg::OnGrowPointAddedSelected)

END_EVENT_TABLE()

CGrowPointEditDlg::CGrowPointEditDlg(const wxString& resourceGroup)
: m_pGrowPointListCtrl(NULL)
, m_pAddedGrowPointTreeCtrl(NULL)
, m_pSceneManipulator(NULL)
, m_pGrowPointEditAction(NULL)
, m_pNavigationDlg(NULL)
, m_pParent(NULL)

{
    //Init(resourceGroup);
	//InitData();
}

CGrowPointEditDlg::CGrowPointEditDlg(
							wxWindow *parent, 
							const wxString& resourceGroup,
							wxWindowID winid,
							const wxPoint& pos,
							const wxSize& size,
							long style,
							const wxString& name
							)
: m_pGrowPointListCtrl(NULL)
, m_pAddedGrowPointTreeCtrl(NULL)
, m_pSceneManipulator(NULL)
, m_pGrowPointEditAction(NULL)
, m_pNavigationDlg(NULL)
, m_pParent(NULL)
						
										
{
    Create(parent, winid, pos, size, style, name);
	//InitData();

}


bool CGrowPointEditDlg::Create(
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
	GrowPointDlg(this, true, true);
	
	//// 获取各控件的引用
	//GetAllControl();

	//// 初始化怪物列表的表头.
	//InitMonsterListCtrlTable();

	//// 读取怪物id和外形映射文件数据
	//LoadMonsterIdNameTable();

	//// 初始化tree结构
	//InitAddedMonsterTree();

	// 得到所有的控件.
	GetAllControl();

	// 初始化怪物列表的表头.
	InitGrowPointListCtrlTable();

	// 初始化树型列表
	InitAddedGrowPointTree();

	GetAction();
	return true;
}

// 显示窗口
bool CGrowPointEditDlg::Show(bool show)
{
	SetActive(show);
	return wxWindow::Show(show);
}


// 初始化怪物列表的表头.
void CGrowPointEditDlg::InitGrowPointListCtrlTable()
{
	m_pGrowPointListCtrl->InsertColumn(0, "编号");	
    m_pGrowPointListCtrl->InsertColumn(1, "名称");	
	m_pGrowPointListCtrl->InsertColumn(2, "外形");	


	DBC::DBCFile excelDB(1);
	if(!excelDB.OpenFromTXT("..\\..\\public\\config\\GrowPoint.txt"))
	{
		wxLogMessage(_T("不能打开生长点信息表， 不能编辑生长点信息\n"));
        Enable(false);
        Hide();
		return;
	}


	m_IdNameMap.clear();
	m_IdMeshMap.clear();

	int iRecordCount = excelDB.GetRecordsNum(); 
	const DBC::DBCFile::FIELD* pInfo = NULL;	
	char bufInfo[512];
	int  iId  = 0;



	for(int i = 0; i < iRecordCount; i++)
	{
		// 读取id
		pInfo = excelDB.Search_Posistion(i, 0);
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "%d", pInfo->iValue);
		m_pGrowPointListCtrl->InsertItem(i, bufInfo);
		iId = pInfo->iValue;
		//id
		m_pGrowPointListCtrl->SetItemData(i, pInfo->iValue);
		

		// 读取名字
		pInfo = excelDB.Search_Posistion(i, 1);
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "%s", pInfo->pString);
		m_pGrowPointListCtrl->SetItem(i, 1, bufInfo);
		m_IdNameMap[iId] = bufInfo;

		// 读取外形.
		pInfo = excelDB.Search_Posistion(i, 2);
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "%s", pInfo->pString);
		m_pGrowPointListCtrl->SetItem(i, 2, bufInfo);
		m_IdMeshMap[iId] = bufInfo;
		
		
	}
	
}


// 得到所有的控件.
void CGrowPointEditDlg::GetAllControl()
{
	// 得到怪物列表控件.
	m_pGrowPointListCtrl = wxDynamicCast(FindWindow(ID_LISTCTRL_GROW_POINT), wxListCtrl);

	// 获得已经添加的怪物列表的控件.
	m_pAddedGrowPointTreeCtrl =  wxDynamicCast(FindWindow(ID_TREECTRL_ADDED_GROW_POINT), wxTreeCtrl);


}

// 初始化怪物列表树.
void CGrowPointEditDlg::InitAddedGrowPointTree()
{

	if(m_pAddedGrowPointTreeCtrl)
	{
		m_pAddedGrowPointTreeCtrl->DeleteAllItems();
		m_AddedGrowPointRoot = m_pAddedGrowPointTreeCtrl->AddRoot("已经添加的生长点");

		ID_NAME_DLG_MAP::iterator it;
		ID_NAME_DLG_MAP::iterator itEnd;
		wxTreeItemId newTreeItemId;
		std::string  strName = "";

		m_IdTreeItemMap.clear();
		itEnd = m_IdNameMap.end();
		for(it = m_IdNameMap.begin(); it != itEnd; it++)
		{
			strName = it->second;
			newTreeItemId = m_pAddedGrowPointTreeCtrl->AppendItem(m_AddedGrowPointRoot, strName.c_str());
			m_IdTreeItemMap[it->first] = newTreeItemId;
		}

	}//

}


// 得到action
void CGrowPointEditDlg::GetAction()
{
	// 得到区域编辑的场景控制
	if(m_pSceneManipulator)
	{
		WX::Action* action = m_pSceneManipulator->_getAction("GrowPointEdit");
		m_pGrowPointEditAction = (WX::CGrowPointEditAction *)action;
	}
}

// 设置场景操作句柄
void CGrowPointEditDlg::SetSceneManipulator(WX::SceneManipulator* sceneManipulator)
{
	m_pSceneManipulator = sceneManipulator;
	
	if(m_pSceneManipulator)
	{
		m_pSceneManipulator->addSceneListener(this);
		
		// 得到底层的action
		GetAction();
		SetActionMeshType();
		SetActive(false);
	}
}


// 激活场景
void CGrowPointEditDlg::SetActive(bool bShow)
{

	if(bShow)
    {
		if(m_pSceneManipulator)
		{
			WX::Action* action = m_pSceneManipulator->_getAction("GrowPointEdit");
			m_pSceneManipulator->setActiveAction(action);//
				
		}
       
    }
	else
	{
		if(m_pGrowPointEditAction)
		{
			m_pSceneManipulator->setActiveAction(NULL);
		}
	}

	// 显示隐藏场景
	if(m_pGrowPointEditAction)
	{
		m_pGrowPointEditAction->HideScene(bShow);

		WX::Action* action = m_pSceneManipulator->_getAction("RegionEdit");
		WX::CRegionEditAction* pRegionEditAction = ( WX::CRegionEditAction*)action;
		pRegionEditAction->HideScene(bShow);

		if(bShow)
		{
			
		}
	}

}


 // 选择已经添加生长点
void CGrowPointEditDlg::OnGrowPointAddedSelected(wxTreeEvent& event)
{

	if(m_pAddedGrowPointTreeCtrl)
	{
		wxTreeItemId TreeItem = event.GetItem();
		
		wxString strInfo = m_pAddedGrowPointTreeCtrl->GetItemText(TreeItem);

		unsigned long ulGrowPointInstance = 0;

		char szBuf[512];

		memset(szBuf, 0, sizeof(szBuf));
		sprintf(szBuf, "%s", strInfo.c_str());

		sscanf(szBuf, "%d", &ulGrowPointInstance);

		if(m_pGrowPointEditAction)
		{
			if(m_pGrowPointEditAction->m_GrowPointMap.count(ulGrowPointInstance))
			{
				m_pGrowPointEditAction->ViewGrowPos(ulGrowPointInstance);
				m_pGrowPointEditAction->SetUnSelectObjectLook(m_pGrowPointEditAction->m_dwCurSelGrowPointInstanceId);
				m_pGrowPointEditAction->SetSelectObjectLook(ulGrowPointInstance);
				m_pGrowPointEditAction->m_dwCurSelGrowPointInstanceId = ulGrowPointInstance;

				if(m_pParent)
				{
					m_pParent->SetFocus();
					m_pParent->GetCanvas()->SetFocus();
					m_pParent->GetCanvas()->Enable(true);
					
				}
			}
		}
		
	}//



}



// 选择一个生长点的类型
void CGrowPointEditDlg::OnGrowPointTypeSelected(wxListEvent& event)
{
	if(m_pGrowPointListCtrl)
	{
		int iSelIndex = event.m_itemIndex;
		int iId = 0;
		if(iSelIndex >= 0)
		{
			iId = m_pGrowPointListCtrl->GetItemData(iSelIndex);
			if(m_IdMeshMap.count(iId))
			{
				if(m_pGrowPointEditAction)
				{
					std::string strMeshName = m_IdMeshMap[iId];
					m_pGrowPointEditAction->ChangeGrowPointMeshType(iId, strMeshName);
					
				}
			}

		}
		
	}//
}

void CGrowPointEditDlg::AddNewGrowPoint(unsigned long ulMonsterId, unsigned long ulInstanceId)
{

	Ogre::Vector3 pos(0, 0, 0);

	if(NULL == m_pAddedGrowPointTreeCtrl)
	{
		return;
	}

	if(m_pGrowPointEditAction)
	{
		pos = m_pGrowPointEditAction->GetGrowLogicPos(ulInstanceId);
	}

	if(0 == m_IdTreeItemMap.count(ulMonsterId))
	{
		// 在列表中找不到对应的怪物名称。
		wxString strAddNew;

		// 加入一个数据.
		strAddNew.Format(wxT("%d, x=%.2f, y=%.2f, z=%.2f"), ulInstanceId, pos.x, pos.y, pos.z);
		wxTreeItemId NewItemId = m_pAddedGrowPointTreeCtrl->AppendItem(m_AddedGrowPointRoot, strAddNew);
		m_IdTreeItemMap[ulInstanceId] = NewItemId;

	}
	else
	{
		wxString strAddNew			= "";
		wxString strCount			= "";
		wxString strOldName			= "";
		std::string strMonsterName  = "";
		wxTreeItemId NewItemId;

		// 得到要修改的节点.
		wxTreeItemId treeItemId = m_IdTreeItemMap[ulMonsterId];

		// 得到节点的个数.
		size_t iCount = m_pAddedGrowPointTreeCtrl->GetChildrenCount(treeItemId);
		iCount++;

		// 加入一个数据.
		//strAddNew = wxString::Format(wxT("%d"), ulInstanceId);
		strAddNew = wxString::Format(wxT("%d, x=%.2f,y=%.2f,z=%.2f"), ulInstanceId, pos.x, pos.y, pos.z);
		NewItemId = m_pAddedGrowPointTreeCtrl->AppendItem(treeItemId, strAddNew);
		
		// 记录新添加的树型的句柄.
		m_AddedTreeItemMap[ulInstanceId] = NewItemId;

		// 设置这个节点目前一共有多少个数据.
		strMonsterName = m_IdNameMap[ulMonsterId];
		strCount = wxString::Format(wxT("%s (%d)"), strMonsterName.c_str(), iCount);
		m_pAddedGrowPointTreeCtrl->SetItemText(treeItemId, strCount);

        // 确保显示
		m_pAddedGrowPointTreeCtrl->EnsureVisible(NewItemId);

		if(m_pNavigationDlg)
		{
			Ogre::Vector3 ScenePos(0, 0, 0);
			ScenePos = m_pGrowPointEditAction->GetGrowPos(ulInstanceId);
			m_pNavigationDlg->SetPointRGB(ScenePos.x, ScenePos.z, 2, 2, 0);
		}


	}

	// 记录一共添加的怪物的数据
	wxString strCount;

	int iGrowPointCount = 0;
	if(m_pGrowPointEditAction)
	{
		iGrowPointCount = m_pGrowPointEditAction->m_GrowPointMap.size();
	}
	strCount = wxString::Format(wxT("已经添加的矿物 (%d)"), iGrowPointCount);
	m_pAddedGrowPointTreeCtrl->SetItemText(m_AddedGrowPointRoot, strCount);

	
}


void CGrowPointEditDlg::onAddNewGrowPoint(unsigned long ulTypeId, unsigned long ulInstanceId)
{
	AddNewGrowPoint(ulTypeId, ulInstanceId);
}

// 删除一个生长点
void CGrowPointEditDlg::onDelGrowPoint( unsigned long ulTypeId, unsigned long ulInstanceId)
{
	DelGrowPoint(ulTypeId, ulInstanceId);
}
	
// 选择一个生长点
void CGrowPointEditDlg::onSelGrowPoint( unsigned long ulTypeId, unsigned long ulInstanceId)
{
	

}

// 删除一个生长点
void CGrowPointEditDlg::DelGrowPoint(unsigned long ulTypeId, unsigned long ulInstanceId)
{

	if(NULL == m_pAddedGrowPointTreeCtrl)
	{
		return;
	}

	if(0 == m_AddedTreeItemMap.count(ulInstanceId))
	{
	
		return;

	}
	else
	{

		
		if(m_pNavigationDlg)
		{
			Ogre::Vector3 ScenePos(0, 0, 0);
			ScenePos = m_pGrowPointEditAction->GetGrowPos(ulInstanceId);
			m_pNavigationDlg->SetPointRGB(ScenePos.x, ScenePos.z, 0, 0, 0);
		}


		wxString strAddNew = "";
		wxString strCount;
		wxString strOldName;
		std::string strGrowName = "";
		wxTreeItemId NewItemId;

		// 得到要修改的节点.
		wxTreeItemId treeItemId = m_IdTreeItemMap[ulTypeId];

		// 得到节点的个数.
		size_t iCount = m_pAddedGrowPointTreeCtrl->GetChildrenCount(treeItemId);
		iCount--;
	
		// 设置这个节点目前一共有多少个数据.
		strGrowName = m_IdNameMap[ulTypeId];
		strCount = wxString::Format(wxT("%s (%d)"), strGrowName.c_str(), iCount);
		m_pAddedGrowPointTreeCtrl->SetItemText(treeItemId, strCount);

		// 得到要删除的节点。并删除.
		NewItemId = m_AddedTreeItemMap[ulInstanceId];
		ID_TREE_ITEM_MAP::iterator it;
		it = m_AddedTreeItemMap.find(ulInstanceId);
		if(it != m_AddedTreeItemMap.end())
		{
			m_AddedTreeItemMap.erase(it);
		}
		m_pAddedGrowPointTreeCtrl->Delete(NewItemId);


	}

	
	// 记录一共添加的怪物的数据
	wxString strCount;

	int iGrowPointCount = 0;
	if(m_pGrowPointEditAction)
	{
		iGrowPointCount = m_pGrowPointEditAction->m_GrowPointMap.size();
	}
	strCount = wxString::Format(wxT("已经添加的怪物 (%d)"), iGrowPointCount);
	m_pAddedGrowPointTreeCtrl->SetItemText(m_AddedGrowPointRoot, strCount);


}

void CGrowPointEditDlg::ClearUI()
{
	m_AddedTreeItemMap.clear();
	m_IdTreeItemMap.clear();

	if(m_pAddedGrowPointTreeCtrl)
	{
		m_pAddedGrowPointTreeCtrl->DeleteAllItems();
	}//

	InitAddedGrowPointTree();
}


void CGrowPointEditDlg::SetActionMeshType()
{
	if(	NULL == m_pGrowPointEditAction)
	{
		return;
	}
	std::string strMeshName;
	m_pGrowPointEditAction->m_typeMeshMap.clear();

	unsigned long ulId;
	ID_NAME_DLG_MAP::iterator it;
	ID_NAME_DLG_MAP::iterator itEnd;

	itEnd = m_IdMeshMap.end();
	for(it = m_IdMeshMap.begin(); it != itEnd; it++)
	{

		if(	m_pGrowPointEditAction)
		{
			ulId = it->first;
			strMeshName = it->second;
			m_pGrowPointEditAction->AddGrowPointMeshType(ulId, strMeshName);
		}
		
	}
}


// 设置父窗口
void CGrowPointEditDlg::SetParent(FairyEditorFrame* pParent)
{
	m_pParent = pParent;
}

