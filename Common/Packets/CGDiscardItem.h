#ifndef _CG_DISCARD_ITEM_H_
#define _CG_DISCARD_ITEM_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class CGDiscardItem:public Packet
	{
	public:
		enum
		{
			FromBag = 0,
			FromBank,
		};

	public:
		CGDiscardItem()
		{
			m_Opt	=	FromBag;
			m_BagIndex	=	0;
		};
		virtual				~CGDiscardItem(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_DISCARDITEM; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)*2;
		}

	public:

		VOID			SetBagIndex(BYTE index){m_BagIndex = index;}
		BYTE			GetBagIndex(){return m_BagIndex;}

		VOID			SetOpt(BYTE Opt){m_Opt = Opt;}
		BYTE			GetOpt(){return m_Opt;}

	private:
		BYTE			m_Opt;
		BYTE			m_BagIndex;
	};


	class CGDiscardItemFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGDiscardItem(); }
		PacketID_t	GetPacketID() const { return PACKET_CG_DISCARDITEM; }
		UINT		GetPacketMaxSize() const { return sizeof(BYTE)*2; }
	};


	class CGDiscardItemHandler
	{
	public:
		static UINT	Execute( CGDiscardItem* pPacket, Player* pPlayer );
	};

}

using namespace Packets;

#endif