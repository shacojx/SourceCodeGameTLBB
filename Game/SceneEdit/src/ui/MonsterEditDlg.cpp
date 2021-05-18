/////////////////////////////////////////////////////////////////////////////
// Name:        MonsterEditDlg.cpp
// Author:      XX
// Created:     XX/XX/XX
// Copyright:   XX
/////////////////////////////////////////////////////////////////////////////

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
    #pragma implementation "MonsterEditDlg.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "MonsterEditDlg.h"


#include <wx/splitter.h>
#include <wx/treectrl.h>
#include <wx/listctrl.h>

#include "MonsterEditAction.h"	// 怪物编辑action
#include "SceneManipulator.h"
#include "CDlgChangeMonsterInfo.h"
#include "TLBB_DBC.h"				// 读取excel
#include "RegionEditAction.h"

#include "winsock.h"				// 用来得到本地的ip地址
#pragma comment( lib, "Ws2_32.lib" )

#define  DIR_STEP		0.3925f		
#define _2PI			6.28
#define DIR_STEP_TRANS	36
// WDR: class implementations
using namespace WX;

BEGIN_EVENT_TABLE(CMonsterEditDlg, wxPanel)

 	EVT_LIST_ITEM_SELECTED(ID_LISTCTRL_MOSTER_EDIT, CMonsterEditDlg::OnSelected)
	
	EVT_BUTTON(ID_BUTTON_MODIFY_MONSTER_INFO, CMonsterEditDlg::OnModifyMonsterInfo)
	EVT_BUTTON(ID_BUTTON_DIR_DEC, CMonsterEditDlg::OnDecMonsterDir)
	EVT_BUTTON(ID_BUTTON_DIR_INC, CMonsterEditDlg::OnIncMonsterDir)
	EVT_CHOICE(ID_CHOICE_TYPE, CMonsterEditDlg::OnSelectedType)
	EVT_CHOICE(ID_CHOICE_AI_FILE, CMonsterEditDlg::OnSelectedAIFile)
	EVT_TEXT_ENTER(ID_TEXTCTRL_MONSTER_REPUTATION_ID, CMonsterEditDlg::OnEditReputationID)

END_EVENT_TABLE()


CMonsterEditDlg::CMonsterEditDlg(const wxString& resourceGroup)
: m_pSceneManipulator(NULL)
, m_pMonsterEditAction(NULL)
, m_dwAddedMonsterCount(0)
, m_bIsModify(false)
// 怪物列表控件。
, m_pMonsterListCtrl(NULL)
// 已经添加的怪物列表。
, m_pAddedMonsterTreeCtrl(NULL)
// 修改按钮.
, m_pModifyBn(NULL)
// 修改多个怪物按钮.
, m_pMultiModifyBn(NULL)
//怪物活动范围按钮.
, pArea(NULL)
, pLine(NULL)
// 活动半径文本。
, m_pAreaText(NULL)
// 活动半径文本。
, m_pRefreshText(NULL)
//路线编辑开关。
, m_pLineCheck(NULL)
, m_pModifyMonsterBn(NULL)
, m_pDirDecBn(NULL)
, m_pDirIncBn(NULL)
// 活动半径文本。
, m_pMonsterNameText(NULL)		// 怪物名字
, m_pMonsterLevelText(NULL)		// 怪物等级
, m_pMonsterExpText(NULL)		// 怪物经验
, m_pMonsterMoveSpeedText(NULL)	// 怪物移动速度
, m_pMonsterfreshTimeText(NULL)	// 怪物刷新时间
, m_pMonsterRadiusText(NULL)	// 怪物活动半径
, m_pMonsterCampIdText(NULL)	// 怪物阵营id
, m_pMonsterBaseAIText(NULL)	// 怪物基础AI ID
, m_pMonsterAdvanceAIText(NULL)	// 怪物高级AI文件。
, m_pMonsterEventText(NULL)		// 怪物事件文件
, m_pMonsterGroupIdText(NULL)	// 群id， 服务器优化使用
, m_pMonsterTeamIdText(NULL)	// 怪物队伍id
, m_pMonsterDirText(NULL)
, m_pChoiceBaseAiId(NULL)
, m_pChoiceAIFile(NULL)
, m_pChoiceEvenFile(NULL)
, m_pMonsterGUIDText(NULL)
, m_pMonsterInstanceName(NULL)
, m_pMonsterTitleName(NULL)
, m_pMonsterPatrol(NULL)
, m_pMonsterType(NULL)
//, m_pShop1(NULL)					// 商店1
//, m_pShop2(NULL)					// 商店2
//, m_pShop3(NULL)					// 商店3
//, m_pShop4(NULL)					// 商店4//

//:m_pListboxRegion(NULL)
//,m_pListboxPointInRegion(NULL)//
{
    //Init(resourceGroup);
	//InitData();
	memset(m_pShop, 0, sizeof(m_pShop));
}

CMonsterEditDlg::CMonsterEditDlg(
							wxWindow *parent, 
							const wxString& resourceGroup,
							wxWindowID winid,
							const wxPoint& pos,
							const wxSize& size,
							long style,
							const wxString& name
							)
							: m_pSceneManipulator(NULL)
, m_pMonsterEditAction(NULL)
, m_dwAddedMonsterCount(0)
, m_bIsModify(false)
// 怪物列表控件。
, m_pMonsterListCtrl(NULL)
// 已经添加的怪物列表。
, m_pAddedMonsterTreeCtrl(NULL)
// 修改按钮.
, m_pModifyBn(NULL)
// 修改多个怪物按钮.
, m_pMultiModifyBn(NULL)
//怪物活动范围按钮.
, pArea(NULL)
, pLine(NULL)
// 活动半径文本。
, m_pAreaText(NULL)
// 活动半径文本。
, m_pRefreshText(NULL)
//路线编辑开关。
, m_pLineCheck(NULL)
, m_pModifyMonsterBn(NULL)
, m_pDirDecBn(NULL)
, m_pDirIncBn(NULL)
// 活动半径文本。
, m_pMonsterNameText(NULL)		// 怪物名字
, m_pMonsterLevelText(NULL)		// 怪物等级
, m_pMonsterExpText(NULL)		// 怪物经验
, m_pMonsterMoveSpeedText(NULL)	// 怪物移动速度
, m_pMonsterfreshTimeText(NULL)	// 怪物刷新时间
, m_pMonsterRadiusText(NULL)	// 怪物活动半径
, m_pMonsterCampIdText(NULL)	// 怪物阵营id
, m_pMonsterBaseAIText(NULL)	// 怪物基础AI ID
, m_pMonsterAdvanceAIText(NULL)	// 怪物高级AI文件。
, m_pMonsterEventText(NULL)		// 怪物事件文件
, m_pMonsterGroupIdText(NULL)	// 群id， 服务器优化使用
, m_pMonsterTeamIdText(NULL)	// 怪物队伍id
, m_pMonsterDirText(NULL)
, m_pChoiceBaseAiId(NULL)
, m_pChoiceAIFile(NULL)
, m_pChoiceEvenFile(NULL)
, m_pMonsterGUIDText(NULL)
, m_pMonsterInstanceName(NULL)
, m_pMonsterTitleName(NULL)
, m_pMonsterPatrol(NULL)
, m_pMonsterType(NULL)
//, m_pShop1(NULL)					// 商店1
//, m_pShop2(NULL)					// 商店2
//, m_pShop3(NULL)					// 商店3
//, m_pShop4(NULL)					// 商店4//

										
{
    
	//InitData();
	memset(m_pShop, 0, sizeof(m_pShop));
	Create(parent, winid, pos, size, style, name);

}


bool CMonsterEditDlg::Create(
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
	MonsterEditDlg(this, true, true);
	
	// 获取各控件的引用
	GetAllControl();

	// 初始化怪物列表的表头.
	InitMonsterListCtrlTable();

	// 读取怪物id和外形映射文件数据
	//LoadMonsterIdNameTable();

	// 初始化tree结构
	InitAddedMonsterTree();

	// 得到怪物基本ai
	GetBaseAITable();

	// 得到高级ai
	GetAIFile();

	// 得到事件脚本
	GetEventFile();

	// 得到商店信息
	GetShopInfo();

	InitMonsterType();

	return true;
}


// 显示窗口
bool CMonsterEditDlg::Show(bool show)
{
	SetActive(show);
	return wxWindow::Show(show);
}

void CMonsterEditDlg::GetAction()
{

	// 得到区域编辑的场景控制
	if(m_pSceneManipulator)
	{
		WX::Action* action = m_pSceneManipulator->_getAction("MonsterEdit");
		m_pMonsterEditAction = (CMonsterEditaction *)action;
	}

}


void CMonsterEditDlg::SetSceneManipulator(WX::SceneManipulator* sceneManipulator)
{
	m_pSceneManipulator = sceneManipulator;
	
	if(m_pSceneManipulator)
	{
		m_pSceneManipulator->addSceneListener(this);
		
		// 得到底层的action
		GetAction();
	}
}




void CMonsterEditDlg::SetActive(bool bShow)
{


	if(m_pSceneManipulator)
	{
		WX::Action* action = m_pSceneManipulator->_getAction("MonsterEdit");
		m_pMonsterEditAction = (CMonsterEditaction *)action;
	}

	if (bShow)
    {
		if(m_pSceneManipulator)
		{
			WX::Action* action = m_pSceneManipulator->_getAction("MonsterEdit");
			m_pSceneManipulator->setActiveAction(action);//
			m_pMonsterEditAction = (CMonsterEditaction *)action;
			
		}
       
    }
	else
	{
		if(m_pMonsterEditAction)
		{
			m_pSceneManipulator->setActiveAction(NULL);
		}
	}

	// 显示隐藏场景
	if(m_pMonsterEditAction)
	{
		m_pMonsterEditAction->HideScene(bShow);
		
		WX::Action* action = m_pSceneManipulator->_getAction("RegionEdit");
		WX::CRegionEditAction* pRegionEditAction = ( WX::CRegionEditAction*)action;
		pRegionEditAction->HideScene(bShow);
	
		if(bShow)
		{
			
		}
	}

}


// 得到所有的控件.
void CMonsterEditDlg::GetAllControl()
{
	
	
	// 得到怪物列表控件.
	m_pMonsterListCtrl = wxDynamicCast(FindWindow(ID_LISTCTRL_MOSTER_EDIT), wxListCtrl);

	// 获得已经添加的怪物列表的控件.
	m_pAddedMonsterTreeCtrl =  wxDynamicCast(FindWindow(ID_TREECTRL_ADDED_MONSTER), wxTreeCtrl);

	if(m_pAddedMonsterTreeCtrl)
	{
		
		m_pAddedMonsterTreeCtrl->Show(false);

	}

	
	// 修改怪物信息按钮.
	m_pModifyMonsterBn = wxDynamicCast(FindWindow(ID_BUTTON_MODIFY_MONSTER_INFO), wxButton);


	// 设置怪物的方向。
	//m_pDirLTBn = wxDynamicCast(FindWindow(ID_BUTTON_LT), wxButton);
	//m_pDirTBn  = wxDynamicCast(FindWindow(ID_BUTTON_T), wxButton);
	//m_pDirRTBn = wxDynamicCast(FindWindow(ID_BUTTON_RT), wxButton);
	//m_pDirLBn  = wxDynamicCast(FindWindow(ID_BUTTON_L), wxButton);
	//m_pDirRBn  = wxDynamicCast(FindWindow(ID_BUTTON_R), wxButton);
	//m_pDirLBBn = wxDynamicCast(FindWindow(ID_BUTTON_LB), wxButton);
	//m_pDirBBn  = wxDynamicCast(FindWindow(ID_BUTTON_B), wxButton);
	//m_pDirRBBn = wxDynamicCast(FindWindow(ID_BUTTON_RB), wxButton);//

	// 方向增加按钮.
	m_pDirDecBn = wxDynamicCast(FindWindow(ID_BUTTON_DIR_DEC), wxButton);
	// 方向减少按钮.
	m_pDirIncBn = wxDynamicCast(FindWindow(ID_BUTTON_DIR_INC), wxButton);//

	// 怪物名字。
	m_pMonsterNameText		= wxDynamicCast(FindWindow(ID_TEXTCTRL_MONSTER_NAME), wxTextCtrl);				// 怪物名字
	if(m_pMonsterNameText)
	{
		m_pMonsterNameText->Enable(false);
		//m_pMonsterNameText->Show(false);
	}

	m_pMonsterLevelText		= wxDynamicCast(FindWindow(ID_TEXTCTRL_MONSTER_LEVEL), wxTextCtrl);				// 怪物等级
	if(m_pMonsterLevelText)
	{
		//m_pMonsterLevelText->Enable(true);
	}

    m_pMonsterExpText		= wxDynamicCast(FindWindow(ID_TEXTCTRL_MONSTER_EXP), wxTextCtrl);				// 怪物经验
	if(m_pMonsterExpText)
	{
		m_pMonsterExpText->Enable(false);
	}

	m_pMonsterMoveSpeedText = wxDynamicCast(FindWindow(ID_TEXTCTRL_MONSTER_MOVE_SPEED), wxTextCtrl);		// 怪物移动速度
	if(m_pMonsterMoveSpeedText)
	{
		m_pMonsterMoveSpeedText->Enable(false);
	}

	m_pMonsterfreshTimeText = wxDynamicCast(FindWindow(ID_TEXTCTRL_MONSTER_REFRESH_TIME), wxTextCtrl);		// 怪物刷新时间
	if(m_pMonsterfreshTimeText)
	{
		//m_pMonsterfreshTimeText->Enable(false);
	}

	m_pMonsterRadiusText	= wxDynamicCast(FindWindow(ID_TEXTCTRL_MONSTER_MVOE_RADIUS), wxTextCtrl);		// 怪物活动半径
	if(m_pMonsterRadiusText)
	{
		m_pMonsterRadiusText->Enable(false);
		//m_pMonsterRadiusText->Show(false);
	}

	m_pMonsterCampIdText	= wxDynamicCast(FindWindow(ID_TEXTCTRL_MONSTER_CAMP_ID), wxTextCtrl);			// 怪物阵营id
	//if(m_pMonsterCampIdText)
	//{
	//	m_pMonsterCampIdText->Enable(false);
	//	//m_pMonsterCampIdText->Show(false);
	//}
	// 以上是用户只读的选项

	// 基本ai
	m_pChoiceBaseAiId       = wxDynamicCast(FindWindow(ID_CHOICE_BASE_AI), wxChoice);						// 怪物基础AI ID
	m_pMonsterBaseAIText	= wxDynamicCast(FindWindow(ID_TEXTCTRL_MONSTER_BASE_AI_ID), wxTextCtrl);		// 怪物基础AI ID

	// 高级ai
	m_pChoiceAIFile			= wxDynamicCast(FindWindow(ID_CHOICE_AI_FILE), wxChoice);						// 怪物高级AI文件。
	m_pMonsterAdvanceAIText = wxDynamicCast(FindWindow(ID_TEXTCTRL_AI_FILE), wxTextCtrl);

	m_pChoiceEvenFile       = wxDynamicCast(FindWindow(ID_COMBO_EVEN_FILE), wxComboBox);	
	m_pMonsterEventText		= wxDynamicCast(FindWindow(ID_TEXTCTRL_EVEN_FILE), wxTextCtrl);					// 怪物事件文件

	m_pMonsterGroupIdText	= wxDynamicCast(FindWindow(ID_TEXTCTRL_GROUP_ID), wxTextCtrl);					// 群id， 服务器优化使用
	m_pMonsterTeamIdText	= wxDynamicCast(FindWindow(ID_TEXTCTRL_TEAM_ID), wxTextCtrl);					// 怪物队伍id
	m_pMonsterDirText       = wxDynamicCast(FindWindow(ID_TEXTCTRL_DIRECT), wxTextCtrl);					// 方向
	m_pMonsterType			= wxDynamicCast(FindWindow(ID_CHOICE_TYPE), wxChoice);							// 怪物类型

	m_pShop[0] = wxDynamicCast(FindWindow(ID_CHOICE_SHOP1), wxChoice);										// 商店1
	m_pShop[1] = wxDynamicCast(FindWindow(ID_CHOICE_SHOP2), wxChoice);										// 商店2
	m_pShop[2] = wxDynamicCast(FindWindow(ID_CHOICE_SHOP3), wxChoice);										// 商店3
	m_pShop[3] = wxDynamicCast(FindWindow(ID_CHOICE_SHOP4), wxChoice);										// 商店4

	// 怪物实例名字
	m_pMonsterInstanceName  = wxDynamicCast(FindWindow(ID_TEXTCTRL_INSTANCE_NAME), wxTextCtrl);	
	
	// 怪物称号
	m_pMonsterTitleName		= wxDynamicCast(FindWindow(ID_TEXTCTRL_TITLE), wxTextCtrl);	

	// 怪物guid
	m_pMonsterGUIDText		= wxDynamicCast(FindWindow(ID_TEXTCTRL_GUID), wxTextCtrl);	

	// 巡逻路线
	m_pMonsterPatrol        = wxDynamicCast(FindWindow(ID_TEXTCTRL_PATROL), wxTextCtrl);	
	if(m_pMonsterGUIDText)
	{
		//m_pMonsterGUIDText->Enable(false);
	}

	//
	m_pMonsterReputationID	= wxDynamicCast(FindWindow(ID_TEXTCTRL_MONSTER_REPUTATION_ID), wxTextCtrl);			// 怪物势力id


	// 隐藏前一个版本的控件供以后使用.
	m_pMonsterBaseAIText->Show(false);
	m_pMonsterAdvanceAIText->Show(false);
	m_pMonsterEventText->Show(false);
						
}

// 初始化怪物列表控件表头.
void CMonsterEditDlg::InitMonsterListCtrlTable()
{
	if(NULL == m_pMonsterListCtrl)
	{
		return;
	}
	m_pMonsterListCtrl->ClearAll();

	DBC::DBCFile excelDBFile(1);
	DBC::DBCFile excelDBFileModelName(1);
		                         
	if(!excelDBFile.OpenFromTXT("..\\..\\public\\config\\MonsterAttrExTable.txt"))
	{
		wxLogMessage(_T("不能打开服务器怪物信息表， 不能编辑怪物信息\n"));
        Enable(false);
        Hide();
		return;
	}

	if(!excelDBFileModelName.OpenFromTXT("..\\config\\CharModel.txt"))
	{
		wxLogMessage(_T("不能打开服务器怪物信息表， 不能编辑怪物信息\n"));
		return;
	}

	std::map<int, std::string> Id_Model_Map;
	int iModelCount = excelDBFileModelName.GetRecordsNum();

	const DBC::DBCFile::FIELD* pInfo = NULL;	
	char bufInfo[512];
	int iId     = -1;
	for(int iIndex = 0; iIndex < iModelCount; iIndex++)
	{
		pInfo = excelDBFileModelName.Search_Posistion(iIndex, 0);
		iId = pInfo->iValue;
		
		// 外形名字
		pInfo = excelDBFileModelName.Search_Posistion(iIndex, 1);
		Id_Model_Map[iId] = pInfo->pString;
	}//


    m_pMonsterListCtrl->InsertColumn(0, "怪物编号");	
    m_pMonsterListCtrl->InsertColumn(1, "怪物名称");	
	m_pMonsterListCtrl->InsertColumn(2, "等级");		
    m_pMonsterListCtrl->InsertColumn(3, "所得经验");	
	m_pMonsterListCtrl->InsertColumn(4, "移动速度");	
	m_pMonsterListCtrl->InsertColumn(5, "重生时间");		
    m_pMonsterListCtrl->InsertColumn(6, "活动范围");	
	m_pMonsterListCtrl->InsertColumn(7, "阵营ID");	

	int iRecordCount = excelDBFile.GetRecordsNum(); 
	int iLastColum   = excelDBFile.GetFieldsNum();
	iId     = -1;

	for(int i = 0; i < iRecordCount; i++)
	{
		// 读取服务器端
		pInfo = excelDBFile.Search_Posistion(i, 0);
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "%d", pInfo->iValue);
		m_pMonsterListCtrl->InsertItem(i, bufInfo);
		//绑定怪物id
		m_pMonsterListCtrl->SetItemData(i, pInfo->iValue);
		iId = pInfo->iValue;
		
		// 怪物名字.
		pInfo = excelDBFile.Search_Posistion(i, 1);
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "%s", pInfo->pString);
		m_pMonsterListCtrl->SetItem(i, 1, bufInfo);//

		// 等级
		pInfo = excelDBFile.Search_Posistion(i, 3);
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "%d", pInfo->iValue);
		m_pMonsterListCtrl->SetItem(i, 2, bufInfo);

		// 所得经验
		pInfo = excelDBFile.Search_Posistion(i, 4);
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "%d", pInfo->iValue);
		m_pMonsterListCtrl->SetItem(i, 3, bufInfo);

		// 移动速度
		pInfo = excelDBFile.Search_Posistion(i, 26);
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "%d", pInfo->iValue);
		m_pMonsterListCtrl->SetItem(i, 4, bufInfo);

		// 阵营
		pInfo = excelDBFile.Search_Posistion(i, 9);
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "%d", pInfo->iValue);
		m_pMonsterListCtrl->SetItem(i, 7, bufInfo);

		// 外形id
		pInfo = excelDBFile.Search_Posistion(i, 39);
		m_IdNameMap[iId] = Id_Model_Map[pInfo->iValue];//


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		////
		//// 读取客户端

		//// ID
        //pInfoClient = excelDBClient.Search_Posistion(i, 0);
		//iId = pInfoClient->iValue;
	
		//// 外形id
		//pInfoClient = excelDBClient.Search_Posistion(i, 1);
		//iLookId     = pInfoClient->iValue;


		//// 记录id与外形映射表.
		//m_Id_Look_IdMap[iId] = iLookId;
		//

		//// 怪物名字.
		//pInfoClient = excelDBClient.Search_Posistion(i, 2);
		//memset(bufInfo, 0, sizeof(bufInfo));
		//sprintf(bufInfo, "%s", pInfoClient->pString);
		//m_pMonsterListCtrl->SetItem(i, 1, bufInfo);
			
	}
}


// 读取基本ai表
void CMonsterEditDlg::GetBaseAITable()
{
	if(NULL == m_pChoiceBaseAiId)
	{
		return;
	}
	m_pChoiceBaseAiId->Clear();

	char bufInfo[512];
	char bufKey[64];
	
	int iAICount = -1;
	iAICount = ::GetPrivateProfileInt("INFO", "AINUMBER", -1, "..\\..\\public\\config\\MonsterAITable.ini");
	for(int i = 0; i < iAICount; i++)
	{
		memset(bufKey,  0, sizeof(bufKey));
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufKey, "AI%d", i);
		::GetPrivateProfileString(bufKey, "DESC", "none", bufInfo, sizeof(bufInfo), "..\\..\\public\\config\\MonsterAITable.ini");
		m_pChoiceBaseAiId->Insert(bufInfo, i);
	}

	if(m_pChoiceBaseAiId->GetCount())
	{
		m_pChoiceBaseAiId->Select(0);
	}

}

// 读取高级ai
void CMonsterEditDlg::GetAIFile()
{
	if(NULL == m_pChoiceAIFile)
	{
		return;
	}
	m_pChoiceAIFile->Clear();


	FILE* pFile = NULL;
	pFile = ::fopen("..\\..\\public\\data\\AIScript.dat", "r");
	if(NULL == pFile)
	{
		return;
	}

	char bufInfo[512];
	int iCount = -1;
	int iId    = -1;
	int iPos   = -1;
	std::string strWork = "";
	std::string strNum  = "";

	while(!feof(pFile))
	{
		memset(bufInfo, 0, sizeof(bufInfo));
		::fgets(bufInfo, sizeof(bufInfo), pFile);
		strWork = bufInfo;
		iPos = strWork.find("=");

		if(iPos <= 0)
		{
			continue;
		}

		strNum = strWork.substr(0, iPos);
		iId    = atoi(strNum.c_str());
		iCount++;
		m_pChoiceAIFile->Insert(bufInfo, iCount, (void*)iId);

	}

	if(m_pChoiceAIFile->GetCount())
	{
		m_pChoiceAIFile->Select(0);
	}

	::fclose(pFile);//
}

void CMonsterEditDlg::InitMonsterType()
{
	if(NULL == m_pMonsterType)
	{
		return;
	}
	m_pMonsterType->Clear();

	INT iId = 0;
	m_pMonsterType->Insert("0 =MONSTER", 0, (void*)iId);

	iId = 1;
	m_pMonsterType->Insert("1 =NPC", 1, (void*)iId);

	m_pMonsterType->Select(1);
}

// 读取事件脚本
void CMonsterEditDlg::GetEventFile()
{
	if(NULL == m_pChoiceEvenFile)
	{
		return;
	}

	m_pChoiceEvenFile->Clear();

	FILE* pFile = NULL;
	pFile = ::fopen("..\\..\\public\\data\\Script.dat", "r");
	if(NULL == pFile)
	{
		return;
	}

	char bufInfo[512];
	int iCount = -1;
	int iId    = -1;
	int iPos   = -1;
	std::string strWork = "";
	std::string strNum  = "";

	strNum = "-1=无脚本";
	iId    = -1;
	iCount++;
	m_pChoiceEvenFile->Insert(strNum.c_str(), iCount, (void*)iId);

	while(!feof(pFile))
	{
		memset(bufInfo, 0, sizeof(bufInfo));
		::fgets(bufInfo, sizeof(bufInfo), pFile);
		strWork = bufInfo;
		iPos = strWork.find("=");

		if(iPos <= 0)
		{
			continue;
		}

		strNum = strWork.substr(0, iPos);
		iId    = atoi(strNum.c_str());
		iCount++;
		m_pChoiceEvenFile->Insert(bufInfo, iCount, (void*)iId);

	}

	if(m_pChoiceEvenFile->GetCount())
	{
		m_pChoiceEvenFile->Select(0);
	}

	::fclose(pFile);//

}//



// 初始化怪物列表控件表头.
void CMonsterEditDlg::GetShopInfo()
{
	if((NULL == m_pShop[0])
		||(NULL == m_pShop[1])
		||(NULL == m_pShop[2])
		||(NULL == m_pShop[3]))
	{
		wxLogMessage(_T("商店控件没有初始化成功.\n"));
        Enable(false);
        Hide();
		return;
	}

	DBC::DBCFile excelDBFile(1);
	if(!excelDBFile.OpenFromTXT("..\\..\\public\\config\\ShopTable.txt"))
	{
		wxLogMessage(_T("不能打开商店信息表.\n"));
        Enable(false);
        Hide();
		return;
	}

	int iRecordCount = excelDBFile.GetRecordsNum(); 
	int iLastColum   = excelDBFile.GetFieldsNum();
	const DBC::DBCFile::FIELD* pInfo = NULL;	
	char bufInfo[512];
    int i;
	for(i = 0; i < iRecordCount; i++)
	{
		// 读取商店索引,绑定id
		pInfo = excelDBFile.Search_Posistion(i, 0);
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "%d", pInfo->iValue);
		m_pShop[0]->Insert(bufInfo, i, (void*)pInfo->iValue);
		m_pShop[1]->Insert(bufInfo, i, (void*)pInfo->iValue);
		m_pShop[2]->Insert(bufInfo, i, (void*)pInfo->iValue);
		m_pShop[3]->Insert(bufInfo, i, (void*)pInfo->iValue);
	
	}

	m_pShop[0]->Insert("无", i, (void*)-1);
	m_pShop[1]->Insert("无", i, (void*)-1);
	m_pShop[2]->Insert("无", i, (void*)-1);
	m_pShop[3]->Insert("无", i, (void*)-1);

	
	m_pShop[0]->Select(i);
	m_pShop[1]->Select(i);
	m_pShop[2]->Select(i);
	m_pShop[3]->Select(i);

  

	
}




// 得到怪物id， 与名字的映射表.
void CMonsterEditDlg::LoadMonsterIdNameTable()
{


	//DBC::DBCFile excelDBFile(1);
	//if(!excelDBFile.OpenFromTXT("..\\LogicEditData\\monster\\MonsterAttrExTable.txt"))
	//{
	//	wxLogMessage(_T("不能打开服务器怪物信息表， 不能编辑怪物信息\n"));
	//	return;
	//}
	FILE* pFile = NULL;
	pFile = ::fopen("..\\..\\Public\\Config\\MonsterAttrExTable.txt", "r");

	char buf[2048];
	::fgets(buf, sizeof(buf), pFile);
	::fgets(buf, sizeof(buf), pFile);

	// 怪物id
	char bufId[64];

	// 怪物外形文件名字
	char bufName[128];

	m_IdNameMap.clear();

	int iId = -1;
	while(!feof(pFile))
	{
		memset(buf, 0, sizeof(buf));
		memset(bufId, 0, sizeof(bufId));
		memset(bufName, 0, sizeof(bufName));

		// 得到一个怪物
		::fgets(buf, sizeof(buf), pFile);
		sscanf(buf, "%s %s ", bufId, bufName);

		if(0 == strcmp("", bufId))
		{
			continue;
		}
		iId = atoi(bufId);
		m_IdNameMap[iId] = bufName;

	}

	std::string strstr = m_IdNameMap[918];

	::fclose(pFile);//

}

void CMonsterEditDlg::OnSelected(wxListEvent& event)
{
	
	if(m_pMonsterListCtrl)
	{
		//int iIndex = m_pMonsterListCtrl->getitems();
		int iSelIndex = event.m_itemIndex;
		int iId     = -1;
		if(iSelIndex >= 0)
		{

			m_iCurSelListCtrlIndex = iSelIndex;
			//ShowMonsterTypeInfo(iSelIndex);
			ShowMonsterTypeInfoByIndex(iSelIndex);
			iId = m_pMonsterListCtrl->GetItemData(iSelIndex);
			if(m_IdNameMap.count(iId))
			{
				if(m_pMonsterEditAction)
				{
					std::string strObjName = m_IdNameMap[iId];
					m_pMonsterEditAction->SetNewEditObject(strObjName.c_str(), iId);
					
				}
			}

			ReSetDataInfo();

			////
			//// 初试化数据
			////
			//if(m_pChoiceEvenFile->GetCount())
			//{
			//	m_pChoiceEvenFile->Select(0);
			//}

			//// 基本ai
			//if(m_pChoiceBaseAiId)
			//{
			//	m_pChoiceBaseAiId->Select(0);
			//}

			//// 高级ai
			//if(m_pChoiceAIFile)
			//{
			//	m_pChoiceAIFile->Select(0);
			//}


			//m_pMonsterTeamIdText->SetLabel("-1");		// 怪物队伍id
			//m_pMonsterDirText->SetLabel("0.00");		// 怪物方向
			//m_pMonsterInstanceName->SetLabel("");		// 怪物实例名字
			//m_pMonsterTitleName->SetLabel("");			// 怪物称号名字
			//m_pMonsterPatrol->SetLabel("-1");			// 怪物巡逻路线//

		}
		
	}//
}


// 初始化怪物列表树.
void CMonsterEditDlg::InitAddedMonsterTree()
{
	if((m_pAddedMonsterTreeCtrl)&&(m_pMonsterListCtrl))
	{
		m_pAddedMonsterTreeCtrl->DeleteAllItems();
		m_AddedMonsterRoot = m_pAddedMonsterTreeCtrl->AddRoot("已经添加的怪物");

		wxTreeItemId newTreeItemId;
		std::string  strName = "";

		m_Id_TreeItem_Map.clear();

		int iCount = m_pMonsterListCtrl->GetItemCount();
		for(int i = 0; i < iCount; i++)
		{
			strName = m_pMonsterListCtrl->GetItemText(i);
			newTreeItemId = m_pAddedMonsterTreeCtrl->AppendItem(m_AddedMonsterRoot, strName.c_str());
			m_Id_TreeItem_Map[m_pMonsterListCtrl->GetItemData(i)] = newTreeItemId;
		}
	}//
}

// 通知界面的回掉函数 添加一个怪物
void CMonsterEditDlg::onAddNewMonster(unsigned long ulMonsterId, unsigned long ulInstanceId)
{
	// 已经做了修改。
	m_bIsModify = true;
	AddNewMonster(ulMonsterId, ulInstanceId);

}


// 通知界面的回掉函数 删除一个怪物
void CMonsterEditDlg::onDelMonster( unsigned long ulMonsterId, unsigned long ulInstanceId)
{

	// 已经做了修改。
	m_bIsModify = true;
	DelMonster(ulMonsterId, ulInstanceId);

}


	// 添加一个怪物
void CMonsterEditDlg::AddNewMonster(unsigned long ulMonsterId, unsigned long ulInstanceId)
{

	if(NULL == m_pAddedMonsterTreeCtrl)
	{
		return;
	}

	if(0 == m_Id_TreeItem_Map.count(ulMonsterId))
	{
		// 在列表中找不到对应的怪物名称。
		wxString strAddNew;
	
		// 加入一个数据.
		strAddNew.Format(wxT("%d"), ulInstanceId);
		//m_pAddedMonsterTreeCtrl->AppendItem(m_AddedMonsterRoot, strAddNew, -1, -1, (wxTreeItemData *)ulInstanceId);
		wxTreeItemId NewItemId = m_pAddedMonsterTreeCtrl->AppendItem(m_AddedMonsterRoot, strAddNew);
		m_Id_AddedMonsterMap[ulInstanceId] = NewItemId;

	}
	else
	{
		wxString strAddNew			= "";
		wxString strCount			= "";
		wxString strOldName			= "";
		std::string strMonsterName  = "";
		wxTreeItemId NewItemId;

		// 得到要修改的节点.
		wxTreeItemId treeItemId = m_Id_TreeItem_Map[ulMonsterId];

		// 得到节点的个数.
		size_t iCount = m_pAddedMonsterTreeCtrl->GetChildrenCount(treeItemId);
		iCount++;

		// 加入一个数据.
		strAddNew = wxString::Format(wxT("%d"), ulInstanceId);
		//m_pAddedMonsterTreeCtrl->AppendItem(treeItemId, strAddNew, -1, -1, (wxTreeItemData *)ulInstanceId);
		NewItemId = m_pAddedMonsterTreeCtrl->AppendItem(treeItemId, strAddNew);
		m_Id_AddedMonsterMap[ulInstanceId] = NewItemId;

		// 设置这个节点目前一共有多少个数据.
		strMonsterName = m_IdNameMap[ulMonsterId];
		strCount = wxString::Format(wxT("%s (%d)"), strMonsterName.c_str(), iCount);
		m_pAddedMonsterTreeCtrl->SetItemText(treeItemId, strCount);
	}

	// 记录一共添加的怪物的数据
	wxString strCount;

	m_dwAddedMonsterCount++;
	strCount = wxString::Format(wxT("已经添加的怪物 (%d)"), m_dwAddedMonsterCount);
	m_pAddedMonsterTreeCtrl->SetItemText(m_AddedMonsterRoot, strCount);

	// 当前编辑的实例
	m_ulCurEditMonsterInstance = ulInstanceId;

	INSTANCE_MONSTERINFO_MAP::iterator it;
	it = m_MonsterInfoTable.find(-1);

	if(it != m_MonsterInfoTable.end())
	{
		CMonsterInstanceInfo monsterInfo = it->second;
		monsterInfo.m_dwInstanceId		 = ulInstanceId;

		// 添加
		m_MonsterInfoTable[ulInstanceId] = monsterInfo;
		if(m_pMonsterEditAction)
		{
			m_pMonsterEditAction->ChangeMonsterDir(ulInstanceId, monsterInfo.m_fMonstDir);
		}

		// 删除临时数据
		m_MonsterInfoTable.erase(it);
	}
	else
	{
		CMonsterInstanceInfo monsterInfo;

		monsterInfo.m_dwInstanceId		= ulInstanceId;
		monsterInfo.m_dwMonsterGUID		= GetGuid();
		monsterInfo.m_dwObjectId		= ulMonsterId;
		monsterInfo.m_strInstanceName	= GetInstanceName();
		monsterInfo.m_strTitleName	    = GetTitleName();
		monsterInfo.m_position          = GetMonsterPos();
		monsterInfo.m_fMonstDir         = GetDir();
		monsterInfo.m_EvenId			= GetEvenId();
		monsterInfo.m_iRefreshTime      = GetRefreshTime();
		monsterInfo.m_iReputationID		= GetReputationID();
		if(monsterInfo.m_iRefreshTime < 0)
		{
			monsterInfo.m_iRefreshTime = 1000;
		}
		monsterInfo.m_iGroupId			= GetGroupId();
		monsterInfo.m_iTeamId			= GetTeamId();
		monsterInfo.m_iBaseAIId			= GetBaseAIId();
		monsterInfo.m_iAdvanceAIId		= GetAdvanceAIId();
		monsterInfo.m_iLineid           = GetPatrolId();
		monsterInfo.m_iLevel			= GetLevel();
		monsterInfo.m_iType				= GetMonsterType();
		monsterInfo.m_iCampId			= GetCamp();
		m_MonsterInfoTable[ulInstanceId] = monsterInfo;
	}

	// 显示怪物.
	ShowMonsterInfo(ulInstanceId);
}
		
// 删除一个怪物.
void CMonsterEditDlg::DelMonster(unsigned long ulMonsterId, unsigned long ulInstanceId)
{

	//if(NULL == m_pAddedMonsterTreeCtrl)
	//{
	//	return;
	//}

	//if(0 == m_Id_TreeItem_Map.count(ulMonsterId))
	//{
	//	
	//	//// 在列表中找不到对应的怪物名称。
	//	//wxString strAddNew;
	//
	//	//// 加入一个数据.
	//	//strAddNew.Format(wxT("%d"), ulInstanceId);
	//	////m_pAddedMonsterTreeCtrl->AppendItem(m_AddedMonsterRoot, strAddNew, -1, -1, (wxTreeItemData *)ulInstanceId);
	//	//m_pAddedMonsterTreeCtrl->AppendItem(m_AddedMonsterRoot, strAddNew);

	//	return;

	//}
	//else
	//{
	//	wxString strAddNew = "";
	//	wxString strCount;
	//	wxString strOldName;
	//	std::string strMonsterName = "";
	//	wxTreeItemId NewItemId;

	//	if(0 == m_Id_AddedMonsterMap.count(ulInstanceId))
	//	{
	//		return;
	//	}

	//	// 得到要修改的节点.
	//	wxTreeItemId treeItemId = m_Id_TreeItem_Map[ulMonsterId];

	//	// 得到节点的个数.
	//	size_t iCount = m_pAddedMonsterTreeCtrl->GetChildrenCount(treeItemId);
	//	iCount--;
	//
	//	// 设置这个节点目前一共有多少个数据.
	//	strMonsterName = m_IdNameMap[ulMonsterId];
	//	strCount = wxString::Format(wxT("%s (%d)"), strMonsterName.c_str(), iCount);
	//	m_pAddedMonsterTreeCtrl->SetItemText(treeItemId, strCount);

	//	// 得到要删除的节点。并删除.
	//	NewItemId = m_Id_AddedMonsterMap[ulInstanceId];
	//	ID_TREE_ITEM_MAP::iterator it;
	//	it = m_Id_AddedMonsterMap.find(ulInstanceId);
	//	if(it != m_Id_AddedMonsterMap.end())
	//	{
	//		m_Id_AddedMonsterMap.erase(it);
	//	}
	//	m_pAddedMonsterTreeCtrl->Delete(NewItemId);


	//}

	//// 记录一共添加的怪物的数据
	//wxString strCount;

	//m_dwAddedMonsterCount--;
	//strCount = wxString::Format(wxT("已经添加的怪物 (%d)"), m_dwAddedMonsterCount);
	//m_pAddedMonsterTreeCtrl->SetItemText(m_AddedMonsterRoot, strCount);//

	INSTANCE_MONSTERINFO_MAP::iterator it;

	it = m_MonsterInfoTable.find(ulInstanceId);
	if(it != m_MonsterInfoTable.end())
	{
		m_MonsterInfoTable.erase(it);
	}

	
}

// 删除怪物.
void CMonsterEditDlg::DelMonster()
{
	if(m_pMonsterEditAction)
	{
		m_pMonsterEditAction->DelSelectMonster();
	}

}


// 是否禁止区域编辑
void CMonsterEditDlg::EnableAreaEdit(bool bEnable)
{

	// 设置区域按钮选中
	if(pArea)
	{
		pArea->SetValue(bEnable);
	}

	// 设置路线按钮不选中
	if(pLine)
	{
		pLine->SetValue(!bEnable);
	}

	// 活动半径文本。
	if(m_pAreaText)
	{
		m_pAreaText->Enable(bEnable);
	}

	if(m_pLineCheck)
	{
		m_pLineCheck->Enable(!bEnable);
	}
	

}

 // 修改怪物参数
void CMonsterEditDlg::OnModifyData( wxCommandEvent &event )
{
	// 已经做了修改。
	m_bIsModify = true;
	ModifySelectData();
}

 // 修改多个怪物参数
void CMonsterEditDlg::OnModifyMultiData( wxCommandEvent &event )
{
	// 已经做了修改。
	m_bIsModify = true;
	ModifyMultiData();
}


// 修改怪物信息
void CMonsterEditDlg::OnModifyMonsterInfo( wxCommandEvent &event )
{
	// 已经做了修改。
	m_bIsModify = true;
	//ModifyMultiData();

	ModifyMonsterInfo();
}

// 减少怪物的方向
void CMonsterEditDlg::OnDecMonsterDir( wxCommandEvent &event )
{
	// 已经做了修改。
	m_bIsModify = true;

	// 减少怪物的方向
	DecMonsterDir();

		
	
}

// 增加怪物方向。
void CMonsterEditDlg::OnIncMonsterDir( wxCommandEvent &event )
{
	// 已经做了修改。
	m_bIsModify = true;
	
	// 增加怪物方向。
	IncMonsterDir();
	
}
void CMonsterEditDlg::OnSelectedType( wxCommandEvent &event )
{
	m_bIsModify = true;

}
void CMonsterEditDlg::OnSelectedAIFile( wxCommandEvent &event )
{
	m_bIsModify = true;

}

void CMonsterEditDlg::OnEditReputationID( wxCommandEvent &event )
{
	m_bIsModify = true;
}

// 修改选择的怪物的参数.
void CMonsterEditDlg::ModifyMultiData()
{

	if(wxMessageBox(_T("你确定改变多个怪的属性吗？这样做没有redo undo！\n"), _T("修改"), wxICON_QUESTION | wxYES_NO, this) == wxNO)
	{
		return;
	}

	wxTreeItemId Treeitem;
	if(NULL == m_pAddedMonsterTreeCtrl)
	{
		return;
	}

	if(NULL == m_pMonsterEditAction)
	{
		return;
	}

	Treeitem = m_pAddedMonsterTreeCtrl->GetSelection();

	if(!Treeitem.IsOk())
	{
		return;
	}
		
	CDlgChangeMosterInfo dlg(this, wxID_ANY, _("Change_Mosnter_Info"));
	dlg.ShowModal();

	switch(dlg.m_iChangeType)
	{
		case CDlgChangeMosterInfo::TIME:
			{
				if(Treeitem == m_AddedMonsterRoot)
				{
					m_pMonsterEditAction->ChangeAllMonsterRefreshTime(dlg.m_iTime);
					return;
				}

				unsigned long dwMonsterId = GetMonsterIdByTreeItem(Treeitem); 
				m_pMonsterEditAction->ChangeMonsterRefreshTime_Type(dwMonsterId, dlg.m_iTime);



			}
			break;
		case CDlgChangeMosterInfo::RADIUS:
			{
				if(Treeitem == m_AddedMonsterRoot)
				{
					m_pMonsterEditAction->ChangeAllMonsterRadius(dlg.m_fRadius);
					return;
				}

				unsigned long dwMonsterId = GetMonsterIdByTreeItem(Treeitem); 
				m_pMonsterEditAction->ChangeMonsterRefreshRadius_Type(dwMonsterId, dlg.m_fRadius);


			}
			break;
		case CDlgChangeMosterInfo::BOTH:
			{
				if(Treeitem == m_AddedMonsterRoot)
				{
					m_pMonsterEditAction->ChangeAllMonsterRefreshTimeRadius(dlg.m_iTime, dlg.m_fRadius);
					return;
				}

				unsigned long dwMonsterId = GetMonsterIdByTreeItem(Treeitem); 
				m_pMonsterEditAction->ChangeMonsterRefreshTimeRadius_Type(dwMonsterId, dlg.m_iTime, dlg.m_fRadius);

			}
			break;
	}

}

// 修改选择的怪物的参数
void CMonsterEditDlg::ModifySelectData()
{
	
	if(m_pMonsterEditAction)
	{
		CMonsterInfo* pMonsterInfo = m_pMonsterEditAction->GetMonster(m_pMonsterEditAction->m_dwCurSelMonsterInstanceId);
		
		if(NULL == pMonsterInfo)
		{
			return;
		}

		wxString strValue = "";
		char bufInfo[512];

		if(m_pMonsterLevelText)
		{
			memset(bufInfo, 0, sizeof(bufInfo));
			strValue  = m_pMonsterLevelText->GetValue();
			sprintf(bufInfo, "%s", strValue.c_str());
			pMonsterInfo->m_iLevel = atoi(bufInfo);
		}

		if(m_pMonsterBaseAIText)
		{
			memset(bufInfo, 0, sizeof(bufInfo));
			strValue  = m_pMonsterBaseAIText->GetValue();
			sprintf(bufInfo, "%s", strValue.c_str());
			pMonsterInfo->m_iBaseAIId = atoi(bufInfo);
		}

		if(m_pMonsterAdvanceAIText)
		{
			pMonsterInfo->m_strAdvanceAIFile = m_pMonsterAdvanceAIText->GetValue().c_str();
		}

		if(m_pMonsterEventText)
		{
			pMonsterInfo->m_strEvent = m_pMonsterEventText->GetValue().c_str();
		}

		if(m_pMonsterGroupIdText)
		{
			memset(bufInfo, 0, sizeof(bufInfo));
			strValue  = m_pMonsterGroupIdText->GetValue();
			sprintf(bufInfo, "%s", strValue.c_str());
			pMonsterInfo->m_iGroupId = atoi(bufInfo);
		}

		if(m_pMonsterTeamIdText)
		{
			memset(bufInfo, 0, sizeof(bufInfo));
			strValue  = m_pMonsterTeamIdText->GetValue();
			sprintf(bufInfo, "%s", strValue.c_str());
			pMonsterInfo->m_iTeamId = atoi(bufInfo);
		}

		if(m_pMonsterDirText)
		{
			memset(bufInfo, 0, sizeof(bufInfo));
			strValue  = m_pMonsterDirText->GetValue();
			sprintf(bufInfo, "%s", strValue.c_str());
			pMonsterInfo->m_fMonstDir = atof(bufInfo);
		}//

	}
			
		//wxString strValue = "";
		//strValue = wxString::Format(wxT("%.4f"), pMonsterInfo->m_fAreaRadius);
		//m_pAreaText->SetLabel(strValue);//

}


void CMonsterEditDlg::onSelMonster( unsigned long ulMonsterId, unsigned long ulInstanceId)
{
	SelMonster(ulMonsterId, ulInstanceId);
}

void CMonsterEditDlg::SelMonster( unsigned long ulMonsterId, unsigned long ulInstanceId)
{
	if(0 == m_Id_AddedMonsterMap.count(ulInstanceId))
	{
		return;
	}

	if(NULL == m_pAddedMonsterTreeCtrl)
	{
		return;
	}

	//// 处理树枝列表信息
	//wxTreeItemId TreeItem= m_Id_AddedMonsterMap[ulInstanceId];
	//m_pAddedMonsterTreeCtrl->SelectItem(TreeItem);
	//m_pAddedMonsterTreeCtrl->EnsureVisible(TreeItem);

	// 设置当前编辑的怪物实例id
	m_ulCurEditMonsterInstance = ulInstanceId;

	// 显示怪物的信息.
	ShowMonsterInfo(ulInstanceId);

	// 显示怪物类型信息。
	ShowMonsterTypeInfo(ulMonsterId);

}


// 显示选中的怪物的信息
void CMonsterEditDlg::ShowMonsterInfo( unsigned long ulInstanceId)
{
	if(m_MonsterInfoTable.count(m_ulCurEditMonsterInstance))
	{
		
		CMonsterInstanceInfo& MonsterInfo = m_MonsterInfoTable[m_ulCurEditMonsterInstance];
				
		char bufInfo[512];

		// 基础ai
		//if(m_pMonsterBaseAIText)
		//{
		//	memset(bufInfo, 0, sizeof(bufInfo));
		//	sprintf(bufInfo, "%d", MonsterInfo.m_iBaseAIId);
		//	m_pMonsterBaseAIText->SetLabel(bufInfo);// 怪物基础AI ID
		//}//

		if(m_pChoiceBaseAiId)
		{
			m_pChoiceBaseAiId->Select(MonsterInfo.m_iBaseAIId);
		}

		// 高级ai
		//if(m_pMonsterAdvanceAIText)
		//{
		//	memset(bufInfo, 0, sizeof(bufInfo));
		//	sprintf(bufInfo, "%d", MonsterInfo.m_iAdvanceAIId);
		//	m_pMonsterAdvanceAIText->SetLabel(bufInfo);
		//}//
		if(m_pChoiceAIFile)
		{
			m_pChoiceAIFile->Select(MonsterInfo.m_iAdvanceAIId);
		}

		if(m_pMonsterType)
		{
			m_pMonsterType->Select(MonsterInfo.m_iType);
		}

		// 事件id
		//if(m_pMonsterEventText)
		//{
		//	memset(bufInfo, 0, sizeof(bufInfo));
		//	sprintf(bufInfo, "%d", MonsterInfo.m_EvenId);
		//	m_pMonsterEventText->SetLabel(bufInfo);
		//}//

		if(m_pChoiceEvenFile)
		{
			int iCount = m_pChoiceEvenFile->GetCount();
			std::string strName = "";
			std::string strNum  = "";
			int iPos = -1;

			for(int i = 0; i < iCount; i++)
			{
				strName = m_pChoiceEvenFile->GetString(i);
				iPos = strName.find("=");

				if(iPos <= 0)
				{
					continue;
				}

				strNum = strName.substr(0, iPos);
				if(MonsterInfo.m_EvenId == atoi(strNum.c_str()))
				{
					m_pChoiceEvenFile->Select(i);
					break;
				}

			}
			
		}

		if(m_pMonsterLevelText)
		{
			memset(bufInfo, 0, sizeof(bufInfo));
			sprintf(bufInfo, "%d", MonsterInfo.m_iLevel);
			m_pMonsterLevelText->SetLabel(bufInfo);
		}

		// 组id
		if(m_pMonsterGroupIdText)
		{
			memset(bufInfo, 0, sizeof(bufInfo));
			sprintf(bufInfo, "%d", MonsterInfo.m_iGroupId);
			m_pMonsterGroupIdText->SetLabel(bufInfo);
		}

		// 队id
		if(m_pMonsterTeamIdText)
		{
			memset(bufInfo, 0, sizeof(bufInfo));
			sprintf(bufInfo, "%d", MonsterInfo.m_iTeamId);
			m_pMonsterTeamIdText->SetLabel(bufInfo);
		}

		// 方向
		if(m_pMonsterDirText)
		{
			memset(bufInfo, 0, sizeof(bufInfo));
			sprintf(bufInfo, "%.4f", MonsterInfo.m_fMonstDir);
			m_pMonsterDirText->SetLabel(bufInfo);
		}

		// guid
		if(m_pMonsterGUIDText)
		{
			memset(bufInfo, 0, sizeof(bufInfo));
			sprintf(bufInfo, "%d", MonsterInfo.m_dwMonsterGUID);
			m_pMonsterGUIDText->SetLabel(bufInfo);
		}


		// 实例名字
		if(m_pMonsterInstanceName)
		{
			m_pMonsterInstanceName->SetLabel(MonsterInfo.m_strInstanceName.c_str());
		}

		// 称号
		if(m_pMonsterTitleName)
		{
			m_pMonsterTitleName->SetLabel(MonsterInfo.m_strTitleName.c_str());
		}

		// 巡逻id
		if(m_pMonsterPatrol)
		{
			memset(bufInfo, 0, sizeof(bufInfo));
			sprintf(bufInfo, "%d", MonsterInfo.m_iLineid);
			m_pMonsterPatrol->SetLabel(bufInfo);
		}

		// 刷新时间
        if(m_pMonsterfreshTimeText)
		{
			memset(bufInfo, 0, sizeof(bufInfo));
			sprintf(bufInfo, "%d", MonsterInfo.m_iRefreshTime);
			m_pMonsterfreshTimeText->SetLabel(bufInfo);
		}

		// 势力
		if( m_pMonsterReputationID && MonsterInfo.m_iReputationID >= 0 )
		{
			memset( bufInfo, 0, sizeof(bufInfo) );
			sprintf( bufInfo, "%d", MonsterInfo.m_iReputationID );
			m_pMonsterReputationID->SetLabel(bufInfo);
		}
		else
			m_pMonsterReputationID->SetLabel("-1");

		// 显示商店
		int iShopSel = -1;
		int iValue   = -1;
		for(int i = 0; i < SHOP_COUNT; i++)
		{
			iShopSel = MonsterInfo.m_iShopArray[i];
			if(-1 == iShopSel)
			{
				m_pShop[i]->Select(m_pShop[i]->GetCount()-1);
			}
			else
			{
				for(int iItem = 0; iItem < m_pShop[i]->GetCount(); iItem++)
				{
					iValue = atoi(m_pShop[i]->GetString(iItem));
					if(iValue == iShopSel)
					{
						m_pShop[i]->Select(iItem);
						break;
					}
					
				}
			}
		}


		
	}

}

// 通过treeitem得到怪物外形id
unsigned long CMonsterEditDlg::GetMonsterIdByTreeItem(wxTreeItemId treeItem)
{
	
	ID_TREE_ITEM_MAP::iterator it;
	ID_TREE_ITEM_MAP::iterator itEnd;

	wxTreeItemId TreeItemFind;

	itEnd = m_Id_TreeItem_Map.end();
	for(it = m_Id_TreeItem_Map.begin(); it != itEnd; it++)
	{
		TreeItemFind = it->second;
		if(TreeItemFind == treeItem)
		{
			return it->first;
		}
	}

	return 0xffffffff;
}

void CMonsterEditDlg::ShowMonsterTypeInfo(int iSelIndex)
{
	if(NULL == m_pMonsterListCtrl)
	{
		return;
	}
	
	wxListItem itemInfo;
	int iSize = m_pMonsterListCtrl->GetItemCount();
    int i;
	for(i = 0; i < iSize; i++)
	{
		itemInfo.m_itemId = i;
		itemInfo.m_col = 0;
		itemInfo.m_mask = wxLIST_MASK_TEXT;
		
		m_pMonsterListCtrl->GetItem( itemInfo);
		if(atoi(itemInfo.GetText()) == iSelIndex)
		{
			break;
		}
	}

	
	if(i >= iSize)
	{
		return;
	}

	// 为什么不能加这句话， 这个界面开发工具包太恶了！
	//memset(&itemInfo, 0, sizeof(itemInfo));
	itemInfo.m_itemId = i;
    itemInfo.m_col = 1;
    itemInfo.m_mask = wxLIST_MASK_TEXT;

	

	// 得到名字
	m_pMonsterListCtrl->GetItem(itemInfo);
	if(m_pMonsterNameText)
	{
		char bufInfo[128];
		memset(bufInfo, 0, sizeof(bufInfo));
		sprintf(bufInfo, "%s--%d", itemInfo.GetText(), iSelIndex);
		
	
		m_pMonsterNameText->SetLabel(bufInfo);
	}

	// 得到等级
	itemInfo.SetColumn(2);
	m_pMonsterListCtrl->GetItem(itemInfo);
	if(m_pMonsterLevelText)
	{
		if( m_pMonsterLevelText->GetLabel() == "-1" || m_pMonsterLevelText->GetLabel() == "" )
			m_pMonsterLevelText->SetLabel(itemInfo.GetText());
	}

	// 得到经验
	itemInfo.SetColumn(3);
	m_pMonsterListCtrl->GetItem(itemInfo);
	if(m_pMonsterExpText)
	{
		m_pMonsterExpText->SetLabel(itemInfo.GetText());
	}

	// 得到移动速度
	itemInfo.SetColumn(4);
	m_pMonsterListCtrl->GetItem(itemInfo);
	if(m_pMonsterMoveSpeedText)
	{
		m_pMonsterMoveSpeedText->SetLabel(itemInfo.GetText());
	}//

	//// 得到重生时间
	//itemInfo.SetColumn(5);
	//m_pMonsterListCtrl->GetItem(itemInfo);
	//if(m_pMonsterfreshTimeText)
	//{
	//	m_pMonsterfreshTimeText->SetLabel(itemInfo.GetText());
	//}//

	// 得到活动范围
	itemInfo.SetColumn(6);
	m_pMonsterListCtrl->GetItem(itemInfo);
	if(m_pMonsterRadiusText)
	{
		m_pMonsterRadiusText->SetLabel(itemInfo.GetText());
	}//

	// 得到阵营ID
	itemInfo.SetColumn(7);
	m_pMonsterListCtrl->GetItem(itemInfo);
	if(m_pMonsterCampIdText)
	{
		m_pMonsterCampIdText->SetLabel(itemInfo.GetText());
	}//
}

	// 修改选择的怪物的参数.
void CMonsterEditDlg::ModifyMonsterInfo()
{
	if(m_MonsterInfoTable.count(m_ulCurEditMonsterInstance))
	{

		CMonsterInstanceInfo& MonsterInfo = m_MonsterInfoTable[m_ulCurEditMonsterInstance];
		
		MonsterInfo.m_strInstanceName	= GetInstanceName();
		MonsterInfo.m_strTitleName	    = GetTitleName();
		MonsterInfo.m_fMonstDir         = GetDir();
		MonsterInfo.m_EvenId			= GetEvenId();
		MonsterInfo.m_iRefreshTime      = GetRefreshTime();
		MonsterInfo.m_iGroupId			= GetGroupId();
		MonsterInfo.m_iTeamId			= GetTeamId();
		MonsterInfo.m_iBaseAIId			= GetBaseAIId();
		MonsterInfo.m_iAdvanceAIId		= GetAdvanceAIId();
		MonsterInfo.m_iLineid           = GetPatrolId();
		MonsterInfo.m_iReputationID		= GetReputationID();
		MonsterInfo.m_iLevel			= GetLevel();
		MonsterInfo.m_iType				= GetMonsterType();
		MonsterInfo.m_iCampId			= GetCamp();

		for(int i = 0; i < SHOP_COUNT; i++)
		{
			MonsterInfo.m_iShopArray[i] = GetShopIndex(i);
		}

	}

}




// 减少怪物的方向
void CMonsterEditDlg::DecMonsterDir()
{
	if(m_pMonsterEditAction)
	{
		CMonsterInfo* pMonsterInfo = m_pMonsterEditAction->GetMonster(m_pMonsterEditAction->m_dwCurSelMonsterInstanceId);
		
		if(NULL == pMonsterInfo)
		{
			return;
		}

		pMonsterInfo->m_fMonstDir -= DIR_STEP;
		m_pMonsterEditAction->ChangeMonsterDir(m_pMonsterEditAction->m_dwCurSelMonsterInstanceId, pMonsterInfo->m_fMonstDir);
		
		
		char buf[1024];
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%.4f", pMonsterInfo->m_fMonstDir);
		if(m_pMonsterDirText)
		{
			m_pMonsterDirText->SetLabel(buf);
		}

		m_pMonsterEditAction->SetModifyed();

	
		if(m_MonsterInfoTable.count(m_ulCurEditMonsterInstance))
		{
			CMonsterInstanceInfo& MonsterInfo = m_MonsterInfoTable[m_ulCurEditMonsterInstance];
			MonsterInfo.m_fMonstDir         = GetDir();
		}

	}

	
}


// 增加怪物方向。
void CMonsterEditDlg::IncMonsterDir()
{

	if(m_pMonsterEditAction)
	{
		CMonsterInfo* pMonsterInfo = m_pMonsterEditAction->GetMonster(m_pMonsterEditAction->m_dwCurSelMonsterInstanceId);
		
		if(NULL == pMonsterInfo)
		{
			return;
		}

		pMonsterInfo->m_fMonstDir += DIR_STEP;
		m_pMonsterEditAction->ChangeMonsterDir(m_pMonsterEditAction->m_dwCurSelMonsterInstanceId, pMonsterInfo->m_fMonstDir);


		char buf[1024];
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "%.4f", pMonsterInfo->m_fMonstDir);
		if(m_pMonsterDirText)
		{
			m_pMonsterDirText->SetLabel(buf);
		}

		m_pMonsterEditAction->SetModifyed();


		if(m_MonsterInfoTable.count(m_ulCurEditMonsterInstance))
		{
			CMonsterInstanceInfo& MonsterInfo = m_MonsterInfoTable[m_ulCurEditMonsterInstance];
			MonsterInfo.m_fMonstDir         = GetDir();
		}

	}



}

// 清空界面.
void CMonsterEditDlg::ClearUI()
{
	m_MonsterInfoTable.clear();
	m_iCurSelListCtrlIndex = -1;
	// 重新构建已经添加物体的树型控件.
	InitAddedMonsterTree();

	// 清空文本框.
	
	if(m_pMonsterNameText)
	{
		m_pMonsterNameText->SetLabel("");
	}

	if(m_pMonsterLevelText)
	{
		m_pMonsterLevelText->SetLabel("");
	}

	if(m_pMonsterExpText)
	{
		m_pMonsterExpText->SetLabel("");
	}

	if(m_pMonsterMoveSpeedText )
	{
		m_pMonsterMoveSpeedText ->SetLabel("");
	}

	if(m_pMonsterfreshTimeText )
	{
		m_pMonsterfreshTimeText ->SetLabel("");
	}

	if(m_pMonsterRadiusText)
	{
		m_pMonsterRadiusText->SetLabel("");
	}

	if(m_pMonsterCampIdText)
	{
		m_pMonsterCampIdText->SetLabel("");
	}

	if(m_pMonsterBaseAIText)
	{
		m_pMonsterBaseAIText->SetLabel("");
	}

	if(m_pMonsterAdvanceAIText)
	{
		m_pMonsterAdvanceAIText->SetLabel("");
	}

	if(m_pMonsterEventText)
	{
		m_pMonsterEventText->SetLabel("");
	}

	if(m_pMonsterGroupIdText)
	{
		m_pMonsterGroupIdText->SetLabel("");
	}

	if(m_pMonsterTeamIdText)
	{
		m_pMonsterTeamIdText->SetLabel("");
	}

	if(m_pMonsterDirText)
	{
		m_pMonsterDirText->SetLabel("");
	}

	ClearCounter();
	
}


// 清空action数据
void CMonsterEditDlg::ClearActionData()
{
	if(m_pMonsterEditAction)
	{
		m_pMonsterEditAction->ReleaseAllMonster();
	}

}

	// 清空计数器
void CMonsterEditDlg::ClearCounter()
{
	m_dwAddedMonsterCount = 0;
}


// 得到guid
DWORD CMonsterEditDlg::GetGuid()
{

	u_long dwId = 0;
	time_t  LocalTime;
	
	// 得到系统时间
	time(&LocalTime);
	
	// 得到本机的ip地址
	u_long ulIp = htonl(INADDR_ANY);

	LocalTime = 0x00ffffff & LocalTime;

	dwId = ulIp<< 24;

	dwId = dwId | LocalTime;

	return dwId; 

}

// 得到基础ai
int CMonsterEditDlg::GetBaseAIId()
{
	if(m_pChoiceBaseAiId)
	{
		return m_pChoiceBaseAiId->GetSelection();
	}
	else
	{
		return -1;
	}
}


// 得到高级ai
int CMonsterEditDlg::GetAdvanceAIId()
{
	if(m_pChoiceAIFile)
	{
		return m_pChoiceAIFile->GetSelection();
	}
	else
	{
		return -1;
	}
}

int CMonsterEditDlg::GetMonsterType()
{
	if(m_pMonsterType)
	{
		return m_pMonsterType->GetSelection();
	}
	else
	{
		return 0;
	}
}
int CMonsterEditDlg::GetCamp()
{
	if(m_pMonsterCampIdText)
	{
		return atoi( m_pMonsterCampIdText->GetLabel().c_str() );
	}
	else
	{
		return 20;
	}
}

// 得到事件id
int CMonsterEditDlg::GetEvenId()
{
	if(m_pChoiceEvenFile)
	{
		int iIndex = m_pChoiceEvenFile->GetSelection();
		std::string strName = m_pChoiceEvenFile->GetString(iIndex);

		int iPos = strName.find("=");

		if(iPos <= 0)
		{
			return -1;
		}

		std::string strNum = strName.substr(0, iPos);
		return atoi(strNum.c_str());


	}
	else
	{
		return -1;
	}
}

// 得到组id
int CMonsterEditDlg::GetGroupId()
{
	if(m_pMonsterGroupIdText)
	{
		if("" == m_pMonsterGroupIdText->GetValue())
		{
			return -1;
		}

		return atoi(m_pMonsterGroupIdText->GetValue().c_str());
	}
	else
	{
		return -1;
	}
	
}

	
// 得到队伍id
int CMonsterEditDlg::GetTeamId()
{
	if(m_pMonsterTeamIdText)
	{
		if("" == m_pMonsterTeamIdText->GetValue())
		{
			return -1;
		}


		return atoi(m_pMonsterTeamIdText->GetValue().c_str());
	}
	else
	{
		return -1;
	}
}

// 得到巡逻路线
int CMonsterEditDlg::GetPatrolId()
{
	if(m_pMonsterPatrol)
	{
		if("" == m_pMonsterPatrol->GetValue())
		{
			return -1;
		}


		return atoi(m_pMonsterPatrol->GetValue().c_str());
	}
	else
	{
		return -1;
	}
}

int CMonsterEditDlg::GetLevel()
{

	if(m_pMonsterLevelText)
	{
		return atoi(m_pMonsterLevelText->GetValue());
	}

	return -1;
}
	
// 得到实例名字
std::string CMonsterEditDlg::GetInstanceName()
{
	std::string strName = "";

	if(m_pMonsterInstanceName)
	{
		strName = m_pMonsterInstanceName->GetValue().c_str();
	}

	return strName;
}

// 得到称号名字
std::string CMonsterEditDlg::GetTitleName()
{
	std::string strName = "";

	if(m_pMonsterTitleName)
	{
		strName = m_pMonsterTitleName->GetValue().c_str();
	}

	return strName;
}

// 得到方向
float CMonsterEditDlg::GetDir()
{

	if(m_pMonsterDirText)
	{
		return atof(m_pMonsterDirText->GetValue());
	}

	return 0;
}


// 得到刷新时间
int CMonsterEditDlg::GetRefreshTime()
{
	if(m_pMonsterfreshTimeText)
	{
		if("" == m_pMonsterfreshTimeText->GetValue())
		{
			return -1;
		}

		return atoi(m_pMonsterfreshTimeText->GetValue().c_str());
	}

	return -1;
}

// 得到位置
Ogre::Vector3 CMonsterEditDlg::GetMonsterPos()
{
	Ogre::Vector3 pos(0, 0, 0);

	if(m_pMonsterEditAction)
	{
		return m_pMonsterEditAction->GetMonsterPos(m_ulCurEditMonsterInstance);
	}

	return  pos;
}



// 从区域读取文件
unsigned long CMonsterEditDlg::ReadMonsterFromFile(std::string& strFileName)
{
	// 清空数据
	m_MonsterInfoTable.clear();

	char bufMonsterIndex[64];
	int iMonsterCount = ::GetPrivateProfileInt("info", "monstercount", 0, strFileName.c_str());
	
	char bufX[64];
	char bufZ[64];

	String  MonsterTypename;
	Ogre::Vector3 pos;

	char bufInstanceName[128];
	CMonsterInstanceInfo MonsterInfo;
	
	for(int i = 0; i < iMonsterCount; i++)
	{

		memset(&MonsterInfo, 0, sizeof(MonsterInfo));
		memset(bufMonsterIndex, 0, sizeof(bufMonsterIndex));
		memset(bufX, 0, sizeof(bufX));
		memset(bufZ, 0, sizeof(bufZ));
		sprintf(bufMonsterIndex, "monster%d", i);

		// 读取怪物类型
		MonsterInfo.m_dwObjectId = ::GetPrivateProfileInt(bufMonsterIndex, "type", -1, strFileName.c_str());

		if(0 == m_IdNameMap.count(MonsterInfo.m_dwObjectId))
		{
			char bufMessage[1024];
			memset(bufMessage, 0, sizeof(bufMessage));
			sprintf(bufMessage, wxT("文件中发现了场景编辑器无法识别的怪物类型-- %d!"), MonsterInfo.m_dwObjectId);
			wxLogMessage(bufMessage);
			continue;
		}
		
		// 读取怪物guid
		MonsterInfo.m_dwMonsterGUID = ::GetPrivateProfileInt(bufMonsterIndex, "guid", 0xffffffff, strFileName.c_str());

		// 读取怪物实例名字
		memset(bufInstanceName, 0, sizeof(bufInstanceName));
		::GetPrivateProfileString(bufMonsterIndex, "name",  "none", bufInstanceName, sizeof(bufInstanceName), strFileName.c_str());
		MonsterInfo.m_strInstanceName = bufInstanceName;

		// 称号
		memset(bufInstanceName, 0, sizeof(bufInstanceName));
		::GetPrivateProfileString(bufMonsterIndex, "title",  "none", bufInstanceName, sizeof(bufInstanceName), strFileName.c_str());
		MonsterInfo.m_strTitleName = bufInstanceName;

		// 读取位置
		::GetPrivateProfileString(bufMonsterIndex, "pos_x",  "none", bufX, sizeof(bufX), strFileName.c_str());
		::GetPrivateProfileString(bufMonsterIndex, "pos_z",  "none", bufZ, sizeof(bufZ),strFileName.c_str());
		MonsterInfo.m_position.x = atof(bufX);
		MonsterInfo.m_position.z = atof(bufZ);

		// 读取方向。
		int iMonsterDir = ::GetPrivateProfileInt(bufMonsterIndex, "dir",  -1, strFileName.c_str());
		iMonsterDir += 9;
		iMonsterDir = iMonsterDir % DIR_STEP_TRANS;
		MonsterInfo.m_fMonstDir = (iMonsterDir*1.0 / DIR_STEP_TRANS) * _2PI;

		// 读取事件id
		MonsterInfo.m_EvenId  = ::GetPrivateProfileInt(bufMonsterIndex, "script_id", -1, strFileName.c_str());

		// 读取刷新时间
		MonsterInfo.m_iRefreshTime = ::GetPrivateProfileInt(bufMonsterIndex, "respawn_time", -1, strFileName.c_str());

		// 读取groupid
		 MonsterInfo.m_iGroupId = ::GetPrivateProfileInt(bufMonsterIndex, "group_id", -1, strFileName.c_str());

		// 读取teamid
		MonsterInfo.m_iTeamId = ::GetPrivateProfileInt(bufMonsterIndex, "team_id", -1, strFileName.c_str());

		// 读怪物的basseAiId
		MonsterInfo.m_iBaseAIId = ::GetPrivateProfileInt(bufMonsterIndex, "base_ai",  -1, strFileName.c_str());

		// 读怪物的高级ai文件名字
		MonsterInfo.m_iAdvanceAIId = ::GetPrivateProfileInt(bufMonsterIndex, "ai_file",  -1, strFileName.c_str());

		// 读取商店信息
		MonsterInfo.m_iShopArray[0] = ::GetPrivateProfileInt(bufMonsterIndex, "shop0",  -1, strFileName.c_str());
		MonsterInfo.m_iShopArray[1] = ::GetPrivateProfileInt(bufMonsterIndex, "shop1",  -1, strFileName.c_str());
		MonsterInfo.m_iShopArray[2] = ::GetPrivateProfileInt(bufMonsterIndex, "shop2",  -1, strFileName.c_str());
		MonsterInfo.m_iShopArray[3] = ::GetPrivateProfileInt(bufMonsterIndex, "shop3",  -1, strFileName.c_str());
	
		// 读取巡逻路线id
		MonsterInfo.m_iLineid = ::GetPrivateProfileInt(bufMonsterIndex, "patrol_id",  -1, strFileName.c_str());

		// 读取怪物势力
		MonsterInfo.m_iReputationID = ::GetPrivateProfileInt(bufMonsterIndex, "ReputationID",  -1, strFileName.c_str());

		// 等级
		MonsterInfo.m_iLevel = ::GetPrivateProfileInt(bufMonsterIndex, "level",  -1, strFileName.c_str());

		MonsterInfo.m_iType = ::GetPrivateProfileInt(bufMonsterIndex, "npc",  1, strFileName.c_str());

		MonsterInfo.m_iCampId = ::GetPrivateProfileInt(bufMonsterIndex, "camp",  20, strFileName.c_str());

		m_MonsterInfoTable[-1] = MonsterInfo;

		// 添加一个新的怪物.
		if(m_pMonsterEditAction)
		{
			m_pMonsterEditAction->AddNewMonster(MonsterInfo.m_position, MonsterInfo.m_dwObjectId, true);
		}
	}

	if(m_pMonsterEditAction)
	{
		m_pMonsterEditAction->HideScene(m_pMonsterEditAction->m_bIsShow);
	}
	return 0;
}


	
// 把区域存储到文件
unsigned long CMonsterEditDlg::SaveMonsterToFile(std::string& strFileName)
{
	char bufWrite[128];
	char bufMonsterIndex[128];
	int iMonsterType  = 0;
	int  iMonsterDir;
	char bufX[64];
	char bufZ[64];

	float fx = 0;
	float fy = 0;
	float fz = 0;

	// 删除旧的文件。
	::DeleteFile(strFileName.c_str());

    String  MonsterTypename;
	Ogre::Vector3 pos;

	int iMonsterCount = 0;
	iMonsterCount = m_MonsterInfoTable.size();

	memset(bufWrite, 0, sizeof(bufWrite));
	sprintf(bufWrite, "%d\r\n", iMonsterCount);

	// 填写怪物的个数
	WritePrivateProfileString("info", "monstercount", bufWrite, strFileName.c_str());
	
	INSTANCE_MONSTERINFO_MAP::iterator it;
	INSTANCE_MONSTERINFO_MAP::iterator itEnd;
	
	itEnd = m_MonsterInfoTable.end();
	int i = 0;
	
	for(it = m_MonsterInfoTable.begin(); it != itEnd; it++)
	{
		CMonsterInstanceInfo& MonsterInfo = it->second;

		memset(bufMonsterIndex, 0, sizeof(bufMonsterIndex));
		sprintf(bufMonsterIndex, "monster%d", i);

		// 写怪物的guid
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_dwMonsterGUID);
		::WritePrivateProfileString(bufMonsterIndex, "guid",  bufWrite, strFileName.c_str());
		
		// 写怪物的类型.
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_dwObjectId);
		::WritePrivateProfileString(bufMonsterIndex, "type", bufWrite, strFileName.c_str());

			
		// 写怪物名字
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%s", MonsterInfo.m_strInstanceName.c_str());
		::WritePrivateProfileString(bufMonsterIndex, "name", bufWrite, strFileName.c_str());

		// 称号
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%s", MonsterInfo.m_strTitleName.c_str());
		::WritePrivateProfileString(bufMonsterIndex, "title", bufWrite, strFileName.c_str());


		// 写怪物的位置
		memset(bufX, 0, sizeof(bufX));

		// 进行场景坐标和逻辑坐标转换
        fx = MonsterInfo.m_position.x;
		fy = MonsterInfo.m_position.y;
		fz = MonsterInfo.m_position.z;

		sprintf(bufX, "%.4f", fx);
		::WritePrivateProfileString(bufMonsterIndex, "pos_x",  bufX, strFileName.c_str());

		memset(bufZ, 0, sizeof(bufZ));
		sprintf(bufZ, "%.4f", fz);
		::WritePrivateProfileString(bufMonsterIndex, "pos_z",  bufZ, strFileName.c_str());

		// 写怪物的方向.
		iMonsterDir = (MonsterInfo.m_fMonstDir / _2PI) * DIR_STEP_TRANS;
		iMonsterDir = iMonsterDir + 27;
		iMonsterDir = iMonsterDir % DIR_STEP_TRANS;
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", iMonsterDir);
		::WritePrivateProfileString(bufMonsterIndex, "dir",  bufWrite, strFileName.c_str());


		// 写怪物的事件文件名字
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_EvenId);
		::WritePrivateProfileString(bufMonsterIndex, "script_id",  bufWrite, strFileName.c_str());


		// 写怪物刷新时间
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iRefreshTime);
		::WritePrivateProfileString(bufMonsterIndex, "respawn_time",  bufWrite, strFileName.c_str());


		// 写怪物的groupid
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iGroupId);
		::WritePrivateProfileString(bufMonsterIndex, "group_id",  bufWrite, strFileName.c_str());

		// 写怪物的teamid
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iTeamId);
		::WritePrivateProfileString(bufMonsterIndex, "team_id",  bufWrite, strFileName.c_str());

		// 写怪物的basseAiId
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iBaseAIId);
		::WritePrivateProfileString(bufMonsterIndex, "base_ai",  bufWrite, strFileName.c_str());

		// 写怪物的高级ai文件名字
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iAdvanceAIId);
		::WritePrivateProfileString(bufMonsterIndex, "ai_file",  bufWrite, strFileName.c_str());

		// 写巡逻路线id
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iLineid);
		::WritePrivateProfileString(bufMonsterIndex, "patrol_id",  bufWrite, strFileName.c_str());

		// 写商店信息

		//1
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iShopArray[0]);
		::WritePrivateProfileString(bufMonsterIndex, "shop0",  bufWrite, strFileName.c_str());

		//2
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iShopArray[1]);
		::WritePrivateProfileString(bufMonsterIndex, "shop1",  bufWrite, strFileName.c_str());

		//3
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iShopArray[2]);
		::WritePrivateProfileString(bufMonsterIndex, "shop2",  bufWrite, strFileName.c_str());

		//4
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iShopArray[3]);
		::WritePrivateProfileString(bufMonsterIndex, "shop3",  bufWrite, strFileName.c_str());

		//势力
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iReputationID);
		::WritePrivateProfileString(bufMonsterIndex, "ReputationID",  bufWrite, strFileName.c_str());
		

		// 写怪物的level
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iLevel);
		::WritePrivateProfileString(bufMonsterIndex, "level",  bufWrite, strFileName.c_str());

		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d", MonsterInfo.m_iType);
		::WritePrivateProfileString(bufMonsterIndex, "npc",  bufWrite, strFileName.c_str());
       
		memset(bufWrite, 0, sizeof(bufWrite));
		sprintf(bufWrite, "%d\r\n", MonsterInfo.m_iCampId);
		::WritePrivateProfileString(bufMonsterIndex, "camp",  bufWrite, strFileName.c_str());

		// 怪物编号
		i++;
	
	}

	return 0;
}


// 得到商店索引
int CMonsterEditDlg::GetShopIndex(int iIndex)
{
	
	if(-1 == iIndex)
	{
		return - 1;
	}

	int iSelectIndex = -1;
	wxChoice*  pShop = NULL;
	pShop = m_pShop[iIndex];
		

	if(pShop)
	{
		iSelectIndex   = pShop->GetSelection();
		if(-1 == iSelectIndex)
		{
			return -1;
		}


		// 假如选择的是最后一个元素.
		if(iSelectIndex == pShop->GetCount() - 1)
		{
			return -1;
		}

		int iShopIndex = (int)pShop->GetClientData(iSelectIndex);
		return iShopIndex;
	}
	else
	{
		return -1;
	}

	return -1;
}

int CMonsterEditDlg::GetReputationID()
{
	if(m_pMonsterReputationID)
	{
		if("" == m_pMonsterReputationID->GetValue())
		{
			return -1;
		}

		return atoi(m_pMonsterReputationID->GetValue().c_str());
	}

	return -1;
}

void CMonsterEditDlg::ShowMonsterTypeInfoByIndex(int iSelIndex)
{
	if(NULL == m_pMonsterListCtrl)
	{
		return;
	}
	
	wxListItem itemInfo;
	int iSize = m_pMonsterListCtrl->GetItemCount();
		
	if(iSelIndex >= iSize)
	{
		return;
	}

	// 为什么不能加这句话， 这个界面开发工具包太恶了！
	//memset(&itemInfo, 0, sizeof(itemInfo));
	itemInfo.m_itemId = iSelIndex;
    itemInfo.m_col = 1;
    itemInfo.m_mask = wxLIST_MASK_TEXT;

	

	// 得到名字
	m_pMonsterListCtrl->GetItem(itemInfo);
	if(m_pMonsterNameText)
	{
		m_pMonsterNameText->SetLabel(itemInfo.GetText());
	}

	// 得到等级
	itemInfo.SetColumn(2);
	m_pMonsterListCtrl->GetItem(itemInfo);
	if(m_pMonsterLevelText)
	{
		//if( m_pMonsterLevelText->GetLabel() == "-1" || m_pMonsterLevelText->GetLabel() == "" )
			m_pMonsterLevelText->SetLabel(itemInfo.GetText());
	}

	// 得到经验
	itemInfo.SetColumn(3);
	m_pMonsterListCtrl->GetItem(itemInfo);
	if(m_pMonsterExpText)
	{
		m_pMonsterExpText->SetLabel(itemInfo.GetText());
	}

	// 得到移动速度
	itemInfo.SetColumn(4);
	m_pMonsterListCtrl->GetItem(itemInfo);
	if(m_pMonsterMoveSpeedText)
	{
		m_pMonsterMoveSpeedText->SetLabel(itemInfo.GetText());
	}//

	//// 得到重生时间
	//itemInfo.SetColumn(5);
	//m_pMonsterListCtrl->GetItem(itemInfo);
	//if(m_pMonsterfreshTimeText)
	//{
	//	m_pMonsterfreshTimeText->SetLabel(itemInfo.GetText());
	//}//

	// 得到活动范围
	itemInfo.SetColumn(6);
	m_pMonsterListCtrl->GetItem(itemInfo);
	if(m_pMonsterRadiusText)
	{
		m_pMonsterRadiusText->SetLabel(itemInfo.GetText());
	}//

	// 得到阵营ID
	itemInfo.SetColumn(7);
	m_pMonsterListCtrl->GetItem(itemInfo);
	if(m_pMonsterCampIdText)
	{
		m_pMonsterCampIdText->SetLabel(itemInfo.GetText());
	}//
}


// 重置数据信息2006-4-5
void CMonsterEditDlg::ReSetDataInfo()
{
	//
	// 初试化数据
	//
	if(m_pChoiceEvenFile->GetCount())
	{
		m_pChoiceEvenFile->Select(0);
	}

	// 基本ai
	if(m_pChoiceBaseAiId)
	{
		m_pChoiceBaseAiId->Select(0);
	}

	// 高级ai
	if(m_pChoiceAIFile)
	{
		m_pChoiceAIFile->Select(0);
	}

	if(m_pMonsterType)
	{
		m_pMonsterType->Select(0);
	}

	m_pMonsterTeamIdText->SetLabel("-1");		// 怪物队伍id
	m_pMonsterDirText->SetLabel("0.00");		// 怪物方向
	m_pMonsterInstanceName->SetLabel("");		// 怪物实例名字
	m_pMonsterTitleName->SetLabel("");			// 怪物称号名字
	m_pMonsterPatrol->SetLabel("-1");			// 怪物巡逻路线
	m_pMonsterGroupIdText->SetLabel("-1");		// 组id

	for(int i = 0; i < SHOP_COUNT; i++)
	{
		m_pShop[i]->Select(m_pShop[i]->GetCount()-1);
	}
}








