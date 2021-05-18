#include "StdAfx.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\event\GMEventSystem.h"
#include "..\..\DataPool\GMUIDataPool.h"
#include "..\..\DataPool\TeamOrGroup.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\object\character\Obj_PlayerOther.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "..\..\DataPool\GMDP_CharacterData.h"
#include "GCTeamAskInvite.h"
#include "..\..\Sound\GMSoundSystem.h"

uint GCTeamAskInviteHandler::Execute( GCTeamAskInvite* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION
	
	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{

//		CUIDataPool::GetMe()->SetTeamInvitorGUID( pPacket->GetGUID());
//		CEventSystem::GetMe()->PushEvent(GE_SHOW_TEAM_YES_NO, pPacket->GetDestName());
		CTeamOrGroup* pTeam = CUIDataPool::GetMe()->GetTeamOrGroup();

		if ( pTeam->HasTeam() )
		{ // 有队伍的时候收到信息表示客户端数据出现问题
			
			return PACKET_EXE_CONTINUE;
		}

		InviteTeam* pTeamInfo = new (InviteTeam);
		pTeamInfo->m_InviterGUID = pPacket->GetGUID();

		for( INT i=0; i<pPacket->GetMemberCount(); ++i )
		{
			TeamCacheInfo* pMemberInfo = new(TeamCacheInfo);

			// 创建一个界面的模型.
			pMemberInfo->m_UIModel.CreateNewUIModel();
			const GCTeamAskInvite::InviterInfo* pInviterInfo = pPacket->GetInviterInfo(i);

			strncpy(pMemberInfo->m_szNick, pInviterInfo->m_szNick, pInviterInfo->m_NickSize);
			pMemberInfo->m_szNick[pInviterInfo->m_NickSize] = '\0';
			pMemberInfo->m_uFamily = pInviterInfo->m_uFamily;
			pMemberInfo->m_Scene = pInviterInfo->m_Scene;
			pMemberInfo->m_uLevel = pInviterInfo->m_Level;
			pMemberInfo->m_uDataID = pInviterInfo->m_uDataID;
			pMemberInfo->m_uDetailFlag = pInviterInfo->m_DetailFlag;

			if ( pInviterInfo->m_DetailFlag>0 )
			{
				pMemberInfo->m_WeaponID = pInviterInfo->m_WeaponID;
				pMemberInfo->m_CapID = pInviterInfo->m_CapID;
				pMemberInfo->m_ArmourID = pInviterInfo->m_ArmourID;
				pMemberInfo->m_CuffID = pInviterInfo->m_CuffID;
				pMemberInfo->m_FootID = pInviterInfo->m_FootID;
				pMemberInfo->m_uFaceMeshID = pInviterInfo->m_uFaceMeshID;
				pMemberInfo->m_uHairColor = pInviterInfo->m_uHairColor;
				pMemberInfo->m_uHairMeshID = pInviterInfo->m_uHairMeshID;

				// 设置ui模型
				pMemberInfo->m_UIModel.m_pAvatar->GetCharacterData()->Set_RaceID(pMemberInfo->m_uDataID);
				pMemberInfo->m_UIModel.SetUIModelInfo(HEQUIP_WEAPON,	pInviterInfo->m_WeaponID);
				pMemberInfo->m_UIModel.SetUIModelInfo(HEQUIP_CAP,		pInviterInfo->m_CapID);
				pMemberInfo->m_UIModel.SetUIModelInfo(HEQUIP_ARMOR,		pInviterInfo->m_ArmourID);
				pMemberInfo->m_UIModel.SetUIModelInfo(HEQUIP_CUFF,		pInviterInfo->m_CuffID);
				pMemberInfo->m_UIModel.SetUIModelInfo(HEQUIP_BOOT,		pInviterInfo->m_FootID);

				if(pMemberInfo->m_uFaceMeshID < 255)
				{
					// 设置脸形
					pMemberInfo->m_UIModel.SetFaceMeshId(pMemberInfo->m_uFaceMeshID);
				}

				if(pMemberInfo->m_uHairMeshID < 255)	
				{
					// 设置发型
					pMemberInfo->m_UIModel.SetFaceHairId(pMemberInfo->m_uHairMeshID);
				}
				
				//if(pMemberInfo->m_uHairColor)
				//{
				//	// 设置颜色
				//	pMemberInfo->m_UIModel.SetHairColor(pMemberInfo->m_uHairColor);
				//}
				//else
				//{
				//	pMemberInfo->m_UIModel.SetHairColor(0);
				//}//

				// 设置颜色
				pMemberInfo->m_UIModel.SetHairColor(pMemberInfo->m_uHairColor);

			}

			pTeamInfo->m_InvitersInfo.push_back(pMemberInfo);
		}


		if ( pTeam->AddInviteTeam(pTeamInfo) )
		{
			ADDNEWDEBUGMSG(STRING("接到一个新邀请。"));
			CSoundSystemFMod::_PlayUISoundFunc(60);
		}

		// 通知界面有人邀请你加入队伍.
		CGameProcedure::s_pEventSystem->PushEvent(GE_TEAM_NOTIFY_APPLY, 2);


	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
