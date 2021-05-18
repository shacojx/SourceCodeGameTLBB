#include "stdafx.h"

#include "MailPool.h"
#include "Config.h"
#include "DB_Struct.h"
MailNodePool* g_pMailNodePool = NULL ;
UserNodePool* g_pUserNodePool = NULL ;

/////////////////////////////////////////////////////////////////////////////////
//
MailNodePool::MailNodePool( )
{
__ENTER_FUNCTION

	m_nMailNodeNumber = 0 ;
	m_nCurrentOffset = 0 ;

	m_pMailNode = NULL ;

__LEAVE_FUNCTION
}

MailNodePool::~MailNodePool( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

__LEAVE_FUNCTION
}

BOOL MailNodePool::Init( UINT MaxMailNode )
{
__ENTER_FUNCTION

	m_nMailNodeNumber = MaxMailNode ;
	m_nCurrentOffset = 0 ;

	m_pMailNode = new MailNode[m_nMailNodeNumber] ;
	if( m_pMailNode == NULL )
		return FALSE ;
	
	_WORLD_INFO& WorldInfo = g_Config.m_WorldInfo;
	if(WorldInfo.m_EnableShareMem)
	{
		BOOL bOffSet = FALSE;
		for( INT i=0; i<m_nMailNodeNumber; i++ )
		{
			if(m_pMailNode[i].m_Mail.m_GUID!=INVALID_INDEX)
			{
				m_pMailNode[i].m_nPoolIndex=i ;
			}
			else if(bOffSet==FALSE)
			{
				m_nCurrentOffset = i;
				bOffSet = TRUE;
			}	
		}
	}
	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID MailNodePool::CleanUp( )
{
__ENTER_FUNCTION

	m_nMailNodeNumber = 0 ;
	m_nCurrentOffset = 0 ;

	SAFE_DELETE_ARRAY( m_pMailNode ) ;


__LEAVE_FUNCTION
}

MailNode* MailNodePool::NewNode( )
{
__ENTER_FUNCTION

	for( INT i=m_nCurrentOffset; i<m_nMailNodeNumber; i++ )
	{
		if( m_pMailNode[i].m_nPoolIndex==INVALID_INDEX )
		{
			m_nCurrentOffset ++ ;
			m_pMailNode[i].m_nPoolIndex=i ;
			return &(m_pMailNode[i]) ;
		}
	}

	return NULL ;

__LEAVE_FUNCTION

	return NULL ;
}

MailNode*	MailNodePool::GetNode(UINT Index)
{
	__ENTER_FUNCTION	

		return &(m_pMailNode[Index]);

	__LEAVE_FUNCTION

	return NULL;
}

VOID MailNodePool::DeleteNode( MailNode* pNode )
{
__ENTER_FUNCTION

	m_nCurrentOffset = pNode->m_nPoolIndex ;
	pNode->CleanUp( ) ;

__LEAVE_FUNCTION
}



/////////////////////////////////////////////////////////////////////////////////
//
UserNodePool::UserNodePool( )
{
__ENTER_FUNCTION

	m_nUserNodeNumber = 0 ;
	m_nCurrentOffset = 0 ;
	m_pUserNode = NULL ;

__LEAVE_FUNCTION
}

UserNodePool::~UserNodePool( )
{
__ENTER_FUNCTION

	CleanUp( ) ;

__LEAVE_FUNCTION
}

BOOL UserNodePool::Init( )
{
__ENTER_FUNCTION

	
	m_nUserNodeNumber = MAX_USERNODE_SIZE ;
	m_nCurrentOffset = 0 ;

	m_pUserNode = new UserNode[m_nUserNodeNumber] ;
	if( m_pUserNode == NULL )
		return FALSE ;

	return TRUE ;

__LEAVE_FUNCTION

	return FALSE ;
}

VOID UserNodePool::CleanUp( )
{
__ENTER_FUNCTION

	m_nUserNodeNumber = 0 ;
	m_nCurrentOffset = 0 ;

	SAFE_DELETE_ARRAY( m_pUserNode ) ;

__LEAVE_FUNCTION
}

UserNode* UserNodePool::NewNode( )
{
__ENTER_FUNCTION

	for( INT i=m_nCurrentOffset; i<m_nUserNodeNumber; i++ )
	{
		if( m_pUserNode[i].m_nPoolIndex==INVALID_INDEX )
		{
			m_nCurrentOffset ++ ;
			m_pUserNode[i].m_nPoolIndex=i ;
			return &(m_pUserNode[i]) ;
		}
	}

	return NULL ;

__LEAVE_FUNCTION

	return NULL ;
}

VOID UserNodePool::DeleteNode( UserNode* pNode )
{
__ENTER_FUNCTION

	m_nCurrentOffset = pNode->m_nPoolIndex ;
	pNode->CleanUp( ) ;

__LEAVE_FUNCTION
}

