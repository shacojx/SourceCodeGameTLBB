#include "StdAfx.h"
#include "TDTimeSystem.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Surface\Obj_SkillObj.h"

#include "..\..\NetWork\NetManager.h"
#include "CGCharAskBaseAttrib.h"

#include "GCNewSkillObj.h"
#include "..\..\World\WorldManager.h"
#include "TDException.h"

uint GCNewSkillObjHandler :: Execute( GCNewSkillObj* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION
	//AxTrace(0, 2, "GCNewSkillObjHandler");

	Assert(FALSE);
	////当前流程是主流程
	//if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	//{
	//	CObjectManager* pObjectManager = CObjectManager::GetMe();

	//	//创建玩家
	//	CObject* pSkillObj = (CObject*)(pObjectManager->NewSurfaceObj( "CObject_SkillObj" ));
	//	if ( pSkillObj == NULL )
	//		return PACKET_EXE_CONTINUE;

	//	SObject_SkillObjInit initSkillObj;
	//	initSkillObj.m_idSkill		= pPacket->getSkillID();
	//	initSkillObj.m_idUser		= pPacket->getUserID();
	//	initSkillObj.m_posCreate	= pPacket->getCreatePos();

	//	initSkillObj.m_idTarget		= pPacket->getTargetID();
	//	initSkillObj.m_posTarget	= pPacket->getTargetPos();
	//	initSkillObj.m_fInitDir		= pPacket->getInitDir();

	//	pSkillObj->Initial( &initSkillObj );

	//	//设置新的位置
	//	//pSkillObj->RegisterToZone();
	//	pSkillObj->SetMsgTime(CGameProcedure::s_pTimeSystem->GetTimeNow());
	//}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

