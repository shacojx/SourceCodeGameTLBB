

#ifndef __GCMAIL_H__
#define __GCMAIL_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCMail : public Packet 
	{
	public:
		GCMail( ){} ;
		virtual ~GCMail( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_MAIL ; }
		virtual UINT			GetPacketSize()const 
		{ 
			return	GetMailListSize(m_MailList) ;
		}

	public:
		//使用数据接口
		VOID		SetMailList( MAIL_LIST* pMailList ){ m_MailList = *pMailList; }
		MAIL_LIST*	GetMailList( ){ return &m_MailList ; }

	private:
		//数据
		MAIL_LIST	m_MailList ;

	};


	class GCMailFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCMail() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_MAIL ; }
		UINT		GetPacketMaxSize()const { return	sizeof(MAIL_LIST) ; }
	};


	class GCMailHandler 
	{
	public:
		static UINT Execute( GCMail* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
