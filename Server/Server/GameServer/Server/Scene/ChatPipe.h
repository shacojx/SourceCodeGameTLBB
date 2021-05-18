
#ifndef __CHATPIPE_H__
#define __CHATPIPE_H__

#include "Type.h"

class Scene ;
class Packet ;
class Obj ;


#define CHAT_ITEM_SIZE 1024
#define COUNTS_PER_TICK 128

struct CHAT_ITEM
{
	Packet*				m_pPacket ;
	ObjID_t				m_SourObjID ;//普通聊天时说话者
	ObjID_t				m_DestObjID ;//队聊、场景聊、私聊、系统、自建聊天频道时目的OBJ

	CHAT_ITEM( )
	{ 
		CleanUp( ) ; 
	} ;

	VOID CleanUp( )
	{ 
		m_pPacket = NULL ; 
		m_SourObjID = INVALID_ID ;
		m_DestObjID = INVALID_ID ;
	} ;
};


class ChatPipe
{
public :
	ChatPipe( ) ;
	~ChatPipe( ) ;


	VOID				CleanUp( ) ;
	BOOL				Init( Scene* pScene ) ;

	BOOL				HeartBeat( UINT uTime ) ;



	Scene*				GetScene( ){ return m_pScene ; } ;

	//向聊天管道内发送消息,支持异步调用
	BOOL				SendPacket( Packet* pPacket, ObjID_t SourID, ObjID_t DestID ) ;
	//从聊天管道内取出消息,支持异步调用
	BOOL				RecvPacket( Packet*& pPacket, ObjID_t* pSourID, ObjID_t* pDestID ) ;


public :
	INT					m_nValidCount ;//可发送的消息数量
	Scene*				m_pScene ;
	CHAT_ITEM*			m_pChatItems ;
	INT					m_nHead ;
	INT					m_nTail ;

	MyLock				m_Lock ;


};


#endif
