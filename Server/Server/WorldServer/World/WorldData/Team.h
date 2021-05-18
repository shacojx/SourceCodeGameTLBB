#ifndef __TEAM_H__
#define __TEAM_H__

#include "Type.h"

#define LEADER_ID 0



struct TEAMMEMBER
{
	GUID_t			m_Member ;

	void			CleanUp( )
	{
		m_Member = INVALID_ID ;
	}
};

class Team
{
public :
	Team( ) ;
	~Team( ) ;

	void			CleanUp( ) ;

	BOOL			IsActive( ) ;
	BOOL			AddMember( TEAMMEMBER* pMember ) ;
	BOOL			DelMember( TEAMMEMBER* pMember ) ;
	BOOL			IsMember( TEAMMEMBER* pMember ) ;
	TEAMMEMBER*		Leader( ) ;
	TEAMMEMBER*		Member( int iIndex ) ;
	int				MemberCount( ){ return m_iNumber ; }
	BOOL			IsFull( ){ return (BOOL)(m_iNumber==MAX_TEAM_MEMBER) ; }
	void			Appoint( TEAMMEMBER* pNewLeader ) ;

	void			SetTeamID( TeamID_t tid )
	{
		Assert( tid>=0 && tid<MAX_TEAMS ) ;
		m_TeamID = tid ;
	};
	TeamID_t		GetTeamID( )
	{
		return m_TeamID ;
	}
	BOOL			IsEmpty( )
	{
		return m_Empty ;
	}
	void			SetEmpty( BOOL bEmpty=TRUE )
	{
		m_Empty = bEmpty ;
	}

	void			AddFollowedMember(GUID_t guid)
	{
		Assert( m_nFollowedMemberCount<=MAX_TEAM_MEMBER );
		m_aFollowedMember[m_nFollowedMemberCount++] = guid;
	}

	void			DelFollowedMember(GUID_t guid);

	void			ClearFollowedMembers()
	{
		m_nFollowedMemberCount = 0;
	}

	INT				GetFollowedmemberCount()
	{
		return m_nFollowedMemberCount;
	}

	GUID_t*			GetFollowedMembers()
	{
		return m_aFollowedMember;
	}


public :
	TEAMMEMBER		m_aMember[MAX_TEAM_MEMBER] ;
	INT				m_iNumber ;

	TeamID_t		m_TeamID ;
	BOOL			m_Empty ;

	INT				m_nFollowedMemberCount;
	GUID_t			m_aFollowedMember[MAX_TEAM_MEMBER];
};


/////////////////////////////////////////////////////////////////////////////////
class TeamList
{
public :
	TeamList( ) ;
	~TeamList( ) ;

	void			CleanUp( ) ;
	BOOL			Init( ) ;

	BOOL			HeartBeat( DWORD dwTime ) ;


	Team*			GetTeam( TeamID_t tid )
	{
		Assert( tid>=0 && tid<MAX_TEAMS ) ;
		return &(m_pTeams[tid]) ;
	};
	TeamID_t		CreateTeam( ) ;
	BOOL			DestoryTeam( TeamID_t tid ) ;


public :
	Team*			m_pTeams ;
	int				m_iCount ;
	int				m_iCreateOffset ;	

};

extern TeamList* g_pTeamList ;





#endif
