#ifndef __GC_NEWPLAYER_INCLUDE__
#define __GC_NEWPLAYER_INCLUDE__


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
class GCNewPlayer : public Packet
{
public:
	GCNewPlayer( ){} ;
	virtual ~GCNewPlayer( ){} ;

	//公用继承接口
	virtual BOOL			Read( SocketInputStream& iStream ) ;
	virtual BOOL			Write( SocketOutputStream& oStream )const ;
	virtual UINT			Execute( Player* pPlayer ) ;

	virtual PacketID_t		GetPacketID()const { return PACKET_GC_NEWPLAYER ; }
	virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t) +
															sizeof(WORLD_POS) +
															sizeof(FLOAT)+
															sizeof(FLOAT)+
															sizeof(WORD); }

public:
	//使用数据接口
	ObjID_t				getObjID( )const{ return m_ObjID ; }
	VOID				setObjID( ObjID_t id ){ m_ObjID = id ; }

	WORLD_POS&			getWorldPos( ){ return m_posWorld ;}
	VOID				setWorldPos( const WORLD_POS& pos ){ m_posWorld = pos ; }

	FLOAT				getDir( )const{ return m_fDir; }
	VOID				setDir( FLOAT fDir ){ m_fDir = fDir; }

	WORD				getEquipVer( )const{ return m_wEquipVer ;}
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


class GCNewPlayerFactory : public PacketFactory 
{
public:
	Packet*		CreatePacket() { return new GCNewPlayer() ; }
	PacketID_t	GetPacketID()const { return PACKET_GC_NEWPLAYER; }
	UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t) +
													sizeof(WORLD_POS) +
													sizeof(FLOAT)+
													sizeof(FLOAT)+
													sizeof(WORD); }
};

class GCNewPlayerHandler 
{
public:
	static UINT Execute( GCNewPlayer* pPacket, Player* pPlayer ) ;
};




}

using namespace Packets;



#endif