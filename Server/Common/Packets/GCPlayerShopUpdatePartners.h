// GCPlayerShopUpdatePartners.h
// 
// 通知客户端金钱存取
// 
//////////////////////////////////////////////////////

#ifndef __GCPLAYERSHOPUPDATEPARTNERS_H__
#define __GCPLAYERSHOPUPDATEPARTNERS_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCPlayerShopUpdatePartners : public Packet
	{
	public:
		struct MsgPartner_t
		{
			GUID_t		m_Guid;
			CHAR		m_szName[MAX_CHARACTER_NAME];
			VOID		CleanUp()
			{
				m_Guid = INVALID_GUID;
				memset(m_szName, 0, MAX_CHARACTER_NAME);
			}
		};
	public:
		GCPlayerShopUpdatePartners( )
		{
			m_PartnerNum	=	0;		
			for(INT i = 0; i<MAX_PARTNER_PER_SHOP; i++)
			{
				m_PartnerList[i].CleanUp();
			}
		};
		virtual ~GCPlayerShopUpdatePartners( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_PLAYERSHOPUPDATEPARTNERS; }
		virtual UINT			GetPacketSize()const { return	sizeof(_PLAYERSHOP_GUID) +
																sizeof(BYTE) +
																m_PartnerNum*sizeof(MsgPartner_t);}

		_PLAYERSHOP_GUID		GetShopID(VOID) {return m_ShopID;}
		VOID					SetShopID(_PLAYERSHOP_GUID nShopID) {m_ShopID = nShopID;}

		BYTE					GetPartnerNum(VOID) {return m_PartnerNum;}
		VOID					SetPartnerNum(BYTE nPartNum) {m_PartnerNum = nPartNum;}

		MsgPartner_t*			GetPartnerList(VOID) {return m_PartnerList;}
		VOID					SetPartnerList(MsgPartner_t* pPartnerList)
		{
			memcpy(m_PartnerList, pPartnerList, m_PartnerNum*sizeof(MsgPartner_t));
		}
	private:
		_PLAYERSHOP_GUID		m_ShopID;									//商店ID
		BYTE					m_PartnerNum;								//
		MsgPartner_t			m_PartnerList[MAX_PARTNER_PER_SHOP];		//
	};

	class GCPlayerShopUpdatePartnersFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCPlayerShopUpdatePartners() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_PLAYERSHOPUPDATEPARTNERS; };
		UINT		GetPacketMaxSize()const { return	sizeof(_PLAYERSHOP_GUID) +
														sizeof(BYTE) +
														MAX_PARTNER_PER_SHOP*sizeof(GCPlayerShopUpdatePartners::MsgPartner_t);}
	};

	class GCPlayerShopUpdatePartnersHandler 
	{
	public:
		static UINT Execute( GCPlayerShopUpdatePartners* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
