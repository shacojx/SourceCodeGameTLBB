// GCExchangeSynchConfirmII.h
// 
// 
// 
//////////////////////////////////////////////////////

#ifndef __GCEXCHANGESYNCHCONFIRMII_H__
#define __GCEXCHANGESYNCHCONFIRMII_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCExchangeSynchConfirmII : public Packet
	{
	public:
		GCExchangeSynchConfirmII( )
		{
			m_IsEnable = 0;
		}
		virtual ~GCExchangeSynchConfirmII( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_EXCHANGESYNCHCONFIRMII; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE);}

	public:
		BYTE					GetIsEnable(VOID) const {return m_IsEnable;};
		VOID					SetIsEnable(BYTE isEnable) {m_IsEnable = isEnable;};

	private:
		BYTE					m_IsEnable;		

	};

	class GCExchangeSynchConfirmIIFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCExchangeSynchConfirmII() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_EXCHANGESYNCHCONFIRMII; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE);};
	};

	class GCExchangeSynchConfirmIIHandler 
	{
	public:
		static UINT Execute( GCExchangeSynchConfirmII* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
