// GCNewPlatform.h

#ifndef __GC_NEWPLATFORM_H__
#define __GC_NEWPLATFORM_H__


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class GCNewPlatform : public Packet
{
public:
	GCNewPlatform( ){} ;
	virtual ~GCNewPlatform( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_NEWPLATFORM; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t) +
															sizeof(WORLD_POS) +
															sizeof(FLOAT) +
															sizeof(BYTE); }

public:
	//使用数据接口
	VOID					setObjID(ObjID_t id) { m_ObjID = id; }
	ObjID_t					getObjID(VOID)const { return m_ObjID; }

	VOID					setWorldPos(const WORLD_POS& pos) { m_posWorld = pos; }
	const WORLD_POS&		getWorldPos(VOID)const { return m_posWorld; }

	VOID					setDir(FLOAT dir) { m_Dir = dir; }
	FLOAT					getDir() { return m_Dir; }

	VOID					setType(BYTE type) { m_Type = type ; }
	BYTE					getType() { return m_Type ; }


private:
	ObjID_t					m_ObjID;		// ObjID
	WORLD_POS				m_posWorld;		// 位置
	FLOAT					m_Dir;			// 方向
	BYTE					m_Type;
};


class GCNewPlatformFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCNewPlatform() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_NEWPLATFORM; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t) +
													sizeof(WORLD_POS) +
													sizeof(FLOAT) +
													sizeof(BYTE) ; }
};

class GCNewPlatformHandler 
{
public:
	static UINT Execute( GCNewPlatform* pPacket, Player* pPlayer ) ;
};

}

using namespace Packets;

#endif // __GC_NEWPLATFORM_H__