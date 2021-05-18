

#ifndef __WGSystemMsg_H__
#define __WGSystemMsg_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{


	class WGSystemMsg : public Packet 
	{
	public:
		WGSystemMsg( ) { m_ContexSize=MAX_SYSTEM_MSG_SIZE-1; };
		virtual ~WGSystemMsg( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_WG_SYSTEMMSG ; }
		virtual UINT			GetPacketSize() const {  return	sizeof(GUID_t)+sizeof(BYTE)*2+sizeof(CHAR)*m_ContexSize ; }



	public:
		VOID					SetGUID( GUID_t guid ){	m_GUID = guid ; }
		GUID_t					GetGUID( ){ return m_GUID ; }

		VOID					SetContex( CHAR* pContex ){memset(m_Contex, 0,m_ContexSize); memcpy(m_Contex,pContex,m_ContexSize); };
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


	class WGSystemMsgFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new WGSystemMsg() ; }
		PacketID_t	GetPacketID() const { return PACKET_WG_SYSTEMMSG ; }
		UINT		GetPacketMaxSize() const { return sizeof(GUID_t)+sizeof(CHAR)*MAX_SYSTEM_MSG_SIZE+sizeof(BYTE)*2;}
	};


	class WGSystemMsgHandler 
	{
	public:
		static UINT Execute( WGSystemMsg* pPacket,Player* pPlayer) ;
	};



};

using namespace Packets ;

#endif
