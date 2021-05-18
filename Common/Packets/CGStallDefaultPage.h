// CGBankMoney.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __CGSTALLDEFAULTPAGE_H__
#define __CGSTALLDEFAULTPAGE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGStallDefaultPage : public Packet
	{
	public:
		enum
		{
			SAVE_MONEY = 0,
			PUTOUT_MONEY,
			UPDATE_MONEY,
		};
	public:
		CGStallDefaultPage( )
		{
			m_DefaultPage		= 0;//0:Item,2:Pet
		};
		virtual ~CGStallDefaultPage( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_STALL_DEFAULT_PAGE; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE);}

	public:

		BYTE					GetDefaultPage(VOID) const {return m_DefaultPage;};
		VOID					SetDefaultPage(BYTE DefaultPage) {m_DefaultPage = DefaultPage;};

	private:
		BYTE					m_DefaultPage;
	};

	class CGStallDefaultPageFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGStallDefaultPage() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_STALL_DEFAULT_PAGE; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE);};
	};

	class CGStallDefaultPageHandler 
	{
	public:
		static UINT Execute( CGStallDefaultPage* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
