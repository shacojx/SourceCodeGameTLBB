#include "StdAfx.h"
#include "LCRetConnect.h"
#include "..\..\procedure\Gameprocedure.h"
#include "..\..\procedure\GamePro_Login.h"

UINT LCRetConnectHandler::Execute(LCRetConnect* pPacket, Player* pPlayer )
{
	__ENTER_FUNCTION
		
		if(NULL == pPacket)
		{
			return PACKET_EXE_ERROR;
		}

		if(NULL == CGameProcedure::s_pProcLogIn)
		{
			return PACKET_EXE_ERROR;
		}
		
		UINT  nPort = pPacket->GetLoginPort();
		CHAR* pIP   = pPacket->GetLoginIP();

		//---------------------------------------------------------------------------------------
		//
		// 下面的if语句等完成动态负载平衡后, 要去掉注释.
		//
		//---------------------------------------------------------------------------------------
		//if((nPort == CGameProcedure::s_pProcLogIn->m_nLoginServerPort)&&
		//   (0 == strcmp(pIP, CGameProcedure::s_pProcLogIn->m_szLoginServerAddr)))//
		{
			// 如果ip地址与端口号相同, 设置连接成功状态.
			CGameProcedure::s_pProcLogIn->SendCheckAccountMsg();
			CGameProcedure::s_pProcLogIn->SetStatus(CGamePro_Login::LOGIN_ACCOUNT_REQUESTING);
		}
		//else
		//{
		//	CGameProcedure::s_pProcLogIn->SetIpAddr(pIP);
		//	CGameProcedure::s_pProcLogIn->SetPort(nPort);
		//	CGameProcedure::s_pProcLogIn->SetStatus(CGamePro_Login::LOGIN_DISCONNECT);//
		//}//

		return PACKET_EXE_CONTINUE;

	__LEAVE_FUNCTION

		return PACKET_EXE_ERROR;
}