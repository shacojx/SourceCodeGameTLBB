

#include "StdAfx.h"
#include "GCRelation.h"

#include "DB_Struct.h"

#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"


#include "..\..\DataPool\GMDataPool.h"
#include "..\..\DataPool\GMDP_Struct_Relation.h"

//#include "..\..\Object\ObjectManager.h"
//#include "..\..\Object\Character\Obj_PlayerMySelf.h"
//#include "..\..\DataPool\GMDP_CharacterData.h"


const char* pRelationString[] = {
	"",
	"Target_Not_Exist",				// RET_ERR_TARGETNOTEXIST
	"Group_Is_Full",					// RET_ERR_GROUPISFULL
	"Is_Firend",						// RET_ERR_ISFRIEND
	"Is_BlackName",					// RET_ERR_ISBLACKNAME
	"Can_Not_Transition",			// RET_ERR_CANNOTTRANSITION
	"Is_Not_Firend",					// RET_ERR_ISNOTFRIEND
	"Is_Not_In_BlackList",			// RET_ERR_ISNOTINBLACKLIST
	"Spouse_To_Black__List",			// RET_ERR_SPOUSETOBLACKLIST
	"Master_To_Black_List",			// RET_ERR_MASTERTOBLACKLIST
	"Prentice_To_Black_List",		// RET_ERR_PRENTICETOBLACKLIST
	"Brother_To_Black__List",		// RET_ERR_BROTHERTOBLACKLIST
	"Del_Spouse",					// RET_ERR_DELSPOUSE
	"Del_Master",					// RET_ERR_DELMASTER
	"Del_Prentice",					// RET_ERR_DELPRENTICE
	"Del_Brother",					// RET_ERR_DELBROTHER
	"Password_Err",					// RET_ERR_PASSWDMISMATCH
	"Reject_Add_Firend",				// RET_ERR_CANNOT_ADDFRIEND
	"Reject_Mail",					// RET_ERR_CANNOTRECEIVEMAIL
	"Reject_Mail_From_stranger",		// RET_ERR_NOTRECVSTRANGEMAIL
	"Unknow_Err",					// RET_ERR_RELATIONUNKNOWN
//	"。",
};

uint GCRelationHandler::Execute( GCRelation* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	{
		if(!CGameProcedure::s_pUISystem) return PACKET_EXE_CONTINUE;

		GC_RELATION* pReturn = pPacket->GetRelation();
		CHAR szText[_MAX_PATH];

		_snprintf(szText, _MAX_PATH, "其他好友操作。");

		Relation* pRelation = CDataPool::GetMe()->GetRelation();
		if ( pRelation == NULL )
		{
			Assert( FALSE );
			return PACKET_EXE_CONTINUE;
		}

		switch( pReturn->m_Type )
		{
		case RET_RELATIONLIST:
			{
				GC_RELATIONLIST* pRelationList = &(pReturn->m_RelationList);

				pRelation->SetMood( pRelationList->GetMood() );

				for( INT i=0; i<pRelationList->GetFriendCount(); ++i )
				{
					pRelation->AddRelation( pRelationList->GetFriend(i) );
				}

				for( INT i=0; i<pRelationList->GetBlackCount(); ++i )
				{
					pRelation->AddRelation( pRelationList->GetBlackName(i) );
				}

				// 需要 push 一个事件通知 UI
				pRelation->UpdateUIList();
				return PACKET_EXE_CONTINUE;
			}
			break;
		case RET_RELATIONINFO:
			{
				pRelation->UpdateRelationInfo( &pReturn->m_RelationInfo );
				return PACKET_EXE_CONTINUE;
			}
			break;
		case RET_TEMPFRIEND_TO_FRIEND:
		case RET_TEMPFRIEND_ADDTO_BLACKLIST:
		case RET_ADDFRIEND:
		case RET_ADDTOBLACKLIST:
			{
				RETURN_ADD_RELATION* pAddRelation = &(pReturn->m_NewRelation);
				RELATION_GROUP Drg;

				if ( pAddRelation->GetRelationType() == (UCHAR)RELATION_TYPE_BLACKNAME )
				{
					_snprintf(szText, _MAX_PATH, "你将%s加入黑名单。", pAddRelation->GetRelationData()->GetName());
					Drg = RELATION_GROUP_BLACK;
				}
				else if ( pAddRelation->GetRelationType() == (UCHAR)RELATION_TYPE_FRIEND )
				{
					_snprintf(szText, _MAX_PATH, "你将%s添加为好友。", pAddRelation->GetRelationData()->GetName());
					Drg = (RELATION_GROUP)pAddRelation->GetGroup();
				}
				else
				{
					_snprintf(szText, _MAX_PATH, "意外的关系类型：%d。", pAddRelation->GetRelationType());
					AssertEx(FALSE, szText);
					return PACKET_EXE_CONTINUE;
				}

				_RELATION* pRelationData;
				pRelationData = pAddRelation->GetRelationData();

				// 如果是临时好友，则移除
				pRelation->RemoveRelation( pRelationData->GetGUID() );
				pRelation->RemoveRelation( RELATION_GROUP_TEMPFRIEND, pRelationData->GetName() );

				SDATA_RELATION_MEMBER Member;

				Member.m_GUID = pRelationData->GetGUID();
				strncpy( Member.m_szName, pRelationData->GetName(), sizeof(Member.m_szName) - 1 );
				Member.m_RelationType = (RELATION_TYPE)pAddRelation->GetRelationType();
				Member.m_nLevel = pRelationData->GetLevel();
				Member.m_nMenPai = pRelationData->GetMenPai();
				Member.m_nPortrait = pRelationData->GetPortrait();
				Member.m_GuildID = pRelationData->GetGuildID();
				strncpy( Member.m_szGuildName, pRelationData->GetGuildName(), sizeof(Member.m_szGuildName) - 1 );
				Member.m_bOnlineFlag = (BOOL)pRelationData->GetOnlineFlag();

				if( Member.m_bOnlineFlag )
				{
					strncpy( Member.m_szMood, pRelationData->GetMood(), sizeof(Member.m_szMood) - 1 );
					strncpy( Member.m_szTitle, pRelationData->GetTitle(), sizeof(Member.m_szTitle) - 1 );
					Member.m_SceneID = pRelationData->GetSceneID();
					Member.m_nTeamSize = pRelationData->GetTeamSize();
				}

				pRelation->AddRelation( Drg, &Member );
			}
			break;
		case RET_TRANSITION:
			{
				RELATION_GUID_UCHAR_UCHAR* pRelationInfo = &(pReturn->m_RelationGUIDUCHARUCHAR);

				RELATION_GROUP Srg;
				INT idx;

				pRelation->GetRelationPosition( pRelationInfo->GetTargetGUID(), Srg, idx );
				pRelation->MoveRelation( Srg, (RELATION_TYPE)pRelationInfo->GetRelationType(),
					(RELATION_GROUP)pRelationInfo->GetGroup(), pRelationInfo->GetTargetGUID() );

				return PACKET_EXE_CONTINUE;
			}
			break;
		case RET_DELFRIEND:
		case RET_DELFROMBLACKLIST:
			{
				pRelation->RemoveRelation( pReturn->m_RelationGUID.GetTargetGUID() );
				return PACKET_EXE_CONTINUE;
			}
			break;
		case RET_ADDFRIENDNOTIFY:
			{
				_snprintf(szText, _MAX_PATH, "%s已经将你添加为好友了。", pReturn->m_NotifyFriend.GetName() );
				ADDTALKMSG(szText);
				CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, szText );
				return PACKET_EXE_CONTINUE;
			}
			break;
		case RET_ONLINELIST:
			{
				RETURN_ONLINE_LIST* pRecv = &(pReturn->m_RelationOnline);

				for( INT i=0; i<pRecv->GetOnlineCount(); ++i )
				{
					pRelation->UpdateOnlineFriend( pRecv->GetOnlineRelation(i) );
				}

				//CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_IsMinorPwdSetup(TRUE);

				return PACKET_EXE_CONTINUE;
			}
			break;
		case RET_RELATIONONLINE:
			{
				RETURN_NOTIFY_ONLINE* pRecv = &(pReturn->m_NotifyOnline);
				pRelation->RelationOnline( pRecv->GetTargetName(), pRecv->GetMood() );
				return PACKET_EXE_CONTINUE;
			}
			break;
		case RET_RELATIONOFFLINE:
			{
				pRelation->RelationOffLine( pReturn->m_RelationGUID.GetTargetGUID() );
				return PACKET_EXE_CONTINUE;
			}
			break;
		case RET_NEWMOOD:
			{
				pRelation->SetMood( pReturn->m_NewMood.GetMood() );
				return PACKET_EXE_CONTINUE;
			}
			break;
		case RET_ERR_PASSWDMISMATCH:
			{
				CEventSystem::GetMe()->PushEvent(GE_MINORPASSWORD_OPEN_UNLOCK_PASSWORD_DLG);
			}
			// break; // 这里不需要 break;
		case RET_ERR_TARGETNOTEXIST:
		case RET_ERR_GROUPISFULL:
		case RET_ERR_ISFRIEND:
		case RET_ERR_ISBLACKNAME:
		case RET_ERR_CANNOTTRANSITION:
		case RET_ERR_ISNOTFRIEND:
		case RET_ERR_ISNOTINBLACKLIST:
		case RET_ERR_SPOUSETOBLACKLIST:
		case RET_ERR_MASTERTOBLACKLIST:
		case RET_ERR_PRENTICETOBLACKLIST:
		case RET_ERR_BROTHERTOBLACKLIST:
		case RET_ERR_DELSPOUSE:
		case RET_ERR_DELMASTER:
		case RET_ERR_DELPRENTICE:
		case RET_ERR_DELBROTHER:
		case RET_ERR_CANNOT_ADDFRIEND:
		case RET_ERR_CANNOTRECEIVEMAIL:
		case RET_ERR_NOTRECVSTRANGEMAIL:
		case RET_ERR_RELATIONUNKNOWN:
			{
				STRING strTemp = "";
				strTemp = NOCOLORMSGFUNC(pRelationString[ pReturn->m_Type - RET_ERR_START ]);
				strncpy(szText, strTemp.c_str(), MAX_PATH-1);
				//strncpy(szText, pRelationString[ pReturn->m_Type - RET_ERR_START ], MAX_PATH-1);
			}
			break;
		case RET_ERR_ISENEMY:
			{
				strncpy(szText, "阵营错误", MAX_PATH-1);
			}
			break;
		default :
			Assert(FALSE);
			return PACKET_EXE_CONTINUE;
		}

		CGameProcedure::s_pEventSystem->PushEvent( GE_NEW_DEBUGMESSAGE, szText );
		ADDTALKMSG(szText);
	}

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
