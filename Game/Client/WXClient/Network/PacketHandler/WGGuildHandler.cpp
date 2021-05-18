/********************************************************************************
 *	文件名：	WGGuildHandler.cpp
 *	全路径：	d:\Prj\Client\Game\Network\PacketHandler\WGGuildHandler.cpp
 *	创建人：	胡繁
 *	创建时间：	2005 年 12 月 14 日	12:10
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#include "StdAfx.h"
#include "WGGuild.h"

uint WGGuildHandler::Execute( WGGuild* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
