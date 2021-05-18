/********************************************************************************
 *	文件名：	GWLevelUpHandler.cpp
 *	全路径：	d:\Prj\Client\Game\Network\PacketHandler\GWLevelUpHandler.cpp
 *	创建人：	胡繁
 *	创建时间：	2005 年 11 月 9 日	15:09
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/
#include "StdAfx.h"
#include "GWLevelUp.h"

using namespace Packets;

uint GWLevelUpHandler::Execute(GWLevelUp* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
