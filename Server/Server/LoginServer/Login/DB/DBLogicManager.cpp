#include "stdafx.h"

#include "DBLogicManager.h"
#include "PlayerPool.h"
#include "PacketFactoryManager.h"

DBLogicManager	g_DBLogicManager;

DBLogicManager::DBLogicManager()
{
	__ENTER_FUNCTION
		AutoLock_T autolock(m_Lock);
		m_QueSize	=	DB_QUEUE_SIZE;
		m_PacketQue	=	new ASYNC_PACKET[m_QueSize];
		AssertEx(m_PacketQue, "[DBLogicManager::DBLogicManager]:Allocate memory failed!!");
		m_Head		=   0;
		m_Tail		=	0;

	__LEAVE_FUNCTION
}

DBLogicManager::~DBLogicManager()
{
	__ENTER_FUNCTION
		AutoLock_T autolock(m_Lock);
		SAFE_DELETE_ARRAY(m_PacketQue);

	__LEAVE_FUNCTION
}

BOOL DBLogicManager::HeartBeat(UINT uTime)
{
	__ENTER_FUNCTION

	

	return ProcessCacheCommands();

	__LEAVE_FUNCTION

	return FALSE;
}

/*
 *  消息缓存的处理，这是DB 唯一的工作
 */
BOOL	DBLogicManager::ProcessCacheCommands()
{
	__ENTER_FUNCTION

	BOOL ret = FALSE ;

	for( uint i=0; i<m_QueSize; i++ )
	{
		Packet* pPacket = NULL ;
		PlayerID_t PlayerID ;
		uint Flag ;

		ret = RecvPacket( pPacket, PlayerID, Flag ) ;
		if( !ret )
			break ;

		Assert( pPacket ) ;

		if( Flag==PF_REMOVE )
		{
			g_pPacketFactoryManager->RemovePacket( pPacket ) ;
			continue ;
		}

		BOOL bNeedRemove = TRUE ;

		if( PlayerID==INVALID_ID )
		{
			//这种情况可能是World 查询数据库
			_MY_TRY
			{
				uint uret = pPacket->Execute(NULL);

				if( uret == PACKET_EXE_ERROR )
				{
				}
				else if( uret == PACKET_EXE_BREAK )
				{
				}
				else if( uret == PACKET_EXE_CONTINUE )
				{
				}
				else if( uret == PACKET_EXE_NOTREMOVE )
				{
					bNeedRemove = FALSE ;
				}
				else if( uret == PACKET_EXE_NOTREMOVE_ERROR )
				{
					bNeedRemove = FALSE ;
				}
			}
			_MY_CATCH
			{
			}
		}
		else
		{
			_MY_TRY
			{
				Player* pPlayer = g_pPlayerPool->GetPlayer( PlayerID ) ;
				Assert( pPlayer ) ;
				uint uret = pPacket->Execute(pPlayer) ;
				if( uret == PACKET_EXE_ERROR )
				{
					//DB 没有踢用户的权限
					MovePacket( PlayerID ) ;
				}
				else if( uret == PACKET_EXE_BREAK )
				{
				}
				else if( uret == PACKET_EXE_CONTINUE )
				{
				}
				else if( uret == PACKET_EXE_NOTREMOVE )
				{
					bNeedRemove = FALSE ;
				}
				else if( uret == PACKET_EXE_NOTREMOVE_ERROR )
				{
					bNeedRemove = FALSE ;
					MovePacket( PlayerID ) ;
				}
			}
			_MY_CATCH
			{
			}
		}

		//回收消息
		if( bNeedRemove )
			g_pPacketFactoryManager->RemovePacket( pPacket ) ;
	}



	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}

BOOL DBLogicManager::RecvPacket( Packet*& pPacket, PlayerID_t& PlayerID, uint& Flag )
{
	__ENTER_FUNCTION
	AutoLock_T autolock(m_Lock);

	if( m_PacketQue[m_Head].m_pPacket==NULL )
	{//缓冲区中没有消息
		return FALSE ;
	}

	pPacket = m_PacketQue[m_Head].m_pPacket ;
	PlayerID = m_PacketQue[m_Head].m_PlayerID ;
	Flag = m_PacketQue[m_Head].m_Flag ;

	m_PacketQue[m_Head].m_pPacket = NULL ;
	m_PacketQue[m_Head].m_PlayerID = INVALID_ID ;
	m_PacketQue[m_Head].m_Flag = PF_NONE ;

	m_Head ++ ;
	if( m_Head>=m_QueSize ) 
		m_Head = 0 ;

	return TRUE ;

	__LEAVE_FUNCTION
	
	return FALSE ;
}

BOOL DBLogicManager::MovePacket( PlayerID_t PlayerID )
{
	__ENTER_FUNCTION

	AutoLock_T autolock(m_Lock);

	uint Cur = m_Head ;

	for( uint i=0; i<m_QueSize; i++ )
	{
		if( m_PacketQue[Cur].m_pPacket == NULL )
			break ;

		if( m_PacketQue[Cur].m_PlayerID == PlayerID )
		{
			m_PacketQue[Cur].m_Flag = PF_REMOVE ;
		}

		Cur ++ ;
		if( Cur>=m_QueSize ) Cur = 0 ;
	}

	return TRUE ;

	__LEAVE_FUNCTION
	
	return FALSE ;
}

BOOL	DBLogicManager::SendPacket(Packet* pPacket, 
								   PlayerID_t PlayerID,
								   uint Flag/* =PF_NONE  */)
{
	__ENTER_FUNCTION

	AutoLock_T autolock(m_Lock);

	if( m_PacketQue[m_Tail].m_pPacket )
	{//缓冲区满
		return FALSE; //这里不能resize 因为DBLogic 处理消息能力有压力控制
		
	}

	m_PacketQue[m_Tail].m_pPacket = pPacket ;
	m_PacketQue[m_Tail].m_PlayerID = PlayerID ;
	m_PacketQue[m_Tail].m_Flag = Flag ;

	m_Tail ++ ;
	if( m_Tail>=m_QueSize ) 
		m_Tail = 0 ;

	return TRUE ;

	__LEAVE_FUNCTION

	return FALSE ;
}


