// GCExchangeSynchLock.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __GCEXCHANGESYNCHLOCK_H__
#define __GCEXCHANGESYNCHLOCK_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCExchangeSynchLock : public Packet
	{
	public:
		GCExchangeSynchLock( )
		{
			m_IsBoth = 0;
			m_IsMyself = 0;
			m_IsLocked = 0;
		};
		virtual ~GCExchangeSynchLock( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_EXCHANGESYNCHLOCK; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE)*3;}

	public:
		BYTE					GetIsMyself(VOID) const {return m_IsMyself;};
		VOID					SetIsMyself(BYTE IsMyself) {m_IsMyself = IsMyself;};

		BYTE					GetIsLocked(VOID) const {return m_IsLocked;};
		VOID					SetIsLocked(BYTE Islocked) {m_IsLocked = Islocked;};

		BYTE					GetIsBoth(VOID) const {return m_IsBoth;};
		VOID					SetIsBoth(BYTE IsBoth) {m_IsBoth = IsBoth;};

	private:

		BYTE					m_IsBoth;
		BYTE					m_IsMyself;		
		BYTE					m_IsLocked;		
	};

	class GCExchangeSynchLockFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCExchangeSynchLock() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_EXCHANGESYNCHLOCK; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)*3;};
	};

	class GCExchangeSynchLockHandler 
	{
	public:
		static UINT Execute( GCExchangeSynchLock* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
