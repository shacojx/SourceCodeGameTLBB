#include "stdafx.h"

#include "GCNewSkillObj.h"

BOOL GCNewSkillObj::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_posCreate), sizeof(WORLD_POS));
	iStream.Read( (CHAR*)(&m_idSkill), sizeof(SkillID_t));
	iStream.Read( (CHAR*)(&m_idUser), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_idTarget), sizeof(ObjID_t));
	iStream.Read( (CHAR*)(&m_posTarget), sizeof(WORLD_POS));
	iStream.Read( (CHAR*)(&m_fInitDir), sizeof(FLOAT));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCNewSkillObj::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_ObjID), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_posCreate), sizeof(WORLD_POS));
	oStream.Write( (CHAR*)(&m_idSkill), sizeof(SkillID_t));
	oStream.Write( (CHAR*)(&m_idUser), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_idTarget), sizeof(ObjID_t));
	oStream.Write( (CHAR*)(&m_posTarget), sizeof(WORLD_POS));
	oStream.Write( (CHAR*)(&m_fInitDir), sizeof(FLOAT));

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCNewSkillObj::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCNewSkillObjHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

