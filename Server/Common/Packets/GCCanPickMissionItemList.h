// GCCanPickMissionItemList.h
// 
// 可以拾取的任务物品列表
// 
//////////////////////////////////////////////////////

#ifndef __GCCANPICKMISSIONITEMLIST_H__
#define __GCCANPICKMISSIONITEMLIST_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class GCCanPickMissionItemList :
		public Packet
	{
	public:
		GCCanPickMissionItemList( ){
		}
		virtual ~GCCanPickMissionItemList( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_CANPICKMISSIONITEMLIST ; }
		virtual UINT			GetPacketSize()const{
			return sizeof( m_yItemCount ) + sizeof( UINT ) * m_yItemCount;
		}

	public:
		VOID			SetCanPickMissionItemList( BYTE yItemCount, const UINT *padwCanPickMissionItemList ) {
			m_yItemCount = yItemCount;
			memcpy( m_aCanPickMissionItemList, padwCanPickMissionItemList, m_yItemCount * sizeof( UINT ) );
		}
		BYTE			GetCanPickMissionItemCount( VOID )const{ return m_yItemCount; }
		const UINT		*GetCanPickMissionItemList( VOID )const{ return m_aCanPickMissionItemList; }

	private:
		BYTE			m_yItemCount;
		UINT			m_aCanPickMissionItemList[MAX_CHAR_CAN_PICK_MISSION_ITEM_NUM];
	};

	class GCCanPickMissionItemListFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCCanPickMissionItemList() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_CANPICKMISSIONITEMLIST; }
		UINT		GetPacketMaxSize()const { return sizeof(BYTE) + sizeof(UINT) * MAX_CHAR_CAN_PICK_MISSION_ITEM_NUM; }
	};

	class GCCanPickMissionItemListHandler 
	{
	public:
		static UINT Execute( GCCanPickMissionItemList* pPacket, Player* pPlayer ) ;
	};
}

#endif // __GCCANPICKMISSIONITEMLIST_H__
