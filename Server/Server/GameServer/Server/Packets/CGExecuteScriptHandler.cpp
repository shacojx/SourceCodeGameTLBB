#include "stdafx.h"


#include "CGExecuteScript.h"
#include "Log.h"
#include "GamePlayer.h"
#include "Obj_Human.h"
#include "Scene.h"
#include "PacketFactoryManager.h"


uint CGExecuteScriptHandler::Execute( CGExecuteScript* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer != NULL ) ;
	if ( pGamePlayer == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExecuteScriptHandler: Error (pGamePlayer == NULL) " ) ;
		return PACKET_EXE_CONTINUE ;
	}

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman != NULL ) ;
	if ( pHuman == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExecuteScriptHandler: Error (pHuman == NULL) " ) ;
		return PACKET_EXE_CONTINUE ;
	}

	Scene* pScene = pHuman->getScene() ;
	Assert( pScene != NULL ) ;
	if ( pScene == NULL )
	{
		g_pLog->FastSaveLog( LOG_FILE_1, "CGExecuteScriptHandler: Error Obj = %d (pScene == NULL) ", pHuman->GetID() ) ;
		return PACKET_EXE_CONTINUE ;
	}

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;
	X_SCRIPT* pScript = pPacket->GetScript() ;
#if 0 //不允许直接执行服务器端脚本，而不经过检查
	switch( pScript->GetParamCount() )
	{
	case 0:
		{
			pScene->GetLuaInterface()->ExeScript_DD( pScript->GetScriptID(),
													 pScript->GetFunName(),
													 pScene->SceneID(),
													 pHuman->GetID() ) ;
		}
		break ;
	case 1:
		{
			pScene->GetLuaInterface()->ExeScript_DDD( pScript->GetScriptID(),
													  pScript->GetFunName(),
													  pScene->SceneID(),
													  pHuman->GetID(),
													  pScript->GetParam(0) ) ;
		}
		break ;
	case 2:
		{
			pScene->GetLuaInterface()->ExeScript_DDDD( pScript->GetScriptID(),
													 pScript->GetFunName(),
													 pScene->SceneID(),
													 pHuman->GetID(),
													 pScript->GetParam(0),
													 pScript->GetParam(1) ) ;
		}
		break ;
	case 3:
		{
			pScene->GetLuaInterface()->ExeScript_DDDDD( pScript->GetScriptID(),
													    pScript->GetFunName(),
													    pScene->SceneID(),
													    pHuman->GetID(),
													    pScript->GetParam(0),
													    pScript->GetParam(1),
													    pScript->GetParam(2) ) ;
		}
		break ;
	case 4:
		{
			pScene->GetLuaInterface()->ExeScript_DDDDDD( pScript->GetScriptID(),
													     pScript->GetFunName(),
													     pScene->SceneID(),
													     pHuman->GetID(),
													     pScript->GetParam(0),
													     pScript->GetParam(1),
													     pScript->GetParam(2),
													     pScript->GetParam(3) ) ;
		}
		break ;
	case 5:
		{
			pScene->GetLuaInterface()->ExeScript_DDDDDDD( pScript->GetScriptID(),
														  pScript->GetFunName(),
														  pScene->SceneID(),
														  pHuman->GetID(),
													      pScript->GetParam(0),
													      pScript->GetParam(1),
													      pScript->GetParam(2),
													      pScript->GetParam(3),
													      pScript->GetParam(4) ) ;
		}
		break ;
	case 6:
		{
			pScene->GetLuaInterface()->ExeScript_DDDDDDDD( pScript->GetScriptID(),
													       pScript->GetFunName(),
														   pScene->SceneID(),
														   pHuman->GetID(),
													       pScript->GetParam(0),
													       pScript->GetParam(1),
													       pScript->GetParam(2),
													       pScript->GetParam(3),
													       pScript->GetParam(4),
													       pScript->GetParam(5) ) ;
		}
		break ;
	default :
		{
			Assert(FALSE) ;
		}
		break ;
	}
#endif
	g_pLog->FastSaveLog( LOG_FILE_1, "CGExecuteScriptHandler: GUID=%X ScriptID=%d FunName=%s", 
		pHuman->GetGUID(), 
		pScript->GetScriptID(), 
		pScript->GetFunName() ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
