

#ifndef __WGCHANNELRESULT_H__
#define __WGCHANNELRESULT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGChannelResult : public Packet 
	{
	public:
		WGChannelResult( ){} ;
		virtual ~WGChannelResult( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_WG_CHANNELRESULT ; }
			virtual UINT			GetPacketSize()const { return	sizeof(PlayerID_t)+
																	sizeof(BYTE)+
																	sizeof(GUID_t)+
																	sizeof(ChannelID_t); }

	public :

	public:
		//使用数据接口
		VOID				SetPlayerID( PlayerID_t pid ){ m_PlayerID = pid ; } ;
		PlayerID_t			GetPlayerID( ){ return m_PlayerID ; } ;

		VOID				SetReturn( BYTE Return ){ m_Return = Return ; }
		BYTE				GetReturn( ){ return m_Return ; }

		VOID				SetGUID( GUID_t guid ){	m_GUID = guid ; }
		GUID_t				GetGUID( ){ return m_GUID ; }

		VOID				SetChannelID( ChannelID_t tid ){	m_ChannelID = tid ; }
		ChannelID_t			GetChannelID( ){ return m_ChannelID ; }

	private:
		//数据
		PlayerID_t			m_PlayerID ;	//连接者
		BYTE				m_Return ;
		GUID_t				m_GUID ;
		ChannelID_t			m_ChannelID ;

	};


	class WGChannelResultFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new WGChannelResult() ; }
		PacketID_t	GetPacketID()const { return PACKET_WG_CHANNELRESULT ; }
		UINT		GetPacketMaxSize()const { return	sizeof(PlayerID_t)+
														sizeof(BYTE)+
														sizeof(GUID_t)+
														sizeof(ChannelID_t); }
	};


	class WGChannelResultHandler 
	{
	public:
		static UINT Execute( WGChannelResult* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
