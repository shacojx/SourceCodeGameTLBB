

#ifndef __WGNOTIFYMAIL_H__
#define __WGNOTIFYMAIL_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGNotifyMail : public Packet 
	{
	public:
		WGNotifyMail( ){} ;
		virtual ~WGNotifyMail( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_WG_NOTIFYMAIL ; }
		virtual UINT			GetPacketSize()const { return	sizeof(PlayerID_t)+
																sizeof(BYTE)+
																sizeof(m_GUID); }

	public :

	public:
		//使用数据接口
		VOID		SetPlayerID( PlayerID_t pid ){ m_PlayerID = pid ; } ;
		PlayerID_t	GetPlayerID( ){ return m_PlayerID ; } ;

		VOID		SetMailCount( BYTE bCount ){ m_MailCount = bCount ; } ;
		BYTE		GetMailCount( ){ return m_MailCount ; } ;

		VOID		SetGUID(GUID_t guid) { m_GUID = guid; }
		GUID_t		GetGUID() { return m_GUID; }

	private:
		//数据
		PlayerID_t	m_PlayerID ;
		BYTE		m_MailCount ;
		GUID_t		m_GUID;


	};


	class WGNotifyMailFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new WGNotifyMail() ; }
		PacketID_t	GetPacketID()const { return PACKET_WG_NOTIFYMAIL ; }
		UINT		GetPacketMaxSize()const { return	sizeof(PlayerID_t)+
														sizeof(BYTE) +
														sizeof(GUID_t); }
	};


	class WGNotifyMailHandler 
	{
	public:
		static UINT Execute( WGNotifyMail* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
