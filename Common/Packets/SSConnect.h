

#ifndef __SSCONNECT_H__
#define __SSCONNECT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


class SSConnect : public Packet 
{
public:
	SSConnect( ){} ;
	virtual ~SSConnect( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream ) const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID() const { return PACKET_SS_CONNECT ; }
	virtual UINT			GetPacketSize() const { return sizeof(ID_t) ; }
	
public:
	//使用数据接口
	ID_t			GetServerID( ){ return m_ServerID ; } ;
	VOID			SetServerID( ID_t id ){ m_ServerID = id ; } ;

private:
	//数据
	ID_t			m_ServerID ;	//发送消息的服务器端程序的ServerID

};


class SSConnectFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new SSConnect() ; }
	PacketID_t	GetPacketID() const { return PACKET_SS_CONNECT ; }
	UINT		GetPacketMaxSize() const { return sizeof(ID_t) ; }
};


class SSConnectHandler 
{
public:
	static UINT Execute( SSConnect* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
