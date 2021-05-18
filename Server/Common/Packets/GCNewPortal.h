#ifndef __GC_NEWPORTAL_INCLUDE__
#define __GC_NEWPORTAL_INCLUDE__


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class GCNewPortal : public Packet
{
public:
	GCNewPortal( ){} ;
	virtual ~GCNewPortal( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_NEWPORTAL ; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t) +
															sizeof(WORLD_POS)+
															sizeof(BYTE) ; }

public:
	//使用数据接口
	VOID			setObjID(ObjID_t id) { m_ObjID = id; }
	ObjID_t			getObjID(VOID)const { return m_ObjID; }

	VOID				setWorldPos(WORLD_POS& pos) { m_posWorld = pos; }
	const WORLD_POS&	getWorldPos(VOID)const { return m_posWorld; }

	VOID			setType(BYTE type){ m_Type= type ; } ;
	BYTE			getType(){ return m_Type ; } ;


private:
	ObjID_t			m_ObjID;		// ObjID
	WORLD_POS		m_posWorld;		// 位置
	BYTE			m_Type ;
};


class GCNewPortalFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCNewPortal() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_NEWPORTAL; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t) +
													sizeof(WORLD_POS)+
													sizeof(BYTE) ; }
};

class GCNewPortalHandler 
{
public:
	static UINT Execute( GCNewPortal* pPacket, Player* pPlayer ) ;
};


}

using namespace Packets;



#endif