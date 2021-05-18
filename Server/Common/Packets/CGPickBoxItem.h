#ifndef _CG_PICK_BOXITEM_H_
#define _CG_PICK_BOXITEM_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"



namespace Packets
{
	class CGPickBoxItem:public Packet
	{

	public:
		CGPickBoxItem(){};
		virtual				~CGPickBoxItem(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_CG_PICKBOXITEM	; }

		virtual UINT			GetPacketSize() const 
		{
			return	sizeof(ObjID_t)+sizeof(_ITEM_GUID); 
		}

	public:
		VOID			setItemId(_ITEM_GUID itemIndex){m_ItemId = itemIndex;}
		_ITEM_GUID		getItemId()	{return m_ItemId;}

		VOID			setItemBoxId(ObjID_t	id){m_ObjID = id;};
		ObjID_t			getItemBoxId(){return m_ObjID;}
	private:
		ObjID_t				m_ObjID;
		_ITEM_GUID			m_ItemId;
	
	};

	class CGPickBoxItemFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGPickBoxItem(); }
		PacketID_t	GetPacketID() const { return PACKET_CG_PICKBOXITEM; }
		UINT		GetPacketMaxSize() const { return	sizeof(ObjID_t)+sizeof(_ITEM_GUID); }
	};

	class CGPickBoxItemHandler
	{
	public:
		static UINT	Execute( CGPickBoxItem* pPacket, Player* pPlayer );
	};

}


#endif