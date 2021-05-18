// GCBankRemoveItem.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __GCBANKREMOVEITEM_H__
#define __GCBANKREMOVEITEM_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCBankRemoveItem : public Packet
	{
	public:
		enum
		{
			EQUIP_POS = 0,
			BAG_POS,
			ERROR_POS,
		};
		enum
		{
			OPERATE_ERROR	=0,		// 错误操作
			OPERATE_MOVE,			// 移动到空格
			OPERATE_SWAP,			// 交换物品
			OPERATE_SPLICE,			// 叠加
			OPERATE_SPLIT,			// 拆分
		};
	public:
		GCBankRemoveItem( )
		{
			m_ToType = BAG_POS;
			m_indexFrom = 0;
			m_indexTo = 0;
			m_OperateType = 0;
		}
		virtual ~GCBankRemoveItem( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_BANKREMOVEITEM; }
		virtual UINT			GetPacketSize()const { return	sizeof(BYTE)*4;}

	public:
		BYTE					GetToType(VOID) const {return m_ToType;}
		VOID					SetToType(BYTE ToType) {m_ToType = ToType;}

		BYTE					GetIndexFrom(VOID) const {return m_indexFrom;}
		VOID					SetIndexFrom(BYTE indexFrom) {m_indexFrom = indexFrom;}

		BYTE					GetIndexTo(VOID) const {return m_indexTo;}
		VOID					SetIndexTo(BYTE indexTo) {m_indexTo = indexTo;}

		BYTE					GetOperateType(VOID) const {return m_OperateType;}
		VOID					SetOperateType(BYTE OperateType) {m_OperateType = OperateType;}
	private:
		BYTE					m_ToType;
		BYTE					m_indexFrom;
		BYTE					m_indexTo;
		BYTE					m_OperateType;
	};

	class GCBankRemoveItemFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCBankRemoveItem() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_BANKREMOVEITEM; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)*4;};
	};

	class GCBankRemoveItemHandler 
	{
	public:
		static UINT Execute( GCBankRemoveItem* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
