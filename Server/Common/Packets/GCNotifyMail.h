

#ifndef __GCNOTIFYMAIL_H__
#define __GCNOTIFYMAIL_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCNotifyMail : public Packet 
	{
	public:
		GCNotifyMail( ){} ;
		virtual ~GCNotifyMail( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_NOTIFYMAIL ; }
		virtual UINT			GetPacketSize()const { return sizeof(BYTE) ; }

	public :

	public:
		//使用数据接口
		VOID		SetMailCount( BYTE bCount ){ m_MailCount = bCount ; } ;
		BYTE		GetMailCount( ){ return m_MailCount ; } ;

	private:
		//数据
		BYTE		m_MailCount ;

	};


	class GCNotifyMailFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCNotifyMail() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_NOTIFYMAIL ; }
		UINT		GetPacketMaxSize()const { return sizeof(BYTE) ; }
	};


	class GCNotifyMailHandler 
	{
	public:
		static UINT Execute( GCNotifyMail* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
