// CGCharJump.h
// 
// 跳
// 
//////////////////////////////////////////////////////

#ifndef __CGCHARJUMP_H__
#define __CGCHARJUMP_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class CGCharJump : public Packet
{
public:
	CGCharJump( ){}
	virtual ~CGCharJump( ){}

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_CG_CHARJUMP ; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t); }

public:
	//使用数据接口
	VOID			setObjID(ObjID_t id) { m_ObjID = id; }
	ObjID_t			getObjID(VOID)const { return m_ObjID; }

private:
	ObjID_t			m_ObjID;			// ObjID
};


class CGCharJumpFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGCharJump() ; }
	PacketID_t	GetPacketID()const { return PACKET_CG_CHARJUMP; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t); }
};

class CGCharJumpHandler 
{
public:
	static UINT Execute( CGCharJump* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif