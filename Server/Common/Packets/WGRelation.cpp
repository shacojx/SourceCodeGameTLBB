#include "stdafx.h"
#include "stdafx.h"


#include "WGRelation.h"



BOOL WGRelation::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	m_Relation.Read( iStream ) ;
	iStream.Read((CHAR*)&m_GUID,sizeof(m_GUID));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL WGRelation::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	m_Relation.Write( oStream ) ;
	oStream.Write( (CHAR*)(&m_GUID), sizeof(m_GUID));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT WGRelation::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return WGRelationHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


