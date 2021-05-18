// GCAddCanPickMissionItem.h
// 
// 添加一个可以拾取的任务物品
// 
//////////////////////////////////////////////////////

#ifndef __GCADDCANPICKMISSIONITEM_H__
#define __GCADDCANPICKMISSIONITEM_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"


namespace Packets
{
	class GCAddCanPickMissionItem :
		public Packet
	{
	public:
		GCAddCanPickMissionItem( ){
		}
		virtual ~GCAddCanPickMissionItem( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_ADDCANPICKMISSIONITEM ; }
		virtual UINT			GetPacketSize()const{ return sizeof( UINT ); }

	public:
		VOID			SetItemDataID( UINT uItemDataID ){ m_uItemDataID =uItemDataID; }
		UINT			GetItemDataID( VOID )const{ return m_uItemDataID; }

	private:
		UINT			m_uItemDataID;
	};

	class GCAddCanPickMissionItemFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCAddCanPickMissionItem() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_ADDCANPICKMISSIONITEM; }
		UINT		GetPacketMaxSize()const { return sizeof(UINT); }
	};

	class GCAddCanPickMissionItemHandler 
	{
	public:
		static UINT Execute( GCAddCanPickMissionItem* pPacket, Player* pPlayer ) ;
	};
}

#endif // __GCADDCANPICKMISSIONITEM_H__
