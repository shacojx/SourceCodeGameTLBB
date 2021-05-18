// WGCityCloseSuccess.h
// 
// 请求申请建立新的城市
// 
//////////////////////////////////////////////////////

#ifndef __WGCITYCLOSESUCCESS_H__
#define __WGCITYCLOSESUCCESS_H__

#include "Type.h"
#include "Packet.h"
#include "PacketFactory.h"

namespace Packets
{
	class WGCityCloseSuccess: public Packet 
	{
	public:
		WGCityCloseSuccess( ){} ;
		virtual ~WGCityCloseSuccess( ){} ;

		//公用继承接口
		virtual BOOL			Read( SocketInputStream& iStream ) ;
		virtual BOOL			Write( SocketOutputStream& oStream ) const ;
		virtual UINT			Execute( Player* pPlayer ) ;

		virtual PacketID_t		GetPacketID() const { return PACKET_WG_CITYCLOSESUCCESS; }
		virtual UINT			GetPacketSize() const { return sizeof(GUID_t) + sizeof(SceneID_t); }

	public:
		GUID_t					GetPlayerGuid(){return m_PlayerGuid;}
		VOID					SetPlayerGuid(GUID_t playerguid){ m_PlayerGuid = playerguid;}

		SceneID_t				GetSceneID(){return m_ScenenID;}
		VOID					SetSceneID(SceneID_t nScenenID){ m_ScenenID = nScenenID;}

	private:
		GUID_t					m_PlayerGuid;
		SceneID_t				m_ScenenID;

	};

	class WGCityCloseSuccessFactory : public PacketFactory 
	{
	public:
		Packet*		CreatePacket() { return new WGCityCloseSuccess() ; }
		PacketID_t	GetPacketID() const { return PACKET_WG_CITYCLOSESUCCESS ; }
		UINT		GetPacketMaxSize() const { return sizeof(GUID_t) + sizeof(SceneID_t); }
	};

	class WGCityCloseSuccessHandler 
	{
	public:
		static UINT Execute( WGCityCloseSuccess* pPacket, Player* pPlayer ) ;
	};
};

using namespace Packets ;

#endif
