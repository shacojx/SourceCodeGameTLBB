// ObjectCommand.cp

#include "StdAfx.h"
#include "..\GameCommand.h"
#include "ObjectCommandDef.h"

#include "ObjectCommand.h"

/////////////////////////////////////////////
// CObjectCommand
/////////////////////////////////////////////
CObjectCommand::CObjectCommand(VOID)
{
	m_nCommandID	= OBJECT_COMMAND_INVALID;
}

CObjectCommand::~CObjectCommand(VOID)
{
}

/////////////////////////////////////////////
// CObjectCommand_Logic 逻辑指令
/////////////////////////////////////////////
CObjectCommand_Logic::CObjectCommand_Logic(VOID)
{
	m_uStartTime	= UINT_MAX;
	m_nLogicCount	= -1;
}

CObjectCommand_Logic::~CObjectCommand_Logic(VOID)
{
}

VOID CObjectCommand_Logic::CleanUp(VOID)
{
	m_uStartTime	= UINT_MAX;
	m_nLogicCount	= -1;
}

BOOL CObjectCommand_Logic::Modify(const CObjectCommand_StopLogic *pCmd)
{
	return TRUE;
}

/////////////////////////////////////////////
// CObjectCommand_StopLogic 中断逻辑指令
/////////////////////////////////////////////
CObjectCommand_StopLogic::CObjectCommand_StopLogic(VOID)
{
	m_nLogicCount	= -1;
}

CObjectCommand_StopLogic::~CObjectCommand_StopLogic(VOID)
{
}

VOID CObjectCommand_StopLogic::CleanUp(VOID)
{
	m_nLogicCount	= -1;
}

/////////////////////////////////////////////
// CObjectCommand_Action 移动
/////////////////////////////////////////////
CObjectCommand_Action::CObjectCommand_Action(VOID)
{
	m_nActionID		= INVALID_ID;
}

CObjectCommand_Action::~CObjectCommand_Action(VOID)
{
}

BOOL CObjectCommand_Action::Init(UINT uStartTime, INT nLogicCount, INT nActionID)
{
	SetStartTime(uStartTime);
	SetLogicCount(nLogicCount);
	m_nActionID		= INVALID_ID;

	SetCommandID(OBJECT_COMMAND_ACTION);
	return TRUE;
}

VOID CObjectCommand_Action::CleanUp(VOID)
{
	m_nActionID		= INVALID_ID;
	CObjectCommand_Logic::CleanUp();
}

/////////////////////////////////////////////
// CObjectCommand_StopAction 中止动作
/////////////////////////////////////////////
CObjectCommand_StopAction::CObjectCommand_StopAction(VOID)
{
	m_uEndTime			= UINT_MAX;
}

CObjectCommand_StopAction::~CObjectCommand_StopAction(VOID)
{
}

BOOL CObjectCommand_StopAction::Init(INT nLogicCount, UINT uEndTime)
{
	SetLogicCount(nLogicCount);
	m_uEndTime			= uEndTime;
	SetCommandID(OBJECT_COMMAND_STOP_ACTION);
	return TRUE;
}

VOID CObjectCommand_StopAction::CleanUp(VOID)
{
	m_uEndTime			= UINT_MAX;
	CObjectCommand_StopLogic::CleanUp();
}

/////////////////////////////////////////////
// CObjectCommand_Move 移动
/////////////////////////////////////////////
CObjectCommand_Move::CObjectCommand_Move(VOID)
{
	m_nNodeCount	= 0;
	m_paposNode		= NULL;
}

CObjectCommand_Move::~CObjectCommand_Move(VOID)
{
	Assert(m_paposNode == NULL);
}

BOOL CObjectCommand_Move::Init(UINT uStartTime, INT nLogicCount, INT nNodeCount, const WORLD_POS *paposNode)
{
	Assert(m_paposNode == NULL);
	SetStartTime(uStartTime);
	SetLogicCount(nLogicCount);
	m_nNodeCount	= nNodeCount;
	if(m_nNodeCount > 0)
	{
		m_paposNode	= new WORLD_POS[m_nNodeCount];
		memcpy(m_paposNode, paposNode, m_nNodeCount * sizeof(WORLD_POS));
	}
	else
	{
		m_paposNode	= NULL;
	}

	SetCommandID(OBJECT_COMMAND_MOVE);
	return TRUE;
}

VOID CObjectCommand_Move::CleanUp(VOID)
{
	m_nNodeCount	= 0;
	SAFE_DELETE_ARRAY(m_paposNode);
	CObjectCommand_Logic::CleanUp();
}

BOOL CObjectCommand_Move::Modify(const CObjectCommand_StopLogic *pCmd)
{
	if(pCmd != NULL && pCmd->GetCommandID() == OBJECT_COMMAND_STOP_MOVE)
	{
		CObjectCommand_StopMove *pStopCmd = (CObjectCommand_StopMove*)pCmd;
		INT nEndNodeIndex	= pStopCmd->GetEndNodeIndex();
		if(m_nNodeCount > nEndNodeIndex)
		{
			m_nNodeCount = nEndNodeIndex + 1;
			m_paposNode[nEndNodeIndex]	= *(pStopCmd->GetEndPos());
		}
		return TRUE;
	}
	return FALSE;
}

BOOL CObjectCommand_Move::ModifyTargetPos(const BYTE numPos, const WORLD_POS* targetPos)
{
	if( m_nNodeCount == 0 || numPos == 0 || m_nNodeCount < numPos ) return FALSE;

	m_nNodeCount = numPos;
	m_paposNode[numPos-1] = *(targetPos+numPos);
	return TRUE;

	//if( m_nNodeCount == 0 ) return FALSE;
	//if( m_nNodeCount == 1 )
	//{
	//	m_paposNode[0] = *targetPos;
	//	return TRUE;
	//}

	//INT relaPos = m_paposNode[0].RelaPos( *targetPos );
	//for( INT i = 1; i < m_nNodeCount; i ++ )
	//{
	//	INT curRela = m_paposNode[i].RelaPos( *targetPos );
	//	if( curRela != relaPos )
	//	{
	//		m_paposNode[i] = *targetPos;
	//		m_nNodeCount = i;
	//		return TRUE;
	//	}
	//}
	//return FALSE;
}

/////////////////////////////////////////////
// CObjectCommand_StopMove 中止移动
/////////////////////////////////////////////
CObjectCommand_StopMove::CObjectCommand_StopMove(VOID)
{
	m_nEndNodeIndex		= -1;
	//m_posEndPos;
}

CObjectCommand_StopMove::~CObjectCommand_StopMove(VOID)
{
}

BOOL CObjectCommand_StopMove::Init(INT nLogicCount, INT nEndNodeIndex, const WORLD_POS *pPos)
{
	SetLogicCount(nLogicCount);
	m_nEndNodeIndex		= nEndNodeIndex;
	m_posEndPos			= *pPos;
	SetCommandID(OBJECT_COMMAND_STOP_MOVE);
	return TRUE;
}

VOID CObjectCommand_StopMove::CleanUp(VOID)
{
	m_nEndNodeIndex		= -1;
	m_posEndPos			= WORLD_POS(-1.f, -1.f);
	CObjectCommand_StopLogic::CleanUp();
}

/////////////////////////////////////////////
// CObjectCommand_MagicSend 法术发招
/////////////////////////////////////////////
CObjectCommand_MagicSend::CObjectCommand_MagicSend(VOID)
{
	m_nMagicID			= INVALID_ID;
	m_nTargetObjID		= INVALID_ID;
	//m_posTarget;
	m_fTargetDir		= -1.f;
}

CObjectCommand_MagicSend::~CObjectCommand_MagicSend(VOID)
{
}

BOOL CObjectCommand_MagicSend::Init(UINT uStartTime,
									INT nLogicCount,
									INT nMagicID,
									ObjID_t nTargetID,
									const WORLD_POS *pTargetPos,
									FLOAT fTargetDir)
{
	SetStartTime(uStartTime);
	SetLogicCount(nLogicCount);
	m_nMagicID			= nMagicID;
	m_nTargetObjID		= nTargetID;
	m_posTarget			= *pTargetPos;
	m_fTargetDir		= fTargetDir;
	SetCommandID(OBJECT_COMMAND_MAGIC_SEND);
	return TRUE;
}

VOID CObjectCommand_MagicSend::CleanUp(VOID)
{
	m_nMagicID			= INVALID_ID;
	m_nTargetObjID		= INVALID_ID;
	m_posTarget			= WORLD_POS(-1.f, -1.f);
	m_fTargetDir		= -1.f;
	CObjectCommand_Logic::CleanUp();
}

/////////////////////////////////////////////
// CObjectCommand_MagicCharge 法术聚气
/////////////////////////////////////////////
CObjectCommand_MagicCharge::CObjectCommand_MagicCharge(VOID)
{
	m_nMagicID			= INVALID_ID;
	m_nTargetObjID		= INVALID_ID;
	//m_posTarget;
	m_fTargetDir		= -1.f;
	m_uTotalTime		= UINT_MAX;
	m_uEndTime			= UINT_MAX;
}

CObjectCommand_MagicCharge::~CObjectCommand_MagicCharge(VOID)
{
}

BOOL CObjectCommand_MagicCharge::Init(UINT uStartTime,
									  INT nLogicCount,
									  INT nMagicID,
									  ObjID_t nTargetID,
									  const WORLD_POS *pTargetPos,
									  FLOAT fTargetDir,
									  UINT uTotalTime)
{
	SetStartTime(uStartTime);
	SetLogicCount(nLogicCount);
	m_nMagicID			= nMagicID;
	m_nTargetObjID		= nTargetID;
	m_posTarget			= *pTargetPos;
	m_fTargetDir		= fTargetDir;
	m_uTotalTime		= uTotalTime;
	m_uEndTime			= m_uTotalTime;
	SetCommandID(OBJECT_COMMAND_MAGIC_CHARGE);
	return TRUE;
}

VOID CObjectCommand_MagicCharge::CleanUp(VOID)
{
	m_nMagicID			= INVALID_ID;
	m_nTargetObjID		= INVALID_ID;
	m_posTarget			= WORLD_POS(-1.f, -1.f);
	m_fTargetDir		= -1.f;
	m_uTotalTime		= UINT_MAX;
	m_uEndTime			= UINT_MAX;
	CObjectCommand_Logic::CleanUp();
}

BOOL CObjectCommand_MagicCharge::Modify(const CObjectCommand_StopLogic *pCmd)
{
	if(pCmd != NULL && pCmd->GetCommandID() == OBJECT_COMMAND_STOP_ACTION)
	{
		CObjectCommand_StopAction *pStopCmd = (CObjectCommand_StopAction*)pCmd;
		UINT uEndTime = 0;
		if(m_uTotalTime > pStopCmd->GetEndTime())
		{
			uEndTime = m_uTotalTime - pStopCmd->GetEndTime();
		}
		if(uEndTime < m_uEndTime)
		{
			m_uEndTime = uEndTime;
		}
		return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////////////
// CObjectCommand_MagicChannel 法术引导
/////////////////////////////////////////////
CObjectCommand_MagicChannel::CObjectCommand_MagicChannel(VOID)
{
	m_nMagicID			= INVALID_ID;
	m_nTargetObjID		= INVALID_ID;
	//m_posTarget;
	m_fTargetDir		= -1.f;
	m_uTotalTime		= UINT_MAX;
	m_uEndTime			= UINT_MAX;
}

CObjectCommand_MagicChannel::~CObjectCommand_MagicChannel(VOID)
{
}

BOOL CObjectCommand_MagicChannel::Init(UINT uStartTime,
									  INT nLogicCount,
									  INT nMagicID,
									  ObjID_t nTargetID,
									  const WORLD_POS *pTargetPos,
									  FLOAT fTargetDir,
									  UINT uTotalTime)
{
	SetStartTime(uStartTime);
	SetLogicCount(nLogicCount);
	m_nMagicID			= nMagicID;
	m_nTargetObjID		= nTargetID;
	m_posTarget			= *pTargetPos;
	m_fTargetDir		= fTargetDir;
	m_uTotalTime		= uTotalTime;
	m_uEndTime			= 0;
	SetCommandID(OBJECT_COMMAND_MAGIC_CHANNEL);
	return TRUE;
}

VOID CObjectCommand_MagicChannel::CleanUp(VOID)
{
	m_nMagicID			= INVALID_ID;
	m_nTargetObjID		= INVALID_ID;
	m_posTarget			= WORLD_POS(-1.f, -1.f);
	m_fTargetDir		= -1.f;
	m_uTotalTime		= UINT_MAX;
	m_uEndTime			= UINT_MAX;
	CObjectCommand_Logic::CleanUp();
}

BOOL CObjectCommand_MagicChannel::Modify(const CObjectCommand_StopLogic *pCmd)
{
	if(pCmd != NULL && pCmd->GetCommandID() == OBJECT_COMMAND_STOP_ACTION)
	{
		CObjectCommand_StopAction *pStopCmd = (CObjectCommand_StopAction*)pCmd;
		UINT uEndTime = pStopCmd->GetEndTime();
		if(uEndTime > m_uEndTime)
		{
			m_uEndTime = uEndTime;
		}
		return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////////////
// CObjectCommand_Ability 生活技能
/////////////////////////////////////////////
CObjectCommand_Ability::CObjectCommand_Ability(VOID)
{
	m_nAbilityID		= INVALID_ID;
	m_nPrescriptionID	= INVALID_ID;
	m_nTargetObjID		= INVALID_ID;
	m_uEndTime			= UINT_MAX;
}

CObjectCommand_Ability::~CObjectCommand_Ability(VOID)
{
}

BOOL CObjectCommand_Ability::Init(UINT uStartTime, INT nLogicCount, INT nAbilityID, INT nPrescriptionID, ObjID_t nTargetObjID)
{
	SetStartTime(uStartTime);
	SetLogicCount(nLogicCount);
	m_nAbilityID		= nAbilityID;
	m_nPrescriptionID	= nPrescriptionID;
	m_nTargetObjID		= nTargetObjID;
	m_uEndTime			= UINT_MAX;
	SetCommandID(OBJECT_COMMAND_ABILITY);
	return TRUE;
}

VOID CObjectCommand_Ability::CleanUp(VOID)
{
	m_nAbilityID		= INVALID_ID;
	m_nPrescriptionID	= INVALID_ID;
	m_uEndTime			= UINT_MAX;
	m_nTargetObjID		= INVALID_ID;
	CObjectCommand_Logic::CleanUp();
}

BOOL CObjectCommand_Ability::Modify(const CObjectCommand_StopLogic *pCmd)
{
	if(pCmd != NULL && pCmd->GetCommandID() == OBJECT_COMMAND_STOP_ACTION)
	{
		CObjectCommand_StopAction *pStopCmd = (CObjectCommand_StopAction*)pCmd;
		UINT uEndTime = pStopCmd->GetEndTime();
		if(uEndTime < m_uEndTime)
		{
			m_uEndTime = uEndTime;
		}
		return TRUE;
	}
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////////////////
// 转换函数
/////////////////////////////////////////////////////////////////////////////////////////

CObjectCommand *NewObjectCommand(const SCommand_Object *pCmd)
{
	CObjectCommand *pRetCmd = NULL;
	if(pCmd != NULL)
	{
		switch(pCmd->m_wID)
		{
		case OC_STOP_ACTION:
			{
				CObjectCommand_StopAction *pNewCmd = new CObjectCommand_StopAction;
				INT nLogicCount;
				UINT uEndTime;
				nLogicCount		= pCmd->m_anParam[0];
				uEndTime		= pCmd->m_auParam[1];

				BOOL bResult = pNewCmd->Init(nLogicCount, uEndTime);
				if(bResult)
				{
					pRetCmd = pNewCmd;
				}
				else
				{
					pNewCmd->CleanUp();
					SAFE_DELETE(pNewCmd);
				}
			}
			break;
		case OC_STOP_MOVE:
			{
				CObjectCommand_StopMove *pNewCmd = new CObjectCommand_StopMove;
				INT nLogicCount, nEndNodeIndex;
				WORLD_POS posEnd;
				nLogicCount		= pCmd->m_anParam[0];
				nEndNodeIndex	= pCmd->m_anParam[1];
				posEnd.m_fX		= pCmd->m_afParam[2];
				posEnd.m_fZ		= pCmd->m_afParam[3];

				BOOL bResult = pNewCmd->Init(nLogicCount, nEndNodeIndex, &posEnd);
				if(bResult)
				{
					pRetCmd = pNewCmd;
				}
				else
				{
					pNewCmd->CleanUp();
					SAFE_DELETE(pNewCmd);
				}
			}
			break;
		case OC_ACTION:
			{
				CObjectCommand_Action *pNewCmd = new CObjectCommand_Action;
				UINT uStartTime;
				INT nLogicCount, nActionID;
				uStartTime		= pCmd->m_auParam[0];
				nLogicCount		= pCmd->m_anParam[1];
				nActionID		= pCmd->m_anParam[2];

				BOOL bResult = pNewCmd->Init(uStartTime, nLogicCount, nActionID);
				if(bResult)
				{
					pRetCmd = pNewCmd;
				}
				else
				{
					pNewCmd->CleanUp();
					SAFE_DELETE(pNewCmd);
				}
			}
			break;
		case OC_MOVE:
			{
				CObjectCommand_Move *pNewCmd = new CObjectCommand_Move;
				UINT uStartTime;
				INT nLogicCount, nNodeCount;
				WORLD_POS *paposNode;
				uStartTime		= pCmd->m_auParam[0];
				nLogicCount		= pCmd->m_anParam[1];
				nNodeCount		= pCmd->m_anParam[2];
				paposNode		= (WORLD_POS*)(pCmd->m_apParam[3]);

				BOOL bResult = pNewCmd->Init(uStartTime, nLogicCount, nNodeCount, paposNode);
				if(bResult)
				{
					pRetCmd = pNewCmd;
				}
				else
				{
					pNewCmd->CleanUp();
					SAFE_DELETE(pNewCmd);
				}
			}
			break;
		case OC_MAGIC_SEND:
			{
				CObjectCommand_MagicSend *pNewCmd = new CObjectCommand_MagicSend;
				UINT		uStartTime;
				INT			nLogicCount;
				INT			nMagicID;
				ObjID_t		nTargetObjID;
				WORLD_POS	posTarget;
				FLOAT		fTargetDir;

				uStartTime		= pCmd->m_auParam[0];
				nLogicCount		= pCmd->m_anParam[1];
				nMagicID		= pCmd->m_anParam[2];
				nTargetObjID	= pCmd->m_anParam[3];
				posTarget.m_fX	= pCmd->m_afParam[4];
				posTarget.m_fZ	= pCmd->m_afParam[5];
				fTargetDir		= pCmd->m_afParam[6];

				BOOL bResult = pNewCmd->Init(uStartTime, nLogicCount, nMagicID, nTargetObjID, &posTarget, fTargetDir);
				if(bResult)
				{
					pRetCmd = pNewCmd;
				}
				else
				{
					pNewCmd->CleanUp();
					SAFE_DELETE(pNewCmd);
				}
			}
			break;
		case OC_MAGIC_CHARGE:
			{
				CObjectCommand_MagicCharge *pNewCmd = new CObjectCommand_MagicCharge;
				UINT		uStartTime;
				INT			nLogicCount;
				INT			nMagicID;
				ObjID_t		nTargetObjID;
				WORLD_POS	posTarget;
				FLOAT		fTargetDir;
				UINT		uTotalTime;

				uStartTime		= pCmd->m_auParam[0];
				nLogicCount		= pCmd->m_anParam[1];
				nMagicID		= pCmd->m_anParam[2];
				nTargetObjID	= pCmd->m_anParam[3];
				posTarget.m_fX	= pCmd->m_afParam[4];
				posTarget.m_fZ	= pCmd->m_afParam[5];
				fTargetDir		= pCmd->m_afParam[6];
				uTotalTime		= pCmd->m_auParam[7];

				BOOL bResult = pNewCmd->Init(uStartTime, nLogicCount, nMagicID, nTargetObjID, &posTarget, fTargetDir, uTotalTime);
				if(bResult)
				{
					pRetCmd = pNewCmd;
				}
				else
				{
					pNewCmd->CleanUp();
					SAFE_DELETE(pNewCmd);
				}
			}
			break;
		case OC_MAGIC_CHANNEL:
			{
				CObjectCommand_MagicChannel *pNewCmd = new CObjectCommand_MagicChannel;
				UINT		uStartTime;
				INT			nLogicCount;
				INT			nMagicID;
				ObjID_t		nTargetObjID;
				WORLD_POS	posTarget;
				FLOAT		fTargetDir;
				UINT		uTotalTime;

				uStartTime		= pCmd->m_auParam[0];
				nLogicCount		= pCmd->m_anParam[1];
				nMagicID		= pCmd->m_anParam[2];
				nTargetObjID	= pCmd->m_anParam[3];
				posTarget.m_fX	= pCmd->m_afParam[4];
				posTarget.m_fZ	= pCmd->m_afParam[5];
				fTargetDir		= pCmd->m_afParam[6];
				uTotalTime		= pCmd->m_auParam[7];

				BOOL bResult = pNewCmd->Init(uStartTime, nLogicCount, nMagicID, nTargetObjID, &posTarget, fTargetDir, uTotalTime);
				if(bResult)
				{
					pRetCmd = pNewCmd;
				}
				else
				{
					pNewCmd->CleanUp();
					SAFE_DELETE(pNewCmd);
				}
			}
			break;
		case OC_ABILITY:
			{
				CObjectCommand_Ability *pNewCmd = new CObjectCommand_Ability;
				UINT		uStartTime;
				INT			nLogicCount;
				INT			nAbilityID;
				INT			nPrescriptionID;
				INT			nTargetID;

				uStartTime		= pCmd->m_auParam[0];
				nLogicCount		= pCmd->m_anParam[1];
				nAbilityID		= pCmd->m_anParam[2];
				nPrescriptionID	= pCmd->m_anParam[3];
				nTargetID		= pCmd->m_anParam[4];

				BOOL bResult = pNewCmd->Init(uStartTime, nLogicCount, nAbilityID, nPrescriptionID, nTargetID);
				if(bResult)
				{
					pRetCmd = pNewCmd;
				}
				else
				{
					pNewCmd->CleanUp();
					SAFE_DELETE(pNewCmd);
				}
			}
			break;
		case OC_MODIFY_ACTION:
		default:
			//Assert(FALSE && "NewObjectCommand");
			break;
		}
	}
	return pRetCmd;
}

VOID DeleteObjectCommand(CObjectCommand *pCmd)
{
	if(pCmd != NULL)
	{
		pCmd->CleanUp();
		SAFE_DELETE(pCmd);
		pCmd = NULL;
	}
}
