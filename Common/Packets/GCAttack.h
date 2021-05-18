

#ifndef __GCATTACK_H__
#define __GCATTACK_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{

#define MAX_DAM_LIST_NUM	(128)

class GCAttack : public Packet 
{
public:
	GCAttack( ){} ;
	virtual ~GCAttack( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_ATTACK ; }
	virtual UINT			GetPacketSize()const { return	sizeof(UINT) +
															sizeof(BYTE) +
															sizeof(_DAMAGE_INFO)*m_byListNum; }
	
public:
	struct _DAMAGE_INFO
	{
		ObjID_t m_idTarget;		//目标对象的ID
		WORD 	m_wDamage;		//该目标因本次攻击损失的HP
		WORD 	m_wLeftHP;		//该目标当前的HP
	};

public:
	//使用数据接口
	UINT					getExp(VOID) { return m_uExp; }
	VOID					setExp(UINT uExp) { m_uExp =uExp; }

	BYTE					getListNum(VOID) { return m_byListNum;  }
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
	UINT					m_uExp;						//自己目前的经验值
	BYTE					m_byListNum;					//受到攻击的对象的个数[0, 128]
	_DAMAGE_INFO				m_listDam[MAX_DAM_LIST_NUM];	//受到攻击的对象列表
};


class GCAttackFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCAttack() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_ATTACK ; }
	UINT		GetPacketMaxSize()const { return	sizeof(UINT) +
													sizeof(BYTE) +
													sizeof(GCAttack::_DAMAGE_INFO)*MAX_DAM_LIST_NUM; }
};


class GCAttackHandler 
{
public:
	static UINT Execute( GCAttack* pPacket, Player* pPlayer ) ;
};



};

using namespace Packets ;

#endif
