#include "stdafx.h"

#include "GameStruct_Relation.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"

#include "DB_Struct.h"

/////////////////////////////////////////////////////////////////////////////////

// 关系人数据
VOID _RELATION::CleanUp()
{
	m_GUID = INVALID_ID;
	m_uNameSize = 0;
	memset((void*)m_szName, 0, sizeof(m_szName));
	m_nLevel = 0;
	m_nMenPai = MATTRIBUTE_WUMENPAI;
	m_nPortrait = -1;
	m_GuildID = INVALID_ID;
	m_uGuildNameSize = 0;
	memset((void*)m_szGuildName, 0, sizeof(m_szGuildName));
	m_uOnlineFlag = 0;
	m_uMoodSize = 0;
	memset((void*)m_szMood, 0, sizeof(m_szMood));
	m_uTitleSize = 0;
	memset((void*)m_szTitle, 0, sizeof(m_szTitle));
	m_SceneID = INVALID_ID;
	m_uTeamSize = 0;
}

UINT _RELATION::GetSize() const
{
	UINT uSize;

	uSize = sizeof(m_GUID) + sizeof(m_uNameSize) + m_uNameSize
		  + sizeof(m_nLevel) + sizeof(m_nMenPai) + sizeof(m_nPortrait) + sizeof(m_GuildID)
		  + sizeof(m_uGuildNameSize) + m_uGuildNameSize + sizeof(m_uOnlineFlag);

	if( m_uOnlineFlag > 0 )
	{
		uSize += sizeof(m_uMoodSize) + m_uMoodSize + sizeof(m_uTitleSize)
			   + m_uTitleSize + sizeof(m_SceneID) + sizeof(m_uTeamSize);
	}

	return uSize;
}

VOID _RELATION::Read( SocketInputStream &iStream )
{
	iStream.Read( (CHAR*)&m_GUID, sizeof(m_GUID) );
	iStream.Read( (CHAR*)&m_uNameSize, sizeof(m_uNameSize) );

	if( m_uNameSize > 0 && m_uNameSize < sizeof(m_szName) )
	{
		iStream.Read(m_szName, m_uNameSize);
	}

	iStream.Read( (CHAR*)&m_nLevel, sizeof(m_nLevel) );
	iStream.Read( (CHAR*)&m_nMenPai, sizeof(m_nMenPai) );
	iStream.Read( (CHAR*)&m_nPortrait, sizeof(m_nPortrait) );
	iStream.Read( (CHAR*)&m_GuildID, sizeof(m_GuildID) );
	iStream.Read( (CHAR*)&m_uGuildNameSize, sizeof(m_uGuildNameSize) );

	if( m_uGuildNameSize > 0 && m_uGuildNameSize < sizeof(m_szGuildName) )
	{
		iStream.Read(m_szGuildName, m_uGuildNameSize);
	}

	iStream.Read( (CHAR*)&m_uOnlineFlag, sizeof(m_uOnlineFlag) );

	if( m_uOnlineFlag > 0 )
	{
		iStream.Read( (CHAR*)&m_uMoodSize, sizeof(m_uMoodSize) );

		if( m_uMoodSize > 0 && m_uMoodSize < sizeof(m_szMood) )
		{
			iStream.Read(m_szMood, m_uMoodSize);
		}

		iStream.Read( (CHAR*)&m_uTitleSize, sizeof(m_uTitleSize) );

		if( m_uTitleSize > 0 && m_uTitleSize < sizeof(m_szTitle) )
		{
			iStream.Read(m_szTitle, m_uTitleSize);
		}

		iStream.Read( (CHAR*)&m_SceneID, sizeof(m_SceneID) );
		iStream.Read( (CHAR*)&m_uTeamSize, sizeof(m_uTeamSize) );
	}
}

VOID _RELATION::Write( SocketOutputStream &oStream ) const
{
	oStream.Write( (CHAR*)&m_GUID, sizeof(m_GUID) );
	oStream.Write( (CHAR*)&m_uNameSize, sizeof(m_uNameSize) );

	if( m_uNameSize > 0 && m_uNameSize < sizeof(m_szName) )
	{
		oStream.Write(m_szName, m_uNameSize);
	}

	oStream.Write( (CHAR*)&m_nLevel, sizeof(m_nLevel) );
	oStream.Write( (CHAR*)&m_nMenPai, sizeof(m_nMenPai) );
	oStream.Write( (CHAR*)&m_nPortrait, sizeof(m_nPortrait) );
	oStream.Write( (CHAR*)&m_GuildID, sizeof(m_GuildID) );
	oStream.Write( (CHAR*)&m_uGuildNameSize, sizeof(m_uGuildNameSize) );

	if( m_uGuildNameSize > 0 && m_uGuildNameSize < sizeof(m_szGuildName) )
	{
		oStream.Write(m_szGuildName, m_uGuildNameSize);
	}

	oStream.Write( (CHAR*)&m_uOnlineFlag, sizeof(m_uOnlineFlag) );

	if( m_uOnlineFlag > 0 )
	{
		oStream.Write( (CHAR*)&m_uMoodSize, sizeof(m_uMoodSize) );

		if( m_uMoodSize > 0 && m_uMoodSize < sizeof(m_szMood) )
		{
			oStream.Write(m_szMood, m_uMoodSize);
		}

		oStream.Write( (CHAR*)&m_uTitleSize, sizeof(m_uTitleSize) );

		if( m_uTitleSize > 0 && m_uTitleSize < sizeof(m_szTitle) )
		{
			oStream.Write(m_szTitle, m_uTitleSize);
		}

		oStream.Write( (CHAR*)&m_SceneID, sizeof(m_SceneID) );
		oStream.Write( (CHAR*)&m_uTeamSize, sizeof(m_uTeamSize) );
	}
}

// 一参数
VOID RELATION_GUID::CleanUp( )
{
	m_TargetGUID = INVALID_ID;
}

UINT RELATION_GUID::GetSize( ) const
{
	return sizeof(m_TargetGUID);
}

VOID RELATION_GUID::Read( SocketInputStream &iStream )
{
	iStream.Read( (CHAR*)&m_TargetGUID, sizeof(m_TargetGUID) );
}

VOID RELATION_GUID::Write( SocketOutputStream &oStream ) const
{
	oStream.Write( (CHAR*)&m_TargetGUID, sizeof(m_TargetGUID) );
}

// 删除好友
VOID REQUEST_DEL_FRIEND::CleanUp( )
{
	RELATION_GUID::CleanUp();
}

UINT REQUEST_DEL_FRIEND::GetSize( ) const
{
	return RELATION_GUID::GetSize();
}

VOID REQUEST_DEL_FRIEND::Read( SocketInputStream& iStream )
{
	RELATION_GUID::Read( iStream );
}

VOID REQUEST_DEL_FRIEND::Write( SocketOutputStream& oStream ) const
{
	RELATION_GUID::Write( oStream );
}

// 修改心情
VOID REQUEST_MODIFY_MOOD::CleanUp( )
{
	m_uMoodSize = 0;
	memset( (void*)m_szMood, 0, sizeof(m_szMood) );
}

UINT REQUEST_MODIFY_MOOD::GetSize( ) const
{
	return sizeof(m_uMoodSize) + m_uMoodSize;
}

VOID REQUEST_MODIFY_MOOD::Read( SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)&m_uMoodSize, sizeof(m_uMoodSize) );

	if( m_uMoodSize > 0 )
	{
		iStream.Read( (CHAR*)m_szMood, m_uMoodSize );
	}
}

VOID REQUEST_MODIFY_MOOD::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)&m_uMoodSize, sizeof(m_uMoodSize) );

	if( m_uMoodSize > 0 )
	{
		oStream.Write( (CHAR*)m_szMood, m_uMoodSize );
	}
}

// 查询关系人详细信息
VOID REQUEST_RELATION_INFO::CleanUp( )
{
	m_TargetGUID = INVALID_ID;
	m_uNameSize = 0;
	memset((void*)m_szTargetName, 0, sizeof(m_szTargetName));
}

UINT REQUEST_RELATION_INFO::GetSize( ) const
{
	return sizeof(m_TargetGUID) + sizeof(m_uNameSize) + m_uNameSize;
}

VOID REQUEST_RELATION_INFO::Read( SocketInputStream &iStream )
{
	iStream.Read( (CHAR*)&m_TargetGUID, sizeof(m_TargetGUID) );
	iStream.Read( (CHAR*)&m_uNameSize, sizeof(m_uNameSize) );

	if( m_uNameSize > 0 && m_uNameSize < sizeof(m_szTargetName) )
	{
		iStream.Read( m_szTargetName, m_uNameSize );
	}
}

VOID REQUEST_RELATION_INFO::Write( SocketOutputStream &oStream ) const
{
	oStream.Write( (CHAR*)&m_TargetGUID, sizeof(m_TargetGUID) );
	oStream.Write( (CHAR*)&m_uNameSize, sizeof(m_uNameSize) );

	if( m_uNameSize > 0 && m_uNameSize < sizeof(m_szTargetName) )
	{
		oStream.Write( m_szTargetName, m_uNameSize );
	}
}

// 右建查看玩家
VOID REQUEST_VIEW_PLAYER::CleanUp( )
{
	m_uNameSize = 0;
	memset((void*)m_szTargetName, 0, sizeof(m_szTargetName));
}

UINT REQUEST_VIEW_PLAYER::GetSize( ) const
{
	return sizeof(m_uNameSize) + m_uNameSize;
}

VOID REQUEST_VIEW_PLAYER::Read( SocketInputStream &iStream )
{
	iStream.Read( (CHAR*)&m_uNameSize, sizeof(m_uNameSize) );

	if( m_uNameSize > 0 && m_uNameSize < sizeof(m_szTargetName) )
	{
		iStream.Read( m_szTargetName, m_uNameSize );
	}
}

VOID REQUEST_VIEW_PLAYER::Write( SocketOutputStream &oStream ) const
{
	oStream.Write( (CHAR*)&m_uNameSize, sizeof(m_uNameSize) );

	if( m_uNameSize > 0 && m_uNameSize < sizeof(m_szTargetName) )
	{
		oStream.Write( m_szTargetName, m_uNameSize );
	}
}

// 增加一个关系人
VOID REQUEST_ADD_RELATION::CleanUp( )
{
	m_TargetGUID = INVALID_ID;
	m_uNameSize = 0;
	memset((void*)m_szTargetName, 0, sizeof(m_szTargetName));
	m_uRelationType = 0;
}

UINT REQUEST_ADD_RELATION::GetSize( ) const
{
	return sizeof(m_TargetGUID) + sizeof(m_uRelationType) + sizeof(m_uNameSize) + m_uNameSize;
}

VOID REQUEST_ADD_RELATION::Read( SocketInputStream &iStream )
{
	iStream.Read( (CHAR*)&m_TargetGUID, sizeof(m_TargetGUID) );
	iStream.Read( (CHAR*)&m_uNameSize, sizeof(m_uNameSize) );

	if( m_uNameSize > 0 && m_uNameSize < sizeof(m_szTargetName) )
	{
		iStream.Read( m_szTargetName, m_uNameSize );
	}

	iStream.Read( (CHAR*)&m_uRelationType, sizeof(m_uRelationType) );
}

VOID REQUEST_ADD_RELATION::Write( SocketOutputStream &oStream ) const
{
	oStream.Write( (CHAR*)&m_TargetGUID, sizeof(m_TargetGUID) );
	oStream.Write( (CHAR*)&m_uNameSize, sizeof(m_uNameSize) );

	if( m_uNameSize > 0 && m_uNameSize < sizeof(m_szTargetName) )
	{
		oStream.Write( m_szTargetName, m_uNameSize );
	}

	oStream.Write( (CHAR*)&m_uRelationType, sizeof(m_uRelationType) );
}

// 增加一个关系人，并且带上组号
VOID REQUEST_ADD_RELATION_WITH_GROUP::CleanUp( )
{
	REQUEST_ADD_RELATION::CleanUp();

	m_uGroup = 0;
}

UINT REQUEST_ADD_RELATION_WITH_GROUP::GetSize( ) const
{
	return REQUEST_ADD_RELATION::GetSize() + sizeof(m_uGroup);
}

VOID REQUEST_ADD_RELATION_WITH_GROUP::Read( SocketInputStream &iStream )
{
	REQUEST_ADD_RELATION::Read( iStream );

	iStream.Read( (CHAR*)&m_uGroup, sizeof(m_uGroup) );
}

VOID REQUEST_ADD_RELATION_WITH_GROUP::Write( SocketOutputStream &oStream ) const
{
	REQUEST_ADD_RELATION::Write( oStream );

	oStream.Write( (CHAR*)&m_uGroup, sizeof(m_uGroup) );
}

// 三参数
VOID RELATION_GUID_UCHAR_UCHAR::CleanUp( )
{
	m_TargetGUID = INVALID_ID;
	m_uRelationType = 0;
	m_uGroup = 0;
}

UINT RELATION_GUID_UCHAR_UCHAR::GetSize( ) const
{
	return sizeof(m_TargetGUID) + sizeof(m_uRelationType) + sizeof(m_uGroup);
}

VOID RELATION_GUID_UCHAR_UCHAR::Read( SocketInputStream &iStream )
{
	iStream.Read( (CHAR*)&m_TargetGUID, sizeof(m_TargetGUID) );
	iStream.Read( (CHAR*)&m_uRelationType, sizeof(m_uRelationType) );
	iStream.Read( (CHAR*)&m_uGroup, sizeof(m_uGroup) );
}

VOID RELATION_GUID_UCHAR_UCHAR::Write( SocketOutputStream &oStream ) const
{
	oStream.Write( (CHAR*)&m_TargetGUID, sizeof(m_TargetGUID) );
	oStream.Write( (CHAR*)&m_uRelationType, sizeof(m_uRelationType) );
	oStream.Write( (CHAR*)&m_uGroup, sizeof(m_uGroup) );
}

//客户端向服务器发送的联系人相关结构数据
VOID CG_RELATION::CleanUp( )
{
	m_Type = REQ_NONE;
}

UINT CG_RELATION::GetSize( ) const
{
	UINT uSize = sizeof(m_Type);

	switch( m_Type )
	{
	case REQ_RELATIONLIST:
		break;
	case REQ_RELATIONINFO:
		uSize += m_RequestInfo.GetSize();
		break;
	case REQ_VIEWPLAYER:
	case REQ_NOTIFY_ADDTEMPFRIEND:
		uSize += m_ViewPlayer.GetSize();
		break;
	case REQ_DELFROMBLACKLIST:
		uSize += m_RelationGUID.GetSize();
		break;
	case REQ_DELFRIEND:
		uSize += m_DelFriend.GetSize();
		break;
	case REQ_ADDFRIEND:
	case REQ_TEMPFRIEND_TO_FRIEND:
		uSize += m_AddRelationWithGroup.GetSize();
		break;
	case REQ_ADDTOBLACKLIST:
	case REQ_TEMPFRIEND_ADDTO_BLACKLIST:
		uSize += m_AddRelation.GetSize();
		break;
	case REQ_TRANSITION:
		uSize += m_RelationGUIDUCHARUCHAR.GetSize();
		break;
	case REQ_MODIFYMOOD:
		uSize += m_ModifyMood.GetSize();
		break;
	default:
		Assert(FALSE);
		return 0;
	}

	return uSize;
}

VOID CG_RELATION::Read( SocketInputStream &iStream )
{
	iStream.Read( (CHAR*)&m_Type, sizeof(m_Type) );

	switch( m_Type )
	{
	case REQ_RELATIONLIST:
		break;
	case REQ_RELATIONINFO:
		m_RequestInfo.CleanUp();
		m_RequestInfo.Read(iStream);
		break;
	case REQ_VIEWPLAYER:
	case REQ_NOTIFY_ADDTEMPFRIEND:
		m_ViewPlayer.CleanUp();
		m_ViewPlayer.Read(iStream);
		break;
	case REQ_DELFROMBLACKLIST:
		m_RelationGUID.CleanUp();
		m_RelationGUID.Read(iStream);
		break;
	case REQ_DELFRIEND:
		m_DelFriend.CleanUp();
		m_DelFriend.Read(iStream);
		break;
	case REQ_ADDFRIEND:
	case REQ_TEMPFRIEND_TO_FRIEND:
		m_AddRelationWithGroup.CleanUp();
		m_AddRelationWithGroup.Read(iStream);
		break;
	case REQ_ADDTOBLACKLIST:
	case REQ_TEMPFRIEND_ADDTO_BLACKLIST:
		m_AddRelation.CleanUp();
		m_AddRelation.Read(iStream);
		break;
	case REQ_TRANSITION:
		m_RelationGUIDUCHARUCHAR.CleanUp();
		m_RelationGUIDUCHARUCHAR.Read(iStream);
		break;
	case REQ_MODIFYMOOD:
		m_ModifyMood.CleanUp();
		m_ModifyMood.Read(iStream);
		break;
	default:
		Assert(FALSE);
		return;
	}
}

VOID CG_RELATION::Write( SocketOutputStream &oStream ) const
{
	oStream.Write( (CHAR*)&m_Type, sizeof(m_Type) );

	switch( m_Type )
	{
	case REQ_RELATIONLIST:
		break;
	case REQ_RELATIONINFO:
		m_RequestInfo.Write(oStream);
		break;
	case REQ_VIEWPLAYER:
	case REQ_NOTIFY_ADDTEMPFRIEND:
		m_ViewPlayer.Write(oStream);
		break;
	case REQ_DELFROMBLACKLIST:
		m_RelationGUID.Write(oStream);
		break;
	case REQ_DELFRIEND:
		m_DelFriend.Write(oStream);
		break;
	case REQ_ADDFRIEND:
	case REQ_TEMPFRIEND_TO_FRIEND:
		m_AddRelationWithGroup.Write(oStream);
		break;
	case REQ_ADDTOBLACKLIST:
	case REQ_TEMPFRIEND_ADDTO_BLACKLIST:
		m_AddRelation.Write(oStream);
		break;
	case REQ_TRANSITION:
		m_RelationGUIDUCHARUCHAR.Write(oStream);
		break;
	case REQ_MODIFYMOOD:
		m_ModifyMood.Write(oStream);
		break;
	default:
		Assert(FALSE);
		return;
	}
}

//////////////////////////////////////////////////////////////////////////

// 参数为 GUID 和 UCHAR
VOID RELATION_GUID_UCHAR::CleanUp( )
{
	m_TargetGUID = INVALID_ID;
	m_uRelationType = 0;
}

UINT RELATION_GUID_UCHAR::GetSize( ) const
{
	return sizeof(m_TargetGUID) + sizeof(m_uRelationType);
}

VOID RELATION_GUID_UCHAR::Read( SocketInputStream &iStream )
{
	iStream.Read( (CHAR*)&m_TargetGUID, sizeof(m_TargetGUID) );
	iStream.Read( (CHAR*)&m_uRelationType, sizeof(m_uRelationType) );
}

VOID RELATION_GUID_UCHAR::Write( SocketOutputStream &oStream ) const
{
	oStream.Write( (CHAR*)&m_TargetGUID, sizeof(m_TargetGUID) );
	oStream.Write( (CHAR*)&m_uRelationType, sizeof(m_uRelationType) );
}

// 修改联系人设置
VOID REQUEST_MODIFY_SETTINGS::CleanUp( )
{
	m_uSettings = 0;
}

UINT REQUEST_MODIFY_SETTINGS::GetSize( ) const
{
	return sizeof(m_uSettings);
}

VOID REQUEST_MODIFY_SETTINGS::Read( SocketInputStream &iStream )
{
	iStream.Read( (CHAR*)&m_uSettings, sizeof(m_uSettings) );
}

VOID REQUEST_MODIFY_SETTINGS::Write( SocketOutputStream &oStream ) const
{
	oStream.Write( (CHAR*)&m_uSettings, sizeof(m_uSettings) );
}

//服务器向 World 发送的联系人相关结构数据
VOID GW_RELATION::CleanUp( )
{
	m_Type = REQ_NONE;
	m_GUID = INVALID_ID;
}

UINT GW_RELATION::GetSize( ) const
{
	UINT uSize = sizeof(m_Type) + sizeof(m_GUID);

	switch( m_Type )
	{
	case REQ_RELATIONINFO:
		uSize += m_RequestInfo.GetSize();
		break;
	case REQ_VIEWPLAYER:
	case REQ_NOTIFY_ADDTEMPFRIEND:
		uSize += m_ViewPlayer.GetSize();
		break;
	case REQ_DELFRIEND:
	case REQ_DELFROMBLACKLIST:
	case REQ_NEWGOODFRIEND:
		uSize += m_RelationGUID.GetSize();
		break;
	case REQ_ADDFRIEND:
	case REQ_TEMPFRIEND_TO_FRIEND:
		uSize += m_AddRelationWithGroup.GetSize();
		break;
	case REQ_ADDTOBLACKLIST:
	case REQ_TEMPFRIEND_ADDTO_BLACKLIST:
		uSize += m_AddRelation.GetSize();
		break;
	case REQ_TRANSITION:
		uSize += m_TransitionRelation.GetSize();
		break;
	case REQ_RELATIONONLINE:
		break;
	case REQ_MODIFYMOOD:
		uSize += m_ModifyMood.GetSize();
		break;
	case REQ_MODIFYSETTINGS:
		uSize += m_Settings.GetSize();
		break;
	default:
		Assert(FALSE);
		return 0;
	}

	return uSize;
}

VOID GW_RELATION::Read( SocketInputStream &iStream )
{
	iStream.Read( (CHAR*)&m_Type, sizeof(m_Type) );
	iStream.Read( (CHAR*)&m_GUID, sizeof(m_GUID) );

	switch( m_Type )
	{
	case REQ_RELATIONINFO:
		m_RequestInfo.CleanUp();
		m_RequestInfo.Read(iStream);
		break;
	case REQ_VIEWPLAYER:
	case REQ_NOTIFY_ADDTEMPFRIEND:
		m_ViewPlayer.CleanUp();
		m_ViewPlayer.Read(iStream);
		break;
	case REQ_DELFRIEND:
	case REQ_DELFROMBLACKLIST:
	case REQ_NEWGOODFRIEND:
		m_RelationGUID.CleanUp();
		m_RelationGUID.Read(iStream);
		break;
	case REQ_ADDFRIEND:
	case REQ_TEMPFRIEND_TO_FRIEND:
		m_AddRelationWithGroup.CleanUp();
		m_AddRelationWithGroup.Read(iStream);
		break;
	case REQ_ADDTOBLACKLIST:
	case REQ_TEMPFRIEND_ADDTO_BLACKLIST:
		m_AddRelation.CleanUp();
		m_AddRelation.Read(iStream);
		break;
	case REQ_TRANSITION:
		m_TransitionRelation.CleanUp();
		m_TransitionRelation.Read(iStream);
		break;
	case REQ_RELATIONONLINE:
		break;
	case REQ_MODIFYMOOD:
		m_ModifyMood.CleanUp();
		m_ModifyMood.Read(iStream);
		break;
	case REQ_MODIFYSETTINGS:
		m_Settings.CleanUp();
		m_Settings.Read(iStream);
		break;
	default:
		Assert(FALSE);
		return;
	}
}

VOID GW_RELATION::Write( SocketOutputStream &oStream ) const
{
	oStream.Write( (CHAR*)&m_Type, sizeof(m_Type) );
	oStream.Write( (CHAR*)&m_GUID, sizeof(m_GUID) );

	switch( m_Type )
	{
	case REQ_RELATIONINFO:
		m_RequestInfo.Write(oStream);
		break;
	case REQ_VIEWPLAYER:
	case REQ_NOTIFY_ADDTEMPFRIEND:
		m_ViewPlayer.Write(oStream);
		break;
	case REQ_DELFRIEND:
	case REQ_DELFROMBLACKLIST:
	case REQ_NEWGOODFRIEND:
		m_RelationGUID.Write(oStream);
		break;
	case REQ_ADDFRIEND:
	case REQ_TEMPFRIEND_TO_FRIEND:
		m_AddRelationWithGroup.Write(oStream);
		break;
	case REQ_ADDTOBLACKLIST:
	case REQ_TEMPFRIEND_ADDTO_BLACKLIST:
		m_AddRelation.Write(oStream);
		break;
	case REQ_TRANSITION:
		m_TransitionRelation.Write(oStream);
		break;
	case REQ_RELATIONONLINE:
		break;
	case REQ_MODIFYMOOD:
		m_ModifyMood.Write(oStream);
		break;
	case REQ_MODIFYSETTINGS:
		m_Settings.Write(oStream);
		break;
	default:
		Assert(FALSE);
		return;
	}
}
/////////////////////////////////////////////////////////////////////////////////

VOID RETURN_ADD_RELATION::CleanUp( )
{
	m_uRelationType = 0;
	m_uGroup = 0;

	m_Relation.CleanUp();
}

UINT RETURN_ADD_RELATION::GetSize( ) const
{
	UINT uSize;

	uSize = sizeof(m_uRelationType) + sizeof(m_uGroup) + m_Relation.GetSize();

	return uSize;
}

VOID RETURN_ADD_RELATION::Read( SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)&m_uRelationType, sizeof(m_uRelationType) );
	iStream.Read( (CHAR*)&m_uGroup, sizeof(m_uGroup) );

	m_Relation.Read( iStream );
}

VOID RETURN_ADD_RELATION::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)&m_uRelationType, sizeof(m_uRelationType) );
	oStream.Write( (CHAR*)&m_uGroup, sizeof(m_uGroup) );

	m_Relation.Write( oStream );
}

VOID RETURN_RELATION_INFO::CleanUp( )
{
	m_uRelationType = RELATION_TYPE_TEMPFRIEND;
	m_uGroup = 0;
	m_nFriendpoint = 0;

	m_Relation.CleanUp();
}

UINT RETURN_RELATION_INFO::GetSize( ) const
{
	UINT uSize;

	uSize = sizeof(m_uRelationType) + m_Relation.GetSize();

	switch( m_uRelationType )
	{
	case RELATION_TYPE_BLACKNAME:
	case RELATION_TYPE_TEMPFRIEND:
		break;
	default:
		uSize += sizeof(m_uGroup) + sizeof(m_nFriendpoint);
	}

	return uSize;
}

VOID RETURN_RELATION_INFO::Read( SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)&m_uRelationType, sizeof(m_uRelationType) );

	switch( m_uRelationType )
	{
	case RELATION_TYPE_BLACKNAME:
	case RELATION_TYPE_TEMPFRIEND:
		break;
	default:
		iStream.Read( (CHAR*)&m_uGroup, sizeof(m_uGroup) );
		iStream.Read( (CHAR*)&m_nFriendpoint, sizeof(m_nFriendpoint) );
	}

	m_Relation.Read( iStream );
}

VOID RETURN_RELATION_INFO::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)&m_uRelationType, sizeof(m_uRelationType) );

	switch( m_uRelationType )
	{
	case RELATION_TYPE_BLACKNAME:
	case RELATION_TYPE_TEMPFRIEND:
		break;
	default:
		oStream.Write( (CHAR*)&m_uGroup, sizeof(m_uGroup) );
		oStream.Write( (CHAR*)&m_nFriendpoint, sizeof(m_nFriendpoint) );
	}

	m_Relation.Write( oStream );
}

// 右键查看玩家信息
VOID RETURN_VIEW_PLAYER::CleanUp( )
{
	m_GUID = INVALID_ID;
	m_uNameSize = 0;
	memset((void*)m_szName, 0, sizeof(m_szName));
}

UINT RETURN_VIEW_PLAYER::GetSize( ) const
{
	return sizeof(m_GUID) + sizeof(m_uNameSize) + m_uNameSize;
}

VOID RETURN_VIEW_PLAYER::Read( SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)&m_GUID, sizeof(m_GUID) );
	iStream.Read( (CHAR*)&m_uNameSize, sizeof(m_uNameSize) );

	if( m_uNameSize > 0 && m_uNameSize < sizeof(m_szName) )
	{
		iStream.Read(m_szName, m_uNameSize);
	}
}

VOID RETURN_VIEW_PLAYER::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)&m_GUID, sizeof(m_GUID) );
	oStream.Write( (CHAR*)&m_uNameSize, sizeof(m_uNameSize) );

	if( m_uNameSize > 0 && m_uNameSize < sizeof(m_szName) )
	{
		oStream.Write(m_szName, m_uNameSize);
	}
}

VOID RETURN_NOTIFY_FRIEND::CleanUp( )
{
	m_GUID = INVALID_ID;
	m_uNameSize = 0;
	memset((void*)m_szName, 0, sizeof(m_szName));
}

UINT RETURN_NOTIFY_FRIEND::GetSize( ) const
{
	return sizeof(m_GUID) + sizeof(m_uNameSize) + m_uNameSize;
}

VOID RETURN_NOTIFY_FRIEND::Read( SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)&m_GUID, sizeof(m_GUID) );
	iStream.Read( (CHAR*)&m_uNameSize, sizeof(m_uNameSize) );

	if( m_uNameSize > 0 && m_uNameSize < sizeof(m_szName) )
	{
		iStream.Read(m_szName, m_uNameSize);
	}
}

VOID RETURN_NOTIFY_FRIEND::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)&m_GUID, sizeof(m_GUID) );
	oStream.Write( (CHAR*)&m_uNameSize, sizeof(m_uNameSize) );

	if( m_uNameSize > 0 && m_uNameSize < sizeof(m_szName) )
	{
		oStream.Write(m_szName, m_uNameSize);
	}
}

VOID _RELATION_ONLINE::CleanUp( )
{
	m_GUID = INVALID_ID;
	m_uMoodSize = 0;
	memset( (void*)m_szMood, 0, sizeof(m_szMood) );
}

UINT _RELATION_ONLINE::GetSize( ) const
{
	return sizeof(m_GUID) + sizeof(m_uMoodSize) + m_uMoodSize;
}

VOID _RELATION_ONLINE::Read( SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)&m_GUID, sizeof(m_GUID) );
	iStream.Read( (CHAR*)&m_uMoodSize, sizeof(m_uMoodSize) );

	if( m_uMoodSize > 0 )
	{
		iStream.Read( (CHAR*)m_szMood, m_uMoodSize );
	}
}

VOID _RELATION_ONLINE::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)&m_GUID, sizeof(m_GUID) );
	oStream.Write( (CHAR*)&m_uMoodSize, sizeof(m_uMoodSize) );

	if( m_uMoodSize > 0 )
	{
		oStream.Write( (CHAR*)m_szMood, m_uMoodSize );
	}
}

// 用于给玩家在线密友列表
VOID RETURN_ONLINE_LIST::CleanUp( )
{
	m_uOnlineCount = 0;

	for( INT i=0; i<RELATION_BLACKNAME_OFFSET-RELATION_FRIEND_OFFSET; ++i )
	{
		m_OnlineInfo[i].CleanUp();
	}
}

UINT RETURN_ONLINE_LIST::GetSize( ) const
{
	UINT uSize = sizeof(m_uOnlineCount);

	if( m_uOnlineCount > 0 && m_uOnlineCount < RELATION_BLACKNAME_OFFSET-RELATION_FRIEND_OFFSET )
	{
		for( INT i=0; i<m_uOnlineCount; ++i )
		{
			uSize += m_OnlineInfo[i].GetSize();
		}
	}

	return uSize;
}

VOID RETURN_ONLINE_LIST::Read( SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)&m_uOnlineCount, sizeof(m_uOnlineCount) );

	if( m_uOnlineCount > 0 && m_uOnlineCount < RELATION_BLACKNAME_OFFSET-RELATION_FRIEND_OFFSET )
	{
		for( INT i=0; i<m_uOnlineCount; ++i )
		{
			m_OnlineInfo[i].Read(iStream);
		}
	}
}

VOID RETURN_ONLINE_LIST::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)&m_uOnlineCount, sizeof(m_uOnlineCount) );

	if( m_uOnlineCount > 0 && m_uOnlineCount < RELATION_BLACKNAME_OFFSET-RELATION_FRIEND_OFFSET )
	{
		for( INT i=0; i<m_uOnlineCount; ++i )
		{
			m_OnlineInfo[i].Write(oStream);
		}
	}
}

// 用于通知有密友上线
VOID RETURN_NOTIFY_ONLINE::CleanUp( )
{
	m_uNameSize = 0;
	memset( (void*)m_szTargetName, 0, sizeof(m_szTargetName) );
	m_uMoodSize = 0;
	memset( (void*)m_szMood, 0, sizeof(m_szMood) );
}

UINT RETURN_NOTIFY_ONLINE::GetSize( ) const
{
	return sizeof(m_uNameSize) + m_uNameSize + sizeof(m_uMoodSize) + m_uMoodSize;
}

VOID RETURN_NOTIFY_ONLINE::Read( SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)&m_uNameSize, sizeof(m_uNameSize) );

	if( m_uNameSize > 0 )
	{
		iStream.Read( (CHAR*)&m_szTargetName, m_uNameSize );
	}

	iStream.Read( (CHAR*)&m_uMoodSize, sizeof(m_uMoodSize) );

	if( m_uMoodSize > 0 )
	{
		iStream.Read( (CHAR*)m_szMood, m_uMoodSize );
	}
}

VOID RETURN_NOTIFY_ONLINE::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)&m_uNameSize, sizeof(m_uNameSize) );

	if( m_uNameSize > 0 )
	{
		oStream.Write( (CHAR*)&m_szTargetName, m_uNameSize );
	}

	oStream.Write( (CHAR*)&m_uMoodSize, sizeof(m_uMoodSize) );

	if( m_uMoodSize > 0 )
	{
		oStream.Write( (CHAR*)m_szMood, m_uMoodSize );
	}
}

// World 向服务器发送的联系人相关结构数据
VOID WG_RELATION::CleanUp( )
{
	m_PlayerID = INVALID_ID;
	m_Type = RET_NONE;
}

UINT WG_RELATION::GetSize( ) const
{
	UINT uSize = sizeof(m_Type) + sizeof(m_PlayerID);

	switch( m_Type )
	{
	case RET_RELATIONINFO:
		uSize += m_RelationInfo.GetSize();
		break;
	case RET_VIEWPLAYER:
		uSize += m_ViewPlayer.GetSize();
		break;
	case RET_TARGETNOTONLINE:
	case RET_RELATIONOFFLINE:
		uSize += m_RelationGUID.GetSize();
		break;
	case RET_ADDFRIEND:
	case RET_ADDTOBLACKLIST:
	case RET_TEMPFRIEND_TO_FRIEND:
	case RET_TEMPFRIEND_ADDTO_BLACKLIST:
		uSize += m_AddRelation.GetSize();
		break;
	case RET_ADDFRIENDNOTIFY:
		uSize += m_NotifyFriend.GetSize();
		break;
	case RET_ONLINELIST:
		uSize += m_RelationOnline.GetSize();
		break;
	case RET_RELATIONONLINE:
		uSize += m_NotifyOnline.GetSize();
		break;
	case RET_NOTIFY_ADDTEMPFRIEND:
		uSize += m_PlayerName.GetSize();
		break;
	case RET_ERR_TARGETNOTEXIST:
	case RET_ERR_ISFRIEND:
	case RET_ERR_ISBLACKNAME:
	case RET_ERR_SPOUSETOBLACKLIST:
	case RET_ERR_MASTERTOBLACKLIST:
	case RET_ERR_PRENTICETOBLACKLIST:
	case RET_ERR_BROTHERTOBLACKLIST:
	case RET_ERR_CANNOT_ADDFRIEND:
	case RET_ERR_CANNOTRECEIVEMAIL:
	case RET_ERR_NOTRECVSTRANGEMAIL:
		break;
	default:
		Assert(FALSE);
		return 0;
	}

	return uSize;
}

VOID WG_RELATION::Read( SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)&m_PlayerID, sizeof(m_PlayerID) );
	iStream.Read( (CHAR*)&m_Type, sizeof(m_Type) );

	switch( m_Type )
	{
	case RET_RELATIONINFO:
		m_RelationInfo.CleanUp();
		m_RelationInfo.Read( iStream );
		break;
	case RET_VIEWPLAYER:
		m_ViewPlayer.CleanUp();
		m_ViewPlayer.Read( iStream );
		break;
	case RET_TARGETNOTONLINE:
	case RET_RELATIONOFFLINE:
		m_RelationGUID.CleanUp();
		m_RelationGUID.Read( iStream );
		break;
	case RET_ADDFRIEND:
	case RET_ADDTOBLACKLIST:
	case RET_TEMPFRIEND_TO_FRIEND:
	case RET_TEMPFRIEND_ADDTO_BLACKLIST:
		m_AddRelation.CleanUp();
		m_AddRelation.Read( iStream );
		break;
	case RET_ADDFRIENDNOTIFY:
		m_NotifyFriend.CleanUp();
		m_NotifyFriend.Read( iStream );
		break;
	case RET_ONLINELIST:
		m_RelationOnline.CleanUp();
		m_RelationOnline.Read( iStream );
		break;
	case RET_RELATIONONLINE:
		m_NotifyOnline.CleanUp();
		m_NotifyOnline.Read( iStream );
		break;
	case RET_NOTIFY_ADDTEMPFRIEND:
		m_PlayerName.CleanUp();
		m_PlayerName.Read( iStream );
		break;
	case RET_ERR_TARGETNOTEXIST:
	case RET_ERR_ISFRIEND:
	case RET_ERR_ISBLACKNAME:
	case RET_ERR_SPOUSETOBLACKLIST:
	case RET_ERR_MASTERTOBLACKLIST:
	case RET_ERR_PRENTICETOBLACKLIST:
	case RET_ERR_BROTHERTOBLACKLIST:
	case RET_ERR_CANNOT_ADDFRIEND:
	case RET_ERR_CANNOTRECEIVEMAIL:
	case RET_ERR_NOTRECVSTRANGEMAIL:
		break;
	default:
		Assert(FALSE);
		return;
	}
}

VOID WG_RELATION::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)&m_PlayerID, sizeof(m_PlayerID) );
	oStream.Write( (CHAR*)&m_Type, sizeof(m_Type) );

	switch( m_Type )
	{
	case RET_RELATIONINFO:
		m_RelationInfo.Write( oStream );
		break;
	case RET_VIEWPLAYER:
		m_ViewPlayer.Write( oStream );
		break;
	case RET_TARGETNOTONLINE:
	case RET_RELATIONOFFLINE:
		m_RelationGUID.Write( oStream );
		break;
	case RET_ADDFRIEND:
	case RET_ADDTOBLACKLIST:
	case RET_TEMPFRIEND_TO_FRIEND:
	case RET_TEMPFRIEND_ADDTO_BLACKLIST:
		m_AddRelation.Write( oStream );
		break;
	case RET_ADDFRIENDNOTIFY:
		m_NotifyFriend.Write( oStream );
		break;
	case RET_ONLINELIST:
		m_RelationOnline.Write( oStream );
		break;
	case RET_RELATIONONLINE:
		m_NotifyOnline.Write( oStream );
		break;
	case RET_NOTIFY_ADDTEMPFRIEND:
		m_PlayerName.Write( oStream );
		break;
	case RET_ERR_TARGETNOTEXIST:
	case RET_ERR_ISFRIEND:
	case RET_ERR_ISBLACKNAME:
	case RET_ERR_SPOUSETOBLACKLIST:
	case RET_ERR_MASTERTOBLACKLIST:
	case RET_ERR_PRENTICETOBLACKLIST:
	case RET_ERR_BROTHERTOBLACKLIST:
	case RET_ERR_CANNOT_ADDFRIEND:
	case RET_ERR_CANNOTRECEIVEMAIL:
	case RET_ERR_NOTRECVSTRANGEMAIL:
		break;
	default:
		Assert(FALSE);
		return;
	}
}


/////////////////////////////////////////////////////////////////////////////////

// 好友列表

VOID _FRIEND_INFO::Read( SocketInputStream &iStream )
{
	iStream.Read( (CHAR*)&m_GUID, sizeof(m_GUID) );
	iStream.Read( (CHAR*)&m_uNameSize, sizeof(m_uNameSize) );

	if( m_uNameSize > 0 && m_uNameSize < sizeof(m_szTargetName) )
	{
		iStream.Read(m_szTargetName, m_uNameSize);
	}

	iStream.Read( (CHAR*)&m_uRelationType, sizeof(m_uRelationType) );
	iStream.Read( (CHAR*)&m_uGroup, sizeof(m_uGroup) );
	iStream.Read( (CHAR*)&m_nFriendpoint, sizeof(m_nFriendpoint) );
}

VOID _FRIEND_INFO::Write( SocketOutputStream &oStream ) const
{
	oStream.Write( (CHAR*)&m_GUID, sizeof(m_GUID) );
	oStream.Write( (CHAR*)&m_uNameSize, sizeof(m_uNameSize) );

	if( m_uNameSize > 0 && m_uNameSize < sizeof(m_szTargetName) )
	{
		oStream.Write(m_szTargetName, m_uNameSize);
	}

	oStream.Write( (CHAR*)&m_uRelationType, sizeof(m_uRelationType) );
	oStream.Write( (CHAR*)&m_uGroup, sizeof(m_uGroup) );
	oStream.Write( (CHAR*)&m_nFriendpoint, sizeof(m_nFriendpoint) );
}

VOID _BLACKNAME_INFO::Read( SocketInputStream &iStream )
{
	iStream.Read( (CHAR*)&m_GUID, sizeof(m_GUID) );
	iStream.Read( (CHAR*)&m_uNameSize, sizeof(m_uNameSize) );

	if( m_uNameSize > 0 && m_uNameSize < sizeof(m_szTargetName) )
	{
		iStream.Read(m_szTargetName, m_uNameSize);
	}
}

VOID _BLACKNAME_INFO::Write( SocketOutputStream &oStream ) const
{
	oStream.Write( (CHAR*)&m_GUID, sizeof(m_GUID) );
	oStream.Write( (CHAR*)&m_uNameSize, sizeof(m_uNameSize) );

	if( m_uNameSize > 0 && m_uNameSize < sizeof(m_szTargetName) )
	{
		oStream.Write(m_szTargetName, m_uNameSize);
	}
}

VOID GC_RELATIONLIST::CleanUp( )
{
	m_uFriendCount = 0;
	m_uBlackCount = 0;
	m_uMoodSize = 0;
	memset((void*)m_szMood, 0, sizeof(m_szMood));

	for( INT i=0; i<RELATION_BLACKNAME_OFFSET - RELATION_FRIEND_OFFSET; ++i )
	{
		m_FriendInfo[i].CleanUp();
	}

	for( INT i=0; i<MAX_RELATION_SIZE - RELATION_BLACKNAME_OFFSET; ++i )
	{
		m_BlackNameInfo[i].CleanUp();
	}
}

UINT GC_RELATIONLIST::GetSize( ) const
{
	UINT uSize;

	uSize = sizeof(m_uFriendCount) + sizeof(m_uBlackCount) + sizeof(m_uMoodSize) + m_uMoodSize;

	for( INT i=0; i<m_uFriendCount; ++i )
	{
		uSize += m_FriendInfo[i].GetSize();
	}

	for( INT i=0; i<m_uBlackCount; ++i )
	{
		uSize += m_BlackNameInfo[i].GetSize();
	}

	return uSize;
}

VOID GC_RELATIONLIST::Read( SocketInputStream &iStream )
{
	iStream.Read( (CHAR*)&m_uFriendCount, sizeof(m_uFriendCount) );
	iStream.Read( (CHAR*)&m_uBlackCount, sizeof(m_uBlackCount) );
	iStream.Read( (CHAR*)&m_uMoodSize, sizeof(m_uMoodSize) );

	if( m_uMoodSize > 0 && m_uMoodSize < MOOD_DATA_SIZE )
	{
		iStream.Read( (CHAR*)m_szMood, m_uMoodSize );
	}

	for( INT i=0; i<m_uFriendCount; ++i )
	{
		m_FriendInfo[i].Read(iStream);
	}

	for( INT i=0; i<m_uBlackCount; ++i )
	{
		m_BlackNameInfo[i].Read(iStream);
	}
}

VOID GC_RELATIONLIST::Write( SocketOutputStream &oStream ) const
{
	oStream.Write( (CHAR*)&m_uFriendCount, sizeof(m_uFriendCount) );
	oStream.Write( (CHAR*)&m_uBlackCount, sizeof(m_uBlackCount) );
	oStream.Write( (CHAR*)&m_uMoodSize, sizeof(m_uMoodSize) );

	if( m_uMoodSize > 0 && m_uMoodSize < MOOD_DATA_SIZE )
	{
		oStream.Write( (CHAR*)m_szMood, m_uMoodSize );
	}

	for( INT i=0; i<m_uFriendCount; ++i )
	{
		m_FriendInfo[i].Write(oStream);
	}

	for( INT i=0; i<m_uBlackCount; ++i )
	{
		m_BlackNameInfo[i].Write(oStream);
	}
}

VOID GC_RELATIONLIST::AddFriend( const _OWN_RELATION* pRelation )
{
	Assert(pRelation);
	Assert(m_uFriendCount<RELATION_BLACKNAME_OFFSET - RELATION_FRIEND_OFFSET);
	m_FriendInfo[m_uFriendCount].m_GUID = pRelation->m_Member.m_MemberGUID;
	strncpy(m_FriendInfo[m_uFriendCount].m_szTargetName, pRelation->m_Member.m_szMemberName,
		sizeof(m_FriendInfo[m_uFriendCount].m_szTargetName) - 1);
	m_FriendInfo[m_uFriendCount].m_uNameSize = (UCHAR)strlen(pRelation->m_Member.m_szMemberName);
	m_FriendInfo[m_uFriendCount].m_uRelationType = pRelation->m_Type;
	m_FriendInfo[m_uFriendCount].m_uGroup = pRelation->m_Group;
	m_FriendInfo[m_uFriendCount].m_nFriendpoint = pRelation->m_FriendPoint;

	++m_uFriendCount;
}

const _FRIEND_INFO* GC_RELATIONLIST::GetFriend( INT index )
{
	Assert(index<m_uFriendCount);
	return &(m_FriendInfo[index]);
	//pRelation->m_Member.m_MemberGUID = m_FriendInfo[index].m_GUID;
	//strncpy(pRelation->m_Member.m_szMemberName, m_FriendInfo[index].m_szTargetName,
	//	sizeof(m_FriendInfo[index].m_szTargetName) - 1);
	//pRelation->m_Group = m_FriendInfo[index].m_uGroup;
	//pRelation->m_FriendPoint = m_FriendInfo[index].m_nFriendpoint;

	//return TRUE;
}

VOID GC_RELATIONLIST::AddBlackName( const _OWN_RELATION* pRelation )
{
	Assert(pRelation);
	Assert(m_uBlackCount<MAX_RELATION_SIZE - RELATION_BLACKNAME_OFFSET);
	m_BlackNameInfo[m_uBlackCount].m_GUID = pRelation->m_Member.m_MemberGUID;
	strncpy(m_BlackNameInfo[m_uBlackCount].m_szTargetName, pRelation->m_Member.m_szMemberName,
		sizeof(m_BlackNameInfo[m_uBlackCount].m_szTargetName) - 1);
	m_BlackNameInfo[m_uBlackCount].m_uNameSize = (UCHAR)strlen(pRelation->m_Member.m_szMemberName);

	++m_uBlackCount;
}

const _BLACKNAME_INFO* GC_RELATIONLIST::GetBlackName( INT index )
{
	Assert(index<m_uBlackCount);
	return &(m_BlackNameInfo[index]);
	//pRelation->m_Member.m_MemberGUID = m_BlackNameInfo[index].m_GUID;
	//strncpy(pRelation->m_Member.m_szMemberName, m_BlackNameInfo[index].m_szTargetName,
	//	sizeof(m_BlackNameInfo[index].m_szTargetName) - 1);

	//return TRUE;
}

VOID GC_RELATION::CleanUp( )
{
	m_Type = RET_NONE;
}

UINT GC_RELATION::GetSize( ) const
{
	UINT uSize = sizeof(m_Type);

	switch( m_Type )
	{
	case RET_RELATIONLIST:
		uSize += m_RelationList.GetSize();
		break;
	case RET_RELATIONINFO:
		uSize += m_RelationInfo.GetSize();
		break;
	case RET_VIEWPLAYER:
		uSize += m_ViewPlayer.GetSize();
		break;
	case RET_ADDFRIEND:
	case RET_ADDTOBLACKLIST:
	case RET_TEMPFRIEND_TO_FRIEND:
	case RET_TEMPFRIEND_ADDTO_BLACKLIST:
		uSize += m_NewRelation.GetSize();
		break;
	case RET_TRANSITION:
		uSize += m_RelationGUIDUCHARUCHAR.GetSize();
		break;
	case RET_DELFRIEND:
	case RET_DELFROMBLACKLIST:
	case RET_RELATIONOFFLINE:
		uSize += m_RelationGUID.GetSize();
		break;
	case RET_ADDFRIENDNOTIFY:
		uSize += m_NotifyFriend.GetSize();
		break;
	case RET_ONLINELIST:
		uSize += m_RelationOnline.GetSize();
		break;
	case RET_RELATIONONLINE:
		uSize += m_NotifyOnline.GetSize();
		break;
	case RET_NEWMOOD:
		uSize += m_NewMood.GetSize();
		break;
	case RET_NOTIFY_ADDTEMPFRIEND:
		uSize += m_PlayerName.GetSize();
		break;
	case RET_ERR_TARGETNOTEXIST:
	case RET_ERR_GROUPISFULL:
	case RET_ERR_ISFRIEND:
	case RET_ERR_ISBLACKNAME:
	case RET_ERR_CANNOTTRANSITION:
	case RET_ERR_ISNOTFRIEND:
	case RET_ERR_ISNOTINBLACKLIST:
	case RET_ERR_SPOUSETOBLACKLIST:
	case RET_ERR_MASTERTOBLACKLIST:
	case RET_ERR_PRENTICETOBLACKLIST:
	case RET_ERR_BROTHERTOBLACKLIST:
	case RET_ERR_DELSPOUSE:
	case RET_ERR_DELMASTER:
	case RET_ERR_DELPRENTICE:
	case RET_ERR_DELBROTHER:
	case RET_ERR_PASSWDMISMATCH:
	case RET_ERR_CANNOT_ADDFRIEND:
	case RET_ERR_CANNOTRECEIVEMAIL:
	case RET_ERR_NOTRECVSTRANGEMAIL:
	case RET_ERR_RELATIONUNKNOWN:
		break;
	default :
		Assert(FALSE);
		return 0;
	};

	return uSize;
}

VOID GC_RELATION::Read( SocketInputStream& iStream )
{
	iStream.Read( (CHAR*)&m_Type, sizeof(m_Type) );

	switch( m_Type )
	{
	case RET_RELATIONLIST:
		m_RelationList.CleanUp();
		m_RelationList.Read(iStream);
		break;
	case RET_RELATIONINFO:
		m_RelationInfo.CleanUp();
		m_RelationInfo.Read(iStream);
		break;
	case RET_VIEWPLAYER:
		m_ViewPlayer.CleanUp();
		m_ViewPlayer.Read( iStream );
		break;
	case RET_ADDFRIEND:
	case RET_ADDTOBLACKLIST:
	case RET_TEMPFRIEND_TO_FRIEND:
	case RET_TEMPFRIEND_ADDTO_BLACKLIST:
		m_NewRelation.CleanUp();
		m_NewRelation.Read(iStream);
		break;
	case RET_TRANSITION:
		m_RelationGUIDUCHARUCHAR.CleanUp();
		m_RelationGUIDUCHARUCHAR.Read(iStream);
		break;
	case RET_DELFRIEND:
	case RET_DELFROMBLACKLIST:
	case RET_RELATIONOFFLINE:
		m_RelationGUID.CleanUp();
		m_RelationGUID.Read(iStream);
		break;
	case RET_ADDFRIENDNOTIFY:
		m_NotifyFriend.CleanUp();
		m_NotifyFriend.Read( iStream );
		break;
	case RET_ONLINELIST:
		m_RelationOnline.CleanUp();
		m_RelationOnline.Read( iStream );
		break;
	case RET_RELATIONONLINE:
		m_NotifyOnline.CleanUp();
		m_NotifyOnline.Read( iStream );
		break;
	case RET_NEWMOOD:
		m_NewMood.CleanUp();
		m_NewMood.Read( iStream );
		break;
	case RET_NOTIFY_ADDTEMPFRIEND:
		m_PlayerName.CleanUp();
		m_PlayerName.Read( iStream );
		break;
	case RET_ERR_TARGETNOTEXIST:
	case RET_ERR_GROUPISFULL:
	case RET_ERR_ISFRIEND:
	case RET_ERR_ISBLACKNAME:
	case RET_ERR_CANNOTTRANSITION:
	case RET_ERR_ISNOTFRIEND:
	case RET_ERR_ISNOTINBLACKLIST:
	case RET_ERR_SPOUSETOBLACKLIST:
	case RET_ERR_MASTERTOBLACKLIST:
	case RET_ERR_PRENTICETOBLACKLIST:
	case RET_ERR_BROTHERTOBLACKLIST:
	case RET_ERR_DELSPOUSE:
	case RET_ERR_DELMASTER:
	case RET_ERR_DELPRENTICE:
	case RET_ERR_DELBROTHER:
	case RET_ERR_PASSWDMISMATCH:
	case RET_ERR_CANNOT_ADDFRIEND:
	case RET_ERR_CANNOTRECEIVEMAIL:
	case RET_ERR_NOTRECVSTRANGEMAIL:
	case RET_ERR_RELATIONUNKNOWN:
		break;
	default :
		Assert(FALSE);
		return;
	};
}

VOID GC_RELATION::Write( SocketOutputStream& oStream ) const
{
	oStream.Write( (CHAR*)&m_Type, sizeof(m_Type) );

	switch( m_Type )
	{
	case RET_RELATIONLIST:
		m_RelationList.Write(oStream);
		break;
	case RET_RELATIONINFO:
		m_RelationInfo.Write(oStream);
		break;
	case RET_VIEWPLAYER:
		m_ViewPlayer.Write( oStream );
		break;
	case RET_ADDFRIEND:
	case RET_ADDTOBLACKLIST:
	case RET_TEMPFRIEND_TO_FRIEND:
	case RET_TEMPFRIEND_ADDTO_BLACKLIST:
		m_NewRelation.Write(oStream);
		break;
	case RET_TRANSITION:
		m_RelationGUIDUCHARUCHAR.Write(oStream);
		break;
	case RET_DELFRIEND:
	case RET_DELFROMBLACKLIST:
	case RET_RELATIONOFFLINE:
		m_RelationGUID.Write(oStream);
		break;
	case RET_ADDFRIENDNOTIFY:
		m_NotifyFriend.Write( oStream );
		break;
	case RET_ONLINELIST:
		m_RelationOnline.Write( oStream );
		break;
	case RET_RELATIONONLINE:
		m_NotifyOnline.Write( oStream );
		break;
	case RET_NEWMOOD:
		m_NewMood.Write( oStream );
		break;
	case RET_NOTIFY_ADDTEMPFRIEND:
		m_PlayerName.Write( oStream );
		break;
	case RET_ERR_TARGETNOTEXIST:
	case RET_ERR_GROUPISFULL:
	case RET_ERR_ISFRIEND:
	case RET_ERR_ISBLACKNAME:
	case RET_ERR_CANNOTTRANSITION:
	case RET_ERR_ISNOTFRIEND:
	case RET_ERR_ISNOTINBLACKLIST:
	case RET_ERR_SPOUSETOBLACKLIST:
	case RET_ERR_MASTERTOBLACKLIST:
	case RET_ERR_PRENTICETOBLACKLIST:
	case RET_ERR_BROTHERTOBLACKLIST:
	case RET_ERR_DELSPOUSE:
	case RET_ERR_DELMASTER:
	case RET_ERR_DELPRENTICE:
	case RET_ERR_DELBROTHER:
	case RET_ERR_PASSWDMISMATCH:
	case RET_ERR_CANNOT_ADDFRIEND:
	case RET_ERR_CANNOTRECEIVEMAIL:
	case RET_ERR_NOTRECVSTRANGEMAIL:
	case RET_ERR_RELATIONUNKNOWN:
		break;
	default :
		Assert(FALSE);
		return;
	};
}

