

#ifndef __GW_SYSTEMMSG_H__
#define __GW_SYSTEMMSG_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"



namespace Packets
{
	class GWSystemMsg : public Packet 
	{
	public:
		enum
		{
			SYSTEMMSG_TYPE_PLAYER,
			SYSTEMMSG_TYPE_GM,
			SYSTEMMSG_TYPE_SCRIPT,
			SYSTEMMSG_TYPE_AUTO,
			SYSTEMMSG_TYPE_TEST,
		};
	public:
		GWSystemMsg( ) { m_ContexSize=MAX_SYSTEM_MSG_SIZE-1; };
		virtual ~GWSystemMsg( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GW_SYSTEMMSG ; }
		virtual UINT			GetPacketSize() const {  return	sizeof(GUID_t)+sizeof(BYTE)*2+sizeof(CHAR)*m_ContexSize ; }

	

	public:
		VOID					SetGUID( GUID_t guid ){	m_GUID = guid ; }
		GUID_t					GetGUID( ){ return m_GUID ; }

		VOID					SetContex( const CHAR* pContex ){memset(m_Contex, 0,m_ContexSize); memcpy(m_Contex,pContex,m_ContexSize); };
		CHAR*					GetContex(){ m_Contex[m_ContexSize] = 0; return m_Contex ; } ;

		VOID					SetContexSize( BYTE bSize ){ m_ContexSize = bSize ; } ;
		BYTE					GetContexSize( ){ return m_ContexSize ; } ;

		VOID					SetMessageType( BYTE bSize ){ m_MessageType = bSize ; } ;
		BYTE					GetMessageType( ){ return m_MessageType ; } ;

	private:
		CHAR	m_Contex[MAX_SYSTEM_MSG_SIZE];
		BYTE	m_ContexSize;
		BYTE	m_MessageType;
		GUID_t	m_GUID;

	};


	class GWSystemMsgFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GWSystemMsg() ; }
		PacketID_t	GetPacketID() const { return PACKET_GW_SYSTEMMSG ; }
		UINT		GetPacketMaxSize() const { return sizeof(GUID_t)+sizeof(CHAR)*MAX_SYSTEM_MSG_SIZE+sizeof(BYTE)*2;}
	};


	class GWSystemMsgHandler 
	{
	public:
		static UINT Execute( GWSystemMsg* pPacket,Player* pPlayer) ;
	};
}

using namespace Packets ;

#endif
