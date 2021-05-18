#ifndef __CG_FINGER_H__
#define __CG_FINGER_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GameStruct_Finger.h"

namespace Packets
{

class CGFinger : public Packet 
{
public:
	CGFinger( ){}
	virtual ~CGFinger( ){}

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream );
	virtual BOOL			Write( SocketOutputStream& oStream ) const;
	virtual UINT			Execute( Player* pPlayer );

	virtual PacketID_t		GetPacketID() const { return PACKET_CG_FINGER; }
	virtual UINT			GetPacketSize() const 
	{ 
		return m_Finger.GetSize(); 
	}

public:
	//使用数据接口
	CG_FINGER*				GetFinger( ){ return &m_Finger; }
	VOID					SetFinger( CG_FINGER* pFinger ){ m_Finger = *pFinger; };

private:
	//数据
	CG_FINGER				m_Finger;

};


class CGFingerFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGFinger(); }
	PacketID_t	GetPacketID() const { return PACKET_CG_FINGER; }
	UINT		GetPacketMaxSize() const { return sizeof(CG_FINGER); }
};


class CGFingerHandler 
{
public:
	static UINT Execute( CGFinger* pPacket, Player* pPlayer );
};



};

using namespace Packets;

#endif // __CG_FINGER_H__
