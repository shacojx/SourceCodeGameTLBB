#ifndef _CG_USEGEM_H_
#define _CG_USEGEM_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGUseGem:	public Packet
	{
	public:
		CGUseGem(){};
		virtual		~CGUseGem(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_USEGEM; }
		virtual UINT			GetPacketSize()const { return sizeof(BYTE)*2+sizeof(ObjID_t); }

		VOID					SetGemBagIndex(BYTE index){m_GemBagIndex = index;}
		BYTE					GetGemBagIndex(){return m_GemBagIndex;}

		VOID					SetEquipBagIndex(BYTE index){m_EquipBagIndex = index;}
		BYTE					GetEquipBagIndex(){return m_EquipBagIndex;}

		VOID					SetPlatformGUID(ObjID_t guid) { m_Platform = guid; }
		ObjID_t					GetPlatformGUID() { return m_Platform; }
	private:

		BYTE					m_GemBagIndex;

		BYTE					m_EquipBagIndex;
		ObjID_t					m_Platform;

	};

	class CGUseGemFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGUseGem() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_CG_USEGEM; }
		UINT		GetPacketMaxSize()const		{ return sizeof(BYTE)*2+sizeof(ObjID_t); }			
	};

	class CGUseGemHandler
	{
	public:
		static UINT	Execute(CGUseGem* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif