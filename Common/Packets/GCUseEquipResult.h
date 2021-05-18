#ifndef _GC_USEEQUIP_RESULT_H_
#define _GC_USEEQUIP_RESULT_H_


#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"



namespace Packets
{
	class GCUseEquipResult:public Packet
	{

	public:
		GCUseEquipResult(){};
		virtual				~GCUseEquipResult(){};

		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_GC_USEEQUIPRESULT; }

		virtual UINT			GetPacketSize() const 
		{
			return sizeof(BYTE)*3+sizeof(_ITEM_GUID)*2+sizeof(UINT)*2;
		}

	public:
		VOID			setBagIndex(BYTE index){m_BagIndex = index;}
		BYTE			getBagIndex(){return m_BagIndex;}

		VOID			setEquipPoint(BYTE Point){m_EquipPoint = Point;}
		BYTE			getEquipPoint(){return m_EquipPoint;}


		VOID			setBagItemId(_ITEM_GUID itemID){m_ItemID	= itemID;}
		_ITEM_GUID		getItemId(){return m_ItemID;}

		VOID			setEquipID(_ITEM_GUID itemID){m_EquipID = itemID;}
		_ITEM_GUID		getEquipID(){return m_EquipID;}


		VOID			setResult(BYTE Result){m_Result = Result;}
		BYTE			getResult(){return m_Result;}

		VOID			setItemResID(UINT id){m_ItemResId = id;}
		UINT			getItemResID(){return m_ItemResId;}


		VOID			setEquipResID(UINT id ){m_EquipResId = id;}
		UINT			getEquipResID(){return m_EquipResId;}


	private:
		BYTE			m_Result;		//结果信息
		BYTE			m_EquipPoint;	//装配点
		BYTE			m_BagIndex;		//存放的位置

		_ITEM_GUID		m_ItemID;		//
		
		UINT			m_ItemResId;	//BagIndex 对应的资源

		_ITEM_GUID		m_EquipID;

		UINT			m_EquipResId;  //EquipPoint对应的资源

	};


	class GCUseEquipResultFactory:public PacketFactory
	{
	public:
		Packet*		CreatePacket() { return new GCUseEquipResult(); }
		PacketID_t	GetPacketID() const { return PACKET_GC_USEEQUIPRESULT; }
		UINT		GetPacketMaxSize() const { return sizeof(BYTE)*3+sizeof(_ITEM_GUID)*2+sizeof(UINT)*2;}
	};


	class GCUseEquipResultHandler
	{
	public:
		static UINT Execute( GCUseEquipResult* pPacket, Player* pPlayer );
	};



}



#endif