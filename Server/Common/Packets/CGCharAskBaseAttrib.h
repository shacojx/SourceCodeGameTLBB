

#ifndef __CGCHARASKBASEATTRIB_H__
#define __CGCHARASKBASEATTRIB_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


class CGCharAskBaseAttrib : public Packet 
{
public:
	CGCharAskBaseAttrib( ){} ;
	virtual ~CGCharAskBaseAttrib( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_CG_CHARASKBASEATTRIB ; }
	virtual UINT			GetPacketSize()const { return sizeof(ObjID_t) ; }
	
public:
	//使用数据接口
	ObjID_t					getTargetID(VOID) { return m_ObjID; }
	VOID					setTargetID(ObjID_t idTarget) { m_ObjID = idTarget; }

private:
	//数据
	ObjID_t					m_ObjID;	//对方的ObjID
};


class CGCharAskBaseAttribFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGCharAskBaseAttrib() ; }
	PacketID_t	GetPacketID()const { return PACKET_CG_CHARASKBASEATTRIB ; }
	UINT		GetPacketMaxSize()const { return sizeof(ObjID_t) ; }
};


class CGCharAskBaseAttribHandler 
{
public:
	static UINT Execute( CGCharAskBaseAttrib* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
