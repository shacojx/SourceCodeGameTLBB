#ifndef _CG_REMOVEGEM_H_
#define _CG_REMOVEGEM_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGRemoveGem:	public Packet
	{
	public:
		CGRemoveGem(){};
		virtual		~CGRemoveGem(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_REMOVEGEM; }
		virtual UINT			GetPacketSize()const { return sizeof(BYTE); }

		VOID					SetEquipBagIndex(BYTE index){m_EquipBagIndex = index;}
		BYTE					GetEquipBagIndex(){return m_EquipBagIndex;}

		VOID					SetGemIndex(BYTE index){m_GemIndex	=	index;}
		BYTE					GetGemIndex(){return m_GemIndex;}

	private:
		BYTE					m_EquipBagIndex;  //装备在包中的位置

		BYTE					m_GemIndex;		  //宝石编号

	};

	class CGRemoveGemFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGRemoveGem() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_CG_REMOVEGEM; }
		UINT		GetPacketMaxSize()const		{ return sizeof(BYTE)*2; }			
	};

	class CGRemoveGemHandler
	{
	public:
		static UINT	Execute(CGRemoveGem* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif