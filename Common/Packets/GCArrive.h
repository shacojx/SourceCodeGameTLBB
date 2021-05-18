#ifndef __GCARRIVE_H__
#define __GCARRIVE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


class GCArrive : public Packet 
{
public:
	GCArrive( ){} ;
	virtual ~GCArrive( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_ARRIVE ; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t)+
		sizeof( INT )+
															sizeof(WORLD_POS); }

public:
	//使用数据接口
	VOID				setObjID(ObjID_t id) { m_ObjID = id; }
	ObjID_t				getObjID(VOID)const { return m_ObjID; }

	VOID				setHandleID( INT nID ) { m_nHandleID = nID; }
	INT					getHandleID( VOID )const{ return m_nHandleID; }

	VOID				setWorldPos(const WORLD_POS& pos) { m_posWorld = pos; }
	const WORLD_POS&	getWorldPos(VOID)const { return m_posWorld; }

private:
	//数据
	ObjID_t			m_ObjID;	//ID
	INT				m_nHandleID;
	WORLD_POS		m_posWorld;	//坐标
};


class GCArriveFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCArrive() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_ARRIVE ; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t)+
		sizeof( INT )+
													sizeof(WORLD_POS);}
};


class GCArriveHandler 
{
public:
	static UINT Execute( GCArrive* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
