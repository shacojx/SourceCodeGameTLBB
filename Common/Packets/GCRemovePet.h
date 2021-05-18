// GCRemovePet.h
// 
// 删除一个宠物
// 
//////////////////////////////////////////////////////

#ifndef __GCREMOVEPET_H__
#define __GCREMOVEPET_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"


namespace Packets
{
	class GCRemovePet :
		public Packet
	{
	public:
		GCRemovePet( ){
		}
		virtual ~GCRemovePet( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_REMOVEPET; }
		virtual UINT			GetPacketSize()const 
		{
			UINT uSize = sizeof(PET_GUID_t);
			return uSize;
		}

	public:
		PET_GUID_t				GetGUID( VOID )const{ return m_GUID; }
		VOID					SetGUID( PET_GUID_t GUID ){ m_GUID = GUID; }

	private:
		PET_GUID_t		m_GUID;							// ID
	};

	class GCRemovePetFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCRemovePet() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_REMOVEPET; }
		UINT		GetPacketMaxSize()const
		{
			UINT uSize = sizeof(GCRemovePet) - sizeof(Packet) ;
			return uSize;
		}
	};

	class GCRemovePetHandler 
	{
	public:
		static UINT Execute( GCRemovePet* pPacket, Player* pPlayer ) ;
	};
}

#endif // __GCREMOVEPET_H__
