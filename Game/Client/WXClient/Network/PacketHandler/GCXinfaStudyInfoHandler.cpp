
#include "stdafx.h"
#include "GCXinfaStudyInfo.h"

#include "..\..\DataPool\GMDataPool.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\ObjectManager.h"
#include "..\..\Object\Character\Obj_PlayerMySelf.h"
#include "..\..\DataPool\GMDP_CharacterData.h"

uint GCXinfaStudyInfoHandler::Execute( GCXinfaStudyInfo* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	//if(CGameProcedure::GetActiveProcedure() == (CGameProcedure*)CGameProcedure::s_pProcMain)
	//{
	//	INT idMenpai = (INT)pPacket->GetMenpaiInfo_ID();

	//	//先判定这个教师是不是自己的门派教师
	//	if(idMenpai != CObjectManager::GetMe()->GetMySelf()->GetCharacterData()->Get_MenPai())
	//	{
	//		CEventSystem::GetMe()->PushEvent(GE_NEW_DEBUGMESSAGE,"你没有加入我的门派！");
	//		return PACKET_EXE_CONTINUE ;
	//	}
	//	INT nObjId = (INT)pPacket->GetTeacherId();
	//	CObject* pObj = (CObject*)CObjectManager::GetMe()->FindServerObject(nObjId);

	//	if(pObj)
	//	{
	//		CDataPool::GetMe()->StudyXinfa_SetXinfaTeacherId(pObj->GetServerID());
	//	}
	//	
	//	CDataPool::GetMe()->StudyXinfa_SetMenpaiInfo( idMenpai );
	//	CEventSystem::GetMe()->PushEvent(GE_TOGLE_SKILLSTUDY,pObj?pObj->GetID():-1);
	//}

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
