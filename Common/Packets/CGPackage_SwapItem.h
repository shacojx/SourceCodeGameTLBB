#ifndef _CG_PACKAGE_SWAPITEM_H_
#define _CG_PACKAGE_SWAPITEM_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGPackage_SwapItem : public Packet
	{
	public:
		CGPackage_SwapItem(){};
		virtual		~CGPackage_SwapItem(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_PACKAGE_SWAPITEM; }
		virtual UINT			GetPacketSize()const { return sizeof(BYTE)*2; }

		VOID					SetPackageIndex1(BYTE index){m_nPackageIndex1 = index;}
		BYTE					GetPackageIndex1(){return m_nPackageIndex1;}

		VOID					SetPackageIndex2(BYTE index){m_nPackageIndex2 = index;}
		BYTE					GetPackageIndex2(){return m_nPackageIndex2;}

	private:
		//要求交换的两个Item的索引
		BYTE					m_nPackageIndex1;
		BYTE					m_nPackageIndex2;
	};

	class CGPackage_SwapItemFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new CGPackage_SwapItem() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_CG_PACKAGE_SWAPITEM; }
		UINT		GetPacketMaxSize()const		{ return sizeof(BYTE)*2; }			
	};

	class CGPackage_SwapItemHandler
	{
	public:
		static UINT	Execute(CGPackage_SwapItem* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif