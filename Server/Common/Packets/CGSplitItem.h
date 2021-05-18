// CGSplitItem.h
// 
// 拆分物品
// 
//////////////////////////////////////////////////////

#ifndef __CGSPLITITEM_H__
#define __CGSPLITITEM_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGSplitItem : public Packet
	{
	public:
		enum
		{
			BANK_CON = 0,
			BAG_CON,
		};
		CGSplitItem(){};

		virtual ~CGSplitItem(){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_SPLITITEM ; }
		virtual UINT			GetPacketSize()const { return	sizeof(INT) + sizeof(INT) + sizeof(INT);}
	public:
		INT						GetContainer(VOID) const {return m_nContainer;};
		VOID					SetContainer(INT nContainer) {m_nContainer = nContainer;};

		INT						GetPosition(VOID) const {return m_nPosition;};
		VOID					SetPosition(INT nPosition) {m_nPosition = nPosition;};

		INT						GetNumber(VOID) const {return m_nNumber;}
		VOID					SetNumber(INT nNumber)  {m_nNumber = nNumber;}
	private:
		INT						m_nContainer;
		INT						m_nPosition;
		INT						m_nNumber;
	};

	class CGSplitItemFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGSplitItem() ; };
		PacketID_t	GetPacketID()const { return PACKET_CG_SPLITITEM; };
		UINT		GetPacketMaxSize()const { return sizeof(INT) + sizeof(INT) + sizeof(INT);};
	};

	class CGSplitItemHandler 
	{
	public:
		static UINT Execute( CGSplitItem* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
