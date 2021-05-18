// GCBankAcquireList.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __GCBANKACQUIRELIST_H__
#define __GCBANKACQUIRELIST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCBankAcquireList : public Packet
	{
	public:
		struct _BANK_ITEM
		{
			BYTE			bankindex;
			BYTE			isBlueEquip;
			BYTE			byNumber;
			UINT			item_guid;
			_ITEM			item_data;
			_BANK_ITEM()
			{
				bankindex = 0;
				isBlueEquip = 0;
				byNumber = 0;
				item_guid = 0;
			}
		};
	public:
		GCBankAcquireList( )
		{
			m_ItemNum = 0;
			m_CurBankSize = 0;
			m_Money = 0;
		};
		virtual ~GCBankAcquireList( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_BANKACQUIRELIST; }
		virtual UINT			GetPacketSize()const 
		{
			INT length = 0;
			for(INT i = 0; i<m_ItemNum; i++)
			{
				length += sizeof(BYTE)*3;
				if(m_ItemList[i].isBlueEquip)
				{
					length += sizeof(_ITEM);
				}
				else
				{
					length += sizeof(UINT);
				}
				
			}
			return	sizeof(BYTE)*2 + sizeof(INT)*2 + length;
		}

	public:
		INT						GetBankMoney(VOID) const {return m_Money;};
		VOID					SetBankMoney(INT BankMoney) {m_Money = BankMoney;};

		INT						GetBankRMB(VOID) const {return m_RMB;};
		VOID					SetBankRMB(INT BankMoney) {m_RMB = BankMoney;};

		BYTE					GetBankSize(VOID) const {return m_CurBankSize;};
		VOID					SetBankSize(BYTE Banksize) {m_CurBankSize = Banksize;};

		BYTE					GetItemNum(VOID) const {return m_ItemNum;};
		VOID					SetItemNum(BYTE itnum) {m_ItemNum = itnum;};

		_BANK_ITEM*				GetBankItemList(VOID) {return m_ItemList;};
		VOID					SetBankItemList(_BANK_ITEM* BankItemList) 
		{
			memcpy(m_ItemList, BankItemList, m_ItemNum*sizeof(_BANK_ITEM));
		}

	private:
		_BANK_ITEM				m_ItemList[MAX_BANK_SIZE];
		BYTE					m_ItemNum;
		BYTE					m_CurBankSize;
		INT						m_Money;
		INT						m_RMB;
	};

	class GCBankAcquireListFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCBankAcquireList() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_BANKACQUIRELIST; };
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)*2 + sizeof(INT)*2 + MAX_BANK_SIZE*(sizeof(BYTE)*3+sizeof(UINT)+sizeof(_ITEM));}
	};

	class GCBankAcquireListHandler 
	{
	public:
		static UINT Execute( GCBankAcquireList* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
