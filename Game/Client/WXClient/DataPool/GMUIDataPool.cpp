#include "StdAfx.h"
#include "..\Object\Character\AI\GMAI_MySelf.h"
#include "TDGameInterface.h"
#include "TDTimeSystem.h"
#include "../Object/Item/Obj_Item.h"
#include "../Object/Item/Obj_Item_Equip.h"
#include "../Network/NetManager.h"
#include "CGAskItemInfo.h"
#include "..\GameCommand.h"
#include "DataPoolCommandDef.h"
#include "TDException.h"
#include "TDVariable.h"
#include "..\Event\GMEventSystem.h"
#include "ScriptCommandStruct.h"
#include "CGEventRequest.h"
#include "CGMissionRefuse.h"
#include "CGMissionAccept.h"
#include "CGMissionContinue.h"
#include "CGMissionSubmit.h"
#include "CGMissionCheck.h"
#include "CGMissionAbandon.h"
#include "CGAskMissionDesc.h"
#include "TDUISystem.h"
#include "..\DataPool\GMDataPool.h"
#include "..\Procedure\GameProcedure.h"

#include "..\Sound\GMSoundSystem.h"
#include "..\NetWork\NetManager.h"
#include "GMUIDataPool.h"
#include "..\Action\GMActionSystem.h"
#include "TeamOrGroup.h"
#include "tddatabase.h"
#include "..\World\WorldManager.h"
#include "CGAskTeamMemberInfo.h"
#include "GCRetMissionDesc.h"

#include "CGTeamKick.h"								// 踢出一个队员.
#include "CGTeamDismiss.h"							// 解散队伍消息.
#include "CGTeamLeave.h"							// 离开队伍消息.
#include "CGTeamAppoint.h"							// 任命队长消息
#include "..\Object\ObjectManager.h"				// 得到物体管理器.
#include "..\Object\Character\Obj_PlayerMySelf.h"

#include "CGAskJoinMenpai.h"
#include "CGAskStudyXinfa.h"
#include "GCMissionModify.h"

#include "GMDP_CharacterData.h"
#include "GMDP_Struct_Quest.h"
#include "..\Procedure\GameProcedure.h"
#include "..\DBC\GMDataBase.h"
#include "..\WxRender\RenderSystem.h"

WX_IMPLEMENT_DYNAMIC(CUIDataPool, GETCLASS(tDataPool));
CUIDataPool* CUIDataPool::s_pMe = NULL;
CUIDataPool::CUIDataPool()
{
	s_pMe = this;

	//---------------------------------------------------------------------------------------------------------------------
	// 队伍 
	//

	// 当前拒绝队伍的索引.
	m_iCurRejectTeamIndex = -1;

	m_bOutGhostTimerWorking	= FALSE;
	m_nOutGhostTimer		= 0;
	m_nCurDialogNpcId		= -1;
	m_nCurShopNpcId			= -1;
}

CUIDataPool::~CUIDataPool()
{
	Release();
}

VOID CUIDataPool::Initial( VOID* )
{
	m_pPlayerData		= new CDetailAttrib_Player;
	m_pPlayerData->Init();

	m_pEventList			= new ScriptParam_EventList;
	m_pMissionInfo			= new ScriptParam_MissionInfo;
	m_pMissionDemandInfo	= new ScriptParam_MissionDemandInfo;
	m_pMissionContinueInfo	= new ScriptParam_MissionContinueInfo;
	m_pMissionTips			= new ScriptParam_MissionTips;
	m_pSkillStudy			= new ScriptParam_SkillStudy;				
}

VOID CUIDataPool::Release( VOID )
{
	m_bOutGhostTimerWorking	= FALSE;
	m_nOutGhostTimer		= 0;
	delete m_pSkillStudy;			m_pSkillStudy = NULL;
	delete m_pMissionTips;			m_pMissionTips = NULL;
	delete m_pMissionContinueInfo;	m_pMissionContinueInfo = NULL;
	delete m_pMissionDemandInfo;	m_pMissionDemandInfo = NULL;
	delete m_pMissionInfo;			m_pMissionInfo = NULL;
	delete m_pEventList;			m_pEventList = NULL;

	if ( m_pPlayerData != NULL )
	{
		m_pPlayerData->Term();
		delete m_pPlayerData;
		m_pPlayerData = NULL;
	}

	ClearTeamInfo();

//	ItemBox_Clear();
//	UserEquip_Clear();
//	UserBag_Clear();
}

VOID CUIDataPool::Tick( VOID )
{
	if ( m_pPlayerData != NULL )
		m_pPlayerData->Tick();

//	BOOL bResult;
//	if ( m_pMySelfAI != NULL && m_idMySelf != INVALID_ID )
//	{
//		//bResult = m_pMySelfAI->Logic();
////		if ( !bResult )
////			return ;
//	}
	if ( m_bOutGhostTimerWorking )
	{
		INT nTempOutGhostTimer = m_nOutGhostTimer - (INT)(CGameProcedure::s_pTimeSystem->GetDeltaTime());
		if ( nTempOutGhostTimer < 0 )
			nTempOutGhostTimer = 0;

		if ( m_nOutGhostTimer != nTempOutGhostTimer )
		{
			// 精确到秒的级别
			INT nSOld = m_nOutGhostTimer/1000;
			INT nSNew = nTempOutGhostTimer/1000;
			m_nOutGhostTimer = nTempOutGhostTimer;
		}
	}
	return ;
}

RC_RESULT CUIDataPool::OnCommand_( const SCommand_DPC *pCmd )
{
	RC_RESULT rcResult = RC_SKIP;

	BOOL bLogValid = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;
	switch ( pCmd->m_wID )
	{
	case DPC_SCRIPT_COMMAND:
		{
			INT nCmdID	= pCmd->m_anParam[0];
			VOID *pBuf	= pCmd->m_apParam[1];
			switch ( nCmdID )
			{
			case SCRIPT_COMMAND_EVENT_LIST_RESPONSE: // 事件列表返回
				*m_pEventList = *((ScriptParam_EventList*)(pBuf));
				OnEventListResponse();
				break;

				//打开任务信息
			case SCRIPT_COMMAND_MISSION_RESPONSE: // 任务事件的查询返回
				*m_pMissionInfo = *((ScriptParam_MissionInfo*)(pBuf));
				OnMissionInfoResponse();
				break;

			case SCRIPT_COMMAND_MISSION_REGIE: // 漕运任务查询返回
				*m_pEventList = *((ScriptParam_EventList*)(pBuf));
				OnMissionRegie();
				break;

				//任务需求信息（完成任务的文字描述，完成需要的物品，奖励的物品）
			case SCRIPT_COMMAND_MISSION_DEMAND_RESPONSE: // 任务需求的查询返回
				*m_pMissionDemandInfo = *((ScriptParam_MissionDemandInfo*)(pBuf));
				OnMissionDemandInfoResponse();
				break;

				//任务继续信息（包含了奖励信息，在点击continue之后再去显示）
			case SCRIPT_COMMAND_MISSION_CONTINUE_RESPONSE: // 任务的继续按钮事件返回
				*m_pMissionContinueInfo = *((ScriptParam_MissionContinueInfo*)(pBuf));
				OnMissionContinueInfoResponse();
				break;

				//任务完成情况提示信息
			case SCRIPT_COMMAND_MISSION_TIPS: // 任务提示
				*m_pMissionTips = *((ScriptParam_MissionTips*)(pBuf));
				OnMissionTips();
				break;

				//技能学习信息
			case SCRIPT_COMMAND_SKILL_STUDY: // 技能信息
				m_pSkillStudy->Reset();
				*m_pSkillStudy = *((ScriptParam_SkillStudy*)(pBuf));
				OnSkillStudy();
				break;
			default:
				break;
			}
		}
		break;
	case DPC_UPDATE_MISSION_LIST://名字似乎不再合适了
		{
			UINT dwObjID				= pCmd->m_adwParam[0];
			UINT dwModifyFlags			= pCmd->m_adwParam[1];
			_OWN_MISSION *paMissionBuf	= (_OWN_MISSION *)(pCmd->m_apParam[2]);

			UINT i;
			CDetailAttrib_Player* playData = const_cast<CDetailAttrib_Player*>(CUIDataPool::GetMe()->GetMySelfDetailAttrib());
			_OWN_MISSION *pMission;
			for ( i = 0; i < MAX_CHAR_MISSION_NUM; i++ )
			{
				if ( dwModifyFlags & (0x00000001 << i) )
				{
					pMission = &(playData->m_listMission.m_aMission[i]);
					memcpy( (void*)pMission, &paMissionBuf[i], sizeof(_OWN_MISSION) );
					CGAskMissionDesc msg;
					msg.setMissionIndex( pMission->m_idMission );
					CNetManager::GetMe()->SendPacket( &msg );
				}
			}
		}
		break;
	case DPC_UPDATE_MISSION_ADD:
		{
			_OWN_MISSION *pMission	= (_OWN_MISSION *)(pCmd->m_apParam[0]);
			m_pPlayerData->AddMission( pMission );

			CGAskMissionDesc msg;
			msg.setMissionIndex( pMission->m_idMission );
			CNetManager::GetMe()->SendPacket( &msg );
			rcResult = RC_OK;
		}
		break;
	case DPC_UPDATE_MISSION_MODIFY:
		{
			if(pCmd->m_anParam[1] == GCMissionModify::MISSIONMODIFY_MISSION)
			{
				_OWN_MISSION *pMission	= (_OWN_MISSION *)(pCmd->m_apParam[0]);
				m_pPlayerData->ModifyMission( pMission );

				CGAskMissionDesc msg;
				msg.setMissionIndex( pMission->m_idMission );
				CNetManager::GetMe()->SendPacket( &msg );
			}
			else if(pCmd->m_anParam[1] == GCMissionModify::MISSIONMODIFY_MISSIONDATA)
			{
				m_pPlayerData->ModifyMissionData((INT*)pCmd->m_apParam[0]);
			}

			rcResult = RC_OK;
		}
		break;
	case DPC_UPDATE_MISSION_REMOVE:
		{
			MissionID_t idMission = (MissionID_t)(pCmd->m_adwParam[0]);
			m_pPlayerData->RemoveMission( idMission );
			rcResult = RC_OK;
		}
		break;
	case DPC_UPDATE_CAN_PICK_MISSION_ITEM_LIST:
		{
			UINT dwItemCount	= pCmd->m_adwParam[0];
			UINT *paItemList	= (UINT*)(pCmd->m_apParam[1]);
			CDetailAttrib_Base	*pAttribData = m_pPlayerData;
			pAttribData->UpdateCanPickMissionItemList( dwItemCount, paItemList );
			rcResult = RC_OK;
		}
		break;
	case DPC_ADD_CAN_PICK_MISSION_ITEM:
		{
			UINT dwItemDataID	= pCmd->m_adwParam[0];
			CDetailAttrib_Base	*pAttribData = m_pPlayerData;
			pAttribData->AddCanPickMissionItem( dwItemDataID );
			rcResult = RC_OK;
		}
		break;
	case DPC_REMOVE_CAN_PICK_MISSION_ITEM:
		{
			UINT dwItemDataID	= pCmd->m_adwParam[0];
			CDetailAttrib_Base	*pAttribData = m_pPlayerData;
			pAttribData->RemoveCanPickMissionItem( dwItemDataID );
			rcResult = RC_OK;
		}
		break;
/*
	case DPC_UPDATE_COOL_DOWN_LIST:
		{
			UINT dwUpdateNum	= pCmd->m_adwParam[0];
			const UINT *pdwSkillCoolDownList	= (const UINT*)(pCmd->m_apConstParam[1]);
			CDetailAttrib_Base	*pAttribData = m_pPlayerData;
			pAttribData->UpdateCoolDownList( pdwSkillCoolDownList, dwUpdateNum );
			rcResult = RC_OK;
		}
		break;
*/
	case DPC_UPDATE_TEAM_OR_GROUP:
		{
			UINT res = pCmd->m_adwParam[0];
			CTeamOrGroup* pTeamOrGroup = GetTeamOrGroup();
			INT MyServerID = CObjectManager::GetMe()->GetMySelf()->GetServerID();
			STRING strTemp = "";

			// 如果得到队伍信息出现异常, 保存并且返回.
			if(NULL == pTeamOrGroup)
			{
				Assert(pTeamOrGroup);
				return RC_ERROR;
			}

			TeamMemberInfo tmInfo;
			/*
			[0]	= Return
			[1]	= GUID
			[2]	= TeamID
			[3]	= GUIDEx
			[4]	= SceneID
			*/
			switch( res )
			{
			case TEAM_RESULT_TEAMREFRESH:
			case TEAM_RESULT_MEMBERENTERTEAM:
				{
					//	MessageBox(NULL, "有新队员加入！", "BEEP(got it))", MB_OK|MB_ICONHAND);
					tmInfo.m_GUID = pCmd->m_adwParam[1];
					tmInfo.m_OjbID = pCmd->m_adwParam[3];
					tmInfo.m_SceneID = pCmd->m_adwParam[4];
					tmInfo.m_nPortrait = pCmd->m_adwParam[5];
					tmInfo.m_uDataID = pCmd->m_adwParam[6];
					strncpy(tmInfo.m_szNick, (char*)&pCmd->m_apParam[7], MAX_CHARACTER_NAME);
					// ...

					pTeamOrGroup->AddMember(&tmInfo, pCmd->m_adwParam[2]);

					//CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "有队员加入队伍========");
					INT iUiIndex =  pTeamOrGroup->GetMemberUIIndex(tmInfo.m_GUID);
					if(INVALID_ID != iUiIndex)
					{
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_MEMBER_ENTER, iUiIndex);
						if(pTeamOrGroup->GetLeaderGUID() == CObjectManager::GetMe()->GetMySelf()->GetServerGUID())
						{
							CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_NOTIFY_APPLY, 3 );
						}
						else
						{
							CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_NOTIFY_APPLY, 0 );
						}

						// 刷新左边的队伍窗口界面。
						// CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_REFRESH_UI );
						
					}

					if ( res == TEAM_RESULT_MEMBERENTERTEAM )
					{
						if ( tmInfo.m_OjbID != MyServerID )
						{ // 别人加入了队伍
							if(bLogValid)
							{
								strTemp = NOCOLORMSGFUNC("TEAM_JOIN_OTHER", tmInfo.m_szNick);
								ADDNEWDEBUGMSG(strTemp);
								ADDTALKMSG(strTemp);
							}

							
							// 刷新左边的队伍窗口界面。
							CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_REFRESH_UI );
							CSoundSystemFMod::_PlayUISoundFunc(62);
						}
						else
						{
							if( tmInfo.m_GUID != pTeamOrGroup->GetLeaderGUID() )
							{ // 加入了别人的队伍
								char* pLeaderName = pTeamOrGroup->GetMember(pTeamOrGroup->GetLeaderGUID())->m_szNick;

								//msg
								if(bLogValid)
								{
									strTemp = NOCOLORMSGFUNC("TEAM_JOIN", pLeaderName);
									ADDNEWDEBUGMSG(strTemp);
									ADDTALKMSG(strTemp);
								}
								// 刷新左边的队伍窗口界面。
								CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_REFRESH_UI );
								CSoundSystemFMod::_PlayUISoundFunc(62);
							}
							else
							{
								if(bLogValid)
								{
									strTemp = NOCOLORMSGFUNC("TEAM_CREATE_OK");
									ADDNEWDEBUGMSG(strTemp);
								}
								CSoundSystemFMod::_PlayUISoundFunc(62);
				
							}

							pTeamOrGroup->ClearInviteTeam();
							pTeamOrGroup->ClearProposer();
						}

						if(IsTeamLeader_Me())
						{
							CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_NOTIFY_APPLY, 3 );
						}
						else
						{
							CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_NOTIFY_APPLY, 0 );
						}

						//  参数4表示, 界面打开就刷新, 界面关闭不刷新.
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_OPEN_TEAMINFO_DLG, 4 );
						ClearAllInviteTeam();
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_MESSAGE, strTemp.c_str() );

						// 刷新左边的队伍窗口界面。2006-4-15
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_REFRESH_UI );
					}
				}
				break;
			case TEAM_RESULT_MEMBERLEAVETEAM:
				{
					TeamMemberInfo* pTMInfo;
					pTMInfo = pTeamOrGroup->GetMember( pCmd->m_adwParam[1] );

					if ( pTMInfo->m_OjbID != MyServerID )
					{
						//msg
						if(bLogValid)
						{
							strTemp = NOCOLORMSGFUNC("TEAM_LEAVE_OTHER", pTMInfo->m_szNick);
							ADDNEWDEBUGMSG(strTemp);
							ADDTALKMSG(strTemp);
						}

						pTeamOrGroup->DelMember( pTMInfo->m_GUID );
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_REFRESH_UI );
						//  参数4表示, 界面打开就刷新, 界面关闭不刷新.
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_OPEN_TEAMINFO_DLG, 4 );
					}
					else
					{
						//msg
						if(bLogValid)
						{
							strTemp = NOCOLORMSGFUNC("TEAM_LEAVE_MYSELF");
							ADDNEWDEBUGMSG(strTemp);
							ADDTALKMSG(strTemp);
						}

						pTeamOrGroup->DismissTeam();
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_NOTIFY_APPLY, 4 );
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_CLEAR_UI );
						// 参数大于2就行.
						//CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_OPEN_TEAMINFO_DLG, 4 );
						SCRIPT_SANDBOX::Talk::s_Talk.TeamDestory();
					}

					CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_MESSAGE, strTemp.c_str() );
					CSoundSystemFMod::_PlayUISoundFunc(61);
				}
				break ;
			case TEAM_RESULT_LEADERLEAVETEAM:
				{
					TeamMemberInfo* pTMInfo;
					pTMInfo = pTeamOrGroup->GetMember( pCmd->m_adwParam[1] );

					if ( pTMInfo->m_OjbID != MyServerID )
					{
						//msg
						if(bLogValid)
						{
							strTemp = NOCOLORMSGFUNC("TEAM_LEAVE_OTHER", pTMInfo->m_szNick);
							ADDNEWDEBUGMSG(strTemp);
							ADDTALKMSG(strTemp);
						}
						pTeamOrGroup->SetLeaderGUID( pCmd->m_adwParam[3] );
						pTeamOrGroup->DelMember( pTMInfo->m_GUID );
						
						if(IsTeamLeader_Me())
						{
							// 如果自己变成队长不刷新界面.打开队长界面.
							CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_NOTIFY_APPLY, 3 );

							//  参数4表示, 界面打开就刷新, 界面关闭不刷新.
							CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_OPEN_TEAMINFO_DLG, 4 );
							
						}
						else
						{
							// 如果自己变成队长不刷新界面.打开队长界面.
							CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_NOTIFY_APPLY, 0 );

							// 刷新队员信息.
							CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_OPEN_TEAMINFO_DLG, 4);
						}
					}
					else
					{
						//msg
						if(bLogValid)
						{
							strTemp = NOCOLORMSGFUNC("TEAM_LEAVE_MYSELF");
							ADDNEWDEBUGMSG(strTemp);
							ADDTALKMSG(strTemp);
						}

						pTeamOrGroup->DismissTeam();
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_NOTIFY_APPLY, 4 );
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_CLEAR_UI );
						SCRIPT_SANDBOX::Talk::s_Talk.TeamDestory();

						// 清空邀请者
						pTeamOrGroup->ClearInviteTeam();

						// 清空申请者
						pTeamOrGroup->ClearProposer();
						
					}

					// 刷新左边的队伍窗口界面。2006-4-15
					CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_REFRESH_UI );

					CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_MESSAGE, strTemp.c_str() );
					CSoundSystemFMod::_PlayUISoundFunc(61);
				}
				break ;
			case TEAM_RESULT_TEAMDISMISS:
				{
					pTeamOrGroup->DismissTeam();
					strTemp = NOCOLORMSGFUNC("TEAM_DISMISS");

					// 删除界面.
					CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_CLEAR_UI);
					CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_NOTIFY_APPLY, 4 );

					CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_MESSAGE, strTemp.c_str() );
					CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_OPEN_TEAMINFO_DLG, -1);
					
					//msg
					if(bLogValid)
					{
						ADDNEWDEBUGMSG(strTemp);
						ADDTALKMSG(strTemp);
					}

					// 清空邀请者
					pTeamOrGroup->ClearInviteTeam();

					// 清空申请者
					pTeamOrGroup->ClearProposer();

					CSoundSystemFMod::_PlayUISoundFunc(61);
					SCRIPT_SANDBOX::Talk::s_Talk.TeamDestory();



				}
				break ;
			case TEAM_RESULT_TEAMKICK:
				{
					TeamMemberInfo* pTMInfo;
					pTMInfo = pTeamOrGroup->GetMember( pCmd->m_adwParam[1] );

					if ( pTMInfo->m_OjbID != MyServerID )
					{
						//msg
						if(bLogValid)
						{
							strTemp = NOCOLORMSGFUNC("TEAM_LEAVE_FORCE", pTMInfo->m_szNick);
							ADDNEWDEBUGMSG(strTemp);
							ADDTALKMSG(strTemp);
						}
						pTeamOrGroup->DelMember( pTMInfo->m_GUID );
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_REFRESH_UI );

						//  参数4表示, 界面打开就刷新, 界面关闭不刷新.
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_OPEN_TEAMINFO_DLG, 4 );
					}
					else
					{
						//msg
						if(bLogValid)
						{
							strTemp = "你被请离了队伍";//NOCOLORMSGFUNC("team_leave_force");
							ADDNEWDEBUGMSG(strTemp);
							ADDTALKMSG(strTemp);
						}
						pTeamOrGroup->DismissTeam();
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_CLEAR_UI);
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_NOTIFY_APPLY, 4 );
                        CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_OPEN_TEAMINFO_DLG, -1);
						SCRIPT_SANDBOX::Talk::s_Talk.TeamDestory();
						
					}

					CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_MESSAGE, strTemp.c_str());
					CSoundSystemFMod::_PlayUISoundFunc(61);

				}
				break ;
			case TEAM_RESULT_TEAMAPPOINT:
				{
					TeamMemberInfo* pTMInfo;
					INT NewLeaderGUID;

					NewLeaderGUID = pCmd->m_adwParam[3];

					pTeamOrGroup->Appoint( NewLeaderGUID );

					pTMInfo = pTeamOrGroup->GetMember( NewLeaderGUID );

					if ( pTMInfo->m_OjbID != MyServerID )
					{
						//msg
						if(bLogValid)
						{
							strTemp = NOCOLORMSGFUNC("TEAM_LEADER_CHANGE", pTMInfo->m_szNick);
							ADDNEWDEBUGMSG(strTemp);
							ADDTALKMSG(strTemp);
						}
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_NOTIFY_APPLY, 0 );
						// 参数4表示, 界面打开就刷新, 界面关闭不刷新.
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_OPEN_TEAMINFO_DLG, 4 );
					}
					else
					{
						//msg
						if(bLogValid)
						{
							strTemp = "你被任命为队伍的队长了";//NOCOLORMSGFUNC("team_leader_change_myself");
							ADDNEWDEBUGMSG(strTemp);
							ADDTALKMSG(strTemp);
						}
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_NOTIFY_APPLY, 3 );
						// 参数4表示, 界面打开就刷新, 界面关闭不刷新.
						CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_OPEN_TEAMINFO_DLG, 4 );
					}

					CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_REFRESH_UI );
					CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_MESSAGE, strTemp.c_str() );
					CSoundSystemFMod::_PlayUISoundFunc(61);
				}
				break ;
			case TEAM_RESULT_STARTCHANGESCENE:
				{ // 队友切换场景，改变头像状态
					TeamMemberInfo* pTMInfo;

					pTMInfo = pTeamOrGroup->GetMember( pCmd->m_adwParam[1] );

					if ( pTMInfo == NULL )
					{ // 这种情况可能会在组队跟随的切换场景过程中出现
						// Assert( FALSE && "Team info is not found when a team member start change scene(ignore this)." );
						return RC_OK;
					}

					pTMInfo->m_SceneID = pCmd->m_adwParam[4];

					if ( MyServerID == pTMInfo->m_OjbID )
					{
						break;
					}

					CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_UPTEDATA_MEMBER_INFO,
						pTeamOrGroup->GetMemberUIIndex( pTMInfo->m_GUID ) );
				//	CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "开始切换场景。" );
				}
				break;
			case TEAM_RESULT_ENTERSCENE:
				{ // 有队友进入新场景，更新场景信息，如果和自己相同场景，则查询他的信息
					TeamMemberInfo* pTMInfo;

					pTMInfo = pTeamOrGroup->GetMember( pCmd->m_adwParam[1] );

					if ( pTMInfo == NULL )
					{ // 这种情况可能会在组队跟随的切换场景过程中出现
						// Assert( FALSE && "Team info is not found when a team member enter scene(ignore this)." );
						return RC_OK;
					}

					if ( MyServerID == pTMInfo->m_OjbID )
					{ // 自己进入新场景，不管，因为会自动获取队友信息
						break;
					}

					pTMInfo->m_SceneID = pCmd->m_adwParam[4];
					pTMInfo->m_OjbID = pCmd->m_adwParam[3];
					pTMInfo->m_bDeadLink = FALSE; // 进入场景必然非断线状态，免去对队友是否断线重连的判断

					//if( CGameProcedure::s_pVariableSystem->GetAs_Int("Scene_ID") == pTMInfo->m_SceneID )
					//{
					CGAskTeamMemberInfo tmMsg;
					tmMsg.SetSceneID( pTMInfo->m_SceneID );
					tmMsg.SetGUID( pTMInfo->m_GUID );
					tmMsg.setObjID( pTMInfo->m_OjbID );

					CNetManager::GetMe()->SendPacket( &tmMsg );
					//}

					CGameProcedure::s_pEventSystem->PushEvent( GE_TEAM_UPTEDATA_MEMBER_INFO,
						pTeamOrGroup->GetMemberUIIndex( pTMInfo->m_GUID ) );

				}
				break;
			default :
				{
					Assert(FALSE) ;
				}
				break ;
			}

			rcResult = RC_OK;
		}
		break;
/*
	case DPC_UI_COMMAND:
		{
			INT nUIIndex	= pCmd->m_anParam[0];
			VOID *pBuf	= pCmd->m_apParam[1];
			*pX_PARAM = *((X_PARAM*)(pBuf)); 

			CEventSystem::GetMe()->PushEvent(GE_UI_COMMAND, nUIIndex);
		}
		break;
*/
	case DPC_UPDATE_MISSION_DESC:
		{
			GCRetMissionDesc *pPacket	= (GCRetMissionDesc *)(pCmd->m_apParam[0]);
			const _OWN_MISSION * OwnMission = CUIDataPool::GetMe()->GetMySelfDetailAttrib()->GetMission(pPacket->GetMissionIndex());
			if( OwnMission )
			{
				_MISSION_INFO* misInfo = &(GetMissionInfo(OwnMission->m_idScript));
				if( pPacket->GetMissionNameLen() )
				{
					misInfo->m_misName = std::string( pPacket->GetMissionName(), pPacket->GetMissionNameLen() );
					ConvertServerString( misInfo->m_misName );
				}
				if( pPacket->GetMissionDescLen() )
				{
					misInfo->m_misDesc = std::string( pPacket->GetMissionDesc(), pPacket->GetMissionDescLen() );
					ConvertServerString( misInfo->m_misDesc );
				}
				if( pPacket->GetMissionTargetLen() )
				{
					//misInfo->tar
				}
			}
		}
		break;
	default:
		rcResult = RC_SKIP;
		break;
	}
	return rcResult;
}

VOID CUIDataPool::OnEventListResponse( VOID )
{
	tObject* pNPC = CObjectManager::GetMe()->FindServerObject(m_pEventList->m_idNPC);

	if(pNPC)
	{
		SetCurDialogNpcId( pNPC->GetServerID() );
	}
	CEventSystem::GetMe()->PushEvent(GE_QUEST_EVENTLIST, pNPC ? pNPC->GetID() : -1);
}

//向服务器发送从列表中选取的任务	
//nExIndex2附加信息
VOID CUIDataPool::SendSelectEvent(INT nIndex,INT nExIndex1,INT nExIndex2)
{
	const ScriptEventItem *pItem;

	BYTE i, yNum;
	INT nSelectIndex;
	yNum = 0;
	nSelectIndex = -1;
	for ( i = 0; i < m_pEventList->m_yItemCount; i++ )
	{
		if (( m_pEventList->GetItem( i )->m_index == nExIndex2) && (m_pEventList->GetItem( i )->m_idScript == nExIndex1 ))
		{
			pItem = m_pEventList->GetItem( i );

			CGEventRequest msg;
			msg.setExIndex( pItem->m_index );
			msg.setScriptID( pItem->m_idScript );
			msg.setNPCID( m_pEventList->m_idNPC );
			CNetManager::GetMe()->SendPacket( &msg );

//			CGameProcedure::s_pGfxSystem->PushDebugString("CGEventRequest");

			break;
		}

		/*if ( m_pEventList->GetItem( i )->m_nType != EVENT_ITEM_TYPE_TEXT )
		{
			if ( (BYTE)nIndex == yNum )
			{
				nSelectIndex = (INT)i;
				break;
			}
			yNum++;
		}*/
	}

	/*if ( nSelectIndex >= 0 )
	{
		pItem = m_pEventList->GetItem( (BYTE)nSelectIndex );

		CGEventRequest msg;
		msg.setExIndex( pItem->m_index );
		msg.setScriptID( pItem->m_idScript );
		msg.setNPCID( m_pEventList->m_idNPC );
		CNetManager::GetMe()->SendPacket( &msg );

		CGameProcedure::s_pGfxSystem->PushDebugString("CGEventRequest");
	}*/
}

//向服务器返回接受任务
VOID CUIDataPool::SendAcceptEvent()
{
	CGMissionAccept msg;
	msg.setScriptID( m_pMissionInfo->m_idScript );
	msg.setNPCID( m_pMissionInfo->m_idNPC );
	CNetManager::GetMe()->SendPacket( &msg );

	CGameProcedure::s_pGfxSystem->PushDebugString("CGMissionAccept");
}

//向服务器返回拒绝任务
VOID CUIDataPool::SendRefuseEvent()
{
	CGMissionRefuse msg;
	msg.setScriptID( m_pMissionInfo->m_idScript );
	msg.setNPCID( m_pMissionInfo->m_idNPC );
	CNetManager::GetMe()->SendPacket( &msg );

	CGameProcedure::s_pGfxSystem->PushDebugString("CGMissionRefuse");
}

//向服务器返回继续任务
VOID CUIDataPool::SendContinueEvent()
{
	CGMissionContinue msg;
	msg.setScriptID( m_pMissionDemandInfo->m_idScript );
	msg.setNPCID( m_pMissionDemandInfo->m_idNPC );
	CNetManager::GetMe()->SendPacket( &msg );

	CGameProcedure::s_pGfxSystem->PushDebugString("CGMissionContinue");
}

//向服务器返回放弃任务
VOID CUIDataPool::SendAbandonEvent( INT nIndex )
{
	CGMissionAbandon msg;
	msg.setScriptID((ScriptID_t)nIndex);
	CNetManager::GetMe()->SendPacket( &msg );//

	CGameProcedure::s_pGfxSystem->PushDebugString("CGMissionAbandon");
}

VOID CUIDataPool::SendMissionItemInfo(INT nPetIndex)
{
	CGMissionCheck msg;

	//使用数据池中的 m_MissionItemList 来查找自己的编号
	INT nItemIndex[3];
	for(int i=0;i<3;i++)
	{
		if(CDataPool::GetMe()->MissionBox_GetItem(i))
			nItemIndex[i] = CDataPool::GetMe()->UserBag_GetItemIndexByGUID(CDataPool::GetMe()->MissionBox_GetItem(i)->GetGUID());
		else
			nItemIndex[i] = 255;
	}

	msg.setPetIndex(nPetIndex);
	msg.setItemIndexList((BYTE*)nItemIndex);
	msg.setScriptID( m_pMissionDemandInfo->m_idScript );
	msg.setNPCID( m_pMissionDemandInfo->m_idNPC );

	CNetManager::GetMe()->SendPacket(&msg);
}

//向服务器返回完成任务
VOID CUIDataPool::SendCompleteEvent( INT nIndex )
{
	CGMissionSubmit msg;
	msg.setScriptID( m_pMissionContinueInfo->m_idScript );
	msg.setNPCID( m_pMissionContinueInfo->m_idNPC );
	//需要传入物品ID
	msg.setSelectRadioID( m_pMissionContinueInfo->GetSelectItemID(nIndex) );
	CNetManager::GetMe()->SendPacket( &msg );

	CGameProcedure::s_pGfxSystem->PushDebugString("CGMissionSubmit");
}

VOID CUIDataPool::OnMissionInfoResponse( VOID )
{
	//清空原有数据
	QuestReward_Clear();

	//生成奖励物品信息
	INT nItemCount = m_pMissionInfo->m_yBonusCount;
	for(INT i=0; i<nItemCount; i++)
	{
		SMissionBonus* pRewardItem = &(m_pMissionInfo->m_aBonus[i]);

		if(	pRewardItem->m_nType == MISSION_BONUS_TYPE_ITEM ||
			pRewardItem->m_nType == MISSION_BONUS_TYPE_ITEM_RADIO)
		{
			tObject_Item* pItem = CObject_Item::NewItem(pRewardItem->m_ItemBonus.m_uItemID);
			if(!pItem)
			{
				TDThrow("Invalid reward item id: %d", pRewardItem->m_ItemBonus.m_uItemID);
			}
			
			//增加到数据池中
			QuestRewardItem newRewardItem;
			newRewardItem.pItemData = pRewardItem;
			newRewardItem.pItemImpl = pItem;
			newRewardItem.bSelected = FALSE;
			QuestReward_AddItem(newRewardItem);
		}
		else
		{
			//增加到数据池中
			QuestRewardItem newRewardItem;
			newRewardItem.pItemData = pRewardItem;
			newRewardItem.pItemImpl = 0;
			newRewardItem.bSelected = FALSE;
			QuestReward_AddItem(newRewardItem);
		}
	}
	for( INT i = 0; i < m_pMissionInfo->m_yTextCount; i ++ )
	{
		std::string strText = m_pMissionInfo->m_aText[i].m_szString;
		if( ConvertServerString( strText ) )
			m_pMissionInfo->m_aText[i] = strText.c_str();
	}

	//通知UI
	tObject* pNPC = CObjectManager::GetMe()->FindServerObject(m_pMissionInfo->m_idNPC);
	if(pNPC)
	{
		SetCurDialogNpcId( pNPC->GetServerID() );
	}
	CEventSystem::GetMe()->PushEvent(GE_QUEST_INFO, pNPC ? pNPC->GetID(): -1, m_pMissionInfo->m_IsPush);
}

VOID CUIDataPool::OnMissionRegie( VOID )
{
	tObject* pNPC = CObjectManager::GetMe()->FindServerObject(m_pEventList->m_idNPC);

	CEventSystem::GetMe()->PushEvent(GE_QUEST_REGIE, pNPC ? pNPC->GetID() : -1);
}

VOID CUIDataPool::OnMissionDemandInfoResponse( VOID )
{
	//清空原有数据
	QuestDemand_Clear();
	//生成需求物品信息
	INT nItemCount = m_pMissionDemandInfo->m_yDemandCount;
	for(INT i=0; i<nItemCount; i++)
	{
		tObject_Item* pItem = CObject_Item::NewItem(m_pMissionDemandInfo->m_aDemandItem[i].m_uItemID);//->m_itemBonus.m_dwItemID);
		
		//增加到数据池中
		QuestDemandItem newDemandItem;
		newDemandItem.pDemandItem = &(m_pMissionDemandInfo->m_aDemandItem[i]);
		newDemandItem.pItemImpl = pItem;
		QuestDemand_AddItem(newDemandItem);
	}
	for( INT i = 0; i < m_pMissionInfo->m_yTextCount; i ++ )
	{
		std::string strText = m_pMissionInfo->m_aText[i].m_szString;
		if( ConvertServerString( strText ) )
			m_pMissionInfo->m_aText[i] = strText.c_str();
	}

	//通知UI
	tObject* pNPC = CObjectManager::GetMe()->FindServerObject(m_pMissionDemandInfo->m_idNPC);
	if ( m_pMissionDemandInfo->m_bDone == ScriptParam_MissionDemandInfo::MISSION_DONE )
	{
		//任务已经完成
		if(pNPC)
		{
			SetCurDialogNpcId( pNPC->GetServerID() );
		}
		CEventSystem::GetMe()->PushEvent(GE_QUEST_CONTINUE_DONE, pNPC ? pNPC->GetID(): -1);
	}
	else if ( m_pMissionDemandInfo->m_bDone == ScriptParam_MissionDemandInfo::MISSION_NODONE )
	{
		//任务没有完成
		if(pNPC)
		{
			SetCurDialogNpcId( pNPC->GetServerID() );
		}
		CEventSystem::GetMe()->PushEvent(GE_QUEST_CONTINUE_NOTDONE, pNPC ? pNPC->GetID(): -1);
	}
	else if( m_pMissionDemandInfo->m_bDone == ScriptParam_MissionDemandInfo::MISSION_CHECK )
	{
		//任务提交界面
		if(pNPC)
		{
			SetCurDialogNpcId( pNPC->GetServerID() );
		}
		//需要二次判定，需要提交物品
		CEventSystem::GetMe()->PushEvent(GE_REPLY_MISSION, pNPC ? pNPC->GetID(): -1);//这个事件同时也会打开背包界面
	}
}

VOID CUIDataPool::OnMissionContinueInfoResponse( VOID )
{
	//清空原有数据
	QuestReward_Clear();

	//生成需求物品信息
	INT nItemCount = m_pMissionContinueInfo->m_yBonusCount;
	for(INT i=0; i<nItemCount; i++)
	{
		tObject_Item* pItem = CObject_Item::NewItem(m_pMissionContinueInfo->m_aBonus[i].m_ItemBonus.m_uItemID);//->m_itemBonus.m_dwItemID);
		
		//增加到数据池中
		QuestRewardItem newRewardItem;
		newRewardItem.pItemData = &(m_pMissionContinueInfo->m_aBonus[i]);
		newRewardItem.pItemImpl = pItem;
		newRewardItem.bSelected = FALSE;
		QuestReward_AddItem(newRewardItem);
	}
	for( INT i = 0; i < m_pMissionContinueInfo->m_yTextCount; i ++ )
	{
		std::string strText = m_pMissionInfo->m_aText[i].m_szString;
		if( ConvertServerString( strText ) )
			m_pMissionInfo->m_aText[i] = strText.c_str();
	}


	//点击“继续”之后
	CEventSystem::GetMe()->PushEvent( GE_QUEST_AFTER_CONTINUE );
}

VOID CUIDataPool::OnMissionTips( VOID )
{
	std::string strText = m_pMissionTips->m_strText.m_szString;
	ConvertServerString( strText );

	CEventSystem::GetMe()->PushEvent( GE_NEW_DEBUGMESSAGE, strText.c_str() );
}

VOID CUIDataPool::OnSkillStudy()//fujia
{
	tObject* pNPC = CObjectManager::GetMe()->FindServerObject(m_pEventList->m_idNPC);

	if(pNPC)
	{
		SetCurDialogNpcId( pNPC->GetServerID() );
	}
	CEventSystem::GetMe()->PushEvent( GE_TOGLE_SKILLSTUDY , pNPC ? pNPC->GetID() : -1);
}//,pObj?pObj->GetID():-1 

VOID CUIDataPool::BeginOutGhostTimer( INT nOutGhostTimer )
{
	m_bOutGhostTimerWorking = TRUE;
	m_nOutGhostTimer = nOutGhostTimer;
}

VOID CUIDataPool::EndOutGhostTimer( VOID )
{
	m_bOutGhostTimerWorking = FALSE;
}

/*
SClientSkill* CUIDataPool::UserData_GetSkill(INT idSkill)
{
	return m_pPlayerData->GetSkill(idSkill);
}

SClientLifeAbility* CUIDataPool::UserData_GetLifeAbility(INT idAbility)
{
	return m_pPlayerData->GetLifeAbility(idAbility);
}

SClientXinFa* CUIDataPool::UserData_GetXinFa(INT idXinFa)
{
	return m_pPlayerData->GetXinFa(idXinFa);
}

//技能冷却信息
BOOL CUIDataPool::UserData_GetSkillCoollInfo(INT nSkillID, INT& nCoolDownTime, INT& nCoolTimeNow)
{
	const SCLIENT_SKILL* pSkill = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_Skill(nSkillID);
	if(!pSkill) return FALSE;

	nCoolDownTime = m_pPlayerData->GetCoolDownTotalTime(pSkill->m_pDefine->m_uID);
	nCoolTimeNow = m_pPlayerData->GetCoolDownTime(pSkill->m_pDefine->m_uID);

	return true;
}

//================================================================================

VOID CUIDataPool::UserBag_Clear(VOID)
{
	for(INT i=0; i<MAX_BAG_SIZE; i++)
	{
		if(m_listUserBag[i])
			CObject_Item::DestroyItem((CObject_Item*)m_listUserBag[i]);
	}

	ZeroMemory(m_listUserBag, sizeof(tObject_Item*)*MAX_BAG_SIZE);
}

VOID CUIDataPool::UserBag_SetItem(INT nBagIndex, tObject_Item* pItem, BOOL bClearOld)
{
	if(	nBagIndex < 0 || nBagIndex>=MAX_BAG_SIZE) return;
	//销毁原有数据
	if(m_listUserBag[nBagIndex] && bClearOld)
		CObject_Item::DestroyItem((CObject_Item*)m_listUserBag[nBagIndex]);

	if(pItem)
	{
		pItem->SetTypeOwner(tObject_Item::IO_MYSELF_PACKET);
		pItem->SetPosIndex(nBagIndex);
	}

	m_listUserBag[nBagIndex] = pItem;

	CEventSystem::GetMe()->PushEvent(GE_PACKAGE_ITEM_CHANGED);
}

tObject_Item* CUIDataPool::UserBag_GetItem(INT nBagIndex)
{
	if(	nBagIndex < 0 || nBagIndex>=MAX_BAG_SIZE) return NULL;
	return m_listUserBag[nBagIndex];
}

VOID CUIDataPool::UserBag_AskExtraInfo(INT nBagIndex)
{
	if(nBagIndex >= MAX_BAG_SIZE) return;
	if(m_setAskBagExtra.find(nBagIndex) != m_setAskBagExtra.end()) return;

	CGAskItemInfo msg;
	msg.setID((BYTE) nBagIndex);
	CNetManager::GetMe()->SendPacket(&msg);

	m_setAskBagExtra.insert(nBagIndex);
}

VOID CUIDataPool::UserBag_SetItemExtraInfo(INT nBagIndex, BOOL bEmpty, const _ITEM* pItem)
{
	if(nBagIndex >= MAX_BAG_SIZE) return;
//	if(m_setAskBagExtra.find(nBagIndex) == m_setAskBagExtra.end()) return;

	if(bEmpty)
	{
		if(m_listUserBag[nBagIndex])
		{
			CObject_Item::DestroyItem((CObject_Item*)m_listUserBag[nBagIndex]);
			m_listUserBag[nBagIndex] = NULL;
		}
	}
	else
	{
		if(m_listUserBag[nBagIndex])
			m_listUserBag[nBagIndex]->SetExtraInfo(pItem);

		m_setAskBagExtra.erase(nBagIndex);
	}
}

//================================================================================
VOID CUIDataPool::ItemBox_Clear(VOID)
{
	for(INT i=0; i<MAX_BOXITEM_NUMBER; i++)
	{
		if(m_listItemBox[i])
		{
			CObject_Item::DestroyItem((CObject_Item*)m_listItemBox[i]);
		}
	}

	m_idItemBox = INVALID_ID;
	ZeroMemory(m_listItemBox, sizeof(tObject_Item*)*MAX_BOXITEM_NUMBER);
}

VOID CUIDataPool::ItemBox_SetItem(INT nBoxIndex, tObject_Item* pItem, BOOL bClearOld)
{
	if(nBoxIndex < 0 || nBoxIndex >= MAX_BOXITEM_NUMBER) return;
	if(m_listItemBox[nBoxIndex] && bClearOld)
			CObject_Item::DestroyItem((CObject_Item*)m_listItemBox[nBoxIndex]);

	if(pItem)
	{
		pItem->SetTypeOwner(tObject_Item::IO_ITEMBOX);
		pItem->SetPosIndex(nBoxIndex);
	}

	m_listItemBox[nBoxIndex] = pItem;
}

tObject_Item* CUIDataPool::ItemBox_GetItem(INT nBoxIndex)
{
	if(nBoxIndex < 0 || nBoxIndex >= MAX_BOXITEM_NUMBER) return NULL;

	return m_listItemBox[nBoxIndex];
}

tObject_Item*	CUIDataPool::ItemBox_GetItem(WORD idWorld, WORD idServer, INT idSerial, INT& nIndex)
{
	for(INT i=0; i<MAX_BOXITEM_NUMBER; i++)
	{
		if(m_listItemBox[i])
		{
			WORD wWorld, wServer;
			GUID_t dwSerial;
			m_listItemBox[i]->GetGUID(wWorld, wServer, dwSerial);

			if(wWorld==idWorld && wServer==idServer && dwSerial==idSerial)
			{
				nIndex = i;
				return m_listItemBox[i];
			}
		}
	}
	return NULL;
}
*/
//================================================================================
VOID CUIDataPool::QuestReward_Clear(VOID)
{
	for(INT i=0; i<(INT)m_vecQuestRewardItem.size(); i++)
	{
		CObject_Item::DestroyItem((CObject_Item*)(m_vecQuestRewardItem[i].pItemImpl));
		m_vecQuestRewardItem[i].pItemImpl = NULL;
	}
	
	m_vecQuestRewardItem.clear();
}

VOID CUIDataPool::QuestDemand_Clear(VOID)
{
	for(INT i=0; i<(INT)m_vecQuestDemandItem.size(); i++)
	{
		CObject_Item::DestroyItem((CObject_Item*)(m_vecQuestDemandItem[i].pItemImpl));
		m_vecQuestDemandItem[i].pItemImpl = NULL;
	}
	
	m_vecQuestDemandItem.clear();
}

VOID CUIDataPool::QuestDemand_AddItem(const QuestDemandItem& itemDemand)
{
	m_vecQuestDemandItem.push_back(itemDemand);
}

VOID CUIDataPool::QuestReward_AddItem(const QuestRewardItem& itemReward)
{
	m_vecQuestRewardItem.push_back(itemReward);
}

//================================================================================
/*
VOID CUIDataPool::UserEquip_Clear(VOID)
{
	for(INT i=0; i<HEQUIP_NUMBER; i++)
	{
		if(m_listUserEquip[i])
			CObject_Item::DestroyItem((CObject_Item*)m_listUserEquip[i]);
	}

	ZeroMemory(m_listUserEquip, sizeof(tObject_Item*)*HEQUIP_NUMBER);
}

VOID CUIDataPool::UserEquip_SetItem(HUMAN_EQUIP ptEquip, tObject_Item* pEquipItem, BOOL bClearOld)
{
	if(pEquipItem)
	{
		HUMAN_EQUIP pointEquip = ((CObject_Item_Equip *)pEquipItem)->GetItemType();
		if(pointEquip != ptEquip) return;
	}

	if(m_listUserEquip[ptEquip] && bClearOld)
		CObject_Item::DestroyItem((CObject_Item*)m_listUserEquip[ptEquip]);
	pEquipItem->SetTypeOwner(tObject_Item::IO_MYSELF_EQUIP);
	m_listUserEquip[ptEquip] = pEquipItem;
}

tObject_Item*	CUIDataPool::UserEquip_GetItem(HUMAN_EQUIP ptEquip)
{
	if(ptEquip < 0 && ptEquip >= HEQUIP_NUMBER) return NULL;

	return m_listUserEquip[ptEquip];
}

//套装是否组合成功
INT CUIDataPool::UserEquip_IsUnion(VOID)
{
	//HEQUIP_CAP			=1,		//帽子	DEFENCE
	//HEQUIP_ARMOR			=2,		//盔甲	DEFENCE
	//HEQUIP_CUFF			=3,		//护腕	DEFENCE
	//HEQUIP_BOOT			=4,		//鞋	DEFENCE

	INT nSetID = -1;
	for(INT i=HEQUIP_CAP; i<=HEQUIP_BOOT; i++)
	{
		//缺失
		if(m_listUserEquip[i] == NULL) return -1;

		//非套装
		if(((CObject_Item_Equip*)(m_listUserEquip[i]))->GetSetID() <= 0) return -1;

		if(nSetID <= 0) 
			nSetID = ((CObject_Item_Equip*)(m_listUserEquip[i]))->GetSetID();
		else
		{
			if(((CObject_Item_Equip*)(m_listUserEquip[i]))->GetSetID() != nSetID)
			{
				return -1;
			}
		}
	}

	return nSetID;
}

//================================================================================
// 商人货架
VOID CUIDataPool::Booth_Clear(VOID)
{
	for(INT i=0; i<MAX_BOOTH_NUMBER; i++)
	{
		if(m_listBooth[i])
			CObject_Item::DestroyItem((CObject_Item*)m_listBooth[i]);
	}

	ZeroMemory(m_listBooth, sizeof(tObject_Item*)*MAX_BAG_SIZE);
	m_nBoothNumber = 0;
}
VOID CUIDataPool::Booth_Sold_Clear(VOID)
{
	for(INT i=0; i<MAX_BOOTH_SOLD_NUMBER; i++)
	{
		if(m_listBoothSold[i])
			CObject_Item::DestroyItem((CObject_Item*)m_listBoothSold[i]);
	}

	ZeroMemory(m_listBoothSold, sizeof(tObject_Item*)*MAX_BOOTH_SOLD_NUMBER);
	m_nBoothSoldNumber = 0;
}

VOID CUIDataPool::Booth_SetItem(INT nBoothIndex, tObject_Item* pItem)
{
	if(	nBoothIndex < 0 || nBoothIndex >= MAX_BOOTH_NUMBER ) return;
	//销毁原有数据
	if(m_listBooth[nBoothIndex])
		CObject_Item::DestroyItem((CObject_Item*)m_listBooth[nBoothIndex]);

	if(pItem)
	{
		pItem->SetTypeOwner(tObject_Item::IO_BOOTH);
		pItem->SetPosIndex(nBoothIndex);
	}

	m_listBooth[nBoothIndex] = pItem;
}

VOID CUIDataPool::Booth_SetSoldItem(INT nBoothIndex, tObject_Item* pItem)
{
	if(	nBoothIndex < 0 || nBoothIndex >= MAX_BOOTH_SOLD_NUMBER ) return;
	//销毁原有数据
	if(m_listBoothSold[nBoothIndex])
		CObject_Item::DestroyItem((CObject_Item*)m_listBoothSold[nBoothIndex]);

	if(pItem)
	{
		pItem->SetTypeOwner(tObject_Item::IO_BOOTH);
		pItem->SetPosIndex(nBoothIndex+200);//用来与货架中的普通商品区分
	}
	m_listBoothSold[nBoothIndex] = pItem;
}

tObject_Item* CUIDataPool::Booth_GetItem(INT nBoothIndex)
{
	if(	nBoothIndex < 0 || nBoothIndex>=MAX_BOOTH_NUMBER) return NULL;
	return m_listBooth[nBoothIndex];
}
tObject_Item* CUIDataPool::Booth_GetSoldItem(INT nBoothIndex)
{
	if(	nBoothIndex < 0 || nBoothIndex>=MAX_BOOTH_SOLD_NUMBER) return NULL;
	return m_listBoothSold[nBoothIndex];
}


tObject_Item* CUIDataPool::Booth_GetItemByID(INT IDtable)
{
	for(INT i=0; i<m_nBoothNumber; i++)
	{
		if(m_listBooth[i]->GetIdTable() == IDtable)
			return m_listBooth[i];
	}
	return	NULL;
}



//================================================================================
//银行数据的访问
VOID CUIDataPool::UserBank_Clear(VOID)
{
	for(INT i=0; i<MAX_BANK_SIZE; i++)
	{
		if(m_listUserBank[i])
			CObject_Item::DestroyItem((CObject_Item*)m_listUserBank[i]);
	}

	ZeroMemory(m_listUserBank, sizeof(tObject_Item*)*MAX_BANK_SIZE);
}

VOID CUIDataPool::UserBank_SetItem(INT nBankIndex, tObject_Item* pItem, BOOL bClearOld)
{
	if(	nBankIndex < 0 || nBankIndex>=MAX_BANK_SIZE) return;
	//销毁原有数据
	if(m_listUserBank[nBankIndex] && bClearOld)
		CObject_Item::DestroyItem((CObject_Item*)m_listUserBank[nBankIndex]);

	if(pItem)
	{
		pItem->SetTypeOwner(tObject_Item::IO_MYSELF_BANK);
		pItem->SetPosIndex(nBankIndex);
	}

	m_listUserBank[nBankIndex] = pItem;
}

tObject_Item* CUIDataPool::UserBank_GetItem(INT nBankIndex)
{
	if(	nBankIndex < 0 || nBankIndex>=MAX_BANK_SIZE) return NULL;
	return m_listUserBank[nBankIndex];
}

VOID CUIDataPool::UserBank_SetItemExtraInfo(INT nBankIndex, BOOL bEmpty, const _ITEM* pItem)
{
	if(nBankIndex >= MAX_BANK_SIZE) return;
	//	if(m_setAskBagExtra.find(nBagIndex) == m_setAskBagExtra.end()) return;

	if(bEmpty)
	{
		if(m_listUserBank[nBankIndex])
		{
			CObject_Item::DestroyItem((CObject_Item*)m_listUserBank[nBankIndex]);
			m_listUserBank[nBankIndex] = NULL;
		}
	}
	else
	{
		if(m_listUserBank[nBankIndex])
			m_listUserBank[nBankIndex]->SetExtraInfo(pItem);
	}
}


//================================================================================

//================================================================================
// 心法教师身上的心法数据
VOID CUIDataPool::SendJoinMenpaiEvent()
{
	CGAskJoinMenpai msg;
	msg.SetMenpaiID(m_idMenpaiTeacher);

	CNetManager::GetMe()->SendPacket( &msg );

	CGameProcedure::s_pGfxSystem->PushDebugString("CGAskJoinMenpai");
}

*/

//------------------------------------------------------------------------------------------------------------------------------------------------
// 组队相关的操作
//
// 由于队伍信息只与当前玩家有关系, 所以在游戏客户端只存在一份, 应该写在数据池中.
//

// 重新获取队伍成员详细信息
VOID CUIDataPool::ClearTeamInfo(VOID)
{
	if( m_TeamOrGroup.HasTeam() )
	{
		m_TeamOrGroup.DismissTeam();
		m_TeamOrGroup.ClearInviteTeam();
		m_TeamOrGroup.ClearProposer();
	}
}

// 设置SOURCE_Id
VOID CUIDataPool::SetSourceGUID(GUID_t Id) 
{
	m_IdSource = Id;
}

	// 得到SOURCE_Id
GUID_t CUIDataPool::GetSourceGUID()
{
	return m_IdSource;
}

// 得到申请者SOURCE_Id
GUID_t CUIDataPool::GetApplySourceGUID(int iApplyIndex)
{
	GUID_t ApplyGUID;
	// 根据索引得到某个申请人
	const TeamCacheInfo*	pApplyInfo = NULL;
	if(CUIDataPool::GetMe()->GetTeamOrGroup())
	{
		pApplyInfo = CUIDataPool::GetMe()->GetTeamOrGroup()->GetProposerByIndex(iApplyIndex);
		if(pApplyInfo)
		{
			ApplyGUID = pApplyInfo->m_GUID;
			return ApplyGUID;
		}

	}
	return INVALID_GUID;
}

	// 设置DESTINATION_Id
VOID CUIDataPool::SetDestinationGUID(GUID_t Id) 
{
	m_IdDestination = Id;
}


	// 得到DESTINATION_Id
GUID_t CUIDataPool::GetDestinationGUID()
{
	return m_IdDestination;
}




// 设置队伍邀请者的Id
VOID CUIDataPool::SetTeamInvitorGUID(GUID_t  Id) 
{
	m_IdSource = Id;
}

// 得到队伍邀请者的id
GUID_t CUIDataPool::GetTeamInvitorGUID()
{
	return m_IdSource;
}

// 得到队伍邀请者的id
GUID_t CUIDataPool::GetTeamInvitorGUID(int iTeamIndex)
{
	const InviteTeam* pInviteTeam = NULL;
	TeamCacheInfo*    pMemberInfo = NULL;
	if(CUIDataPool::GetMe()->GetTeamOrGroup())
	{
		
		pInviteTeam = CUIDataPool::GetMe()->GetTeamOrGroup()->GetInviteTeamByIndex(iTeamIndex);
		if(pInviteTeam)
		{
			// 记录当前邀请队伍的guid
			GUID_t guid = pInviteTeam->m_InviterGUID;
			return guid;
		}
	}

	return INVALID_GUID;
}

// 离开队伍
VOID CUIDataPool::LeaveTeam()
{
	CGTeamLeave msg;
	//BOOL bLogValid = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;
	
	msg.SetGUID(CObjectManager::GetMe()->GetMySelf()->GetServerGUID());
	CNetManager::GetMe()->SendPacket(&msg);
	//if(bLogValid)	ADDNEWDEBUGMSG(STRING("发送离开队伍消息!"));
}

// 解散队伍
VOID CUIDataPool::DismissTeam()
{
	CGTeamDismiss msg;
	//BOOL bLogValid = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;
	
	msg.SetGUID(CObjectManager::GetMe()->GetMySelf()->GetServerGUID());
	CNetManager::GetMe()->SendPacket(&msg);
	// CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"发送解散队伍消息!");
	//if(bLogValid)	ADDNEWDEBUGMSG(STRING("发送解散队伍消息!"));
}


// 踢人信息
VOID CUIDataPool::KickTeamMember()
{

	const TeamMemberInfo* pMemberInfo = NULL;
	BOOL bLogValid = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;

	if(GetTeamOrGroup())
	{
		pMemberInfo = GetTeamOrGroup()->GetMemberByUIIndex(m_iSelectedTeamMember);
		if(pMemberInfo)
		{
			if(CObjectManager::GetMe()->GetMySelf()->GetServerGUID() == pMemberInfo->m_GUID)
			{
				return;
			}

			CGTeamKick msg;
			msg.SetSourGUID(CObjectManager::GetMe()->GetMySelf()->GetServerGUID());
			msg.SetDestGUID(pMemberInfo->m_GUID);
			CNetManager::GetMe()->SendPacket(&msg);
			//if(bLogValid)	ADDNEWDEBUGMSG(STRING("发送踢出队伍消息!"));

		}
		else
		{
			if(bLogValid)	
			{
				ADDNEWDEBUGMSG(STRING("你的队伍解散了!"));
			}
		}
	}
	else
	{
		if(bLogValid)	
		{
			ADDNEWDEBUGMSG(STRING("你的队伍解散了!"));
		}
	}

}

// 踢人信息
VOID CUIDataPool::KickTeamMember(int iIndex)
{

	//const TeamMemberInfo* pMemberInfo = NULL;
	//BOOL bLogValid = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;

	//if(GetTeamOrGroup())
	//{
	//	pMemberInfo = GetTeamOrGroup()->GetMemberByIndex(iIndex);
	//	if(pMemberInfo)
	//	{
	//		CGTeamKick msg;
	//		msg.SetSourGUID(CObjectManager::GetMe()->GetMySelf()->GetServerGUID());
	//		msg.SetDestGUID(pMemberInfo->m_GUID);
	//		CNetManager::GetMe()->SendPacket(&msg);
	//		CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"发送踢出队伍消息!");

	//	}
	//	else
	//	{
	//		CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"找不到要删除的队员!");
	//	}
	//}
	//else
	//{
	//	CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"找不到要删除的队员!");
	//}//

	const TeamMemberInfo* pMemberInfo = NULL;
	BOOL bLogValid = (CGameProcedure::s_pUISystem != NULL)?TRUE:FALSE;

	if(GetTeamOrGroup())
	{
		pMemberInfo = GetTeamOrGroup()->GetMemberByUIIndex(iIndex);
		if(pMemberInfo)
		{
			if(CObjectManager::GetMe()->GetMySelf()->GetServerGUID() == pMemberInfo->m_GUID)
			{
				return;
			}

			CGTeamKick msg;
			msg.SetSourGUID(CObjectManager::GetMe()->GetMySelf()->GetServerGUID());
			msg.SetDestGUID(pMemberInfo->m_GUID);
			CNetManager::GetMe()->SendPacket(&msg);
			//if(bLogValid)	ADDNEWDEBUGMSG(STRING("发送踢出队伍消息!"));

		}
		else
		{
			if(bLogValid)	
			{
				ADDNEWDEBUGMSG(STRING("你的队伍解散了!"));
			}
		}
	}
	else
	{
		if(bLogValid)	
		{
			ADDNEWDEBUGMSG(STRING("你的队伍解散了!"));
		}
	}

}



	// 提升队长
VOID CUIDataPool::AppointLeader(int iIndex)
{
	const TeamMemberInfo* pMemberInfo = NULL;
	if(GetTeamOrGroup())
	{
		pMemberInfo = GetTeamOrGroup()->GetMemberByUIIndex(iIndex);
		if(pMemberInfo)
		{
			CGTeamAppoint msg;
			msg.SetSourGUID(CObjectManager::GetMe()->GetMySelf()->GetServerGUID());
			msg.SetDestGUID(pMemberInfo->m_GUID);
		
			CNetManager::GetMe()->SendPacket(&msg);

			// 清空申请者.
			GetTeamOrGroup()->ClearProposer();
			//CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"发送任命队长消息!");

			return;
		}
	}
		
	//CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"发送任命队长消息失败!");
	
}

// 提升队长
VOID CUIDataPool::AppointLeader()
{
	const TeamMemberInfo* pMemberInfo = NULL;
	if(GetTeamOrGroup())
	{
		pMemberInfo = GetTeamOrGroup()->GetMemberByUIIndex(m_iSelectedTeamMember);
		if(pMemberInfo)
		{
			CGTeamAppoint msg;
			msg.SetSourGUID(CObjectManager::GetMe()->GetMySelf()->GetServerGUID());
			msg.SetDestGUID(pMemberInfo->m_GUID);
		
			CNetManager::GetMe()->SendPacket(&msg);

			// 清空申请者.
			GetTeamOrGroup()->ClearProposer();
			//CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"发送任命队长消息!");
		}
	}
	else
	{
		CGameProcedure::s_pEventSystem->PushEvent(GE_NEW_DEBUGMESSAGE,"发送任命队长消息失败!");
	}
}


// 同意加入一个队伍后, 清空所有的邀请队伍.
VOID CUIDataPool::ClearAllInviteTeam()
{
	if(GetTeamOrGroup())
	{
		GetTeamOrGroup()->ClearInviteTeam();
	}
}

// 拒绝当前拒绝的队伍.
VOID CUIDataPool::EraseCurRejectTeam()
{
	if(GetTeamOrGroup())
	{
		GetTeamOrGroup()->EraseInviteTeam(m_iCurRejectTeamIndex);
	}
}


// 某个队员是否是队长.
BOOL CUIDataPool::IsTeamLeader(INT iIndex)
{
	
	const TeamMemberInfo* pMemberInfo = NULL;
	if(CUIDataPool::GetMe()->GetTeamOrGroup())
	{
		pMemberInfo = CUIDataPool::GetMe()->GetTeamOrGroup()->GetMemberByIndex(iIndex);

		if(pMemberInfo)
		{
			if(pMemberInfo->m_GUID == CUIDataPool::GetMe()->GetTeamOrGroup()->GetLeaderGUID())
			{
				return TRUE;
			}

		}
	}

	return FALSE;
}

// 自己是否是队长.
BOOL CUIDataPool::IsTeamLeader_Me()
{
	if(CUIDataPool::GetMe()->GetTeamOrGroup()->HasTeam())
	{
		if( CUIDataPool::GetMe()->GetTeamOrGroup()->GetLeaderGUID()
		 == CObjectManager::GetMe()->GetMySelf()->GetServerGUID()
		 )
		{
			return TRUE;
		}
	}

	return FALSE;
}


// 设置当前队伍的类型.
void CUIDataPool::SetCurTeamType(INT iTeamType)
{
	m_iCurTeamType = iTeamType;
}

// 得到当前的队伍的类型.
INT  CUIDataPool::GetCurTeamType()
{

	return m_iCurTeamType;

}

// 当前是否在一个队伍中
BOOL CUIDataPool::IsInTeam()
{
	return CUIDataPool::GetMe()->GetTeamOrGroup()->HasTeam();
}


// 队员与自己是否在同一个场景中
BOOL CUIDataPool::IsInScene(INT iIndex)
{
	return CUIDataPool::GetMe()->GetTeamOrGroup()->IsInScene(iIndex);
}

// 判断一个玩家与自己是否在同一个队伍中。
BOOL CUIDataPool::IsTeamMember(GUID_t Id)
{
	CTeamOrGroup* pTeamGroup = NULL;

	// 得到队伍信息
	pTeamGroup = CUIDataPool::GetMe()->GetTeamOrGroup();

	if(pTeamGroup)
	{
		// 没有队伍
		if(!pTeamGroup->HasTeam())
		{
			return FALSE;
		}

		// 得到队伍中队员的个数
		int iTeamMemberCount = pTeamGroup->GetTeamMemberCount();
		const TeamMemberInfo* pTeamMemberInfo = NULL;
		for(int i = 0; i < iTeamMemberCount; i++)
		{
			// 得到队员的信息
			pTeamMemberInfo = pTeamGroup->GetMemberByIndex(i);
			if(pTeamMemberInfo)
			{
				if(Id == pTeamMemberInfo->m_GUID)
				{
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}


// 通过guid得到队友名字.2006-4-12
LPCTSTR CUIDataPool::GetTeamNameByGUID(GUID_t Id)
{
	CTeamOrGroup* pTeamGroup = NULL;

	// 得到队伍信息
	pTeamGroup = CUIDataPool::GetMe()->GetTeamOrGroup();

	if(pTeamGroup)
	{
		// 没有队伍
		if(!pTeamGroup->HasTeam())
		{
			return FALSE;
		}

		// 得到队伍中队员的个数
		int iTeamMemberCount = pTeamGroup->GetTeamMemberCount();
		const TeamMemberInfo* pTeamMemberInfo = NULL;
		for(int i = 0; i < iTeamMemberCount; i++)
		{
			// 得到队员的信息
			pTeamMemberInfo = pTeamGroup->GetMemberByIndex(i);
			if(pTeamMemberInfo)
			{
				if(Id == pTeamMemberInfo->m_GUID)
				{
					return pTeamMemberInfo->m_szNick;
				}
			}
		}
	}
	return NULL;
}

	// 通过guid得到队友HP.2006-4-12
int CUIDataPool::GetTeamHPByGUID(GUID_t Id)
{

	CTeamOrGroup* pTeamGroup = NULL;

	// 得到队伍信息
	pTeamGroup = CUIDataPool::GetMe()->GetTeamOrGroup();

	if(pTeamGroup)
	{
		// 没有队伍
		if(!pTeamGroup->HasTeam())
		{
			return FALSE;
		}

		// 得到队伍中队员的个数
		int iTeamMemberCount = pTeamGroup->GetTeamMemberCount();
		const TeamMemberInfo* pTeamMemberInfo = NULL;
		for(int i = 0; i < iTeamMemberCount; i++)
		{
			// 得到队员的信息
			pTeamMemberInfo = pTeamGroup->GetMemberByIndex(i);
			if(pTeamMemberInfo)
			{
				if(Id == pTeamMemberInfo->m_GUID)
				{
					return pTeamMemberInfo->m_nHP;
				}
			}
		}
	}

	return 0;
}

	// 通过guid得到队友MP.2006-4-12
int CUIDataPool::GetTeamMPByGUID(GUID_t Id)
{

	CTeamOrGroup* pTeamGroup = NULL;

	// 得到队伍信息
	pTeamGroup = CUIDataPool::GetMe()->GetTeamOrGroup();

	if(pTeamGroup)
	{
		// 没有队伍
		if(!pTeamGroup->HasTeam())
		{
			return FALSE;
		}

		// 得到队伍中队员的个数
		int iTeamMemberCount = pTeamGroup->GetTeamMemberCount();
		const TeamMemberInfo* pTeamMemberInfo = NULL;
		for(int i = 0; i < iTeamMemberCount; i++)
		{
			// 得到队员的信息
			pTeamMemberInfo = pTeamGroup->GetMemberByIndex(i);
			if(pTeamMemberInfo)
			{
				if(Id == pTeamMemberInfo->m_GUID)
				{
					return pTeamMemberInfo->m_nMP;
				}
			}
		}
	}
	return 0;
}

	// 通过guid得到队友Range.2006-4-12
int CUIDataPool::GetTeamRangeByGUID(GUID_t Id)
{
	CTeamOrGroup* pTeamGroup = NULL;

	// 得到队伍信息
	pTeamGroup = CUIDataPool::GetMe()->GetTeamOrGroup();

	if(pTeamGroup)
	{
		// 没有队伍
		if(!pTeamGroup->HasTeam())
		{
			return FALSE;
		}

		// 得到队伍中队员的个数
		int iTeamMemberCount = pTeamGroup->GetTeamMemberCount();
		const TeamMemberInfo* pTeamMemberInfo = NULL;
		for(int i = 0; i < iTeamMemberCount; i++)
		{
			// 得到队员的信息
			pTeamMemberInfo = pTeamGroup->GetMemberByIndex(i);
			if(pTeamMemberInfo)
			{
				if(Id == pTeamMemberInfo->m_GUID)
				{
					return pTeamMemberInfo->m_nAnger;
				}
			}
		}
	}
	return 0;
}

// 通过guid得到队友Level.2006-4-12
int CUIDataPool::GetTeamLevelByGUID(GUID_t Id)
{
	CTeamOrGroup* pTeamGroup = NULL;

	// 得到队伍信息
	pTeamGroup = CUIDataPool::GetMe()->GetTeamOrGroup();

	if(pTeamGroup)
	{
		// 没有队伍
		if(!pTeamGroup->HasTeam())
		{
			return FALSE;
		}

		// 得到队伍中队员的个数
		int iTeamMemberCount = pTeamGroup->GetTeamMemberCount();
		const TeamMemberInfo* pTeamMemberInfo = NULL;
		for(int i = 0; i < iTeamMemberCount; i++)
		{
			// 得到队员的信息
			pTeamMemberInfo = pTeamGroup->GetMemberByIndex(i);
			if(pTeamMemberInfo)
			{
				if(Id == pTeamMemberInfo->m_GUID)
				{
					return pTeamMemberInfo->m_uLevel;
				}
			}
		}
	}

	return 0;
}


	// 通过guid得到队友头像.2006-4-12
LPCTSTR CUIDataPool::GetTeamIconByGUID(GUID_t Id)
{
	CTeamOrGroup* pTeamGroup = NULL;

	// 得到队伍信息
	pTeamGroup = CUIDataPool::GetMe()->GetTeamOrGroup();

	if(pTeamGroup)
	{
		// 没有队伍
		if(!pTeamGroup->HasTeam())
		{
			return FALSE;
		}

		// 得到队伍中队员的个数
		int iTeamMemberCount = pTeamGroup->GetTeamMemberCount();
		const TeamMemberInfo* pTeamMemberInfo = NULL;
		for(int i = 0; i < iTeamMemberCount; i++)
		{
			// 得到队员的信息
			pTeamMemberInfo = pTeamGroup->GetMemberByIndex(i);
			if(pTeamMemberInfo)
			{
				if(Id == pTeamMemberInfo->m_GUID)
				{
					DBC_DEFINEHANDLE(s_pFaceImageDBC, DBC_CHARACTER_FACE)
					//打开生物定义表
					const _DBC_CHAR_FACE* pFaceInfo = NULL;
					pFaceInfo = (const _DBC_CHAR_FACE*)(s_pFaceImageDBC->Search_Index_EQU(pTeamMemberInfo->m_nPortrait));
					// 得到头像名字

					if(pFaceInfo)
					{
						return pFaceInfo->pImageSetName;
					}
					else
					{
						return NULL;
					}

				}
			}
		}
	}


	return NULL;
}

// 通过guid得到队友HP.2006-4-12
float CUIDataPool::GetTeamHPPercentByGUID(GUID_t Id)
{
	
	CTeamOrGroup* pTeamGroup = NULL;

	// 得到队伍信息
	pTeamGroup = CUIDataPool::GetMe()->GetTeamOrGroup();

	if(pTeamGroup)
	{
		// 没有队伍
		if(!pTeamGroup->HasTeam())
		{
			return FALSE;
		}

		// 得到队伍中队员的个数
		int iTeamMemberCount = pTeamGroup->GetTeamMemberCount();
		const TeamMemberInfo* pTeamMemberInfo = NULL;
		for(int i = 0; i < iTeamMemberCount; i++)
		{
			// 得到队员的信息
			pTeamMemberInfo = pTeamGroup->GetMemberByIndex(i);
			if(pTeamMemberInfo)
			{
				if(Id == pTeamMemberInfo->m_GUID)
				{
					if(pTeamMemberInfo->m_dwMaxHP)
					{
						return (1.0f * pTeamMemberInfo->m_nHP) / ((float)(pTeamMemberInfo->m_dwMaxHP));
					}
				}
			}
		}
	}

	return 0;
}

// 通过guid得到队友MP.2006-4-12
float CUIDataPool::GetTeamMPPercentByGUID(GUID_t Id)
{

	CTeamOrGroup* pTeamGroup = NULL;

	// 得到队伍信息
	pTeamGroup = CUIDataPool::GetMe()->GetTeamOrGroup();

	if(pTeamGroup)
	{
		// 没有队伍
		if(!pTeamGroup->HasTeam())
		{
			return FALSE;
		}

		// 得到队伍中队员的个数
		int iTeamMemberCount = pTeamGroup->GetTeamMemberCount();
		const TeamMemberInfo* pTeamMemberInfo = NULL;
		for(int i = 0; i < iTeamMemberCount; i++)
		{
			// 得到队员的信息
			pTeamMemberInfo = pTeamGroup->GetMemberByIndex(i);
			if(pTeamMemberInfo)
			{
				if(Id == pTeamMemberInfo->m_GUID)
				{
					if(pTeamMemberInfo->m_dwMaxMP)
					{
						return (1.0f * pTeamMemberInfo->m_nMP) / ((float)(pTeamMemberInfo->m_dwMaxMP));
					}
				}
			}
		}
	}

	return 0;
}


// 设置队员邀请者和被邀请者guid2006－4－28
INT CUIDataPool::SetMemberInviteGUID(GUID_t SId, GUID_t DId)
{
	INT iPos = -1; 
	for(int i = 0; i < MAX_INVITE_COUNT; i++)
	{
		if(!m_MemberInviteInfo[i].bHave)
		{
			m_MemberInviteInfo[i].bHave = TRUE;
			m_MemberInviteInfo[i].SourceId = SId;
			m_MemberInviteInfo[i].DestinId = DId;
			iPos = i;
			break;
		}
	}

	return iPos;
}


//	得到队员邀请者和被邀请者guid， 并删除2006－4－28
BOOL CUIDataPool::GetMemberInviteGUID(int iPos, GUID_t& SId, GUID_t& DId, BOOL bDel)
{
	if((iPos < 0)||(iPos >= MAX_INVITE_COUNT))
	{
		return FALSE;
	}

	SId = m_MemberInviteInfo[iPos].SourceId;
	DId = m_MemberInviteInfo[iPos].DestinId;

	if(bDel)
	{
		m_MemberInviteInfo[iPos].bHave = FALSE;
	}

	return TRUE;


}








//
// 组队相关的操作
//---------------------------------------------------------------------------------------------------------------------------------------



//转义所有从server传过来的字符串
BOOL CUIDataPool::ConvertServerString( std::string& strConvert )
{
	BOOL bConvert = FALSE;
	CHAR szNum[32];
	size_t nPos = strConvert.find( "@itemid_" );
	while( nPos != std::string::npos )
	{
		std::string strItemIndex = strConvert.substr( nPos + 8 );
		INT nItemIndex = atoi( strItemIndex.c_str() );
		itoa( nItemIndex, szNum, 10 );
		std::string strItemName = CObjectManager::GetMe()->ItemNameByTBIndex( nItemIndex );
		strConvert = strConvert.substr( 0, nPos ) + strItemName + strItemIndex.substr( strlen(szNum) );
		nPos = strConvert.find( "@itemid_" );
		bConvert = TRUE;
	}
	nPos = strConvert.find( "@monsterid_" );
	while( nPos != std::string::npos )
	{
		std::string strItemIndex = strConvert.substr( nPos + 11 );
		INT nItemIndex = atoi( strItemIndex.c_str() );

		DBC_DEFINEHANDLE(s_pMonsterAttr, DBC_CREATURE_ATT);
		const _DBC_CREATURE_ATT* pMonsterAttr = (const _DBC_CREATURE_ATT*)s_pMonsterAttr->Search_Index_EQU( nItemIndex );
		if( pMonsterAttr )
		{
			itoa( nItemIndex, szNum, 10 );
			std::string strItemName = pMonsterAttr->pName;
			strConvert = strConvert.substr( 0, nPos ) + strItemName + strItemIndex.substr( strlen(szNum) );
			bConvert = TRUE;
		}
		else
		{
			break;
		}
		nPos = strConvert.find( "@monsterid_" );
	}

	return bConvert;
}

int CUIDataPool::GetStudySkill_Num()
{
	return m_pSkillStudy->m_yStudyCount;
}

int CUIDataPool::GetStudySkill_Id(GUID_t iIndex)
{
	if (iIndex<0||iIndex>m_pSkillStudy->m_yStudyCount) 
	{
		return -1;
	}
	return m_pSkillStudy->m_aSkillItem[iIndex].nSkillID;
}

int CUIDataPool::GetStudySkill_Level(GUID_t iIndex)
{
	if (iIndex<0||iIndex>m_pSkillStudy->m_yStudyCount) 
	{
		return -1;
	}
	return m_pSkillStudy->m_aSkillItem[iIndex].nLevel;
}

int CUIDataPool::GetStudySkill_NPCID()
{
	return m_pSkillStudy->m_nReserve;
}
int CUIDataPool::GetStudySkill_Type(GUID_t iIndex)
{
	if (iIndex<0||iIndex>m_pSkillStudy->m_yStudyCount) 
	{
		return -1;
	}
	return m_pSkillStudy->m_aSkillItem[iIndex].SkillType;
}
