/********************************************************************************
 *	文件名：	GWEnterTeamFollowHandler.cpp
 *	全路径：	d:\Prj\Client\Game\Network\PacketHandler\GWEnterTeamFollowHandler.cpp
 *	创建人：	胡繁
 *	创建时间：	2005 年 11 月 23 日	15:39
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/
#include "StdAfx.h"
#include "GWEnterTeamFollow.h"

uint GWEnterTeamFollowHandler::Execute( GWEnterTeamFollow* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
