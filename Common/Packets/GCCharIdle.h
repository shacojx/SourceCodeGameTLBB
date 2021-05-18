#ifndef __GCCHARIDLE_H__
#define __GCCHARIDLE_H__


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class GCCharIdle : public Packet
{
public:
	GCCharIdle( ){ m_bMustStop = FALSE; } ;
	virtual ~GCCharIdle( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_CHARIDLE ; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t)+
															sizeof(WORLD_POS)+
															sizeof(BOOL); }

public:
	//使用数据接口
	VOID				setObjID(ObjID_t id) { m_ObjID = id; }
	ObjID_t				getObjID(VOID)const { return m_ObjID; }

	VOID				setWorldPos(const WORLD_POS& pos) { m_posWorld = pos; }
	const WORLD_POS&	getWorldPos(VOID)const { return m_posWorld; }

	VOID				setMustStop(BOOL bValue) { m_bMustStop = bValue; }
	BOOL				getMustStop(VOID) const { return m_bMustStop; }

private:
	//数据
	ObjID_t			m_ObjID;	//ID
	WORLD_POS		m_posWorld;	//坐标
	BOOL			m_bMustStop; //是否强行停下

};


class GCCharIdleFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCCharIdle() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_CHARIDLE; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t)+
													sizeof(WORLD_POS)+
													sizeof(BOOL); }
};

class GCCharIdleHandler 
{
public:
	static UINT Execute( GCCharIdle* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif