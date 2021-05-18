// CGLockTarget.h
// 
// 玩家锁定目标
// 
//////////////////////////////////////////////////////

#ifndef __CGLOCKTARGET_H__
#define __CGLOCKTARGET_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
class CGLockTarget : public Packet
{
public:
	CGLockTarget( ):m_nTargetID(INVALID_ID)	{};
	virtual ~CGLockTarget( ){};

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_CG_LOCK_TARGET; }
	virtual UINT			GetPacketSize()const { return	sizeof(m_nTargetID);}

public:
	ObjID_t			getTargetID(VOID) const {return m_nTargetID;};
	VOID			setTargetID(ObjID_t const nID) {m_nTargetID = nID;};
private:
	ObjID_t m_nTargetID;
};


class CGLockTargetFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGLockTarget() ; }
	PacketID_t	GetPacketID()const { return PACKET_CG_LOCK_TARGET; };
	UINT		GetPacketMaxSize()const { return sizeof(ObjID_t);};
};

class CGLockTargetHandler 
{
public:
	static UINT Execute( CGLockTarget* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif



