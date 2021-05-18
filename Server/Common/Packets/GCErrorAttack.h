

#ifndef __GCERRORATTACK_H__
#define __GCERRORATTACK_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


class GCErrorAttack : public Packet 
{
public:
	GCErrorAttack( ){} ;
	virtual ~GCErrorAttack( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_ERRORATTACK ; }
	virtual UINT			GetPacketSize()const { return	sizeof(BYTE) +
															sizeof(ObjID_t)*2; }
public:	
	//失败原因
	//见 GameDefine2.h ATTACK_ERROR

public:
	//使用数据接口
	BYTE			getReason(VOID) { return m_byReason; }
	VOID			setReason(BYTE byReason) { m_byReason = byReason; }

	ObjID_t			getAttackID(VOID) { return m_AttObjID; }
	VOID			setAttackID(ObjID_t idAttack) { m_AttObjID = idAttack; }

	ObjID_t			getTargetID(VOID) { return m_TarObjID; }
	VOID			setTargetID(ObjID_t idTarget) { m_TarObjID = idTarget; }

private:
	//数据
	BYTE			m_byReason;		//失败原因 enum ATTACK_ERROR
	ObjID_t			m_AttObjID;		//攻击方的ID
	ObjID_t			m_TarObjID;		//被攻击方的ID
};


class GCErrorAttackFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCErrorAttack() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_ERRORATTACK ; }
	UINT		GetPacketMaxSize()const { return	sizeof(BYTE) +
													sizeof(ObjID_t)*2; }
};


class GCErrorAttackHandler 
{
public:
	static UINT Execute( GCErrorAttack* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
