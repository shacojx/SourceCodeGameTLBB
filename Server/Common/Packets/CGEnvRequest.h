#ifndef __CG_ENVIROMENTREQUEST_H__
#define __CG_ENVIROMENTREQUEST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{

class CGEnvRequest : public Packet
{
public:
	CGEnvRequest( ){} ;
	virtual ~CGEnvRequest( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_CG_ENVREQUEST ; }
	virtual UINT			GetPacketSize()const { return	0; }

public:
	//使用数据接口

private:
	//数据

};


class CGEnvRequestFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGEnvRequest() ; }
	PacketID_t	GetPacketID()const { return PACKET_CG_ENVREQUEST; }
	UINT		GetPacketMaxSize()const { return 0; }
};

class CGEnvRequestHandler 
{
public:
	static UINT Execute( CGEnvRequest* pPacket, Player* pPlayer ) ;
};

}

using namespace Packets;

#endif