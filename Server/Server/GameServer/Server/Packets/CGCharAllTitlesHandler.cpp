#include "stdafx.h"
//CGCharAllTitlesHandler.cpp
#include "CGCharAllTitles.h"
#include "GCCharAllTitles.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Monster.h"
#include "Obj_Character.h"
uint CGCharAllTitlesHandler::Execute( CGCharAllTitles* pPacket, Player* pPlayer )
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

	ObjID_t ObjID = pPacket->getTargetID() ;
	if( ObjID == INVALID_ID )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	Obj* pObj = pScene->GetObjManager()->GetObj( ObjID ) ;
	if( pObj==NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGCharAskBaseAttribHandler: not find obj=%d", ObjID ) ;
		return PACKET_EXE_CONTINUE ;
	}

	if( !IsCharacterObj( pObj->GetObjType() ) )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGCharAskBaseAttribHandler: not character obj=%d", ObjID ) ;
		return PACKET_EXE_CONTINUE ;
	}

	if(Obj::OBJ_TYPE_HUMAN == pObj->GetObjType() )
	{
		Obj_Human* phum = (Obj_Human*)pObj ;

		//PACKET_ALL_TITLES_MSG
		GCCharAllTitles::TitleId titlelist[MAX_TITLE_ID_SIZE];
		int num = 0;
		if(phum->GetLevelTitle())
		{
			titlelist[num].m_titleid		=	_TITLE::LEVEL_TITLE;
			titlelist[num++].m_titleData	=	static_cast<SHORT>(phum->GetLevelTitle());
		}
		if(phum->GetLifeTitle())
		{
			titlelist[num].m_titleid		=	_TITLE::LIFE_TITLE;
			titlelist[num++].m_titleData	=	static_cast<SHORT>(phum->GetLifeTitle());
		}
		if(phum->GetRankTitle())
		{
			titlelist[num].m_titleid		=	_TITLE::RANK_TITLE;
			titlelist[num++].m_titleData	=	static_cast<SHORT>(phum->GetRankTitle());
		}
		if(phum->GetStoryTitle())
		{
			titlelist[num].m_titleid		=	_TITLE::STORY_TITLE;
			titlelist[num++].m_titleData	=	static_cast<SHORT>(phum->GetStoryTitle());
		}
		if(phum->GetKillBossTitle())
		{
			titlelist[num].m_titleid		=	_TITLE::KILLBOSS_TITLE;
			titlelist[num++].m_titleData	=	static_cast<SHORT>(phum->GetKillBossTitle());
		}
		if(phum->GetTreasureTitle())
		{
			titlelist[num].m_titleid		=	_TITLE::TREASURE_TITLE;
			titlelist[num++].m_titleData	=	static_cast<SHORT>(phum->GetTreasureTitle());
		}

		for(int i = 0; i<MAX_DYNAMIC_TITLE_SIZE; i++ )
		{
			if(phum->GetDyTitle(i))
			{
				titlelist[num].m_titleid		=	(_TITLE::DY_TITLE0 + i);
				titlelist[num++].m_titleData	=	phum->GetDyTitle(i);
			}
		}

		GCCharAllTitles::TitleString titlestrlist[MAX_STRING_TITLE_SIZE];
		int strnum = 0;
		const CHAR* szTemp = NULL;
		if(strlen(phum->GetNickName())!=0)
		{
			szTemp = phum->GetNickName();
			titlestrlist[strnum].m_titleid	=	_TITLE::NICK_TITLE;
			titlestrlist[strnum].m_strlength = (BYTE)strlen(szTemp);
			memset(titlestrlist[strnum].m_szTitle, 0, MAX_CHARACTER_TITLE);
			memcpy(titlestrlist[strnum].m_szTitle, szTemp, titlestrlist[strnum].m_strlength);
			strnum++;
		}
		if(strlen(phum->GetSpouseName())!=0)
		{
			szTemp = phum->GetSpouseName();
			titlestrlist[strnum].m_titleid	=	_TITLE::SPOUSE_TITLE;
			titlestrlist[strnum].m_strlength = (BYTE)strlen(szTemp);
			memset(titlestrlist[strnum].m_szTitle, 0, MAX_CHARACTER_TITLE);
			memcpy(titlestrlist[strnum].m_szTitle, szTemp, titlestrlist[strnum].m_strlength);
			strnum++;
		}
		if(strlen(phum->GetBangPaiName())!=0)
		{
			szTemp = phum->GetBangPaiName();
			titlestrlist[strnum].m_titleid	=	_TITLE::BANGPAI_TITLE;
			titlestrlist[strnum].m_strlength = (BYTE)strlen(szTemp);
			memset(titlestrlist[strnum].m_szTitle, 0, MAX_CHARACTER_TITLE);
			memcpy(titlestrlist[strnum].m_szTitle, szTemp, titlestrlist[strnum].m_strlength);
			strnum++;
		}
		if(strlen(phum->GetJieBaiName())!=0)
		{
			szTemp = phum->GetJieBaiName();
			titlestrlist[strnum].m_titleid	=	_TITLE::JIEBAI_TITLE;
			titlestrlist[strnum].m_strlength = (BYTE)strlen(szTemp);
			memset(titlestrlist[strnum].m_szTitle, 0, MAX_CHARACTER_TITLE);
			memcpy(titlestrlist[strnum].m_szTitle, szTemp, titlestrlist[strnum].m_strlength);
			strnum++;
		}
		if(strlen(phum->GetShiTuName())!=0)
		{
			szTemp = phum->GetShiTuName();
			titlestrlist[strnum].m_titleid	=	_TITLE::SHITU_TITLE;
			titlestrlist[strnum].m_strlength = (BYTE)strlen(szTemp);
			memset(titlestrlist[strnum].m_szTitle, 0, MAX_CHARACTER_TITLE);
			memcpy(titlestrlist[strnum].m_szTitle, szTemp, titlestrlist[strnum].m_strlength);
			strnum++;
		}
		if(strlen(phum->GetShangDianName())!=0)
		{
			szTemp = phum->GetShangDianName();
			titlestrlist[strnum].m_titleid	=	_TITLE::SHANGDIAN_TITLE;
			titlestrlist[strnum].m_strlength = (BYTE)strlen(szTemp);
			memset(titlestrlist[strnum].m_szTitle, 0, MAX_CHARACTER_TITLE);
			memcpy(titlestrlist[strnum].m_szTitle, szTemp, titlestrlist[strnum].m_strlength);
			strnum++;
		}

		GCCharAllTitles Msg;
		Msg.setTargetID(ObjID);
		Msg.SetTitleIdNum(num);
		Msg.SetTitleIdList(titlelist);
		Msg.SetTitleStrNum(strnum);
		Msg.SetTitleStrList(titlestrlist);
		//PACKET_ALL_TITLES_MSG


		pHuman->GetPlayer()->SendPacket(&Msg);
	}


	return PACKET_EXE_CONTINUE ;

	__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
