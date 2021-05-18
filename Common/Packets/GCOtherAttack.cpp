#include "stdafx.h"


#include "GCOtherAttack.h"



BOOL GCOtherAttack::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;
	
	iStream.Read( (CHAR*)(&m_byListNum), sizeof(BYTE) ) ;
	if(m_byListNum > MAX_DAM_LIST_NUM) m_byListNum = MAX_DAM_LIST_NUM;

	iStream.Read( (CHAR*)(m_listDam), sizeof(_DAMAGE_INFO)*m_byListNum ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCOtherAttack::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t) ) ;
	oStream.Write( (CHAR*)(&m_byListNum), sizeof(BYTE) ) ;
	oStream.Write( (CHAR*)(m_listDam), sizeof(_DAMAGE_INFO)*(m_byListNum>MAX_DAM_LIST_NUM ? MAX_DAM_LIST_NUM : m_byListNum ) ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCOtherAttack::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCOtherAttackHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}


