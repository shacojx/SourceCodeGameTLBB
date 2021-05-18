/********************************************************************************
 *	文件名：	GWTeamMemberEnterSceneHandler.cpp
 *	全路径：	d:\Prj\Client\Game\Network\PacketHandler\GWTeamMemberEnterSceneHandler.cpp
 *	创建人：	胡繁
 *	创建时间：	2005 年 11 月 3 日	18:10
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#include "StdAfx.h"
#include "GWTeamMemberEnterScene.h"

uint GWTeamMemberEnterSceneHandler::Execute( GWTeamMemberEnterScene* pPacket, Player* pPlayer )
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR;
}
