#ifndef _GC_BANK_ITEM_INFO_H_
#define _GC_BANK_ITEM_INFO_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"



namespace Packets
{
	class GCBankItemInfo : public Packet
	{

	public:
		GCBankItemInfo(){};
		virtual	~GCBankItemInfo(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_BANKITEMINFO; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(WORD)+GetItemRealSize(m_ITEM)+sizeof(BOOL)	;
		}

	public:

		VOID			setBankIndex(WORD nIndex)		{ m_BankIndex = nIndex;}
		WORD			getBankIndex(VOID)				{ return m_BankIndex;}
		
		_ITEM*			getItem()				{return &m_ITEM;}
		VOID			setItem(const	_ITEM* pItem)	
		{
			Assert(pItem);
			memcpy(&m_ITEM,pItem,sizeof(_ITEM));
		}

		VOID			setIsNull(BOOL	bNull)		{m_nsNull	= bNull;}
		BOOL			getIsNull()	{return m_nsNull;}
	private:
		
		WORD			m_BankIndex;		//item 的BagIndex
		BOOL			m_nsNull;		//物品是否为空		TRUE 代表没有Item,FALSE 代表有Item	
		_ITEM			m_ITEM;

	};


	class GCBankItemInfoFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCBankItemInfo(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_BANKITEMINFO; }
		UINT		GetPacketMaxSize() const { return	sizeof(WORD)+sizeof(_ITEM)+sizeof(BOOL); }
	};


	class GCBankItemInfoHandler
	{
	public:
		static UINT Execute( GCBankItemInfo* pPacket, Player* pPlayer );
	};

}

using namespace Packets;


#endif