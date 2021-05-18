#ifndef __CG_OTHEREQUIPMENT_H__
#define __CG_OTHEREQUIPMENT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{

class CGOtherEquip : public Packet
{
public:
	CGOtherEquip( ){} ;
	virtual ~CGOtherEquip( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_CG_OTHEREQUIPMENT ; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t); } 
	
public:
	//使用数据接口
	VOID			setObjID(ObjID_t id) { m_ObjID = id; }
	ObjID_t			getObjID(VOID)const { return m_ObjID; }

private:
	//数据
	ObjID_t		m_ObjID;		//对方玩家ObjectID
};

class CGOtherEquipFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGOtherEquip() ; }
	PacketID_t	GetPacketID()const { return PACKET_CG_OTHEREQUIPMENT ; }
	UINT		GetPacketMaxSize()const { return sizeof(ObjID_t); } 
};

class CGOtherEquipHandler 
{
public:
	static UINT Execute( CGOtherEquip* pPacket, Player* pPlayer ) ;
};

}

using namespace Packets;

#endif