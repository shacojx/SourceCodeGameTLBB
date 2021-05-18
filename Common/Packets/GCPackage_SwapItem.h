#ifndef _GC_PACKAGE_SWAPITEM_H_
#define _GC_PACKAGE_SWAPITEM_H_

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class GCPackage_SwapItem : public Packet
	{
	public:
		GCPackage_SwapItem(){};
		virtual		~GCPackage_SwapItem(){};

		//公用接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_PACKAGE_SWAPITEM; }
		virtual UINT			GetPacketSize()const { return sizeof(BYTE)*3; }

		VOID					SetResult(BYTE index){m_nResult = index;}
		BYTE					GetResult(){return m_nResult;}

		VOID					SetPackageIndex1(BYTE index){m_nPackageIndex1 = index;}
		BYTE					GetPackageIndex1(){return m_nPackageIndex1;}

		VOID					SetPackageIndex2(BYTE index){m_nPackageIndex2 = index;}
		BYTE					GetPackageIndex2(){return m_nPackageIndex2;}

	private:
		//结果
		// 0 - 成功
		// 1 - 失败
		BYTE					m_nResult;
		//要求交换的两个Item的索引
		BYTE					m_nPackageIndex1;
		BYTE					m_nPackageIndex2;
	};

	class GCPackage_SwapItemFactory: public	PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCPackage_SwapItem() ; }
		PacketID_t	GetPacketID()const			{ return PACKET_GC_PACKAGE_SWAPITEM; }
		UINT		GetPacketMaxSize()const		{ return sizeof(BYTE)*3; }			
	};

	class GCPackage_SwapItemHandler
	{
	public:
		static UINT Execute(GCPackage_SwapItem* pPacket,Player* pPlayer);
	};

}

using namespace Packets;

#endif