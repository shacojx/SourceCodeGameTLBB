#include "stdafx.h"


#include "GCRelation.h"



BOOL GCRelation::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	m_Relation.Read(iStream);

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCRelation::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	m_Relation.Write(oStream);

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCRelation::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCRelationHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


