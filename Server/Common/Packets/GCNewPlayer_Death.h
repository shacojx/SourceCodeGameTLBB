#ifndef __GCNEWPLAYER_DEATH_H__
#define __GCNewPlayer_Death_H__


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class GCNewPlayer_Death : public Packet
{
public:
	GCNewPlayer_Death( ){} ;
	virtual ~GCNewPlayer_Death( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_NEWPLAYER_DEATH; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t) +
															sizeof(WORLD_POS) +
															sizeof(FLOAT)+
															sizeof(FLOAT)+
															sizeof(WORD); }

public:
	//使用数据接口
	VOID				setObjID(ObjID_t id) { m_ObjID = id; }
	ObjID_t				getObjID(VOID)const { return m_ObjID; }

	VOID				setWorldPos(const WORLD_POS& pos) { m_posWorld = pos; }
	const WORLD_POS&	getWorldPos(VOID)const { return m_posWorld; }

	FLOAT				getDir( )const{ return m_fDir; }
	VOID				setDir( FLOAT fDir ){ m_fDir = fDir; }

	WORD				getEquipVer( ){ return m_wEquipVer ;}
	VOID				setEquipVer( WORD ver ){ m_wEquipVer = ver ; }

	VOID				setMoveSpeed( FLOAT fMoveSpeed ){ m_fMoveSpeed = fMoveSpeed; }
	FLOAT				getMoveSpeed( VOID )const{ return m_fMoveSpeed; }

private:
	ObjID_t			m_ObjID;		// ObjID
	WORLD_POS		m_posWorld;		// 位置
	FLOAT			m_fDir;			// 方向
	WORD			m_wEquipVer;	// 玩家的装备版本号
	FLOAT			m_fMoveSpeed;	// 移动速度
};


class GCNewPlayer_DeathFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCNewPlayer_Death() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_NEWPLAYER_DEATH; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t) +
													sizeof(WORLD_POS) +
													sizeof(FLOAT)+
													sizeof(FLOAT)+
													sizeof(WORD); }
};

class GCNewPlayer_DeathHandler 
{
public:
	static UINT Execute( GCNewPlayer_Death* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif