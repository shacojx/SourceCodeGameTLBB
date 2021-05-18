#include "stdafx.h"


#include "CGRelation.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "PacketFactoryManager.h"
#include "GCRelation.h"
#include "GWRelation.h"
#include "ServerManager.h"

uint CGRelationHandler::Execute( CGRelation* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer;
	Assert( pGamePlayer );

	Obj_Human* pHuman = pGamePlayer->GetHuman();
	Assert( pHuman );

	Scene* pScene = pHuman->getScene();
	if( pScene==NULL )
	{
		Assert(FALSE);
		return PACKET_EXE_ERROR;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

	CG_RELATION* pRelationPacket = pPacket->GetRelation();
	GCRelation Msg;
	INT nErr = 0;

	switch( pRelationPacket->m_Type )
	{
	case REQ_RELATIONLIST:
		{
			GC_RELATIONLIST& RelationList = Msg.GetRelation()->m_RelationList;

			RelationList.CleanUp();

			pHuman->GetHumanRelation()->FillRelationList(RelationList);
			Msg.GetRelation()->m_Type = RET_RELATIONLIST;

			RelationList.SetMood( pHuman->GetHumanRelation()->GetMood() );

			g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: GUID=%X get relation list.",
				pHuman->GetGUID() );

			GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);

			GW_RELATION* pRelation = pMsg->GetRelation();
			pRelation->CleanUp();
			pRelation->m_Type = REQ_RELATIONONLINE;
			pRelation->SetGUID( pHuman->GetGUID() );

			g_pServerManager->SendPacket( pMsg, INVALID_ID );
			g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: GUID=%X inform online message.",
				pHuman->GetGUID() );
		}
		break;
	case REQ_RELATIONINFO:
		{ // 因为可能请求的是临时好友的信息，所以在此不作判断，直接发给 World
			GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GW_RELATION* pRelation = pMsg->GetRelation();
			pRelation->CleanUp();
			pRelation->m_Type = REQ_RELATIONINFO;
			pRelation->SetGUID( pHuman->GetGUID() );
			pRelation->m_RequestInfo.CleanUp();
			pRelation->m_RequestInfo.SetTargetGUID(pRelationPacket->m_RequestInfo.GetTargetGUID());

			if( pRelationPacket->m_RequestInfo.m_uNameSize > 0 )
			{
				pRelation->m_RequestInfo.SetTargetName(pRelationPacket->m_RequestInfo.GetTargetName());
			}

			g_pServerManager->SendPacket( pMsg, INVALID_ID );
			g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: GUID=%X get relation infomation of GUID=%X.",
				pHuman->GetGUID(), pRelationPacket->m_RequestInfo.GetTargetGUID() );

			return PACKET_EXE_CONTINUE;
		}
		break;
	case REQ_VIEWPLAYER:
		{
			GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GW_RELATION* pSend = pMsg->GetRelation();
			pSend->CleanUp();
			pSend->m_Type = REQ_VIEWPLAYER;
			pSend->SetGUID( pHuman->GetGUID() );
			pSend->m_ViewPlayer.CleanUp();
			pSend->m_ViewPlayer.SetTargetName(pRelationPacket->m_ViewPlayer.GetTargetName());

			g_pServerManager->SendPacket( pMsg, INVALID_ID );
			g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: GUID=%X View Player: %s.",
				pHuman->GetGUID(), pRelationPacket->m_ViewPlayer.GetTargetName() );

			return PACKET_EXE_CONTINUE;
		}
		break;
	case REQ_ADDFRIEND:
	case REQ_TEMPFRIEND_TO_FRIEND:
		{
			HumanRelation* pMyRelation = pHuman->GetHumanRelation();
			REQUEST_ADD_RELATION_WITH_GROUP* pMyFriend = &(pRelationPacket->m_AddRelationWithGroup);

			if( pMyFriend->GetTargetGUID() == INVALID_ID )
			{
				if( strlen(pMyFriend->GetTargetName()) < 1 )
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s attempt to add a friend without GUID and name.",
						pHuman->GetName() );
					return PACKET_EXE_CONTINUE;
				}
				else if( strcmp(pMyFriend->GetTargetName(), pHuman->GetName()) == 0 )
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s attempt to add self as a friend.",
						pHuman->GetName() );
					return PACKET_EXE_CONTINUE;
				}

			}
			else if( pMyFriend->GetTargetGUID() == pHuman->GetGUID() )
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s attempt to add self as a friend.",
					pHuman->GetName() );
				return PACKET_EXE_CONTINUE;
			}

			RELATION_GROUP grp = (RELATION_GROUP)pMyFriend->GetGroup();
			switch( grp )
			{ // 只能是以下分组
			case RELATION_GROUP_F1:
			case RELATION_GROUP_F2:
			case RELATION_GROUP_F3:
			case RELATION_GROUP_F4:
				break;
			default:
				AssertEx( FALSE, "Invalid Friend Group.");
				return PACKET_EXE_CONTINUE;
			}

			// 看看是否还有空位
			if( pMyRelation->IsGroupFull( grp ) )
			{ // 没有空位了
				nErr = RET_ERR_GROUPISFULL;
				break;
			}

			// 看看目前关系表中已经有该玩家（不管是朋友还是黑名单）
			RELATION_TYPE RelationType;

			if( pMyFriend->GetTargetGUID() != INVALID_ID )
			{ // 也许是误操作或者重复操作导致多次加入同一个好友等
				RelationType = pMyRelation->GetRelationType( pMyFriend->GetTargetGUID() );
			}
			else
			{
				RelationType = RELATION_TYPE_STRANGER;
			}

			switch( RelationType )
			{
			case RELATION_TYPE_FRIEND:
				nErr = RET_ERR_ISFRIEND;
				break;
			case RELATION_TYPE_BLACKNAME:
				nErr = RET_ERR_ISBLACKNAME;
				break;
			case RELATION_TYPE_STRANGER:
				{ // 正确关系
					GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);

					GW_RELATION* pRelation = pMsg->GetRelation();
					pRelation->CleanUp();
					pRelation->m_Type = pRelationPacket->m_Type;
					pRelation->SetGUID( pHuman->GetGUID() );
					REQUEST_ADD_RELATION_WITH_GROUP& Relation = pRelation->m_AddRelationWithGroup;
					Relation.CleanUp();
					Relation.SetTargetGUID( pMyFriend->GetTargetGUID() );
					Relation.SetTargetName( pMyFriend->GetTargetName() );
					Relation.SetRelationType( pMyFriend->GetRelationType() );
					Relation.SetGroup( pMyFriend->GetGroup() );

					g_pServerManager->SendPacket( pMsg, INVALID_ID );
					g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s add Name=%s as a new friend.",
						pHuman->GetName(), pMyFriend->GetTargetName() );

					return PACKET_EXE_CONTINUE;
				}
				break;
			default:
				nErr = RET_ERR_RELATIONUNKNOWN;
				Assert(FALSE);
				break;
			}
		}
		break;
	case REQ_ADDTOBLACKLIST:
	case REQ_TEMPFRIEND_ADDTO_BLACKLIST:
		{
			HumanRelation* pMyRelation = pHuman->GetHumanRelation();
			REQUEST_ADD_RELATION* pNewBlackName = &(pRelationPacket->m_AddRelation);

			if( pNewBlackName->GetTargetGUID() == INVALID_ID )
			{
				if( strlen(pNewBlackName->GetTargetName()) < 1 )
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s attempt to add a blackname without GUID and name.",
						pHuman->GetName() );
					return PACKET_EXE_CONTINUE;
				}
				else if( strcmp(pNewBlackName->GetTargetName(), pHuman->GetName()) == 0 )
				{
					g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s attempt to add self into blacklist.",
						pHuman->GetName() );
					return PACKET_EXE_CONTINUE;
				}

			}
			else if( pNewBlackName->GetTargetGUID() == pHuman->GetGUID() )
			{
				g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s attempt to add self into blacklist.",
					pHuman->GetName() );
				return PACKET_EXE_CONTINUE;
			}

			// 看看目前关系表中已经有该玩家（不管是朋友还是黑名单）
			RELATION_TYPE RelationType;

			if( pNewBlackName->GetTargetGUID() != INVALID_ID )
			{
				RelationType = pMyRelation->GetRelationType( pNewBlackName->GetTargetGUID() );
			}
			else
			{
				RelationType = RELATION_TYPE_STRANGER;
			}

			switch( RelationType )
			{
			case RELATION_TYPE_BLACKNAME:
				nErr = RET_ERR_ISBLACKNAME;
				break;
			case RELATION_TYPE_MARRY:
				nErr = RET_ERR_SPOUSETOBLACKLIST;
				break;
			case RELATION_TYPE_MASTER:
				nErr = RET_ERR_MASTERTOBLACKLIST;
				break;
			case RELATION_TYPE_PRENTICE:
				nErr = RET_ERR_PRENTICETOBLACKLIST;
				break;
			case RELATION_TYPE_BROTHER:
				nErr = RET_ERR_BROTHERTOBLACKLIST;
				break;
			case RELATION_TYPE_FRIEND:
				{ // 用转换
					if( pMyRelation->RelationTransition( RELATION_TYPE_BLACKNAME, pNewBlackName->GetTargetGUID() ) != TRUE )
					{
						nErr = RET_ERR_RELATIONUNKNOWN;
					}
					else
					{
						Msg.GetRelation()->m_Type = RET_TRANSITION;

						RELATION_GUID_UCHAR_UCHAR& RelationInfo = Msg.GetRelation()->m_RelationGUIDUCHARUCHAR;
						RelationInfo.CleanUp();
						RelationInfo.SetTargetGUID( pNewBlackName->GetTargetGUID() );
						RelationInfo.SetRelationType( RELATION_TYPE_BLACKNAME );
						RelationInfo.SetGroup( RELATION_GROUP_BLACK );

						g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s add GUID=%X to black list.",
							pHuman->GetName(), pNewBlackName->GetTargetGUID() );
					}
				}
				break;
			case RELATION_TYPE_STRANGER:
				{ // 正确关系
					GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
					GW_RELATION* pRelation = pMsg->GetRelation();
					pRelation->CleanUp();
					pRelation->m_Type = pRelationPacket->m_Type;
					pRelation->SetGUID( pHuman->GetGUID() );
					REQUEST_ADD_RELATION& Relation = pRelation->m_AddRelation;
					Relation.CleanUp();
					Relation.SetTargetGUID( pNewBlackName->GetTargetGUID() );
					Relation.SetTargetName( pNewBlackName->GetTargetName() );
					Relation.SetRelationType( pNewBlackName->GetRelationType() );

					g_pServerManager->SendPacket( pMsg, INVALID_ID );
					g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: Name=%s add Name=%s to black list.",
						pHuman->GetName(), pNewBlackName->GetTargetName() );

					return PACKET_EXE_CONTINUE;
				}
				break;
			default:
				nErr = RET_ERR_RELATIONUNKNOWN;
				Assert(FALSE);
				break;
			}
		}
		break;
	case REQ_TRANSITION:
		{
			HumanRelation* pMyRelation = pHuman->GetHumanRelation();
			RELATION_GUID_UCHAR_UCHAR* pTrans = &(pRelationPacket->m_RelationGUIDUCHARUCHAR);
			GUID_t guid = pTrans->GetTargetGUID();

			// 先判断是否好友的各个组之间的转换
			RELATION_TYPE oldRelationType = pMyRelation->GetRelationType( guid );

			if( pMyRelation->IsFriend( guid )
			 && oldRelationType == (RELATION_TYPE)pTrans->GetRelationType()
			 ) // 好友换组，包括配偶，师徒，结拜兄弟等等
			{
				RELATION_GROUP grp = (RELATION_GROUP)pTrans->GetGroup();
				switch( grp )
				{ // 只能是以下分组
				case RELATION_GROUP_F1:
				case RELATION_GROUP_F2:
				case RELATION_GROUP_F3:
				case RELATION_GROUP_F4:
					break;
				default:
					AssertEx( FALSE, "Invalid Friend Group.");
					return PACKET_EXE_CONTINUE;
				}

				// 看看是否还有空位
				if( pMyRelation->IsGroupFull( grp ) )
				{ // 没有空位了
					nErr = RET_ERR_GROUPISFULL;
				}
				else
				{
					pMyRelation->SetFriendGroup( guid, grp );

					Msg.GetRelation()->m_Type = RET_TRANSITION;
					Msg.GetRelation()->m_RelationGUIDUCHARUCHAR = *pTrans;

					g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: GUID=%X transition GUID=%X to friend group %d.",
						pHuman->GetGUID(), guid, grp );
				}
			}
			else
			{ // 其他的转换情况
				if( pMyRelation->CanTransition( oldRelationType, (RELATION_TYPE)pTrans->GetRelationType() ) != TRUE )
				{
					nErr = RET_ERR_CANNOTTRANSITION;
				}
				else if( pMyRelation->RelationTransition(
					(RELATION_TYPE)pTrans->GetRelationType(), guid
					) != TRUE )
				{
					nErr = RET_ERR_RELATIONUNKNOWN;
				}
				else
				{
					Msg.GetRelation()->m_Type = RET_TRANSITION;
					Msg.GetRelation()->m_RelationGUIDUCHARUCHAR = *pTrans;

					GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
					GW_RELATION* pRelation = pMsg->GetRelation();
					pRelation->CleanUp();
					pRelation->m_Type = pRelationPacket->m_Type;
					pRelation->SetGUID( pHuman->GetGUID() );
					RELATION_GUID_UCHAR& Relation = pRelation->m_TransitionRelation;
					Relation.CleanUp();
					Relation.SetTargetGUID( guid );
					Relation.SetRelationType( pTrans->GetRelationType() );

					g_pServerManager->SendPacket( pMsg, INVALID_ID );

					g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: GUID=%X transition GUID=%X from %d to %d.",
						pHuman->GetGUID(), guid, oldRelationType, pTrans->GetRelationType() );
				}
			}
		}
		break;
	case REQ_DELFRIEND:
		{
			HumanRelation* pMyRelation = pHuman->GetHumanRelation();
			GUID_t guid = pRelationPacket->m_DelFriend.GetTargetGUID();

			// 看是否有这么个朋友
			RELATION_TYPE RelationType = pMyRelation->GetRelationType( guid );

			switch( RelationType )
			{
			case RELATION_TYPE_MARRY:
				nErr = RET_ERR_DELSPOUSE;
				break;
			case RELATION_TYPE_MASTER:
				nErr = RET_ERR_DELMASTER;
				break;
			case RELATION_TYPE_PRENTICE:
				nErr = RET_ERR_DELPRENTICE;
				break;
			case RELATION_TYPE_BROTHER:
				nErr = RET_ERR_DELBROTHER;
				break;
			case RELATION_TYPE_FRIEND: // 正常情况
				break;
			default:
				nErr = RET_ERR_ISNOTFRIEND;
				break;
			}

			if ( nErr )
			{ // 如果有问题，马上退出
				break;
			}

			if( pMyRelation->GetRelationInfo( guid )->m_FriendPoint >= g_Config.m_ConfigInfo.m_nPasswdPoint
			 && (pHuman->__IsPasswordSetup() && pHuman->__IsPasswordUnlock() != TRUE)
			 )
			{ // 看是否需要对比二级密码
				nErr = RET_ERR_PASSWDMISMATCH;
			}
			else if( pMyRelation->DelRelation( RELATION_TYPE_FRIEND, guid ) != TRUE )
			{ // 删掉它
				nErr = RET_ERR_RELATIONUNKNOWN;
			}
			else
			{
				Msg.GetRelation()->m_Type = RET_DELFRIEND;
				Msg.GetRelation()->m_RelationGUID.CleanUp();
				Msg.GetRelation()->m_RelationGUID.SetTargetGUID( guid );

				GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
				GW_RELATION* pRelation = pMsg->GetRelation();
				pRelation->CleanUp();
				pRelation->m_Type = pRelationPacket->m_Type;
				pRelation->SetGUID( pHuman->GetGUID() );
				RELATION_GUID& Relation = pRelation->m_RelationGUID;
				Relation.CleanUp();
				Relation.SetTargetGUID( guid );

				g_pServerManager->SendPacket( pMsg, INVALID_ID );

				g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: GUID=%X del GUID=%X from friend list.",
					pHuman->GetGUID(), guid );
			}
		}
		break;
	case REQ_DELFROMBLACKLIST:
		{
			HumanRelation* pMyRelation = pHuman->GetHumanRelation();
			GUID_t guid = pRelationPacket->m_RelationGUID.GetTargetGUID();

			// 看是否存在于黑名单
			if( pMyRelation->HaveRelation( RELATION_TYPE_BLACKNAME, guid ) != TRUE )
			{
				nErr = RET_ERR_ISNOTINBLACKLIST;
			}
			else if( pMyRelation->DelRelation( RELATION_TYPE_BLACKNAME, guid ) != TRUE )
			{ // 删掉它
				nErr = RET_ERR_RELATIONUNKNOWN;
			}
			else
			{
				Msg.GetRelation()->m_Type = RET_DELFROMBLACKLIST;
				Msg.GetRelation()->m_RelationGUID.CleanUp();
				Msg.GetRelation()->m_RelationGUID.SetTargetGUID( guid );

				GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
				GW_RELATION* pRelation = pMsg->GetRelation();
				pRelation->CleanUp();
				pRelation->m_Type = pRelationPacket->m_Type;
				pRelation->SetGUID( pHuman->GetGUID() );
				RELATION_GUID& Relation = pRelation->m_RelationGUID;
				Relation.CleanUp();
				Relation.SetTargetGUID( guid );

				g_pServerManager->SendPacket( pMsg, INVALID_ID );

				g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: GUID=%X del GUID=%X from black list.",
					pHuman->GetGUID(), guid );
			}
		}
		break;
	case REQ_MODIFYMOOD:
		{
			pHuman->GetHumanRelation()->SetMood( pRelationPacket->m_ModifyMood.GetMood() );
			Msg.GetRelation()->m_Type = RET_NEWMOOD;
			Msg.GetRelation()->m_NewMood.SetMood( pHuman->GetHumanRelation()->GetMood() );

			GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GW_RELATION* pRelation = pMsg->GetRelation();
			pRelation->CleanUp();
			pRelation->m_Type = pRelationPacket->m_Type;
			pRelation->SetGUID( pHuman->GetGUID() );
			REQUEST_MODIFY_MOOD& Relation = pRelation->m_ModifyMood;
			Relation.CleanUp();
			Relation.SetMood( pHuman->GetHumanRelation()->GetMood() );

			//如果当前显示的心情被置为称号，那么同时更新当前称号
			if( pHuman->GetTitleType() == _TITLE::MOOD_TITLE )
			{
				pHuman->SetTitleType(_TITLE::MOOD_TITLE);
				pHuman->SetTitle(pHuman->GetHumanRelation()->GetMood());
			}

			g_pServerManager->SendPacket( pMsg, INVALID_ID );
			g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: %s modify mood.", pHuman->GetName() );
		}
		break;
	case REQ_NOTIFY_ADDTEMPFRIEND:
		{
			GWRelation* pMsg = (GWRelation*)g_pPacketFactoryManager->CreatePacket(PACKET_GW_RELATION);
			GW_RELATION* pRelation = pMsg->GetRelation();
			pRelation->CleanUp();
			pRelation->m_Type = pRelationPacket->m_Type;
			pRelation->SetGUID( pHuman->GetGUID() );
			REQUEST_VIEW_PLAYER& Relation = pRelation->m_ViewPlayer;
			Relation.CleanUp();
			Relation.SetTargetName( pRelationPacket->m_ViewPlayer.GetTargetName() );

			g_pServerManager->SendPacket( pMsg, INVALID_ID );

			g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: GUID=%X add Name=%s from black list.",
				pHuman->GetGUID(), Relation.GetTargetName() );
		}
		break;
	default :
		{
			Assert(FALSE);
			return PACKET_EXE_CONTINUE;
		}
		break;
	}

	if ( nErr )
	{
		Msg.GetRelation()->m_Type = nErr;

		g_pLog->FastSaveLog( LOG_FILE_1, "CGRelationHandler: GUID=%X Error code %d.",
			pHuman->GetGUID(), nErr );
	}

	pGamePlayer->SendPacket( &Msg );

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
