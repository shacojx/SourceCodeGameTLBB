#include "StdAfx.h"
#include ".\cursormng.h"
#include "..\resource.h"
#include "..\Network\NetManager.h"
#include "..\gamecommand.h"
#include "..\object\Object.h"
#include "..\Interface\GMGameInterface.h"
#include "..\Procedure\GameProcedure.h"
#include "..\Object\ObjectManager.h"
#include "..\Object\Character\Obj_Character.h"
#include "..\Object\Character\Obj_PlayerMySelf.h"
#include "..\Object\TripperObj\GMTripperObj_ItemBox.h"
#include "DB_Struct.h"
#include "..\Global.h"
#include "TDUtil.h"
#include "..\Event\GMEventSystem.h"
#include "TDActionSystem.h"
#include "TDTimeSystem.h"
#include "..\Action\GMActionSystem.h"
#include "..\DataPool\GMDP_CharacterData.h"
#include "..\DataPool\GMDataPool.h"
#include "..\Action\GMActionSystem_Item.h"
#include "CGUseItem.h"
#include "..\Object\Item\Obj_Item_Medicine.h"
#include "..\Action\GMActionSystem_PetSkill.h"
#include "..\interface\GMGameInterface_Script.h"
#include "..\Input\GMInputSystem.h"
#include "CGExchangeApplyI.h"
#include "CGRelation.h"

WX_IMPLEMENT_DYNAMIC(CCursorMng, GETCLASS(tCursorSystem));
CCursorMng* CCursorMng::s_pMe = NULL;
CCursorMng::CCursorMng(VOID)
{
	s_pMe = this;

	m_bShow = TRUE;
	m_nCursorState	= CURSOR_NORMAL;
	m_hUICursor = NULL;
	m_cmdCurrent_Left.Cleanup();
	m_cmdCurrent_Right.Cleanup();

	m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_NULL;
	m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_NULL;
}

CCursorMng::~CCursorMng(VOID)
{
}

VOID CCursorMng::SetCursor(ENUM_CURSOR_TYPE nType )
{
	m_nCursorState		= nType;
	::PostMessage( g_hMainWnd, WM_SETCURSOR, 0, HTCLIENT );
}

INT CCursorMng::GetCursor_State(VOID)
{
//	if(!m_bShow) return CURSOR_NORMAL;

	return m_nCursorState;
}

HCURSOR CCursorMng::GetCursor(VOID)
{
	if(!m_bShow) return NULL;

	return m_hCursor[m_nCursorState];
}

//响应WM_SETCURSOR
VOID CCursorMng::OnSetCursor(VOID)
{
	if(m_bShow)
	{
		if(m_hUICursor)
		{
			::SetCursor(m_hUICursor);
		}
		else
		{
			::SetCursor(GetCursor());
		}
	}
}

VOID CCursorMng::ShowCursor(BOOL bShow)
{
	if(bShow == m_bShow) return;

	m_bShow = bShow;
	::ShowCursor(m_bShow);
}


// 激活当前鼠标上挂接的命令
VOID CCursorMng::MouseCommand_Active(SCommand_Mouse& cmd)
{
	switch(cmd.m_typeMouse)
	{
	case SCommand_Mouse::MCT_PLAYER_MOVETO:
		{
			static UINT uPrevMoveTime = 0;
			if(CGameProcedure::s_pTimeSystem->GetTimeNow() > uPrevMoveTime + 100)
			{
				uPrevMoveTime = CGameProcedure::s_pTimeSystem->GetTimeNow();
				CGameProcedure::s_pGameInterface->Player_MoveTo(fVector2(cmd.m_afParam[0], cmd.m_afParam[1]));
	//			AxTrace(0, 0, "Active: MoveTo(Pos=%.1f, %.1f)", (float)cmd.m_afParam[0], (float)cmd.m_afParam[1]);
			}
		}
		break;

	case SCommand_Mouse::MCT_PLAYER_SELECT:
		{
			// 
			CGameProcedure::s_pGameInterface->Object_SelectAsMainTarget((INT)cmd.m_adwParam[0], 1);
			
			
			if(	m_cmdCurrent_Left.m_typeMouse == SCommand_Mouse::MCT_SKILL_DIR ||
				m_cmdCurrent_Left.m_typeMouse == SCommand_Mouse::MCT_SKILL_AREA)
			{
				m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_NULL;
				m_cmdCurrent_Left.m_apParam[0] = NULL;
				m_cmdCurrent_Left.m_afParam[1] = NULL;
				//恢复激活技能
				CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
			}
//			AxTrace(0, 0, "Active: SelectPlayer(ID=%d)", (int)cmd.m_afParam[0]);
		}
		break;

	case SCommand_Mouse::MCT_SKILL_OBJ:
		{
			//首先选择
			CGameProcedure::s_pGameInterface->Object_SelectAsMainTarget((INT)cmd.m_adwParam[1]);

			tActionItem* pItem = (tActionItem*)cmd.m_apParam[0];
			if(!pItem) break;

			//进入攻击
			if(pItem->GetType() == AOT_PET_SKILL)
			{
				int nPetSkillID = ((PET_SKILL*)(((CActionItem_PetSkill*)pItem)->GetImpl()))->m_pDefine->m_nID;
				CDataPool::GetMe()->Pet_SendUseSkillMessage(nPetSkillID, 
					INVALID_ID, cmd.m_afParam[1], cmd.m_afParam[2]);
			}
			else
			{
				CGameProcedure::s_pGameInterface->Player_UseSkill(
							((tActionItem*)cmd.m_apParam[0])->GetID(),
							(INT)cmd.m_adwParam[1]);
			}
//			AxTrace(0, 0, "Active: SkillObj(Skill=%d, ID=%d)", (int)cmd.m_afParam[0], (int)cmd.m_adwParam[1]);
		}
		break;

	case SCommand_Mouse::MCT_SKILL_AREA:
		{
			tActionItem* pItem = (tActionItem*)cmd.m_apParam[0];
			if(!pItem) break;

			if(pItem->GetType() == AOT_PET_SKILL)
			{
				int nPetSkillID = ((PET_SKILL*)(((CActionItem_PetSkill*)pItem)->GetImpl()))->m_pDefine->m_nID;
				CDataPool::GetMe()->Pet_SendUseSkillMessage(nPetSkillID, 
					INVALID_ID, cmd.m_afParam[1], cmd.m_afParam[2]);
			}
			else
			{
				CGameProcedure::s_pGameInterface->Player_UseSkill(
							((tActionItem*)cmd.m_apParam[0])->GetID(),
							fVector2(cmd.m_afParam[1], cmd.m_afParam[2]));
			}
//			AxTrace(0, 0, "Active: SkillArea(Skill=%d, Pos=%.1f,%.1f)", 
//				(int)cmd.m_afParam[0], (int)cmd.m_adwParam[1], (int)cmd.m_afParam[2]);

			//恢复激活Action
			CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		}
		break;

	case SCommand_Mouse::MCT_CANCEL_SKILL:
		{
			//恢复激活Action
			CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		}
		break;

	case SCommand_Mouse::MCT_SKILL_DIR:
		{
			TDThrow("Not support any more!");
		}
		break;

	case SCommand_Mouse::MCT_HIT_TRIPPEROBJ:
		{
			CGameProcedure::s_pGameInterface->TripperObj_Active(cmd.m_adwParam[0]);
//			AxTrace(0, 0, "Active: TripObj(ID=%d)", (int)cmd.m_adwParam[0]);
		}
		break;

	case SCommand_Mouse::MCT_SPEAK:
		{
			//首先选择
			CGameProcedure::s_pGameInterface->Object_SelectAsMainTarget((INT)cmd.m_adwParam[0]);
			
			//谈话
			CGameProcedure::s_pGameInterface->Player_Speak(cmd.m_adwParam[0]);

//			AxTrace(0, 0, "Active: Speak(ID=%d)", (int)cmd.m_adwParam[0]);
		}
		break;

	case SCommand_Mouse::MCT_CONTEXMENU:
		{
			//首先选择
			CGameProcedure::s_pGameInterface->Object_SelectAsMainTarget((INT)cmd.m_adwParam[0], 1);
			
			//显示右键菜单
			CGameProcedure::s_pGameInterface->Object_ShowContexMenu(cmd.m_adwParam[0]);
//			AxTrace(0, 0, "Active: ContexMenu(ID=%d)", (int)cmd.m_adwParam[0]);
		}
		break;

	case SCommand_Mouse::MCT_REPAIR:
		{
			//do nothing...
		}
		break;

	case SCommand_Mouse::MCT_CANCLE_REPAIR:
		{
			//取消修理
			CActionSystem::GetMe()->SetDefaultAction(CGameInterface::GetMe()->Skill_GetActive());
		}
		break;

    
	case SCommand_Mouse::MCT_UI_USE_IDENTIFY:
		{
			// 使用鉴定卷轴
			
			break;
		}
	case SCommand_Mouse::MCT_UI_USE_CANCEL_IDENTIFY:
		{
			// 取消使用鉴定卷轴
			CDataPool::GetMe()->Identify_UnLock();
			break;
		}
	case SCommand_Mouse::MCT_UI_USE_EXCHANGE:
		{
			if(TRUE == CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsInStall())
			{
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你正在摆摊……");
				return ;
			}
			CGameProcedure::s_pGameInterface->Object_SelectAsMainTarget((INT)cmd.m_adwParam[0]);
			CObject* pChar = (CObject*) CObjectManager::GetMe()->GetMainTarget();
			if(pChar && g_theKernel.IsKindOf(pChar->GetClass(), GETCLASS(CObject_PlayerOther)))
			{
				//先判定距离是不是合法
				if(pChar)
				{
					const fVector3 pMePos = CObjectManager::GetMe()->GetMySelf()->GetPosition();
					const fVector3 pOtherPos = ((CObject*)pChar)->GetPosition();

					FLOAT fDistance = 
						TDU_GetDist(fVector2(pMePos.x, pMePos.z),
						fVector2(pOtherPos.x, pOtherPos.z));

					if( EXCHANGE_MAX_DISTANCE < fDistance )
					{
						CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"距离太远，启动交易失败");
						return;
					}
				}

				//不能向已经死亡的玩家发送交易申请
				if ( ((CObject_Character*)pChar)->CharacterLogic_Get() == CObject_Character::CHARACTER_LOGIC_DEAD )
				{
					CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"不能向已经死亡的玩家申请交易");
					return ;
				}

				CGExchangeApplyI msg;
				msg.SetObjID(pChar->GetServerID());
				CNetManager::GetMe()->SendPacket(&msg);
				CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"交易请求已经发送");
				CGameProcedure::s_pGameInterface->Skill_CancelAction();
				return ;
			}
		}
		break;
	case SCommand_Mouse::MCT_UI_USE_CANCEL_EXCHANGE:
		{
			CActionSystem::GetMe()->SetDefaultAction(CGameInterface::GetMe()->Skill_GetActive());
			break;
		}
	case SCommand_Mouse::MCT_SALE:
		break;
	case SCommand_Mouse::MCT_CANCEL_SALE:
		{
			CActionSystem::GetMe()->SetDefaultAction(CGameInterface::GetMe()->Skill_GetActive());
			break;
		}
	case SCommand_Mouse::MCT_BUYMULT:
		break;
	case SCommand_Mouse::MCT_CANCEL_BUYMULT:
		{
			CActionSystem::GetMe()->SetDefaultAction(CGameInterface::GetMe()->Skill_GetActive());
			break;
		}
	case SCommand_Mouse::MCT_UI_USE_ADDFRIEND:
		{
			//首先选择
			CGRelation Msg;
			Msg.GetRelation()->CleanUp();
			Msg.GetRelation()->m_Type = REQ_ADDFRIEND;
			REQUEST_ADD_RELATION_WITH_GROUP* pFriend = &(Msg.GetRelation()->m_AddRelationWithGroup);
			pFriend->CleanUp();
			CGameProcedure::s_pGameInterface->Object_SelectAsMainTarget((INT)cmd.m_adwParam[0]);
			CObject_Character* pCharObj = (CObject_Character*)CObjectManager::GetMe()->GetMainTarget();
			if( pCharObj == NULL )
				break;
			if( pCharObj == CObjectManager::GetMe()->GetMySelf() ) // 如果是自己，就不加
				break;
			if( g_theKernel.IsKindOf( pCharObj->GetClass(), GETCLASS(CObject_PlayerOther) ) ) // 如果是玩家
			{
				ENUM_RELATION sCamp = CGameInterface::GetMe()->GetCampType( 
					pCharObj, 
					(CObject*)CObjectManager::GetMe()->GetMySelf() );
				if( sCamp != RELATION_FRIEND ) // 如果是同一阵营的
				{
					CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "无效目标");
					break;
				}
			}
			else
			{
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, "无效目标");
				break;
			}
			pFriend->SetTargetName( pCharObj->GetCharacterData()->Get_Name() );
			pFriend->SetGroup( SCRIPT_SANDBOX::Friend::m_nCurTeam );
			pFriend->SetRelationType( RELATION_TYPE_FRIEND );
			CNetManager::GetMe()->SendPacket( &Msg );
		}
		break;
	case SCommand_Mouse::MCT_UI_USE_CANCEL_ADDFRIEND:
		CActionSystem::GetMe()->SetDefaultAction(CGameInterface::GetMe()->Skill_GetActive());
		break;
	case SCommand_Mouse::MCT_USE_ITEM:
		{
			//使用物品
			CGameProcedure::s_pGameInterface->PacketItem_UserItem((
				tActionItem*)cmd.m_apParam[0], 
				cmd.m_adwParam[1],
				fVector2(cmd.m_afParam[2], cmd.m_afParam[3]));

			//恢复激活Action
			CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		}
		break;

	case SCommand_Mouse::MCT_CANCEL_USE_ITEM:
		{
			//恢复激活Action
			CActionSystem::GetMe()->SetDefaultAction(CGameProcedure::s_pGameInterface->Skill_GetActive());
		}
		break;

	case SCommand_Mouse::MCT_LEAP:
		{
			//轻功...
			//SkillID_t id = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->GetDefaultFlySkillID();
			//CObjectManager::GetMe()->GetMySelf()->CharacterLogic_GetAI()->PushCommand_Fly(id, cmd.m_afParam[0], cmd.m_afParam[1]);
		}
		break;
	case SCommand_Mouse::MCT_CATCH_PET:
		{
			CGameProcedure::s_pGameInterface->Player_UseSkill( cmd.m_adwParam[0], int( cmd.m_adwParam[1] ) );
		}
		break;
	default:
		AxTrace(0, 0, "Active: ERROR!(%d)", cmd.m_typeMouse);
		break;
	}
}

VOID CCursorMng::Initial( VOID * pHandle)
{
	HINSTANCE hInstance = *( ( HINSTANCE*) pHandle );

	m_hCursor[ CURSOR_WINBASE ]		= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_ARROW));
	m_hCursor[ CURSOR_NORMAL ]		= ::LoadCursorFromFile("..\\Data\\Cursor\\Normal.ani");
	m_hCursor[ CURSOR_ATTACK ]		= ::LoadCursorFromFile("..\\Data\\Cursor\\Attack.ani");
	m_hCursor[ CURSOR_AUTORUN ]		= ::LoadCursorFromFile("..\\Data\\Cursor\\AutoRun.ani");
	m_hCursor[ CURSOR_PICKUP ]		= ::LoadCursorFromFile("..\\Data\\Cursor\\PickUp.ani");
	m_hCursor[ CURSOR_UNREACHABLE ] = ::LoadCursorFromFile("..\\Data\\Cursor\\UnReachable.ani");
	m_hCursor[ CURSOR_MINE ]		= ::LoadCursorFromFile("..\\Data\\Cursor\\Mine.ani");
	m_hCursor[ CURSOR_HERBS ]		= ::LoadCursorFromFile("..\\Data\\Cursor\\Herbs.ani");
	m_hCursor[ CURSOR_FISH ]		= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_ARROW )); 
	m_hCursor[ CURSOR_SPEAK ]		= ::LoadCursorFromFile("..\\Data\\Cursor\\Speak.ani");
	m_hCursor[ CURSOR_INTERACT ]	= ::LoadCursorFromFile("..\\Data\\Cursor\\Interact.ani");
	m_hCursor[ CURSOR_REPAIR ]		= ::LoadCursorFromFile("..\\Data\\Cursor\\Repair.ani");
	m_hCursor[ CURSOR_HOVER ]		= ::LoadCursorFromFile("..\\Data\\Cursor\\Hover.ani");
	m_hCursor[ CURSOR_IDENTIFY ]	= ::LoadCursorFromFile("..\\Data\\Cursor\\Identify.ani");
	m_hCursor[ CURSOR_ADDFRIEND ]	= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_ARROW ));
	m_hCursor[ CURSOR_EXCHANGE ]	= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_ARROW));
	m_hCursor[ CURSOR_CATCH_PET ]	= ::LoadCursor(hInstance, MAKEINTRESOURCE(IDC_ARROW));
}

VOID CCursorMng::MouseCommand_Set(BOOL bHoverInUI, CObject* pSelectObj, const fVector3& fvPos, tActionItem* pActiveSkill)
{
	m_cmdCurrent_Left.m_typeMouse	= SCommand_Mouse::MCT_NULL;
	m_cmdCurrent_Right.m_typeMouse	= SCommand_Mouse::MCT_NULL;
	BOOL bCtrlDown = CInputSystem::GetMe()->IsKeyDown(KC_LCONTROL) || CInputSystem::GetMe()->IsKeyDown(KC_RCONTROL);

	//在UI上空
	if(bHoverInUI)
	{
		if(!pActiveSkill) return;

		switch(pActiveSkill->GetType())
		{
		//修理装备
		case AOT_MOUSECMD_REPARE:
			{
				//左键挂锤子
				m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_REPAIR;
				//右键取消
				m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_CANCLE_REPAIR;		
			}
			break;

		//鉴定装备
		case AOT_MOUSECMD_IDENTIFY:
			{
				//左键鉴定
				m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_UI_USE_IDENTIFY;
				//右键取消
				m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_UI_USE_CANCEL_IDENTIFY;		
			}
			break;

		case AOT_MOUSECMD_ADDFRIEND:
			{
				//左键鉴定
				m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_UI_USE_ADDFRIEND;
				//右键取消
				m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_UI_USE_CANCEL_ADDFRIEND;		
			}
			break;
		case AOT_MOUSECMD_EXCHANGE:
			{
				//左键鉴定
				m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_UI_USE_EXCHANGE;
				//右键取消
				m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_UI_USE_CANCEL_EXCHANGE;		
			}
			break;
		case AOT_MOUSECMD_SALE:
			{
				//左键鉴定
				m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_SALE;
				//右键取消
				m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_CANCEL_SALE;		
			}
			break;
		case AOT_MOUSECMD_BUYMULT:
			{
				//左键鉴定
				m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_BUYMULT;
				//右键取消
				m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_CANCEL_BUYMULT;		
			}
			break;

		default:
			break;
		}
	}
	else
	{

		//计算相应obj鼠标指令
		if ( pSelectObj != NULL )
		{
			pSelectObj->FillMouseCommand_Left( &m_cmdCurrent_Left, pActiveSkill );
			pSelectObj->FillMouseCommand_Right( &m_cmdCurrent_Right, pActiveSkill );
		}

		if ( m_cmdCurrent_Left.m_typeMouse == SCommand_Mouse::MCT_NULL )
		{
			if ( pActiveSkill != NULL )
			{
				switch(pActiveSkill->GetType())
				{
				case AOT_SKILL:
				case AOT_PET_SKILL:
					{
						const void* pSkillImpl = pActiveSkill->GetImpl();
						if(!pSkillImpl)
						{
							m_cmdCurrent_Left.m_typeMouse = bCtrlDown ? SCommand_Mouse::MCT_LEAP : SCommand_Mouse::MCT_PLAYER_MOVETO;
							m_cmdCurrent_Left.m_afParam[0] = fvPos.x;
							m_cmdCurrent_Left.m_afParam[1] = fvPos.z;
							return;
						}

						//选择类型
						ENUM_SELECT_TYPE typeSel = (ENUM_SELECT_TYPE)( pActiveSkill->GetType()==AOT_SKILL ? 
							((SCLIENT_SKILL*)pSkillImpl)->m_pDefine->m_nSelectType : 
							((PET_SKILL*)pSkillImpl)->m_pDefine->m_nSelectType);


						switch(typeSel)
						{
						//点选角色
						case SELECT_TYPE_CHARACTER:
							{
								m_cmdCurrent_Left.m_typeMouse = bCtrlDown ? SCommand_Mouse::MCT_LEAP : SCommand_Mouse::MCT_PLAYER_MOVETO;
								m_cmdCurrent_Left.m_afParam[0] = fvPos.x;
								m_cmdCurrent_Left.m_afParam[1] = fvPos.z;
							}
							break;

						//点选范围
						case SELECT_TYPE_POS:
							{
								//储存技能
								m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_SKILL_AREA;
								m_cmdCurrent_Left.m_apParam[0] = (VOID*)pActiveSkill;
								m_cmdCurrent_Left.m_afParam[1] = fvPos.x;
								m_cmdCurrent_Left.m_afParam[2] = fvPos.z;
								FLOAT fRingRange = (FLOAT)( pActiveSkill->GetType()==AOT_SKILL ? 
									((SCLIENT_SKILL*)pSkillImpl)->m_pDefine->m_fDamageRange : 
								((PET_SKILL*)pSkillImpl)->m_pDefine->m_fDamageRange);
								m_cmdCurrent_Left.m_afParam[5] = fRingRange * 100;

								//取消技能
								m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_CANCEL_SKILL;
							}
							break;
						//方向
						case SELECT_TYPE_DIR:
							{
								//储存技能
								m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_SKILL_DIR;
								m_cmdCurrent_Left.m_apParam[0] = (VOID*)pActiveSkill;
								
								//计算方向
								
								//主角位置
								fVector3 fvPlayer = CObjectManager::GetMe()->GetMySelf()->GetPosition();
								//鼠标位置
								fVector3 fvTarget = fvPos;

								m_cmdCurrent_Left.m_afParam[1] = TDU_GetYAngle(
										fVector2(fvPlayer.x, fvPlayer.z),
										fVector2(fvTarget.x, fvTarget.z));
							}
							break;
						case SELECT_TYPE_SELF:
							//ERROR...
							break;
						}
					}
					break;

				case AOT_ITEM:
					{
						if(!pActiveSkill || pActiveSkill->GetType() != AOT_ITEM) break;
						CObject_Item* pItem = (CObject_Item*)(((CActionItem_Item*)pActiveSkill)->GetItemImpl());
						if(!pItem) break;
						//必须是能够使用的物品
						if(pItem->GetItemClass() != ICLASS_COMITEM) break;

						BOOL bAreaItem = ((CObject_Item_Medicine*)pItem)->IsAreaTargetType();

						m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_USE_ITEM;
						m_cmdCurrent_Left.m_apParam[0] = (VOID*)pActiveSkill;
						m_cmdCurrent_Left.m_adwParam[1] = -1;
						m_cmdCurrent_Left.m_afParam[2] = fvPos.x;
						m_cmdCurrent_Left.m_afParam[3] = fvPos.z;
						m_cmdCurrent_Left.m_adwParam[4] = bAreaItem;

						m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_CANCEL_USE_ITEM;
					}
					break;

				//修理单独处理
				case AOT_MOUSECMD_REPARE:
					{
						//左键挂锤子
						m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_REPAIR;
						//右键取消
						m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_CANCLE_REPAIR;		
					}
					break;

				//鉴定装备
				case AOT_MOUSECMD_IDENTIFY:
					{
						//左键鉴定
						m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_UI_USE_IDENTIFY;
						//右键取消
						m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_UI_USE_CANCEL_IDENTIFY;		
					}
					break;
				case AOT_MOUSECMD_ADDFRIEND:
					{
						//左键鉴定
						m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_UI_USE_ADDFRIEND;
						//右键取消
						m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_UI_USE_CANCEL_ADDFRIEND;		
					}
					break;
				case AOT_MOUSECMD_EXCHANGE:
					{
						//左键鉴定
						m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_UI_USE_EXCHANGE;
						//右键取消
						m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_UI_USE_CANCEL_EXCHANGE;		
					}
					break;
				case AOT_MOUSECMD_CATCHPET:
					{
						m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_CATCH_PET;
						m_cmdCurrent_Left.m_afParam[0] = fvPos.x;
						m_cmdCurrent_Left.m_afParam[1] = fvPos.z;
					}
					break;
				case AOT_MOUSECMD_SALE:
					{
						//左键鉴定
						m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_SALE;
						//右键取消
						m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_CANCEL_SALE;		
					}
					break;
				case AOT_MOUSECMD_BUYMULT:
					{
						//左键
						m_cmdCurrent_Left.m_typeMouse  = SCommand_Mouse::MCT_BUYMULT;
						//右键
						m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_CANCEL_BUYMULT;		
					}
					break;
				default:
					//移动
					{
						m_cmdCurrent_Left.m_typeMouse = SCommand_Mouse::MCT_PLAYER_MOVETO;
						m_cmdCurrent_Left.m_afParam[0] = fvPos.x;
						m_cmdCurrent_Left.m_afParam[1] = fvPos.z;
					}
					break;
				}
			}
		}

		//选择玩家
		if ( m_cmdCurrent_Right.m_typeMouse == SCommand_Mouse::MCT_NULL )
		{
			m_cmdCurrent_Right.m_typeMouse = SCommand_Mouse::MCT_PLAYER_SELECT;
			m_cmdCurrent_Right.m_anParam[0] = INVALID_ID;
		}
	}
}

