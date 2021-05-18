

#ifndef __WGRETSCENEDATA_H__
#define __WGRETSCENEDATA_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGRetSceneData : public Packet 
	{
	public:
		WGRetSceneData( ){} ;
		virtual ~WGRetSceneData( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream )const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID()const { return PACKET_WG_RETSCENEDATA ; }
		virtual UINT			GetPacketSize()const { return sizeof(SceneID_t)+
															  sizeof(SCENE_INIT_DATA); }

	public :

	public:
		//使用数据接口
		VOID			SetSceneID(SceneID_t SceneID){ m_SceneID = SceneID; }
		SceneID_t		GetSceneID(VOID) const { return m_SceneID; }
	
		VOID			SetSceneInitData( SCENE_INIT_DATA* pInit )
		{
			m_SceneInitData = *pInit ;
		}
		SCENE_INIT_DATA* GetSceneInitData(){ return &m_SceneInitData; }

	private:
		//数据
		SceneID_t				m_SceneID ;
		SCENE_INIT_DATA			m_SceneInitData ;
	};


	class WGRetSceneDataFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new WGRetSceneData() ; }
		PacketID_t	GetPacketID()const { return PACKET_WG_RETSCENEDATA ; }
		UINT		GetPacketMaxSize()const { return sizeof(SceneID_t)+
													 sizeof(SCENE_INIT_DATA); }
	};


	class WGRetSceneDataHandler 
	{
	public:
		static UINT Execute( WGRetSceneData* pPacket, Player* pPlayer ) ;
	};



};

using namespace Packets ;

#endif
