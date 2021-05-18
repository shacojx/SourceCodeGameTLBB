// WGCityClose.h
// 
// 请求申请建立新的城市
// 
//////////////////////////////////////////////////////

#ifndef __WGCITYCLOSE_H__
#define __WGCITYCLOSE_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGCityClose : public Packet 
	{
	public:
		WGCityClose( ){} ;
		virtual ~WGCityClose( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_WG_CITYCLOSE; }
		virtual UINT			GetPacketSize() const { return sizeof(SceneID_t) + sizeof(PlayerID_t); }

	public:
		SceneID_t				GetSceneID(){ return m_SceneID;}
		VOID					SetSceneID(SceneID_t sceneid){ m_SceneID = sceneid;}

		VOID					SetPlayerID( PlayerID_t pid ) { m_PlayerID = pid; }
		PlayerID_t				GetPlayerID( ) { return m_PlayerID; }
	private:
		SceneID_t				m_SceneID;
		PlayerID_t				m_PlayerID; 

	};

	class WGCityCloseFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new WGCityClose() ; }
		PacketID_t	GetPacketID() const { return PACKET_WG_CITYCLOSE; }
		UINT		GetPacketMaxSize() const { return sizeof(SceneID_t) + sizeof(PlayerID_t); }
	};

	class WGCityCloseHandler 
	{
	public:
		static UINT Execute( WGCityClose* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets ;

#endif
