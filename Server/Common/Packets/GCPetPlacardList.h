// GCPetPlacardList.h
// 
// 宠物公告板信息
// 
//////////////////////////////////////////////////////

#ifndef __GCPETPLACARDLIST_H__
#define __GCPETPLACARDLIST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "GameStruct_Pet.h"

namespace Packets
{
	class GCPetPlacardList :
		public Packet
	{
	public:
		GCPetPlacardList( )
		{
			m_nCount		= 0;
			//m_aItems;
		}
		virtual ~GCPetPlacardList( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_PETPLACARDLIST; }
		virtual UINT			GetPacketSize()const 
		{
			return sizeof(m_nCount) + m_nCount * sizeof(_PET_PLACARD_ITEM);
		}

	public:
		INT GetItemCount(VOID)const{return m_nCount;}

		const _PET_PLACARD_ITEM	*GetItem(INT nIndex)const
		{
			if(nIndex < MAX_PETPLACARD_LIST_ITEM_NUM)
			{
				return &m_aItems[nIndex];
			}
			else
			{
				return NULL;
			}
		}

		VOID AddItem(const _PET_PLACARD_ITEM *pItem)
		{
			if(pItem != NULL && m_nCount < MAX_PETPLACARD_LIST_ITEM_NUM)
			{
				m_aItems[m_nCount] = *pItem;
				m_nCount++;
			}
		}

	private:
		INT						m_nCount;									// 条数
		_PET_PLACARD_ITEM		m_aItems[MAX_PETPLACARD_LIST_ITEM_NUM];		// 数据
	};

	class GCPetPlacardListFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCPetPlacardList() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_PETPLACARDLIST; }
		UINT		GetPacketMaxSize()const
		{
			return sizeof(GCPetPlacardList) - sizeof(Packet) ;
		}
	};

	class GCPetPlacardListHandler 
	{
	public:
		static UINT Execute( GCPetPlacardList* pPacket, Player* pPlayer ) ;
	};
}

#endif // __GCPETPLACARDLIST_H__
