// CGStallBuy.h
// 
// 告诉客户端买卖是否成功
// 
//////////////////////////////////////////////////////

#ifndef __CGSTALLBUY_H__
#define __CGSTALLBUY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"
#include "GCStallError.h"

namespace Packets
{
	class CGStallBuy : public Packet
	{
	public:
		CGStallBuy( )
		{
			m_objID = INVALID_ID;
			m_nSerial = 0;
		};
		virtual ~CGStallBuy( ){};

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_STALLBUY; }
		virtual UINT			GetPacketSize()const { return	sizeof(ObjID_t) + 
																sizeof(_ITEM_GUID) + 
																sizeof(PET_GUID_t) + 
																sizeof(UINT);}

		ObjID_t					GetObjID(VOID) const {return m_objID;};
		VOID					SetObjID(ObjID_t ObjID) {m_objID = ObjID;};

		_ITEM_GUID				GetObjGUID(VOID) const {return m_ItemGuid;}
		VOID					SetObjGUID(_ITEM_GUID Guid) {m_ItemGuid = Guid;}

		PET_GUID_t				GetPetGUID(VOID) const {return m_PetGuid;}
		VOID					SetPetGUID(PET_GUID_t PetGuid) {m_PetGuid = PetGuid;}

		UINT					GetSerial(VOID) const {return m_nSerial;}
		VOID					SetSerial(UINT nSerial) {m_nSerial = nSerial;}

	private:
		ObjID_t					m_objID;		//摊主id
		_ITEM_GUID				m_ItemGuid;
		PET_GUID_t				m_PetGuid;
		UINT					m_nSerial;

	};

	class CGStallBuyFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGStallBuy() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_STALLBUY; };
		UINT		GetPacketMaxSize()const { return	sizeof(ObjID_t) + 
														sizeof(_ITEM_GUID) + 
														sizeof(PET_GUID_t) +
														sizeof(UINT);}
	};

	class CGStallBuyHandler 
	{
	public:
		static UINT Execute( CGStallBuy* pPacket, Player* pPlayer ) ;
	};
}

using namespace Packets;

#endif
