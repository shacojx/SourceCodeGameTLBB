#include "stdafx.h"

#include "TurnPlayerQueue.h"
#include "DB_Struct.h"

WorldPlayerCounter	g_WorldPlayerCounter;

TurnPlayerQueue*	g_pProcessPlayerQueue = NULL;

WorldPlayerQueue*	g_pWorldPlayerQueue = NULL;

TurnPlayerQueue::TurnPlayerQueue()
{
	__ENTER_FUNCTION
	
	m_Size				=	MAX_TURN_PLAYER;
	m_pTurnPlayerQueue  =	new TURN_PLAYER_INFO[m_Size];
	m_Head	= 0;
    m_Tail	= 0;

	__LEAVE_FUNCTION
}

TurnPlayerQueue::~TurnPlayerQueue()
{
	__ENTER_FUNCTION

		SAFE_DELETE_ARRAY(m_pTurnPlayerQueue);

	__LEAVE_FUNCTION
}

BOOL TurnPlayerQueue::Init()
{
	return TRUE;
}

BOOL TurnPlayerQueue::AddInPlayer(PlayerID_t pID,const CHAR* Name,UINT& QueuePos)
{

	__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	if( m_pTurnPlayerQueue[m_Tail].Used)
	{//队列满
		return FALSE;
	}

	m_pTurnPlayerQueue[m_Tail].Used	= TRUE;
	m_pTurnPlayerQueue[m_Tail].PlayerID		= pID ;
	
	strncpy(m_pTurnPlayerQueue[m_Tail].PlayerName,Name,MAX_ACCOUNT);
	m_pTurnPlayerQueue[m_Tail].PlayerName[MAX_ACCOUNT] = '\0';
	m_pTurnPlayerQueue[m_Tail].QueuePos		= m_Tail;
	
	QueuePos = m_Tail;

	m_Tail ++ ;
	if( m_Tail>=m_Size ) 
		m_Tail = 0 ;

	return TRUE;
	
	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	TurnPlayerQueue::GetOutPlayer(PlayerID_t& pID,CHAR* NameBuff)
{
	__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	if( m_pTurnPlayerQueue[m_Head].Used== FALSE )
	{//没有队列玩家
		return FALSE ;
	}
	
	Assert(NameBuff);
	strncpy(NameBuff,
			m_pTurnPlayerQueue[m_Head].PlayerName,
			MAX_ACCOUNT);
	NameBuff[MAX_ACCOUNT] = '\0';

	pID = m_pTurnPlayerQueue[m_Head].PlayerID ;
	
	m_pTurnPlayerQueue[m_Head].Used = FALSE;
	
	m_Head ++ ;
	if( m_Head>=m_Size ) 
		m_Head = 0 ;


	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}


WorldPlayerQueue::WorldPlayerQueue()
{
	__ENTER_FUNCTION

	m_Size				=	MAX_WORLDTURN_PLAYER;
	m_pWorldPlayerQueue  =	new WORLD_PLAYER_INFO[m_Size];
	
	m_Head	= 0;
	m_Tail	= 0;

	__LEAVE_FUNCTION
}

WorldPlayerQueue::~WorldPlayerQueue()
{
	__ENTER_FUNCTION

		SAFE_DELETE_ARRAY(m_pWorldPlayerQueue);

	__LEAVE_FUNCTION
}

BOOL WorldPlayerQueue::Init()
{
	return TRUE;
}

BOOL WorldPlayerQueue::AddInPlayer(PlayerID_t pID,const CHAR* Name,GUID_t	guid,BYTE age,UINT& QueuePos)
{

	__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	if( m_pWorldPlayerQueue[m_Tail].Used)
	{//队列满
		return FALSE;
	}

	m_pWorldPlayerQueue[m_Tail].Used	= TRUE;
	m_pWorldPlayerQueue[m_Tail].PlayerID		= pID ;

	strncpy(m_pWorldPlayerQueue[m_Tail].PlayerName,Name,MAX_ACCOUNT);
	m_pWorldPlayerQueue[m_Tail].PlayerName[MAX_ACCOUNT] = '\0';
	m_pWorldPlayerQueue[m_Tail].QueuePos		= m_Tail;
	m_pWorldPlayerQueue[m_Tail].Guid			= guid;
	m_pWorldPlayerQueue[m_Tail].Age				= age;


	QueuePos = m_Tail;

	m_Tail ++ ;
	if( m_Tail>=m_Size ) 
		m_Tail = 0 ;

	return TRUE;

	__LEAVE_FUNCTION

	return FALSE;
}

BOOL	WorldPlayerQueue::GetOutPlayer(UINT QueuePos)
{
	__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	if( m_pWorldPlayerQueue[m_Head].Used== FALSE )
	{//没有队列玩家
		return FALSE ;
	}
	Assert(QueuePos == m_Head);

	m_pWorldPlayerQueue[m_Head].Used = FALSE;

	m_Head ++ ;
	if( m_Head>=m_Size ) 
		m_Head = 0 ;

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}
BOOL	WorldPlayerQueue::FindHeadPlayer(UINT& QueuePos)
{
	__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	if( m_pWorldPlayerQueue[m_Head].Used== FALSE )
	{//没有队列玩家
		return FALSE ;
	}
	QueuePos = m_Head;

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

WORLD_PLAYER_INFO&		WorldPlayerQueue::GetPlayer(UINT QueuePos)
{
	return m_pWorldPlayerQueue[QueuePos];
}

