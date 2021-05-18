// GCNewSpecial.h

#ifndef __GC_NEWSPECIAL_H__
#define __GC_NEWSPECIAL_H__


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class GCNewSpecial : public Packet
{
public:
	GCNewSpecial( ){} ;
	virtual ~GCNewSpecial( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_NEWSPECIAL; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t) +
															sizeof(WORLD_POS) +
															sizeof(FLOAT) +
															sizeof(INT); }

public:
	//使用数据接口
	VOID					setObjID(ObjID_t id) { m_ObjID = id; }
	ObjID_t					getObjID(VOID)const { return m_ObjID; }

	VOID					setWorldPos(const WORLD_POS& pos) { m_posWorld = pos; }
	const WORLD_POS&		getWorldPos(VOID)const { return m_posWorld; }

	VOID					setDir(FLOAT dir) { m_Dir = dir; }
	FLOAT					getDir() { return m_Dir; }

	VOID					setDataID( INT nDataID ){ m_nDataID = nDataID; }
	INT						getDataID( VOID )const{ return m_nDataID; }

private:
	ObjID_t					m_ObjID;		// ObjID
	WORLD_POS				m_posWorld;		// 位置
	FLOAT					m_Dir;			// 方向
	INT						m_nDataID;
};


class GCNewSpecialFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCNewSpecial() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_NEWSPECIAL; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t) +
													sizeof(WORLD_POS) +
													sizeof(FLOAT) +
													sizeof(INT) ; }
};

class GCNewSpecialHandler 
{
public:
	static UINT Execute( GCNewSpecial* pPacket, Player* pPlayer ) ;
};

}

using namespace Packets;

#endif // __GC_NEWSPECIAL_H__