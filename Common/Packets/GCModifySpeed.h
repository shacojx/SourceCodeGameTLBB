
#ifndef __GCMODIFYSPEED_H__
#define __GCMODIFYSPEED_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class GCModifySpeed : public Packet 
	{
	public:
		GCModifySpeed( ){} ;
		virtual ~GCModifySpeed( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_MODIFYSPEED ; }
		virtual UINT			GetPacketSize()const { return sizeof( ObjID_t )
															+ sizeof( UINT ); }
		
		VOID					setObjID(ObjID_t id) { m_ObjID = id; }
		ObjID_t					getObjID(VOID)const { return m_ObjID; }

		VOID					setMoveSpeed(UINT uMoveSpeed) { m_uMoveSpeed =uMoveSpeed; }
		UINT					getMoveSpeed() { return m_uMoveSpeed; }

	private:
		ObjID_t m_ObjID;
		UINT	m_uMoveSpeed;
	};


	class GCModifySpeedFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCModifySpeed() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_MODIFYSPEED ; }
		UINT		GetPacketMaxSize()const { return sizeof( ObjID_t )
													+ sizeof( UINT ); }

	};


	class GCModifySpeedHandler 
	{
	public:
		static UINT Execute( GCModifySpeed* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
