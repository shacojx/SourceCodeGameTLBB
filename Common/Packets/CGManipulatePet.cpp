#include "stdafx.h"
// CGManipulatePet.cpp

#include "CGManipulatePet.h"


BOOL CGManipulatePet::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	iStream.Read( (CHAR*)(&m_PetGUID), sizeof(m_PetGUID));
	iStream.Read( (CHAR*)(&m_nType), sizeof(m_nType));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGManipulatePet::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(m_ObjID));
	oStream.Write( (CHAR*)(&m_PetGUID), sizeof(m_PetGUID));
	oStream.Write( (CHAR*)(&m_nType), sizeof(m_nType));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGManipulatePet::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGManipulatePetHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

