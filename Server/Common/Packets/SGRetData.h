

#ifndef __SGRETDATA_H__
#define __SGRETDATA_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

struct PLAYERDATA
{

};


namespace Packets
{


class SGRetData : public Packet 
{
public:
	SGRetData( ){} ;
	virtual ~SGRetData( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_SG_RETDATA ; }
	virtual UINT			GetPacketSize()const { return	sizeof(PLAYERDATA) ; }
	
public:
	//使用数据接口
	PLAYERDATA*				GetPlayerData(){ return &m_Data ; } ;

private:
	PLAYERDATA				m_Data ;

};


class SGRetDataFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new SGRetData() ; }
	PacketID_t	GetPacketID()const { return PACKET_SG_RETDATA ; }
	UINT		GetPacketMaxSize()const { return	sizeof(PLAYERDATA) ; }
};


class SGRetDataHandler 
{
public:
	static UINT Execute( SGRetData* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
