#include "stdafx.h"
#include  "GCTeamAskInvite.h"


BOOL	GCTeamAskInvite::Read(SocketInputStream& iStream )
{
__ENTER_FUNCTION

	iStream.Read( (CHAR*)&m_GUID, sizeof(GUID_t) );
	iStream.Read( (CHAR*)&m_uMemberSize, sizeof(UCHAR) );

	for( INT i=0; i<m_uMemberSize; ++i )
	{
		iStream.Read( (CHAR*)&(m_InviterInfo[i].m_DetailFlag), sizeof(UCHAR) );
		iStream.Read( (CHAR*)&(m_InviterInfo[i].m_NickSize), sizeof(UCHAR) );

		if ( m_InviterInfo[i].m_NickSize>0 )
		{
			iStream.Read( (CHAR*)&(m_InviterInfo[i].m_szNick), sizeof(CHAR) * m_InviterInfo[i].m_NickSize );
		}

		iStream.Read( (CHAR*)&m_InviterInfo[i].m_uFamily, sizeof(UINT) );
		iStream.Read( (CHAR*)&m_InviterInfo[i].m_Scene, sizeof(SceneID_t) );
		iStream.Read( (CHAR*)&m_InviterInfo[i].m_Level, sizeof(INT) );
		iStream.Read( (CHAR*)&m_InviterInfo[i].m_uDataID, sizeof(USHORT) );

		if ( m_InviterInfo[i].m_DetailFlag>0 )
		{
			iStream.Read( (CHAR*)&m_InviterInfo[i].m_WeaponID, sizeof(UINT) );
			iStream.Read( (CHAR*)&m_InviterInfo[i].m_CapID, sizeof(UINT) );
			iStream.Read( (CHAR*)&m_InviterInfo[i].m_ArmourID, sizeof(UINT) );
			iStream.Read( (CHAR*)&m_InviterInfo[i].m_CuffID, sizeof(UINT) );
			iStream.Read( (CHAR*)&m_InviterInfo[i].m_FootID, sizeof(UINT) );
			iStream.Read( (CHAR*)&m_InviterInfo[i].m_uFaceMeshID, sizeof(UINT) );
			iStream.Read( (CHAR*)&m_InviterInfo[i].m_uHairMeshID, sizeof(UINT) );
			iStream.Read( (CHAR*)&m_InviterInfo[i].m_uHairColor, sizeof(UINT) );
		}
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


BOOL	GCTeamAskInvite::Write(SocketOutputStream& oStream )	const
{
__ENTER_FUNCTION

	oStream.Write( (CHAR*)&m_GUID, sizeof(GUID_t) );
	oStream.Write( (CHAR*)&m_uMemberSize, sizeof(UCHAR) );

	for( INT i=0; i<m_uMemberSize; ++i )
	{
		oStream.Write( (CHAR*)&(m_InviterInfo[i].m_DetailFlag), sizeof(UCHAR) );
		oStream.Write( (CHAR*)&(m_InviterInfo[i].m_NickSize), sizeof(UCHAR) );

		if ( m_InviterInfo[i].m_NickSize>0 )
		{
			oStream.Write( (CHAR*)&(m_InviterInfo[i].m_szNick), sizeof(CHAR) * m_InviterInfo[i].m_NickSize );
		}

		oStream.Write( (CHAR*)&m_InviterInfo[i].m_uFamily, sizeof(UINT) );
		oStream.Write( (CHAR*)&m_InviterInfo[i].m_Scene, sizeof(SceneID_t) );
		oStream.Write( (CHAR*)&m_InviterInfo[i].m_Level, sizeof(INT) );
		oStream.Write( (CHAR*)&m_InviterInfo[i].m_uDataID, sizeof(USHORT) );

		if ( m_InviterInfo[i].m_DetailFlag>0 )
		{
			oStream.Write( (CHAR*)&m_InviterInfo[i].m_WeaponID, sizeof(UINT) );
			oStream.Write( (CHAR*)&m_InviterInfo[i].m_CapID, sizeof(UINT) );
			oStream.Write( (CHAR*)&m_InviterInfo[i].m_ArmourID, sizeof(UINT) );
			oStream.Write( (CHAR*)&m_InviterInfo[i].m_CuffID, sizeof(UINT) );
			oStream.Write( (CHAR*)&m_InviterInfo[i].m_FootID, sizeof(UINT) );
			oStream.Write( (CHAR*)&m_InviterInfo[i].m_uFaceMeshID, sizeof(UINT) );
			oStream.Write( (CHAR*)&m_InviterInfo[i].m_uHairMeshID, sizeof(UINT) );
			oStream.Write( (CHAR*)&m_InviterInfo[i].m_uHairColor, sizeof(UINT) );
		}
	}

	return TRUE;

__LEAVE_FUNCTION

	return FALSE;
}


UINT	GCTeamAskInvite::Execute(Player* pPlayer )
{
__ENTER_FUNCTION

	return GCTeamAskInviteHandler::Execute(this,pPlayer);

__LEAVE_FUNCTION

	return FALSE;
}

