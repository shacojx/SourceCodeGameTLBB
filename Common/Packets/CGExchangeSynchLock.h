// CGExchangeSynchLock.h
// 
// 
// 
//////////////////////////////////////////////////////

#ifndef __CGEXCHANGESYNCHLOCK_H__
#define __CGEXCHANGESYNCHLOCK_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGExchangeSynchLock : public Packet
	{
	public:
		CGExchangeSynchLock( )
		{
			m_LockMyself = 0;
		}
		virtual ~CGExchangeSynchLock( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_EXCHANGESYNCHLOCK; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE);}

	public:
		BYTE					GetLockMyself(VOID) const {return m_LockMyself;};
		VOID					SetLockMyself(BYTE lockmyself) {m_LockMyself = lockmyself;};

	private:
		BYTE					m_LockMyself;		
	};

	class CGExchangeSynchLockFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGExchangeSynchLock() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_EXCHANGESYNCHLOCK; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE);};
	};

	class CGExchangeSynchLockHandler 
	{
	public:
		static UINT Execute( CGExchangeSynchLock* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
