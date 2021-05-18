#include "StdAfx.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\event\GMEventSystem.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\DataPool\TeamOrGroup.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"

#include "GCTeamAskApply.h"



#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\Event\GMEventSystem.h"

#include "..\..\DataPool\GMDP_CharacterData.h"
#include "..\..\Sound\GMSoundSystem.h"

uint GCTeamAskApplyHandler::Execute( GCTeamAskApply* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
	//	CUIDataPool::GetMe()->SetSourceGUID( pPacket->GetSourGUID() );
	//	CUIDataPool::GetMe()->SetDestinationGUID( pPacket->GetDestGUID() );
	//	CEventSystem::GetMe()->PushEvent(GE_TEAM_APPLY, pPacket->GetSourName());

		CTeamOrGroup* pTeam = CUIDataPool::GetMe()->GetTeamOrGroup();

		if ( pTeam->HasTeam() == FALSE
		 ||  pTeam->GetLeaderGUID() != CObjectManager::GetMe()->GetMySelf()->GetServerGUID()
		 )
		{ // 没有队伍或者不是队长的时候收到信息表示客户端数据出现问题
			TDAssert( FALSE );
			return PACKET_EXE_CONTINUE;
		}

		TeamCacheInfo* pProposerInfo = new(TeamCacheInfo);

		// 创建一个ui界面模型
		pProposerInfo->m_UIModel.CreateNewUIModel();

		
		pProposerInfo->m_GUID = pPacket->GetSourGUID();
		strncpy(pProposerInfo->m_szNick, pPacket->GetSourName(), MAX_CHARACTER_NAME-1);
		pProposerInfo->m_szNick[MAX_CHARACTER_NAME-1] = '\0';
		pProposerInfo->m_uFamily     = pPacket->GetFamily();
		pProposerInfo->m_Scene       = pPacket->GetScene();
		pProposerInfo->m_uLevel      = pPacket->GetLevel();
		pProposerInfo->m_uDataID  = pPacket->GetDataID();
		pProposerInfo->m_uDetailFlag = pPacket->GetDetailFlag();

		if ( pProposerInfo->m_uDetailFlag>0 )
		{
			pProposerInfo->m_WeaponID = pPacket->GetWeaponID();
			pProposerInfo->m_CapID    = pPacket->GetCapID();
			pProposerInfo->m_ArmourID = pPacket->GetArmourID();
			pProposerInfo->m_CuffID   = pPacket->GetCuffID();
			pProposerInfo->m_FootID   = pPacket->GetFootID();
			pProposerInfo->m_uFaceMeshID = pPacket->GetFaceModel();
			pProposerInfo->m_uHairColor = pPacket->GetHairColor();
			pProposerInfo->m_uHairMeshID = pPacket->GetHairModel();

			pProposerInfo->m_UIModel.m_pAvatar->GetCharacterData()->Set_RaceID(pProposerInfo->m_uDataID);
			// 设置ui模型
			pProposerInfo->m_UIModel.SetUIModelInfo(HEQUIP_WEAPON,	pPacket->GetWeaponID());
			pProposerInfo->m_UIModel.SetUIModelInfo(HEQUIP_CAP,		pPacket->GetCapID());
			pProposerInfo->m_UIModel.SetUIModelInfo(HEQUIP_ARMOR,	pPacket->GetArmourID());
			pProposerInfo->m_UIModel.SetUIModelInfo(HEQUIP_CUFF,	pPacket->GetCuffID());
			pProposerInfo->m_UIModel.SetUIModelInfo(HEQUIP_BOOT,    pPacket->GetFootID());

			if(pProposerInfo->m_uFaceMeshID < 255)
			{
				// 设置脸形
				pProposerInfo->m_UIModel.SetFaceMeshId(pProposerInfo->m_uFaceMeshID);
			}
				
			if(pProposerInfo->m_uHairMeshID < 255)
			{
				// 设置发型
				pProposerInfo->m_UIModel.SetFaceHairId(pProposerInfo->m_uHairMeshID);
			}
			
			//if(pProposerInfo->m_uHairColor < 255)
			//{
			//	// 设置颜色
			//	pProposerInfo->m_UIModel.SetHairColor(pProposerInfo->m_uHairColor);
			//}
			//else
			//{
			//	// 设置颜色
			//	pProposerInfo->m_UIModel.SetHairColor(0);
			//}//

			// 设置颜色
			pProposerInfo->m_UIModel.SetHairColor(pProposerInfo->m_uHairColor);

		}


		if ( pTeam->AddProposer(pProposerInfo) )
		{
			ADDNEWDEBUGMSG(STRING("有队员申请入队啦。"));
			CSoundSystemFMod::_PlayUISoundFunc(60);
		}

		// 通知界面有人申请加入队伍, 闪烁按钮.
		CGameProcedure::s_pEventSystem->PushEvent(GE_TEAM_NOTIFY_APPLY, 1);

	}



__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
