#ifndef _GC_DISCARDEQUIP_RESULT_
#define _GC_DISCARDEQUIP_RESULT_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


enum	DISCARDEQUIP_RESULT
{
	DISCARDEQUIP_SUCCESS,
	DISCARDEQUIP_CANNT_DISCARD,
	DISCARDEQUIP_NO_SUCH_EQUIP,
	DISCARDEQUIP_FAIL
};


namespace Packets
{
	class GCDiscardEquipResult:public Packet
	{

	public:
		GCDiscardEquipResult(){};
		virtual				~GCDiscardEquipResult(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_DISCARDEQUIPRESULT; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)*2+sizeof(UINT);
		}

	public:
		
		VOID			SetEquipPoint(BYTE index){m_EquipPoint = index;}
		BYTE			GetEquipPoint(){return m_EquipPoint;}

		VOID			SetResult(BYTE Result){m_Result = Result;}
		BYTE			GetResult(){return m_Result;}

		VOID			SetItemTableIndex(UINT index){m_ItemTableIndex = index;}
		UINT			GetItemTableIndex(){return m_ItemTableIndex;}


	private:
			
		BYTE			m_Result;				//是否成功，不成功包含错误信息
		BYTE			m_EquipPoint;		    //成功后，丢弃物品的EquipPoint
		UINT			m_ItemTableIndex;
	};


	class GCDiscardEquipResultFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCDiscardEquipResult(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_DISCARDEQUIPRESULT; }
		UINT		GetPacketMaxSize() const { return sizeof(BYTE)*2+sizeof(UINT); }
	};


	class GCDiscardEquipResultHandler
	{
	public:
		static UINT Execute( GCDiscardEquipResult* pPacket, Player* pPlayer );
	};



}

using namespace Packets;

#endif