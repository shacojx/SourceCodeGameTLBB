#include "stdafx.h"
#include "GameStruct_Finger.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"

#include "DB_Struct.h"

/////////////////////////////////////////////////////////////////////////////////

// FREQ_GUID
VOID FINGER_BY_GUID::CleanUp( )
{
	m_TargetGUID = INVALID_ID;
	m_uOnlineFlag = 1;			// 默认搜索在线玩家...
}

UINT FINGER_BY_GUID::GetSize( ) const
{
	return sizeof(m_TargetGUID) + sizeof(m_uOnlineFlag);
}

VOID FINGER_BY_GUID::Read( SocketInputStream &iStream )
{
	iStream.Read( (CHAR*)&m_TargetGUID, sizeof(m_TargetGUID) );
	iStream.Read( (CHAR*)&m_uOnlineFlag, sizeof(m_uOnlineFlag) );
}

VOID FINGER_BY_GUID::Write( SocketOutputStream &oStream ) const
{
	oStream.Write( (CHAR*)&m_TargetGUID, sizeof(m_TargetGUID) );
	oStream.Write( (CHAR*)&m_uOnlineFlag, sizeof(m_uOnlineFlag) );
}

// FREQ_NAME
VOID FINGER_BY_NAME::CleanUp( )
{
	m_uNameSize = 0;
	memset((void*)m_szTargetName, 0, sizeof(m_szTargetName));
	m_uOnlineFlag = 1;			// 默认搜索在线玩家...
	m_uPreciseFlag = 1;			// 默认精确查找...
	m_uPosition = 0;
}

UINT FINGER_BY_NAME::GetSize( ) const
{
	return sizeof(m_uNameSize) + m_uNameSize + sizeof(m_uOnlineFlag)
		+ sizeof(m_uPreciseFlag) + sizeof(m_uPosition);
}

VOID FINGER_BY_NAME::Read( SocketInputStream &iStream )
{
	iStream.Read( (CHAR*)&m_uNameSize, sizeof(m_uNameSize) );

	if( m_uNameSize > 0 && m_uNameSize < sizeof(m_szTargetName) )
	{
		iStream.Read( m_szTargetName, m_uNameSize );
	}

	iStream.Read( (CHAR*)&m_uOnlineFlag, sizeof(m_uOnlineFlag) );
	iStream.Read( (CHAR*)&m_uPreciseFlag, sizeof(m_uPreciseFlag) );
	iStream.Read( (CHAR*)&m_uPosition, sizeof(m_uPosition) );
}

VOID FINGER_BY_NAME::Write( SocketOutputStream &oStream ) const
{
	oStream.Write( (CHAR*)&m_uNameSize, sizeof(m_uNameSize) );

	if( m_uNameSize > 0 && m_uNameSize < sizeof(m_szTargetName) )
	{
		oStream.Write( m_szTargetName, m_uNameSize );
	}

	oStream.Write( (CHAR*)&m_uOnlineFlag, sizeof(m_uOnlineFlag) );
	oStream.Write( (CHAR*)&m_uPreciseFlag, sizeof(m_uPreciseFlag) );
	oStream.Write( (CHAR*)&m_uPosition, sizeof(m_uPosition) );
}

// FREQ_ADVANCED
VOID ADVANCED_FINGER::CleanUp( )
{
	m_Flags = 0;
	m_nMenPai = INVALID_ID;
	m_GuildID = INVALID_ID;
	m_Sex = INVALID_ID;
	m_nBottomLevel = INVALID_ID;
	m_nTopLevel = INVALID_ID;
	m_uPosition = 0;
}

UINT ADVANCED_FINGER::GetSize( ) const
{
	UINT uSize = sizeof(m_Flags) + sizeof(m_uPosition);

	for( int i=0; i<AFT_NUMBER; ++i )
	{
		if( IsFingerCondition((ADVANCED_FINGER_TYPE)i) )
		{
			switch( i )
			{
			case AFT_MENPAI:
				uSize += sizeof(m_nMenPai);
				break;
			case AFT_GUILD:
				uSize += sizeof(m_GuildID);
				break;
			case AFT_SEX:
				uSize += sizeof(m_Sex);
				break;
			case AFT_LEVEL:
				uSize += sizeof(m_nBottomLevel) + sizeof(m_nTopLevel);
				break;
			}
		}
	}

	return uSize;
}

VOID ADVANCED_FINGER::Read( SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)&m_Flags, sizeof(m_Flags) );
	iStream.Read( (CHAR*)&m_uPosition, sizeof(m_uPosition) );

	for( int i=0; i<AFT_NUMBER; ++i )
	{
		if( IsFingerCondition((ADVANCED_FINGER_TYPE)i) )
		{
			switch( i )
			{
			case AFT_MENPAI:
				iStream.Read( (CHAR*)&m_nMenPai, sizeof(m_nMenPai) );
				break;
			case AFT_GUILD:
				iStream.Read( (CHAR*)&m_GuildID, sizeof(m_GuildID) );
				break;
			case AFT_SEX:
				iStream.Read( (CHAR*)&m_Sex, sizeof(m_Sex) );
				break;
			case AFT_LEVEL:
				iStream.Read( (CHAR*)&m_nBottomLevel, sizeof(m_nBottomLevel) );
				iStream.Read( (CHAR*)&m_nTopLevel, sizeof(m_nTopLevel) );
				break;
			}
		}
	}
}

VOID ADVANCED_FINGER::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)&m_Flags, sizeof(m_Flags) );
	oStream.Write( (CHAR*)&m_uPosition, sizeof(m_uPosition) );

	for( int i=0; i<AFT_NUMBER; ++i )
	{
		if( IsFingerCondition((ADVANCED_FINGER_TYPE)i) )
		{
			switch( i )
			{
			case AFT_MENPAI:
				oStream.Write( (CHAR*)&m_nMenPai, sizeof(m_nMenPai) );
				break;
			case AFT_GUILD:
				oStream.Write( (CHAR*)&m_GuildID, sizeof(m_GuildID) );
				break;
			case AFT_SEX:
				oStream.Write( (CHAR*)&m_Sex, sizeof(m_Sex) );
				break;
			case AFT_LEVEL:
				oStream.Write( (CHAR*)&m_nBottomLevel, sizeof(m_nBottomLevel) );
				oStream.Write( (CHAR*)&m_nTopLevel, sizeof(m_nTopLevel) );
				break;
			}
		}
	}
}


//客户端向服务器发送的玩家查询相关结构数据
VOID CG_FINGER::CleanUp( )
{
	m_Type = FREQ_NONE;
}

UINT CG_FINGER::GetSize( ) const
{
	UINT uSize = sizeof(m_Type);

	switch( m_Type )
	{
	case FREQ_GUID:
		uSize += m_FingerByGUID.GetSize();
		break;
	case FREQ_NAME:
		uSize += m_FingerByName.GetSize();
		break;
	case FREQ_ADVANCED:
		uSize += m_AdvancedFinger.GetSize();
		break;
	default:
		Assert(FALSE);
		return 0;
	}

	return uSize;
}

VOID CG_FINGER::Read( SocketInputStream &iStream )
{
	iStream.Read( (CHAR*)&m_Type, sizeof(m_Type) );

	switch( m_Type )
	{
	case FREQ_GUID:
		m_FingerByGUID.CleanUp();
		m_FingerByGUID.Read(iStream);
		break;
	case FREQ_NAME:
		m_FingerByName.CleanUp();
		m_FingerByName.Read(iStream);
		break;
	case FREQ_ADVANCED:
		m_AdvancedFinger.CleanUp();
		m_FingerByName.Read(iStream);
		break;
	default:
		Assert(FALSE);
		return;
	}
}

VOID CG_FINGER::Write( SocketOutputStream &oStream ) const
{
	oStream.Write( (CHAR*)&m_Type, sizeof(m_Type) );

	switch( m_Type )
	{
	case FREQ_GUID:
		m_FingerByGUID.Write(oStream);
		break;
	case FREQ_NAME:
		m_FingerByName.Write(oStream);
		break;
	case FREQ_ADVANCED:
		m_AdvancedFinger.Write(oStream);
		break;
	default:
		Assert(FALSE);
		return;
	}
}

// GW_FINGER
VOID GW_FINGER::CleanUp( )
{
	CG_FINGER::CleanUp();
	m_GUID = INVALID_ID;
}

UINT GW_FINGER::GetSize( ) const
{
	UINT uSize;

	uSize = CG_FINGER::GetSize();
	uSize += sizeof(m_GUID);

	return uSize;
}

VOID GW_FINGER::Read( SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)&m_GUID, sizeof(m_GUID) );
	CG_FINGER::Read( iStream );
}

VOID GW_FINGER::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)&m_GUID, sizeof(m_GUID) );
	CG_FINGER::Write( oStream );
}

//////////////////////////////////////////////////////////////////////////

// 昵称 + GUID + OnlineFlag + Level + Sex + MenpaiID + GuildID + 头像
VOID FINGER_PLAYER_INFO::CleanUp()
{
	m_GUID = INVALID_ID;
	m_uNameSize = 0;
	memset((void*)m_szName, 0, sizeof(m_szName));
	m_uOnlineFlag = 0;
	m_nLevel = 0;
	m_Sex = 0;
	m_nMenPai = MATTRIBUTE_WUMENPAI;
	m_GuildID = INVALID_ID;
	m_uGuildNameSize = 0;
	memset((void*)m_szGuildName, 0, sizeof(m_szGuildName));
	m_nPortrait = -1;
}

UINT FINGER_PLAYER_INFO::GetSize() const
{
	UINT uSize;

	uSize = sizeof(m_GUID) + sizeof(m_uNameSize) + m_uNameSize + sizeof(m_uOnlineFlag)
		  + sizeof(m_nLevel) + sizeof(m_Sex) + sizeof(m_nMenPai) + sizeof(m_GuildID)
		  + sizeof(m_uGuildNameSize) + m_uGuildNameSize + sizeof(m_nPortrait);

	return uSize;
}

VOID FINGER_PLAYER_INFO::Read( SocketInputStream &iStream )
{
	iStream.Read( (CHAR*)&m_GUID, sizeof(m_GUID) );
	iStream.Read( (CHAR*)&m_uNameSize, sizeof(m_uNameSize) );

	if( m_uNameSize > 0 && m_uNameSize < sizeof(m_szName) )
	{
		iStream.Read(m_szName, m_uNameSize);
	}

	iStream.Read( (CHAR*)&m_uOnlineFlag, sizeof(m_uOnlineFlag) );
	iStream.Read( (CHAR*)&m_nLevel, sizeof(m_nLevel) );
	iStream.Read( (CHAR*)&m_Sex, sizeof(m_Sex) );
	iStream.Read( (CHAR*)&m_nMenPai, sizeof(m_nMenPai) );
	iStream.Read( (CHAR*)&m_GuildID, sizeof(m_GuildID) );
	iStream.Read( (CHAR*)&m_uGuildNameSize, sizeof(m_uGuildNameSize) );

	if( m_uGuildNameSize > 0 && m_uGuildNameSize < sizeof(m_szGuildName) )
	{
		iStream.Read(m_szGuildName, m_uGuildNameSize);
	}

	iStream.Read( (CHAR*)&m_nPortrait, sizeof(m_nPortrait) );
}

VOID FINGER_PLAYER_INFO::Write( SocketOutputStream &oStream ) const
{
	oStream.Write( (CHAR*)&m_GUID, sizeof(m_GUID) );
	oStream.Write( (CHAR*)&m_uNameSize, sizeof(m_uNameSize) );

	if( m_uNameSize > 0 && m_uNameSize < sizeof(m_szName) )
	{
		oStream.Write(m_szName, m_uNameSize);
	}

	oStream.Write( (CHAR*)&m_uOnlineFlag, sizeof(m_uOnlineFlag) );
	oStream.Write( (CHAR*)&m_nLevel, sizeof(m_nLevel) );
	oStream.Write( (CHAR*)&m_Sex, sizeof(m_Sex) );
	oStream.Write( (CHAR*)&m_nMenPai, sizeof(m_nMenPai) );
	oStream.Write( (CHAR*)&m_GuildID, sizeof(m_GuildID) );
	oStream.Write( (CHAR*)&m_uGuildNameSize, sizeof(m_uGuildNameSize) );

	if( m_uGuildNameSize > 0 && m_uGuildNameSize < sizeof(m_szGuildName) )
	{
		oStream.Write(m_szGuildName, m_uGuildNameSize);
	}

	oStream.Write( (CHAR*)&m_nPortrait, sizeof(m_nPortrait) );
}

// FRET_PLAYERLIST
VOID RETURN_FINGER_PLAYERLIST::CleanUp()
{
	m_uPlayerCount = 0;

	for( int i=0; i<FINGER_PLAYER_COUNT; ++i )
	{
		m_FingerPlayerInfo[i].CleanUp();
	}

	m_FingerFlag = 0;
	m_uPosition = 0;
}

UINT RETURN_FINGER_PLAYERLIST::GetSize() const
{
	UINT uSize = sizeof(m_uPlayerCount) + sizeof(m_FingerFlag) + sizeof(m_uPosition);

	if( m_uPlayerCount > 0 && m_uPlayerCount <= FINGER_PLAYER_COUNT )
	{
		for( int i=0; i<m_uPlayerCount; ++i )
		{
			uSize += m_FingerPlayerInfo[i].GetSize();
		}
	}

	return uSize;
}

VOID RETURN_FINGER_PLAYERLIST::Read( SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)&m_uPlayerCount, sizeof(m_uPlayerCount) );

	if( m_uPlayerCount > 0 && m_uPlayerCount <= FINGER_PLAYER_COUNT )
	{
		for( int i=0; i<m_uPlayerCount; ++i )
		{
			m_FingerPlayerInfo[i].Read(iStream);
		}
	}

	iStream.Read( (CHAR*)&m_FingerFlag, sizeof(m_FingerFlag) );
	iStream.Read( (CHAR*)&m_uPosition, sizeof(m_uPosition) );
}

VOID RETURN_FINGER_PLAYERLIST::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)&m_uPlayerCount, sizeof(m_uPlayerCount) );

	if( m_uPlayerCount > 0 && m_uPlayerCount <= FINGER_PLAYER_COUNT )
	{
		for( int i=0; i<m_uPlayerCount; ++i )
		{
			m_FingerPlayerInfo[i].Write(oStream);
		}
	}

	oStream.Write( (CHAR*)&m_FingerFlag, sizeof(m_FingerFlag) );
	oStream.Write( (CHAR*)&m_uPosition, sizeof(m_uPosition) );
}

VOID RETURN_FINGER_PLAYERLIST::AddPlayerInfo( const USER_SIMPLE_DATA* pPlayerInfo )
{
__ENTER_FUNCTION

	Assert( pPlayerInfo && m_uPlayerCount < FINGER_PLAYER_COUNT );
	FINGER_PLAYER_INFO& data = m_FingerPlayerInfo[m_uPlayerCount++];
	data.SetGUID( pPlayerInfo->m_GUID );
	data.SetName( pPlayerInfo->m_Name );
	data.SetLevel( pPlayerInfo->m_nLevel );
	data.SetSex( (CHAR)pPlayerInfo->m_uSex );
	data.SetMenPai( pPlayerInfo->m_uMenPai );
	data.SetGuildID( pPlayerInfo->m_GuildID );
	data.SetPortrait( pPlayerInfo->m_nPortrait );

__LEAVE_FUNCTION

}

VOID GC_FINGER::CleanUp( )
{
	m_Type = FRET_NONE;
}

UINT GC_FINGER::GetSize( ) const
{
	UINT uSize = sizeof(m_Type);

	switch( m_Type )
	{
	case FRET_PLAYERLIST:
		uSize += m_FingerPlayerList.GetSize();
		break;
	case FRET_ERR_NORESULT:
		break;
	default:
		Assert(FALSE);
		return 0;
	}

	return uSize;
}

VOID GC_FINGER::Read( SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)&m_Type, sizeof(m_Type) );

	switch( m_Type )
	{
	case FRET_PLAYERLIST:
		m_FingerPlayerList.CleanUp();
		m_FingerPlayerList.Read(iStream);
		break;
	case FRET_ERR_NORESULT:
		break;
	default:
		Assert(FALSE);
		return;
	}
}

VOID GC_FINGER::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)&m_Type, sizeof(m_Type) );

	switch( m_Type )
	{
	case FRET_PLAYERLIST:
		m_FingerPlayerList.Write(oStream);
		break;
	case FRET_ERR_NORESULT:
		break;
	default:
		Assert(FALSE);
		return;
	}
}

VOID WG_FINGER::CleanUp( )
{
	GC_FINGER::CleanUp();
	m_PlayerID = INVALID_ID;
}

UINT WG_FINGER::GetSize( ) const
{
	return GC_FINGER::GetSize() + sizeof(m_PlayerID);
}

VOID WG_FINGER::Read( SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)&m_PlayerID, sizeof(m_PlayerID) );
	GC_FINGER::Read( iStream );
}

VOID WG_FINGER::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)&m_PlayerID, sizeof(m_PlayerID) );
	GC_FINGER::Write( oStream );
}


//////////////////////////////////////////////////////////////////////////
