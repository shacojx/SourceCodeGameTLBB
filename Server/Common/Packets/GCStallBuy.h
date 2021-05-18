// GCStallBuy.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __GCSTALLBUY_H__
#define __GCSTALLBUY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "GCStallError.h"

namespace Packets
{
	class GCStallBuy : public Packet
	{
	public:
		GCStallBuy( )
		{
			m_objID = INVALID_ID;
			m_nSerial = 0;
			m_ToIndex = 0;
			m_ToType = 0;
		};
		virtual ~GCStallBuy( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_STALLBUY; }
		virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t) + 
																sizeof(_ITEM_GUID) +
																sizeof(PET_GUID_t) +
																sizeof(UINT) +
																sizeof(BYTE)*2;}

		ObjID_t					GetObjID(VOID) const {return m_objID;};
		VOID					SetObjID(ObjID_t ObjID) {m_objID = ObjID;};

		_ITEM_GUID				GetObjGUID(VOID) const {return m_ItemGuid;}
		VOID					SetObjGUID(_ITEM_GUID Guid) {m_ItemGuid = Guid;}

		PET_GUID_t				GetPetGUID(VOID) const {return m_PetGuid;}
		VOID					SetPetGUID(PET_GUID_t PetGuid) {m_PetGuid = PetGuid;}

		UINT					GetSerial(VOID) const {return m_nSerial;}
		VOID					SetSerial(UINT nSerial) {m_nSerial = nSerial;}

		BYTE					GetToIndex(VOID) const {return m_ToIndex;}
		VOID					SetToIndex(BYTE ToIndex) {m_ToIndex = ToIndex;}

		BYTE					GetToType(VOID) const {return m_ToType;}
		VOID					SetToType(BYTE ToType) {m_ToType = ToType;}

	public:
		ObjID_t					m_objID;		//摊主objid
		_ITEM_GUID				m_ItemGuid;
		PET_GUID_t				m_PetGuid;
		UINT					m_nSerial;
		BYTE					m_ToIndex;
		BYTE					m_ToType;

	};

	class GCStallBuyFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCStallBuy() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_STALLBUY; };
		UINT		GetPacketMaxSize()const {	return sizeof(ObjID_t) + 
												sizeof(_ITEM_GUID) +
												sizeof(PET_GUID_t) +
												sizeof(UINT) +
												sizeof(BYTE)*2;}
	};

	class GCStallBuyHandler 
	{
	public:
		static UINT Execute( GCStallBuy* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
