// GCMissionModify.h
// 
// 更改一个任务参数
// 
//////////////////////////////////////////////////////

#ifndef __GCMISSIONMODIFY_H__
#define __GCMISSIONMODIFY_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"
#include "DB_Struct.h"

namespace Packets
{
	class GCMissionModify : public Packet
	{
	public:
		GCMissionModify( ){
			m_Mission.Cleanup();
		}
		virtual ~GCMissionModify( ){}


		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_GC_MISSIONMODIFY ; }
		virtual UINT			GetPacketSize()const 
		{ 
			if( m_nFlag==MISSIONMODIFY_MISSION )
			{
				return	sizeof(INT) + 
						sizeof(_OWN_MISSION) ;
			}
			else if( m_nFlag==MISSIONMODIFY_MISSIONDATA )
			{
				return	sizeof(INT) + 
						sizeof(m_aMissionData) ;
			}
			else 
			{
				Assert(FALSE) ;
			}
			return 0 ;
		}
	public :
		enum MISSIONMODIFY
		{
			MISSIONMODIFY_MISSION = 0 ,//修改Mission
			MISSIONMODIFY_MISSIONDATA ,//修改MissonData
		};


	public:
		INT GetFlag(){ return m_nFlag ; }
		VOID SetFlag( INT nFlag ){ m_nFlag = nFlag ; } ;

		_OWN_MISSION *GetMission( VOID ){
			return &m_Mission;
		}
		VOID SetMission( const _OWN_MISSION *pMission ){
			m_Mission = *pMission;
		}

		INT* GetMissionData(){ return m_aMissionData ; } ;
		VOID SetMissionData(const INT* pMissionData){
			memcpy((void*)m_aMissionData,(void*)pMissionData,sizeof(m_aMissionData));
		}


	private:
		INT				m_nFlag;//MISSIONMODIFY
		_OWN_MISSION	m_Mission;
		INT				m_aMissionData[MAX_CHAR_MISSION_DATA_NUM] ;

	};

	class GCMissionModifyFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new GCMissionModify() ; }
		PacketID_t	GetPacketID()const { return PACKET_GC_MISSIONMODIFY; }
		UINT		GetPacketMaxSize()const { return	sizeof(INT) + 
														sizeof(_OWN_MISSION) + 
														sizeof(INT)*MAX_CHAR_MISSION_DATA_NUM; }
	};

	class GCMissionModifyHandler 
	{
	public:
		static UINT Execute( GCMissionModify* pPacket, Player* pPlayer ) ;
	};
}

#endif
