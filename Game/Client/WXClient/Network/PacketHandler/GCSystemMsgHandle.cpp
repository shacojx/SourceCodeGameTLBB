#include "StdAfx.h"
#include "GCSystemMsg.h"
#include "..\..\Procedure\GameProcedure.h"
#include "..\..\Event\GMEventSystem.h"
#include "..\..\Object\Character\Obj_Character.h"
#include "..\..\Object\ObjectManager.h"
#include "../../DBC/GMDataBase.h"
#include "TDException.h"
#include "../../Global.h"
#include "GCChat.h"

//#include "../CEGUISystem/UIString.h"
uint GCSystemMsgHandler::Execute( GCSystemMsg* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		std::string str(pPacket->GetContex(),pPacket->GetContexSize());
			
		CEventSystem::GetMe()->PushEvent(GE_NEW_GONGGAOMESSAGE,str.c_str());

		GCChat msg;
		char strMsg[MAX_CHAT_SIZE]={0};
		strcpy(strMsg,str.c_str());
		msg.SetChatType( CHAT_TYPE_SYSTEM );
		msg.SetContexSize( (BYTE)strlen(strMsg));
		msg.SetContex(strMsg);
		SCRIPT_SANDBOX::Talk::s_Talk.HandleRecvTalkPacket(&msg);
		return PACKET_EXE_CONTINUE ;
			

		__LEAVE_FUNCTION

		return PACKET_EXE_ERROR ;
}
