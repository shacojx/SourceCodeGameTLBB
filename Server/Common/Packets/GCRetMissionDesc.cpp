#include "stdafx.h"
// GCRetMissionDesc.cpp
// 
/////////////////////////////////////////////////////

#include "GCRetMissionDesc.h"

using namespace Packets;

BOOL GCRetMissionDesc::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION
	iStream.Read( (CHAR*)(&m_nMissionIndex), sizeof(m_nMissionIndex) ) ;

	iStream.Read( (CHAR*)(&m_nMissionName), sizeof(m_nMissionName) ) ;
	iStream.Read( (CHAR*)(&m_szMissionName), m_nMissionName ) ;

	iStream.Read( (CHAR*)(&m_nMissionTarget), sizeof(m_nMissionTarget) ) ;
	iStream.Read( (CHAR*)(&m_szMissionTarget), m_nMissionTarget ) ;

	iStream.Read( (CHAR*)(&m_nMissionDesc), sizeof(m_nMissionDesc) ) ;
	iStream.Read( (CHAR*)(&m_szMissionDesc), m_nMissionDesc ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL GCRetMissionDesc::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION
	oStream.Write( (CHAR*)(&m_nMissionIndex), sizeof(m_nMissionIndex) ) ;

	oStream.Write( (CHAR*)(&m_nMissionName), sizeof(m_nMissionName) ) ;
	oStream.Write( (CHAR*)(&m_szMissionName), m_nMissionName ) ;

	oStream.Write( (CHAR*)(&m_nMissionTarget), sizeof(m_nMissionTarget) ) ;
	oStream.Write( (CHAR*)(&m_szMissionTarget), m_nMissionTarget ) ;

	oStream.Write( (CHAR*)(&m_nMissionDesc), sizeof(m_nMissionDesc) ) ;
	oStream.Write( (CHAR*)(&m_szMissionDesc), m_nMissionDesc ) ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

UINT GCRetMissionDesc::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCRetMissionDescHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID GCRetMissionDesc::SetMissionIndex(UINT id)
{
	m_nMissionIndex = id;
}

UINT GCRetMissionDesc::GetMissionIndex()
{
	return m_nMissionIndex;
}

VOID GCRetMissionDesc::SetMissionName( CHAR* szName )
{
	m_nMissionName = (BYTE)strlen( szName );
	Assert( m_nMissionName < _MAX_PATH-1 );

	strncpy( m_szMissionName, szName, m_nMissionName );
}

CHAR* GCRetMissionDesc::GetMissionName()
{
	return m_szMissionName;
}

VOID GCRetMissionDesc::SetMissionTarget( CHAR* szTarget )
{
	m_nMissionTarget = (BYTE)strlen( szTarget );
	Assert( m_nMissionTarget < _MAX_PATH-1 );

	strncpy( m_szMissionTarget, szTarget, m_nMissionTarget );
}

CHAR* GCRetMissionDesc::GetMissionTarget()
{
	return m_szMissionTarget;
}

VOID GCRetMissionDesc::SetMissionDesc( CHAR* szDesc )
{
	m_nMissionDesc = (BYTE)strlen( szDesc );
	Assert( m_nMissionDesc < _MAX_PATH-1 );

	strncpy( m_szMissionDesc, szDesc, m_nMissionDesc );
}

CHAR* GCRetMissionDesc::GetMissionDesc()
{
	return m_szMissionDesc;
}
