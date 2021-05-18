

#ifndef __WG_FINGER_H__
#define __WG_FINGER_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{

class WGFinger : public Packet 
{
public:
	WGFinger( ){}
	virtual ~WGFinger( ){}

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream );
	virtual BOOL			Write( SocketOutputStream& oStream ) const;
	virtual UINT			Execute( Player* pPlayer );

	virtual PacketID_t		GetPacketID() const { return PACKET_WG_FINGER; }
	virtual UINT			GetPacketSize() const 
	{ 
		return m_Finger.GetSize() + sizeof(m_GUID); 
	}

public:
	//使用数据接口
	WG_FINGER*				GetFinger( ){ return &m_Finger; }
	VOID					SetFinger( WG_FINGER* pFinger ){ m_Finger = *pFinger; };

	VOID					SetGUID(GUID_t guid) { m_GUID = guid; }
	GUID_t					GetGUID() { return m_GUID; }

private:
	//数据
	WG_FINGER				m_Finger;

	GUID_t					m_GUID;

};


class WGFingerFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new WGFinger(); }
	PacketID_t	GetPacketID() const { return PACKET_WG_FINGER; }
	UINT		GetPacketMaxSize() const { return sizeof(WG_FINGER) + sizeof(GUID_t); }
};


class WGFingerHandler 
{
public:
	static UINT Execute( WGFinger* pPacket, Player* pPlayer );
};



};

using namespace Packets;

#endif // __WG_FINGER_H__
