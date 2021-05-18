#ifndef __GC_NEWITEM_INCLUDE__
#define __GC_NEWITEM_INCLUDE__


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class GCNewItem : public Packet
{
public:
	GCNewItem( ){} ;
	virtual ~GCNewItem( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_NEWITEM ; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t)*2 +
															sizeof(UINT) +
															sizeof(WORLD_POS); }

public:
	//使用数据接口
	VOID			setObjID(ObjID_t id) { m_ObjID = id; }
	ObjID_t			getObjID(VOID)const { return m_ObjID; }

	VOID			setAmount(UINT am) { m_uAmount = am; }
	UINT			getAmount(VOID)const { return m_uAmount; }

	VOID			setOwner(GUID_t id) { m_idOwner = id; }
	GUID_t			getOwner(VOID)const { return m_idOwner; }

	VOID				setWorldPos(WORLD_POS& pos) { m_posWorld = pos; }
	const WORLD_POS&	getWorldPos(VOID)const { return m_posWorld; }

private:
	//数据

	ObjID_t			m_ObjID;		//物品的ObjID
	UINT			m_uAmount;		//物品的数值或者其他属性
	GUID_t			m_idOwner;		//物品主人的GUID_t
	WORLD_POS		m_posWorld;		//物品的位置

};


class GCNewItemFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCNewItem() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_NEWITEM; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t)*2 +
													sizeof(UINT) + 
													sizeof(WORLD_POS); }
};

class GCNewItemHandler 
{
public:
	static UINT Execute( GCNewItem* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif