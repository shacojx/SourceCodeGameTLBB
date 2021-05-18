#include "stdafx.h"
#include "GCUseEquipSkillResult.h"


using namespace Packets;


BOOL GCUseEquipSkillResult::Read( SocketInputStream& iStream ) 
{
	__ENTER_FUNCTION
		iStream.Read( (CHAR*)(&m_Result), sizeof(USE_EQUIPSKILL_RESULT));
	
	return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

BOOL GCUseEquipSkillResult::Write( SocketOutputStream& oStream ) const
{
	__ENTER_FUNCTION
		oStream.Write( (CHAR*)(&m_Result), sizeof(USE_EQUIPSKILL_RESULT));
		return TRUE ;
	__LEAVE_FUNCTION
		return FALSE ;
}

UINT GCUseEquipSkillResult::Execute( Player* pPlayer )
{
	__ENTER_FUNCTION
		return GCUseEquipSkillResultHandler::Execute( this, pPlayer ) ;
	__LEAVE_FUNCTION
		return FALSE ;
}