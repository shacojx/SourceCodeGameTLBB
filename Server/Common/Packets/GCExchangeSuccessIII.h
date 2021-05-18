// GCExchangeSuccessIII.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __GCEXCHANGESUCCESSIII_H__
#define __GCEXCHANGESUCCESSIII_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "ExchangeMsgDefine.h"

namespace Packets
{
	class GCExchangeSuccessIII : public Packet
	{
	public:
		struct _SUCC_ITEM
		{
			BYTE m_FromType;
			BYTE m_FromIndex;
			BYTE m_ToType;
			BYTE m_ToIndex;
			_SUCC_ITEM()
			{
				m_FromType = 0;
				m_ToType = 0;
				m_FromIndex = 0;
				m_ToIndex = 0;
			}
		};
	public:
		GCExchangeSuccessIII( )
		{
			m_ItemNum = 0;
		};
		virtual ~GCExchangeSuccessIII( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_EXCHANGESUCCESSIII; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE)+sizeof(BYTE)*4*m_ItemNum;}

	public:
		BYTE					GetItemNum(VOID) const {return m_ItemNum;};
		VOID					SetItemNum(INT Num) {m_ItemNum = Num;};

		_SUCC_ITEM*				GetItemList(VOID) {return m_ItemList;};
		VOID					SetItemList(_SUCC_ITEM* ItemList) 
		{
			memcpy(m_ItemList, ItemList, m_ItemNum*sizeof(BYTE)*4);
		}

	private:
		BYTE					m_ItemNum;
		_SUCC_ITEM				m_ItemList[EXCHANGE_BOX_SIZE+EXCHANGE_PET_BOX_SIZE];
	};

	class GCExchangeSuccessIIIFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCExchangeSuccessIII() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_EXCHANGESUCCESSIII; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)+sizeof(BYTE)*4*(EXCHANGE_BOX_SIZE+EXCHANGE_PET_BOX_SIZE);};
	};

	class GCExchangeSuccessIIIHandler 
	{
	public:
		static UINT Execute( GCExchangeSuccessIII* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
