#include "stdafx.h"
#include "BWValidateUser.h"



BOOL BWValidateUser::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
		iStream.Read( (CHAR*)(&m_szUserName), sizeof(m_szUserName) ) ;
	iStream.Read( (CHAR*)(&m_szPassword), sizeof(m_szPassword) ) ;
		iStream.Read( (CHAR*)(&m_PID), sizeof(m_PID) ) ;
	iStream.Read( (CHAR*)(&m_LoginID), sizeof(m_LoginID) ) ;
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL BWValidateUser::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION
		oStream.Write( (CHAR*)(&m_szUserName), sizeof(m_szUserName) ) ;
	oStream.Write( (CHAR*)(&m_szPassword), sizeof(m_szPassword) ) ;
	oStream.Write( (CHAR*)(&m_PID), sizeof(m_PID) ) ;
	oStream.Write( (CHAR*)(&m_LoginID), sizeof(m_LoginID) ) ;
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT BWValidateUser::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return BWValidateUserHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


