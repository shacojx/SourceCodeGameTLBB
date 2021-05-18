#include "StdAfx.h"
#include "Gamepro_Main.h"
#include "..\Global.h"
#include "..\World\WorldManager.h"
#include "..\Object\Character\Obj_PlayerMySelf.h"
#include "..\Object\ObjectManager.h"
#include "CGEnvRequest.h"
#include "CGCharAskEquipment.h"
#include "CGAskDetailEquipList.h"
#include "CGCharAskBaseAttrib.h"
#include "CGAskDetailAttrib.h"
#include "CGAskDetailXinFaList.h"
#include "CGAskDetailSkillList.h"
#include "CGAskMissionList.h"
#include "CGAskTeamInfo.h"
#include "CGAskSetting.h"
#include "CGCharAllTitles.h"
#include "CGRelation.h"
#include "CGAskMail.h"
#include "CGBankMoney.h"
#include "CGCityAskAttr.h"
#include "..\Input\GMInputSystem.h"
#include "TDSound.h"
#include "TDUtil.h"
#include "..\GameCommand.h"
#include "..\Object\ObjectCommandDef.h"
#include "..\Interface\GMGameInterface.h"
#include "..\Object\TripperObj\GMTripperObj_ItemBox.h"
#include "CGAskMyBagList.h"
#include "CGAskChangeScene.h"
#include "GCMissionList.h"
#include "CGMinorPasswd.h"
#include "TDDebuger.h"
#include "..\Cursor\CursorMng.h"
#include "..\DBC\GMDataBase.h"
#include "..\Object\ProjTex\Obj_ProjTex.h"
#include "CGAskDetailAbilityInfo.h"
#include "..\Action\GMActionSystem.h"
#include "..\DataPool\GMDP_CharacterData.h"
#include "TDVariable.h"
#include "GamePro_Login.h"
#include "..\DataPool\GMUIDataPool.h"
#include "..\DataPool\GMDataPool.h"
#include "..\Event\GMEventSystem.h"
#include "TDEventDefine.h"
#include "..\Sound\GMSoundSystem.h"
#include "TDUISystem.h"
#include "..\Object\Character\AI\GMAI_MySelf.h"
#include "..\Object\Character\AI\GMAICommandDef.h"
#include "..\FakeObject\GMFakeObjSystem.h"
#include "TDTimesystem.h"
#include "AxProfile.h"
#include "..\Object\GMObjectLoadQueue.h"
#include "..\Object\Surface\Obj_Special.h"
#include "..\Object\Surface\Obj_Bullet.h"

#include "..\Interface\GMInterface_Script_PetInviteFriend.h"
#include "..\Object\Obj_Building.h"
#include "..\Helper\GMHelperSystem.h"
#include "..\Event\GMEventSystem.h"
#include "GamePro_Enter.h"
#include "CGEnterScene.h"
#include "CGApplyPrivateInfo.h"
#include "CGAskPrivateInfo.h"
#include "..\WxRender\RenderSystem.h"
#include "..\cegui\cegui\include\CEGUIImage.h"

bool		gbTestMinimap = true;
int			CGamePro_Main::m_nResetCamera		= 0;

static	FLOAT	fTransDistance = 1.0f;
CGamePro_Main::CGamePro_Main()
{
	m_bCameraDraging = FALSE;
	m_pSkillArea = NULL;
	m_pActiveSkill = NULL;
	m_pMouseTarget = NULL;
	m_bAutoRun = FALSE;
}

CGamePro_Main::~CGamePro_Main()
{
	//PetInviteFriend
	SCRIPT_SANDBOX::PetInviteFriend::s_PetInviteFriend.DestoryPetList();
}

VOID CGamePro_Main::Init(VOID)
{
	m_pMySelf = CObjectManager::GetMe()->GetMySelf();
	INT m_nID = m_pMySelf->GetServerID();
	m_pActiveSkill = NULL;
	m_pSkillArea = NULL;
	m_pMouseTarget = NULL;
	m_bAutoRun = FALSE;

	//------------------------------------------------------------------
	//重置LogicCount
	m_pMySelf->ResetLogicCount();

	//------------------------------------------------------------------
	//清空加载队列
	CObjectManager::GetMe()->GetLoadQueue()->ClearAllTask();

	//------------------------------------------------------------------
	//发送环境请求
	CGEnvRequest msgEnvRequest;
	CNetManager::GetMe()->SendPacket(&msgEnvRequest);

	//------------------------------------------------------------------
	//发送自身数据请求

	//自身基本信息
	CGCharAskBaseAttrib msgMyBaseAttrib;
	msgMyBaseAttrib.setTargetID(m_nID);
	CNetManager::GetMe()->SendPacket(&msgMyBaseAttrib);

	//基本属性
	CGAskDetailAttrib	msgMyAttrib;
	msgMyAttrib.setTargetID( m_nID );
	CNetManager::GetMe()->SendPacket(&msgMyAttrib);

	////自身装备(用于创建渲染层外形)
	//CGCharAskEquipment msgAskMyEquip;
	//msgAskMyEquip.setObjID(m_nID);
	//CNetManager::GetMe()->SendPacket(&msgAskMyEquip);

	//生活技能
	CGAskDetailAbilityInfo msgAbility;
	msgAbility.setTargetID(m_nID );
	CNetManager::GetMe()->SendPacket(&msgAbility);

	//技能系
	CGAskSkillClass	msgMyXinFa;
	msgMyXinFa.setTargetID( m_nID );
	CNetManager::GetMe()->SendPacket(&msgMyXinFa);

	//技能
	CGAskDetailSkillList	msgMySkill;
	msgMySkill.setTargetID( m_nID );
	CNetManager::GetMe()->SendPacket(&msgMySkill);

	//组队数据
	CUIDataPool::GetMe()->ClearTeamInfo();
	CGAskTeamInfo msgTeamInfo;
	msgTeamInfo.SetObjID( m_nID );
	CNetManager::GetMe()->SendPacket(&msgTeamInfo);

	//如果是城市向服务器请求城市数据
	CGCityAskAttr	MsgCityBuilding;
	CNetManager::GetMe()->SendPacket( &MsgCityBuilding );

	//所有称号
	CGCharAllTitles	msgAllTitles;
	msgAllTitles.setTargetID( m_nID );
	CNetManager::GetMe()->SendPacket(&msgAllTitles);

	//是第一次登陆
	if(CGameProcedure::s_pProcEnter->GetEnterType() == ENTER_TYPE_FIRST)
	{
		// 二级密码是否已经设置
		CGMinorPasswd MsgMinorPasswd;
		MsgMinorPasswd.GetMinorPassword()->CleanUp();
		MsgMinorPasswd.GetMinorPassword()->m_Type = MREQT_PASSWDSETUP;
		CNetManager::GetMe()->SendPacket(&MsgMinorPasswd);

		// 二级密码是否处在强制解除阶段
		CGMinorPasswd MsgDelMinorPasswd;
		MsgDelMinorPasswd.GetMinorPassword()->CleanUp();
		MsgDelMinorPasswd.GetMinorPassword()->m_Type = MREQT_DELETEPASSWDTIME;
		CNetManager::GetMe()->SendPacket(&MsgDelMinorPasswd);

		//详细装备(用于在数据池中存储物品详细属性)
		CGAskDetailEquipList msgAskMyDetailEquip;
		msgAskMyDetailEquip.setObjID( m_nID );
		msgAskMyDetailEquip.SetAskMode(ASK_EQUIP_ALL);
		CNetManager::GetMe()->SendPacket(&msgAskMyDetailEquip);

		//背包
		CGAskMyBagList	msgMyBag;
		msgMyBag.SetAskMode(ASK_ALL);
		CNetManager::GetMe()->SendPacket(&msgMyBag);

		//MissionList
		CGAskMissionList	msgMissionList;
		msgMissionList.setTargetID( m_nID );
		CNetManager::GetMe()->SendPacket(&msgMissionList);

		//向服务器请求快捷栏数据
		CGAskSetting msgSetting;
		CNetManager::GetMe()->SendPacket( &msgSetting );

		//向服务器请求关系人列表
		CGRelation MsgRelation;
		MsgRelation.GetRelation()->m_Type = REQ_RELATIONLIST;
		CNetManager::GetMe()->SendPacket( &MsgRelation );

		//向服务器请求邮件
		CGAskMail MsgMail;
		MsgMail.SetAskType( ASK_TYPE_LOGIN );
		CNetManager::GetMe()->SendPacket( &MsgMail );

		//向服务器请求银行中的金钱数量
		BYTE SaveType = CGBankMoney::UPDATE_MONEY | CGBankMoney::UPDATA_RMB;
		CGBankMoney	MsgBankMoney;
		MsgBankMoney.SetSaveType(SaveType);
		CNetManager::GetMe()->SendPacket( &MsgBankMoney );

		CGAskPrivateInfo msgPrivateInfo;
		msgPrivateInfo.SetPlayerID( INVALID_ID );
		CNetManager::GetMe()->SendPacket( &msgPrivateInfo );
	}


	s_pGfxSystem->Camera_SetZoom(fTransDistance);

	//Talk
	SCRIPT_SANDBOX::Talk::s_Talk.Initial();
	//PetInviteFriend
	SCRIPT_SANDBOX::PetInviteFriend::s_PetInviteFriend.InitPetList();

	//-----------------------------------------------------------------------------------------------------------------
	// 重新设置显示状态。2006-4-10
	if(CGameProcedure::s_pProcEnter->GetEnterType() == ENTER_TYPE_FIRST)
	{
		static BOOL bFirst = TRUE;
		if(bFirst)
		{
			const char* szNeedReset[] = 
			{
				"View_FullScreen",		//全屏模式
				"View_Resoution",		//分辨率
				"View_FullScreenLight",	//全屏泛光效果
			};
			for(int i=0; i<sizeof(szNeedReset)/sizeof(const char*); i++)
			{
				CEventSystem::GetMe()->PushEvent(
											GE_VARIABLE_CHANGED, 
											szNeedReset[i], 
											CGameProcedure::s_pVariableSystem->GetAs_String(szNeedReset[i]).c_str()
											);
			}
		}

		bFirst = FALSE;
	}
	//fujia 2008.1.15  不可以改变窗口大小
	////允许变窗口大小。
	//LONG lStyle = ::GetWindowLong(g_hMainWnd, GWL_STYLE);
	//lStyle = lStyle | WS_THICKFRAME ;
	//lStyle = lStyle | WS_MAXIMIZEBOX ;
	//::SetWindowLong(g_hMainWnd, GWL_STYLE, lStyle);//

	//Rresh title
	::PostMessage(g_hMainWnd, WM_NCACTIVATE, TRUE, 0);

	ShowWindow(g_hMainWnd, SW_SHOW);
}

VOID CGamePro_Main::Tick(VOID)
{
	CGameProcedure::Tick();

	if ( m_pMySelf != NULL )
	{
//		static FLOAT nLastPosX, nLastPosY;

		//---- for debug
		CHAR szTemp[MAX_PATH] = {0};
		//行走速度
		_snprintf(szTemp, MAX_PATH, "%.2f", m_pMySelf->GetCharacterData()->Get_MoveSpeed());
		CGameProcedure::s_pGfxSystem->SetDebugString("Speed", szTemp);
		//行走动作频率
		_snprintf(szTemp, MAX_PATH, "%.2f", ((CObject_PlayerOther*)m_pMySelf)->Debug_GetRate() );
		CGameProcedure::s_pGfxSystem->SetDebugString("Rate", szTemp);
		//显示位置
		fVector3 fvPosGfx;
		CGameProcedure::s_pGfxSystem->Axis_Trans(CRenderSystem::AX_GAME, m_pMySelf->GetPosition(),
			CRenderSystem::AX_GFX, fvPosGfx);

		_snprintf(szTemp, MAX_PATH, "%.1f, %.1f", 
			m_pMySelf->GetPosition().x, m_pMySelf->GetPosition().z);
		CGameProcedure::s_pGfxSystem->SetDebugString("Position", szTemp);
		static UINT nTimer = CGameProcedure::s_pTimeSystem->GetTimeNow();
		static UINT	nTalkTimer = CGameProcedure::s_pTimeSystem->GetTimeNow();
		if( gbTestMinimap )
		{
			if( CGameProcedure::m_bNeedFreshMinimap ) // 如果立马需要刷新小地图
			{
				if( CGameProcedure::s_pTimeSystem->GetTimeNow() - nTimer > 100 ) // 每秒刷10次
				{
					CWorldManager::GetMe()->setMinimapSerachRange( 3 );
					CWorldManager::GetMe()->MinimapNeedFresh();
					CWorldManager::GetMe()->UpdateMinimapData();
					CEventSystem::GetMe()->PushEvent(GE_UPDATE_MAP);	
					nTimer = CGameProcedure::s_pTimeSystem->GetTimeNow();
					CGameProcedure::m_bWaitNeedFreshMinimap = false;
					CGameProcedure::m_bNeedFreshMinimap		= false;
				}
			}
			else if( CGameProcedure::m_bWaitNeedFreshMinimap ) // 除了主角以外的别的东西变了
			{
				if( CGameProcedure::s_pTimeSystem->GetTimeNow() - nTimer > 1000 ) // 每秒刷3次
				{
					CWorldManager::GetMe()->setMinimapSerachRange( 3 );
					CWorldManager::GetMe()->MinimapNeedFresh();
					CWorldManager::GetMe()->UpdateMinimapData();
					CEventSystem::GetMe()->PushEvent(GE_UPDATE_MAP);	
					nTimer = CGameProcedure::s_pTimeSystem->GetTimeNow();
					CGameProcedure::m_bWaitNeedFreshMinimap = false;
				}
			}
		}

		if(CGameProcedure::s_pTimeSystem->GetTimeNow() - nTalkTimer > 1000)
		{
			CEventSystem::GetMe()->PushEvent(GE_TIME_UPDATE);
			nTalkTimer = CGameProcedure::s_pTimeSystem->GetTimeNow();
		}
		if( m_nResetCamera != 0 ) // 摄影机复位过程中
		{
			s_pGfxSystem->Camera_AddDirection( CGameProcedure::s_pTimeSystem->GetDeltaTime() / 1000 * m_nResetCamera ); 
			if( m_nResetCamera < 0 )
			{
				if( s_pGfxSystem->Camera_GetDirection() < __PI ) // 如果已经抄过界限了
				{
					s_pGfxSystem->Camera_Direction( __PI );
					m_nResetCamera = 0;
				}
			}
			else 
			{
				if( s_pGfxSystem->Camera_GetDirection() > __PI ) 
				{
					s_pGfxSystem->Camera_Direction( __PI );
					m_nResetCamera = 0;
				}
			}
		}
	}
}

VOID CGamePro_Main::Render(VOID)
{
	if(s_pGfxSystem) s_pGfxSystem->RenderFrame();
}

VOID CGamePro_Main::ProcessInput(VOID)
{
	ProcessInput_Keybord();
	ProcessInput_Mouse();
}

//处理键盘
VOID CGamePro_Main::ProcessInput_Keybord(VOID)
{
	static INT CanDieUseKey[] ={KC_ESCAPE, KC_SYSRQ};

	CObject_PlayerMySelf *pMySelf	= CObjectManager::GetMe()->GetMySelf();

	//-----------------------------------------------------------
	//如果主角渲染层对象尚未建立，暂不接受任何键盘消息
	if(pMySelf == NULL || pMySelf->GetRenderInterface() == NULL) return;

	//分析事件队列
	InputEventQueue& inputQueue = s_pInputSystem->GetInputQueue();
	for(INT i=0; i<(INT)inputQueue.size(); i++)
	{
		InputEvent& event = inputQueue[i];
		if(event.bProcessed) continue;

		//人物死亡的时候能处理的快捷键才放行

		if(pMySelf->IsDie() && (event.nEvent==EVENT_KEY_DOWN || event.nEvent == EVENT_KEY_UP))
		{
			BOOL bCanDieUse = FALSE;
			for(INT i = 0; i < sizeof(CanDieUseKey)/sizeof(INT); ++i)
			{
				if(CanDieUseKey[i] == event.kIndex) 
				{
					bCanDieUse = TRUE;
					break;
				}
			}

			//不可使用的键
			if(!bCanDieUse)
			{
				event.bProcessed = TRUE;
				continue;
			}
		}

		if(event.nEvent == EVENT_KEY_DOWN)
		{
		switch(event.kIndex)
			{
			//主工具条快捷键
			case KC_F1:
			case KC_F2:
			case KC_F3:
			case KC_F4:
			case KC_F5:
			case KC_F6:
			case KC_F7:
			case KC_F8:
			case KC_F9:
			case KC_F10:
				CActionSystem::GetMe()->MainMenuBar_DoAction(event.kIndex-KC_F1, true);
				break;
			case KC_A:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_a");
				}
				break;
			case KC_GRAVE:
				break;
			case KC_B:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_b");
				}
				break;
			case KC_C:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_c");
				}
				break;
			case KC_D:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_d");
				}
				break;
			case KC_E:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_e");
				}
				break;
			case KC_F:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_f");
				}
				break;
			//调试键(ALT+G)
			case KC_G:	
				{
#ifndef _FINAL
					if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
					{
						CGameProcedure::s_pGfxSystem->Debug_ToggleShowDebugPanel();
						event.bProcessed = true;
					}
#endif
				}
				break;
			case KC_H:
				{
					if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
					{
						CHelperSystem* pHelpSystem = (CHelperSystem*)CGameProcedure::s_pHelperSystem;
						if(pHelpSystem)
						{
							pHelpSystem->GotoAnchorHelp(STRING(""));
						}
					}
				}
				break;
			case KC_I:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					gbTestMinimap = !gbTestMinimap;
				}
				break;
			case KC_J:
				{
					if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
					{
						// 处理跳跃
						CAI_Base *pMySelfAI;
						pMySelfAI		= CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

						((CAI_MySelf*)pMySelfAI)->PushCommand_Jump();
					}
				}
				break;
			case KC_K:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_k");
				}
				break;
			case KC_L:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_l");
				}
				break;
			case KC_M:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_m");
				}
				break;
			//调试键(ALT+N)
			case KC_N:
				break;
			case KC_O:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_o");
				}
				break;
			case KC_P:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_p");
				}
				break;
			case KC_Q:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_q");
				}
				break;
			case KC_R:
				break;
			case KC_S:
					if(CGameProcedure::s_pUISystem->IsIMEEditboxActive("InfoEdit_EditInfo"))
					{
						CEventSystem::GetMe()->PushEvent( GE_SEND_MAIL );
					}
				break;
			case KC_T:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_t");
				}
				break;
			case KC_U:
				break;
			case KC_V:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_v");
				}
				break;
			case KC_W:
				break;
			case KC_X:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_x");
				}
				break;
			case KC_Y:
				break;
			case KC_Z:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_z");
				}
				break;
			case KC_1:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift_1");
				}
				break;
			case KC_2:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift_2");
				}
				break;
			case KC_3:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift_3");
				}
				break;
			case KC_4:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift_4");
				}
				break;
			case KC_5:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift_5");
				}
				break;
			case KC_6:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift_6");
				}
				break;
			case KC_7:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift_7");
				}
				break;
			case KC_8:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift_8");
				}
				break;
			case KC_9:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift_9");
				}
				break;
			case KC_0:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "shift_0");
				}
				break;
			case KC_PGUP:
			case KC_PGDOWN:
				{
					// 处理跳跃
					CAI_Base *pMySelfAI;
					pMySelfAI		= CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI();

					//查询如果玩家处于摆摊状态，不能跳
					if(FALSE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
					{
						((CAI_MySelf*)pMySelfAI)->PushCommand_Jump();
					}
				}
				break;
			case KC_TAB:
				{
					if( s_pInputSystem->IsKeyDown( KC_LMENU ) ) 
						break;
					std::vector< STRING > vParam;
					vParam.push_back( CWorldManager::GetMe()->GetActiveScene()->GetSceneDefine()->szSceneMap );
					vParam.push_back( "2" );
					CEventSystem::GetMe()->PushEvent(GE_TOGLE_SCENEMAP, vParam );
					CGameProcedure::m_bNeedFreshMinimap = true;
					CGameProcedure::m_bWaitNeedFreshMinimap = true;

				}
				break;
			case KC_ESCAPE:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxActive("Chat_Frame_EditBox"))
				{
					CEventSystem::GetMe()->PushEvent( GE_CHAT_CLOSED );
				}else
				{
					s_pUISystem->OnPushEcsape();
					//取消特殊鼠标指令
					CGameInterface::GetMe()->Skill_CancelAction();
					
				}
				break;
			case KC_LEFT:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					s_pGfxSystem->Camera_AddDirection(0.04f);
				}
				break;
			case KC_RIGHT:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					s_pGfxSystem->Camera_AddDirection(-0.04f);
				}
				break;
			case KC_UP:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					s_pGfxSystem->Camera_SetZoom( true, 2 );
				}
				else
				{
					if(CGameProcedure::s_pUISystem->IsIMEEditboxActive("Chat_Frame_EditBox"))
					{
						SCRIPT_SANDBOX::Talk::s_Talk.HandleKey_Shift_ArrowUp(0);
					}
				}
				break;
			case KC_DOWN:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
						s_pGfxSystem->Camera_SetZoom( FALSE, 2 );
				}
				else
				{
					if(CGameProcedure::s_pUISystem->IsIMEEditboxActive("Chat_Frame_EditBox"))
					{
						SCRIPT_SANDBOX::Talk::s_Talk.HandleKey_Shift_ArrowDown(0);
					}
				}
				break;
			case KC_RETURN:
			case KC_NUMPADENTER:
				if( CGameProcedure::s_pUISystem->IsIMEEditboxActive("InfoEdit_EditInfo"))
				{
					CEventSystem::GetMe()->PushEvent( GE_SEND_MAIL );
				}else if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
						CEventSystem::GetMe()->PushEvent( GE_CHAT_OPEN );
				}
				break;
			case KC_DELETE:
				if(CGameProcedure::s_pUISystem->IsIMEEditboxEmpty())
				{
					CEventSystem::GetMe()->PushEvent(GE_ACCELERATE_KEYSEND, "alt_delete");
				}
				break;
			case KC_MINUS:
					//聊天图标
					/*if(CGameProcedure::s_pUISystem->IsIMEEditboxActive("Chat_Frame_EditBox"))
					{
						SCRIPT_SANDBOX::Talk::s_Talk.HandleKey_Alt_FaceMotion();
					}*/
				break;
			case KC_COMMA:
				//聊天栏GM命令滚动
				if(s_pInputSystem->IsKeyDown(KC_LSHIFT) || s_pInputSystem->IsKeyDown(KC_RSHIFT))
				{
					if(CGameProcedure::s_pUISystem->IsIMEEditboxActive("Chat_Frame_EditBox"))
					{
						SCRIPT_SANDBOX::Talk::s_Talk.HandleKey_Shift_ArrowUp(1);
					}
				}
				break;
			case KC_PERIOD:
				//聊天栏GM命令滚动
				if(s_pInputSystem->IsKeyDown(KC_LSHIFT) || s_pInputSystem->IsKeyDown(KC_RSHIFT))
				{
					if(CGameProcedure::s_pUISystem->IsIMEEditboxActive("Chat_Frame_EditBox"))
					{
						SCRIPT_SANDBOX::Talk::s_Talk.HandleKey_Shift_ArrowDown(1);
					}
				}
				break;
			case KC_ADD:
				break;
			case KC_SUBTRACT:
				break;
			case KC_SYSRQ:
				break;
			default:
				break;
			}
		}
		else if(event.nEvent == EVENT_KEY_UP)
		{
			switch(event.kIndex)
			{
			//主工具条快捷键
			case KC_F1:
			case KC_F2:
			case KC_F3:
			case KC_F4:
			case KC_F5:
			case KC_F6:
			case KC_F7:
			case KC_F8:
			case KC_F9:
			case KC_F10:
				CActionSystem::GetMe()->MainMenuBar_DoAction(event.kIndex-KC_F1, FALSE);
				break;
			default:
				break;
			}
		}
	}

}

//处理鼠标输入
VOID CGamePro_Main::ProcessInput_Mouse(VOID)
{
	static const INT	DRAG_MOVE_TIME_INTERVAL	= 500;

	//-----------------------------------------------------------
	//鼠标状态
	const UINT dwMouseFlag = s_pInputSystem->MouseGetFlag();
	const UINT dwMouseFlagOld = s_pInputSystem->MouseGetFlagOld();
	POINT pt = s_pInputSystem->MouseGetPos();
	POINT ptOld = s_pInputSystem->MouseGetPosOld();

	RECT rectClient;
	::GetClientRect(g_hMainWnd, &rectClient);
	//-----------------------------------------------------------
	//如果主窗口不激活或者鼠标不再客户区,返回
	if(!CGameProcedure::IsWindowActive() || !(((CInputSystem*)s_pInputSystem)->MouseIsInClient()))
	{
		return;
	}

	//-----------------------------------------------------------
	//取得鼠标信息
	INT nMousePosXDiff	= pt.x - ptOld.x;
	INT nMousePosYDiff	= pt.y - ptOld.y;
	BOOL bLBtnDown		= ( ( dwMouseFlag & MOUSE_LBDOWN) != FALSE );
	BOOL bRBtnDown		= ( ( dwMouseFlag & MOUSE_RBDOWN) != FALSE );
	BOOL bLBtnClick		= s_pInputSystem->IsEventExist(EVENT_MOUSE_LBCLICK);
	BOOL bRBtnClick		= s_pInputSystem->IsEventExist(EVENT_MOUSE_RBCLICK);

	//-----------------------------------------------------------
	//鼠标碰撞上的物体
	fVector3 fvMouseHitPlan;
	CObject* pSelectObj = ( CObject* )CObjectManager::GetMe()->GetMouseOverObject( pt.x, pt.y, fvMouseHitPlan );
	//鼠标是否在ui上空
	BOOL bInUIHover = s_pUISystem && s_pUISystem->IsMouseHover() && CInputSystem::GetMe()->GetCapture()!=ICS_GAME;

	//-----------------------------------------------------------
	//如果左右键都没有按下,计算鼠标命令
	HCURSOR LastCursor = NULL;

	if(!bLBtnClick && !bRBtnClick)
	{
		CCursorMng::GetMe()->MouseCommand_Set(bInUIHover, pSelectObj, fvMouseHitPlan, CActionSystem::GetMe()->GetDefaultAction());
		//设置光标类型
		const SCommand_Mouse& cmd = CCursorMng::GetMe()->MouseCommand_GetLeft();

		if(bInUIHover)
		{
			// 2006－3－29
			switch(cmd.m_typeMouse)
			{
			case SCommand_Mouse::MCT_REPAIR:		//修理
				CCursorMng::GetMe()->SetCursor( CURSOR_REPAIR);
				break;

			case SCommand_Mouse::MCT_UI_USE_IDENTIFY: //鉴定
				CCursorMng::GetMe()->SetCursor( CURSOR_IDENTIFY);
				break;

			default:
				CCursorMng::GetMe()->SetCursor( CURSOR_NORMAL );
				break;
			}
		}
		else
		{
			LastCursor = CCursorMng::GetMe()->GetCursor();
			switch(cmd.m_typeMouse)
			{
			case SCommand_Mouse::MCT_PLAYER_MOVETO:	//移动
				CCursorMng::GetMe()->SetCursor( CURSOR_NORMAL );
				break;

			case SCommand_Mouse::MCT_PLAYER_SELECT:	//选择目标
				CCursorMng::GetMe()->SetCursor( CURSOR_NORMAL );
				break;

			case SCommand_Mouse::MCT_SKILL_OBJ:		//目标攻击
				CCursorMng::GetMe()->SetCursor( CURSOR_ATTACK );
				break;

			case SCommand_Mouse::MCT_SKILL_AREA:	//区域攻击
				CCursorMng::GetMe()->SetCursor( CURSOR_ATTACK );
				break;

			case SCommand_Mouse::MCT_SKILL_DIR:		//方向型技能
				CCursorMng::GetMe()->SetCursor( CURSOR_ATTACK );
				break;

			case SCommand_Mouse::MCT_HIT_TRIPPEROBJ:	//TripperObj
				{
					CTripperObject* pTripperObj = (CTripperObject*)CObjectManager::GetMe()->FindServerObject(cmd.m_adwParam[0]);
					if(pTripperObj)
						CCursorMng::GetMe()->SetCursor( pTripperObj->Tripper_GetCursor() );
					else
						CCursorMng::GetMe()->SetCursor( CURSOR_NORMAL );
				}
				break;
			case SCommand_Mouse::MCT_SPEAK:			//谈话
				CCursorMng::GetMe()->SetCursor( CURSOR_SPEAK);
				break;

			case SCommand_Mouse::MCT_USE_ITEM:	//物品
				CCursorMng::GetMe()->SetCursor(CURSOR_HOVER);
				break;

			case SCommand_Mouse::MCT_REPAIR:		//修理
				CCursorMng::GetMe()->SetCursor( CURSOR_REPAIR);
				break;

			case SCommand_Mouse::MCT_UI_USE_IDENTIFY: //鉴定
				CCursorMng::GetMe()->SetCursor( CURSOR_IDENTIFY);
				break;

			default:
				CCursorMng::GetMe()->SetCursor( CURSOR_NORMAL );
				break;
			}

			//显示/隐藏技能范围指示器
			if(SCommand_Mouse::MCT_SKILL_AREA == cmd.m_typeMouse || 
			   (SCommand_Mouse::MCT_USE_ITEM == cmd.m_typeMouse &&
			     cmd.m_adwParam[4] == TRUE))
			{
				if(!m_pSkillArea)
				{
					m_pSkillArea = (CObject_ProjTex_AuraDure*)CObjectManager::GetMe()->NewObject("CObject_ProjTex_AuraDure");
					m_pSkillArea->Initial(NULL);
				}
				m_pSkillArea->SetShowEnable(TRUE);
				m_pSkillArea->SetPosition(fvMouseHitPlan);
			}
			else if(m_pSkillArea && m_pSkillArea->GetShowEnable())
			{
				m_pSkillArea->SetShowEnable(FALSE);
			}
		}

		//if(SCommand_Mouse::MCT_REPAIR == cmd.m_typeMouse)	//"修理"单独处理
		//	CCursorMng::GetMe()->SetCursor( CURSOR_REPAIR);

	}

	//-----------------------------------------------------------
	//分析事件队列
	InputEventQueue& inputQueue = s_pInputSystem->GetInputQueue();
	for(INT i=0; i<(INT)inputQueue.size(); i++)
	{
		InputEvent& event = inputQueue[i];
		if(event.bProcessed) continue;

		switch(event.nEvent)
		{
		//右键点击
		case EVENT_MOUSE_RBDOWN:
			{
				m_bCameraDraging = TRUE;
				event.bProcessed = true;
				//隐藏鼠标
				s_pInputSystem->SetCapture(ICS_GAME);
				CCursorMng::GetMe()->ShowCursor(FALSE);
				::SetCapture(g_hMainWnd);
			}
			break;

		//右键拖动
		case EVENT_MOUSE_RDRAG_MOVE:
			{
				//调节摄像机方向
				if(m_bCameraDraging)
				{
					FLOAT fSub = max( FLOAT( nMousePosXDiff /10.0f ), 1.0f) / 100;
					s_pGfxSystem->Camera_AddDirection( FLOAT( -nMousePosXDiff /300.0f )); 
					event.bProcessed = true;

					//鼠标归到原来位置
					s_pInputSystem->MouseSetPos(ptOld.x, ptOld.y);
					CCursorMng::GetMe()->ShowCursor(FALSE);
				}
			}
			break;

		//左键长时间按下
		case EVENT_MOUSE_LBDOWNREPEAT:
			{
				// 开始自动行走
				m_bAutoRun = TRUE;
				event.bProcessed = true;
			}
			break;

		//左键按下
		case EVENT_MOUSE_LBDOWN:
			{
				//取消自动行走
				m_bAutoRun = FALSE; 
				event.bProcessed = true;
			}
			break;

		//左键点击
		case EVENT_MOUSE_LBUP:
			{
				//激活鼠标命令
				if(s_pUISystem && s_pUISystem->IsDragFlag())
				{
					s_pUISystem->ChangeDragFlag(false);
				} 
				else
				{
					CCursorMng::GetMe()->MouseCommand_Active(CCursorMng::GetMe()->MouseCommand_GetLeft());
				}
				//CGameProcedure::s_pGfxSystem->PushDebugString("Active Left");

				event.bProcessed = true;
			}
			break;
		//右键抬起
		case EVENT_MOUSE_RBUP:
			{
				//显示鼠标
				CCursorMng::GetMe()->ShowCursor(true);
				//event.bProcessed = true;
				s_pInputSystem->SetCapture(ICS_NONE);
				::ReleaseCapture();
			}
			break;

		//右键点击
		case EVENT_MOUSE_RBCLICK:
			{
				//激活鼠标命令
				if( pSelectObj )
				{
					m_bAutoRun = FALSE; 
				}

				CCursorMng::GetMe()->MouseCommand_Active(CCursorMng::GetMe()->MouseCommand_GetRight());
				event.bProcessed = true;
			}
			break;
		//滚轮向上
		case EVENT_MOUSE_WHEEL_UP:
			{
				s_pGfxSystem->Camera_SetZoom( true, 2 );
				event.bProcessed = true;
			}
			break;

		//滚轮向下
		case EVENT_MOUSE_WHEEL_DOWN:
			{
				s_pGfxSystem->Camera_SetZoom( FALSE, 2 );
				event.bProcessed = true;
			}
			break;
		}
	}

	//-----------------------------------------------------------
	//如果右键已经松开
	if(!bRBtnDown)
	{
		m_bCameraDraging = FALSE;
		if(m_bAutoRun) // 自动行中... 
		{
			CCursorMng::GetMe()->SetCursor( CURSOR_AUTORUN );
			CGameProcedure::s_pGameInterface->Player_MoveTo( fVector2(fvMouseHitPlan.x,  fvMouseHitPlan.z) );
//			CCursorMng::GetMe()->SetCursor( CURSOR_AUTORUN );
		}
		else if( LastCursor != NULL && CCursorMng::GetMe()->GetCursor() != LastCursor )
		{
			CCursorMng::GetMe()->OnSetCursor();
		}
	}
}

VOID CGamePro_Main::SetMouseTargetProjTex(const fVector2&  fvPos)
{
	if(!m_pMouseTarget)
	{
		//创建
		m_pMouseTarget = (CObject_ProjTex_MouseTarget*)CObjectManager::GetMe()->NewObject("CObject_ProjTex_MouseTarget");
		m_pMouseTarget->Initial(NULL);
	}
	//判断目标是否不可走
	m_pMouseTarget->SetReachAble(!CPath::IsPointInUnreachRegion(fvPos));
	//设置位置
	m_pMouseTarget->SetMapPosition(fvPos);
}


VOID CGamePro_Main::CloseRequest(VOID)
{
	CActionSystem::GetMe()->SaveAction();
}

VOID CGamePro_Main::Release(VOID)
{
	m_bCameraDraging = FALSE;
	m_bAutoRun = FALSE;

	//释放MouseTarget
	if(m_pMouseTarget)
	{
		m_pMouseTarget->Destroy();
		m_pMouseTarget = NULL;
	}

	//释放SkillArea
	if(m_pSkillArea)
	{
		m_pSkillArea->Destroy();
		m_pSkillArea = NULL;
	}

	//保存摄像机距离
	fTransDistance	=	s_pGfxSystem->Camera_GetZoom();
}

LRESULT CGamePro_Main::MainWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return CGameProcedure::MainWndProc(hWnd, message, wParam, lParam);
}

VOID CGamePro_Main::OnSetCursor(VOID)
{

}
