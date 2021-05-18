#ifndef _CG_USE_EQUIP_H_
#define _CG_USE_EQUIP_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"



namespace Packets
{
	class CGUseEquip:public Packet
	{

	public:
		CGUseEquip(){};
		virtual				~CGUseEquip(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_USEEQUIP; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE);
		}

	public:
		VOID			setBagIndex(BYTE index){m_BagIndex = index;}
		BYTE			getBagIndex(){return m_BagIndex;}


	private:
		
		BYTE			m_BagIndex;		//使用Bag中的位置存放的位置

	};


	class CGUseEquipFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGUseEquip(); }
		PacketID_t	GetPacketID() const { return PACKET_CG_USEEQUIP; }
		UINT		GetPacketMaxSize() const { return	sizeof(BYTE); }
	};


	class CGUseEquipHandler
	{
	public:
		static UINT	Execute( CGUseEquip* pPacket, Player* pPlayer );
	};



}


#endif