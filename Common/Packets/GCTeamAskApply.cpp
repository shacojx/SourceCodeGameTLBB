#include "stdafx.h"
#include  "GCTeamAskApply.h"


BOOL	GCTeamAskApply::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_SourGUID, sizeof(GUID_t) );
	iStream.Read( (CHAR*)&m_DestGUID, sizeof(GUID_t) );
	iStream.Read( (CHAR*)&m_SourNameSize, sizeof(UCHAR) );
	iStream.Read( (CHAR*)&m_DestNameSize, sizeof(UCHAR) );

	if ( m_SourNameSize>0 )
	{
		iStream.Read((CHAR*)&m_SourName, m_SourNameSize);
	}

	if ( m_DestNameSize>0 )
	{
		iStream.Read((CHAR*)&m_DestName, m_DestNameSize);
	}

	iStream.Read( (CHAR*)&m_uFamily, sizeof(UINT) );
	iStream.Read( (CHAR*)&m_Scene, sizeof(SceneID_t) );
	iStream.Read( (CHAR*)&m_Level, sizeof(INT) );
	iStream.Read( (CHAR*)&m_DetailFlag, sizeof(UCHAR) );
	iStream.Read( (CHAR*)&m_uDataID, sizeof(USHORT) );

	if ( m_DetailFlag>0 )
	{
		iStream.Read( (CHAR*)&m_WeaponID, sizeof(UINT) );
		iStream.Read( (CHAR*)&m_CapID, sizeof(UINT) );
		iStream.Read( (CHAR*)&m_ArmourID, sizeof(UINT) );
		iStream.Read( (CHAR*)&m_CuffID, sizeof(UINT) );
		iStream.Read( (CHAR*)&m_FootID, sizeof(UINT) );
		iStream.Read( (CHAR*)&m_uFaceMeshID, sizeof(m_uFaceMeshID) );
		iStream.Read( (CHAR*)&m_uHairMeshID, sizeof(m_uHairMeshID) );
		iStream.Read( (CHAR*)&m_uHairColor, sizeof(m_uHairColor) );
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL	GCTeamAskApply::Write(SocketOutputStream& oStream )	const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_SourGUID, sizeof(GUID_t) );
	oStream.Write( (CHAR*)&m_DestGUID, sizeof(GUID_t) );
	oStream.Write( (CHAR*)&m_SourNameSize, sizeof(UCHAR) );
	oStream.Write( (CHAR*)&m_DestNameSize, sizeof(UCHAR) );

	if ( m_SourNameSize>0 )
	{
		oStream.Write((CHAR*)&m_SourName, m_SourNameSize);
	}

	if ( m_DestNameSize>0 )
	{
		oStream.Write((CHAR*)&m_DestName, m_DestNameSize);
	}

	oStream.Write( (CHAR*)&m_uFamily, sizeof(UINT) );
	oStream.Write( (CHAR*)&m_Scene, sizeof(SceneID_t) );
	oStream.Write( (CHAR*)&m_Level, sizeof(INT) );
	oStream.Write( (CHAR*)&m_DetailFlag, sizeof(UCHAR) );
	oStream.Write( (CHAR*)&m_uDataID, sizeof(USHORT) );

	if ( m_DetailFlag>0 )
	{
		oStream.Write( (CHAR*)&m_WeaponID, sizeof(UINT) );
		oStream.Write( (CHAR*)&m_CapID, sizeof(UINT) );
		oStream.Write( (CHAR*)&m_ArmourID, sizeof(UINT) );
		oStream.Write( (CHAR*)&m_CuffID, sizeof(UINT) );
		oStream.Write( (CHAR*)&m_FootID, sizeof(UINT) );
		oStream.Write( (CHAR*)&m_uFaceMeshID, sizeof(m_uFaceMeshID) );
		oStream.Write( (CHAR*)&m_uHairMeshID, sizeof(m_uHairMeshID) );
		oStream.Write( (CHAR*)&m_uHairColor, sizeof(m_uHairColor) );
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	GCTeamAskApply::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return GCTeamAskApplyHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

