


#ifndef __GCDIE_H__
#define __GCDIE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


class GCDie : public Packet 
{
public:
	GCDie( ){} ;
	virtual ~GCDie( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_DIE ; }
	virtual UINT			GetPacketSize()const { return sizeof(ObjID_t)*2 ; }
	

public:
	//使用数据接口
	VOID			setObjID(ObjID_t id) { m_ObjID = id; }
	ObjID_t			getObjID(VOID)const { return m_ObjID; }

	VOID			setCorpseID(ObjID_t id) { m_CorpseID = id; }
	ObjID_t			getCorpseID(VOID)const { return m_CorpseID; }


private:
	//数据
	ObjID_t		m_ObjID;		//死亡对象ObjectID
	ObjID_t		m_CorpseID;		//尸体ObjectID

};


class GCDieFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCDie() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_DIE ; }
	UINT		GetPacketMaxSize()const { return 	sizeof(ObjID_t)*2 ; }
};


class GCDieHandler 
{
public:
	static UINT Execute( GCDie* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
