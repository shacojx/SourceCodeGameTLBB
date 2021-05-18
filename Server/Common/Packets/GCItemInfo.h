#ifndef _GC_ITEM_INFO_H_
#define _GC_ITEM_INFO_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"



namespace Packets
{
	class GCItemInfo : public Packet
	{

	public:
		GCItemInfo(){};
		virtual	~GCItemInfo(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_ITEMINFO; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(WORD)+GetItemRealSize(m_ITEM)+sizeof(BOOL)	;
		}

	public:

		VOID			setID(WORD id)			{ m_BagIndex = id;}
		WORD			getID(VOID)				{ return m_BagIndex;}
		
		_ITEM*			getItem()				{return &m_ITEM;}
		VOID			setItem(const	_ITEM* pItem)	
		{
			Assert(pItem);
			memcpy(&m_ITEM,pItem,sizeof(_ITEM));
		}

		VOID			setIsNull(BOOL	bNull)		{m_nsNull	= bNull;}
		BOOL			getIsNull()	{return m_nsNull;}
	private:
		
		WORD			m_BagIndex;		//item 的BagIndex
		BOOL			m_nsNull;		//物品是否为空		TRUE 代表没有Item,FALSE 代表有Item	
		_ITEM			m_ITEM;

	};


	class GCItemInfoFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCItemInfo(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_ITEMINFO; }
		UINT		GetPacketMaxSize() const { return	sizeof(WORD)+sizeof(_ITEM)+sizeof(BOOL); }
	};


	class GCItemInfoHandler
	{
	public:
		static UINT Execute( GCItemInfo* pPacket, Player* pPlayer );
	};

}

using namespace Packets;


#endif