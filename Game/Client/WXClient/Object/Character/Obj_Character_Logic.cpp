// Obj_Character_Logic.cpp

#include "StdAfx.h"
#include "..\..\NetWork\NetManager.h"

#include "TDUtil.h"
#include "TDSound.h"
#include "TDTimeSystem.h"
#include "TDDBC_Struct.h"
#include "TDException.h"

#include "SkillDataMgr.h"
#include "CGCharPositionWarp.h"
#include "CGCharSit.h"

#include "..\..\Global.h"
#include "..\..\Common\BuffImpactMgr.h"
#include "..\..\Common\DirectlyImpactMgr.h"
#include "..\..\World\WorldManager.h"
#include "..\..\Action\GMActionSystem.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\DBC\GMDataBase.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Sound\GMSoundSource.h"
#include "..\..\Sound\GMSoundSystem.h"
#include "..\ObjectDef.h"
#include "..\ObjectManager.h"
#include "..\ObjectCommand.h"
#include "..\ObjectCommandDef.h"
#include "..\TripperObj\GMTripperObj_ItemBox.h"
#include "..\Surface\Obj_Effect.h"
#include "..\Surface\Obj_Bullet.h"

#include "Obj_PlayerMySelf.h"

#include "Obj_Character.h"
#include "..\WxRender\RenderSystem.h"

#define OUTMSG(x)		{ADDTALKMSG(x);}

// 压入一条指令
BOOL CObject_Character::PushCommand(const SCommand_Object *pCmd )
{
	switch( pCmd->m_wID )
	{
	case OC_LOGIC_EVENT:
		{
			CObjectManager* pObjectManager = CObjectManager::GetMe();

			_LOGIC_EVENT *pLogicEvent;
			pLogicEvent				= (_LOGIC_EVENT*)(pCmd->m_apParam[0]);

			INT nCreaterID			= pLogicEvent->m_nSenderID;
			INT nCreaterLogicCount	= pLogicEvent->m_nSenderLogicCount;

			CObject* pCreater = (CObject*)(pObjectManager->FindServerObject( nCreaterID ));
			if ( pCreater != NULL && pCreater->IsLogicReady( nCreaterLogicCount ) )
			{
				DoLogicEvent(pLogicEvent);
			}
			else
			{// 如施法者无效或传来的Impact是前面Skill对应的则直接显示
				AddLogicEvent(pLogicEvent);
			}
		}
		break;
	case OC_STOP_ACTION:
	case OC_STOP_MOVE:
	case OC_STOP_SIT:
		{
			CObjectCommand_StopLogic *pStopCommand = (CObjectCommand_StopLogic*)(NewObjectCommand(pCmd));
			//Assert(pStopCommand != NULL && "CObject_Character::PushCommand new stop logic command");
			if(pStopCommand != NULL)
			{
				CObjectCommand_Logic *pLogicCommand = FindLogicCommand(pStopCommand->GetLogicCount());
				if(pLogicCommand != NULL)
				{
					pLogicCommand->Modify(pStopCommand);

					if(pStopCommand->GetCommandID() == OBJECT_COMMAND_STOP_MOVE
						&& pStopCommand->GetLogicCount() == GetLogicCount())
					{
						CheckMoveStop();
					}
				}
				else
				{
					if(pStopCommand->GetCommandID() == OBJECT_COMMAND_STOP_MOVE && pStopCommand->GetLogicCount() == GetLastModifyPosLogicCount())
					{
						if(CharacterLogic_IsStopped() || CharacterLogic_Get() != CHARACTER_LOGIC_MOVE)
						{
							CObjectCommand_StopMove *pStopMoveCommand = (CObjectCommand_StopMove*)pStopCommand;
							fVector2 fvPos(pStopMoveCommand->GetEndPos()->m_fX, pStopMoveCommand->GetEndPos()->m_fZ);

							FLOAT fStopToCurrentDist = TDU_GetDist(fvPos, fVector2(GetPosition().x, GetPosition().z));
							if(fStopToCurrentDist > DEF_CHARACTER_POS_ADJUST_DIST)
							{
								if(CObjectManager::GetMe()->GetMySelf() == this)
								{
									// 当位置差大于某个值时
									if(fStopToCurrentDist > DEF_CLIENT_ADJUST_POS_WARP_DIST)
									{
										// 瞬移到当前服务器对应的位置
										SetMapPosition( fvPos );
									}
									else
									{
										// 调整服务器位置到当前主角客户端对应的位置
										CGCharPositionWarp msgWarp;
										WORLD_POS posCur(GetPosition().x, GetPosition().z);
										WORLD_POS posServer(fvPos.x, fvPos.y);
										msgWarp.setObjID(GetServerID());
										msgWarp.setServerPos(posServer);
										msgWarp.setClientPos(posCur);
										CNetManager::GetMe()->SendPacket(&msgWarp);
									}
								}
								else
								{
									// 瞬移到当前服务器对应的位置
									SetMapPosition( fvPos );
								}
							}
						}
					}
				}
				DeleteObjectCommand(pStopCommand);
			}
		}
		break;
	case OC_ACTION:
	case OC_MOVE:
	case OC_MAGIC_SEND:
	case OC_MAGIC_CHARGE:
	case OC_MAGIC_CHANNEL:
	case OC_ABILITY:
		{
			CObjectCommand_Logic *pLogicCommand = (CObjectCommand_Logic*)(NewObjectCommand(pCmd));
			//Assert(pLogicCommand != NULL && "CObject_Character::PushCommand new logic command");
			if(pLogicCommand != NULL)
			{
				// 丢弃
				if(pLogicCommand->GetLogicCount() <= GetLogicCount())
				{
					DeleteObjectCommand(pLogicCommand);
				}
				else
				{
					CObjectCommand_Logic *pFindCommand = FindLogicCommand(pLogicCommand->GetLogicCount());
					// 没有找到对应的指令才加入，主要是防止与客户端主角的预测指令相冲突
					if(pFindCommand == NULL)
					{
						PushLogicCommand(pLogicCommand);
						//if(GetCurrentLogicCommand() != NULL && pLogicCommand->GetLogicCount() < GetCurrentLogicCommand()->GetLogicCount())
						//{
						//	// 停止客户端预测指令，执行服务器指令
						//	CharacterLogic_Stop(FALSE);
						//}
					}
					else
					{
						DeleteObjectCommand(pLogicCommand);
					}
				}
			}

			if(CharacterLogic_IsStopped())
			{
				ProcessLogicCommand();
			}
		}
		break;
	case OC_MODIFY_ACTION:
		{
			INT nModifyTime, nLogicCount;
			nLogicCount		= pCmd->m_anParam[0];
			nModifyTime		= pCmd->m_anParam[1];
			if(GetLogicCount() == nLogicCount && !CharacterLogic_IsStopped())
			{
				if(CharacterLogic_Get() == CHARACTER_LOGIC_SKILL_GATHER)
				{
					CObjectCommand_MagicCharge *pMagicChargeCommand = (CObjectCommand_MagicCharge*)(GetCurrentLogicCommand());
					if(nModifyTime >= 0)
					{
						UINT uModifyTime = (UINT)nModifyTime;
						if(m_paramLogic_MagicCharge.m_uCurrentTime > uModifyTime)
						{
							m_paramLogic_MagicCharge.m_uCurrentTime -= uModifyTime;
						}
						else
						{
							m_paramLogic_MagicCharge.m_uCurrentTime = 0;
						}
					}
					else
					{
						UINT uModifyTime = (UINT)(abs(nModifyTime));
						if(m_paramLogic_MagicCharge.m_uCurrentTime + uModifyTime > pMagicChargeCommand->GetTotalTime())
						{
							m_paramLogic_MagicCharge.m_uCurrentTime = pMagicChargeCommand->GetTotalTime();
						}
						else
						{
							m_paramLogic_MagicCharge.m_uCurrentTime += uModifyTime;
						}
					}
				}
				else if(CharacterLogic_Get() == CHARACTER_LOGIC_SKILL_LEAD)
				{
					CObjectCommand_MagicChannel *pMagicChannelCommand = (CObjectCommand_MagicChannel*)(GetCurrentLogicCommand());
					if(nModifyTime >= 0)
					{
						UINT uModifyTime = (UINT)nModifyTime;
						if(m_paramLogic_MagicChannel.m_uCurrentTime > uModifyTime)
						{
							m_paramLogic_MagicChannel.m_uCurrentTime -= uModifyTime;
						}
						else
						{
							m_paramLogic_MagicChannel.m_uCurrentTime = 0;
						}
					}
					else
					{
						UINT uModifyTime = (UINT)(abs(nModifyTime));
						if(m_paramLogic_MagicChannel.m_uCurrentTime + uModifyTime > pMagicChannelCommand->GetTotalTime())
						{
							m_paramLogic_MagicChannel.m_uCurrentTime = pMagicChannelCommand->GetTotalTime();
						}
						else
						{
							m_paramLogic_MagicChannel.m_uCurrentTime += uModifyTime;
						}
					}
				}
			}
		}
		break;
	default:
		{
			DoCommand(pCmd);
		}
		break;
	}
	return TRUE;
}

// 处理缓存的指令
BOOL CObject_Character::ProcessLogicCommand( VOID )
{
	FLOAT fSpeed;
	INT nLogicCommandCount = (INT)(m_listLogicCommand.size());
	if(nLogicCommandCount > 0)
	{
		fSpeed = (FLOAT)(nLogicCommandCount) * 0.5f + 1.f;
	}
	else
	{
		fSpeed = 1.f;
	}
	SetLogicSpeed(fSpeed);

	if(CharacterLogic_IsStopped())
	{
		BOOL bResult = DoNextLogicCommand();
		if(!bResult)
		{
			Start_Idle();
		}
	}
	else if(CharacterLogic_Get() == CHARACTER_LOGIC_IDLE)
	{
		DoNextLogicCommand();
	}
	return TRUE;
}

BOOL CObject_Character::DoNextLogicCommand(VOID)
{
	if(!IsLogicCommandListEmpty())
	{
		CObjectCommand_Logic *pLogicCommand = NextLogicCommand();

		BOOL bResult = DoLogicCommand(pLogicCommand);

		if(!bResult)
		{
			DeleteObjectCommand(pLogicCommand);
		}

		return bResult;
	}
	return FALSE;
}

BOOL CObject_Character::DoLogicCommand(CObjectCommand_Logic *pLogicCmd)
{
	BOOL bResult = FALSE;
	switch(pLogicCmd->GetCommandID())
	{
	case OBJECT_COMMAND_ACTION:
		{
			bResult = Start_Action(pLogicCmd);
		}
		break;
	case OBJECT_COMMAND_MOVE:
		{
			bResult = Start_Move(pLogicCmd);
		}
		break;
	case OBJECT_COMMAND_MAGIC_SEND:
		{
			bResult = Start_MagicSend(pLogicCmd);
		}
		break;
	case OBJECT_COMMAND_MAGIC_CHARGE:
		{
			bResult = Start_MagicCharge(pLogicCmd);
		}
		break;
	case OBJECT_COMMAND_MAGIC_CHANNEL:
		{
			bResult = Start_MagicChannel(pLogicCmd);
		}
		break;
	case OBJECT_COMMAND_ABILITY:
		{
			bResult = Start_Ability(pLogicCmd);
		}
		break;
	default:
		//Assert(FALSE && "CObject_Character::DoLogicCommand unknow pLogicCmd id");
		break;
	}
	return bResult;
}
VOID CObject_Character::SetLogicSpeed(FLOAT fSpeed)
{
	if(fabsf(m_fLogic_Speed - fSpeed) > 0.1f)
	{
		m_fLogic_Speed = fSpeed;
		OnLogicSpeedChanged();
	}
}

VOID CObject_Character::OnLogicSpeedChanged(VOID)
{
	ChangeActionSpeed(GetLogicSpeed());
}

RC_RESULT CObject_Character::DoSimulantCommand(const SCommand_Object *pCmd)
{
	CleanupLogicCommandList();
	if(CharacterLogic_Get() == CHARACTER_LOGIC_SKILL_SEND)
	{
		if(!m_paramLogic_MagicSend.m_bCanBreak && !CharacterLogic_IsStopped())
		{
			return RC_WAIT;
		}
	}

	CharacterLogic_Stop(FALSE);
	PushCommand(pCmd);
	return RC_OK;
}

// 立即执行此指令
RC_RESULT CObject_Character::DoCommand( const SCommand_Object *pCmd )
{
	return OnCommand( pCmd );
}

RC_RESULT CObject_Character::OnCommand(const SCommand_Object *pCmd )
{
	RC_RESULT rcResult = RC_SKIP;

	switch ( pCmd->m_wID )
	{
	case OC_TELEPORT:
		{
			fVector3 fvServerPos;
			fvServerPos.x		= pCmd->m_afParam[0];
			fvServerPos.z		= pCmd->m_afParam[1];

			CWorldManager::GetMe()->GetActiveScene()->GetMapHeight(fVector2(fvServerPos.x, fvServerPos.z), fvServerPos.y);

			SetMapPosition( fVector2(fvServerPos.x, fvServerPos.z) );
			rcResult = RC_OK;
		}
		break;
	case OC_JUMP:
		{
			Jump();

			rcResult = RC_OK;
		}
		break;
	case OC_DEATH:
		{
			BOOL bPlayDieAni;
			bPlayDieAni		= pCmd->m_abParam[0];
			BOOL bResult = Start_Dead( bPlayDieAni );
			if ( bResult )
				rcResult = RC_OK;
			else
				rcResult = RC_ERROR;
		}
		break;
	case OC_UPDATE_IMPACT:
		{
			ImpactID_t idImpact;
			BOOL bEnable;
			ObjID_t		nCreatorID;
			idImpact	= (ImpactID_t)(pCmd->m_adwParam[0]);
			bEnable		= pCmd->m_abParam[1];
			nCreatorID	= pCmd->m_anParam[2];
			ChangeImpact( idImpact, nCreatorID, bEnable, FALSE, TRUE );
			rcResult = RC_OK;
		}
		break;
	case OC_UPDATE_IMPACT_EX:
		{
			ImpactID_t idImpact;
			idImpact	= (ImpactID_t)(pCmd->m_adwParam[0]);
			ChangeImpact( idImpact, INVALID_ID, TRUE, FALSE, FALSE );
			rcResult = RC_OK;
		}
		break;
	case OC_LEVEL_UP:
		{
			CHAR szText[128];
			sprintf(szText, "%d;0", (INT)(BASE_ACTION_LEVEL_UP));
			SetChatMoodAction(STRING(szText));
			rcResult = RC_OK;
		}
		break;

	default:
		rcResult = RC_SKIP;
		break;
	}

	return rcResult;
}

BOOL CObject_Character::IsLogicCommandListEmpty(VOID)
{
	return m_listLogicCommand.empty();
}

BOOL CObject_Character::PushLogicCommand(CObjectCommand_Logic *pCmd)
{
	INT nLogicCount = pCmd->GetLogicCount();
	if(!m_listLogicCommand.empty())
	{
		CObjectLogicCommandList::iterator itCur, itEnd;
		CObjectCommand_Logic *pCommand;
		itCur = m_listLogicCommand.begin();
		itEnd = m_listLogicCommand.end();
		while( itCur != itEnd )
		{
			pCommand = *itCur;
			if(pCommand->GetLogicCount() > nLogicCount)
			{
				m_listLogicCommand.insert(itCur, pCmd);
				return TRUE;
			}
			itCur++;
		}
	}
	m_listLogicCommand.push_back(pCmd);
	return TRUE;
}

const CObjectCommand_Logic *CObject_Character::GetNextLogicCommand(VOID)const
{
	if(!m_listLogicCommand.empty())
	{
		CObjectCommand_Logic *pCmd = m_listLogicCommand.front();
		return pCmd;
	}
	return NULL;
}

CObjectCommand_Logic *CObject_Character::NextLogicCommand(VOID)
{
	if(!m_listLogicCommand.empty())
	{
		CObjectCommand_Logic *pCmd = m_listLogicCommand.front();
		m_listLogicCommand.pop_front();
		return pCmd;
	}
	return NULL;
}

CObjectCommand_Logic *CObject_Character::FindLogicCommand(INT nLogicCount)
{
	// 当前正在执行的指令
	if(m_pCurrentLogicCommand != NULL && m_pCurrentLogicCommand->GetLogicCount() == nLogicCount)
	{
		return m_pCurrentLogicCommand;
	}

	// 缓存的指令
	CObjectLogicCommandList::iterator itCur, itEnd;
	CObjectCommand_Logic *pCommand;
	itCur = m_listLogicCommand.begin();
	itEnd = m_listLogicCommand.end();
	while ( itCur != itEnd )
	{
		pCommand = *itCur;
		itCur++;
		if(pCommand->GetLogicCount() == nLogicCount)
		{
			return pCommand;
		}
	}
	return NULL;
}

BOOL CObject_Character::CleanupLogicCommandList( VOID )
{
	CObjectLogicCommandList::iterator itCur, itEnd;
	CObjectCommand_Logic *pCommand;
	itCur = m_listLogicCommand.begin();
	itEnd = m_listLogicCommand.end();
	while ( itCur != itEnd )
	{
		pCommand = *itCur;
		itCur++;
		DeleteObjectCommand(pCommand);
	}
	m_listLogicCommand.erase( m_listLogicCommand.begin(), m_listLogicCommand.end() );
	return TRUE;
}

BOOL CObject_Character::IsLogicReady( INT nLogicCount )const
{
	if(GetLogicCount() > nLogicCount)
	{
		return TRUE;
	}
	else if(GetLogicCount() == nLogicCount)
	{
		if(CharacterLogic_Get() == CHARACTER_LOGIC_SKILL_SEND)
		{
			if(IsBeAttackEffectShowed())
			{
				return TRUE;
			}
		}
		else
		{
			return TRUE;
		}
	}
	return FALSE;
}

VOID CObject_Character::CharacterLogic_Set( ENUM_CHARACTER_LOGIC eLogic )
{
	m_bCharacterLogicStopped	= FALSE;
	m_eCharacterLogic			= eLogic;
}

VOID CObject_Character::CharacterLogic_Stop(BOOL bFinished)
{
	SetCurrentLogicCommand(NULL);

	CharacterLogic_OnStopped(bFinished);
}

VOID CObject_Character::CharacterLogic_OnStopped(BOOL bFinished)
{
	m_bCharacterLogicStopped = TRUE;

	if(GetCharacterData()->IsSit())
	{
		StandUp();
	}

	//设置进度条
	if ( CObjectManager::GetMe()->GetMySelf() == this )
	{
		CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_HIDE);
	}

	if(CharacterLogic_Get() == CHARACTER_LOGIC_ABILITY_ACTION)
	{
		if(m_pLifeAbilitySound != NULL)
		{
			//停止生活技能声音
			m_pLifeAbilitySound->Stop();
			CGameProcedure::s_pSoundSystem->Source_Destroy(m_pLifeAbilitySound);
			m_pLifeAbilitySound = 0;
		}
	}
}

CObjectCommand_Logic *CObject_Character::GetCurrentLogicCommand(VOID)
{
	return m_pCurrentLogicCommand;
}

VOID CObject_Character::SetCurrentLogicCommand(CObjectCommand_Logic *pLogicCommand)
{
	if(m_pCurrentLogicCommand != NULL)
	{
		DeleteObjectCommand(m_pCurrentLogicCommand);
	}
	m_pCurrentLogicCommand = pLogicCommand;
}

VOID CObject_Character::CheckMoveStop(VOID)
{
	if(CharacterLogic_Get() != CHARACTER_LOGIC_MOVE)
		return ;

	CObjectCommand_Move *pMoveCommand = (CObjectCommand_Move*)GetCurrentLogicCommand();
	if(pMoveCommand != NULL && pMoveCommand->GetNodeCount() > 0)
	{
		// 主角不走回头路，不瞬移
		BOOL bAdjustPos = FALSE;
		WORLD_POS posAdjust;
		if(m_paramLogic_Move.m_nCurrentNodeIndex >= pMoveCommand->GetNodeCount())
		{
			const WORLD_POS *paPos = pMoveCommand->GetNodeList();
			INT nEndNodeIndex = pMoveCommand->GetNodeCount() - 1;
			posAdjust = paPos[nEndNodeIndex];

			bAdjustPos	= TRUE;
		}
		else if(m_paramLogic_Move.m_nCurrentNodeIndex == pMoveCommand->GetNodeCount() - 1)
		{
			const WORLD_POS *paPos = pMoveCommand->GetNodeList();
			INT nEndNodeIndex = pMoveCommand->GetNodeCount() - 1;
			WORLD_POS posCommandTarget = paPos[nEndNodeIndex];

			FLOAT fLenCSTarget = fabsf(m_paramLogic_Move.m_posSaveTarget.m_fX - posCommandTarget.m_fX)
				+ fabsf(m_paramLogic_Move.m_posSaveTarget.m_fZ - posCommandTarget.m_fZ);

			// 目标点变动了
			if(fLenCSTarget > 0.01f)
			{
				FLOAT fSaveToServerDist = TDU_GetDist(fVector2(m_paramLogic_Move.m_posSaveTarget.m_fX, m_paramLogic_Move.m_posSaveTarget.m_fZ),
					fVector2(posCommandTarget.m_fX, posCommandTarget.m_fZ));

				FLOAT fSaveToCurrentDist = TDU_GetDist(fVector2(m_paramLogic_Move.m_posSaveTarget.m_fX, m_paramLogic_Move.m_posSaveTarget.m_fZ),
					fVector2(GetPosition().x, GetPosition().z));

				// 这里忽略了服务器传过来的目标点不在路径上的情况
				if(fSaveToServerDist - fSaveToCurrentDist >= 0.0f)
				{
					posAdjust = posCommandTarget;
					bAdjustPos = TRUE;
				}

				m_paramLogic_Move.m_posSaveTarget.m_fX	= posCommandTarget.m_fX;
				m_paramLogic_Move.m_posSaveTarget.m_fZ	= posCommandTarget.m_fZ;
			}
		}

		if(bAdjustPos)
		{
			if(CObjectManager::GetMe()->GetMySelf() == this)
			{
				// 当位置差大于某个值时
				FLOAT fAdjustToCurrentDist = TDU_GetDist(fVector2(posAdjust.m_fX, posAdjust.m_fZ),
					fVector2(GetPosition().x, GetPosition().z));

				if(fAdjustToCurrentDist > DEF_CLIENT_ADJUST_POS_WARP_DIST)
				{
					// 瞬移到当前服务器对应的位置
					fVector2 fvPos(posAdjust.m_fX, posAdjust.m_fZ);
					SetMapPosition( fvPos );
				}
				else
				{
					// 调整服务器位置到当前主角客户端对应的位置
					CGCharPositionWarp msgWarp;
					WORLD_POS posCur(GetPosition().x, GetPosition().z);
					msgWarp.setObjID(GetServerID());
					msgWarp.setServerPos(posAdjust);
					msgWarp.setClientPos(posCur);
					CNetManager::GetMe()->SendPacket(&msgWarp);
				}
			}
			else
			{
				FLOAT fAdjustToCurrentDist = TDU_GetDist(fVector2(posAdjust.m_fX, posAdjust.m_fZ),
					fVector2(GetPosition().x, GetPosition().z));

				if(fAdjustToCurrentDist > DEF_CHARACTER_POS_ADJUST_DIST)
				{
					// 瞬移到当前服务器对应的位置
					fVector2 fvPos(posAdjust.m_fX, posAdjust.m_fZ);
					SetMapPosition( fvPos );
				}
			}
			CharacterLogic_Stop(TRUE);
		}
	}
}

BOOL CObject_Character::Jump( VOID )
{
	if(IsDie())
	{
		return FALSE;
	}

	CCharacterData *pCharacterData = GetCharacterData();
	if(pCharacterData == NULL)
	{
		return FALSE;
	}

	if(pCharacterData->Get_ModelID() != INVALID_ID)
	{
		return FALSE;
	}

	if(GetCharacterType() == CT_PLAYERMYSELF || GetCharacterType() == CT_PLAYEROTHER)
	{
		if(pCharacterData->Get_TeamFollowFlag())
			return FALSE;

		if(pCharacterData->Get_IsInStall())
			return FALSE;
	}

	INT nBaseAction;
	nBaseAction = BASE_ACTION_N_JUMP_R;
	
	if (CharacterLogic_Get() != CHARACTER_LOGIC_MOVE)
	{
		nBaseAction = BASE_ACTION_N_JUMP_N;
	}
	else
	{
		if ( GetCharacterData()->Get_MountID() == -1 )
		{
			INT nRet = rand() % 4;
			if (nRet == 0)
				nBaseAction = BASE_ACTION_N_JUMP_R;
			else
				nBaseAction = BASE_ACTION_N_JUMP_N;
		}
		else
		{
			nBaseAction = BASE_ACTION_N_JUMP_R;
		}
	}

	//ChangeAction( GetActionNameByActionSetID( nBaseAction, GetWeaponType() ), 1.f, FALSE );
	ChangeAction( nBaseAction, 1.f, FALSE );

	if(CharacterLogic_Get() == CHARACTER_LOGIC_MOVE||GetCharacterData()->Get_MountID() == -1)
	{
		SetbJumping(TRUE);
		RegisterPhyEvent(PE_COLLISION_WITH_GROUND);
		//由物理层去计算
		PhyEnable(TRUE);
		FLOAT fTEST = 12.0; 
		AddLinearSpeed(fVector3(0.0, fTEST, 0.0));
	}

	StandUp();
	return TRUE;
}

BOOL CObject_Character::SitDown(VOID)
{
	if(IsDie())
	{
		return FALSE;
	}

	CCharacterData *pCharacterData = GetCharacterData();
	if(pCharacterData == NULL)
	{
		return FALSE;
	}

	if(pCharacterData->Get_MountID() != INVALID_ID)
	{
		return FALSE;
	}

	if(pCharacterData->Get_ModelID() != INVALID_ID)
	{
		return FALSE;
	}

	if(GetCharacterType() == CT_PLAYERMYSELF || GetCharacterType() == CT_PLAYEROTHER)
	{
		if(pCharacterData->Get_TeamFollowFlag())
			return FALSE;

		if(pCharacterData->Get_IsInStall())
			return FALSE;
	}

	if (CharacterLogic_Get() == CHARACTER_LOGIC_IDLE && GetCharacterData() != NULL)
	{
		if(!GetCharacterData()->IsSit() && GetCharacterData()->Get_MountID() == INVALID_ID)
		{
			GetCharacterData()->Set_Sit(TRUE);
			if(CObjectManager::GetMe()->GetMySelf() == this)
			{
				CGCharSit cmdSit;
				cmdSit.setObjID(GetServerID());
				cmdSit.setSit(GetCharacterData()->IsSit());

				CNetManager::GetMe()->SendPacket(&cmdSit);
			}
		}
	}
	return TRUE;
}

BOOL CObject_Character::StandUp(VOID)
{
	if(IsDie())
	{
		return FALSE;
	}

	CCharacterData *pCharacterData = GetCharacterData();
	if(pCharacterData == NULL)
	{
		return FALSE;
	}

	if(pCharacterData->Get_MountID() != INVALID_ID)
	{
		return FALSE;
	}

	if(pCharacterData->Get_ModelID() != INVALID_ID)
	{
		return FALSE;
	}

	if(GetCharacterType() == CT_PLAYERMYSELF || GetCharacterType() == CT_PLAYEROTHER)
	{
		if(pCharacterData->Get_TeamFollowFlag())
			return FALSE;

		if(pCharacterData->Get_IsInStall())
			return FALSE;
	}

	if(GetCharacterData()->IsSit())
	{
		GetCharacterData()->Set_Sit(FALSE);
		if(CObjectManager::GetMe()->GetMySelf() == this)
		{
			CGCharSit cmdSit;
			cmdSit.setObjID(GetServerID());
			cmdSit.setSit(GetCharacterData()->IsSit());

			CNetManager::GetMe()->SendPacket(&cmdSit);
		}
	}
	return TRUE;
}

VOID CObject_Character::OnMoveStop(VOID)
{
	if ( CharacterLogic_Get() == CHARACTER_LOGIC_MOVE )
		CharacterLogic_Stop(TRUE);
}

BOOL CObject_Character::OnAnimationEnd(LPCTSTR szAnimationName)
{
    FreshAnimation();
	return true;
}

BOOL CObject_Character::OnAnimationCanBreak(LPCTSTR szAnimationName)
{
	if(CharacterLogic_Get() == CHARACTER_LOGIC_SKILL_SEND)
	{
		m_paramLogic_MagicSend.m_bCanBreak	= TRUE;
	}
	//if(!IsLogicCommandListEmpty())
	//{
	//	CharacterLogic_Stop(TRUE);
	//}
	return true;
}

BOOL CObject_Character::OnAnimationHit(LPCTSTR szAnimationName)
{
	static INT nHitCount; // 记录连击招的第几次Hit
	if ( CharacterLogic_Get() == CHARACTER_LOGIC_SKILL_SEND )
	{
		CObjectCommand_MagicSend *pMagicSendCommand = (CObjectCommand_MagicSend*)GetCurrentLogicCommand();
		const SSkillData *pClientSkill = GetSkillDataMgr()->GetSkillData(m_paramLogic_MagicSend.m_nSaveMagicID);
		if ( pClientSkill != NULL && pMagicSendCommand != NULL )
		{
			//设置当前目标位置数据
			INT idTarget = pMagicSendCommand->GetTargetObjID();
			CObject* pTarget = (CObject*)(CObjectManager::GetMe()->FindServerObject( idTarget ));
			if (pTarget != NULL) 
			{
				BOOL bShowAll = FALSE;
				if (pClientSkill->m_nHitsOrINTerval > 0)
				{// 如果是连招
					++nHitCount;
					if (nHitCount == pClientSkill->m_nHitsOrINTerval)
					{// 如果是最后一招
						m_paramLogic_MagicSend.m_bBeAttackEffectShowed = TRUE;	
						bShowAll = TRUE;
						nHitCount = 0;
					}
				}
				else
				{
					m_paramLogic_MagicSend.m_bBeAttackEffectShowed = TRUE;
				}
			}
			else
			{
				m_paramLogic_MagicSend.m_bBeAttackEffectShowed = TRUE;
			}
		}

	}

	return TRUE;
}

VOID CObject_Character::FreshAnimation( VOID )
{
	SetAnimationEnd(TRUE);
	switch ( CharacterLogic_Get() )
	{
	case CHARACTER_LOGIC_IDLE:
		{
			BOOL bPlayIdleMotion = FALSE;
			UINT nTimeNow = CGameProcedure::s_pTimeSystem->GetTimeNow();
			if(m_paramLogic_Idle.m_uLastIdleMotionTime + m_paramLogic_Idle.m_uIdleInterval < nTimeNow)
			{
				bPlayIdleMotion = TRUE;
				m_paramLogic_Idle.m_uLastIdleMotionTime	= nTimeNow;
				m_paramLogic_Idle.m_uIdleInterval		= CalcIdleRandInterval();
			}

			if(IsHaveChatMoodAction())
			{
				if(GetCharacterData()->IsSit())
				{
					StandUp();
				}
				else
				{
					PlayChatMoodAction();
				}
			}
			else
			{
				BOOL bFightState, bSit;
				INT nBaseAction;
				bFightState	= IsFightState();
				bSit = GetCharacterData()->IsSit();
				if(bSit)
				{
					nBaseAction = BASE_ACTION_N_SIT_IDLE;
				}
				else
				{
					if(bFightState)
					{
						nBaseAction = BASE_ACTION_F_IDLE;
					}
					else
					{
						if(bPlayIdleMotion)
						{
							nBaseAction = rand()%2 + BASE_ACTION_N_IDLE_EX0;
						}
						else
						{
							nBaseAction = BASE_ACTION_N_IDLE;
						}
					}
				}
				ChangeAction(nBaseAction, GetLogicSpeed(), FALSE);
			}
		}
		break;
	case CHARACTER_LOGIC_ACTION:
		break;
	case CHARACTER_LOGIC_MOVE:
		{
			BOOL bFightState;
			INT nBaseAction;
			bFightState	= IsFightState();
			if ( bFightState )
			{
				nBaseAction	= BASE_ACTION_F_RUN;
			}
			else
			{
				nBaseAction	= BASE_ACTION_N_RUN;
			}
			ChangeAction( nBaseAction, GetLogicSpeed()/*GetCharacterData()->Get_SpeedRate()*/, TRUE );
		}
		break;
	case CHARACTER_LOGIC_SKILL_GATHER:
		break;
	case CHARACTER_LOGIC_SKILL_LEAD:
		break;
	case CHARACTER_LOGIC_SKILL_SEND:
		{
			if(m_paramLogic_MagicSend.m_bDoNextAction_Concatenation)
			{
				m_paramLogic_MagicSend.m_bDoNextAction_Concatenation = FALSE;
				const SSkillData *pClientSkill = GetSkillDataMgr()->GetSkillData(m_paramLogic_MagicSend.m_nSaveMagicID);
				if ( pClientSkill != NULL )
				{
					// 如果是连招的特殊处理
					BOOL bPlayAction = TRUE;
					INT nSkillActionType = pClientSkill->m_nSkillActionType;
					switch( nSkillActionType )
					{
					case SKILL_ACTION_TYPE_CONCATENATION:
						{
							m_paramLogic_MagicSend.m_nActionIndex++;
							if(m_paramLogic_MagicSend.m_nActionIndex >= m_paramLogic_MagicSend.m_nActionCount)
							{
								m_paramLogic_MagicSend.m_nActionIndex = 0;
							}
							bPlayAction = TRUE;
						}
						break;
					case SKILL_ACTION_TYPE_CONCATENATION_EX:
						{
							m_paramLogic_MagicSend.m_nActionIndex++;
							if ( m_paramLogic_MagicSend.m_nActionIndex >= m_paramLogic_MagicSend.m_nActionCount - 1 )
							{
								m_paramLogic_MagicSend.m_nActionIndex = 1;
							}
							bPlayAction = TRUE;
						}
						break;
					case SKILL_ACTION_TYPE_NONE:
					default:
						break;
					}

					if(bPlayAction)
					{
						INT nRandAnimID = GetSkillDataMgr()->GetRandAnim(m_paramLogic_MagicSend.m_nSaveMagicID, m_paramLogic_MagicSend.m_nActionIndex, TRUE);
						ChangeAction( nRandAnimID, GetLogicSpeed(), FALSE );
					}
				}
			}
		}
		break;
	case CHARACTER_LOGIC_ABILITY_ACTION:
		break;
	case CHARACTER_LOGIC_DEAD:
		ChangeAction( BASE_ACTION_F_CADAVER, GetLogicSpeed(), FALSE, 0.0f );
		break;
	case CHARACTER_LOGIC_STALL:
		if(m_paramLogic_Stall.m_nCurrentAnimation == SLogicParam_Stall::STALL_ANIMATION_IDLE)
		{
			ChangeAction( BASE_ACTION_N_SIT_IDLE, GetLogicSpeed(), TRUE);
		}
		break;
	default:
		break;
	}
}

BOOL CObject_Character::Start_Idle(VOID)
{
	if(m_bJumping)
	{
	}
	else if(IsHaveChatMoodAction())
	{
		PlayChatMoodAction();
	}
	else
	{
		BOOL bFightState, bSit;
		INT nBaseAction;
		bFightState	= IsFightState();
		bSit = GetCharacterData()->IsSit();
		if(bSit)
		{
			nBaseAction = BASE_ACTION_N_SIT_IDLE;
		}
		else
		{
			nBaseAction = (bFightState)?(BASE_ACTION_F_IDLE):(BASE_ACTION_N_IDLE);
		}

		ChangeAction( nBaseAction, GetLogicSpeed(), FALSE );
	}

	m_paramLogic_Idle.m_uIdleInterval		= CalcIdleRandInterval();
	m_paramLogic_Idle.m_uLastIdleMotionTime	= CGameProcedure::s_pTimeSystem->GetTimeNow();
	m_paramLogic_Idle.m_uStartTime			= CGameProcedure::s_pTimeSystem->GetTimeNow();

	CharacterLogic_Set( CHARACTER_LOGIC_IDLE );

	return TRUE;
}

BOOL CObject_Character::Start_Action(CObjectCommand_Logic *pLogicCommand)
{
	//Assert(!CharacterLogic_IsStopped() && "CObject_Character::Start_Action");

	if(pLogicCommand == NULL)
		return FALSE;

	if(pLogicCommand->GetCommandID() != OBJECT_COMMAND_ACTION)
		return FALSE;

	CObjectCommand_Action *pActionCommand = (CObjectCommand_Action*)pLogicCommand;
	if(pActionCommand->GetActionID() == INVALID_ID)
		return FALSE;

	if ( CharacterLogic_Get() == CHARACTER_LOGIC_DEAD )
		return FALSE;

	if ( !m_bJumping )
	{
		ChangeAction( pActionCommand->GetActionID(), GetLogicSpeed(), FALSE );
	}

	SetCurrentLogicCommand(pLogicCommand);
	SetLogicCount(pLogicCommand->GetLogicCount());

	CharacterLogic_Set( CHARACTER_LOGIC_ACTION );

	return TRUE;
}

BOOL CObject_Character::Start_Move(CObjectCommand_Logic *pLogicCommand)
{
	//Assert(!CharacterLogic_IsStopped() && "CObject_Character::Start_Move");

	if(pLogicCommand == NULL)
		return FALSE;

	if(pLogicCommand->GetCommandID() != OBJECT_COMMAND_MOVE)
		return FALSE;

	CObjectCommand_Move *pMoveCommand = (CObjectCommand_Move*)pLogicCommand;
	if(pMoveCommand->GetNodeCount() <= 0)
		return FALSE;

	if ( CharacterLogic_Get() == CHARACTER_LOGIC_DEAD )
		return FALSE;


	ENUM_CHARACTER_LOGIC ePrevLogic = CharacterLogic_Get();
	if ( CHARACTER_LOGIC_MOVE != ePrevLogic )
	{
		if ( !m_bJumping )
		{
			BOOL bFightState;
			INT nBaseAction;
			bFightState	= IsFightState();
			if ( bFightState )
			{
				nBaseAction	= BASE_ACTION_F_RUN;
			}
			else
			{
				nBaseAction	= BASE_ACTION_N_RUN;
			}
			ChangeAction( nBaseAction, GetLogicSpeed()/*GetCharacterData()->Get_SpeedRate()*/, true );
		}
	}

	SetCurrentLogicCommand(pLogicCommand);
	SetLogicCount(pLogicCommand->GetLogicCount());

	SetLastModifyPosLogicCount(pLogicCommand->GetLogicCount());

	const WORLD_POS *pNodeList = pMoveCommand->GetNodeList();
	m_paramLogic_Move.m_nCurrentNodeIndex	= 0;
	m_paramLogic_Move.m_posSaveStart		= WORLD_POS(GetPosition().x, GetPosition().z);
	m_paramLogic_Move.m_posSaveTarget		= pNodeList[0];

	CharacterLogic_Set( CHARACTER_LOGIC_MOVE );

	return TRUE;
}

BOOL CObject_Character::Start_MagicCharge(CObjectCommand_Logic *pLogicCommand)
{
	//Assert(!CharacterLogic_IsStopped() && "CObject_Character::Start_MagicCharge");

	if(pLogicCommand == NULL)
		return FALSE;

	if(pLogicCommand->GetCommandID() != OBJECT_COMMAND_MAGIC_CHARGE)
		return FALSE;

	if ( CharacterLogic_Get() == CHARACTER_LOGIC_DEAD )
		return FALSE;

	CObjectCommand_MagicCharge *pMagicChargeCommand = (CObjectCommand_MagicCharge*)pLogicCommand;
	if(pMagicChargeCommand->GetTotalTime() == 0)
		return FALSE;

	const SSkillData *pClientSkill = GetSkillDataMgr()->GetSkillData(pMagicChargeCommand->GetMagicID());
	if ( pClientSkill == NULL )
		return FALSE;

	switch ( pClientSkill->m_nSelectType )
	{
	case SELECT_TYPE_CHARACTER:
	case SELECT_TYPE_POS:
		{
			BOOL bCalcDir;
			fVector2 fvTarget;
			bCalcDir = FALSE;
			if ( pMagicChargeCommand->GetTargetObjID() != INVALID_ID )
			{
				if ( pMagicChargeCommand->GetTargetObjID() != GetServerID() )
				{
					CObjectManager* pObjectManager = CObjectManager::GetMe();
					CObject* pTarget = (CObject*)(pObjectManager->FindServerObject( pMagicChargeCommand->GetTargetObjID() ));
					if ( pTarget != NULL )
					{
						fvTarget.x = pTarget->GetPosition().x;
						fvTarget.y = pTarget->GetPosition().z;
						bCalcDir = TRUE;
					}
				}
			}
			else
			{
				fvTarget.x = pMagicChargeCommand->GetTargetPos()->m_fX;
				fvTarget.y = pMagicChargeCommand->GetTargetPos()->m_fZ;
				bCalcDir = TRUE;
			}
			if ( bCalcDir )
			{
				FLOAT fDir;
				fVector2 fvThis;
				fvThis.x = GetPosition().x;
				fvThis.y = GetPosition().z;

				fDir = TDU_GetYAngle( fvThis, fvTarget );
				SetFaceDir( fDir );
			}
		}
		break;
	case SELECT_TYPE_DIR:
		SetFaceDir(pMagicChargeCommand->GetTargetDir());
		break;
	case SELECT_TYPE_NONE:
		break;
	}

	INT nRandAnimID = GetSkillDataMgr()->GetRandAnim(pMagicChargeCommand->GetMagicID(), 0, FALSE);
	if ( nRandAnimID != -1 )
	{
		ChangeAction( nRandAnimID, GetLogicSpeed(), TRUE );
	}

	if ( CObjectManager::GetMe()->GetMySelf() == this )
	{
		CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_SHOW, pClientSkill->m_lpszName);
		CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_WIDTH, 0);
	}

	SetCurrentLogicCommand(pLogicCommand);
	SetLogicCount(pLogicCommand->GetLogicCount());

	m_paramLogic_MagicCharge.m_uCurrentTime		= 0;

	if(pClientSkill->m_nFriendness < 0)
	{
		SetFightState(TRUE);
	}
	CharacterLogic_Set( CHARACTER_LOGIC_SKILL_GATHER );

	return TRUE;
}

BOOL CObject_Character::Start_MagicChannel(CObjectCommand_Logic *pLogicCommand)
{
	//Assert(!CharacterLogic_IsStopped() && "CObject_Character::Start_MagicChannel");

	if(pLogicCommand == NULL)
		return FALSE;

	if(pLogicCommand->GetCommandID() != OBJECT_COMMAND_MAGIC_CHANNEL)
		return FALSE;

	if ( CharacterLogic_Get() == CHARACTER_LOGIC_DEAD )
		return FALSE;

	CObjectCommand_MagicChannel *pMagicChannelCommand = (CObjectCommand_MagicChannel*)pLogicCommand;
	if(pMagicChannelCommand->GetTotalTime() == 0)
		return FALSE;

	const SSkillData *pClientSkill = GetSkillDataMgr()->GetSkillData(pMagicChannelCommand->GetMagicID());
	if ( pClientSkill == NULL )
		return FALSE;

	switch ( pClientSkill->m_nSelectType )
	{
	case SELECT_TYPE_CHARACTER:
	case SELECT_TYPE_POS:
		{
			BOOL bCalcDir;
			fVector2 fvTarget;
			bCalcDir = FALSE;
			if ( pMagicChannelCommand->GetTargetObjID() != INVALID_ID )
			{
				if ( pMagicChannelCommand->GetTargetObjID() != GetServerID() )
				{
					CObjectManager* pObjectManager = CObjectManager::GetMe();
					CObject* pTarget = (CObject*)(pObjectManager->FindServerObject( pMagicChannelCommand->GetTargetObjID() ));
					if ( pTarget != NULL )
					{
						fvTarget.x = pTarget->GetPosition().x;
						fvTarget.y = pTarget->GetPosition().z;
						bCalcDir = TRUE;
					}
				}
			}
			else
			{
				fvTarget.x = pMagicChannelCommand->GetTargetPos()->m_fX;
				fvTarget.y = pMagicChannelCommand->GetTargetPos()->m_fZ;
				bCalcDir = TRUE;
			}
			if ( bCalcDir )
			{
				FLOAT fDir;
				fVector2 fvThis;
				fvThis.x = GetPosition().x;
				fvThis.y = GetPosition().z;

				fDir = TDU_GetYAngle( fvThis, fvTarget );
				SetFaceDir( fDir );
			}
		}
		break;
	case SELECT_TYPE_DIR:
		SetFaceDir(pMagicChannelCommand->GetTargetDir());
		break;
	case SELECT_TYPE_NONE:
		break;
	}

	INT nRandAnimID = GetSkillDataMgr()->GetRandAnim(pMagicChannelCommand->GetMagicID(), 0, FALSE);
	if ( nRandAnimID != -1 )
	{
		ChangeAction( nRandAnimID, GetLogicSpeed(), TRUE );
	}

	if ( CObjectManager::GetMe()->GetMySelf() == this )
	{
		CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_SHOW, pClientSkill->m_lpszName);
		CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_WIDTH, 1.f);
	}

	SetCurrentLogicCommand(pLogicCommand);
	SetLogicCount(pLogicCommand->GetLogicCount());

	m_paramLogic_MagicChannel.m_uCurrentTime		= pMagicChannelCommand->GetTotalTime();

	if(pClientSkill->m_nFriendness < 0)
	{
		SetFightState(TRUE);
	}
	CharacterLogic_Set( CHARACTER_LOGIC_SKILL_LEAD );

	return TRUE;
}

BOOL CObject_Character::Start_MagicSend(CObjectCommand_Logic *pLogicCommand)
{
	if(pLogicCommand == NULL)
		return FALSE;

	if(pLogicCommand->GetCommandID() != OBJECT_COMMAND_MAGIC_SEND)
		return FALSE;

	if ( CharacterLogic_Get() == CHARACTER_LOGIC_DEAD )
		return FALSE;

	CObjectCommand_MagicSend *pMagicSendCommand = (CObjectCommand_MagicSend*)pLogicCommand;

	const SSkillData *pClientSkill = GetSkillDataMgr()->GetSkillData(pMagicSendCommand->GetMagicID());
	if ( pClientSkill == NULL )
		return FALSE;

	switch ( pClientSkill->m_nSelectType )
	{
	case SELECT_TYPE_CHARACTER:
	case SELECT_TYPE_POS:
		{
			BOOL bCalcDir;
			fVector2 fvTarget;
			bCalcDir = FALSE;
			if ( pMagicSendCommand->GetTargetObjID() != INVALID_ID )
			{
				if ( pMagicSendCommand->GetTargetObjID() != GetServerID() )
				{
					CObjectManager* pObjectManager = CObjectManager::GetMe();
					CObject* pTarget = (CObject*)(pObjectManager->FindServerObject( pMagicSendCommand->GetTargetObjID() ));
					if ( pTarget != NULL )
					{
						fvTarget.x = pTarget->GetPosition().x;
						fvTarget.y = pTarget->GetPosition().z;
						bCalcDir = TRUE;
					}
				}
			}
			else
			{
				fvTarget.x = pMagicSendCommand->GetTargetPos()->m_fX;
				fvTarget.y = pMagicSendCommand->GetTargetPos()->m_fZ;
				bCalcDir = TRUE;
			}
			if ( bCalcDir )
			{
				FLOAT fDir;
				fVector2 fvThis;
				fvThis.x = GetPosition().x;
				fvThis.y = GetPosition().z;

				fDir = TDU_GetYAngle( fvThis, fvTarget );
				SetFaceDir( fDir );
			}
		}
		break;
	case SELECT_TYPE_DIR:
		SetFaceDir(pMagicSendCommand->GetTargetDir());
		break;
	case SELECT_TYPE_NONE:
		break;
	}

	BOOL bOldSkillID;
	if (CharacterLogic_Get() == CHARACTER_LOGIC_SKILL_SEND
		&& m_paramLogic_MagicSend.m_nSaveMagicID == pMagicSendCommand->GetMagicID())
	{
		bOldSkillID = TRUE;
	}
	else
	{
		bOldSkillID = FALSE;
	}

	m_paramLogic_MagicSend.m_nSaveMagicID					= pMagicSendCommand->GetMagicID();
	m_paramLogic_MagicSend.m_bDoNextAction_Concatenation	= FALSE;
	m_paramLogic_MagicSend.m_bBeAttackEffectShowed			= FALSE;
	m_paramLogic_MagicSend.m_bCanBreak						= FALSE;
	m_paramLogic_MagicSend.m_nActionCount					= GetSkillDataMgr()->GetAnimCount(pMagicSendCommand->GetMagicID());
	m_paramLogic_MagicSend.m_uAnimationTime					= 0;
	m_paramLogic_MagicSend.m_uAnimationEndElapseTime		= 0;

	// 如果是连招的特殊处理
	BOOL bPlayAction = TRUE;
	INT nSkillActionType = pClientSkill->m_nSkillActionType;
	switch( nSkillActionType )
	{
	case SKILL_ACTION_TYPE_CONCATENATION:
		{
			if ( bOldSkillID )
			{
				m_paramLogic_MagicSend.m_nActionIndex++;
				if(m_paramLogic_MagicSend.m_nActionIndex >= m_paramLogic_MagicSend.m_nActionCount)
				{
					m_paramLogic_MagicSend.m_nActionIndex = 0;
				}
				bPlayAction = FALSE;
			}
			else
			{
				m_paramLogic_MagicSend.m_nActionIndex = 0;
			}
		}
		break;
	case SKILL_ACTION_TYPE_CONCATENATION_EX:
		if ( bOldSkillID )
		{
			m_paramLogic_MagicSend.m_nActionIndex++;
			if ( m_paramLogic_MagicSend.m_nActionIndex >= m_paramLogic_MagicSend.m_nActionCount - 1 )
			{
				m_paramLogic_MagicSend.m_nActionIndex = 1;
			}
			bPlayAction = FALSE;
		}
		else
		{
			m_paramLogic_MagicSend.m_nActionIndex = 0;
		}
		break;
	case SKILL_ACTION_TYPE_NONE:
	default:
		if ( m_paramLogic_MagicSend.m_nActionCount > 0 )
			m_paramLogic_MagicSend.m_nActionIndex = (::rand())%m_paramLogic_MagicSend.m_nActionCount;
		else
			m_paramLogic_MagicSend.m_nActionIndex = 0;
		break;
	}

	if(bPlayAction || IsAnimationEnd())
	{
		INT nRandAnimID = GetSkillDataMgr()->GetRandAnim(m_paramLogic_MagicSend.m_nSaveMagicID, m_paramLogic_MagicSend.m_nActionIndex, TRUE);
		ChangeAction( nRandAnimID, GetLogicSpeed(), FALSE );
	}
	else
	{
		m_paramLogic_MagicSend.m_bDoNextAction_Concatenation = TRUE;
	}

	if ( CObjectManager::GetMe()->GetMySelf() == this )
	{
		// 物品使用不触发其他的技能公共cooldowm 
		if( !pClientSkill->m_bAutoRedo && pClientSkill->m_nClassByUser != 3)
			CActionSystem::GetMe()->UpdateCommonCoolDown(INVALID_ID);
	}

	SetCurrentLogicCommand(pLogicCommand);
	SetLogicCount(pLogicCommand->GetLogicCount());

	if(pClientSkill->m_nFriendness < 0)
	{
		SetFightState(TRUE);
	}
	CharacterLogic_Set(CHARACTER_LOGIC_SKILL_SEND);
	return TRUE;
}

BOOL CObject_Character::Start_Dead( BOOL bPlayDieAni )
{
	//Assert(!CharacterLogic_IsStopped() && "CObject_Character::Start_Dead");

	//Disalbe(OSF_RAY_QUERY);

	CharacterLogic_Stop(FALSE);

	CleanupLogicCommandList();

	if(m_bDropBox_HaveData)
	{
		//创建ItemBox
		CTripperObject_ItemBox* pBox = (CTripperObject_ItemBox*)CObjectManager::GetMe()->NewObject( "CTripperObject_ItemBox", m_nDropBox_ItemBoxID);
		pBox->Initial(NULL);	
		//设置位置
		pBox->SetMapPosition( fVector2(m_posDropBox_CreatePos.m_fX, m_posDropBox_CreatePos.m_fZ));
		//设置掉落箱的归属
		pBox->SetOwnerGUID(m_DropBox_OwnerGUID);

		m_bDropBox_HaveData = FALSE;
	}

	// 必需下马
	ReleaseMountRenderInterface();

	CharacterLogic_Set( CHARACTER_LOGIC_DEAD );

	if ( bPlayDieAni )
	{
		ChangeAction( BASE_ACTION_F_DIE, GetLogicSpeed(), FALSE );
	}
	else
	{
		ChangeAction( BASE_ACTION_F_CADAVER, GetLogicSpeed(), FALSE, 0.0f );
	}

	SetFightState( FALSE );
	CGameProcedure::m_bWaitNeedFreshMinimap = true;

	//设置选择优先级
	if(m_pRenderInterface )
	{
		if(CT_PLAYEROTHER == GetCharacterType())
			m_pRenderInterface->RayQuery_SetLevel(tEntityNode::RL_PLAYER_DEADBODY);
		else
			m_pRenderInterface->RayQuery_SetLevel(tEntityNode::RL_CREATURE_DEADBODY);
	}

	RemoveAllImpact();

	if(this == CObjectManager::GetMe()->GetMySelf())
	{
		STRING strTemp = "你死亡了";//COLORMSGFUNC("DIE_YOU_DIED_MSG");
		ADDTALKMSG(strTemp);
	}
	return TRUE;
}

BOOL CObject_Character::Start_Ability(CObjectCommand_Logic *pLogicCommand)
{
	//Assert(!CharacterLogic_IsStopped() && "CObject_Character::Start_Ability");

	if(pLogicCommand == NULL)
		return FALSE;

	if(pLogicCommand->GetCommandID() != OBJECT_COMMAND_ABILITY)
		return FALSE;

	if ( CharacterLogic_Get() == CHARACTER_LOGIC_DEAD )
		return FALSE;

	CObjectCommand_Ability *pAbilityCommand = (CObjectCommand_Ability*)pLogicCommand;

	DBC_DEFINEHANDLE(s_pAbilityDbc, DBC_LIFEABILITY_DEFINE);
	const _DBC_LIFEABILITY_DEFINE* pAbilityDef = (const _DBC_LIFEABILITY_DEFINE*)s_pAbilityDbc->Search_Index_EQU(pAbilityCommand->GetAbilityID());
	if(pAbilityDef == NULL)
		return FALSE;

	UINT uTotalTime = 0;
	if(pAbilityCommand->GetPrescriptionID() >= 0)
	{
		DBC_DEFINEHANDLE(s_pPrescrDBC, DBC_LIFEABILITY_ITEMCOMPOSE);
		const _DBC_LIFEABILITY_ITEMCOMPOSE* pDefine = 
			(const _DBC_LIFEABILITY_ITEMCOMPOSE*)s_pPrescrDBC->Search_Index_EQU(pAbilityCommand->GetPrescriptionID());

		//记录起始时间和总共需要的时间
		if(pDefine != NULL && pDefine->nProficiencyTime > 0)
			uTotalTime = (UINT)(pDefine->nProficiencyTime);
	}

	if(uTotalTime == 0)
	{
		//记录起始时间和总共需要的时间
		if(pAbilityDef->nTimeOperation > 0)
			uTotalTime = (UINT)(pAbilityDef->nTimeOperation);
	}

	if(uTotalTime == 0)
		return FALSE;

	// 方向
	CObject *pTarget = (CObject*)((CObjectManager::GetMe())->FindServerObject( pAbilityCommand->GetTargetObjID() ));
	if(pTarget != NULL && pTarget != this)
	{
		fVector3 fvTarget = pTarget->GetPosition();
		fVector2 fvThis;
		fVector2 fvTarget2;

		fvThis.x = GetPosition().x;
		fvThis.y = GetPosition().z;
		fvTarget2.x = fvTarget.x;
		fvTarget2.y = fvTarget.z;

		FLOAT fDir = TDU_GetYAngle(fvThis, fvTarget2);
		SetFaceDir( fDir );

	}

	//// 隐藏武器
	//if(GetCharacterType() == CT_PLAYEROTHER || GetCharacterType() == CT_PLAYERMYSELF)
	//	((CObject_PlayerOther*)this)->EquipItem_BodyLocator( HEQUIP_WEAPON, pAbilityDef->nItemVisualLocator);

	// 更改人物动作
	ChangeAction( pAbilityDef->nAnimOperation, GetLogicSpeed(), TRUE );

	//更新生活技能声音
	if(CObjectManager::GetMe()->GetMySelf() == this)
	{
		//创建声音
		if(!m_pLifeAbilitySound)
		{
			m_pLifeAbilitySound = CGameProcedure::s_pSoundSystem->Source_Create(CSoundSourceFMod::type_skill, FALSE, FALSE);
			TDAssert(m_pLifeAbilitySound);

		}

		//根据生活技能开始播放不同声音
		switch(pAbilityCommand->GetAbilityID())
		{
		case 1:			//烹饪
			m_pLifeAbilitySound->SetBuffer(CGameProcedure::s_pSoundSystem->Buffer_Create(16+59));
			m_pLifeAbilitySound->SetLooping(TRUE);
			m_pLifeAbilitySound->Play();
		case 2:			//中医
			m_pLifeAbilitySound->SetBuffer(CGameProcedure::s_pSoundSystem->Buffer_Create(17+59));
			m_pLifeAbilitySound->SetLooping(TRUE);
			m_pLifeAbilitySound->Play();
			break;
		case 3:			//加工
			m_pLifeAbilitySound->SetBuffer(CGameProcedure::s_pSoundSystem->Buffer_Create(20+59));//还有18
			m_pLifeAbilitySound->SetLooping(TRUE);
			m_pLifeAbilitySound->Play();
			break;
		case 4:			//铸剑
		case 5:			//制衣
		case 6:			//艺术制作
		case 7:			//工程学
			m_pLifeAbilitySound->SetBuffer(CGameProcedure::s_pSoundSystem->Buffer_Create(18+59));
			m_pLifeAbilitySound->SetLooping(TRUE);
			m_pLifeAbilitySound->Play();
			break;
		case 8:			//采矿
			m_pLifeAbilitySound->SetBuffer(CGameProcedure::s_pSoundSystem->Buffer_Create(12+59));
			m_pLifeAbilitySound->SetLooping(TRUE);
			m_pLifeAbilitySound->Play();
			break;
		case 9:			//采药
			m_pLifeAbilitySound->SetBuffer(CGameProcedure::s_pSoundSystem->Buffer_Create(10+59));
			m_pLifeAbilitySound->SetLooping(TRUE);
			m_pLifeAbilitySound->Play();
			break;
		case 10:		//钓鱼
			m_pLifeAbilitySound->SetBuffer(CGameProcedure::s_pSoundSystem->Buffer_Create(14+59));
			m_pLifeAbilitySound->SetLooping(FALSE);
			m_pLifeAbilitySound->Play();
			break;
		default:		//...
			break;
		}

	}

	SetCurrentLogicCommand(pLogicCommand);
	SetLogicCount(pLogicCommand->GetLogicCount());

	m_paramLogic_Ability.m_uCurrentTime		= 0;
	m_paramLogic_Ability.m_uTotalTime		= uTotalTime;

	//进度条
	if ( CObjectManager::GetMe()->GetMySelf() == this )
	{
		CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_SHOW, pAbilityDef->szProgressbarName);
		CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_WIDTH, 0);
	}

	//设置人物逻辑为“生活技能进行中...”
	CharacterLogic_Set( CHARACTER_LOGIC_ABILITY_ACTION );
	return TRUE;
}

BOOL CObject_Character::Start_Stall(BOOL bPlayAni)
{
	ChangeAction( BASE_ACTION_N_SIT_DOWN, GetLogicSpeed(), FALSE);
	m_paramLogic_Stall.m_nCurrentAnimation	= SLogicParam_Stall::STALL_ANIMATION_SITDOWN;

	CharacterLogic_Set(CHARACTER_LOGIC_STALL);
	return TRUE;
}

BOOL CObject_Character::Tick_Idle(UINT uElapseTime)
{
	m_paramLogic_Idle.m_uStartTime += uElapseTime;
	if(GetCharacterData()->IsSit() && IsHaveChatMoodAction())
	{
		FreshAnimation();
	}
	return TRUE;
}

BOOL CObject_Character::Tick_Action(UINT uElapseTime)
{
	if(IsAnimationEnd() || !IsLogicCommandListEmpty())
	{
		CharacterLogic_Stop(TRUE);
	}
	return TRUE;
}

BOOL CObject_Character::Tick_Move(UINT uElapseTime)
{
	CObjectCommand_Move *pMoveCommand = (CObjectCommand_Move*)GetCurrentLogicCommand();
	if(pMoveCommand == NULL)
	{
		return FALSE;
	}

	if(pMoveCommand->GetNodeCount() <= 0)
	{
		return FALSE;
	}

	//// 主角不走回头路，不瞬移
	//BOOL bAdjustPos = FALSE;
	//WORLD_POS posAdjust;
	//if(m_paramLogic_Move.m_nCurrentNodeIndex >= pMoveCommand->GetNodeCount())
	//{
	//	const WORLD_POS *paPos = pMoveCommand->GetNodeList();
	//	INT nEndNodeIndex = pMoveCommand->GetNodeCount() - 1;
	//	posAdjust = paPos[nEndNodeIndex];

	//	bAdjustPos	= TRUE;
	//}
	//else if(m_paramLogic_Move.m_nCurrentNodeIndex == pMoveCommand->GetNodeCount() - 1)
	//{
	//	const WORLD_POS *paPos = pMoveCommand->GetNodeList();
	//	INT nEndNodeIndex = pMoveCommand->GetNodeCount() - 1;
	//	WORLD_POS posCommandTarget = paPos[nEndNodeIndex];

	//	FLOAT fLenCSTarget = fabsf(m_paramLogic_Move.m_posSaveTarget.m_fX - posCommandTarget.m_fX)
	//		+ fabsf(m_paramLogic_Move.m_posSaveTarget.m_fZ - posCommandTarget.m_fZ);

	//	// 目标点变动了
	//	if(fLenCSTarget > 0.01f)
	//	{
	//		FLOAT fSaveToServerDist = TDU_GetDist(fVector2(m_paramLogic_Move.m_posSaveTarget.m_fX, m_paramLogic_Move.m_posSaveTarget.m_fZ),
	//			fVector2(posCommandTarget.m_fX, posCommandTarget.m_fZ));

	//		FLOAT fSaveToCurrentDist = TDU_GetDist(fVector2(m_paramLogic_Move.m_posSaveTarget.m_fX, m_paramLogic_Move.m_posSaveTarget.m_fZ),
	//			fVector2(GetPosition().x, GetPosition().z));

	//		// 这里忽略了服务器传过来的目标点不在路径上的情况
	//		if(fSaveToServerDist - fSaveToCurrentDist >= 0.0f)
	//		{
	//			posAdjust = posCommandTarget;
	//			bAdjustPos = TRUE;
	//		}

	//		m_paramLogic_Move.m_posSaveTarget.m_fX	= posCommandTarget.m_fX;
	//		m_paramLogic_Move.m_posSaveTarget.m_fZ	= posCommandTarget.m_fZ;
	//	}
	//}

	//if(bAdjustPos)
	//{
	//	if(CObjectManager::GetMe()->GetMySelf() == this)
	//	{
	//		// 当位置差大于某个值时
	//		FLOAT fAdjustToCurrentDist = TDU_GetDist(fVector2(posAdjust.m_fX, posAdjust.m_fZ),
	//			fVector2(GetPosition().x, GetPosition().z));

	//		if(fAdjustToCurrentDist > DEF_CLIENT_ADJUST_POS_WARP_DIST)
	//		{
	//			// 瞬移到当前服务器对应的位置
	//			fVector2 fvPos(posAdjust.m_fX, posAdjust.m_fZ);
	//			SetMapPosition( fvPos );
	//		}
	//		else
	//		{
	//			// 调整服务器位置到当前主角客户端对应的位置
	//			CGCharPositionWarp msgWarp;
	//			WORLD_POS posCur(GetPosition().x, GetPosition().z);
	//			msgWarp.setObjID(GetServerID());
	//			msgWarp.setServerPos(posAdjust);
	//			msgWarp.setClientPos(posCur);
	//			CNetManager::GetMe()->SendPacket(&msgWarp);
	//		}
	//	}
	//	else
	//	{
	//		FLOAT fAdjustToCurrentDist = TDU_GetDist(fVector2(posAdjust.m_fX, posAdjust.m_fZ),
	//			fVector2(GetPosition().x, GetPosition().z));

	//		if(fAdjustToCurrentDist > DEF_CHARACTER_POS_ADJUST_DIST)
	//		{
	//			// 瞬移到当前服务器对应的位置
	//			fVector2 fvPos(posAdjust.m_fX, posAdjust.m_fZ);
	//			SetMapPosition( fvPos );
	//		}
	//	}
	//	CharacterLogic_Stop(TRUE);
	//	return TRUE;
	//}

	const WORLD_POS *paPos = pMoveCommand->GetNodeList();

	fVector2 fvStartPos2D(m_paramLogic_Move.m_posSaveStart.m_fX, m_paramLogic_Move.m_posSaveStart.m_fZ);
	fVector2 fvCurrentPos2D(GetPosition().x, GetPosition().z);
	fVector2 fvTargetPos2D(paPos[m_paramLogic_Move.m_nCurrentNodeIndex].m_fX, paPos[m_paramLogic_Move.m_nCurrentNodeIndex].m_fZ);

	// 当前位置与当前的目标路径点路径长度的平方
	fVector2 fvDistToTarget = fvTargetPos2D - fvCurrentPos2D;
	FLOAT fDistToTargetSq = fvDistToTarget.x * fvDistToTarget.x + fvDistToTarget.y * fvDistToTarget.y;
	FLOAT fDistToTarget = sqrt(fDistToTargetSq) ;

	// 这一帧可移动的路径长度
	FLOAT fElapseTime	= ((FLOAT)(uElapseTime))/1000.0f;
	FLOAT fSpeed		= GetCharacterData()->Get_MoveSpeed();
	FLOAT fMoveDist		= fSpeed * fElapseTime;

	if(fMoveDist < 0.01f)
	{
		return FALSE;
	}

	if(fDistToTarget < 0.01f)
	{
		CharacterLogic_Stop(TRUE);
		return TRUE;
	}

	BOOL bStopMove = FALSE;
	fVector2 fvSetToPos = fvCurrentPos2D;
	FLOAT fSetToDir = GetFaceDir();
	while(true)
	{
		if(fMoveDist > fDistToTarget)
		{
			m_paramLogic_Move.m_nCurrentNodeIndex++;
			if(m_paramLogic_Move.m_nCurrentNodeIndex >= pMoveCommand->GetNodeCount())
			{
				// 走到了
				bStopMove	= TRUE;
				fvSetToPos	= fvTargetPos2D;
				fSetToDir	= TDU_GetYAngle(fvCurrentPos2D, fvTargetPos2D);
				break;
			}
			else
			{
				// 改变m_paramLogic_Move中的各个值
				fMoveDist		-= fDistToTarget;
				fvStartPos2D	= fvTargetPos2D;
				fvCurrentPos2D	= fvTargetPos2D;
				fvTargetPos2D	= fVector2(paPos[m_paramLogic_Move.m_nCurrentNodeIndex].m_fX, paPos[m_paramLogic_Move.m_nCurrentNodeIndex].m_fZ);

				m_paramLogic_Move.m_posSaveStart.m_fX	= fvStartPos2D.x;
				m_paramLogic_Move.m_posSaveStart.m_fZ	= fvStartPos2D.y;
				m_paramLogic_Move.m_posSaveTarget.m_fX	= fvTargetPos2D.x;
				m_paramLogic_Move.m_posSaveTarget.m_fZ	= fvTargetPos2D.y;
			}
		}
		else
		{
			FLOAT fDistX = (fMoveDist*(fvTargetPos2D.x-fvCurrentPos2D.x))/fDistToTarget;
			FLOAT fDistZ = (fMoveDist*(fvTargetPos2D.y-fvCurrentPos2D.y))/fDistToTarget;

			fvSetToPos.x = fvCurrentPos2D.x + fDistX;
			fvSetToPos.y = fvCurrentPos2D.y + fDistZ;

			//需要位置修正，防止因为误差走入阻挡区内部
			if( CObjectManager::GetMe()->GetMySelf() == this && CPath::IsPointInUnreachRegion(fvSetToPos))
			{
				fvSetToPos = TDU_GetReflect(fvStartPos2D, fvTargetPos2D, fvSetToPos);
			}

			fSetToDir = TDU_GetYAngle(fvCurrentPos2D, fvTargetPos2D);
			break;
		}
	}

	SetFaceDir(fSetToDir);
	SetMapPosition( fvSetToPos );

	if(bStopMove)
	{
		CharacterLogic_Stop(TRUE);
	}
	return TRUE;
}

BOOL CObject_Character::Tick_MagicCharge(UINT uElapseTime)
{
	CObjectCommand_MagicCharge *pMagicChargeCommand = (CObjectCommand_MagicCharge*)GetCurrentLogicCommand();
	if(pMagicChargeCommand == NULL)
	{
		return FALSE;
	}

	m_paramLogic_MagicCharge.m_uCurrentTime += uElapseTime;
	if(m_paramLogic_MagicCharge.m_uCurrentTime >= pMagicChargeCommand->GetEndTime())
	{
		m_paramLogic_MagicCharge.m_uCurrentTime = pMagicChargeCommand->GetEndTime();
	}

	if ( CObjectManager::GetMe()->GetMySelf() == this )
	{
		FLOAT fCurTime		= (FLOAT)(m_paramLogic_MagicCharge.m_uCurrentTime);
		FLOAT fTotalTime	= (FLOAT)(pMagicChargeCommand->GetTotalTime());
		CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_WIDTH, fCurTime / fTotalTime);
	}

	if(m_paramLogic_MagicCharge.m_uCurrentTime >= pMagicChargeCommand->GetEndTime())
	{
		BOOL bFinished;
		if(pMagicChargeCommand->GetEndTime() == pMagicChargeCommand->GetTotalTime())
		{
			bFinished = TRUE;
		}
		else
		{
			bFinished = FALSE;
		}
		CharacterLogic_Stop(bFinished);
	}

	// 人物的方向
	ObjID_t nTargetObjID = pMagicChargeCommand->GetTargetObjID();
	if(nTargetObjID != INVALID_ID)
	{
		CObject *pObj = (CObject*)(CObjectManager::GetMe()->FindServerObject(nTargetObjID));
		if(pObj != NULL && pObj != this)
		{
			fVector2 fvThis(GetPosition().x, GetPosition().z);
			fVector2 fvTarget(pObj->GetPosition().x, pObj->GetPosition().z);

			FLOAT fDir = TDU_GetYAngle(fvThis, fvTarget);
			SetFaceDir(fDir);
		}
	}
	return TRUE;
}

BOOL CObject_Character::Tick_MagicChannel(UINT uElapseTime)
{
	CObjectCommand_MagicChannel *pMagicChannelCommand = (CObjectCommand_MagicChannel*)GetCurrentLogicCommand();
	if(pMagicChannelCommand == NULL)
	{
		return FALSE;
	}

	if(m_paramLogic_MagicChannel.m_uCurrentTime > uElapseTime)
	{
		m_paramLogic_MagicChannel.m_uCurrentTime -= uElapseTime;
	}
	else
	{
		m_paramLogic_MagicChannel.m_uCurrentTime = 0;
	}

	if(m_paramLogic_MagicChannel.m_uCurrentTime < pMagicChannelCommand->GetEndTime())
	{
		m_paramLogic_MagicChannel.m_uCurrentTime = pMagicChannelCommand->GetEndTime();
	}

	if ( CObjectManager::GetMe()->GetMySelf() == this )
	{
		FLOAT fCurTime		= (FLOAT)(m_paramLogic_MagicChannel.m_uCurrentTime);
		FLOAT fTotalTime	= (FLOAT)(pMagicChannelCommand->GetTotalTime());
		CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_WIDTH, fCurTime / fTotalTime);
	}

	if(m_paramLogic_MagicChannel.m_uCurrentTime <= pMagicChannelCommand->GetEndTime())
	{
		BOOL bFinished;
		if(pMagicChannelCommand->GetEndTime() == 0)
		{
			bFinished = TRUE;
		}
		else
		{
			bFinished = FALSE;
		}
		CharacterLogic_Stop(bFinished);
	}

	// 人物的方向
	ObjID_t nTargetObjID = pMagicChannelCommand->GetTargetObjID();
	if(nTargetObjID != INVALID_ID)
	{
		CObject *pObj = (CObject*)(CObjectManager::GetMe()->FindServerObject(nTargetObjID));
		if(pObj != NULL && pObj != this)
		{
			fVector2 fvThis(GetPosition().x, GetPosition().z);
			fVector2 fvTarget(pObj->GetPosition().x, pObj->GetPosition().z);

			FLOAT fDir = TDU_GetYAngle(fvThis, fvTarget);
			SetFaceDir(fDir);
		}
	}
	return TRUE;
}

BOOL CObject_Character::Tick_MagicSend(UINT uElapseTime)
{
	const SSkillData *pClientSkill = GetSkillDataMgr()->GetSkillData( m_paramLogic_MagicSend.m_nSaveMagicID );
	if ( pClientSkill == NULL )
		return FALSE;

	m_paramLogic_MagicSend.m_uAnimationTime += uElapseTime;
	if(m_paramLogic_MagicSend.m_uAnimationTime > 5000)
	{
		CharacterLogic_Stop(TRUE);
		return TRUE;
	}

	if(IsAnimationEnd())
	{
		m_paramLogic_MagicSend.m_uAnimationEndElapseTime += uElapseTime;

		BOOL bStop = TRUE;
		INT nSkillActionType = pClientSkill->m_nSkillActionType;
		if(!IsLogicCommandListEmpty() && (/*nSkillActionType == SKILL_ACTION_TYPE_CONCATENATION || */nSkillActionType == SKILL_ACTION_TYPE_CONCATENATION_EX))
		{
			if(m_paramLogic_MagicSend.m_uAnimationEndElapseTime < 500)
			{
				bStop = FALSE;
			}
		}

		if(bStop)
		{
			CharacterLogic_Stop(TRUE);
		}
	}

	if(m_paramLogic_MagicSend.m_bCanBreak && !IsLogicCommandListEmpty())
	{
		CharacterLogic_Stop(TRUE);
	}
	return TRUE;
}

BOOL CObject_Character::Tick_Dead(UINT uElapseTime)
{
	CCharacterData *pCharacterData = GetCharacterData();
	if(pCharacterData == NULL)
	{
		return FALSE;
	}

	if(!pCharacterData->IsDie())
	{
		CharacterLogic_Stop(TRUE);
	}
	return TRUE;
}

BOOL CObject_Character::Tick_Ability(UINT uElapseTime)
{
	CObjectCommand_Ability *pAbilityCommand = (CObjectCommand_Ability*)GetCurrentLogicCommand();
	if(pAbilityCommand == NULL)
	{
		return FALSE;
	}

	m_paramLogic_Ability.m_uCurrentTime += uElapseTime;

	UINT uEndTime = pAbilityCommand->GetEndTime();
	if(uEndTime == UINT_MAX)
	{
		uEndTime = m_paramLogic_Ability.m_uTotalTime;
	}

	if(m_paramLogic_Ability.m_uCurrentTime > uEndTime)
	{
		m_paramLogic_Ability.m_uCurrentTime = uEndTime;
	}

	if ( CObjectManager::GetMe()->GetMySelf() == this )
	{
		CEventSystem::GetMe()->PushEvent(GE_PROGRESSBAR_WIDTH, (FLOAT)m_paramLogic_Ability.m_uCurrentTime/(FLOAT)m_paramLogic_Ability.m_uTotalTime);
	}

	if(m_paramLogic_Ability.m_uCurrentTime >= uEndTime)
	{
		if(m_paramLogic_Ability.m_uTotalTime == uEndTime)
		{
			CharacterLogic_Stop(TRUE);
		}
		else
		{
			CharacterLogic_Stop(FALSE);
		}
	}
	return TRUE;
}

BOOL CObject_Character::Tick_Stall(UINT uElapseTime)
{
	CCharacterData *pCharacterData = GetCharacterData();
	if(pCharacterData == NULL)
	{
		return FALSE;
	}

	if(IsAnimationEnd())
	{
		if(m_paramLogic_Stall.m_nCurrentAnimation == SLogicParam_Stall::STALL_ANIMATION_SITDOWN)
		{
			ChangeAction(BASE_ACTION_N_SIT_IDLE, GetLogicSpeed(), TRUE);
			m_paramLogic_Stall.m_nCurrentAnimation = SLogicParam_Stall::STALL_ANIMATION_IDLE;
		}

		if(m_paramLogic_Stall.m_nCurrentAnimation == SLogicParam_Stall::STALL_ANIMATION_STANDUP)
		{
			CharacterLogic_Stop(TRUE);
		}
	}

	if(!(pCharacterData->Get_IsInStall())
		&& m_paramLogic_Stall.m_nCurrentAnimation != SLogicParam_Stall::STALL_ANIMATION_STANDUP)
	{
		ChangeAction(BASE_ACTION_N_SIT_STAND, GetLogicSpeed(), FALSE);
		m_paramLogic_Stall.m_nCurrentAnimation = SLogicParam_Stall::STALL_ANIMATION_STANDUP;
	}
	return TRUE;
}

VOID CObject_Character::AddLogicEvent(const _LOGIC_EVENT *pLogicEvent)
{
	if(pLogicEvent != NULL)
	{
		_LOGIC_EVENT *pNewLogicEvent	= new _LOGIC_EVENT;
		*pNewLogicEvent					= *pLogicEvent;
		pNewLogicEvent->m_uBeginTime	= CGameProcedure::s_pTimeSystem->GetTimeNow();
		pNewLogicEvent->m_uRemoveTime	= 10000;	// 10秒
		m_listLogicEvent.push_back(pNewLogicEvent);
	}
}

VOID CObject_Character::RemoveLogicEvent( INT nLogicCount )
{
	CLogicEventList::iterator itCur, itEnd, itDel;
	_LOGIC_EVENT* pLogicEvent;
	itCur = m_listLogicEvent.begin();
	itEnd = m_listLogicEvent.end();
	while( itCur != itEnd )
	{// 只删除该逻辑ID对应的伤害信息，即只删除该技能对应的伤害信息
		pLogicEvent = *itCur;
		itDel = itCur;
		itCur++;
		if ( pLogicEvent->m_nSenderLogicCount == nLogicCount )
		{
			SAFE_DELETE( pLogicEvent );
			m_listLogicEvent.erase( itDel );
			break;
		}
	}
}

VOID CObject_Character::RemoveAllLogicEvent( VOID )
{
	CLogicEventList::iterator itCur, itEnd, itDel;
	_LOGIC_EVENT* pLogicEvent;
	itCur = m_listLogicEvent.begin();
	itEnd = m_listLogicEvent.end();
	while( itCur != itEnd )
	{// 只删除该逻辑ID对应的伤害信息，即只删除该技能对应的伤害信息
		pLogicEvent = *itCur;
		itDel = itCur;
		itCur++;
		SAFE_DELETE( pLogicEvent );
	}
	m_listLogicEvent.clear();
}

BOOL CObject_Character::IsMySelf(INT ObjID) const
{
	CObject *pMySelf = CObjectManager::GetMe()->GetMySelf();
	if (pMySelf)
	{
		return ObjID == pMySelf->GetServerID();
	}
	return FALSE;
}

BOOL CObject_Character::IsMyPet(INT ObjID) const
{
	PET_GUID_t CurrentPetGuid = CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_CurrentPetGUID();
	SDATA_PET* pPet = CDataPool::GetMe()->Pet_GetPet(CurrentPetGuid);
	if (pPet)
	{
		return ObjID == pPet->m_idServer;
	}

	return FALSE;
}

VOID CObject_Character::ShowLogicEvent( INT nServerObjID, INT nLogicCount, BOOL bShowAll/* = FALSE */ )
{
	CLogicEventList::iterator itCur, itEnd, itPrev;
	_LOGIC_EVENT *pLogicEvent;
	itCur = m_listLogicEvent.begin();
	itEnd = m_listLogicEvent.end();
	while ( itCur != itEnd )
	{// 只显示该技能释放者的该逻辑ID对应的伤害信息
		pLogicEvent = *itCur;
		itPrev = itCur;
		itCur++;
		if ( pLogicEvent->m_nSenderID == nServerObjID
			&& pLogicEvent->m_nSenderLogicCount <= nLogicCount	)
		{
			//AxTrace(0, 0, "%s", "Show Damage In DamageQueue!");
			DoLogicEvent(pLogicEvent);
			SAFE_DELETE( pLogicEvent );
			m_listLogicEvent.erase( itPrev );
			// 如果只显示一条记录则break;

			if(m_listLogicEvent.empty() && IsDie() && CharacterLogic_Get() != CHARACTER_LOGIC_DEAD)
			{
				Start_Dead(TRUE);
			}

			if (!bShowAll)
				break;
		}
	}

}

VOID CObject_Character::Update_LogicEvent( VOID )
{// 正常情况下该函数内没有记录不需要更新，在网络延迟的情况下才有可能

	if(m_listLogicEvent.empty())
	{
		return;
	}

	CLogicEventList::iterator itCur, itEnd, itPrev;
	_LOGIC_EVENT *pLogicEvent;

	UINT uTimeNow = CGameProcedure::s_pTimeSystem->GetTimeNow();
	itCur = m_listLogicEvent.begin();
	itEnd = m_listLogicEvent.end();

	while ( itCur != itEnd )
	{// 更新所有的伤害信息,如果超过该伤害的持续时间则强制删除
		pLogicEvent = *itCur;
		itPrev = itCur;
		itCur++;

		BOOL bMustRemove = FALSE;
		BOOL bMustDo = FALSE;
		if(pLogicEvent->m_nSenderID != INVALID_ID)
		{
			CObject *pObject = (CObject*)(CObjectManager::GetMe()->FindServerObject(pLogicEvent->m_nSenderID));
			if(pObject != NULL)
			{
				if(pObject->IsLogicReady(pLogicEvent->m_nSenderLogicCount))
				{
					bMustDo = TRUE;
				}
				else if(pLogicEvent->m_uBeginTime + pLogicEvent->m_uRemoveTime < uTimeNow)
				{
					bMustRemove = TRUE;
				}
			}
			else
			{
				bMustDo = TRUE;
			}
		}
		else
		{
			bMustDo = TRUE;
		}
		if(bMustDo || bMustRemove)
		{
			//AxTrace(0, 0, "%s", "Show Damage in Update_LogicEvent!");
			if(bMustDo)
			{
				DoLogicEvent(pLogicEvent);
			}
			SAFE_DELETE( pLogicEvent );
			m_listLogicEvent.erase( itPrev );

			if(m_listLogicEvent.empty() && IsDie() && CharacterLogic_Get() != CHARACTER_LOGIC_DEAD)
			{
				Start_Dead(TRUE);
			}
		}
	}
}

VOID CObject_Character::DoLogicEvent(const _LOGIC_EVENT *pLogicEvent)
{
	if(pLogicEvent != NULL)
	{
		switch(pLogicEvent->m_nEventType)
		{
		case LOGIC_EVENT_TYPE_BULLET:
			DoLogicEvent_Bullet(pLogicEvent);
			break;
		case LOGIC_EVENT_TYPE_DAMAGE:
			DoLogicEvent_Damage(pLogicEvent);
			break;
		default:
			break;
		}
	}
}

VOID CObject_Character::DisplayDamageBoard(const _DAMAGE_INFO* pDamageInfo)
{
	if (pDamageInfo == NULL)
		return;
	if (GetRenderInterface() == NULL || CGameProcedure::s_pUISystem == NULL)
		return;
	CObject *pMySelf = CObjectManager::GetMe()->GetMySelf();
	if (pMySelf == NULL)
		return;
	if (this->GetCharacterType() == CT_MONSTER && ( ((CObject_PlayerNPC*)this)->IsDisplayBoard() == FALSE))
		return;
	
	CHAR szDamage[32];
	memset(szDamage, 0, sizeof(szDamage));

	fVector2 fvPos = m_pInfoBoard ? m_pInfoBoard->GetPosition() : fVector2(0.f, 0.f);
	fVector3 fvCurPos = GetPosition();
	INT nCasterID = pDamageInfo->m_nSenderID;
	INT nReceiverID = pDamageInfo->m_nTargetID;
	
	BOOL bNeedDisplay = TRUE;
	bool bDouble = pDamageInfo->m_bIsCriticalHit > 0;
	if (TRUE == IsMyPet(nCasterID))
	{
		if(0 > pDamageInfo->m_nHealthIncrement)//伤害
		{
			_sntprintf(szDamage, 32, "%d", pDamageInfo->m_nHealthIncrement);
			CGameProcedure::s_pUISystem->AddNewBeHitBoard(bDouble, szDamage, fvPos.x, fvPos.y, 6, 6);
		}
	}
	else if (TRUE == IsMySelf(nCasterID))
	{
		if(0 > pDamageInfo->m_nHealthIncrement)//伤害
		{
			_sntprintf(szDamage, 32, "%d", pDamageInfo->m_nHealthIncrement);
			CGameProcedure::s_pUISystem->AddNewBeHitBoard(bDouble, szDamage, fvPos.x, fvPos.y, 7, 7);
			AxTrace(1, 0, "%d", pDamageInfo->m_nHealthIncrement);
		}
	}
	else if (TRUE == IsMyPet(nReceiverID))
	{
		if(0 > pDamageInfo->m_nHealthIncrement)//伤害
		{
			_sntprintf(szDamage, 32, "%d", pDamageInfo->m_nHealthIncrement);
			CGameProcedure::s_pUISystem->AddNewBeHitBoard(bDouble, szDamage, fvPos.x, fvPos.y, 1, 1);
		}
	}
	
	if (TRUE == IsMySelf(nReceiverID))
	{
		if(0 < pDamageInfo->m_nHealthIncrement)//治疗
		{
			_sntprintf(szDamage, 32, "+%d", pDamageInfo->m_nHealthIncrement);
			CGameProcedure::s_pUISystem->AddNewBeHitBoard(bDouble, szDamage, fvPos.x, fvPos.y, 0, 0);
		}
		if(0 > pDamageInfo->m_nHealthIncrement)//伤害
		{
			_sntprintf(szDamage, 32, "%d", pDamageInfo->m_nHealthIncrement);
			CGameProcedure::s_pUISystem->AddNewBeHitBoard(bDouble, szDamage, fvPos.x, fvPos.y, 1, 1);
			AxTrace(4, 0, "%d", pDamageInfo->m_nHealthIncrement);
		}
		if(0 < pDamageInfo->m_nRageIncrement)//怒气增加
		{
			_sntprintf(szDamage, 32, "+%d", pDamageInfo->m_nRageIncrement);
			CGameProcedure::s_pUISystem->AddNewBeHitBoard(bDouble, szDamage, fvPos.x, fvPos.y, 2, 2);
		}
		if(0 > pDamageInfo->m_nRageIncrement)//怒气减少
		{
			_sntprintf(szDamage, 32, "%d", pDamageInfo->m_nRageIncrement);
			CGameProcedure::s_pUISystem->AddNewBeHitBoard(bDouble, szDamage, fvPos.x, fvPos.y, 3, 3);
		}
		if(0 < pDamageInfo->m_nManaIncrement)//mana增加
		{
			_sntprintf(szDamage, 32, "+%d", pDamageInfo->m_nManaIncrement);
			CGameProcedure::s_pUISystem->AddNewBeHitBoard(bDouble, szDamage, fvPos.x, fvPos.y, 4, 4);
		}
		//if(0 > pDamageInfo->m_nManaIncrement)//mana减少
		//{
		//	_sntprintf(szDamage, 32, "%d", pDamageInfo->m_nManaIncrement);
		//	CGameProcedure::s_pUISystem->AddNewBeHitBoard(bDouble, szDamage, fvPos.x, fvPos.y, 5, 5);
		//}
	}
}

VOID	CObject_Character::DisplayMissImmuAndSoOn(const _DAMAGE_INFO* pDamageInfo)
{
	if (pDamageInfo == NULL)
		return;
	if (GetRenderInterface() == NULL || CGameProcedure::s_pUISystem == NULL)
		return;

	fVector2 fvPos = m_pInfoBoard ? m_pInfoBoard->GetPosition() : fVector2(0.f, 0.f);
	fVector3 fvCurPos = GetPosition();
	if ( m_pMountRenderInterface != NULL )
	{
		fvCurPos.y += m_fMountAddHeight;
	}
	GetRenderInterface()->Actor_GetInfoBoardPos(fvPos, &fvCurPos);

	CHAR m_szDamage[32];
	memset(m_szDamage, 0, sizeof(m_szDamage));
	CObject *pMySelf = CObjectManager::GetMe()->GetMySelf();
	if (!pMySelf)
	{
		return;
	}
	BOOL bFlag = FALSE;
	STRING strMsg = "";
	switch (pDamageInfo->m_nImpactID)
	{
	case MissFlag_T::FLAG_MISS:
		{
			strMsg = "未击中";//NOCOLORMSGFUNC("Combat_Miss");
		}
		bFlag = TRUE;
		break;
	case MissFlag_T::FLAG_IMMU:
		{
			strMsg = "免疫";//NOCOLORMSGFUNC("Combat_Immu");
		}
		bFlag = TRUE;
		break;
	case MissFlag_T::FLAG_ABSORB:
		{
			strMsg = "吸收";//NOCOLORMSGFUNC("Combat_Absorb");
		}
		bFlag = TRUE;
		break;
	case MissFlag_T::FLAG_COUNTERACT:
		{
			strMsg = "抵消";
		}
		bFlag = TRUE;
		break;
	case MissFlag_T::FLAG_TRANSFERED:
		{
			strMsg = "转移";//NOCOLORMSGFUNC("Combat_Transfered");
		}
		bFlag = TRUE;
		break;
	default:
		break;
	}
	_sntprintf(m_szDamage, 32, "%s", strMsg.c_str());
	if (TRUE==bFlag)
	{
		CGameProcedure::s_pUISystem->AddNewBeHitBoard(false, m_szDamage, fvPos.x, fvPos.y, 8, 8 );
	}
}

VOID CObject_Character::DoLogicEvent_Damage(const _LOGIC_EVENT *pLogicEvent)
{
	const _DAMAGE_INFO *pDamageInfo	= &(pLogicEvent->m_damage);
	switch (pDamageInfo->m_nType)
	{
	case _DAMAGE_INFO::TYPE_DIE:
		{
			Start_Dead(TRUE);
		}
		break;
	case _DAMAGE_INFO::TYPE_DROP_BOX:
		{
			ObjID_t ObjID = pDamageInfo->m_aAttachedParams[0];
			INT idOwner = pDamageInfo->m_aAttachedParams[1];
			FLOAT fX = pDamageInfo->m_aAttachedParams[2]/1000.f;
			FLOAT fZ = pDamageInfo->m_aAttachedParams[3]/1000.f;
			//创建ItemBox
			CTripperObject_ItemBox* pBox = (CTripperObject_ItemBox*)CObjectManager::GetMe()->NewObject( "CTripperObject_ItemBox", ObjID);
			pBox->Initial(NULL);	
			//设置位置
			pBox->SetMapPosition(fVector2(fX, fZ));
			//设置掉落箱的归属
			pBox->SetOwnerGUID(idOwner);
		}
		break;
	case _DAMAGE_INFO::TYPE_EFFECT:
		{
			// 表现特效效果
			if ( pDamageInfo->m_nImpactID != INVALID_ID )
			{
				_DIRECT_IMPACT *pDirectlyImpact = GetDirectlyImpactMgr()->GetDirectlyImpact( pDamageInfo->m_nImpactID );
				if ( pDirectlyImpact != NULL )
				{
					fVector3 fvPos;
					if ( GetRenderInterface() != NULL && strlen( pDirectlyImpact->m_pszEffectLocator ) > 0 )
					{
						GetRenderInterface()->Actor_GetLocator( pDirectlyImpact->m_pszEffectLocator, fvPos );
					}
					else
					{
						fvPos = GetPosition();
					}

					if ( strlen( pDirectlyImpact->m_pszEffect ) > 0 )
					{
						CObject_Effect *pEffectObj = (CObject_Effect*)(CObjectManager::GetMe()->NewObject( "CObject_Effect" ));
						if ( pEffectObj != NULL )
						{
							SObject_EffectInit initEffect;
							initEffect.m_fvPos 			= fvPos;
							initEffect.m_fvRot 			= fVector3( 0.f, 0.f, 0.f );
							initEffect.m_pszEffectName	= pDirectlyImpact->m_pszEffect;
							initEffect.m_bLoopEffect	= FALSE;
							pEffectObj->Initial( &initEffect );
						}
					}

					if ( strlen( pDirectlyImpact->m_pszSound ) > 0 )
					{
						fVector3 fvPos = GetPosition();
						//此接口是提供给fairy调用的音效接口，所以传入坐标必须是gfx坐标
						fVector3 fvGame;
						if(!CGameProcedure::s_pGfxSystem->Axis_Trans(CRenderSystem::AX_GAME, fvPos, 
							CRenderSystem::AX_GFX, fvGame))
						{
							return;
						}
						CSoundSystemFMod::_PlaySoundFunc( pDirectlyImpact->m_pszSound, &fvGame.x, false );
					}
				}
			}
		}
		break;
	case _DAMAGE_INFO::TYPE_HEAL_AND_DAMAGE:
		{
			//如果是伤害，则显示被击动作
			if ( pDamageInfo->m_nHealthIncrement< 0 )
			{
				SetFightState(TRUE);
				if ( CharacterLogic_Get() == CHARACTER_LOGIC_IDLE )
				{
					ChangeAction( BASE_ACTION_F_BE_HIT, 1.f, FALSE );
				}
			}
			// 显示伤血信息
			DisplayDamageBoard(pDamageInfo);
		}
		break;
	case _DAMAGE_INFO::TYPE_SKILL_TEXT:
		{
			// 显示未击中和免疫信息
			DisplayMissImmuAndSoOn(pDamageInfo);
		}
		break;
	default:
		break;
	}
	return ;
}

VOID CObject_Character::DoLogicEvent_Bullet(const _LOGIC_EVENT *pLogicEvent)
{
	if(pLogicEvent != NULL)
	{
		const _LOGIC_EVENT_BULLET *pBulletInfo = &(pLogicEvent->m_bullet);

		CObject *pSender = (CObject*)(CObjectManager::GetMe()->FindServerObject(pLogicEvent->m_nSenderID));
		if(pSender != NULL)
		{
			fVector3 fvSenderPos, fvSenderRot;
			if(g_theKernel.IsKindOf(pSender->GetClass(), GETCLASS(CObject_Character)))
			{
				CObject_Character *pCharacterSender = (CObject_Character*)(pSender);
				if(pBulletInfo->m_pszSenderLocator != NULL
					&& strlen(pBulletInfo->m_pszSenderLocator) > 0
					&& pCharacterSender->GetRenderInterface() != NULL)
				{
					fvSenderPos = pCharacterSender->GetPosition();
					pCharacterSender->GetRenderInterface()->Actor_GetLocator(pBulletInfo->m_pszSenderLocator, fvSenderPos);
				}
				else
				{
					fvSenderPos = pCharacterSender->GetPosition();
				}
				fvSenderRot = pCharacterSender->GetRotation();
			}
			else
			{
				fvSenderPos	= pSender->GetPosition();
				fvSenderRot = pSender->GetRotation();
			}

			SObject_BulletInit initBullet;
			initBullet.m_fvPos				= fvSenderPos;
			initBullet.m_fvRot				= fvSenderRot;
			initBullet.m_idSend				= pLogicEvent->m_nSenderID;
			initBullet.m_nSendLogicCount	= pLogicEvent->m_nSenderLogicCount;
			initBullet.m_nBulletID			= pBulletInfo->m_nBulletID;
			initBullet.m_bSingleTarget		= pBulletInfo->m_bHitTargetObj;
			if(pBulletInfo->m_bHitTargetObj)
			{
				initBullet.m_fvTargetPos	= fVector3( -1.f, -1.f, -1.f );
				initBullet.m_idTarget		= pBulletInfo->m_nTargetID;
			}
			else
			{
				initBullet.m_fvTargetPos.x	= pBulletInfo->m_fTargetX;
				initBullet.m_fvTargetPos.z	= pBulletInfo->m_fTargetZ;
				CWorldManager::GetTerrainHeight_GamePos(pBulletInfo->m_fTargetX, pBulletInfo->m_fTargetZ, (initBullet.m_fvTargetPos.y));
			}

			CObject_Bullet *pBullet = (CObject_Bullet*)CObjectManager::GetMe()->NewObject( "CObject_Bullet");
			pBullet->Initial( &initBullet );
		}
	}
}

//例:
//
// 鞠躬,0,鞠躬,0
//  |	|___________________不循环
//	|_______________________动作名称
//

VOID CObject_Character::SetChatMoodAction(STRING& strOrder)
{
	if(IsDie())
		return ;

	CCharacterData *pCharacterData = GetCharacterData();
	if(pCharacterData == NULL)
		return ;

	if(pCharacterData->Get_MountID() != INVALID_ID)
		return ;

	if(pCharacterData->Get_ModelID() != INVALID_ID)
		return ;

	if(GetCharacterType() == CT_PLAYERMYSELF || GetCharacterType() == CT_PLAYEROTHER)
	{
		if(pCharacterData->Get_TeamFollowFlag())
			return ;

		if(pCharacterData->Get_IsInStall())
			return ;
	}

	ClearAllChatMoodAction();
	//SetChatMoodActionPlaying(FALSE);

	const CHAR splitKey = ';';

	STRING::size_type sB = 0;
	STRING::size_type sE = strOrder.find_first_of(splitKey);
	STRING::size_type sLen = strOrder.size();

	do {
		CHAT_MOOD_DATA cData;

		if(sE == STRING::npos)
		{
			//set last action
			cData.m_ActionId = atoi(strOrder.substr(sB).c_str());
			cData.m_bLoop = FALSE;
			m_listChatMoodAction.push_back(cData);	//填加到动作列表
			break;
		}

		//copy action
		cData.m_ActionId = atoi(strOrder.substr(sB, sE-sB).c_str());

		//get loopdefin
		STRING strOp = strOrder.substr(sE+1, 1);

		//loop
		if(strOp == "1")
		{
			cData.m_bLoop = TRUE;
			sE += 3; //skip '1' adn ';'
		}
		else if(strOp == "0") //no loop
		{
			cData.m_bLoop = FALSE;
			sE += 3; //skip '0' and ';'
		}
		else
		{
			//ignore wrong ';'
			sE += 1;
		}

		m_listChatMoodAction.push_back(cData);			//填加到动作列表
		if(sE >= sLen || cData.m_bLoop == TRUE) break;	//只有最后一个动作才能是循环动作，因为循环动作不触发finish事件

		//save new begin point
		sB = sE;

		//find next KeyStart
		sE = strOrder.find(splitKey, sB);

	} while(TRUE);
}

VOID CObject_Character::ClearAllChatMoodAction()
{
	if(m_listChatMoodAction.empty())
		return;

	m_listChatMoodAction.erase(m_listChatMoodAction.begin(), m_listChatMoodAction.end());	
}

VOID CObject_Character::PlayChatMoodAction()
{
	//if(CharacterLogic_Get() == ENUM_CHARACTER_LOGIC::CHARACTER_LOGIC_IDLE)
	//{
	if(!m_listChatMoodAction.empty())
	{
		CHAT_MOOD_DATA* pChatMood = &(*(m_listChatMoodAction.begin()));

		if(m_pRenderInterface)
		{
			ChangeAction(pChatMood->m_ActionId, 1.0f, pChatMood->m_bLoop);
		}

		if(pChatMood->m_bLoop)		// todo_yangjun 等有更好的逻辑的是欧，再单独处理站立的问题
			m_bNeedStandUp = TRUE;

		SetChatMoodActionPlaying(TRUE);
		m_listChatMoodAction.pop_front();
	}
	else
	{
		SetChatMoodActionPlaying(FALSE);
		m_bNeedStandUp = FALSE;
		m_bStanding = FALSE;
	}
	//}
	//else
	//{
	//	ClearAllChatMoodAction();
	//	SetChatMoodActionPlaying(FALSE);
	//	m_bNeedStandUp = FALSE;
	//	m_bStanding = FALSE;
	//}
}

BOOL CObject_Character::IsHaveChatMoodAction()
{
	return (m_listChatMoodAction.empty())?(FALSE):(TRUE);
}

VOID CObject_Character::UpdateBuffEffect(VOID)
{
	if(m_pRenderInterface != NULL)
	{
		CImpactEffectMap::iterator itCur, itEnd;
		SImpactEffect *pImpactEffect;
		UINT uImpactID;
		itEnd = m_mapImpactEffect.end();
		for(itCur = m_mapImpactEffect.begin(); itCur != itEnd; itCur++)
		{
			uImpactID		= itCur->first;
			pImpactEffect	= itCur->second;
			if(pImpactEffect != NULL)
			{
				_BUFF_IMPACT *pBuffImpact = GetBuffImpactMgr()->GetBuffImpact(uImpactID);
				if(pBuffImpact != NULL)
				{
					if(strlen(pBuffImpact->m_lpszEffect_Continuous) > 0 && m_pRenderInterface != NULL)
					{
						UINT uEffect = m_pRenderInterface->AddEffect(pBuffImpact->m_lpszEffect_Continuous, pBuffImpact->m_lpszBind_Continuous);
						pImpactEffect->SetEffect( uEffect );
					}
				}
			}
		}
	}
}

BOOL CObject_Character::ModifyMoveCommand(const INT nLogicCount, const BYTE numPos, const WORLD_POS* targetPos)
{
	CObjectCommand_Logic* pLogicHandle = FindLogicCommand(nLogicCount);
	if( pLogicHandle && IsMoving() )
	{
		CObjectCommand_Move* pLoginCommandMove = (CObjectCommand_Move*)pLogicHandle;
		return pLoginCommandMove->ModifyTargetPos( numPos, targetPos );
	}
	return FALSE;
}

