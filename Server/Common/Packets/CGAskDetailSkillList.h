// CGAskDetailSkillList.h
// 
// 取当前主角的技能表
// 
//////////////////////////////////////////////////////

#ifndef __CGASKDETAILSKILLLIST_H__
#define __CGASKDETAILSKILLLIST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class CGAskDetailSkillList : public Packet
{
public:
	CGAskDetailSkillList( ){}
	virtual ~CGAskDetailSkillList( ){}

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_CG_ASKDETAILSKILLLIST ; }
	virtual UINT			GetPacketSize()const { return sizeof(ObjID_t) ; }
	
public:
	//使用数据接口
	ObjID_t					getTargetID(VOID) { return m_ObjID; }
	VOID					setTargetID(ObjID_t idTarget) { m_ObjID = idTarget; }

private:
	//数据
	ObjID_t					m_ObjID;	//对方的ObjID
};


class CGAskDetailSkillListFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new CGAskDetailSkillList() ; }
	PacketID_t	GetPacketID()const { return PACKET_CG_ASKDETAILSKILLLIST; }
	UINT		GetPacketMaxSize()const { return sizeof(ObjID_t) ; }
};

class CGAskDetailSkillListHandler 
{
public:
	static UINT Execute( CGAskDetailSkillList* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif