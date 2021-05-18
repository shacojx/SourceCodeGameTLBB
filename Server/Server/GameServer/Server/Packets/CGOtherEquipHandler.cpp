#include "stdafx.h"


#include "CGOtherEquip.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"
#include "GCOtherEquip.h"
#include "Log.h"



uint CGOtherEquipHandler::Execute( CGOtherEquip* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	GamePlayer* pGamePlayer = (GamePlayer*)pPlayer ;
	Assert( pGamePlayer ) ;

	Obj_Human* pHuman = pGamePlayer->GetHuman() ;
	Assert( pHuman ) ;

	Scene* pScene = pHuman->getScene() ;
	if( pScene==NULL ) return PACKET_EXE_CONTINUE ;

	//检查线程执行资源是否正确
	Assert( MyGetCurrentThreadID()==pScene->m_ThreadID ) ;

	ObjID_t ObjID = pPacket->getObjID() ;
    if( ObjID==INVALID_ID )
	{
		Assert(FALSE) ;
		return PACKET_EXE_ERROR ;
	}

	//开始检测要求装备信息的Obj是不是一个玩家的Obj_Human数据类
	Obj* pDestObj = pScene->GetObjManager()->GetObj( ObjID ) ;
	if( pDestObj==NULL )
	{//玩家ObjID已经离开场景了
//		Assert( FALSE ) ;
		return PACKET_EXE_CONTINUE ;
	}

	if( !IsCharacterObj( pDestObj->GetObjType() ) )
	{
		Assert( FALSE ) ;
		return PACKET_EXE_ERROR ;
	}

	Obj_Character* pDestCharacter = (Obj_Character*)pDestObj ;
	if( pDestCharacter->GetObjType()!=Obj::OBJ_TYPE_HUMAN )
	{
		Assert( FALSE ) ;
		return PACKET_EXE_ERROR ;
	}
	//检测完成

	Obj_Human* pDestHuman = (Obj_Human*)pDestCharacter ;

	GCOtherEquip Msg ;
	Msg.setObjID( ObjID ) ;
	Msg.setName( pDestHuman->GetName() ) ;
	Msg.setDirectory( pDestHuman->getDir() ) ;
//其他数据

	pGamePlayer->SendPacket( &Msg ) ;

	g_pLog->FastSaveLog( LOG_FILE_1, "CGOtherEquipHandler::ObjID=%d", ObjID ) ;

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
