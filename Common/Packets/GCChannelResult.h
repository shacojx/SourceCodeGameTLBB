

#ifndef __GCCHANNELRESULT_H__
#define __GCCHANNELRESULT_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCChannelResult : public Packet 
	{
	public:
		GCChannelResult( ){} ;
		virtual ~GCChannelResult( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_CHANNELRESULT ; }
			virtual UINT			GetPacketSize()const { return	sizeof(BYTE)+
																	sizeof(GUID_t)+
																	sizeof(ChannelID_t); }

	public :

	public:
		//使用数据接口
		VOID				SetReturn( BYTE Return ){ m_Return = Return ; }
		BYTE				GetReturn( ){ return m_Return ; }

		VOID				SetGUID( GUID_t guid ){	m_GUID = guid ; }
		GUID_t				GetGUID( ){ return m_GUID ; }

		VOID				SetChannelID( ChannelID_t tid ){	m_ChannelID = tid ; }
		ChannelID_t			GetChannelID( ){ return m_ChannelID ; }

	private:
		//数据
		BYTE				m_Return ;
		GUID_t				m_GUID ;
		ChannelID_t			m_ChannelID ;

	};


	class GCChannelResultFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCChannelResult() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_CHANNELRESULT ; }
		UINT		GetPacketMaxSize()const { return	sizeof(BYTE)+
														sizeof(GUID_t)+
														sizeof(ChannelID_t); }
	};


	class GCChannelResultHandler 
	{
	public:
		static UINT Execute( GCChannelResult* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
