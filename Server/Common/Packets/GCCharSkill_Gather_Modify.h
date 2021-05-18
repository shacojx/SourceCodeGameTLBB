// GCCharSkill_Gather_Modify.h
// 
// 技能聚气时间的延长等等
// 
//////////////////////////////////////////////////////

#ifndef __GCCHARSKILL_GATHER_MODIFY_H__
#define __GCCHARSKILL_GATHER_MODIFY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class GCCharSkill_Gather_Modify : public Packet
{
public:
	GCCharSkill_Gather_Modify( ){}
	virtual ~GCCharSkill_Gather_Modify( ){}

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_CHARSKILL_GATHER_MODIFY ; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t) +
															sizeof(FLOAT); }

public:
	//使用数据接口
	VOID			setObjID(ObjID_t id) { m_ObjID = id; }
	ObjID_t			getObjID(VOID)const { return m_ObjID; }

	VOID			setSubTime(INT nSubTime) { m_nSubTime = nSubTime; }
	INT				getSubTime(VOID)const { return m_nSubTime; }

private:
	ObjID_t			m_ObjID;			// ObjID
	INT				m_nSubTime;			// 缩短的引导时间
};


class GCCharSkill_Gather_ModifyFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCCharSkill_Gather_Modify() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_CHARSKILL_GATHER_MODIFY; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t) +
													sizeof(FLOAT); }
};

class GCCharSkill_Gather_ModifyHandler 
{
public:
	static UINT Execute( GCCharSkill_Gather_Modify* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif