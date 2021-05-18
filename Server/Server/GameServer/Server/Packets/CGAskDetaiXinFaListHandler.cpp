#include "stdafx.h"
#include "Log.h"
#include "GamePlayer.h"
#include "GCDetailXinFaList.h"
#include "Scene.h"
#include "HumanDB.h"
#include "CGAskDetailXinFaList.h"


uint CGAskSkillClassHandler::Execute( CGAskSkillClass* pPacket, Player* pPlayer )
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

	ObjID_t ObjID = pHuman->GetID();
	if( ObjID == INVALID_ID )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	Obj_Character *pTarget = (Obj_Character*)(pScene->GetObjManager()->GetObj(pPacket->getTargetID()));
	if( pTarget==NULL )
	{
		Assert( pTarget ) ;
		return PACKET_EXE_ERROR ;
	}

	// 权限判断
	// to be continue...
	if ( pTarget->GetObjType() != Obj::OBJ_TYPE_HUMAN
		&& pTarget->GetObjType() != Obj::OBJ_TYPE_PET )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGAskSkillClassHandler: faile obj=%d scene=%d", 
			ObjID, pScene->SceneID() ) ;
		return PACKET_EXE_CONTINUE;
	}

	_XINFA_DB_LOAD	XinFa;
	XinFa.m_Count = 0;
	INT nID, nScaned;
	for( nID = 0, nScaned = 0; nID < g_XinFaDataMgr.MAXSIZE && nScaned < g_XinFaDataMgr.Size(); )
	{
		Combat_Module::Skill_Module::XinFaData_T const* pXinFaData = g_XinFaDataMgr.GetInstanceByID(nID);
		if( pXinFaData != NULL )
		{
			nScaned ++;
			if( pHuman->GetMenPai() == pXinFaData->GetMenPai() )
			{
				XinFa.m_aXinFa[XinFa.m_Count].m_nXinFaID	=	nID;
				XinFa.m_aXinFa[XinFa.m_Count].m_nXinFaLevel	=	1;
				XinFa.m_Count ++;
			}
			if( XinFa.m_Count >= 3 )
				break;
		}
		nID ++;
	}

	GCSkillClass	Msg;
	Msg.setObjID( pTarget->GetID() );
	Msg.setXinFaList(XinFa.m_Count, XinFa.m_aXinFa);
	pGamePlayer->SendPacket( &Msg ) ;

	g_pLog->FastSaveLog( LOG_FILE_1, "CGAskSkillClassHandler: ok obj=%d scene=%d", 
		ObjID, pScene->SceneID() ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
