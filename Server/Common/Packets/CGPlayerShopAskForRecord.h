// CGPlayerShopAskForRecord.h
// 
// 向服务器申请创建玩家商店
// 
//////////////////////////////////////////////////////

#ifndef __CGPLAYERSHOPASKFORRECORD_H__
#define __CGPLAYERSHOPASKFORRECORD_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GCPlayerShopError.h"
namespace Packets
{
	class CGPlayerShopAskForRecord : public Packet
	{
	public:
		enum
		{
			TYPE_EXCHANGE_RECORD = 0,
			TYPE_MANAGER_RECORD,
		};
	public:
		CGPlayerShopAskForRecord( )
		{
			m_ShopID.Reset();
			m_bType = 0;
			m_bPage = 0;
		}
		virtual ~CGPlayerShopAskForRecord( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_PLAYERSHOPASKFORRECORD; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE)*2+sizeof(_PLAYERSHOP_GUID);}

		BYTE					GetType(){return m_bType;}
		VOID					SetType(BYTE bType){m_bType = bType;}

		BYTE					GetPage(){return m_bPage;}
		VOID					SetPage(BYTE bPage){m_bPage = bPage;}

		_PLAYERSHOP_GUID		GetShopID(VOID) const {return m_ShopID;}
		VOID					SetShopID(_PLAYERSHOP_GUID nShopID) {m_ShopID = nShopID;}

	private:
		_PLAYERSHOP_GUID		m_ShopID;			//商店ID
		BYTE					m_bType;
		BYTE					m_bPage;
	};

	class CGPlayerShopAskForRecordFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGPlayerShopAskForRecord() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_PLAYERSHOPASKFORRECORD; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)*2+sizeof(_PLAYERSHOP_GUID);};
	};

	class CGPlayerShopAskForRecordHandler 
	{
	public:
		static UINT Execute( CGPlayerShopAskForRecord* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
