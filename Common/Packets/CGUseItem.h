#ifndef _CG_USEITEM_H_
#define _CG_USEITEM_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class CGUseItem:	public Packet
	{
	public:
		CGUseItem()
		{
			m_BagIndex			= 255;

			m_Target			= INVALID_ID;
			//m_posTarget;
			m_fDir				= -1;
			//m_TargetPetGUID;
			m_TargetItemIndex	= 255;
		}
		virtual		~CGUseItem(){}

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_USEITEM; }
		virtual UINT			GetPacketSize()const { return sizeof(BYTE) + sizeof(ObjID_t) + sizeof(WORLD_POS) + sizeof(FLOAT) + sizeof(PET_GUID_t) + sizeof(BYTE); }

		VOID					SetBagIndex(BYTE index){m_BagIndex = index;}
		BYTE					GetBagIndex(){return m_BagIndex;}

		VOID					SetTargetObjID(ObjID_t id) { m_Target = id; }
		ObjID_t					GetTargetObjID(VOID)const{ return m_Target; }

		VOID					SetTargetPos(const WORLD_POS *pPos) { m_posTarget = *pPos; }
		const WORLD_POS			*GetTargetPos(VOID)const{ return &m_posTarget; }

		VOID					SetTargetDir(FLOAT fDir) { m_fDir = fDir; }
		FLOAT					GetTargetDir(VOID)const{ return m_fDir; }

		VOID					SetTargetPetGUID(PET_GUID_t guidPet) { m_TargetPetGUID = guidPet; }
		PET_GUID_t				GetTargetPetGUID(VOID)const{ return m_TargetPetGUID; }

		VOID					SetTargetItem(BYTE byIndex) { m_TargetItemIndex = byIndex; }
		BYTE					GetTargetItem(VOID)const{ return m_TargetItemIndex; }

	private:
		
		BYTE					m_BagIndex;

		ObjID_t					m_Target;
		WORLD_POS				m_posTarget;
		FLOAT					m_fDir;
		PET_GUID_t				m_TargetPetGUID;
		BYTE					m_TargetItemIndex;
	};

	class CGUseItemFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGUseItem() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_CG_USEITEM; }
		UINT		GetPacketMaxSize()const		{ return sizeof(BYTE) + sizeof(ObjID_t) + sizeof(WORLD_POS) + sizeof(FLOAT) + sizeof(PET_GUID_t) + sizeof(BYTE); }			
	};

	class CGUseItemHandler
	{
	public:
		static UINT	Execute(CGUseItem* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif