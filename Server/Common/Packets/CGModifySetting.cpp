#include "stdafx.h"


#include "CGModifySetting.h"



BOOL CGModifySetting::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_Type, sizeof(BYTE) );
	iStream.Read( (CHAR*)&m_Value, sizeof(_OWN_SETTING) );

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGModifySetting::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_Type, sizeof(BYTE) );
	oStream.Write( (CHAR*)&m_Value, sizeof(_OWN_SETTING) );
	
	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGModifySetting::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return CGModifySettingHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


