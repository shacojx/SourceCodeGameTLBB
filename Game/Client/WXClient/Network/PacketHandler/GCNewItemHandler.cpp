#include "StdAfx.h"
#include "GCNewItem.h"

uint GCNewItemHandler :: Execute( GCNewItem* pPacket, Player* pPlayer ) 
{
__ENTER_FUNCTION

	return PACKET_EXE_CONTINUE ;

__LEAVE_FUNCTION

	return PACKET_EXE_ERROR ;
}

