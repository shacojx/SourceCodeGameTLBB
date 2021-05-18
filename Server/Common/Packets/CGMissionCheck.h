// CGMissionCheck.h
// 
// 任务完成验证
// 
//////////////////////////////////////////////////////

#ifndef __CGMISSIONCHECK_H__
#define __CGMISSIONCHECK_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class CGMissionCheck : public Packet
	{
	public:
		enum
		{
			ITEM_MAX_NUM = 3,
		};
	public:
		CGMissionCheck( )
		{
			m_PetIndex = 0;
			m_ItemIndexList[0] = 0;
			m_ItemIndexList[1] = 0;
			m_ItemIndexList[2] = 0;
		}
		virtual ~CGMissionCheck( ){}

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_CG_MISSIONCHECK; }
		virtual UINT			GetPacketSize()const { return sizeof(BYTE)*4 + sizeof(ObjID_t) + sizeof(ScriptID_t); }

	public:
		BYTE					getPetIndex( VOID )const{ return m_PetIndex; }
		VOID					setPetIndex( BYTE PetIndex ) { m_PetIndex = PetIndex; }

		BYTE*					getItemIndexList( VOID ){ return m_ItemIndexList; }
		VOID					setItemIndexList( BYTE* ItemList )
		{
			memcpy(m_ItemIndexList, ItemList, sizeof(BYTE)*ITEM_MAX_NUM); 
		}

		ObjID_t					getNPCID( VOID )const{ return m_idNPC; }
		VOID					setNPCID( ObjID_t idNPC ) { m_idNPC = idNPC; }

		ScriptID_t				getScriptID( VOID )const{ return m_idScript; }
		VOID					setScriptID( ScriptID_t idScript ) { m_idScript = idScript; }


	private:
		BYTE					m_ItemIndexList[ITEM_MAX_NUM];
		BYTE					m_PetIndex;
		ObjID_t					m_idNPC;
		ScriptID_t				m_idScript;		// 任务的脚本ID


	};


	class CGMissionCheckFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new CGMissionCheck() ; }
		PacketID_t	GetPacketID()const { return PACKET_CG_MISSIONCHECK; }
		UINT		GetPacketMaxSize()const { return sizeof(BYTE)*4 + sizeof(ObjID_t) + sizeof(ScriptID_t); }
	};

	class CGMissionCheckHandler 
	{
	public:
		static UINT Execute( CGMissionCheck* pPacket, Player* pPlayer ) ;
	};
}

#endif
