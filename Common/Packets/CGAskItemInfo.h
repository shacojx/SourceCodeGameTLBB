#ifndef _CG_ASK_ITEM_INFO_H_
#define _CG_ASK_ITEM_INFO_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"



namespace Packets
{
	class CGAskItemInfo : public Packet
	{

	public:
		CGAskItemInfo(){};
		virtual				~CGAskItemInfo(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_ASKITEMINFO; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(WORD);
		}

	public:

		VOID			setID(WORD id)			{ m_BagIndex = id;	}
		WORD			getID(VOID)				{ return m_BagIndex;}

	private:
		
		WORD			m_BagIndex;		//item 在包中的位置

	};


	class CGAskItemInfoFactory : public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGAskItemInfo(); }
		PacketID_t	GetPacketID() const { return PACKET_CG_ASKITEMINFO; }
		UINT		GetPacketMaxSize() const { return	sizeof(WORD); }
	};


	class CGAskItemInfoHandler
	{
	public:
		static UINT	Execute( CGAskItemInfo* pPacket, Player* pPlayer );
	};



}

using namespace Packets;


#endif