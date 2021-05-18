

#ifndef __GW_FINGER_H__
#define __GW_FINGER_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{

class GWFinger : public Packet 
{
public:
	GWFinger( ){}
	virtual ~GWFinger( ){}

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream );
	virtual BOOL			Write( SocketOutputStream& oStream ) const;
	virtual UINT			Execute( Player* pPlayer );

	virtual PacketID_t		GetPacketID() const { return PACKET_GW_FINGER; }
	virtual UINT			GetPacketSize() const 
	{ 
		return m_Finger.GetSize(); 
	}

public:
	//使用数据接口
	GW_FINGER*				GetFinger( ){ return &m_Finger; }
	VOID					SetFinger( GW_FINGER* pFinger ){ m_Finger = *pFinger; };

private:
	//数据
	GW_FINGER				m_Finger;

};


class GWFingerFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GWFinger(); }
	PacketID_t	GetPacketID() const { return PACKET_GW_FINGER; }
	UINT		GetPacketMaxSize() const { return sizeof(GW_FINGER); }
};


class GWFingerHandler 
{
public:
	static UINT Execute( GWFinger* pPacket, Player* pPlayer );
};



};

using namespace Packets;

#endif // __GW_FINGER_H__
