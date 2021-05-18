#include "stdafx.h"
/*
客户端申请物品上架
*/

#include "CGPlayerShopPartner.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"

#include "GCPlayerShopError.h"
#include "GCPlayerShopUpdatePartners.h"
UINT CGPlayerShopPartnerHandler::Execute( CGPlayerShopPartner* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}
	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	_PLAYERSHOP_GUID nShopID	=	pPacket->GetShopID();			//商店ID
	BYTE			 bOpt		=	pPacket->GetOpt();
	GUID_t			 nGuid		=	pPacket->GetGuid();
	GCPlayerShopError							MsgError;
	GCPlayerShopUpdatePartners					MsgToClient;
	GCPlayerShopUpdatePartners::MsgPartner_t	SendingArray[MAX_PARTNER_PER_SHOP];
	
	PlayerShopManager*	pPlayerShopManager	= pScene->GetPlayerShopManager();
	PlayerShop*		pPlayerShop		= pPlayerShopManager->GetPlayerShopByGUID(nShopID);
	Assert(pPlayerShop);

	//是不是自己的店
	BOOL bIsMine = (pHuman->GetGUID() == pPlayerShop->GetOwnerGuid())? TRUE:FALSE;

	//是不是自己可以管理的店
	BOOL bCanManager = pPlayerShop->IsPartner(pHuman->GetGUID());

	if(bIsMine == FALSE)
	{
		//log...
		return PACKET_EXE_CONTINUE;
	}

	switch(bOpt)
	{
	case CGPlayerShopPartner::OPT_ADD_PARTNER:
		{
			RET_TYPE_PARTNER oResult = pPlayerShop->AddPartner(nGuid);
			if(oResult == RET_TYPE_SUCCESS)
			{//刷新此界面合作伙伴列表
				Partner_t*	pCurParList = pPlayerShop->GetPartnerList();
				INT k = 0;
				for(INT i = 0; i<MAX_PARTNER_PER_SHOP; i++)
				{
					if(pCurParList[i].m_Guid != INVALID_GUID)
					{
						SendingArray[k].CleanUp();
						SendingArray[k].m_Guid = pCurParList[i].m_Guid;
						memcpy(SendingArray[k++].m_szName, pCurParList[i].m_szName, MAX_CHARACTER_NAME);
					}
				}

				MsgToClient.SetShopID(nShopID);
				MsgToClient.SetPartnerNum(k);
				MsgToClient.SetPartnerList(SendingArray);
			}
			else
			{
				if(oResult == RET_TYPE_LIST_FULL)
				{
					MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_PARTNER_LIST_FULL);
				}
				else if(oResult == RET_TYPE_ALREADY_IN_LIST)
				{
					MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_PARTNER_ALREADY_IN_LIST);
				}
				else if(oResult == RET_TYPE_NOT_FIND_IN_WORLD)
				{
					MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_PARTNER_NOT_FIND_IN_WORLD);
				}
				else if(oResult == RET_TYPE_INVALID)
				{
					MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_PARTNER_INVALID);
				}

				pGamePlayer->SendPacket(&MsgError);

				g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopPartnerHandler::ObjName=%s, OPT_ADD_PARTNER, FAILID = %d"
					,pHuman->GetName(), oResult);
				return PACKET_EXE_CONTINUE ;

			}
		}
		break;
	case CGPlayerShopPartner::OPT_DEL_PARTNER:
		{
			RET_TYPE_PARTNER oResult = pPlayerShop->RemovePartner(nGuid);
			if(oResult == RET_TYPE_SUCCESS)
			{//刷新此界面合作伙伴列表
				Partner_t*	pCurParList = pPlayerShop->GetPartnerList();
				INT k = 0;
				for(INT i = 0; i<MAX_PARTNER_PER_SHOP; i++)
				{
					if(pCurParList[i].m_Guid != INVALID_GUID)
					{
						SendingArray[k].CleanUp();
						SendingArray[k].m_Guid = pCurParList[i].m_Guid;
						memcpy(SendingArray[k++].m_szName, pCurParList[i].m_szName, MAX_CHARACTER_NAME);
					}
				}

				MsgToClient.SetShopID(nShopID);
				MsgToClient.SetPartnerNum(k);
				MsgToClient.SetPartnerList(SendingArray);
			}
			else
			{
				if(oResult == RET_TYPE_LIST_EMPTY)
				{
					MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_PARTNER_LIST_EMPTY);
				}
				else if(oResult == RET_TYPE_NOT_FIND_IN_LIST)
				{
					MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_PARTNER_NOT_FIND_IN_LIST);
				}
				else if(oResult == RET_TYPE_INVALID)
				{
					MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_PARTNER_INVALID);
				}

				pGamePlayer->SendPacket(&MsgError);

				g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopPartnerHandler::ObjName=%s, OPT_DEL_PARTNER, FAILID = %d"
					,pHuman->GetName(), oResult);
				return PACKET_EXE_CONTINUE ;
			}
		}
		break;
	default:
		break;
	}

	pGamePlayer->SendPacket(&MsgToClient);

	g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopPartnerHandler::ObjName=%s"
		,pHuman->GetName());

	//pGamePlayer->SendPacket(&MsgToClient);

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
