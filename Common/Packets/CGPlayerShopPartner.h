// CGPlayerShopPartner.h
// 
// 对合作伙伴的操作都放在这个消息中处理
// 
//////////////////////////////////////////////////////

#ifndef __CGPLAYERSHOPPARTNER_H__
#define __CGPLAYERSHOPPARTNER_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGPlayerShopPartner : public Packet
	{
	public:
		enum
		{
			OPT_NONE = 0,
			OPT_ADD_PARTNER,
			OPT_DEL_PARTNER,
		};
	public:
		CGPlayerShopPartner( )
		{
			m_Opt		 = OPT_NONE;			//操作类型
			m_PartnerID	 = INVALID_GUID;		//目标ID
		};
		virtual ~CGPlayerShopPartner( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_PLAYERSHOPPARTNER; }
		virtual UINT			GetPacketSize()const { return	sizeof(_PLAYERSHOP_GUID) + 
																sizeof(BYTE) + 
																sizeof(GUID_t);}

		_PLAYERSHOP_GUID		GetShopID(VOID) const {return m_ShopID;}
		VOID					SetShopID(_PLAYERSHOP_GUID nShopID) {m_ShopID = nShopID;}

		BYTE					GetOpt(){ return m_Opt;}
		VOID					SetOpt(BYTE bOpt){ m_Opt = bOpt;}

		GUID_t					GetGuid(){ return m_PartnerID;}
		VOID					SetGuid(GUID_t Guid){ m_PartnerID = Guid;}

	private:
		_PLAYERSHOP_GUID		m_ShopID;			//商店ID
		BYTE					m_Opt;				//操作类型
		GUID_t					m_PartnerID;		//目标ID

	};

	class CGPlayerShopPartnerFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGPlayerShopPartner() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_PLAYERSHOPPARTNER; };
		UINT		GetPacketMaxSize()const { return	sizeof(_PLAYERSHOP_GUID) + 
														sizeof(BYTE) + 
														sizeof(GUID_t);}
	};

	class CGPlayerShopPartnerHandler 
	{
	public:
		static UINT Execute( CGPlayerShopPartner* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
