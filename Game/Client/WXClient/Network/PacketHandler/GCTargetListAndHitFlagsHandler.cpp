#include "StdAfx.h"
#include "TDTimeSystem.h"
#include "TDException.h"
#include "..\..\Global.h"
#include "..\..\Dbc\GMDataBase.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Object.h"
#include "..\..\Object\Character\Obj_Character.h"
#include "..\..\Object\Surface\Obj_Special.h"
#include "..\..\GameCommand.h"
#include "..\..\Object\ObjectCommandDef.h"
#include "GCTargetListAndHitFlags.h"

using namespace Packets;

uint GCTargetListAndHitFlagsHandler :: Execute( GCTargetListAndHitFlags* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		CObjectManager* pObjectManager = CObjectManager::GetMe();

		CObject* pObj = (CObject*)(pObjectManager->FindServerObject( (INT)pPacket->GetObjID() ));
		if ( pObj == NULL )
			return PACKET_EXE_CONTINUE;

		const CHAR *pszSenderLocator = NULL;
		BOOL bHitCharacter = FALSE;
		INT nBulletID	= INVALID_ID;
		if(GCTargetListAndHitFlags::SPECIAL_OBJ_ACTIVATE == pPacket->GetDataType())
		{
			DBC_DEFINEHANDLE(s_pSpecialDataDBC, DBC_SPECIAL_OBJ_DATA);
			const _DBC_SPECIAL_OBJ_DATA *pSpecialObjData = (const _DBC_SPECIAL_OBJ_DATA*)s_pSpecialDataDBC->Search_Index_EQU(pPacket->GetSkillOrSpecialObjDataID());
			if(pSpecialObjData != NULL)
			{
				nBulletID			= pSpecialObjData->m_nBulletID;
				bHitCharacter		= TRUE;
				pszSenderLocator	= NULL;
			}
		}
		else
		{
			DBC_DEFINEHANDLE(s_pSkillTable, DBC_SKILL_DATA);
			const _DBC_SKILL_DATA* pSkillData = (const _DBC_SKILL_DATA*)s_pSkillTable->Search_Index_EQU(pPacket->GetSkillOrSpecialObjDataID());
			if(pSkillData != NULL)
			{
				nBulletID			= pSkillData->m_nBulletID;
				pszSenderLocator	= pSkillData->m_pszBulletSendLocator;
				switch(pSkillData->m_nSelectType)
				{
				case SELECT_TYPE_POS:
				case SELECT_TYPE_DIR:
					bHitCharacter	= FALSE;
					break;
				case SELECT_TYPE_NONE:
				case SELECT_TYPE_CHARACTER:
				case SELECT_TYPE_SELF:
				default:
					bHitCharacter	= TRUE;
					break;
				}
			}
		}

		if(nBulletID != INVALID_ID)
		{
			_LOGIC_EVENT_BULLET infoBullet;
			infoBullet.m_nBulletID			= nBulletID;
			infoBullet.m_pszSenderLocator	= pszSenderLocator;
			infoBullet.m_bHitTargetObj		= bHitCharacter;
			if(bHitCharacter)
			{
				_LOGIC_EVENT logicEvent;
				logicEvent.Init(pPacket->GetObjID(), pPacket->GetLogicCount(), &infoBullet);

				SCommand_Object cmdTemp;
				cmdTemp.m_wID			= OC_LOGIC_EVENT;
				cmdTemp.m_apParam[0]	= (VOID*)(&logicEvent);

				if(g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
				{
					CObject_Character *pCharacter = (CObject_Character*)pObj;

					INT nNum = (INT)(pPacket->GetTargetNum());
					INT i;
					for(i = 0; i < nNum; i++)
					{
						logicEvent.m_bullet.m_nTargetID		= pPacket->GetTargetObjIDByIndex(i);
						pCharacter->PushCommand(&cmdTemp);
					}
				}
				else if(g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Special)))
				{
					CObject_Special *pSpecial = (CObject_Special*)pObj;

					//INT nNum = (INT)(pPacket->GetTargetNum());
					//INT i;
					//for(i = 0; i < nNum; i++)
					//{
					//	logicEvent.m_bullet.m_nTargetID		= pPacket->GetTargetObjIDByIndex(i);
					//	pObj->PushCommand(&cmdTemp);
					//}
				}
			}
			else
			{
				infoBullet.m_fTargetX		= pPacket->GetTargetPos().m_fX;
				infoBullet.m_fTargetZ		= pPacket->GetTargetPos().m_fZ;

				_LOGIC_EVENT logicEvent;
				logicEvent.Init(pPacket->GetObjID(), pPacket->GetLogicCount(), &infoBullet);

				SCommand_Object cmdTemp;
				cmdTemp.m_wID			= OC_LOGIC_EVENT;
				cmdTemp.m_apParam[0]	= (VOID*)(&logicEvent);

				if(g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Character)))
				{
					CObject_Character *pCharacter = (CObject_Character*)pObj;
					pCharacter->PushCommand(&cmdTemp );
				}
				else if(g_theKernel.IsKindOf(pObj->GetClass(), GETCLASS(CObject_Special)))
				{
					CObject_Special *pSpecial = (CObject_Special*)pObj;
				}
			}
		}

		pObj->PushDebugString("GCTargetListAndHitFlags");
		pObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
	}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

