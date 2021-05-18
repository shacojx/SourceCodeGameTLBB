// GCStallError.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __GCSTALLERROR_H__
#define __GCSTALLERROR_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace STALL_MSG
{
	enum
	{
		POS_ERR	= 0,
		POS_BAG,				//背包
		POS_EQUIP,				//装备
		POS_PET,				//宠物
	};
	enum
	{
		ERR_ERR = 0,
		ERR_ILLEGAL,
		ERR_CLOSE,
		ERR_OWNER_INVALID,
		ERR_NEED_NEW_COPY,
		ERR_NOT_ENOUGH_ROOM,
		ERR_NOT_ENOUGH_ROOM_IN_STALL,
		ERR_NOT_ENOUGH_MONEY,
		ERR_ALREADY_LOCK,
		ERR_NOT_ENOUGH_MONEY_TO_OPEN,
		ERR_PET_LEVEL_TOO_HIGH,
	};
}
namespace Packets
{
	class GCStallError : public Packet
	{
	public:
		GCStallError( )
		{
			m_ID = 0;
		}
		virtual ~GCStallError( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_STALLERROR; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE);}

	public:
		BYTE					GetID(VOID) const {return m_ID;};
		VOID					SetID(BYTE EID) {m_ID = EID;};

	private:
		BYTE					m_ID;		//成功与否
	};

	class GCStallErrorFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCStallError() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_STALLERROR; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE);};
	};

	class GCStallErrorHandler 
	{
	public:
		static UINT Execute( GCStallError* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
