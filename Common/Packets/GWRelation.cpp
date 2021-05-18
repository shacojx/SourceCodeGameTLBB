#include "stdafx.h"
#include "stdafx.h"


#include "GWRelation.h"



BOOL GWRelation::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	m_Relation.Read( iStream ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GWRelation::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	m_Relation.Write( oStream ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GWRelation::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GWRelationHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


