#include "stdafx.h"
#include "WBRetValidateUser.h"



BOOL WBRetValidateUser::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
		iStream.Read( (CHAR*)(&szAccount), sizeof(szAccount) ) ;
	iStream.Read( (CHAR*)(&szPassWord), sizeof(szPassWord) ) ;
	iStream.Read( (CHAR*)(&m_uFettle), sizeof(m_uFettle) ) ;
	iStream.Read( (CHAR*)(&m_PID), sizeof(m_PID) ) ;
	iStream.Read( (CHAR*)(&m_LoginID), sizeof(m_LoginID) ) ;
	iStream.Read( (CHAR*)(&m_Age), sizeof(m_Age) ) ;
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

BOOL WBRetValidateUser::Write( SocketOutputStream& oStream )const
{
	__ENTER_FUNCTION
		oStream.Write( (CHAR*)(&szAccount), sizeof(szAccount) ) ;
	oStream.Write( (CHAR*)(&szPassWord), sizeof(szPassWord) ) ;
	oStream.Write( (CHAR*)(&m_uFettle), sizeof(m_uFettle) ) ;
	oStream.Write( (CHAR*)(&m_PID), sizeof(m_PID) ) ;
	oStream.Write( (CHAR*)(&m_LoginID), sizeof(m_LoginID) ) ;
	oStream.Write( (CHAR*)(&m_Age), sizeof(m_Age) ) ;
	return TRUE ;

	__LEAVE_FUNCTION

		return FALSE ;
}

UINT WBRetValidateUser::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION

		return WBRetValidateUserHandler::Execute( this, pPlayer ) ;

	__LEAVE_FUNCTION

		return FALSE ;
}


