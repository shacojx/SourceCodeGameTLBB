#include "stdafx.h"
/*
客户端申请物品上架
*/

#include "CGPlayerShopFavorite.h"

#include "Type.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"

#include "GCPlayerShopError.h"
#include "GCPlayerShopUpdateFavorite.h"

UINT CGPlayerShopFavoriteHandler::Execute( CGPlayerShopFavorite* pPacket, Player* pPlayer )
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

	BYTE			 bOpt			=	pPacket->GetOpt();
	_PLAYERSHOP_GUID nFavoriteID	=	pPacket->GetFavoriteID();			//商店ID

	GCPlayerShopError MsgError;
	GCPlayerShopUpdateFavorite	MsgToClient;
	switch(bOpt)
	{
	case CGPlayerShopFavorite::OPT_ADD_FAVORITE:
		{
			if(pHuman->AddFavorite( nFavoriteID ) == FALSE)
			{
				MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_ADD_TO_FAVORITE);
				pGamePlayer->SendPacket(&MsgError);

				g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopPartnerHandler::ObjName=%s, ERROR ADD"
					,pHuman->GetName());

				return PACKET_EXE_CONTINUE ;
			}
			MsgToClient.SetShopID(nFavoriteID);
			MsgToClient.SetIsInFavorite(TRUE);
		}
		break;
	case CGPlayerShopFavorite::OPT_DEL_FAVORITE:
		{
			if(pHuman->DelFavorite( nFavoriteID ) == FALSE)
			{
				MsgError.SetID(PLAYERSHOP_MSG::ERR_SHOP_DEL_FROM_FAVORITE);
				pGamePlayer->SendPacket(&MsgError);

				g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopPartnerHandler::ObjName=%s, ERROR DEL"
					,pHuman->GetName());

				return PACKET_EXE_CONTINUE ;
			}
			MsgToClient.SetShopID(nFavoriteID);
			MsgToClient.SetIsInFavorite(FALSE);
		}
		break;
	default:
		break;
	}

	g_pLog->FastSaveLog( LOG_FILE_1, "CGPlayerShopPartnerHandler::ObjName=%s"
		,pHuman->GetName());

	pGamePlayer->SendPacket(&MsgToClient);

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
