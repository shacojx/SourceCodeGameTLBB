#include "stdafx.h"
// GCTeamMemberInfo.cpp

#include "GCTeamMemberInfo.h"

BOOL GCTeamMemberInfo::Read( SocketInputStream& iStream ) 
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)(&m_GUID), sizeof(GUID_t) );
	iStream.Read( (CHAR*)(&m_Flags), sizeof(m_Flags) );

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_FAMILY) )
	{
		iStream.Read( (CHAR*)(&m_uFamily), sizeof(m_uFamily) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_LEVEL) )
	{
		iStream.Read( (CHAR*)(&m_uLevel), sizeof(m_uLevel) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_POSITION) )
	{
		iStream.Read( (CHAR*)(&m_WorldPos), sizeof(m_WorldPos) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_HP) )
	{
		iStream.Read( (CHAR*)(&m_nHP), sizeof(m_nHP) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_MAX_HP) )
	{
		iStream.Read( (CHAR*)(&m_uMaxHP), sizeof(m_uMaxHP) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_MP) )
	{
		iStream.Read( (CHAR*)(&m_nMP), sizeof(m_nMP) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_MAX_MP) )
	{
		iStream.Read( (CHAR*)(&m_uMaxMP), sizeof(m_uMaxMP) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_ANGER) )
	{
		iStream.Read( (CHAR*)(&m_nAnger), sizeof(m_nAnger) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_WEAPON) )
	{
		iStream.Read( (CHAR*)&m_WeaponID, sizeof(UINT) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_CAP) )
	{
		iStream.Read( (CHAR*)&m_CapID, sizeof(UINT) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_ARMOR) )
	{
		iStream.Read( (CHAR*)&m_ArmourID, sizeof(UINT) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_CUFF) )
	{
		iStream.Read( (CHAR*)&m_CuffID, sizeof(UINT) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_BOOT) )
	{
		iStream.Read( (CHAR*)&m_FootID, sizeof(UINT) );
	}


/*
	if( IsUpdateAttrib(TEAM_MEMBER_ATT_BUFF) )
	{
	}
*/

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_DEADLINK) )
	{
		iStream.Read( (CHAR*)&m_DeadLinkFlag, sizeof(UCHAR) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_DEAD) )
	{
		iStream.Read( (CHAR*)&m_DeadFlag, sizeof(UCHAR) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_FACEMESH) )
	{
		iStream.Read( (CHAR*)&m_uFaceMeshID, sizeof(m_uFaceMeshID) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_HAIRMESH) )
	{
		iStream.Read( (CHAR*)&m_uHairMeshID, sizeof(m_uHairMeshID) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_HAIRCOLOR) )
	{
		iStream.Read( (CHAR*)&m_uHairColor, sizeof(m_uHairColor) );
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

BOOL GCTeamMemberInfo::Write( SocketOutputStream& oStream )const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)(&m_GUID), sizeof(GUID_t) );
	oStream.Write( (CHAR*)(&m_Flags), sizeof(m_Flags) );

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_FAMILY) )
	{
		oStream.Write( (CHAR*)(&m_uFamily), sizeof(m_uFamily) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_LEVEL) )
	{
		oStream.Write( (CHAR*)(&m_uLevel), sizeof(m_uLevel) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_POSITION) )
	{
		oStream.Write( (CHAR*)(&m_WorldPos), sizeof(m_WorldPos) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_HP) )
	{
		oStream.Write( (CHAR*)(&m_nHP), sizeof(m_nHP) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_MAX_HP) )
	{
		oStream.Write( (CHAR*)(&m_uMaxHP), sizeof(m_uMaxHP) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_MP) )
	{
		oStream.Write( (CHAR*)(&m_nMP), sizeof(m_nMP) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_MAX_MP) )
	{
		oStream.Write( (CHAR*)(&m_uMaxMP), sizeof(m_uMaxMP) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_ANGER) )
	{
		oStream.Write( (CHAR*)(&m_nAnger), sizeof(m_nAnger) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_WEAPON) )
	{
		oStream.Write( (CHAR*)&m_WeaponID, sizeof(UINT) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_CAP) )
	{
		oStream.Write( (CHAR*)&m_CapID, sizeof(UINT) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_ARMOR) )
	{
		oStream.Write( (CHAR*)&m_ArmourID, sizeof(UINT) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_CUFF) )
	{
		oStream.Write( (CHAR*)&m_CuffID, sizeof(UINT) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_BOOT) )
	{
		oStream.Write( (CHAR*)&m_FootID, sizeof(UINT) );
	}


	/*
	if( IsUpdateAttrib(TEAM_MEMBER_ATT_BUFF) )
	{
	}
	*/

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_DEADLINK) )
	{
		oStream.Write( (CHAR*)&m_DeadLinkFlag, sizeof(UCHAR) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_DEAD) )
	{
		oStream.Write( (CHAR*)&m_DeadFlag, sizeof(UCHAR) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_FACEMESH) )
	{
		oStream.Write( (CHAR*)&m_uFaceMeshID, sizeof(m_uFaceMeshID) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_HAIRMESH) )
	{
		oStream.Write( (CHAR*)&m_uHairMeshID, sizeof(m_uHairMeshID) );
	}

	if( IsUpdateAttrib(TEAM_MEMBER_ATT_HAIRCOLOR) )
	{
		oStream.Write( (CHAR*)&m_uHairColor, sizeof(m_uHairColor) );
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}

UINT GCTeamMemberInfo::Execute( Player* pPlayer )
{
__ENTER_FUNCTION

	return GCTeamMemberInfoHandler::Execute( this, pPlayer ) ;

__LEAVE_FUNCTION

	return FALSE ;
}

