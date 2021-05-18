#include "stdafx.h"


#include "GCMonsterSpeak.h"



BOOL GCMonsterSpeak::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;
	iStream.Read( (CHAR*)(&m_bySpeakContentSize), sizeof(BYTE)); 
	if(m_bySpeakContentSize >= MAX_MONSTERSPEAK_CONTENT) m_bySpeakContentSize = MAX_MONSTERSPEAK_CONTENT-1;

	iStream.Read( (CHAR*)(&m_szSpeakContent), sizeof(CHAR)*m_bySpeakContentSize); 
	m_szSpeakContent[m_bySpeakContentSize] = '\0';
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCMonsterSpeak::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;
	oStream.Write( (CHAR*)(&m_bySpeakContentSize), sizeof(BYTE));
	oStream.Write( (CHAR*)(&m_szSpeakContent), sizeof(CHAR)*m_bySpeakContentSize);

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCMonsterSpeak::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

	return GCMonsterSpeakHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

	return FALSE ;
}

