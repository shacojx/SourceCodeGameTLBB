

#ifndef __GCRELATION_H__
#define __GCRELATION_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


class GCRelation : public Packet 
{
public:
	GCRelation( ){} ;
	virtual ~GCRelation( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream ) const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID() const { return PACKET_GC_RELATION ; }
	virtual UINT			GetPacketSize() const { return m_Relation.GetSize(); }
	
public:
	//使用数据接口
	GC_RELATION*			GetRelation( ){ return &m_Relation; }
	VOID					SetRelation( GC_RELATION* pRelation ){ m_Relation = *pRelation; }

private:
	//数据
	GC_RELATION				m_Relation;

};


class GCRelationFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCRelation() ; }
	PacketID_t	GetPacketID() const { return PACKET_GC_RELATION ; }
	UINT		GetPacketMaxSize() const { return sizeof(GC_RELATION); }
};


class GCRelationHandler 
{
public:
	static UINT Execute( GCRelation* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
