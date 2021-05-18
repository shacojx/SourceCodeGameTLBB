#ifndef _GC_BOX_ITEMLIST_H_
#define _GC_BOX_ITEMLIST_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"



namespace Packets
{


	class GCBoxItemList:public Packet
	{
	public:
		GCBoxItemList(){};
		virtual	~GCBoxItemList(){};


		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_BOXITEMLIST; }
		virtual UINT			GetPacketSize() const 
		{ 

			UINT	isize	=	sizeof(ObjID_t)+sizeof(BYTE)+sizeof(WORD);
			for(INT	i = 0;i<m_ItemNumber;i++)
			{
		
				isize+= GetItemRealSize(m_ItemList[i]);

			}
			return isize ;
		}

	public:
		//使用数据接口
		VOID			setItemNum(BYTE num) { m_ItemNumber = num; }
		BYTE			getItemNum(VOID) const { return m_ItemNumber; }

		VOID			setItemList(const _ITEM* pItemList)
		{
			memcpy(m_ItemList,pItemList,sizeof(_ITEM)*MAX_BOXITEM_NUMBER);
		}

		VOID			setItemData(const _ITEM*	pItem,UCHAR uIndex)
		{
			Assert(uIndex<MAX_BOXITEM_NUMBER);
			memcpy(&m_ItemList[uIndex] ,pItem,sizeof(_ITEM));
		}
		const _ITEM*			getItemList(VOID)	const	
		{
			return  m_ItemList;
		}
		
		VOID			setItemBoxId(ObjID_t ObjId){m_ItemBoxID = ObjId;}
		ObjID_t			getItemBoxId(){return m_ItemBoxID;}

		VOID			setItemBoxType(WORD IBType)
		{
			m_ItemBoxType	=	IBType;
		}

		WORD			getItemBoxType(){return m_ItemBoxType;}
	private:
		 WORD       m_ItemBoxType;
		 ObjID_t	m_ItemBoxID;
		 BYTE		m_ItemNumber;
		_ITEM		m_ItemList[MAX_BOXITEM_NUMBER];

	};

	class GCBoxItemListFactory:public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCBoxItemList() ; }
		PacketID_t	GetPacketID() const { return PACKET_GC_BOXITEMLIST; }
		UINT		GetPacketMaxSize() const { return	sizeof(ObjID_t)+sizeof(BYTE)+sizeof(WORD)+
												sizeof(_ITEM)*MAX_BOXITEM_NUMBER; }
	};

	class GCBoxItemListHandler
	{
	public:
		static UINT	Execute( GCBoxItemList* pPacket, Player* pPlayer );
	};



}





#endif