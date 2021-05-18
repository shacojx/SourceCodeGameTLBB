#include "stdafx.h"
#include "Team.h"


TeamList* g_pTeamList = NULL ;


Team::Team( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

__LEAVE_FUNCTION
}

Team::~Team( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

__LEAVE_FUNCTION
}

void Team::CleanUp( )
{
__ENTER_FUNCTION

	m_iNumber = 0 ;
	m_nFollowedMemberCount = 0;
	m_Empty = TRUE ;
	for( int i=0; i<MAX_TEAM_MEMBER; i++ )
	{
		m_aMember[i].CleanUp( ) ;
	}

__LEAVE_FUNCTION
}

BOOL Team::IsActive( )
{
__ENTER_FUNCTION

	if( IsEmpty() ) return FALSE ;
	if( m_iNumber>0 ) return TRUE ;


__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Team::AddMember( TEAMMEMBER* pMember )
{
__ENTER_FUNCTION

	Assert( pMember ) ;

	if( m_iNumber>=MAX_TEAM_MEMBER )
		return FALSE ;

	m_aMember[m_iNumber] = *pMember ;
	m_iNumber ++ ;

__LEAVE_FUNCTION

	return TRUE ;
}

BOOL Team::DelMember( TEAMMEMBER* pMember )
{
__ENTER_FUNCTION

	Assert( pMember ) ;

	BOOL bFind = FALSE ;
	int i ;

	for( i=0; i<m_iNumber; i++ )
	{
		if( m_aMember[i].m_Member == pMember->m_Member )
		{
			bFind = TRUE ;
			break ;
		}
	}
	if( bFind )
	{
		for( ;i<m_iNumber-1; i++ )
		{
			m_aMember[i] = m_aMember[i+1] ;
		}

		m_iNumber -- ;
		Assert( m_iNumber>=0 ) ;

		if ( m_nFollowedMemberCount>0 )
		{
			DelFollowedMember( pMember->m_Member );
		}
		return TRUE ;
	}

__LEAVE_FUNCTION

	return FALSE ;
}

BOOL Team::IsMember( TEAMMEMBER* pMember )
{
__ENTER_FUNCTION

	Assert( pMember ) ;

	for( int i=0; i<m_iNumber; i++ )
	{
		if( m_aMember[i].m_Member == pMember->m_Member )
			return TRUE ;
	}

__LEAVE_FUNCTION

	return FALSE ;
}

TEAMMEMBER* Team::Leader( )
{
__ENTER_FUNCTION

	if( IsActive() )
		return &(m_aMember[LEADER_ID]) ;

__LEAVE_FUNCTION

	return NULL ;
}

TEAMMEMBER* Team::Member( int iIndex )
{
__ENTER_FUNCTION

	Assert( iIndex>=0 && iIndex<MAX_TEAM_MEMBER ) ;
	if( IsActive() )
		return &(m_aMember[iIndex]) ;

__LEAVE_FUNCTION

	return NULL ;
}

void Team::Appoint( TEAMMEMBER* pNewLeader )
{
__ENTER_FUNCTION

	TEAMMEMBER LM;
	BOOL flag = FALSE;
	INT i;

	for( i=0; i<m_iNumber; i++ )
	{
		if( m_aMember[i].m_Member == pNewLeader->m_Member )
		{
			LM = m_aMember[i];
			flag = TRUE;
			break;
		}
	}

	if ( flag )
	{
		for( ; i>0; --i )
		{
			m_aMember[i] = m_aMember[i-1];
		}

		m_aMember[LEADER_ID] = LM;
	}

__LEAVE_FUNCTION
}

void Team::DelFollowedMember(GUID_t guid)
{
__ENTER_FUNCTION

	for( INT i=0; i<m_nFollowedMemberCount; ++i )
	{
		if ( guid == m_aFollowedMember[i] )
		{
			for( INT j=i; j<GetFollowedmemberCount()-1; ++j )
			{
				m_aFollowedMember[j] = m_aFollowedMember[j+1];
			}

			--m_nFollowedMemberCount;
			return;
		}
	}

__LEAVE_FUNCTION
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
TeamList::TeamList( )
{
__ENTER_FUNCTION

	m_iCount = 0 ;
	m_iCreateOffset = 0 ;
	m_pTeams = NULL ;

__LEAVE_FUNCTION
}

TeamList::~TeamList( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

__LEAVE_FUNCTION
}

void TeamList::CleanUp( )
{
__ENTER_FUNCTION

	m_iCount = 0 ;
	m_iCreateOffset = 0 ;
	SAFE_DELETE_ARRAY( m_pTeams ) ;

__LEAVE_FUNCTION
}

BOOL TeamList::Init( )
{
__ENTER_FUNCTION

	m_iCount = 0 ;
	m_pTeams = new Team[MAX_TEAMS] ;
	Assert( m_pTeams ) ;

	for( int i=0; i<MAX_TEAMS; i++ )
	{
		m_pTeams[i].SetTeamID( (TeamID_t)i ) ;
	}


__LEAVE_FUNCTION

	return TRUE ;
}

BOOL TeamList::HeartBeat( DWORD dwTime )
{

	return TRUE ;
}

TeamID_t TeamList::CreateTeam( )
{
__ENTER_FUNCTION

	for( int i=0; i<MAX_TEAMS; i++ )
	{
		Team* pTeam = GetTeam(m_iCreateOffset) ;
		if( pTeam && pTeam->IsEmpty() )
		{
			TeamID_t tid = m_iCreateOffset ;
			pTeam->SetEmpty( FALSE ) ;
			m_iCount ++ ;
			Assert( m_iCount>=0&&m_iCount<MAX_TEAMS ) ;
			
			m_iCreateOffset ++ ;
			if( m_iCreateOffset >= MAX_TEAMS )
				m_iCreateOffset = 0 ;

			return tid ;
		}

		m_iCreateOffset ++ ;
		if( m_iCreateOffset >= MAX_TEAMS )
			m_iCreateOffset = 0 ;

	}

__LEAVE_FUNCTION

	return INVALID_ID ;
}

BOOL TeamList::DestoryTeam( TeamID_t tid )
{
__ENTER_FUNCTION

	Team* pTeam = GetTeam( tid ) ;
	if( pTeam )
	{
		pTeam->CleanUp( ) ;
		m_iCount -- ;
		Assert( m_iCount>=0 ) ;

		return TRUE ;
	}

__LEAVE_FUNCTION

	return FALSE ;
}

