

#ifndef __GCOTHERATTACK_H__
#define __GCOTHERATTACK_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GCAttack.h"


namespace Packets
{

class GCOtherAttack : public Packet 
{
public:
	GCOtherAttack( ){} ;
	virtual ~GCOtherAttack( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_OTHERATTACK ; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t) +
															sizeof(BYTE) +
															sizeof(_DAMAGE_INFO)*m_byListNum; }
	
public:
	
	typedef GCAttack::_DAMAGE_INFO _DAMAGE_INFO;
	

public:
	//使用数据接口
	ObjID_t					getAttackID(VOID) { return m_ObjID; }
	VOID					setAttackID(ObjID_t idAttack) { m_ObjID = idAttack; }

	BYTE					getListNum(VOID) { return m_byListNum; }
	VOID					setListNum(BYTE byListNum) { m_byListNum = byListNum; }

	_DAMAGE_INFO*				getDamList(VOID) { return m_listDam; }
	VOID					setDamList(_DAMAGE_INFO* pSour, BYTE byListNum)
	{
		if(pSour == NULL) return;
		m_byListNum = byListNum > MAX_DAM_LIST_NUM ? MAX_DAM_LIST_NUM : byListNum;

		memcpy(m_listDam, pSour, sizeof(_DAMAGE_INFO)*m_byListNum);
	}

private:
	//数据
	ObjID_t					m_ObjID;						//攻击方的ObjID
	BYTE					m_byListNum;					//受到攻击的对象的个数[0, 128]
	_DAMAGE_INFO				m_listDam[MAX_DAM_LIST_NUM];	//受到攻击的对象列表

};


class GCOtherAttackFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCOtherAttack() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_OTHERATTACK ; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t) +
													sizeof(BYTE) +
													sizeof(GCOtherAttack::_DAMAGE_INFO)*MAX_DAM_LIST_NUM; }
};


class GCOtherAttackHandler 
{
public:
	static UINT Execute( GCOtherAttack* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
