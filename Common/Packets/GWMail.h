

#ifndef __GWMAIL_H__
#define __GWMAIL_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GWMail : public Packet 
	{
	public:
		GWMail( ){} ;
		virtual ~GWMail( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GW_MAIL ; }
		virtual UINT			GetPacketSize()const 
		{ 
			return	GetMailSize(m_Mail) ;
		}

	public:
		//使用数据接口
		VOID		SetMail( MAIL* pMail ){ m_Mail = *pMail ; }
		MAIL*		GetMail( ){ return &m_Mail ; }

	private:
		//数据
		MAIL		m_Mail ;
		

	};


	class GWMailFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GWMail() ; }
		PacketID_t	GetPacketID()const { return PACKET_GW_MAIL ; }
		UINT		GetPacketMaxSize()const { return	sizeof(MAIL) ; }
	};


	class GWMailHandler 
	{
	public:
		static UINT Execute( GWMail* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
