#include "stdafx.h"


#include "CGRelation.h"



BOOL CGRelation::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	m_Relation.Read( iStream ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL CGRelation::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	m_Relation.Write( oStream ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT CGRelation::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return CGRelationHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


