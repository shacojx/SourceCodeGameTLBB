#include "stdafx.h"
//CGCharAllTitlesHandler.cpp
#include "CGCharUpdateCurTitle.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Monster.h"
#include "Obj_Character.h"
#include "PacketFactoryManager.h"
#include "ServerManager.h"

uint CGCharUpdateCurTitleHandler::Execute( CGCharUpdateCurTitle* pPacket, Player* pPlayer )
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
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID );

	BYTE		titleType	= pPacket->getTitleType();
	SHORT		TitleData	= pPacket->getTitleData();

	Assert(	titleType>=_TITLE::NO_TITLE&& titleType< _TITLE::MAX_NUM_TITLE);

	if(titleType == _TITLE::NO_TITLE)
	{
		pHuman->SetTitle("");
		pHuman->SetTitleType(_TITLE::NO_TITLE);
	}
	else if(titleType>=_TITLE::NICK_TITLE )
	{//字符串
		switch(titleType)
		{
		case 	_TITLE::NICK_TITLE :
			{
				pHuman->SetTitle(pHuman->GetNickName());
				pHuman->SetTitleType(_TITLE::NICK_TITLE);
			}
			break;
		case	_TITLE::SPOUSE_TITLE:
			{
				pHuman->SetTitle(pHuman->GetSpouseName());
				pHuman->SetTitleType(_TITLE::SPOUSE_TITLE);
			}
			break;
		case	_TITLE::BANGPAI_TITLE:
			{
				pHuman->SetTitle(pHuman->GetBangPaiName());
				pHuman->SetTitleType(_TITLE::BANGPAI_TITLE);
			}
			break;
		case	_TITLE::JIEBAI_TITLE:
			{
				pHuman->SetTitle(pHuman->GetJieBaiName());
				pHuman->SetTitleType(_TITLE::JIEBAI_TITLE);
			}
		case	_TITLE::SHANGDIAN_TITLE:
			{
				pHuman->SetTitle(pHuman->GetShangDianName());
				pHuman->SetTitleType(_TITLE::SHANGDIAN_TITLE);
			}
			break;
		}
	}
	else
	{//id
		IDTOSTRING(szTmp, TitleData, MAX_CHARACTER_TITLE);
		pHuman->SetTitle(szTmp);
		pHuman->SetTitleType(_TITLE::NO_TITLE);
	}

	g_pLog->FastSaveLog(LOG_FILE_1, "CGCharUpdateCurTitleHandler: ");

	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
