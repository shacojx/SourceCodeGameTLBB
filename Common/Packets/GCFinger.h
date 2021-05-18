

#ifndef __GC_FINGER_H__
#define __GC_FINGER_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{

class GCFinger : public Packet 
{
public:
	GCFinger( ){}
	virtual ~GCFinger( ){}

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream );
	virtual BOOL			Write( SocketOutputStream& oStream ) const;
	virtual UINT			Execute( Player* pPlayer );

	virtual PacketID_t		GetPacketID() const { return PACKET_GC_FINGER; }
	virtual UINT			GetPacketSize() const 
	{ 
		return m_Finger.GetSize(); 
	}

public:
	//使用数据接口
	GC_FINGER*				GetFinger( ){ return &m_Finger; }
	VOID					SetFinger( GC_FINGER* pFinger ){ m_Finger = *pFinger; };

private:
	//数据
	GC_FINGER				m_Finger;

};


class GCFingerFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCFinger(); }
	PacketID_t	GetPacketID() const { return PACKET_GC_FINGER; }
	UINT		GetPacketMaxSize() const { return sizeof(GC_FINGER); }
};


class GCFingerHandler 
{
public:
	static UINT Execute( GCFinger* pPacket, Player* pPlayer );
};



};

using namespace Packets;

#endif // __GC_FINGER_H__
