#include "stdafx.h"
// CGGemCompound.cpp

#include "CGGemCompound.h"

BOOL	CGGemCompound::Read(SocketInputStream& iStream )	
{
	__ENTER_FUNCTION
	
		iStream.Read((CHAR*)&m_GemBagIndex1,sizeof(BYTE));
		iStream.Read((CHAR*)&m_GemBagIndex2,sizeof(BYTE));
		iStream.Read((CHAR*)&m_Platform,sizeof(ObjID_t));
		return TRUE;
	__LEAVE_FUNCTION
		return	FALSE;
}

BOOL	CGGemCompound::Write(SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
		
		oStream.Write((CHAR*)&m_GemBagIndex1,sizeof(BYTE));
		oStream.Write((CHAR*)&m_GemBagIndex2,sizeof(BYTE));
		oStream.Write((CHAR*)&m_Platform,sizeof(ObjID_t));
		return	TRUE;

	__LEAVE_FUNCTION
		return	FALSE;
}

UINT	CGGemCompound::Execute(Player* pPlayer )
{
	__ENTER_FUNCTION
		
		return CGGemCompoundHandler::Execute(this,pPlayer);

	__LEAVE_FUNCTION

		return	FALSE;
}
