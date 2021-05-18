#ifndef __CGHEARTBEAT_H__
#define __CGHEARTBEAT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
class CGHeartBeat : public Packet 
{
public:
	CGHeartBeat( ){} ;
	virtual ~CGHeartBeat( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_CG_HEARTBEAT ; }
	virtual UINT			GetPacketSize()const { return 0 ; }
	
public:
	//使用数据接口

private:
	//数据
};


class CGHeartBeatFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGHeartBeat() ; }
	PacketID_t	GetPacketID()const { return PACKET_CG_HEARTBEAT ; }
	UINT		GetPacketMaxSize()const { return 0 ; }
};


class CGHeartBeatHandler 
{
public:
	static UINT Execute( CGHeartBeat* pPacket, Player* pPlayer ) ;
};



};


using namespace Packets ;

#endif
