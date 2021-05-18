// GCPlayerShopRecordList.h
// 
// 向服务器申请创建玩家商店
// 
//////////////////////////////////////////////////////

#ifndef __GCPLAYERSHOPRECORDLIST_H__
#define __GCPLAYERSHOPRECORDLIST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GCPlayerShopError.h"

namespace Packets
{
	class GCPlayerShopRecordList : public Packet
	{
	public:
		struct	MessageEntry_t
		{
			CHAR	szContent[MAX_RECORD_LEN_ENTRY];
			MessageEntry_t()
			{
				memset(szContent, 0, MAX_RECORD_LEN_ENTRY);
			}
		};
	public:
		GCPlayerShopRecordList( )
		{
			m_ShopID.Reset();
			m_EntryNum = 0;
		}
		virtual ~GCPlayerShopRecordList( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_PLAYERSHOPRECORDLIST; }
		virtual UINT			GetPacketSize()const { return	sizeof(_PLAYERSHOP_GUID)+sizeof(BYTE)+sizeof(MessageEntry_t)*m_EntryNum + sizeof(INT);}

		_PLAYERSHOP_GUID		GetShopID(VOID) const {return m_ShopID;}
		VOID					SetShopID(_PLAYERSHOP_GUID nShopID) {m_ShopID = nShopID;}

		BYTE					GetEntryNum(){ return m_EntryNum;}
		VOID					SetEntryNum(BYTE bNum){ m_EntryNum = bNum;}

		MessageEntry_t*			GetEntryList(){return m_EntryList;}
		VOID					SetEntryList(MessageEntry_t* EntryList)
		{
			memcpy(m_EntryList, EntryList, m_EntryNum*sizeof(MessageEntry_t));
		}

		INT						GetPage(){return m_nPage;}
		VOID					SetPage(INT nPage){m_nPage = nPage;}

	private:
		_PLAYERSHOP_GUID		m_ShopID;			//商店ID
		BYTE					m_EntryNum;
		MessageEntry_t			m_EntryList[MAX_RECORD_NUM_PER_PAGE];
		INT						m_nPage;
	};

	class GCPlayerShopRecordListFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCPlayerShopRecordList() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_PLAYERSHOPRECORDLIST; };
		UINT		GetPacketMaxSize()const { return sizeof(_PLAYERSHOP_GUID)+sizeof(BYTE)+sizeof(GCPlayerShopRecordList::MessageEntry_t)*MAX_RECORD_NUM_PER_PAGE + sizeof(INT);};
	};

	class GCPlayerShopRecordListHandler 
	{
	public:
		static UINT Execute( GCPlayerShopRecordList* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
