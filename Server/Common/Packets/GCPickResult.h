#ifndef _GC_PICK_RESULT_H_
#define _GC_PICK_RESULT_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"





namespace Packets
{
	class GCPickResult:public Packet
	{

	public:
		GCPickResult(){};
		virtual				~GCPickResult(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_PICKRESULT; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(m_ObjID)+sizeof(BYTE)*2+sizeof(_ITEM_GUID)*2+sizeof(WORD);
		}

	public:
		VOID			setItemID(_ITEM_GUID itemIndex){m_ItemID = itemIndex;}
		_ITEM_GUID		getItemID()	{return m_ItemID;}
		VOID			setItemBoxId(ObjID_t	id){m_ObjID = id;};
		ObjID_t			getItemBoxId(){return m_ObjID;}

		VOID			setResult(BYTE Result){m_Result = Result;}
		BYTE			getResult(){return m_Result;}

		VOID			setBagIndex(BYTE index){m_BagIndex = index;}
		BYTE			getBagIndex(){return m_BagIndex;}

		VOID			setItemTableIndex(WORD tableIndex){m_ItemTableIndex= tableIndex;}
		WORD			getItemTableIndex(){return m_ItemTableIndex;}

		VOID			setBagItemGUID(_ITEM_GUID itemGUID){m_BagItemGUID = itemGUID;}
		_ITEM_GUID		getBagItemGUID()	{return m_BagItemGUID;}

	private:
		ObjID_t			m_ObjID;
		_ITEM_GUID		m_ItemID;
		_ITEM_GUID		m_BagItemGUID;		//存放位置的物品的GUID
		BYTE			m_Result;			//是否成功，不成功包含错误信息
		BYTE			m_BagIndex;			//成功后，存放的位置
		WORD			m_ItemTableIndex;

	};


	class GCPickResultFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCPickResult(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_PICKRESULT; }
		UINT		GetPacketMaxSize() const {
			return sizeof(ObjID_t)+sizeof(BYTE)*2+sizeof(_ITEM_GUID)*2+sizeof(WORD); }
	};


	class GCPickResultHandler
	{
	public:
		static UINT Execute( GCPickResult* pPacket, Player* pPlayer );
	};



}


#endif