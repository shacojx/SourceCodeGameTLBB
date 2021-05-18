

#ifndef __WGMAIL_H__
#define __WGMAIL_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGMail : public Packet 
	{
	public:
		WGMail( ){} ;
		virtual ~WGMail( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_WG_MAIL ; }
		virtual UINT			GetPacketSize()const 
		{ 
			return	sizeof(PlayerID_t)+GetMailListSize(m_MailList)+sizeof(m_GUID) ;
		}

	public:
		//使用数据接口
		VOID		SetPlayerID( PlayerID_t pid ){ m_PlayerID = pid ; } ;
		PlayerID_t	GetPlayerID( ){ return m_PlayerID ; } ;

		VOID		SetMailList( MAIL_LIST* pMailList ){ m_MailList = *pMailList; }
		MAIL_LIST*	GetMailList( ){ return &m_MailList ; }

		VOID		SetGUID( GUID_t guid ){	m_GUID = guid ; }
		GUID_t		GetGUID( ){ return m_GUID ; }
	private:
		//数据
		PlayerID_t	m_PlayerID ;//收信人的PlayerID
		MAIL_LIST	m_MailList ;
		GUID_t		m_GUID ;

	};


	class WGMailFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new WGMail() ; }
		PacketID_t	GetPacketID()const { return PACKET_WG_MAIL ; }
		UINT		GetPacketMaxSize()const { return	sizeof(PlayerID_t)+
														sizeof(MAIL_LIST)+
														sizeof(GUID_t); }
	};


	class WGMailHandler 
	{
	public:
		static UINT Execute( WGMail* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
