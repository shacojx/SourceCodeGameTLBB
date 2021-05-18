

#ifndef __GWASKMAIL_H__
#define __GWASKMAIL_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "CGAskMail.h"


namespace Packets
{
	class GWAskMail : public Packet 
	{
	public:
		GWAskMail( ){} ;
		virtual ~GWAskMail( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GW_ASKMAIL ; }
		virtual UINT			GetPacketSize()const { return	sizeof(GUID_t)+
																sizeof(BYTE) ; }

	public :

	public:
		//使用数据接口
		VOID				SetGUID( GUID_t guid ){	m_GUID = guid ; }
		GUID_t				GetGUID( ){ return m_GUID ; }

		VOID					SetAskType( BYTE type ){ m_AskType = type ; }
		BYTE					GetAskType( ){ return m_AskType ; }


	private:
		//数据
		GUID_t				m_GUID ;
		BYTE				m_AskType ;// enum ASK_TYPE

	};


	class GWAskMailFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GWAskMail() ; }
		PacketID_t	GetPacketID()const { return PACKET_GW_ASKMAIL ; }
		UINT		GetPacketMaxSize()const { return sizeof(GUID_t)+
													  sizeof(BYTE) ; }
	};


	class GWAskMailHandler 
	{
	public:
		static UINT Execute( GWAskMail* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
