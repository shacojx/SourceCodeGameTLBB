

#ifndef __GCTEST_H__
#define __GCTEST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

#define MAX_SAY_SIZE 64
struct _PGCTEST
{
	SceneID_t	SceneID ;
	CHAR		szSay[MAX_SAY_SIZE] ;
	PlayerID_t	PlayerID ;
	SOCKET		s ;
	UINT		Status ;
	ID_t		PlayerManagerID ;

	_PGCTEST()
	{
		SceneID = INVALID_ID ;
		memset( szSay, 0, MAX_SAY_SIZE ) ;
		PlayerID = INVALID_ID ;
		s = INVALID_SOCKET ;
		Status = PS_SERVER_EMPTY ;
		PlayerManagerID = INVALID_ID ;
	};
};


namespace Packets
{


class GCTest : public Packet 
{
public:
	GCTest( ){} ;
	virtual ~GCTest( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_TEST ; }
	virtual UINT			GetPacketSize()const { return	sizeof(UINT)+
															sizeof(_PGCTEST); }
	
public:
	//使用数据接口
	UINT					GetType( ){ return m_Type ; } ;
	VOID					SetType( UINT type ){ m_Type = type ; } ;
	_PGCTEST*				GetTest( ){ return &m_Test ; } ;
	VOID					SetTest( _PGCTEST* test ){ m_Test = *test ; } ;

	//类型
	enum {
		TYPE_GC_NONE = 0 ,
		TYPE_GC_CONNECTOK ,
		TYPE_GC_SAY ,
		TYPE_GC_ENTER ,
		TYPE_GC_CHANGE ,
	};

private:
	//数据
	UINT		m_Type ;
	_PGCTEST	m_Test ;

};


class GCTestFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCTest() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_TEST ; }
	UINT		GetPacketMaxSize()const { return	sizeof(UINT)+
													sizeof(_PGCTEST); }
};


class GCTestHandler 
{
public:
	static UINT Execute( GCTest* pPacket, Player* pPlayer ) ;
};


};

using namespace Packets ;



#endif
