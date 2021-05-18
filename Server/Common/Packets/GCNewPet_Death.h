// GCNewPet_Death.h
//
///////////////////////////////////////////////////////

#ifndef __GCNEWPET_DEATH_H__
#define __GCNEWPET_DEATH_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCNewPet_Death : public Packet
	{
	public:
		GCNewPet_Death( ){} ;
		virtual ~GCNewPet_Death( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_NEWPET_DEATH; }
		virtual UINT			GetPacketSize()const
		{
			return	sizeof(ObjID_t) + sizeof(FLOAT) + sizeof(WORLD_POS) + sizeof(FLOAT);
		}

	public:
		//使用数据接口
		VOID				setObjID(ObjID_t id) { m_ObjID = id; }
		ObjID_t				getObjID(VOID)const { return m_ObjID; }

		VOID				setWorldPos(const WORLD_POS& pos) { m_posWorld = pos; }
		const WORLD_POS&	getWorldPos(VOID)const { return m_posWorld; }

		FLOAT				getDir( )const{ return m_fDir; }
		VOID				setDir( FLOAT fDir ){ m_fDir = fDir; }

		VOID				setMoveSpeed( FLOAT fMoveSpeed ){ m_fMoveSpeed = fMoveSpeed; }
		FLOAT				getMoveSpeed( VOID )const{ return m_fMoveSpeed; }

	private:
		ObjID_t			m_ObjID;		// ObjID
		WORLD_POS		m_posWorld;		// 位置
		FLOAT			m_fDir;			// 方向
		FLOAT			m_fMoveSpeed;	// 移动速度
	};


	class GCNewPet_DeathFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCNewPet_Death() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_NEWPET_DEATH; }
		UINT		GetPacketMaxSize()const
		{
			return	sizeof(ObjID_t) + sizeof(FLOAT) + sizeof(WORLD_POS) + sizeof(FLOAT);
		}
	};

	class GCNewPet_DeathHandler 
	{
	public:
		static UINT Execute( GCNewPet_Death* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
