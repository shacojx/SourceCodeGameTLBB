#ifndef _CG_DISCARD_EQUIP_H_
#define _CG_DISCARD_EQUIP_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class CGDiscardEquip:public Packet
	{

	public:
		CGDiscardEquip(){};
		virtual				~CGDiscardEquip(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_DISCARDEQUIP; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE);
		}

	public:

		VOID			SetEquipPoint(BYTE index){m_EquipPoint = index;}
		BYTE			GetEquipPoint(){return m_EquipPoint;}

	private:

		BYTE			m_EquipPoint;
	};


	class CGDiscardEquipFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGDiscardEquip(); }
		PacketID_t	GetPacketID() const { return PACKET_CG_DISCARDEQUIP; }
		UINT		GetPacketMaxSize() const { return sizeof(BYTE); }
	};


	class CGDiscardEquipHandler
	{
	public:
		static UINT Execute( CGDiscardEquip* pPacket, Player* pPlayer );
	};

}

using namespace Packets;

#endif