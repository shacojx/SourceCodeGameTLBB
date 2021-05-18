// CGAskDetailAttrib.h
// 
// 取角色的详细属性
// 
//////////////////////////////////////////////////////

#ifndef __CGASKDETAILATTRIB_H__
#define __CGASKDETAILATTRIB_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class CGAskDetailAttrib : public Packet
{
public:
	CGAskDetailAttrib( ){}
	virtual ~CGAskDetailAttrib( ){}

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_CG_ASKDETAILATTRIB ; }
	virtual UINT			GetPacketSize()const { return sizeof(ObjID_t) ; }
	
public:
	//使用数据接口
	ObjID_t					getTargetID(VOID) { return m_ObjID; }
	VOID					setTargetID(ObjID_t idTarget) { m_ObjID = idTarget; }

private:
	//数据
	ObjID_t					m_ObjID;	//对方的ObjID
};


class CGAskDetailAttribFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGAskDetailAttrib() ; }
	PacketID_t	GetPacketID()const { return PACKET_CG_ASKDETAILATTRIB; }
	UINT		GetPacketMaxSize()const { return sizeof(ObjID_t) ; }
};

class CGAskDetailAttribHandler 
{
public:
	static UINT Execute( CGAskDetailAttrib* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif