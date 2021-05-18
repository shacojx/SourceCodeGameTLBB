// CGAskJoinMenpai.h
// 
// 取当前主角的技能表
// 
//////////////////////////////////////////////////////

#ifndef __CGASKJOINMENPAI_H__
#define __CGASKJOINMENPAI_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class CGAskJoinMenpai : public Packet
{
public:
	CGAskJoinMenpai( ){}
	virtual ~CGAskJoinMenpai( ){}

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_CG_ASKJOINMENPAI ; }
	virtual UINT			GetPacketSize()const { return sizeof(TeamID_t) ; }
	
public:
	//使用数据接口
	TeamID_t				GetMenpaiID(VOID) { return m_MenpaiID; }
	VOID					SetMenpaiID(TeamID_t idMenpai) { m_MenpaiID = idMenpai; }

private:
	//数据
	TeamID_t				m_MenpaiID;	
};


class CGAskJoinMenpaiFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGAskJoinMenpai() ; }
	PacketID_t	GetPacketID()const { return PACKET_CG_ASKJOINMENPAI; }
	UINT		GetPacketMaxSize()const { return sizeof(TeamID_t) ; }
};

class CGAskJoinMenpaiHandler 
{
public:
	static UINT Execute( CGAskJoinMenpai* pPacket, Player* pPlayer ) ;
};

}

using namespace Packets;



#endif