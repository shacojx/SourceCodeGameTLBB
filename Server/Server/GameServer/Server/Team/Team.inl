/********************************************************************************
 *	文件名：	Team.inl
 *	全路径：	d:\Prj\Server\Server\Team\Team.inl
 *	创建时间：	2005 年 11 月 4 日	10:00
 *
 *	功能说明：	
 *	修改记录：
*********************************************************************************/

#ifndef __TEAM_INL__
#define __TEAM_INL__

VOID TeamInfo::CleanUp( )
{
	m_TeamID = INVALID_ID;
	m_MemberCount = 0; 
	m_MyGUID = INVALID_ID;
	m_MySceneID = INVALID_ID;
	m_SceneMemberCount = 0;
	m_bTeamFollowFlag = FALSE;
	m_nFollowedMembersCount = 0;
}

VOID TeamInfo::SetMyGUID( GUID_t guid )
{
	m_MyGUID = guid;
}

VOID TeamInfo::SetMySceneID( SceneID_t sid )
{
	m_MySceneID = sid;
}

VOID TeamInfo::DisMiss( )
{
	GUID_t MyGUID = m_MyGUID;

	CleanUp();
	m_MyGUID = MyGUID;
}

BOOL TeamInfo::HasTeam( ) const
{
	return ( m_MemberCount > 0 );
}

BOOL TeamInfo::IsFull( )
{
	return ( m_MemberCount >= MAX_TEAM_MEMBER );
}

BOOL TeamInfo::IsLeader( ) const
{
	if ( HasTeam( ) && Leader()->m_GUID == m_MyGUID )
	{
		return TRUE;
	}

	return FALSE;
}

TeamID_t TeamInfo::GetTeamID( ) const
{
	return m_TeamID;
}

VOID TeamInfo::SetTeamID( const TeamID_t teamid )
{
	m_TeamID = teamid;
}

INT TeamInfo::GetTeamMemberCount( ) const
{
	return m_MemberCount;
}

VOID TeamInfo::AddSceneMember( ObjID_t oid )
{
	if( m_SceneMemberCount>=MAX_TEAM_MEMBER )
	{ // 同场景的队友不包括自己，所以这里应该总小于 MAX_TEAM_MEMBER - 1
		// 但是因为这个函数有不止一个入口，所以可能会出现重复加入的情况
		// 但是不应该出现 >= MAX_TEAM_MEMBER 的情况
		Assert(FALSE);
		return;
	}

	if( oid == INVALID_ID )
	{
		return;
	}

	for( INT i=0; i<m_SceneMemberCount; ++i )
	{
		if( m_aScenePlayer[i] == oid )
			return;
	}

	m_aScenePlayer[m_SceneMemberCount++] = oid;
}

VOID TeamInfo::DelSceneMember( ObjID_t oid )
{
	for( INT i=0; i<m_SceneMemberCount; ++i )
	{
		if( m_aScenePlayer[i] == oid )
		{
			for( INT j=i; j<m_SceneMemberCount-1; ++j )
			{
				m_aScenePlayer[j] = m_aScenePlayer[j+1];
			}

			--m_SceneMemberCount;
			break;
		}
	}
}

INT TeamInfo::GetSceneMemberCount() const
{
	return m_SceneMemberCount;
}

const ObjID_t TeamInfo::GetSceneMemberObjID( const INT memberindex ) const
{
	if ( memberindex<0 || memberindex>=m_SceneMemberCount )
	{
		Assert( FALSE );
		return INVALID_ID;
	}

	return m_aScenePlayer[memberindex];
}

const TEAMMEMBER* TeamInfo::Leader( ) const
{ 
	return &(m_aMember[LEADER_ID]);
}

const TEAMMEMBER* TeamInfo::GetTeamMember( const INT memberindex ) const
{
	if ( memberindex<0 || memberindex>=m_MemberCount )
	{
		Assert( FALSE );
		return NULL;
	}

//	TEAMMEMBER* pMember = &(m_aMember[memberindex]);

	return &(m_aMember[memberindex]);
}

const TEAMMEMBER* TeamInfo::GetTeamMemberByGUID( const GUID_t guid ) const
{
	for( INT i=0; i<m_MemberCount; ++i )
	{
		if( m_aMember[i].m_GUID == guid )
		{
			return &(m_aMember[i]);
		}
	}

	return NULL;
}

// 设置队伍跟随状态
VOID TeamInfo::SetTeamFollowFlag(BOOL flag)
{
	m_bTeamFollowFlag = flag;
}

// 获得队伍跟随状态
const BOOL TeamInfo::GetTeamFollowFlag() const
{
	if ( HasTeam() )
	{
		return m_bTeamFollowFlag;
	}
	else
	{
		return FALSE;
	}
}

// 得到跟随队员的数量
const INT TeamInfo::GetFollowedMembersCount() const
{
	return m_nFollowedMembersCount;
}

// 得到第i个跟随的队友
const _FOLLOWEDMEMBER* TeamInfo::GetFollowedMember( INT i ) const
{
	if ( i<0 || i>=GetFollowedMembersCount() )
	{
		Assert(FALSE);
		return NULL;
	}

	return &(m_FollowedMembers[i]);
}

VOID TeamInfo::ClearFollowedMembers()
{
	m_nFollowedMembersCount = 0;

	for( INT i=0; i<MAX_TEAM_MEMBER; ++i )
	{
		m_FollowedMembers[i].CleanUp();
	}
}

// 清除跟随者指针
VOID TeamInfo::EraseFollowedMemberPointer( GUID_t guid )
{
	if ( m_bTeamFollowFlag )
	{
		for( INT i=0; i<GetFollowedMembersCount(); ++i )
		{
			if ( guid == m_FollowedMembers[i].m_GUID )
			{ // 更新指针
				m_FollowedMembers[i].m_pHuman = NULL;
				return;
			}
		}
	}
}

#endif // __TEAM_INL__
