// GCJoinMenpai.h
// 
// 取当前主角的技能表
// 
//////////////////////////////////////////////////////

#ifndef __GCJOINMENPAI_H__
#define __GCJOINMENPAI_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

#define		JOINMENPAI_SUCCEED			1
#define		JOINMENPAI_NO_SUCCEED		0

namespace Packets
{
class GCJoinMenpai : public Packet
{
public:
	GCJoinMenpai( ){}
	virtual ~GCJoinMenpai( ){}

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_JOINMENPAI ; }
	virtual UINT			GetPacketSize()const { return sizeof(short) ; }
	
public:
	//使用数据接口
	short					GetIsSucceedJoin(VOID) { return m_SucceedJoin; }
	VOID					SetIsSucceedJoin(short bSucceedJoin) { m_SucceedJoin = bSucceedJoin; }

private:
	//数据
	short					m_SucceedJoin;	
};


class GCJoinMenpaiFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCJoinMenpai() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_JOINMENPAI; }
	UINT		GetPacketMaxSize()const { return sizeof(short) ; }
};

class GCJoinMenpaiHandler 
{
public:
	static UINT Execute( GCJoinMenpai* pPacket, Player* pPlayer ) ;
};

}

using namespace Packets;



#endif