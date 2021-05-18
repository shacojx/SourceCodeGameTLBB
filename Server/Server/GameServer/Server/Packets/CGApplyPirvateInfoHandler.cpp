#include "stdafx.h"


#include "CGApplyPrivateInfo.h"
#include "Log.h"
#include "Player.h"
#include "GamePlayer.h"
#include "Scene.h"
#include "Obj_Human.h"


uint CGApplyPrivateInfoHandler::Execute( CGApplyPrivateInfo* pPacket, Player* pPlayer )
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

	//只能提交自己的数据，所以不需要在消息中加ObjID
	//_PRIVATE_INFO_DB_LOAD PrivateData;

	//PrivateData.m_Age = pPacket->GetAge();
	//PrivateData.m_BloodType = pPacket->GetBloodType();
	//PrivateData.m_CharGUID = pPacket->GetCharGUID();

	//PrivateData.m_Consella = pPacket->GetConsella();
	//PrivateData.m_InfoType = pPacket->GetInfoType();
	//PrivateData.m_Province = pPacket->GetProvince();
	//PrivateData.m_Sex = pPacket->GetSex();
	//PrivateData.m_YearAnimal = pPacket->GetYearAnimal();

	//strncpy(PrivateData.m_City, pPacket->GetCity(), pPacket->GetCityLength());
	//strncpy(PrivateData.m_EmailInfo, pPacket->GetEmailInfo(), pPacket->GetEmailInfoLength());
	//strncpy(PrivateData.m_JobInfo, pPacket->GetJobInfo(), pPacket->GetJobInfoLength());
	//strncpy(PrivateData.m_LuckWord, pPacket->GetLuckWord(), pPacket->GetLuckWordLength());
	//strncpy(PrivateData.m_SchoolInfo, pPacket->GetSchoolInfo(), pPacket->GetSchoolInfoLength());


	//pHuman->GetDB()->SetPrivateInfo(&PrivateData);

	g_pLog->FastSaveLog( LOG_FILE_1, "CGApplyPrivateInfoHandler::ObjName=%s"
		,pHuman->GetName());

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}
