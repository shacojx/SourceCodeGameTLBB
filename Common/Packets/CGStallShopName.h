// CGStallShopName.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __CGSTALLSHOPNAME_H__
#define __CGSTALLSHOPNAME_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "GCStallError.h"

namespace Packets
{
	class CGStallShopName : public Packet
	{
	public:
		CGStallShopName( )
		{
			m_StallNameSize = 0;
			memset(m_StallName, 0, MAX_STALL_NAME);
		}
		virtual ~CGStallShopName( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_STALLSHOPNAME; }
		virtual UINT			GetPacketSize()const { return sizeof(BYTE) + sizeof(CHAR)*m_StallNameSize;}

		VOID					SetStallNameSize( BYTE bSize ){ m_StallNameSize = bSize ; } ;
		BYTE					GetStallNameSize( ){ return m_StallNameSize ; } ;

		VOID					SetStallName( CHAR* pStallName ){
			strncpy( m_StallName, pStallName, MAX_STALL_NAME-1 ) ;
		};
		CHAR*					GetStallName(){ return (CHAR*)m_StallName ; } ;


	private:
		BYTE					m_StallNameSize;
		CHAR					m_StallName[MAX_STALL_NAME];
	};

	class CGStallShopNameFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGStallShopName() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_STALLSHOPNAME; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE) + sizeof(CHAR)*MAX_STALL_NAME;};
	};

	class CGStallShopNameHandler 
	{
	public:
		static UINT Execute( CGStallShopName* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
